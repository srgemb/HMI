
//*************************************************************************************************
//
// Модуль формирует перечень задач и номера версий ПО.
//
//*************************************************************************************************

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "lpc_clkpwr.h"

#include "cmsis_os2.h"
#include "rl_net.h"

#include "sdcard.h"
#include "version.h"
#include "message.h"
#include "system.h"

//*********************************************************************************************
// Локальные константы
//*********************************************************************************************
static char * const state_name[] = { 
    "Inactive", 
    "Ready", 
    "Running", 
    "Blocked", 
    "Terminated", 
    "Error"
 };

//*********************************************************************************************
// Прототипы локальных функций
//*********************************************************************************************
static char *TaskStateDesc( osThreadState_t state );
static char *Version( uint32_t version, char *str );

//*********************************************************************************************
// Вывод статистики по задачам
// char *buff - указатель на буфер для размещения результата 
// return     - кол-во байт размещенных в буфере
//*********************************************************************************************
uint32_t ListTask( char *buff ) {

    uint8_t i;
    char *ptr;
    uint32_t cnt_task, stack_space, stack_size; 
    osThreadState_t state;
    osPriority_t priority;
    osThreadId_t th_id[30];
    const char *name;

    if ( buff == NULL )
        return 0;
    ptr = buff;
    //вывод шапки параметров
    ptr += sprintf( ptr, "%s", Message( CONS_MSG_TASK_HDR1 ) );
    ptr += sprintf( ptr, "%s", Message( CONS_MSG_TASK_HDR2 ) );
    //заполним весь массив th_id значением NULL
    memset( th_id, 0x00, sizeof( th_id ) );
    cnt_task = osThreadGetCount();
    cnt_task = osThreadEnumerate( &th_id[0], sizeof( th_id )/sizeof( th_id[0] ) );
    for ( i = 0; i < cnt_task; i++ ) {
        state = osThreadGetState( th_id[i] );
        priority = osThreadGetPriority( th_id[i] );
        stack_size = osThreadGetStackSize( th_id[i] );
        stack_space = osThreadGetStackSpace( th_id[i] );
        name = osThreadGetName( th_id[i] );
        if ( name != NULL && strlen( name ) )
            ptr += sprintf( ptr, "%2u %-16s    %2u    %-10s %5u %5u\r\n", i+1, name, priority, TaskStateDesc( state ), stack_size, stack_space );
        else ptr += sprintf( ptr, "%2u ID = %-11u    %2u    %-10s %5u %5u\r\n", i+1, (uint32_t)th_id[i], priority, TaskStateDesc( state ), stack_size, stack_space );
       }
    ptr += sprintf( ptr, "%s", Message( CONS_MSG_TASK_HDR2 ) );
    return strlen( buff );
 }

//*************************************************************************************************
// Вывод значений частоты ядра/шины/версии
// char *buff - указатель на буфер для размещения результата 
// return     - кол-во байт размещенных в буфере
//*************************************************************************************************
uint32_t GetVersion( char *buff ) {
    
    char *ptr;
    osVersion_t osv;
    osStatus_t status;
    char val[32], infobuf[32];
    
    if ( buff == NULL )
        return 0;
    ptr = buff;
    ptr += sprintf( ptr, Message( CONS_MSG_APP_VER ), version );
    ptr += sprintf( ptr, Message( CONS_MSG_BUILD_VER ), compiler_date, compiler_time );
    FormatDot( CLKPWR_GetCLK( CLKPWR_CLKTYPE_CPU ), val );
    ptr += sprintf( ptr, Message( CONS_MSG_CPU_CLOCK ), val );
    FormatDot( CLKPWR_GetCLK( CLKPWR_CLKTYPE_PER ), val );
    ptr += sprintf( ptr, Message( CONS_MSG_CLOCK_PER ), val );
    FormatDot( osKernelGetTickFreq(), val );
    ptr += sprintf( ptr, Message( CONS_MSG_CLOCK_KERNEL ), val );
    FormatDot( osKernelGetSysTimerFreq(), val );
    ptr += sprintf( ptr, Message( CONS_MSG_CLOCK_SYSTIMER ), val );
    status = osKernelGetInfo( &osv, infobuf, sizeof( infobuf ) );
    if ( status == osOK ) {
        ptr += sprintf( ptr, Message( CONS_MSG_KERNEL_INFO ), infobuf );
        ptr += sprintf( ptr, Message( CONS_MSG_KERNEL_VER ), Version( osv.kernel, val ) );
        ptr += sprintf( ptr, Message( CONS_MSG_KERNEL_API ), Version( osv.api, val ) );
       }
    return strlen( buff );
 }

//*********************************************************************************************
// Расшифровка версии в текстовый буфер
// uint32_t version - значение версии в формате: mmnnnrrrr dec
// char *str        - указатель на строку для размещения результата
// return           - указатель на строку с расшифровкой в формате major.minor.rev
//*********************************************************************************************
static char *Version( uint32_t version, char *str ) {

    uint32_t major, minor, rev;
    
    rev = version%10000;
    version /= 10000;
    minor = version%1000;
    major = version/1000;
    sprintf( str, "%d.%d.%d", major, minor, rev );
    return str; 
 }

//*********************************************************************************************
// Возвращает расшифровку статуса задачи
// osThreadState_t state - код статуса задачи
// return                - указатель на строку с расшифровкой статуса задачи   
//*********************************************************************************************
static char *TaskStateDesc( osThreadState_t state ) {

    if ( state == osThreadInactive )
        return state_name[0];
    if ( state == osThreadReady )
        return state_name[1];
    if ( state == osThreadRunning )
        return state_name[2];
    if ( state == osThreadBlocked )
        return state_name[3];
    if ( state == osThreadTerminated )
        return state_name[4];
    if ( state == osThreadError )
        return state_name[5];
    return NULL;
 }

