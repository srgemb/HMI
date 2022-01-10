
//*************************************************************************************************
//
// Управление FTP
//
//*************************************************************************************************

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "rl_fs.h"
#include "cmsis_os2.h"
#include "rl_net.h"
#include "rl_net_lib.h"
#include "rl_net_ds.h"

#include "rtc.h"
#include "net.h"
#include "uart.h"
#include "eeprom.h"
#include "sdcard.h"
#include "message.h"

//*************************************************************************************************
// Локальные константы
//*************************************************************************************************
//#define FTP_LOG_ALL_UART                //Вывод лог FTP всегда в UART

//*************************************************************************************************
// Внешние переменные
//*************************************************************************************************
extern CONFIG_HMI cfg_hmi;

//*********************************************************************************************
// Локальные переменные
//*********************************************************************************************
static char * const access_mode[] = {
    "File Read ",
    "File Write ",
    "Directory Create ",
    "Directory Remove ",
    "Directory List "
 };

static char * const ftp_event[] = {
    "FTP user logged in, session is busy\r\n",
    "FTP user logged out, session is idle\r\n",
    "FTP user login failed (invalid credentials)\r\n",
    "FTP file download ended\r\n",
    "FTP file upload ended\r\n",
    "FTP file deleted\r\n",
    "FTP File or directory renamed\r\n",
    "FTP directory created\r\n",
    "FTP directory removed\r\n",
    "FTP requested file operation denied\r\n",
    "FTP local file operation error\r\n",
    "FTP generic file operation error\r\n",
    "FTP generic FTP server error\r\n"
 };

//*********************************************************************************************
// Прототипы локальных функций
//*********************************************************************************************
static void FTPLog( char *str );

//*********************************************************************************************
// Запуск FTP сервиса
//*********************************************************************************************
void FTPServer( void ) {
    
    char str[80];
    netStatus stat;

    netFTPs_Stop();
    netFTPs_SetPort( 21 );
    netFTPs_LoginOnOff( true );
    stat = netFTPs_Start();
    sprintf( str, MessageFtp( MSG_FTP_START ), NetStateDesc( stat ) );
    UARTSendStr( str );
}

//*********************************************************************************************
// Функция обратного вызова обработки сообщений
//*********************************************************************************************
void netFTPs_Notify( netFTPs_Event event ) {
 
    switch ( event ) {
        case netFTPs_EventLogin:
        FTPLog( ftp_event[event] );     // User logged in, session is busy
        break;

        case netFTPs_EventLogout:
        FTPLog( ftp_event[event] );     // User logged out, session is idle
        break;

        case netFTPs_EventLoginFailed:
        FTPLog( ftp_event[event] );     // User login failed (invalid credentials)
        break;

        case netFTPs_EventDownload:
        FTPLog( ftp_event[event] );     // File download ended
        break;

        case netFTPs_EventUpload:
        FTPLog( ftp_event[event] );     // File upload ended
        break;

        case netFTPs_EventDelete:
        FTPLog( ftp_event[event] );     // File deleted
        break;

        case netFTPs_EventRename:
        FTPLog( ftp_event[event] );     // File or directory renamed
        break;

        case netFTPs_EventMakeDirectory:
        FTPLog( ftp_event[event] );     // Directory created
        break;

        case netFTPs_EventRemoveDirectory:
        FTPLog( ftp_event[event] );     // Directory removed
        break;

        case netFTPs_EventOperationDenied:
        FTPLog( ftp_event[event] );     // Requested file operation denied
        break;

        case netFTPs_EventLocalFileError:
        FTPLog( ftp_event[event] );     // Local file operation error
        break;

        case netFTPs_EventFileError:
        FTPLog( ftp_event[event] );     // Generic file operation error
        break;

        case netFTPs_EventError:
        FTPLog( ftp_event[event] );     // Generic FTP server error
        break;
     }
 }

//*********************************************************************************************
// Функция обратного вызова обработки сообщений
// const NET_ADDR *addr - IP адрес удаленного хоста
// returns = true       - Connection from the remote client is allowed.
//         = false      - Connection is denied.
//*********************************************************************************************
bool netFTPs_AcceptClient( const NET_ADDR *addr ) {

    char str[160];
    
    sprintf( str, MessageFtp( MSG_FTP_ACCEPT ), addr->addr[0], addr->addr[1], addr->addr[2], addr->addr[3] );
    FTPLog( str );
    return true;
 }

//*********************************************************************************************
// Проверка имени пользователя
// const char *username - имя пользователя
// return = User identification number
//        = 0 - if the user is not existing
//*********************************************************************************************
uint8_t netFTPs_CheckUsername( const char *username ) {
  
    char str[160];
    uint8_t result;

    //проверка пользователя
    if ( strcmp( username, cfg_hmi.user_name2 ) == 0 )
        result = 1; // Username is correct, return user_id
    else result = 0;
    memset( str, 0x00, sizeof( str ) );
    //sprintf( str, MessageFtp( MSG_FTP_USER ) /*"FTP user name: \"%s\" %s\r\n"*/, username, result ? "access is allowed." : "access denied." );
    sprintf( str, MessageFtp( MSG_FTP_USER ), username, result ? MessageNet( MSG_NET_USER_ACCEPT ) : MessageNet( MSG_NET_USER_DENIED ) );
    FTPLog( str );
    return result;
 }

//*********************************************************************************************
// Проверка пароля пользователя
// uint8_t user_id   - ID пользователя
// const char *fname - имя файла
// return = true     - password accepted.
//        = false    - invalid password.
//*********************************************************************************************
bool netFTPs_CheckPassword( uint8_t user_id, const char *password ) {

    char str[160];
    uint8_t result;

    memset( str, 0x00, sizeof( str ) );
    if ( CheckMD5( password, cfg_hmi.passwd_hash2 ) == SUCCESS ) 
        result = true;
    else result = false;
    //sprintf( str, MessageFtp( MSG_FTP_PASSWD ) /*"FTP user ID: %d, password: \"%s\" %s\r\n"*/, user_id, password, result ? "password accepted." : "invalid password." );
    sprintf( str, MessageFtp( MSG_FTP_PASSWD ), user_id, password, result ? MessageNet( MSG_NET_PASS_ACCEPT ) : MessageNet( MSG_NET_PASS_DENIED ) );
    FTPLog( str );
    return result;
 }

//*********************************************************************************************
// Проверка прав доступа к файлу
// uint8_t user_id   - ID пользователя
// const char *fname - имя файла
// uint32_t access   - тип доступа
// returns = true    - File access is allowed.
//           false   - File access is denied.
//*********************************************************************************************
bool netFTPs_FileAccess( uint8_t user_id, const char *fname, uint32_t access ) {

    char str[160], acs[80];

    memset( str, 0x00, sizeof( str ) );
    memset( acs, 0x00, sizeof( acs ) );
    if ( access & NET_ACCESS_FILE_READ )
        strcat( acs, access_mode[0] );
    if ( access & NET_ACCESS_FILE_WRITE )
        strcat( acs, access_mode[1] );
    if ( access & NET_ACCESS_DIRECTORY_CREATE )
        strcat( acs, access_mode[2] );
    if ( access & NET_ACCESS_DIRECTORY_REMOVE )
        strcat( acs, access_mode[3] );
    if ( access & NET_ACCESS_DIRECTORY_LIST )
        strcat( acs, access_mode[4] );
    sprintf( str, MessageFtp( MSG_FTP_FILE_NAME ), user_id, fname, access, acs );
    FTPLog( str );
    return true;
 }

//*********************************************************************************************
// Логирование данных FTP сервера
//*********************************************************************************************
static void FTPLog( char *str ) {

    FILE *ftp_log;

    #ifdef FTP_LOG_ALL_UART
    UARTSendStr( str );
    #endif
    if ( !SDStatus() ) {
        //карты нет, вывод в UART
        #ifndef FTP_LOG_ALL_UART
        UARTSendStr( str );
        #endif
        return;
       }
    ftp_log = fopen( "ftp_server.log", "a" );
    if ( ftp_log == NULL ) {
        //файл не открылся, вывод в UART
        #ifndef FTP_LOG_ALL_UART
        UARTSendStr( MessageFtp( MSG_FTP_LOG_ERROR ) );
        UARTSendStr( str );
        #endif
        return;
       }
    //запись только текста сообщения
    fprintf( ftp_log, "%s %s", RTCGetLog(), str );
    fclose( ftp_log );    
 }
