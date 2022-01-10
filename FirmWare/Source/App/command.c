    
//*************************************************************************************************
//
// Обработка консольных команд
//
//*************************************************************************************************

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "rl_fs.h"
#include "cmsis_os2.h"
#include "rtx_config.h"

#include "lpc_eeprom.h"
#include "lpc_clkpwr.h"
#include "driver_nand.h"

#include "device.h"
#include "dev_param.h"

#include "rtc.h"
#include "message.h"
#include "sdcard.h"
#include "ring_uart.h"
#include "scheduler.h"
#include "eeprom.h"
#include "uart.h"
#include "can.h"
#include "can_data.h"
#include "net.h"
#include "system.h"
#include "ring_uart.h"
#include "ram_disk.h"
#include "events.h"
#include "command.h"
#include "parse.h"
#include "ext_ram_addr.h"
#include "screen.h"
#include "events.h"

//*************************************************************************************************
// Внешние переменные
//*************************************************************************************************
extern CONFIG_HMI cfg_hmi;
extern CONFIG_LCD cfg_lcd;

//*************************************************************************************************
// Переменные с внешним доступом
//*************************************************************************************************
osMutexId_t command_mutex = NULL;
osMessageQueueId_t command_queue = NULL;

//*************************************************************************************************
// Локальные константы
//*************************************************************************************************
#define EXEC_JOBS_ENABLE        true        //разрешение выполнения команды из планировщика

//*************************************************************************************************
// Атрибуты объектов RTOS
//*************************************************************************************************
static const osThreadAttr_t task_attr = {
    .name = "Command",
    .stack_size = 1408,
    .priority = osPriorityNormal
 };

static const osMessageQueueAttr_t mess_attr = { .name = "Command" };
static const osMutexAttr_t mutx_attr = { .name = "Command", .attr_bits = osMutexRobust };

typedef struct {               
    char     name_cmd[20];              //имя команды
    uint32_t (*func)( char *buff );     //указатель на функцию
    bool     exec_job;                  //разрешено выполнять из планировщика
 } COMMAND;

//*********************************************************************************************
// Локальные переменные
//*********************************************************************************************
static char *ptr_buff = (char *)UARTBUF_ADDR;

//*********************************************************************************************
// Прототипы локальных функций
//*********************************************************************************************
static uint32_t CmdHelp( char *buff );
static uint32_t CmdTime( char *buff );
static uint32_t CmdDate( char *buff );
static uint32_t CmdDateTime( char *buff );
static uint32_t CmdRTCSync( char *buff );
static uint32_t CmdReset( char *buff );
static uint32_t CmdScreen( char *buff );
static uint32_t CmdEeprom( char *buff );
static uint32_t CmdConfig( char *buff );
static uint32_t CmdSystem( char *buff );
static uint32_t CmdJobs( char *buff );
static uint32_t CmdConfNet( char *buff );
static uint32_t CmdConfSave( char *buff );
static uint32_t CmdPing( char *buff );
static uint32_t CmdTask( char *buff );
static uint32_t CmdSDCard( char *buff );
static uint32_t CmdRamDir( char *buff );
static uint32_t CmdCTime( char *buff );
static uint32_t CmdTypeFile( char *buff );
static uint32_t CmdReserv( char *buff );
static uint32_t CmdExtOut( char *buff );
static uint32_t CmdIntStat( char *buff );
static uint32_t CmdRTCSync( char *buff );

static uint32_t CmdCharge( char *buff );
static uint32_t CmdInvCtrl( char *buff );
static uint32_t CmdAlt( char *buff );
static uint32_t CmdPvMode( char *buff );
static uint32_t CmdTrcMode( char *buff );
static uint32_t CmdReserv( char *buff );
static uint32_t CmdExtOut( char *buff );
static uint32_t CmdGen( char *buff );
static uint32_t CmdVoice( char *buff );
static uint32_t CmdVolume( char *buff );
static uint32_t CmdSound( char *buff );

uint32_t ExecCommand( SourceCmd src_call, char *cmnd, char *buff );

//*********************************************************************************************
// Локальные переменные
//*********************************************************************************************
static const COMMAND cmd[] = {
    "time",     CmdTime,        0,
    "date",     CmdDate,        0,
    "rtc",      CmdRTCSync,     0,
    "eeprom",   CmdEeprom,      0,
    "config",   CmdConfig,      0,
    "system",   CmdSystem,      0,
    "task",     CmdTask,        0,
    "ctime",    CmdCTime,       0,
    "dtime",    CmdDateTime,    0,
    "link",     CmdIntStat,     0,
    "ramdir",   CmdRamDir,      0,
    "lan",      CmdConfNet,     0,
    "jobs",     CmdJobs,        0,
    "save",     CmdConfSave,    0,
    "ping",     CmdPing,        0,
    "sdcard",   CmdSDCard,      0,
    "type",     CmdTypeFile,    0,
    "inv",      CmdInvCtrl,     EXEC_JOBS_ENABLE,
    "alt",      CmdAlt,         EXEC_JOBS_ENABLE,
    "pv",       CmdPvMode,      EXEC_JOBS_ENABLE,
    "charge",   CmdCharge,      EXEC_JOBS_ENABLE,
    "trc",      CmdTrcMode,     EXEC_JOBS_ENABLE,
    "gen",      CmdGen,         EXEC_JOBS_ENABLE,
    "voice",    CmdVoice,       EXEC_JOBS_ENABLE,
    "volume",   CmdVolume,      EXEC_JOBS_ENABLE,
    "sound",    CmdSound,       EXEC_JOBS_ENABLE,
    "res",      CmdReserv,      EXEC_JOBS_ENABLE,
    "ext",      CmdExtOut,      EXEC_JOBS_ENABLE,
    "reset",    CmdReset,       0,
    "scr",      CmdScreen,      0,
    "?",        CmdHelp,        0
 };

//*************************************************************************************************
// Инициализация командного интерфейса
//*************************************************************************************************
void CommandInit( void ) {

    command_mutex = osMutexNew( &mutx_attr );
    command_queue = osMessageQueueNew( 16, sizeof( QUEUE_CMD ), &mess_attr );
    osThreadNew( TaskCommand, NULL, &task_attr );
 }

//*************************************************************************************************
// Задача обработки принятых команд по UART
//*************************************************************************************************
void TaskCommand( void *pvParameters ) {

    QUEUE_CMD msg;
    osStatus_t status;
  
    for ( ;; ) {
        //ожидание сообщения
        status = osMessageQueueGet( command_queue, &msg, NULL, osWaitForever );
        if ( status == osOK ) {
            //обработка сообщения
            if ( msg.id_msg == MSG_COMMAND_UART ) {
                //ждем пока освободится блокировка
                osMutexAcquire( command_mutex, osWaitForever );
                //захват семафора
                osMutexAcquire( command_mutex, 0 );
                ExecCommand( SOURCE_CMD_UART, msg.ptr_cmd, ptr_buff );
                UARTRecvClear(); //чистим приемный буфер UART
                UARTSendStr( ptr_buff );
                //освободим блокировку
                osMutexRelease( command_mutex );
               }
           }
        #if ( OS_ROBIN_ENABLE == 0 )
        osThreadYield();
        #endif
       }
 }
 
//*********************************************************************************************
// Выполнение команды из TCP
// const char *cmd  - указатель на команду
// char *buf        - указатель на буфер для размещения результата
// uint32_t buf_len - размер буфера для размещения результата
// uint32_t *pvar   - указатель на переменную, в которой будет храниться кол-во уже 
//                    отправленных байт
// return           - кол-во байт для отправки, 31 бит = 1 - признак наличия не отправленных данных
//*********************************************************************************************
uint32_t ExecTelnet( SourceCmd src_call, const char *cmd, char *buf, uint32_t buf_len, uint32_t *pvar ) {

    uint32_t len;
    
    if ( !*pvar ) {
        //ждем пока освободится блокировка
        osMutexAcquire( command_mutex, osWaitForever );
        //захват семафора
        osMutexAcquire( command_mutex, 0 );
        ExecCommand( src_call, (char *)cmd, ptr_buff ); //первичное выполнение команды
       }
    //повторный вызов команды без исполнения, только возврат результата
    len = strlen( ptr_buff + *pvar );
    if ( len <= buf_len ) {
        //результат весь помещается в один буфер
        memcpy( buf, ptr_buff + *pvar, len );
        //освободим блокировку
        osMutexRelease( command_mutex );
        return len;
       }
    else {
        //результат весь не помещается, отправляем несколько блоков
        memcpy( buf, ptr_buff + *pvar, buf_len );
        *pvar += buf_len; //во внешней переменной сохраним смещение для следующего блока
        return buf_len | ( 1U << 31 ); //признак повторного вызова, только для TELNET
       }
 }

//*********************************************************************************************
// Обработка принятой команды 
// SourceCmd src_call - источник запроса
// char *cmnd         - указатель на буфер содержащий команду
// char *buff         - указатель на буфер содержащий результат команды
// return             - кол-во байт ответа в char *buff
//*********************************************************************************************
uint32_t ExecCommand( SourceCmd src_call, char *cmnd, char *buff ) {

    char *ptr;
    uint8_t i;
    uint32_t len;
    
    if ( buff == NULL )
        return 0;
    if ( !strlen( cmnd ) ) {
        sprintf( buff, "%s", Message( CONS_MSG_PROMPT ) );
        return strlen( buff );
       }
    //разбор параметров команды
    ParseCommand( cmnd );
    //чистим буфер для размещения результата
    memset( buff, 0x00, UARTBUF_SIZE );
    for ( i = 0; i < SIZE_ARRAY( cmd ); i++ ) {
        //проверка и выполнение команды
        if ( strcasecmp( (const char *)&cmd[i].name_cmd, GetParamVal( IND_PAR_CMND ) ) )
            continue;
        if ( src_call == SOURCE_CMD_JOB ) {
            //для команды из планировщика заданий проверяем разрешение исполнения
            ptr = buff;
            if ( cmd[i].exec_job == EXEC_JOBS_ENABLE ) {
                //выполнение команды
                cmd[i].func( ptr ); //выполнение функции (команды)
                break;
               }
            else {
                sprintf( buff, "%s", Message( CONS_MSG_ERR_NOJOB ) );
                break;
               }
           }
        else {
            //для всех остальных источников
            ptr = buff;
            ptr += sprintf( ptr, "\r\n" ); //начало вывода с новой строки
            len = cmd[i].func( ptr );      //выполнение функции (команды)
            if ( src_call == SOURCE_CMD_UART )
                sprintf( buff + len + 1, "%s", Message( CONS_MSG_PROMPT ) );
            break;
           }
       }
    if ( i == SIZE_ARRAY( cmd ) ) {
        //команда не указана
        if ( src_call == SOURCE_CMD_UART )
            sprintf( buff, "\r\n%s%s", Message( CONS_MSG_ERR_CMND ), Message( CONS_MSG_PROMPT ) );
        else sprintf( buff, "\r\n%s", Message( CONS_MSG_ERR_CMND ) );
        return strlen( buff );
       }
    return strlen( buff );
 }

//*********************************************************************************************
// Обработка команды из планировщика
// char *cmnd - указатель на буфер содержащий команду
// return     - кол-во байт ответа в char *buff
//*********************************************************************************************
char *ExecuteJob( char *cmnd ) {

    if ( cmnd == NULL )
        return NULL;
    //ждем пока освободится блокировка
    osMutexAcquire( command_mutex, osWaitForever );
    //захват семафора
    osMutexAcquire( command_mutex, 0 );
    ExecCommand( SOURCE_CMD_JOB, cmnd, ptr_buff );
    //освободим блокировку
    osMutexRelease( command_mutex );
    return ptr_buff;
 }

//*********************************************************************************************
// Вывод перечня доступных команд
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdHelp( char *buff ) {
    
    if ( buff == NULL )
        return 0;
    return sprintf( buff, "%s", MessageHelp() );
 }
 
//*********************************************************************************************
// Вывод/установка времени
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdTime( char *buff ) {

    if ( buff == NULL )
        return 0;
    if ( GetParamCnt() == 2 ) {
        if ( RTCSetTime( GetParamVal( IND_PARAM1 ), SET_MODE_REMOTE ) != SUCCESS )
            return sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
       }
    return sprintf( buff, "%s", RTCGetTime( "\r\n" ) );
 }
 
//*********************************************************************************************
// Вывод/установка даты
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdDate( char *buff ) {

    if ( buff == NULL )
        return 0;
    if ( GetParamCnt() == 2 ) {
        if ( RTCSetDate( GetParamVal( IND_PARAM1 ), SET_MODE_REMOTE ) != SUCCESS )
            return sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM )  );
       }
    return sprintf( buff, "%s", RTCGetDate( "\r\n" ) );
 }

//*********************************************************************************************
// Вывод даты/времени в полном формате
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdDateTime( char *buff ) {

    return sprintf( buff, "%s", RTCGetDateTime( "\r\n", DOW_ENG_LANG ) );
 }

//*********************************************************************************************
// Обновить значение часов
// "RTC HMI/CTRL - синхронизация часов/календаря HMI->CTRL CTRL->HMI
// char *buff    - указатель на буфер для размещения результата
// return        - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdRTCSync( char *buff ) {

    RTC rtc_hmi;
    char new_val[16];
    
    if ( buff == NULL )
        return 0;
    //проверка связи с управляющим контроллером
    if ( LinkStat() != CAN_LINK_OK )
        return sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
    if ( GetParamCnt() == 2 && !strcasecmp( GetParamVal( IND_PARAM1 ), "hmi" ) ) {
        //синхронизировать часы "HMI" -> "контроллер"
        RTCHmi( &rtc_hmi ); //дата/время локальных часов
        SendCommand( ID_DEV_RTC, (uint8_t *)&rtc_hmi, sizeof( RTC ) );
        return sprintf( buff, "%s", Message( CONS_MSG_OK ) );
       }
    if ( GetParamCnt() == 2 && !strcasecmp( GetParamVal( IND_PARAM1 ), "ctrl" ) ) {
        //синхронизировать часы "контроллер" -> "HMI"
        sprintf( new_val, "%02d:%02d:%02d", rtc.hour, rtc.min, rtc.sec );
        RTCSetTime( new_val, SET_MODE_LOCAL );
        sprintf( new_val, "%02d.%02d.%04d", rtc.day, rtc.month, rtc.year );
        RTCSetDate( new_val, SET_MODE_LOCAL );
        return sprintf( buff, "%s", Message( CONS_MSG_OK ) );
       }
    return sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM )  );
 }
 
//*********************************************************************************************
// Вывод списка задач и статистики
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdTask( char *buff ) {

    if ( buff == NULL )
        return 0;
    return ListTask( buff );
 }

//*********************************************************************************************
// Операции с SD картой
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdSDCard( char *buff ) {

    if ( buff == NULL )
        return 0;
    if ( GetParamCnt() == 1 ) 
        return SDCid( buff );
    if ( GetParamCnt() == 2 && !strcasecmp( GetParamVal( IND_PARAM1 ), "mount" ) ) 
        return SDMount( buff );
    if ( GetParamCnt() == 2 && !strcasecmp( GetParamVal( IND_PARAM1 ), "unmount" ) ) 
        return SDUnMount( buff );
    if ( GetParamCnt() == 2 && !strcasecmp( GetParamVal( IND_PARAM1 ), "dir" ) ) 
        return SDDir( buff, GetParamVal( IND_PARAM2 ) );
    if ( GetParamCnt() == 2 && !strcasecmp( GetParamVal( IND_PARAM1 ), "format" ) ) 
        return SDFormat( buff );
    return sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM )  );
 }
 
//*********************************************************************************************
// Управление заданиями
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdJobs( char *buff ) {

    LoadJobs( JOBS_LOAD_FORC );
    return 0;
 }

//*********************************************************************************************
// Время управляющего контроллера
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdCTime( char *buff ) {

    if ( buff == NULL )
        return 0;
    return sprintf( buff, "%02d.%02d.%04d %02d:%02d:%02d\r\n", rtc.day, rtc.month, rtc.year, rtc.hour, rtc.min, rtc.sec );
 }

//*********************************************************************************************
// Перезапуск контроллера
//*********************************************************************************************
static uint32_t CmdReset( char *buff ) {

    NVIC_SystemReset();
    return 0;
 }

//*********************************************************************************************
// Вывод кол-во ошибок обмена по CAN шине
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdIntStat( char *buff ) {

    char *ptr, *param_link[] = { "NO", "OK" };

    if ( buff == NULL )
        return 0;
    ptr = buff;
    ptr += sprintf( ptr, "CAN interface state:\r\n" );
    ptr += sprintf( ptr, "          Link: %s\r\n", param_link[LinkGetValue( HMI_LINK ).uint8] );
    ptr += sprintf( ptr, "    Total send: %u\r\n", LinkGetValue( HMI_SEND ).uint32 );
    ptr += sprintf( ptr, "    Error send: %u\r\n", LinkGetValue( HMI_ERROR_SEND ).uint32 );
    ptr += sprintf( ptr, "Error transmit: %u\r\n", LinkGetValue( HMI_ERROR_TX ).uint8 );
    ptr += sprintf( ptr, " Error receive: %u\r\n", LinkGetValue( HMI_ERROR_RX ).uint8 );
    return strlen( buff );                                 
 }

//*************************************************************************************************
// Управление инверторами
// uint8_t cnt_par - кол-во параметров, включая команду
// CmdId id_cmd    - идентификатор команды
// Source src      - режим вывода информации в консоль
//*************************************************************************************************
static uint32_t CmdInvCtrl( char *buff ) {

    Device inv;
    uint8_t ts, cmd;

    if ( buff == NULL )
        return 0;
    if ( GetParamCnt() != 3 ) {
        sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    //номер инвертора
    ts = atoi( GetParamVal( IND_PARAM1 ) );
    if ( ts == 1 )
        inv = ID_DEV_INV1;
    if ( ts == 2 )
        inv = ID_DEV_INV2;
    if ( ( strcasecmp( GetParamVal( IND_PARAM2 ), "on" ) && strcasecmp( GetParamVal( IND_PARAM2 ), "off" ) ) || ( ts != 1 && ts != 2 ) ) {
        sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    if ( !strcasecmp( GetParamVal( IND_PARAM2 ), "on" ) ) {
        cmd = INV_CTRL_ON;
        if ( SendCommand( inv, &cmd, sizeof( cmd ) ) == ERROR ) {
            sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
            return strlen( buff );
           }
       }
    if ( !strcasecmp( GetParamVal( IND_PARAM2 ), "off" ) ) {
        cmd = INV_CTRL_OFF;
        if ( SendCommand( inv, &cmd, sizeof( cmd ) ) == ERROR ) {
            sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
            return strlen( buff );
           }
       }
    sprintf( buff, "%s", Message( CONS_MSG_OK ) );
    return strlen( buff );
 }

//*************************************************************************************************
// Перевод нагрузки на основную сеть (генератор) или инверторы
// uint8_t cnt_par - кол-во параметров, включая команду
// CmdId id_cmd    - идентификатор команды
// Source src      - режим вывода информации в консоль
//*************************************************************************************************
static uint32_t CmdAlt( char *buff ) {

    uint8_t cmd;
    int ac, dc;

    if ( GetParamCnt() != 2 ) {
        sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    ac = strcasecmp( GetParamVal( IND_PARAM1 ), "ac" );
    dc = strcasecmp( GetParamVal( IND_PARAM1 ), "dc" );
    if ( ac && dc ) {
        sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    if ( !ac ) {
        cmd = CAN_ALT_AC;
        if ( SendCommand( ID_DEV_ALT, &cmd, sizeof( cmd ) ) == ERROR ) {
            sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
            return strlen( buff );
           }
       }
    if ( !dc ) {
        cmd = CAN_ALT_DC;
        if ( SendCommand( ID_DEV_ALT, &cmd, sizeof( cmd ) ) == ERROR ) {
            sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
            return strlen( buff );
           }
       }
    sprintf( buff, "%s", Message( CONS_MSG_OK ) );
    return strlen( buff );
 }

//*************************************************************************************************
// Управление вкл/выкл и режимом солнечных панелей
// uint8_t cnt_par - кол-во параметров, включая команду
// CmdId id_cmd    - идентификатор команды
// Source src      - режим вывода информации в консоль
//*************************************************************************************************
static uint32_t CmdPvMode( char *buff ) {

    int on, off, prl, ser;
    CAN_PV can_pv;

    memset( (uint8_t *)&can_pv, 0x00, sizeof( can_pv ) );
    if ( GetParamCnt() != 2 ) {
        sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    //выполняем команды
    on = strcasecmp( GetParamVal( IND_PARAM1 ), "on" );
    off = strcasecmp( GetParamVal( IND_PARAM1 ), "off" );
    prl = strcasecmp( GetParamVal( IND_PARAM1 ), "par" );
    ser = strcasecmp( GetParamVal( IND_PARAM1 ), "ser" );
    if ( on && off && prl && ser ) {
        sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       } 
    if ( !on ) {
        can_pv.mask = CAN_PV_CTRL;
        can_pv.ctrl = PV_CTRL_ON;
        if ( SendCommand( ID_DEV_PV, (uint8_t *)&can_pv, sizeof( can_pv ) ) == ERROR ) {
            sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
            return strlen( buff );
           }
       }
    if ( !off ) {
        can_pv.mask = CAN_PV_CTRL;
        can_pv.ctrl = PV_CTRL_OFF;
        if ( SendCommand( ID_DEV_PV, (uint8_t *)&can_pv, sizeof( can_pv ) ) == ERROR ) {
            sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
            return strlen( buff );
           }
       }
    if ( !prl ) {
        can_pv.mask = CAN_PV_MODE;
        can_pv.mode = PV_MODE_PAR;
        if ( SendCommand( ID_DEV_PV, (uint8_t *)&can_pv, sizeof( can_pv ) ) == ERROR ) {
            sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
            return strlen( buff );
           }
       }
    if ( !ser ) {
        can_pv.mask = CAN_PV_MODE;
        can_pv.mode = PV_MODE_SER;
        if ( SendCommand( ID_DEV_PV, (uint8_t *)&can_pv, sizeof( can_pv ) ) == ERROR ) {
            sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
            return strlen( buff );
           }
       }
    sprintf( buff, "%s", Message( CONS_MSG_OK ) );
    return strlen( buff );
 }
 
//*************************************************************************************************
// Управление/состояние зарядкой АКБ от сети AC
// uint8_t cnt_par - кол-во параметров, включая команду
// CmdId id_cmd    - идентификатор команды
// Source src      - режим вывода информации в консоль
//*************************************************************************************************
static uint32_t CmdCharge( char *buff ) {

    ChargeMode chrgmode;
    uint8_t mode, last_chrg; 
    
    if ( GetParamCnt() == 2 || GetParamCnt() == 3 ) {
        //режим заряда 0/2/3/8
        mode = atoi( GetParamVal( IND_PARAM1 ) );
        if ( mode != 0 && mode != 2 && mode != 3 && mode != 8 ) {
            sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
            return strlen( buff );
           } 
        //запуск заряда если кол-во дней от полного заряда > указанного в параметре (>=10)
        //значение "дней от полного заряда" берем из BATMON
        last_chrg = atoi( GetParamVal( IND_PARAM2 ) );
        if ( last_chrg && batmon.link ) {
            if ( mode && batmon.h9 < last_chrg ) {
                sprintf( buff, Message( CONS_MSG_CHARGE_DAY ), batmon.h9 );
                return strlen( buff );
               }
           }
        if ( mode == 0 )
            chrgmode = CHARGE_OFF;
        if ( mode == 2 )
            chrgmode = CHARGE_MODE2;
        if ( mode == 3 )
            chrgmode = CHARGE_MODE3;
        if ( mode == 8 )
            chrgmode = CHARGE_MODE8;
        if ( SendCommand( ID_DEV_CHARGER, (uint8_t *)&chrgmode, sizeof( chrgmode ) ) == ERROR ) {
            sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
            return strlen( buff );
           }
        sprintf( buff, "%s", Message( CONS_MSG_OK ) );
        return strlen( buff );
       } 
    sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
    return strlen( buff );
 }
 
//*************************************************************************************************
// Управление вкл/выкл питания реле актуаторов трекера и управление дополнительными выходами
// uint8_t cnt_par - кол-во параметров, включая команду
// CmdId id_cmd    - идентификатор команды
// Source src      - режим вывода информации в консоль
//*************************************************************************************************
static uint32_t CmdTrcMode( char *buff ) {

    CAN_TRC trc;
    
    memset( (uint8_t *)&trc, 0x00, sizeof( trc ) );
    if ( GetParamCnt() != 2 && GetParamCnt() != 3 ) {
        sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    if ( GetParamCnt() == 2 ) {
        //выполняем команды вкл/выкл реле питания актуаторов
        if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "on" ) ) {
            trc.ctrl = CAN_TRC_POWER;
            trc.power_act = TRC_ACT_ON;
            if ( SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) ) == ERROR ) {
                sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
                return strlen( buff );
               }
            sprintf( buff, "%s", Message( CONS_MSG_OK ) );
            return strlen( buff );
           }
        if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "off" ) ) {
            trc.ctrl = CAN_TRC_POWER;
            trc.power_act = TRC_ACT_OFF;
            if ( SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) ) == ERROR ) {
                sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
                return strlen( buff );
               }
            sprintf( buff, "%s", Message( CONS_MSG_OK ) );
            return strlen( buff );
           }
        if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "stop" ) ) {
            //прервать позиционирование
            trc.ctrl = CAN_TRC_STOP;
            if ( SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) ) == ERROR ) {
                sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
                return strlen( buff );
               }
            sprintf( buff, "%s", Message( CONS_MSG_OK ) );
            return strlen( buff );
           }
        if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "cmd" ) ) {
            //переход в режим позиционирования только по командам
            trc.ctrl = CAN_TRC_CMD;
            if ( SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) ) == ERROR ) {
                sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
                return strlen( buff );
               }
            sprintf( buff, "%s", Message( CONS_MSG_OK ) );
            return strlen( buff );
           }
        if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "int" ) ) {
            //переход в режим позиционирования по сенсору
            trc.ctrl = CAN_TRC_INT;
            if ( SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) ) == ERROR ) {
                sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
                return strlen( buff );
               }
            sprintf( buff, "%s", Message( CONS_MSG_OK ) );
            return strlen( buff );
           }
        if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "init" ) ) {
            //позиционирование актуаторов в начальное положения (закрыты)
            trc.ctrl = CAN_TRC_INIT;
            if ( SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) ) == ERROR ) {
                sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
                return strlen( buff );
               }
            sprintf( buff, "%s", Message( CONS_MSG_OK ) );
            return strlen( buff );
           }
        if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "save" ) ) {
            //сохранить текущие значения позиционирования в EEPROM контроллера трекера
            trc.ctrl = CAN_TRC_SAVE;
            if ( SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) ) == ERROR ) {
                sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
                return strlen( buff );
               }
            sprintf( buff, "%s", Message( CONS_MSG_OK ) );
            return strlen( buff );
           }
        if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "rest" ) ) {
            //восстановить значения позиционирования из EEPROM контроллера трекера
            trc.ctrl = CAN_TRC_REST;
            if ( SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) ) == ERROR ) {
                sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
                return strlen( buff );
               }
            sprintf( buff, "%s", Message( CONS_MSG_OK ) );
            return strlen( buff );
           }
        if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "reset" ) ) {
            //перезапуск контроллера трекера
            trc.ctrl = CAN_TRC_RESET;
            if ( SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) ) == ERROR ) {
                sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
                return strlen( buff );
               }
            sprintf( buff, "%s", Message( CONS_MSG_OK ) );
            return strlen( buff );
           }
        sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       } 
    if ( GetParamCnt() == 3 ) {
        //позиционирование по параметрам
        if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "ver" ) && strlen( GetParamVal( IND_PARAM2 ) ) ) {
            trc.ctrl = CAN_TRC_POS;
            trc.type_act = TRC_POS_VERTICAL;
            //определим тип значения параметра позиционирования
            if ( strchr( GetParamVal( IND_PARAM2 ), '.' ) == NULL )
                trc.type_value = TRC_POS_LENGTH;  //значение в мм
            else trc.type_value = TRC_POS_DEGREE; //значение в град.
            trc.value = atof( GetParamVal( IND_PARAM2 ) );
            if ( SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) ) == ERROR ) {
                sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
                return strlen( buff );
               }
            sprintf( buff, "%s", Message( CONS_MSG_OK ) );
            return strlen( buff );
           }
        if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "hor" ) && strlen( GetParamVal( IND_PARAM2 ) ) ) {
            trc.ctrl = CAN_TRC_POS;
            trc.type_act = TRC_POS_HORIZONTAL;
            //определим тип значения параметра позиционирования
            if ( strchr( GetParamVal( IND_PARAM2 ), '.' ) == NULL )
                trc.type_value = TRC_POS_LENGTH;  //значение в мм
            else trc.type_value = TRC_POS_DEGREE; //значение в град.
            trc.value = atof( GetParamVal( IND_PARAM2 ) );
            if ( SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) ) == ERROR ) {
                sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
                return strlen( buff );
               }
            sprintf( buff, "%s", Message( CONS_MSG_OK ) );
            return strlen( buff );
           }
        sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       } 
    sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
    return strlen( buff );
 }

//*************************************************************************************************
// Управление генератором
// Запуск/останов/вывод состояния генератора
// uint8_t cnt_par - кол-во параметров, включая команду
// CmdId id_cmd    - идентификатор команды
// Source src      - режим вывода информации в консоль
//*************************************************************************************************
static uint32_t CmdGen( char *buff ) {

    uint8_t cmd;
    
    if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "start" ) ) {
        cmd = CAN_GEN_START;
        if ( SendCommand( ID_DEV_GEN, &cmd, sizeof( cmd ) ) == ERROR ) {
            sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
            return strlen( buff );
           }
        sprintf( buff, "%s", Message( CONS_MSG_OK ) );
        return strlen( buff );
       } 
    if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "stop" ) ) {
        cmd = CAN_GEN_STOP;
        if ( SendCommand( ID_DEV_GEN, &cmd, sizeof( cmd ) ) == ERROR ) {
            sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
            return strlen( buff );
           }
        sprintf( buff, "%s", Message( CONS_MSG_OK ) );
        return strlen( buff );
       } 
    if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "test" ) ) {
        cmd = CAN_GEN_TEST;
        if ( SendCommand( ID_DEV_GEN, &cmd, sizeof( cmd ) ) == ERROR ) {
            sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
            return strlen( buff );
           }
        sprintf( buff, "%s", Message( CONS_MSG_OK ) );
        return strlen( buff );
       } 
    sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
    return strlen( buff );
 }

//*************************************************************************************************
// Воспроизведение голосового сообщения по номеру
// uint8_t cnt_par - кол-во параметров, включая команду
// CmdId id_cmd    - идентификатор команды
// Source src      - режим вывода информации в консоль
//*************************************************************************************************
static uint32_t CmdVoice( char *buff ) {

    CAN_INFO info;

    memset( (uint8_t *)&info, 0x00, sizeof( info ) );
    info.ctrl = CAN_VOICE;
    info.voice = (VoiceId)atoi( GetParamVal( IND_PARAM1 ) );
    if ( SendCommand( ID_DEV_VOICE, (uint8_t *)&info, sizeof( info ) ) == ERROR ) {
        sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
        return strlen( buff );
       }
    sprintf( buff, "%s", Message( CONS_MSG_OK ) );
    return strlen( buff );
 }

//*************************************************************************************************
// Установка уровня громкости голосового сообщения
// uint8_t cnt_par - кол-во параметров, включая команду
// CmdId id_cmd    - идентификатор команды
// Source src      - режим вывода информации в консоль
//*************************************************************************************************
static uint32_t CmdVolume( char *buff ) {

    CAN_INFO info;

    memset( (uint8_t *)&info, 0x00, sizeof( info ) );
    if ( GetParamCnt() != 2 ) {
        sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    info.ctrl = CAN_VOLUME;
    info.volume = (Volume)atoi( GetParamVal( IND_PARAM1 ) );
    if ( SendCommand( ID_DEV_VOICE, (uint8_t *)&info, sizeof( info ) ) == ERROR ) {
        sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
        return strlen( buff );
       }
    sprintf( buff, "%s", Message( CONS_MSG_OK ) );
    return strlen( buff );
 }

//*************************************************************************************************
// Воспроизведение звукового сообщения по номеру
// uint8_t cnt_par - кол-во параметров, включая команду
// CmdId id_cmd    - идентификатор команды
// Source src      - режим вывода информации в консоль
//*************************************************************************************************
static uint32_t CmdSound( char *buff ) {

    CAN_INFO info;
    
    memset( (uint8_t *)&info, 0x00, sizeof( info ) );
    if ( GetParamCnt() != 2 ) {
        sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    //воспроизведение по номеру фрагмента
    info.ctrl = CAN_SOUND;
    info.sound = (SoundId)atoi( GetParamVal( IND_PARAM1 ) );
    if ( SendCommand( ID_DEV_VOICE, (uint8_t *)&info, sizeof( info ) ) == ERROR ) {
        sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
        return strlen( buff );
       }
    sprintf( buff, "%s", Message( CONS_MSG_OK ) );
    return strlen( buff );
 }

//*************************************************************************************************
// Управление резервными реле
// uint8_t cnt_par - кол-во параметров, включая команду
// CmdId id_cmd    - идентификатор команды
// Source src      - режим вывода информации в консоль
//*************************************************************************************************
static uint32_t CmdReserv( char *buff ) {

    int id, on, off, puls;
    CAN_RELAY relay;
    
    memset( (uint8_t *)&relay, 0x00, sizeof( relay ) );
    if ( GetParamCnt() != 3 ) {
        sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    //выполняем команды
    id = atoi( GetParamVal( IND_PARAM1 ) );
    on = strcasecmp( GetParamVal( IND_PARAM2 ), "on" );
    off = strcasecmp( GetParamVal( IND_PARAM2 ), "off" );
    puls = strcasecmp( GetParamVal( IND_PARAM2 ), "pulse" );
    if ( on && off && puls ) {
        sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       } 
    relay.relay = (RelayRes)id;
    if ( !on ) {
        relay.mode = RELAY_ON;
        if ( SendCommand( ID_DEV_RESERV, (uint8_t *)&relay, sizeof( relay ) ) == ERROR ) {
            sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
            return strlen( buff );
           }
       }
    if ( !off ) {
        relay.mode = RELAY_OFF;
        if ( SendCommand( ID_DEV_RESERV, (uint8_t *)&relay, sizeof( relay ) ) == ERROR ) {
            sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
            return strlen( buff );
           }
       }
    if ( !puls ) {
        relay.mode = RELAY_PULSE;
        if ( SendCommand( ID_DEV_RESERV, (uint8_t *)&relay, sizeof( relay ) ) == ERROR ) {
            sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
            return strlen( buff );
           }
       }
    sprintf( buff, "%s", Message( CONS_MSG_OK ) );
    return strlen( buff );
 }

//*************************************************************************************************
// Управление дополнительными выходами
// uint8_t cnt_par - кол-во параметров, включая команду
// CmdId id_cmd    - идентификатор команды
// Source src      - режим вывода информации в консоль
//*************************************************************************************************
static uint32_t CmdExtOut( char *buff ) {

    CAN_EXT ext;
    bool chk1 = false, chk2 = false;
    
    memset( (uint8_t *)&ext, 0x00, sizeof( ext ) );
    if ( GetParamCnt() != 3 ) {
        sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    //разбор параметров команды
    if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "lf" ) ) {
        chk1 = true;
        ext.relay = EXT_OUT_LF;
       }
    if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "rt" ) ) {
        chk1 = true;
        ext.relay = EXT_OUT_RT;
       }
    if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "up" ) ) {
        chk1 = true;
        ext.relay = EXT_OUT_UP;
       }
    if ( !strcasecmp( GetParamVal( IND_PARAM1 ), "dn" ) ) {
        chk1 = true;
        ext.relay = EXT_OUT_DN;
       }
    if ( !strcasecmp( GetParamVal( IND_PARAM2 ), "on" ) ) {
        chk2 = true;
        ext.mode = RELAY_ON;
       }
    if ( !strcasecmp( GetParamVal( IND_PARAM2 ), "off" ) ) {
        chk2 = true;
        ext.mode = RELAY_OFF;
       }
    if ( !strcasecmp( GetParamVal( IND_PARAM2 ), "pulse" ) ) {
        chk2 = true;
        ext.mode = RELAY_PULSE;
       }
    //выполнение команды
    if ( chk1 == true && chk2 == true ) {
        if ( SendCommand( ID_DEV_EXTOUT, (uint8_t *)&ext, sizeof( ext ) ) == ERROR ) {
            sprintf( buff, "%s", Message( CONS_MSG_ERR_SEND ) );
            return strlen( buff );
           }
        sprintf( buff, "%s", Message( CONS_MSG_OK ) );
        return strlen( buff );
       }
    sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
    return strlen( buff );
 }

//*********************************************************************************************
// Сохранение параметров в EEPROM
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdConfSave( char *buff ) {

    char *ptr;
    
    if ( buff == NULL )
        return 0;
    ptr = buff;
    ptr += ConfigHmiSave( &cfg_hmi, ptr );
    ptr += ConfigLcdSave( &cfg_lcd, ptr );
    return strlen( buff );
 }

//*************************************************************************************************
// Вывод дампа памяти EEPROM по 1 блоку (64 байта)
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*************************************************************************************************
static uint32_t CmdEeprom( char *buff ) {

    if ( buff == NULL )
        return 0;
    //вывод дампа памяти EEPROM
    if ( ( GetParamCnt() == 1 || GetParamCnt() == 2 ) && strcasecmp( GetParamVal( IND_PARAM1 ), "clr" ) )
        return Eeprom( buff, atoi( GetParamVal( IND_PARAM1 ) ), EEPROM_READ_PAGE );
    //очистка блока памяти в EEPROM
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "clr" ) )
        return Eeprom( buff, atoi( GetParamVal( IND_PARAM2 ) ), EEPROM_CLEAR_PAGE );
    return sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM )  );
 }
 
//*********************************************************************************************
// Управление настройками LAN
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdConfNet( char *buff ) {

    uint16_t port;
    char *ptr, host[NET_HOSTNAME_LEN];
    uint8_t ip4_addr[NET_ADDR_IP4_LEN], mac_addr[NET_ADDR_ETH_LEN];

    if ( buff == NULL )
        return 0;
    ptr = buff;
    if ( GetParamCnt() == 1 )
        return GetEthInfo( buff );
    if ( GetParamCnt() == 2 && !strcasecmp( GetParamVal( IND_PARAM1 ), "save" ) )
        return ConfigHmiSave( &cfg_hmi, buff );
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "mac" ) ) {
        if ( netMAC_aton( GetParamVal( IND_PARAM2 ), &mac_addr[0] ) ) {
            memcpy( cfg_hmi.mac_addr, mac_addr, sizeof( cfg_hmi.mac_addr ) );
            if ( netIF_SetOption( NET_IF_CLASS_ETH, netIF_OptionMAC_Address, cfg_hmi.mac_addr, sizeof( cfg_hmi.mac_addr ) ) == netOK )
                sprintf( ptr, "%s", Message( CONS_MSG_OK ) );
            else sprintf( ptr, "%s", Message( CONS_MSG_ERR_PARAM ) );
           }
        else sprintf( ptr, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "ip" ) ) {
        if ( netIP_aton( GetParamVal( IND_PARAM2 ), NET_ADDR_IP4, &ip4_addr[0] ) ) {
            memcpy( cfg_hmi.ip_addr, ip4_addr, sizeof( cfg_hmi.ip_addr ) );
            netIF_SetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_Address, cfg_hmi.ip_addr, sizeof( cfg_hmi.ip_addr ) );
            sprintf( ptr, "%s", Message( CONS_MSG_OK ) );
           }
        else sprintf( ptr, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "mask" ) ) {
        if ( netIP_aton( GetParamVal( IND_PARAM2 ), NET_ADDR_IP4, &ip4_addr[0] ) ) {
            memcpy( cfg_hmi.ip_mask, ip4_addr, sizeof( cfg_hmi.ip_mask ) );
            netIF_SetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_SubnetMask, cfg_hmi.ip_mask, sizeof( cfg_hmi.ip_mask ) );
            sprintf( ptr, "%s", Message( CONS_MSG_OK ) );
           }
        else sprintf( ptr, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "gate" ) ) {
        if ( netIP_aton( GetParamVal( IND_PARAM2 ), NET_ADDR_IP4, &ip4_addr[0] ) ) {
            memcpy( cfg_hmi.ip_gate, ip4_addr, sizeof( cfg_hmi.ip_gate ) );
            netIF_SetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_DefaultGateway, cfg_hmi.ip_gate, sizeof( cfg_hmi.ip_gate ) );
            sprintf( ptr, "%s", Message( CONS_MSG_OK ) );
           }
        else sprintf( ptr, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "dns1" ) ) {
        if ( netIP_aton( GetParamVal( IND_PARAM2 ), NET_ADDR_IP4, &ip4_addr[0] ) ) {
            memcpy( cfg_hmi.ip_dns1, ip4_addr, sizeof( cfg_hmi.ip_dns1 ) );
            netIF_SetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_PrimaryDNS, cfg_hmi.ip_dns1, sizeof( cfg_hmi.ip_gate ) );
            sprintf( ptr, "%s", Message( CONS_MSG_OK ) );
           }
        else sprintf( ptr, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "dns2" ) ) {
        if ( netIP_aton( GetParamVal( IND_PARAM2 ), NET_ADDR_IP4, &ip4_addr[0] ) ) {
            memcpy( cfg_hmi.ip_dns2, ip4_addr, sizeof( cfg_hmi.ip_dns2 ) );
            netIF_SetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_SecondaryDNS, cfg_hmi.ip_dns2, sizeof( cfg_hmi.ip_gate ) );
            sprintf( ptr, "%s", Message( CONS_MSG_OK ) );
           }
        else sprintf( ptr, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "host" ) ) {
        if ( strlen( GetParamVal( IND_PARAM2 ) ) <= NET_HOSTNAME_LEN ) {
            memset( host, 0x00, sizeof( host ) );
            strcpy( host, GetParamVal( IND_PARAM2 ) );
            if ( netSYS_SetHostName( host ) == netOK )
                sprintf( ptr, "%s", Message( CONS_MSG_OK ) );
            else sprintf( ptr, "%s", Message( CONS_MSG_ERR_PARAM ) );
           }
        else sprintf( ptr, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "port1" ) ) {
        port = atoi( GetParamVal( IND_PARAM2 ) );
        if ( port >= 1024 ) {
            cfg_hmi.ip_port1 = port;
            sprintf( ptr, "%s", Message( CONS_MSG_OK ) );
           }
        else sprintf( ptr, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "port2" ) ) {
        port = atoi( GetParamVal( IND_PARAM2 ) );
        if ( port >= 1024 ) {
            cfg_hmi.ip_port2 = port;
            sprintf( ptr, "%s", Message( CONS_MSG_OK ) );
           }
        else sprintf( ptr, "%s", Message( CONS_MSG_ERR_PARAM ) );
        return strlen( buff );
       }
    return sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
 }

//*********************************************************************************************
// Проверка соединения с удаленным узлом по IP или имени хоста
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdPing( char *buff ) {

    if ( buff == NULL )
        return 0;
    if ( GetParamCnt() != 2 )
        return sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
    return PingEcho( buff, GetParamVal( IND_PARAM1 ) );
 }

//*********************************************************************************************
// Вывод версии прошивки
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdSystem( char *buff ) {

    if ( buff == NULL )
        return 0;
    return GetVersion( buff );
 }

//*********************************************************************************************
// Вывод списка загруженных файлов на электронном диске
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdRamDir( char *buff ) {

    if ( buff == NULL )
        return 0;
    return RamDiskDir( buff );
 }

//*********************************************************************************************
// Настройка параметров контроллера
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdConfig( char *buff ) {

    if ( buff == NULL )
        return 0;
    if ( GetParamCnt() == 1 ) {
        return GetNetInfo( buff );
       }
    if ( GetParamCnt() == 2 && !strcasecmp( GetParamVal( IND_PARAM1 ), "save" ) ) {
        ConfigHmiSave( &cfg_hmi, buff );
        return sprintf( buff, "%s", Message( CONS_MSG_OK ) );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "http_user" ) ) {
        memset( cfg_hmi.user_name1, 0x00, sizeof( cfg_hmi.user_name1 ) );
        strncpy( cfg_hmi.user_name1, GetParamVal( IND_PARAM2 ), sizeof( cfg_hmi.user_name1 ) );
        return sprintf( buff, "%s", Message( CONS_MSG_OK ) );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "http_pass" ) ) {
        memset( cfg_hmi.passwd_hash1, 0x00, sizeof( cfg_hmi.passwd_hash1 ) );
        CalcMD5( GetParamVal( IND_PARAM2 ), cfg_hmi.passwd_hash1 );
        return sprintf( buff, "%s", Message( CONS_MSG_OK ) );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "ftp_user" ) ) {
        memset( cfg_hmi.user_name2, 0x00, sizeof( cfg_hmi.user_name2 ) );
        strncpy( cfg_hmi.user_name2, GetParamVal( IND_PARAM2 ), sizeof( cfg_hmi.user_name2 ) );
        return sprintf( buff, "%s", Message( CONS_MSG_OK ) );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "ftp_pass" ) ) {
        memset( cfg_hmi.passwd_hash2, 0x00, sizeof( cfg_hmi.passwd_hash2 ) );
        CalcMD5( GetParamVal( IND_PARAM2 ), cfg_hmi.passwd_hash2 );
        return sprintf( buff, "%s", Message( CONS_MSG_OK ) );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "telnet_user" ) ) {
        memset( cfg_hmi.user_name3, 0x00, sizeof( cfg_hmi.user_name3 ) );
        strncpy( cfg_hmi.user_name3, GetParamVal( IND_PARAM2 ), sizeof( cfg_hmi.user_name3 ) );
        return sprintf( buff, "%s", Message( CONS_MSG_OK ) );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "telnet_pass" ) ) {
        memset( cfg_hmi.passwd_hash3, 0x00, sizeof( cfg_hmi.passwd_hash3 ) );
        CalcMD5( GetParamVal( IND_PARAM2 ), cfg_hmi.passwd_hash3 );
        return sprintf( buff, "%s", Message( CONS_MSG_OK ) );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "mail" ) ) {
        memset( cfg_hmi.mail_addr, 0x00, sizeof( cfg_hmi.mail_addr ) );
        strncpy( cfg_hmi.mail_addr, GetParamVal( IND_PARAM2 ), sizeof( cfg_hmi.mail_addr ) );
        return sprintf( buff, "%s", Message( CONS_MSG_OK ) );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "mail_port" ) ) {
        cfg_hmi.mail_port = atoi( GetParamVal( IND_PARAM2 ) );
        return sprintf( buff, "%s", Message( CONS_MSG_OK ) );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "mail_user" ) ) {
        memset( cfg_hmi.mail_user, 0x00, sizeof( cfg_hmi.mail_user ) );
        strncpy( cfg_hmi.mail_user, GetParamVal( IND_PARAM2 ), sizeof( cfg_hmi.mail_user ) );
        return sprintf( buff, "%s", Message( CONS_MSG_OK ) );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "mail_pass" ) ) {
        memset( cfg_hmi.mail_passwd, 0x00, sizeof( cfg_hmi.mail_passwd ) );
        strncpy( cfg_hmi.mail_passwd, GetParamVal( IND_PARAM2 ), sizeof( cfg_hmi.mail_passwd ) );
        return sprintf( buff, "%s", Message( CONS_MSG_OK ) );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "mail_from" ) ) {
        memset( cfg_hmi.mail_from, 0x00, sizeof( cfg_hmi.mail_from ) );
        strncpy( cfg_hmi.mail_from, GetParamVal( IND_PARAM2 ), sizeof( cfg_hmi.mail_from ) );
        return sprintf( buff, "%s", Message( CONS_MSG_OK ) );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "mail_to" ) ) {
        memset( cfg_hmi.mail_to, 0x00, sizeof( cfg_hmi.mail_to ) );
        strncpy( cfg_hmi.mail_to, GetParamVal( IND_PARAM2 ), sizeof( cfg_hmi.mail_to ) );
        return sprintf( buff, "%s", Message( CONS_MSG_OK ) );
       }
    if ( GetParamCnt() == 3 && !strcasecmp( GetParamVal( IND_PARAM1 ), "mail_cc" ) ) {
        memset( cfg_hmi.mail_cc, 0x00, sizeof( cfg_hmi.mail_cc ) );
        strncpy( cfg_hmi.mail_cc, GetParamVal( IND_PARAM2 ), sizeof( cfg_hmi.mail_cc ) );
        return sprintf( buff, "%s", Message( CONS_MSG_OK ) );
       }
    return sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
 }

//*********************************************************************************************
// Вывод текстового файла в консоль
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdTypeFile( char *buff ) {

    if ( buff == NULL )
        return 0;
    return FileType( buff, GetParamVal( IND_PARAM1 ), 0 );
 }

//*********************************************************************************************
// Формирование копии экрана в формате BMP на SD карте
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
static uint32_t CmdScreen( char *buff ) {

    uint16_t ind;
    
    ind = atoi( GetParamVal( IND_PARAM1 ) );
    if ( ind < 256 ) {
        CrtScreen( ind );
        return sprintf( buff, "%s", Message( CONS_MSG_OK ) );
       }
    else return sprintf( buff, "%s", Message( CONS_MSG_ERR_PARAM ) );
 }
 
//*********************************************************************************************
// Возвращает указатель на буфер памяти во внешней памяти
//*********************************************************************************************
char *GetBuffer( void ) {

    return ptr_buff;
 }

//*********************************************************************************************
// Возвращает размер буфера памяти во внешней памяти
//*********************************************************************************************
uint32_t GetBuffSize( void ) {

    return UARTBUF_SIZE;
 }
