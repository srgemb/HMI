
//*************************************************************************************************
//
// Управление обменом данными по CAN шине
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

#include "can_lpc40xx.h"

#include "device.h"
#include "dev_data.h"
#include "dev_param.h"
#include "can_data.h"
#include "can_def.h"

#include "rtc.h"
#include "uart.h"
#include "sdcard.h"
#include "priority.h"
#include "command.h"
#include "can.h"
#include "data.h"
#include "events.h"

//*************************************************************************************************
// Переменные с внешним доступом
//*************************************************************************************************
osMessageQueueId_t link_msg = NULL;

//*************************************************************************************************
// Локальные константы
//*************************************************************************************************
#define CAN_BITRATE_NOMINAL     125000      //скорость обмена (125 kbit/s)
#define CAN_TIME_OUT            400         //время отсутствия принятых пакетов по CAN шине (msec)

//*************************************************************************************************
// Внешние переменные
//*************************************************************************************************
extern ARM_DRIVER_CAN Driver_CAN1;

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
ARM_DRIVER_CAN *CanDrv;

static uint8_t rx_data[8];
static bool req_config = false;
static Status link_status = ERROR;
static uint32_t rx_obj_idx, tx_obj_idx;
static int32_t send_stat = 0, send_pack = 0, error_send = 0;

static osTimerId_t timer_link;
static osMessageQueueId_t cmd_msg = NULL;
static osSemaphoreId_t can_semaph = NULL;
static ARM_CAN_MSG_INFO rx_msg_info, tx_msg_info;

//*************************************************************************************************
// Атрибуты объектов RTOS
//*************************************************************************************************
static const osThreadAttr_t link_attr = {
    .name = "Link", 
    .stack_size = 384,
    .priority = osPriorityNormal
 };

static const osThreadAttr_t cmd_attr = {
    .name = "Cmd", 
    .stack_size = 896,
    .priority = osPriorityNormal
 };

static const osSemaphoreAttr_t sem_attr = { .name = "Cmd" };
static const osMessageQueueAttr_t que1_attr = { .name = "Link" };
static const osMessageQueueAttr_t que2_attr = { .name = "Cmd" };
static const osTimerAttr_t timer1_attr = { .name = "Link" };

//*************************************************************************************************
// Прототипы локальных функций
//*************************************************************************************************
static void TaskLink( void *pvParameters );
static void TaskCmd( void *pvParameters );
static void Timer1Callback( void *arg );
static void CANSendFrame( uint32_t can_id, uint8_t *ptr_data, uint8_t len_data );
static void CAN_SignalObjectEvent( uint32_t obj_idx, uint32_t event );
static void CmdSaveLog( MSGQUEUE_CAN *que_cmd );
static void ConfigSaveLog( ConfigParam id_par, ConfigValSet cfg_set );

//*************************************************************************************************
// Инициализация CAN1 для обмена данными с внешним контроллером управляющим контроллером
//*************************************************************************************************
void CANInit( void ) {

    int32_t status;
    uint32_t i, clock;
    ARM_CAN_CAPABILITIES can_cap;
    ARM_CAN_OBJ_CAPABILITIES can_obj_cap;
    
    //очистим данные и параметры от управляющего контроллера
    ClearData();
    memset( (uint8_t *)&config, 0x00, sizeof( config ) );
    //семафор ожидания завершения передачи одного сообщения
    can_semaph = osSemaphoreNew( 1, 0, &sem_attr );
    //очередь сообщений
    link_msg = osMessageQueueNew( 128, sizeof( MSGQUEUE_CAN ), &que1_attr );
    cmd_msg = osMessageQueueNew( 32, sizeof( MSGQUEUE_CAN ), &que2_attr );
    //таймер паузы в приеме пакетов
    timer_link = osTimerNew( Timer1Callback, osTimerOnce, NULL, &timer1_attr );
    //создаем задачу
    osThreadNew( TaskLink, NULL, &link_attr );
    osThreadNew( TaskCmd, NULL, &cmd_attr );
    //инициализация CAN
    CanDrv = &Driver_CAN1;
    can_cap = CanDrv->GetCapabilities();
    CanDrv->Initialize( NULL, CAN_SignalObjectEvent );
    CanDrv->PowerControl( ARM_POWER_FULL );
    CanDrv->SetMode( ARM_CAN_MODE_INITIALIZATION );
    //настройка скорости
    clock = CanDrv->GetClock();
    if ( ( clock % ( 8U * CAN_BITRATE_NOMINAL ) ) == 0U ) {                               
        //If CAN base clock is divisible by 8 * nominal bitrate without remainder
        //Set nominal bitrate
        //Set nominal bitrate to configured constant value
        //Set propagation segment to 5 time quanta
        //Set phase segment 1 to 1 time quantum (sample point at 87.5% of bit time)
        //Set phase segment 2 to 1 time quantum (total bit is 8 time quanta long)
        //Resynchronization jump width is same as phase segment 2
        status = CanDrv->SetBitrate( ARM_CAN_BITRATE_NOMINAL, CAN_BITRATE_NOMINAL, ARM_CAN_BIT_PROP_SEG( 5U ) | ARM_CAN_BIT_PHASE_SEG1( 1U ) | ARM_CAN_BIT_PHASE_SEG2( 1U ) | ARM_CAN_BIT_SJW( 1U ) );
    } else if ( ( clock % ( 10U * CAN_BITRATE_NOMINAL ) ) == 0U ) {                     
        //If CAN base clock is divisible by 10 * nominal bitrate without remainder
        //Set nominal bitrate
        //Set nominal bitrate to configured constant value
        //Set propagation segment to 7 time quanta
        //Set phase segment 1 to 1 time quantum (sample point at 90% of bit time)
        //Set phase segment 2 to 1 time quantum (total bit is 10 time quanta long)
        //Resynchronization jump width is same as phase segment 2
        status = CanDrv->SetBitrate( ARM_CAN_BITRATE_NOMINAL, CAN_BITRATE_NOMINAL, ARM_CAN_BIT_PROP_SEG( 7U ) | ARM_CAN_BIT_PHASE_SEG1( 1U ) | ARM_CAN_BIT_PHASE_SEG2( 1U ) | ARM_CAN_BIT_SJW( 1U ) );                
    } else if ( ( clock % ( 12U * CAN_BITRATE_NOMINAL ) ) == 0U ) {                       
        //If CAN base clock is divisible by 12 * nominal bitrate without remainder
        //Set nominal bitrate
        //Set nominal bitrate to configured constant value
        //Set propagation segment to 7 time quanta
        //Set phase segment 1 to 2 time quantum (sample point at 83.3% of bit time)
        //Set phase segment 2 to 2 time quantum (total bit is 12 time quanta long)
        //Resynchronization jump width is same as phase segment 2
        status = CanDrv->SetBitrate( ARM_CAN_BITRATE_NOMINAL, CAN_BITRATE_NOMINAL, ARM_CAN_BIT_PROP_SEG( 7U ) | ARM_CAN_BIT_PHASE_SEG1( 2U ) | ARM_CAN_BIT_PHASE_SEG2( 2U ) | ARM_CAN_BIT_SJW( 2U ) );                
       }
    if ( status != ARM_DRIVER_OK ) 
        return; //инициализация не выполнена
    //определение ID объектов сообщений
    rx_obj_idx = 0xFFFFFFFFU;
    tx_obj_idx = 0xFFFFFFFFU;
    for ( i = 0; i < can_cap.num_objects; i++ ) {
        //поиск первого доступного объекта для приема
        can_obj_cap = CanDrv->ObjectGetCapabilities( i );
        if ( ( rx_obj_idx == 0xFFFFFFFFU ) && ( can_obj_cap.rx == 1 ) )
            rx_obj_idx = i;
        //поиск первого доступного объекта для передачи
        else if ( ( tx_obj_idx == 0xFFFFFFFFU ) && ( can_obj_cap.tx == 1 ) ) { 
                tx_obj_idx = i; 
                break; 
               }
       }
    if ( ( rx_obj_idx == 0xFFFFFFFFU ) || ( tx_obj_idx == 0xFFFFFFFFU ) )
        return; //доступных объектов нет
    //конфигурация найденных объектов прием/передача/фильтр
    CanDrv->ObjectConfigure( tx_obj_idx, ARM_CAN_OBJ_TX );
    CanDrv->ObjectConfigure( rx_obj_idx, ARM_CAN_OBJ_RX );
    //для установки фильтра с учетом 29-битного идентификаторов необходимо добавить атрибут ARM_CAN_ID_IDE_Msk 
    CanDrv->ObjectSetFilter( rx_obj_idx, ARM_CAN_FILTER_ID_RANGE_ADD, 0 | ARM_CAN_ID_IDE_Msk, CAN_FILTER_RANGE );
    CanDrv->SetMode( ARM_CAN_MODE_NORMAL ); 
    //настройка прерывания
    NVIC_SetPriority( CAN_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), PRIORITY_SSP, SUB_PRIORITY_CAN ) );
 }

//*************************************************************************************************
// Задача управления приемом данных по протоколу CAN
//*************************************************************************************************
static void TaskLink( void *pvParameters ) {

    osStatus_t status;
    MSGQUEUE_LOG mess;
    MSGQUEUE_CAN data, que_cmd;
    
    for ( ;; ) {
        status = osMessageQueueGet( link_msg, &data, NULL, osWaitForever );
        if ( status == osOK ) {
            //есть принятые данные
            link_status = SUCCESS;
            osTimerStart( timer_link, CAN_TIME_OUT );
            //разбор принятых данных
            CanData( data );
            //если нет данных параметров конфигурации, запросим параметры
            if ( data.dev_id == ID_DEV_RTC && ( !strlen( config.job_file ) || !strlen( config.job_test ) ) && req_config == true ) {
                memset( (uint8_t *)&que_cmd, 0x00, sizeof( que_cmd ) );
                que_cmd.dev_id = ID_CONFIG;
                osMessageQueuePut( cmd_msg, &que_cmd, 0, osWaitForever );
               }
            if ( data.dev_id == ID_CONFIG ) {
                //получены данные с параметрами, загрузим задания планировщика
                osEventFlagsSet( job_event, EVN_SCHEDULER_LOAD );
               }
            if ( data.dev_id == ID_DEV_LOG ) {
                //получено сообщение о событии, передаем в TaskEvent()
                mess.dev_id = (Device)data.param_id;
                mess.id_mess = (LogMessId)data.data[0];
                osMessageQueuePut( log_msg, &mess, 0, osWaitForever );
               }
           }
        #if ( OS_ROBIN_ENABLE == 0 )
        osThreadYield();
        #endif
       }
 }

//*************************************************************************************************
// Задача передачи команд по протоколу CAN
//*************************************************************************************************
static void TaskCmd( void *pvParameters ) {

    uint32_t can_id;
    osStatus_t status;
    MSGQUEUE_CAN que_cmd;
    
    for ( ;; ) {
        status = osMessageQueueGet( cmd_msg, &que_cmd, NULL, osWaitForever );
        if ( status == osOK ) {
            can_id = CAN_DEV_ID( que_cmd.dev_id ) | CAN_PARAM_ID( que_cmd.param_id ) | CAN_PACK_SUB( que_cmd.sub_pack_id );
            CANSendFrame( can_id, (uint8_t *)&que_cmd.data, que_cmd.len_data );
            CmdSaveLog( &que_cmd );
           }
        #if ( OS_ROBIN_ENABLE == 0 )
        osThreadYield();
        #endif
       }
 }

//*************************************************************************************************
// Функция обратного вызова таймера - пауза при приеме данных
//*************************************************************************************************
static void Timer1Callback( void *arg ) {

    ClearData();
    link_status = ERROR;
 }

//*************************************************************************************************
// Обработка событий CAN шины
//*************************************************************************************************
static void CAN_SignalObjectEvent( uint32_t obj_idx, uint32_t event ) {

    MSGQUEUE_CAN que_link;
    
    if ( event == ARM_CAN_EVENT_RECEIVE ) {
        if ( obj_idx == rx_obj_idx ) {
            //принятые данные помещаем в очередь на исполнение
            CanDrv->MessageRead( rx_obj_idx, &rx_msg_info, rx_data, sizeof( rx_data ) );
            que_link.dev_id = (Device)CAN_GET_DEV_ID( ( rx_msg_info.id & CAN_MASK_DEV_ID ) );
            que_link.param_id = (ConfigParam)CAN_GET_PARAM_ID( ( rx_msg_info.id & CAN_MASK_PARAM_ID ) );
            que_link.sub_pack_id = CAN_GET_PACK_ID( ( rx_msg_info.id & CAN_MASK_PACK_ID ) );
            que_link.len_data = rx_msg_info.dlc;
            //помещаем в очередь
            memcpy( (uint8_t *)que_link.data, (uint8_t *)rx_data, que_link.len_data );
            osMessageQueuePut( link_msg, &que_link, 0, 0 );
            return;
           }
       }
    if ( event == ARM_CAN_EVENT_SEND_COMPLETE ) {
        if ( obj_idx == tx_obj_idx ) {
            //снимаем семафор ожидания для отправки следующего блока данных
            osSemaphoreRelease( can_semaph );
           }
       }
 }

//*************************************************************************************************
// Запуск передачи одного сообщения по CAN шине
// uint32_t can_id   - ID пакета
// uint8_t *ptr_data - указатель на блок данных
// uint8_t len_data  - размер блока данных для передачи
//*************************************************************************************************
static void CANSendFrame( uint32_t can_id, uint8_t *ptr_data, uint8_t len_data ) {

    //установим семафор ожидания завершения передачи
    osSemaphoreAcquire( can_semaph, 0 );
    memset( &tx_msg_info, 0x00, sizeof( ARM_CAN_MSG_INFO ) );
    tx_msg_info.id = ARM_CAN_EXTENDED_ID( can_id );
    if ( can_id == CAN_DEV_ID( ID_CONFIG ) && len_data == 0 )
        tx_msg_info.rtr = 1;
    send_stat = CanDrv->MessageSend( tx_obj_idx, &tx_msg_info, ptr_data, len_data );
    send_pack++;
    if ( send_stat < 0 )
        error_send++;
    //ждем семафор - завершение передачи предыдущего блока
    osSemaphoreAcquire( can_semaph, osWaitForever );
 }

//*************************************************************************************************
// Передача команды управляющему контроллеру по CAN шине 
// Device dev_id     - ID устройства
// uint8_t *ptr_data - указатель на блок данных
// uint8_t len_data  - размер блока данных для передачи
//*************************************************************************************************
Status SendCommand( Device dev_id, uint8_t *ptr_data, uint8_t len_data ) {

    MSGQUEUE_CAN que_cmd;
    
    if ( link_status == ERROR )
        return ERROR;
    que_cmd.dev_id = dev_id;
    if ( dev_id == ID_CONFIG && *ptr_data == CAN_CONFIG_SAVE && len_data == 1 ) {
        //команда записи параметров в EEPROM
        que_cmd.dev_id = dev_id;
        que_cmd.param_id = 0;
        que_cmd.sub_pack_id = *ptr_data;
        que_cmd.len_data = 0;
       }
    else {
        //передача обычных команд
        que_cmd.dev_id = dev_id;
        que_cmd.param_id = 0;
        que_cmd.sub_pack_id = 0;
        que_cmd.len_data = len_data;
       }
    //помещаем в очередь
    memcpy( (uint8_t *)que_cmd.data, ptr_data, len_data );
    osMessageQueuePut( cmd_msg, &que_cmd, 0, osWaitForever );
    return SUCCESS;
 }

//*************************************************************************************************
// Передача параметра настройки управляющему контроллеру по CAN шине 
// ConfigParam conf_id - ID параметра
// ConfigValSet value  - значение параметра
//*************************************************************************************************
Status SendConfig( ConfigParam conf_id, ConfigValSet value ) {

    MSGQUEUE_CAN que_cmd;
    uint8_t len_data, *ptr_data, length;
    uint8_t sub_pack_id = 0, cnt_pack = 1, offset = 0;
    
    if ( link_status == ERROR )
        return ERROR;
    ConfigSaveLog( conf_id, value );
    que_cmd.dev_id = ID_CONFIG;
    que_cmd.param_id = conf_id;
    //размер параметра в байтах
    len_data = ConfigParSize( conf_id );
    if ( len_data > 8 ) {
        length = 8;
        cnt_pack = 2;
       }
    else length = len_data;
    if ( conf_id == CFG_SCR_FILE || conf_id == CFG_JOB_FILE || conf_id == CFG_JOB_TEST )
        ptr_data = (uint8_t *)value.ptr; //данные размещены по указателю из ConfigValSet
    else ptr_data = (uint8_t *)&value;   //данные размещены в ConfigValSet
    do { 
        que_cmd.len_data = length;
        que_cmd.sub_pack_id = sub_pack_id;
        //размещаем данные для передачи в очередь
        memset( (uint8_t *)que_cmd.data, 0x00, sizeof( que_cmd.data ) );
        memcpy( (uint8_t *)que_cmd.data, ptr_data + offset, length );
        //значение параметра помещаем в очередь для передачи
        osMessageQueuePut( cmd_msg, &que_cmd, 0, osWaitForever );
        sub_pack_id++;
        offset += length;
      } while ( --cnt_pack );
    return SUCCESS;
 }
 
//*************************************************************************************************
// Разрешение на выполнение запроса параметров конфигурации
//*************************************************************************************************
void ReqConfig( void ) {

    req_config = true;
 }


//*************************************************************************************************
// Возвращает значение параметра
// ParamHmi id_param - ID параметра
// return ValueParam - значение параметра
//*************************************************************************************************
ValueParam LinkGetValue( ParamHmi id_param ) {

    ValueParam value;
    ARM_CAN_STATUS can_status;
    
    value.uint32 = NULL;
    can_status = CanDrv->GetStatus();
    if ( id_param == HMI_LINK ) {
        if ( link_status == ERROR )
            value.uint8 = LINK_CONN_NO;
        else value.uint8 = LINK_CONN_OK;
       }
    if ( id_param == HMI_SEND )
        value.uint32 = send_pack;
    if ( id_param == HMI_ERROR_SEND )
        value.uint32 = error_send;
    if ( id_param == HMI_ERROR_TX )
        value.uint8 = can_status.tx_error_count;
    if ( id_param == HMI_ERROR_RX )
        value.uint8 = can_status.rx_error_count;
    return value;
 }

//*************************************************************************************************
// Возвращает статус связи по CAN шине
// return CANLinkStat - состояние связи
//*************************************************************************************************
CANLinkStat LinkStat( void ) {

    ARM_CAN_STATUS can_status;

    can_status = CanDrv->GetStatus();
    if ( link_status == ERROR )
        return CAN_LINK_NO; //связи нет
    else {
        if ( error_send || can_status.tx_error_count || can_status.rx_error_count )
            return CAN_LINK_ERROR; //есть ошибки
        else return CAN_LINK_OK;   //связь без ошибок
       }
 }
 
//*************************************************************************************************
// Добавляет в протокол "cmd_yyyymmdd.log" команды управления от HMI
// MSGQUEUE_CAN *que_cmd - данные пакета
//*************************************************************************************************
static void CmdSaveLog( MSGQUEUE_CAN *que_cmd ) {

    uint8_t i;
    FILE *cmd_log;
    char name[40];

    if ( SDStatus() == ERROR )
        return; //карты нет
    //открываем файл
    sprintf( name, "\\execute\\cmd_%s.log", RTCFileName() );
    cmd_log = fopen( name, "a" );
    if ( cmd_log == NULL )
        return; //файл не открылся
    //запишем данные
    fprintf( cmd_log, "%s ID=0x%03X %s PAR=0x%02X %s SUB=0x%02X LEN=%u DATA=", RTCGetLog(), que_cmd->dev_id, DevName( que_cmd->dev_id ), 
             que_cmd->param_id, que_cmd->param_id ? ConfigName( que_cmd->param_id ) : "", que_cmd->sub_pack_id, que_cmd->len_data );
    for ( i = 0; i < que_cmd->len_data; i++ )
        fprintf( cmd_log, "0x%02X ", *( que_cmd->data + i ) );
    fprintf( cmd_log, "\r\n" );
    fclose( cmd_log );
 }

//*************************************************************************************************
// Добавляем в протокол "cfg_yyyymmdd.log" изменяемые значения параметров
// ConfigParam id_par   - ID параметра
// ConfigValSet cfg_set - значение параметра
//*************************************************************************************************
static void ConfigSaveLog( ConfigParam id_par, ConfigValSet cfg_set ) {

    FILE *cfg_log;
    const DevParam *dev_ptr;
    char name[40], buff[40], old[40], str[120];

    if ( SDStatus() == ERROR )
        return; //карты нет
    //открываем файл
    sprintf( name, "\\execute\\cfg_%s.log", RTCFileName() );
    cfg_log = fopen( name, "a" );
    if ( cfg_log == NULL )
        return; //файл не открылся
    dev_ptr = DevParamPtr( ID_CONFIG );
    if ( dev_ptr[id_par].subtype == STRING )
        sprintf( buff, "%s", cfg_set.ptr );
    if ( dev_ptr[id_par].subtype == NUMBER || dev_ptr[id_par].subtype == SYS_MODE || 
         dev_ptr[id_par].subtype == MODE_DIR || dev_ptr[id_par].subtype == BOOL1 || dev_ptr[id_par].subtype == BOOL7 )
        sprintf( buff, "%d", cfg_set.uint16 );
    if ( dev_ptr[id_par].subtype == FLOAT )
        sprintf( buff, "%.6f", cfg_set.dbl );
    if ( dev_ptr[id_par].subtype == SDATE )
        sprintf( buff, "%02u.%02u.%04u", cfg_set.date.day, cfg_set.date.month, cfg_set.date.year );
    if ( dev_ptr[id_par].subtype == TIMESTART )  
        sprintf( buff, "%d,%d,%d,%d,%d,%d,%d,%d", cfg_set.uint8_array[0], cfg_set.uint8_array[1], cfg_set.uint8_array[2], cfg_set.uint8_array[3],
        cfg_set.uint8_array[4], cfg_set.uint8_array[5], cfg_set.uint8_array[6], cfg_set.uint8_array[7] );
    strcpy( old, ParamGetForm( ID_CONFIG, id_par, PARAM_VALUE ) );
    sprintf( str, "%s %s: new value: %s, old value: %s\r\n", RTCGetLog(), ConfigName( id_par ), buff, old );
    UARTSendStr( str );
    fprintf( cfg_log, "%s", str );
    fclose( cfg_log );
 }
