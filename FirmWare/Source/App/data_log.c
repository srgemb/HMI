
//*************************************************************************************************
//
// Логирование данных (состояния) устройств
//
//*************************************************************************************************

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "lpc_rtc.h"

#include "cmsis_os2.h"
#include "rtx_config.h"

#include "device.h"
#include "dev_data.h"
#include "dev_param.h"
#include "can_data.h"
#include "can_def.h"

#include "can.h"
#include "rtc.h"
#include "main.h"
#include "data.h"
#include "sdcard.h"
#include "trc_calc.h"
#include "events.h"
#include "message.h"

//*************************************************************************************************
// Переменные с внешним доступом
//*************************************************************************************************
osMessageQueueId_t log_msg = NULL;
osEventFlagsId_t log_event = NULL;

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
static osTimerId_t timer1, timer2, timer3, timer4, timer5;

//*************************************************************************************************
// Атрибуты объектов RTOS
//*************************************************************************************************
static const osThreadAttr_t log1_attr = {
    .name = "DataLog", 
    .stack_size = 1024,
    .priority = osPriorityNormal
 };

static const osThreadAttr_t log2_attr = {
    .name = "EventLog", 
    .stack_size = 1024,
    .priority = osPriorityNormal
 };

static const osMessageQueueAttr_t que_attr = { .name = "EventLog" };
static const osEventFlagsAttr_t evn_attr = { .name = "DataLog" };
static const osTimerAttr_t timer1_attr = { .name = "LogBatmon" };
static const osTimerAttr_t timer2_attr = { .name = "LogMppt" };
static const osTimerAttr_t timer3_attr = { .name = "LogCharge" };
static const osTimerAttr_t timer4_attr = { .name = "LogInv" };
static const osTimerAttr_t timer5_attr = { .name = "LogTrc" };

//*************************************************************************************************
// Прототипы локальных функций
//*************************************************************************************************
static void BatmonLog( void );
static void DayLog( void );
static void MpptLog( void );
static void ChargeLog( void );
static void InvLog( void );
static void TrcLog( void );
static void GenLog( void );
static void Timer1Callback( void *arg );
static void Timer2Callback( void *arg );
static void Timer3Callback( void *arg );
static void Timer4Callback( void *arg );
static void Timer5Callback( void *arg );
static void TaskLog( void *pvParameters );
static void TaskEvent( void *pvParameters );
static uint16_t BatMonTTG( BatMonTTGTime type );
static void Logging( char *dir, char *prefix, char *msg );

//*************************************************************************************************
// Инициализация таймеров и задачи логирования данных
//*************************************************************************************************
void LogInit( void ) {

    timer1 = osTimerNew( Timer1Callback, osTimerOnce, NULL, &timer1_attr );
    timer2 = osTimerNew( Timer2Callback, osTimerOnce, NULL, &timer2_attr );
    timer3 = osTimerNew( Timer3Callback, osTimerOnce, NULL, &timer3_attr );
    timer4 = osTimerNew( Timer4Callback, osTimerOnce, NULL, &timer4_attr );
    timer5 = osTimerNew( Timer5Callback, osTimerOnce, NULL, &timer5_attr );
    //очередь сообщений
    log_event = osEventFlagsNew( &evn_attr );
    log_msg = osMessageQueueNew( 16, sizeof( MSGQUEUE_LOG ), &que_attr );
    //создаем задачу
    osThreadNew( TaskLog, NULL, &log1_attr );
    osThreadNew( TaskEvent, NULL, &log2_attr );
 }

//*************************************************************************************************
// Задача обработки событий логирования данных
//*************************************************************************************************
static void TaskLog( void *pvParameters ) {
    
    int32_t event;

    for ( ;; ) {
        //запуск таймера интервальной записи данных
        if ( !osTimerIsRunning( timer1 ) )
            osTimerStart( timer1, config.datlog_upd_bmon * SEC_TO_TICK );
        if ( !osTimerIsRunning( timer2 ) )
            osTimerStart( timer2, config.datlog_upd_mppt * SEC_TO_TICK );
        if ( !osTimerIsRunning( timer3 ) )
            osTimerStart( timer3, config.datlog_upd_chrg * SEC_TO_TICK );
        if ( !osTimerIsRunning( timer4 ) )
            osTimerStart( timer4, config.datlog_upd_inv * SEC_TO_TICK );
        if ( !osTimerIsRunning( timer5 ) )
            osTimerStart( timer5, config.datlog_upd_trc * SEC_TO_TICK );
        //ждем события, но не более 1 ms, т.к. необходимо перезапустить таймеры
        event = osEventFlagsWait( log_event, EVN_TIMER_MASK, osFlagsWaitAny, 1 );
        if ( event != osErrorTimeout ) {
            if ( event & EVN_RTC_SECONDS )
                DayLog(); //логирование суточных данных BATMON
            if ( event & EVN_TIMER_BATMON ) {
                BatmonLog(); //интервальное сохранение данных
                osTimerStart( timer1, config.datlog_upd_bmon * SEC_TO_TICK );
               }
            if ( event & EVN_TIMER_MPPT ) {
                MpptLog(); //интервальное сохранение данных
                osTimerStart( timer2, config.datlog_upd_mppt * SEC_TO_TICK );
               }
            if ( event & EVN_TIMER_CHARGE ) {
                ChargeLog(); //интервальное сохранение данных
                osTimerStart( timer3, config.datlog_upd_chrg * SEC_TO_TICK );
               }
            if ( event & EVN_TIMER_INV ) {
                InvLog(); //интервальное сохранение данных
                osTimerStart( timer4, config.datlog_upd_inv * SEC_TO_TICK );
               }
            if ( event & EVN_TIMER_TRC ) {
                TrcLog(); //интервальное сохранение данных
                osTimerStart( timer5, config.datlog_upd_trc * SEC_TO_TICK );
               }
           }
        #if ( OS_ROBIN_ENABLE == 0 )
        osThreadYield();
        #endif
       }
 }

//*************************************************************************************************
// Задача обработки логирования событий
//*************************************************************************************************
static void TaskEvent( void *pvParameters ) {
    
    osStatus_t status;
    MSGQUEUE_LOG mess;

    for ( ;; ) {
        status = osMessageQueueGet( log_msg, &mess, NULL, osWaitForever );
        if ( status == osOK ) {
            if ( mess.dev_id == ID_DEV_ALT )
                Logging( "alt", "alt", MessageLog( mess.id_mess ) );
            if ( mess.dev_id == ID_DEV_CHARGER )
                Logging( "charger", "pb", MessageLog( mess.id_mess ) );
            if ( mess.dev_id == ID_DEV_INV1 )
                Logging( "inv", "inv1", MessageLog( mess.id_mess ) );
            if ( mess.dev_id == ID_DEV_INV2 )
                Logging( "inv", "inv2", MessageLog( mess.id_mess ) );
            if ( mess.dev_id == ID_DEV_PV )
                Logging( "mppt", "pv", MessageLog( mess.id_mess ) );
            if ( mess.dev_id == ID_DEV_TRC && mess.id_mess )
                Logging( "trc", "trc", MessageLog( mess.id_mess ) );
            if ( mess.dev_id == ID_DEV_TRC && !mess.id_mess )
                TrcLog(); //логирование команд управления трекером
            if ( mess.dev_id == ID_DEV_GEN )
                GenLog();
           }
        #if ( OS_ROBIN_ENABLE == 0 )
        osThreadYield();
        #endif
       }
 }

//*************************************************************************************************
// Функция обратного вызова таймера - логирование данных монитора АКБ
//*************************************************************************************************
static void Timer1Callback( void *arg ) {

    osEventFlagsSet( log_event, EVN_TIMER_BATMON );
 }

//*************************************************************************************************
// Функция обратного вызова таймера - логирование данных контроллера MPPT
//*************************************************************************************************
static void Timer2Callback( void *arg ) {

    osEventFlagsSet( log_event, EVN_TIMER_MPPT );
 }

//*************************************************************************************************
// Функция обратного вызова таймера - логирование данных контроллера заряда
//*************************************************************************************************
static void Timer3Callback( void *arg ) {

    osEventFlagsSet( log_event, EVN_TIMER_CHARGE );
 }

//*************************************************************************************************
// Функция обратного вызова таймера - логирование данных инверторов
//*************************************************************************************************
static void Timer4Callback( void *arg ) {

    osEventFlagsSet( log_event, EVN_TIMER_INV );
 }

//*************************************************************************************************
// Функция обратного вызова таймера - логирование данных трекера
//*************************************************************************************************
static void Timer5Callback( void *arg ) {

    osEventFlagsSet( log_event, EVN_TIMER_TRC );
 }

//*************************************************************************************************
// Добавляет в протокол "bm_yyyymmdd.csv" данные монитора АКБ
// Логирование ведется при включенном параметре "log_enable_bmon" 
// с интервалом времени по параметру "datlog_upd_bmon"
//*************************************************************************************************
static void BatmonLog( void ) {

    uint32_t pos;
    char name[80];
    FILE *bm_log;

    if ( batmon.link == LINK_CONN_NO )
        return; //данных нет
    if ( !config.log_enable_bmon )
        return; //логирование выключено
    if ( SDStatus() == ERROR )
        return; //карты нет
    //открываем файл
    if ( !config.mode_logging )
        sprintf( name, "\\batmon\\bm_%s.csv", RTCFileName() );
    else sprintf( name, "\\batmon\\%s\\bm_%s.csv", RTCFileShort(), RTCFileName() );
    bm_log = fopen( name, "a" );
    if ( bm_log == NULL )
        return; //файл не открылся
    pos = ftell( bm_log );
    //запишем наименование полей
    if ( !pos )
        fprintf( bm_log, "Date;Time;Bat_V(V);Bat_I(A);Energy from BAT(Ah);SOC(%%);TTGo;Total energy from BAT(Ah);Alarm;Relay;Last discharge(Ah);Medium discharge(Ah)\r\n" );
    //запишем данные
    fprintf( bm_log, "%s;%s;%.2f;%+.2f;%+.2f;%.1f;%3d:%02d;%.2f;%u;%u;%5.2f;%5.2f\r\n", RTCGetDate( NULL ), RTCGetTime( NULL ), 
            batmon.voltage, batmon.current, batmon.cons_energy, batmon.soc, BatMonTTG( BATMON_TTG_HOUR ), BatMonTTG( BATMON_TTG_MIN ), batmon.h6,
            batmon.alarm, batmon.relay, batmon.h2, batmon.h3 ); 
    fclose( bm_log );
 }

//*************************************************************************************************
// Добавляет в протокол "bm_yyyymm.csv" суточные данные монитора АКБ
//*************************************************************************************************
static void DayLog( void ) {

    uint32_t pos;
    char name[80];
    FILE *time_log;
    RTC_TIME_Type rtc;

    if ( !config.log_enable_bmon )
        return; //логирование выключено
    if ( SDStatus() == ERROR )
        return; //карты нет
    RTC_GetFullTime( LPC_RTC, &rtc );
    if ( rtc.HOUR != 23 || rtc.MIN != 59 || rtc.SEC != 59 )
        return;
    //формируем имя файла
    sprintf( name, "\\batmon\\bm_%s.csv", RTCFileShort() );
    time_log = fopen( name, "a" );
    if ( time_log == NULL )
        return; //файл не открылся
    pos = ftell( time_log );
    if ( !pos )
        fprintf( time_log, "Date;Bat_V(V);Energy(Ah);SOC(%%);H6(Ah)\r\n" ); //запишем наименование полей
    //запишем данные
    fprintf( time_log, "%s;%.2f;%+.2f;%.1f;%.2f\r\n", RTCGetDate( NULL ), batmon.voltage, batmon.cons_energy, batmon.soc, batmon.h6 ); 
    fclose( time_log );
 }

//*************************************************************************************************
// Возвращает значения параметра TTG, продолжительность работы в часах/минутах
// BatMonTTGTime type - тип результата: часы/минуты 
// return uint16_t    - значение параметра
//*************************************************************************************************
static uint16_t BatMonTTG( BatMonTTGTime type ) {

    if ( type == BATMON_TTG_MIN )
        return batmon.ttg%60;
    if ( type == BATMON_TTG_HOUR )
        return (uint16_t)batmon.ttg/60;
    return 0;
 }

//*************************************************************************************************
// Добавляет в протокол "mppt_yyyymmdd.csv" данные контроллера заряда MPPT
//*************************************************************************************************
static void MpptLog( void ) {

    uint32_t pos;
    FILE *mppt_log;
    char name_csv[80];

    if ( mppt.link == LINK_CONN_NO )
        return; //данных нет
    if ( !config.log_enable_mppt )
        return; //логирование выключено
    if ( SDStatus() == ERROR )
        return; //карты нет
    //открываем файл для записи данных
    if ( !config.mode_logging )
        sprintf( name_csv, "\\mppt\\mppt_%s.csv", RTCFileName() );
    else sprintf( name_csv, "\\mppt\\%s\\mppt_%s.csv", RTCFileShort(), RTCFileName() );
    mppt_log = fopen( name_csv, "a" );
    if ( mppt_log == NULL )
        return; //файл не открылся
    pos = ftell( mppt_log );
    if ( !pos )
        fprintf( mppt_log, "Date;Time;PV_V(V);PV_I(A);OUT_V(V);OUT_I(A);WHr;AHr;Float;ModeCharge;SOC(%%);Bat_I(A);PVOn;PVMode\r\n" ); //запишем наименование полей
    //запишем данные
    fprintf( mppt_log, "%s;%s;%.1f;%.1f;%.1f;%.1f;%d;%d;%d;%s;%03d;%+.1f;%s;%s\r\n", RTCGetDate( NULL ), RTCGetTime( NULL ),
        mppt.u01_in_voltage, mppt.u02_in_current, mppt.u03_out_voltage, mppt.u04_out_current, mppt.u05_energy1,
        mppt.u05_energy2, mppt.u07_time_flt, ParamGetDesc( ID_DEV_MPPT, MPPT_CHARGE_MODE ),
        mppt.u12_soc, mppt.u13_bat_current, ParamGetDesc( ID_DEV_MPPT, MPPT_PVON ), ParamGetDesc( ID_DEV_MPPT, MPPT_PVMODE ) ); 
    fclose( mppt_log );
 }

//*************************************************************************************************
// Добавляет в протокол "pb_yyyymmdd.csv" данные заряда
//*************************************************************************************************
static void ChargeLog( void ) {

    uint32_t pos;
    char name[80];
    FILE *bp_log;

    if ( !config.log_enable_chrg )
        return; //логирование выключено
    if ( SDStatus() == ERROR )
        return; //карты нет
    if ( charger.charge_mode == CHARGE_OFF )
        return; //зарядка выкл
    if ( !config.mode_logging )
        sprintf( name, "\\charger\\pb_%s.csv", RTCFileName() );
    else sprintf( name, "\\charger\\%s\\pb_%s.csv", RTCFileShort(), RTCFileName() );
    bp_log = fopen( name, "a" );
    if ( bp_log == NULL )
        return; //файл не открылся
    pos = ftell( bp_log );
    if ( !pos )
        fprintf( bp_log, "Date;Time;AC;Dev;Mode;Stat;SOC(%%);I(A);V\r\n" ); //запишем наименование полей
    //запишем данные
    fprintf( bp_log, "%s;%s;%s;%s;%d;%s;%.1f;%4.1f;%.2f\r\n", RTCGetDate( NULL ), RTCGetTime( NULL ),
        ParamGetDesc( ID_DEV_CHARGER, CHARGE_CONN_AC ),
        ParamGetDesc( ID_DEV_CHARGER, CHARGE_DEV_STAT ),
        charger.charge_mode, 
        ParamGetDesc( ID_DEV_CHARGER, CHARGE_BANK_STAT ),
        batmon.soc, charger.current, batmon.voltage );
    fclose( bp_log );
 }

//*************************************************************************************************
// Пишет в протокол "inv_yyyymmdd.csv" данные по обоим инверторам
// Вызов из TSCheckAnswer после разбор данных
//*************************************************************************************************
static void InvLog( void ) {

    uint32_t pos;
    char name[40];
    FILE *ts_log;

    if ( !config.log_enable_inv )
        return; //логирование выключено
    if ( SDStatus() == ERROR )
        return; //карты нет
    if ( inv1.mode != INV_MODE_ON && inv2.mode != INV_MODE_ON )
        return; //оба инвертора выключены
    //открываем файл
    if ( !config.mode_logging )
        sprintf( name, "\\inv\\inv_%s.csv", RTCFileName() );
    else sprintf( name, "\\inv\\%s\\inv_%s.csv", RTCFileShort(), RTCFileName() );
    ts_log = fopen( name, "a" );
    if ( ts_log == NULL )
        return; //файл не открылся
    pos = ftell( ts_log );
    if ( !pos )
        fputs( "Date;Time;Pwr1(%);Pwr1(W);Temp1(C);Conn1;Mode1;Error1;Pwr3(%);Pwr3(W);Temp3(C);Conn3;Mode3;Error3;\r\n", ts_log ); //запишем наименование полей
    //запишем данные
    fprintf( ts_log, "%s;%s;%d;%d;%4.1f;%s;%s;%s;%d;%d;%4.1f;%s;%s;%s\r\n", 
        RTCGetDate( NULL ), RTCGetTime( NULL ), 
        inv1.power_perc, inv1.power_watt, inv1.temperature, 
        inv1.dc_conn == INV_CTRL_ON ? "Да " : "Нет",
        ParamGetDesc( ID_DEV_INV1, INV_MODE ),
        ErrorDescr( ID_DEV_INV1, inv1.dev_error, 0 ),
        inv2.power_perc, inv2.power_watt, inv2.temperature, 
        inv2.dc_conn == INV_CTRL_ON ? "Да " : "Нет", 
        ParamGetDesc( ID_DEV_INV2, INV_MODE ),
        ErrorDescr( ID_DEV_INV2, inv2.dev_error, 0 ) ); 
    fclose( ts_log );
 }

//*************************************************************************************************
// Логирование состояние трекера в файл: trc_YYYYMMDD.log
//*************************************************************************************************
static void TrcLog( void ) {

    char name[64];
    FILE *trc_log;

    if ( !config.log_enable_trc )
        return; //логирование выключено
    if ( SDStatus() == ERROR )
        return; //карты нет
    if ( !tracker.link ) 
        return; //трекер не подключен по интерфейсу RS-485
    if ( !config.mode_logging )
        sprintf( name, "\\trc\\trc_%s.log", RTCFileName() );
    else sprintf( name, "\\trc\\%s\\trc_%s.log", RTCFileShort(), RTCFileName() );
    //пишем в конец файла
    trc_log = fopen( name, "a" );
    if ( trc_log == NULL )
        return; //файл не открылся
    fprintf( trc_log, "%s STAT: 0x%04X %s VER=%03d/%.1f° HRZ=%03d/%.1f°\r\n", RTCGetLog(), tracker.stat, 
        ParamGetDesc( ID_DEV_TRC, TRC_MODE2 ), tracker.act_pos_vert, AngleVert( tracker.act_pos_vert ), 
        tracker.act_pos_horz, AngleHorz( tracker.act_pos_horz ) ); 
    fclose( trc_log );
 }

//*************************************************************************************************
// Логирование команд и состояния генератора в протокол
//*************************************************************************************************
static void GenLog( void ) {

    FILE *log;
    char name[40], prnval[40];
    
    if ( !config.log_enable_gen )
        return; //логирование выключено
    if ( SDStatus() == ERROR )
        return; //карты нет
    if ( !config.mode_logging )
        sprintf( name, "\\gen\\gen_%s.log", RTCFileName() );
    else sprintf( name, "\\gen\\%s\\gen_%s.log", RTCFileShort(), RTCFileName() );
    log = fopen( name, "a" );
    if ( log == NULL )
        return;
    if ( gen_ptr->stat == GEN_STAT_STEP_START ) //только для одного параметра подставляем значения
        sprintf( prnval, ParamGetDesc( ID_DEV_GEN, GEN_PAR_STAT ), gen_ptr->cycle1 + 1, gen_ptr->cycle2 ); 
    else sprintf( prnval, "%s", ParamGetDesc( ID_DEV_GEN, GEN_PAR_STAT ) );
    if ( gen_ptr->error )
        fprintf( log, "%s %s %s\r\n", RTCGetLog(), ParamGetDesc( ID_DEV_GEN, GEN_PAR_MODE ), ErrorDescr( ID_DEV_GEN, gen_ptr->error, 0 ) );
    else fprintf( log, "%s %s %s\r\n", RTCGetLog(), ParamGetDesc( ID_DEV_GEN, GEN_PAR_MODE ), prnval );
    fclose( log );
 }

//*************************************************************************************************
// Запись в протокол текстовой строки
// char *dir,   - имя каталога для записи в файл
// char *prefix - префикс имени файла
// char *msg    - текстовая строка добавляемая в файл протокола
//*************************************************************************************************
static void Logging( char *dir, char *prefix, char *msg ) {

    FILE *log;
    char name[80];
    
    if ( SDStatus() == ERROR )
        return; //карты нет
    if ( !config.log_enable_alt )
        return; //логирование выключено
    //формируем имя файла
    if ( !config.mode_logging )
        sprintf( name, "\\%s\\%s_%s.log", dir, prefix, RTCFileName() );
    else sprintf( name, "\\%s\\%s\\%s_%s.log", dir, RTCFileShort(), prefix, RTCFileName() );
    log = fopen( name, "a" );
    if ( log == NULL )
        return;
    //запись в протокол текстовой строки
    fprintf( log, "%s %s\r\n", RTCGetLog(), msg );
    fclose( log );
 }

