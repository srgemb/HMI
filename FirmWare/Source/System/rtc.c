
//*************************************************************************************************
//
// Управление часами реального времени
//
//*************************************************************************************************

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "lpc_types.h"
#include "lpc_rtc.h"
#include "cmsis_os2.h"
#include "rtx_config.h"

#include "dev_data.h"

#include "rtc.h"
#include "can.h"
#include "sdcard.h"
#include "events.h"

//*************************************************************************************************
// Локальные константы
//*************************************************************************************************
#define BUFFER              40          //размер буфера для формирования результата одного параметра     
#define RTC_SYNC_LOCK       5           //время блокировки автоматической синхронизации часов (sec)

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
static uint8_t sync_rtc = 0;
static osEventFlagsId_t rtc_event = NULL;
static char str_time[40], str_date1[40], str_date2[40], str_date3[40], str_date4[40];
static char date_time1[40], date_time2[40];

static char * const dows_en_short[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", NULL };
static char * const dows_ru_short[] = { "Вск", "Пон", "Втр", "Срд", "Чтв", "Птн", "Суб", NULL };

#ifdef ENGLISH_LANG
    static char * const dows_long[] =  { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
    static char * const month_short[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", NULL };
    const static char month_long[12][10] = { 
        "January", "February", "March", "April", "May", "June", 
        "July", "August", "September", "October", "November", "December" 
       };
#endif

#ifdef RUSSIAN_LANG
    static char * const month_short[] = { 
        "Янв", "Фев", "Мрт", "Апр", "Май", "Июн", 
        "Июл", "Авг", "Сен", "Окт", "Ноб", "Дек", NULL
       };
    static char * constdows_long[] = { "Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота" };
    const static char month_long[12][10] = { 
        "Январь", "Феврал", "Март", "Апрель", "Май", "Июнь", 
        "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь" 
       };
#endif

//*************************************************************************************************
// Прототипы локальных функций
//*************************************************************************************************
static void TaskRtc( void *pvParameters );
static void SyncDateTimeLog( char *new_val, char *old_val );

//*************************************************************************************************
// Атрибуты объектов RTOS
//*************************************************************************************************
static const osThreadAttr_t rtc_attr = {
    .name = "Rtc",
    .stack_size = 768,
    .priority = osPriorityNormal
 };
         
static const osEventFlagsAttr_t evn_attr = { .name = "Rtc" };

//*********************************************************************************************
// Инициализация часов
//*********************************************************************************************
void RTCInit( void ) {

    //создаем флаг события
    rtc_event = osEventFlagsNew( &evn_attr );
    //создаем задачу
    osThreadNew( TaskRtc, NULL, &rtc_attr );
    //инициализация часов
    RTC_Init( LPC_RTC );
    //включение обработки прерывания
    NVIC_SetPriority( RTC_IRQn, 17 );
    NVIC_EnableIRQ( RTC_IRQn );
    //вкл часов
    RTC_Cmd( LPC_RTC, ENABLE );
    RTC_CntIncrIntConfig( LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE );
 }

//*************************************************************************************************
// Задача обработки сообщения от прерывания RTC
//*************************************************************************************************
static void TaskRtc( void *pvParameters ) {

    RTC rtc_hmi;
    int32_t sec_ctrl, sec_hmi;
    RTC_TIME_Type rtc_local;
    char new_val[16], old_val[16];
    
    for ( ;; ) {
        //ждем прерывания от часов RTC
        osEventFlagsWait( rtc_event, EVN_RTC_IRQ, osFlagsWaitAll, osWaitForever );
        if ( job_event != NULL )
            osEventFlagsSet( job_event, EVN_RTC_SECONDS ); //обработка заданий планировщика
        if ( log_event != NULL )
            osEventFlagsSet( log_event, EVN_RTC_SECONDS ); //логирование состояния BATMON
        RTCHmi( &rtc_hmi );
        if ( sync_rtc )
            sync_rtc--; //разблокировка автоматической синхронизации даты/время
        if ( LinkStat() != CAN_LINK_OK )
            continue;
        //дата/время локальных часов
        RTC_GetFullTime( LPC_RTC, &rtc_local );
        //проверка необходимости синхронизации даты/времени
        if ( !sync_rtc && ( rtc.day != rtc_local.DOM || rtc.month != rtc_local.MONTH || rtc.year != rtc_local.YEAR ) ) {
            if ( rtc.day && rtc.month && rtc.year ) {
                //синхронизация даты "Управляющий контроллер" -> "HMI"
                sprintf( new_val, "%02d.%02d.%04d", rtc.day, rtc.month, rtc.year );
                sprintf( old_val, "%02d.%02d.%04d", rtc_local.DOM, rtc_local.MONTH, rtc_local.YEAR );
                SyncDateTimeLog( new_val, old_val );
                RTCSetDate( new_val, SET_MODE_LOCAL );
               }
           }
        //автоматическую синхронизацию часов выполняем при расхождении более 3 секунд
        sec_hmi = rtc_local.HOUR * 3600 + rtc_local.MIN * 60 + rtc_local.SEC;
        sec_ctrl = rtc.hour * 3600 + rtc.min * 60 + rtc.sec;
        if ( !sync_rtc && sec_ctrl && abs( sec_hmi - sec_ctrl ) > 3 ) {
            if ( rtc.hour && rtc.min && rtc.sec ) {
                //синхронизация времени "Управляющий контроллер" -> "HMI"
                sprintf( new_val, "%02d:%02d:%02d", rtc.hour, rtc.min, rtc.sec );
                sprintf( old_val, "%02d:%02d:%02d", rtc_local.HOUR, rtc_local.MIN, rtc_local.SEC );
                SyncDateTimeLog( new_val, old_val );
                RTCSetTime( new_val, SET_MODE_LOCAL );
               }
           }
        #if ( OS_ROBIN_ENABLE == 0 )
        osThreadYield();
        #endif
       } 
 }

//*********************************************************************************************
// Обработка секундного прерывания часов
//*********************************************************************************************
void RTC_IRQHandler( void ) {

    if ( RTC_GetIntPending( LPC_RTC, RTC_INT_COUNTER_INCREASE ) == SET )
        RTC_ClearIntPending( LPC_RTC, RTC_INT_COUNTER_INCREASE );
    //передаем событие в задачу TaskRtc()
    osEventFlagsSet( rtc_event, EVN_RTC_IRQ );
 }
 
//*********************************************************************************************
// Заполняет структуру rtc_hmi текущими значениями дата/время
//*********************************************************************************************
void RTCHmi( RTC *rtc_hmi ) {

    RTC_TIME_Type rtc_local;

    //дата/время локальных часов
    RTC_GetFullTime( LPC_RTC, &rtc_local );
    rtc_hmi->hour = rtc_local.HOUR;
    rtc_hmi->min = rtc_local.MIN;
    rtc_hmi->sec = rtc_local.SEC;
    rtc_hmi->day = rtc_local.DOM;
    rtc_hmi->month = rtc_local.MONTH;
    rtc_hmi->year = rtc_local.YEAR;
    rtc_hmi->dow = rtc_local.DOW;
 }

//*********************************************************************************************
// Возвращает время в формате HH:MI:SS
// return - строка с временем в формате HH:MI:SS
//*********************************************************************************************
char *RTCGetTime( char *endstr ) {

    RTC_TIME_Type rtc;
    
    RTC_GetFullTime( LPC_RTC, &rtc );
    if ( endstr != NULL )
        sprintf( str_time, "%02d:%02d:%02d%s", rtc.HOUR, rtc.MIN, rtc.SEC, endstr );  
    else sprintf( str_time, "%02d:%02d:%02d", rtc.HOUR, rtc.MIN, rtc.SEC );  
    return str_time;
 }

//*********************************************************************************************
// Возвращает дату в формате DD.MM.YYYY
// return - строка с датой в формате DD.MM.YYYY
//*********************************************************************************************
char *RTCGetDate( char *endstr ) {

    RTC_TIME_Type rtc;
    
    RTC_GetFullTime( LPC_RTC, &rtc );
    if ( endstr != NULL )
        sprintf( str_date1, "%02d.%02d.%04d%s", rtc.DOM, rtc.MONTH, rtc.YEAR, endstr );  
    else sprintf( str_date1, "%02d.%02d.%04d", rtc.DOM, rtc.MONTH, rtc.YEAR );  
    return str_date1;
 }

//*********************************************************************************************
// Возвращает дату в формате DOW DD.MM.YYYY
// return - строка с датой в формате DOW DD.MM.YYYY
//*********************************************************************************************
char *RTCGetDowDate( char *endstr, RtcDowLang rus_eng ) {

    char *dows = ""; 
    RTC_TIME_Type rtc;
    
    RTC_GetFullTime( LPC_RTC, &rtc );
    if ( rus_eng == DOW_ENG_LANG )
        dows = dows_en_short[rtc.DOW];
    if ( rus_eng == DOW_RUS_LANG )
        dows = dows_ru_short[rtc.DOW];
    if ( endstr != NULL )
        sprintf( str_date2, "%s %02d.%02d.%04d%s", dows, rtc.DOM, rtc.MONTH, rtc.YEAR, endstr );  
    else sprintf( str_date2, "%s %02d.%02d.%04d", dows, rtc.DOM, rtc.MONTH, rtc.YEAR );  
    return str_date2;
 }

//*********************************************************************************************
// Возвращает дату в формате YYYYDDMM для формирования имени файла протокола
// return - строка с датой в формате YYYYDDMM
//*********************************************************************************************
char *RTCFileName( void ) {

    RTC_TIME_Type rtc;
    
    RTC_GetFullTime( LPC_RTC, &rtc );
    sprintf( str_date3, "%04d%02d%02d", rtc.YEAR, rtc.MONTH, rtc.DOM );  
    return str_date3;
 }

//*********************************************************************************************
// Возвращает дату/время в полном формате DD.MM.YYYY Week HH.MI.SS
// char *endstr - строка для добавления к результату
// return       - строка с датой и временем в формате DD.MM.YYYY Week HH:MI:SS
//*********************************************************************************************
char *RTCGetDateTime( char *endstr, RtcDowLang rus_eng ) {

    char *dows = ""; 
    RTC_TIME_Type rtc;
    
    RTC_GetFullTime( LPC_RTC, &rtc );
    if ( rus_eng == DOW_ENG_LANG )
        dows = dows_en_short[rtc.DOW];
    if ( rus_eng == DOW_RUS_LANG )
        dows = dows_ru_short[rtc.DOW];
    if ( endstr != NULL )
        sprintf( date_time1, "%02d.%02d.%04d %s %02d:%02d:%02d %s", rtc.DOM, rtc.MONTH, rtc.YEAR, dows, rtc.HOUR, rtc.MIN, rtc.SEC, endstr );
    else sprintf( date_time1, "%02d.%02d.%04d %s %02d:%02d:%02d", rtc.DOM, rtc.MONTH, rtc.YEAR, dows, rtc.HOUR, rtc.MIN, rtc.SEC );
    return date_time1;
 }

//*********************************************************************************************
// Возвращает дату/время в формате DD.MM.YYYY HH.MI.SS для записи в протокол
// return - строка с датой и временем в формате DD.MM.YYYY HH.MI.SS
//*********************************************************************************************
char *RTCGetLog( void ) {

    RTC_TIME_Type rtc;
    
    RTC_GetFullTime( LPC_RTC, &rtc );
    sprintf( date_time2, "%02d.%02d.%04d %02d:%02d:%02d", rtc.DOM, rtc.MONTH, rtc.YEAR, rtc.HOUR, rtc.MIN, rtc.SEC );
    return date_time2;
 }

//*************************************************************************************************
// Возвращает дату в формате YYYYMM для формирования имени файла протокола
//*************************************************************************************************
char *RTCFileShort( void ) {

    RTC_TIME_Type rtc;
    
    RTC_GetFullTime( LPC_RTC, &rtc );
    sprintf( str_date4, "%04d%02d", rtc.YEAR, rtc.MONTH );  
    return str_date4;
 }

//*********************************************************************************************
// Установка времени, входной формат HH:MI:SS
// char *param      - строка с временем в формате HH:MI:SS
//                    допустимые значения: HH:MI:SS 00-23:00-59:00-59
// RtcSetMode mode  - режим установки: HMI/управляющий контроллер
// return = SUCCESS - параметры указаны верно, время изменено
//          ERROR   - ошибка в формате или неправильное значение
//*********************************************************************************************
Status RTCSetTime( char *param, RtcSetMode mode ) {

    RTC rtc_hmi;
    uint8_t hour, min, sec, idx, chk = 0;
    char *mask, mask1[] = "NN:NN:NN", mask2[] = "NN:NN";

    if ( strlen( param ) == strlen( mask1 ) )
        mask = mask1;
    if ( strlen( param ) == strlen( mask2 ) )
        mask = mask2;
    //проверка формата
    for ( idx = 0; idx < strlen( mask ); idx++ ) {
        if ( mask[idx] == 'N' && isdigit( *(param+idx) ) )
            chk++;
        if ( mask[idx] == ':' && ispunct( *(param+idx) ) )
            chk++;
       } 
    if ( chk == strlen( mask1 ) ) {
        //полный формат "NN:NN:NN"
        //проверка значений
        hour = atoi( param );
        min = atoi( param + 3 );
        sec = atoi( param + 6 );
        if ( hour > 23 || min > 59 || sec > 59 )
            return ERROR;
        sync_rtc = RTC_SYNC_LOCK; //блокируем автоматическую синхронизацию часов
        //установка времени
        RTC_SetTime( LPC_RTC, RTC_TIMETYPE_HOUR, hour );
        RTC_SetTime( LPC_RTC, RTC_TIMETYPE_MINUTE, min );
        RTC_SetTime( LPC_RTC, RTC_TIMETYPE_SECOND, sec );
        //установленные значения RTC
        RTCHmi( &rtc_hmi ); //дата/время локальных часов
        if ( mode == SET_MODE_REMOTE )
            SendCommand( ID_DEV_RTC, (uint8_t *)&rtc_hmi, sizeof( RTC ) ); //передача в контроллер
        return SUCCESS;
       }
    if ( chk == strlen( mask2 ) ) {
        //сокращенный формат "NN:NN"
        hour = atoi( param );
        min = atoi( param + 3 );
        if ( hour > 23 || min > 59 )
            return ERROR;
        sync_rtc = RTC_SYNC_LOCK; //блокируем автоматическую синхронизацию часов
        //установка времени
        RTC_SetTime( LPC_RTC, RTC_TIMETYPE_HOUR, hour );
        RTC_SetTime( LPC_RTC, RTC_TIMETYPE_MINUTE, min );
        //установленные значения RTC
        RTCHmi( &rtc_hmi ); //дата/время локальных часов
        if ( mode == SET_MODE_REMOTE )
            SendCommand( ID_DEV_RTC, (uint8_t *)&rtc_hmi, sizeof( RTC ) ); //передача в контроллер
        return SUCCESS;
       }
    return ERROR;
 }

//*********************************************************************************************
// Установка даты, входной формат DD.MM.YYYY
// char *param      - строка с датой в формате DD.MM.YYYY
//                    допустимые значения: DD.MM.YYYY 1-31.1-12.2000-2099
// RtcSetMode mode  - режим установки: HMI/управляющий контроллер
// return = SUCCESS - параметры указаны верно, время изменено
//          ERROR   - ошибка в формате или неправильное значение
//*********************************************************************************************
Status RTCSetDate( char *param, RtcSetMode mode ) {
 
    RTC rtc_hmi;
    uint8_t day, month;
    uint16_t year;
        
    if ( !CheckDate( param, &day, &month, &year ) )
        return ERROR;
    sync_rtc = RTC_SYNC_LOCK; //блокируем автоматическую синхронизацию часов
    RTC_SetTime( LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, day );
    RTC_SetTime( LPC_RTC, RTC_TIMETYPE_MONTH, month );
    RTC_SetTime( LPC_RTC, RTC_TIMETYPE_YEAR, year );
    RTC_SetTime( LPC_RTC, RTC_TIMETYPE_DAYOFWEEK, DayOfWeek( day, month, year ) );
    RTC_SetTime( LPC_RTC, RTC_TIMETYPE_DAYOFYEAR, DayOfYear( day, month, year ) );
    //установленные значения RTC
    RTCHmi( &rtc_hmi ); //дата/время локальных часов
    if ( mode == SET_MODE_REMOTE )
        SendCommand( ID_DEV_RTC, (uint8_t *)&rtc_hmi, sizeof( RTC ) ); //передача в контроллер
    return SUCCESS;
 }

//*********************************************************************************************
// Возвращает текущее время в формате десятичных долей часа: H.xxxxxx
// return = 12.36591 - результат в десятичных долях часа
//*********************************************************************************************
double GetTimeDecimal( void ) {

    double result;
    RTC_TIME_Type rtc;
    
    RTC_GetFullTime( LPC_RTC, &rtc );
    result = (double)rtc.SEC / 60.0;
    result = ( result + (double)rtc.MIN ) / 60.0;
    result += (double)rtc.HOUR;
    return result;
 }

//*********************************************************************************************
// Перевод времени (часы, минуты, секунды) в десятичные доли часа
// http://deep125.narod.ru/astra_calc/p7.htm
// uint8_t hour      - значение часов
// uint8_t minute    - значение минут
// uint8_t seconds   - значение секунд
// return = 12.36591 - результат в десятичных долях часа
//*********************************************************************************************
double TimeToDecimal( uint8_t hour, uint8_t minute, uint8_t seconds ) {

    double result;
    
    result = (double)seconds / 60.0;
    result = ( result + (double)minute ) / 60.0;
    result += (double)hour;
    return result;
 }

//*********************************************************************************************
// Перевод целых секунд в десятичные доли часа
// return = 12.36591 - результат в десятичных долях часа
//*********************************************************************************************
double SecToDecimal( uint16_t value ) {

    uint16_t hour, min, sec;
    
    hour = value / 3600; 
    sec = value % 3600;
    min = sec / 60;
    sec = sec % 60;
    return TimeToDecimal( hour, min, sec );
 }

//*********************************************************************************************
// Проверяет вхождения времени во временной интервал
// float time1      - начальное время
// float time       - проверяемое время
// float time2      - конечное время
// return = ERROR   - не входит в интервал
//          SUCCESS - входит в интервал
//*********************************************************************************************
Status TimeCheck( double time_beg, double time, double time_end ) {

    if ( time < time_beg )
        return ERROR;
    if ( time > time_end )
        return ERROR;
    return SUCCESS;
 }
 
//*********************************************************************************************
// Расчет дня недели по дате
// Все деления целочисленные (остаток отбрасывается).
// Результат: 0 — воскресенье, 1 — понедельник и т.д.
//*********************************************************************************************
uint16_t DayOfWeek( int day, int month, int year ) {
 
    int a, y, m;
    
    a = (14 - month) / 12;
    y = year - a;
    m = month + 12 * a - 2;
    return (7000 + (day + y + y / 4 - y / 100 + y / 400 + (31 * m) / 12)) % 7;
}
 
//*********************************************************************************************
// Расчет номера дня (1-365) года по дате
//*********************************************************************************************
uint16_t DayOfYear( int day, int month, int year ) {

    uint16_t i, leap;
    uint16_t day_tab[2][13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31,
                                0,31,29,31,30,31,30,31,31,30,31,30,31 };
    
    leap = ( year%400 ) == 0 || ( ( year%4 ) == 0 && ( year%100 ) != 0 );
    for ( i = 1; i < month; i++ ) 
        day += day_tab[leap][i];
    return day;
 }

//*********************************************************************************************
// Возвращает максимальный день месяца 
// int month - номер месяца
// int year  - год
//*********************************************************************************************
uint8_t DaysOfMonth( uint8_t month, uint16_t year ) {

    uint16_t leap;
    uint16_t day_tab[2][13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31,
                                0,31,29,31,30,31,30,31,31,30,31,30,31 };
    
    leap = ( year%400 ) == 0 || ( ( year%4 ) == 0 && ( year%100 ) != 0 );
    return day_tab[leap][month];
 }

//*********************************************************************************************
// Проверяет формат даты в "value" по маске DD.MM.YYYY (01-31.01-12.2000-2099)
// return = SUCCESS - формат соответствует маске, данные заносятся в переменные: day,month,year
//        = ERROR   - формат не соответствует маске
//*********************************************************************************************
Status CheckDate( char *value, uint8_t *day, uint8_t *month, uint16_t *year ) {

    uint16_t ch_year;
    char mask[] = "NN.NN.NNNN";
    uint8_t idx, ch_day, ch_mon, chk = 0;

    //проверка формата
    for ( idx = 0; idx < strlen( mask ); idx++ ) {
        if ( mask[idx] == 'N' && isdigit( *(value+idx) ) )
            chk++;
        if ( mask[idx] == '.' && ispunct( *(value+idx) ) )
            chk++;
       } 
    if ( chk != strlen( mask ) )
        return ERROR; //не соответствие формату
    //проверка значений
    ch_day = atoi( value );
    ch_mon = atoi( value + 3 );
    ch_year = atoi( value + 6 );
    if ( ch_day < 1 || ch_day > 31 || ch_mon < 1 || ch_mon > 12 || ch_year < 2000 || ch_year > 2099 )
        return ERROR; //недопустимые значения
    //проверка прошла
    *day = ch_day;
    *month = ch_mon; 
    *year = ch_year;
    return SUCCESS;
 }

//*********************************************************************************************
// Проверяет формат даты в "value" по маске DD.MM.YYYY (01-31.01-12.2000-2099)
// return = SUCCESS - формат соответствует маске, данные заносятся в переменные: day,month,year
//        = ERROR   - формат не соответствует маске
//*********************************************************************************************
static void SyncDateTimeLog( char *new_val, char *old_val ) {

    FILE *log;
    
    if ( SDStatus() == ERROR )
        return;
    log = fopen( "date_time_sync.log", "a" );               
    fprintf( log, "%s -> %s\r\n", new_val, old_val );
    fclose( log );
 }
