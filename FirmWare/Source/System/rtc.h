
#ifndef __RTC_H
#define __RTC_H

#include "dev_data.h"

#include "lpc_types.h"
#include "stdbool.h"

typedef enum {
    DOW_ENG_LANG,                           //день недели на английском
    DOW_RUS_LANG                            //день недели на русском
 } RtcDowLang;

typedef enum {
    SET_MODE_LOCAL,                         //установка только локальных часов
    SET_MODE_REMOTE                         //установка локальных и удаленных часов
 } RtcSetMode;

//*********************************************************************************************
//Функции управления
//*********************************************************************************************
void RTCInit( void );
Status RTCSetTime( char *param, RtcSetMode mode );
Status RTCSetDate( char *param, RtcSetMode mode );

//*********************************************************************************************
//Функции статуса
//*********************************************************************************************
void RTCHmi( RTC *rtc_hmi );
char *RTCGetTime( char *endstr );
char *RTCGetDate( char *endstr );
char *RTCGetDowDate( char *endstr, RtcDowLang rus_eng );
char *RTCGetDateTime( char *endstr, RtcDowLang rus_eng );
char *RTCFileShort( void );
Status TimeCheck( double time_beg, double time, double time_end );
double GetTimeDecimal( void );
double SecToDecimal( uint16_t value );
double TimeToDecimal( uint8_t hour, uint8_t minute, uint8_t seconds );
uint16_t DayOfWeek( int day, int month, int year );
uint16_t DayOfYear( int day, int month, int year );
uint8_t DaysOfMonth( uint8_t month, uint16_t year );
char *RTCGetLog( void );
uint8_t RTCGetIndPar( char *name );
char *RTCGetParam( uint8_t param );
char *RTCFileName( void );
Status CheckDate( char *value, uint8_t *day, uint8_t *month, uint16_t *year );

#endif
