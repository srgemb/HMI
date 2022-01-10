
//*************************************************************************************************
//
// Основной модуль
//
//*************************************************************************************************

#include <stdio.h>
#include <stdbool.h>

#include "RTE_Components.h"

#include "lpc_wwdt.h"

#include "cmsis_os2.h"

#ifdef RTE_Compiler_EventRecorder
#include "EventRecorder.h"
#endif

#include "net.h"
#include "can.h"
#include "rtc.h"
#include "lcd.h"
#include "uart.h"
#include "main.h"
#include "ports.h"
#include "sdcard.h"
#include "eeprom.h"
#include "data_log.h"
#include "system.h"
#include "message.h"
#include "command.h"
#include "window.h"
#include "ram_disk.h"
#include "scheduler.h"

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
uint32_t reset;

//*************************************************************************************************
// Атрибуты объектов RTOS
//*************************************************************************************************
static const osThreadAttr_t init_attr = {
    .name = "Init", 
    .stack_size = 1280,
    .priority = osPriorityNormal };
    
//*************************************************************************************************
// Прототипы локальных функций
//*************************************************************************************************
static void ResetLog( void );
static void WDTInit( void );
static void TaskInit( void *pvParameters );

//*************************************************************************************************
// MAIN ....
//*************************************************************************************************
int main( void ) {

    NVIC_SetPriorityGrouping( 3 );

    #if !defined( HMI_RELEASE ) && defined( RTE_Compiler_EventRecorder )
    EventRecorderInitialize( EventRecordAll, 1U );
    EventRecorderEnable( EventRecordError, 0xF0U, 0xF8U ); //RTOS Events
    EventRecorderEnable( EventRecordAll, 0xF2U, 0xF2U );   //Thread Events
    #endif

    osKernelInitialize();
    //создаем задачу инициализации
    osThreadNew( TaskInit, NULL, &init_attr );
    //стартуем OS
    if ( osKernelGetState() == osKernelReady )
        osKernelStart();
    for ( ;; );
 }

//*************************************************************************************************
// Задача инициализации 
//*************************************************************************************************
static void TaskInit( void *pvParameters ) {

    WDTInit();          //включим WDT
    UARTInit();         //UART консоли
    EepromInit();       //инициализация EEPROM, загрузка параметров настроек
    PortsInit();        //порты управления/состояния
    RTCInit();          //часы реального времени RTC
    SDInit();           //монтирование SD карты
    ResetLog();         //логирование источника сброса контроллера
    RamDiskInit();      //инициализация электронного (RAM) диска
    CommandInit();      //командный интерфейс
    CANInit();          //инициализация CAN интерфейса
    LogInit();          //инициализация логирования данных уст-в
    NetInit();          //инициализация сетевого интерфейса и сервисов
    JobInit();          //планировщик заданий
    LCDInit();          //инициализация графического контроллера
    WindowInit();       //Инициализация графической библиотеки

    osThreadTerminate( osThreadGetId() );
 }

//*********************************************************************************************
// Инициализация WDT
//*********************************************************************************************
static void WDTInit( void ) {

    #if defined( HMI_RELEASE )
    WWDT_Init( WDT_TIMEOUT );
    WWDT_Enable( ENABLE );
    WWDT_SetMode( WWDT_RESET_MODE, ENABLE );
    WWDT_Start( WDT_TIMEOUT );
    WWDT_Feed();
    #endif
 }

//*********************************************************************************************
// Перезапуск WDT
//*********************************************************************************************
void WDTReset( void ) {

    #if defined( HMI_RELEASE )
    WWDT_FeedStdSeq();
    #endif
 }

//*************************************************************************************************
// Сохраним в файл флаги источника сброса процессора
//*************************************************************************************************
static void ResetLog( void ) {

    FILE *log;

    log = fopen( "startup.log", "a" );
    if ( log == NULL )
        return;
    fprintf( log, "%s ", RTCGetDateTime( NULL, DOW_ENG_LANG ) );
    if ( reset & RESET_POR )
        fprintf( log, "POR " );
    if ( reset & RESET_EXTR )
        fprintf( log, "EXTR " );
    if ( reset & RESET_WDTR )
        fprintf( log, "WDTR " );
    if ( reset & RESET_BODR )
        fprintf( log, "BODR " );
    if ( reset & RESET_SYSRESET )
        fprintf( log, "SYSRESET " );
    if ( reset & RESET_LOCKUP )
        fprintf( log, "LOCKUP " );
    fprintf( log, "\r\n" );
    fclose( log );
 }
