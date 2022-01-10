
//*************************************************************************************************
//
// Управление TELNET
//
//*************************************************************************************************

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "lpc_clkpwr.h"

#include "cmsis_os2.h"
#include "rl_net.h"
#include "rl_net_lib.h"
#include "rl_net_ds.h"

#include "net.h"
#include "rtc.h"
#include "uart.h"
#include "eeprom.h"
#include "eeprom.h"
#include "sdcard.h"
#include "command.h"
#include "message.h"

//*********************************************************************************************
// Локальные константы
//*********************************************************************************************
//#define TELNET_LOG_ALL_UART                //Вывод лога всегда в UART

//*************************************************************************************************
// Внешние переменные
//*************************************************************************************************
extern CONFIG_HMI cfg_hmi;

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
char log_str[160];

//*********************************************************************************************
// Прототипы локальных функций
//*********************************************************************************************
static void TelnetLog( char *str );

//*********************************************************************************************
// Запуск сервиса Telnet
//*********************************************************************************************
void Telnet( void ) {
    
    netStatus stat;

    netTELNETs_Stop();
    netTELNETs_SetPort( 23 );
    netTELNETs_LoginOnOff( true );
    stat = netTELNETs_Start();
    sprintf( log_str, MessageNet( MSG_TELNET_START ), NetStateDesc( stat ) );
    UARTSendStr( log_str );
}

//*********************************************************************************************
// Функция обратного вызова обработки сообщений
// Parameters
//    [in]	msg	code of requested message.
//    [out]	buf	output buffer to write the message to.
//    [in]	buf_len	length of the output buffer in bytes.
// Returns
//    number of bytes written to output buffer.
//*********************************************************************************************
uint32_t netTELNETs_ProcessMessage( netTELNETs_Message msg, char *buf, uint32_t buf_len ) {

    uint32_t rlen = 0;

    switch ( msg ) {
        case netTELNETs_MessageWelcome:
            // Initial welcome message
            rlen = sprintf( buf, MessageNet( MSG_TELNET_WELCOME ), cfg_hmi.host_name );
            break;
        case netTELNETs_MessagePrompt:
            // Prompt message
            rlen = sprintf( buf, "%s", MessageNet( MSG_TELNET_PROMPT ) );
            break;
        case netTELNETs_MessageLogin:
            // Login welcome message, if authentication is enabled
            rlen = sprintf( buf, MessageNet( MSG_TELNET_LOGIN ), cfg_hmi.host_name );
            break;
        case netTELNETs_MessageUsername:
            // Username request login message
            rlen = sprintf( buf, "%s", MessageNet( MSG_TELNET_USER ) );
            break;
        case netTELNETs_MessagePassword:
            // Password request login message
            rlen = sprintf( buf, "%s", MessageNet( MSG_TELNET_PASSWD ) );
            break;
        case netTELNETs_MessageLoginFailed:
            // Incorrect login error message
            rlen = sprintf( buf, "%s", MessageNet( MSG_TELNET_LOGIN_ERR ) );
            sprintf( log_str, MessageNet( MSG_TELNET_LOGIN_ERR1 ), netTELNETs_GetUsername() );
            TelnetLog( log_str );
            break;
        case netTELNETs_MessageLoginTimeout:
            // Login timeout error message
            rlen = sprintf( buf, "%s", MessageNet( MSG_TELNET_TIMEOUT ) );
            break;
        case netTELNETs_MessageUnsolicited:
            // Unsolicited message (ie. from basic interpreter)
            break;
       }
    return rlen;
 }

//*********************************************************************************************
// Функция обратного вызова обработки команд
// Parameters
// [in]	cmd	pointer to command string from Telnet client.
// [out]	buf	output buffer to write the return message to.
// [in]	buf_len	length of the output buffer in bytes.
// [in,out]	pvar	pointer to a session's local buffer of 4 bytes.
//     1st call = cleared to 0.
//     2nd call = not altered by the system.
//     3rd call = not altered by the system, etc.
// Returns
//     number of bytes written to output buffer.
//     return len | (1u<<31) = repeat flag, the system calls this function again for the same command.
//     return len | (1u<<30) = disconnect flag, the system disconnects the Telnet client.
//*********************************************************************************************
uint32_t netTELNETs_ProcessCommand( const char *cmd, char *buf, uint32_t buf_len, uint32_t *pvar ) {

    uint32_t len = 0;
    volatile uint8_t cnt_par;
    
    if ( strlen( cmd ) && !*pvar )
        TelnetLog( (char *)cmd );
    if ( netTELNETs_CheckCommand( cmd, "QUIT" ) || netTELNETs_CheckCommand( cmd, "EXIT" ) ) {
        len = sprintf( buf, "%s", MessageNet( MSG_TELNET_END ) );
        TelnetLog( buf );
        return len | ( 1u << 30 ); //закрываем соединение
       }
    //вызов функции выполнения команды
    if ( strlen( cmd ) )
        len = ExecTelnet( SOURCE_CMD_TELNET, cmd, buf, buf_len, pvar );
    else len = sprintf( buf, "%s", MessageNet( MSG_TELNET_ENT_COMMAND ) );
    return len;
 }

//*********************************************************************************************
// Функция обратного вызова - проверка IP клиента
// true = Connection from the remote client is allowed.
// false = Connection is denied.
//*********************************************************************************************
bool netTELNETs_AcceptClient( const NET_ADDR *addr ) {

    memset( log_str, 0x00, sizeof( log_str ) );
    if ( addr->addr_type == NET_ADDR_IP4 ) {
        sprintf( log_str, MessageNet( MSG_TELNET_ACCEPT ), addr->addr[0], addr->addr[1], addr->addr[2], addr->addr[3] );
        TelnetLog( log_str );
       }
    return true;
 }

//*********************************************************************************************
// Функция обратного вызова - проверка имени клиента
// User identification number.
// 0 if the user is not existing.
//*********************************************************************************************
uint8_t netTELNETs_CheckUsername( const char *username ) {

    uint8_t result;

    //проверка пользователя
    if ( strcmp( username, cfg_hmi.user_name3 ) == 0 )
        result = 1; // Username is correct, return user_id
    else result = 0;
    memset( log_str, 0x00, sizeof( log_str ) );
    sprintf( log_str, MessageNet( MSG_TELNET_USER_OUT ), username, result ? MessageNet( MSG_NET_USER_ACCEPT ) : MessageNet( MSG_NET_USER_DENIED ) );
    TelnetLog( log_str );
    return result;
 }

//*********************************************************************************************
// Функция обратного вызова - проверка пароля
// true = password accepted.
// false = invalid password. 
//*********************************************************************************************
bool netTELNETs_CheckPassword( uint8_t user_id, const char *password ) {

    uint8_t result;

    memset( log_str, 0x00, sizeof( log_str ) );
    if ( CheckMD5( password, cfg_hmi.passwd_hash3 ) == SUCCESS ) 
        result = true;
    else result = false;
    sprintf( log_str, MessageNet( MSG_TELNET_USER_PASS ), user_id, password, result ? MessageNet( MSG_NET_PASS_ACCEPT ) : MessageNet( MSG_NET_PASS_DENIED ) );
    TelnetLog( log_str );
    return result;
 }

//*********************************************************************************************
// Логирование данных Telnet сервера
// char *str - строка для логирования
//*********************************************************************************************
static void TelnetLog( char *str ) {

    FILE *log;

    #ifdef TELNET_LOG_ALL_UART
    UARTSendStr( str );
    #endif
    if ( SDStatus() == ERROR ) {
        //карты нет, вывод в UART
        #ifndef TELNET_LOG_ALL_UART
        UARTSendStr( MessageNet( MSG_TELNET_TELLOG_LOG ) );
        UARTSendStr( str );
        UARTSendStr( Message( CONS_MSG_CRLF ) );
        #endif
        return;
       }
    log = fopen( "telnet_server.log", "a" );
    if ( log == NULL ) {
        //файл не открылся, вывод в UART
        #ifndef TELNET_LOG_ALL_UART
        UARTSendStr( MessageNet( MSG_TELNET_TELLOG_LOG ) );
        UARTSendStr( str );
        UARTSendStr( Message( CONS_MSG_CRLF ) );
        #endif
        return;
       }
    //запись только текста сообщения
    fprintf( log, "%s %s\r\n", RTCGetLog(), str );
    fclose( log );    
 }
