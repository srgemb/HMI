
//*************************************************************************************************
//
// Управление файлами на SD карте
//
//*************************************************************************************************

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "lpc_types.h"
#include "lpc_gpio.h"

#include "mci_lpc40xx.h"
#include "rl_fs.h"

#include "uart.h"
#include "sdcard.h"
#include "ring_uart.h"
#include "command.h"
#include "message.h"

#define MkLower( ch )   ( isupper( ch ) ? tolower( ch ) : ch )
#define MkUpper( ch )   ( islower( ch ) ? toupper( ch ) : ch ) 

//*************************************************************************************************
// Локальные константы
//*************************************************************************************************
#define SD_DRIVE        "M0:"

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
static Status sd_mount = ERROR;

static char * const fsStatus_desc[] = {
    "Operation succeeded.",                 //fsOK 	
    "Unspecified error.",                   //fsError
    "Operation not supported.",             //fsUnsupported
    "Resource access denied.",              //fsAccessDenied
    "Invalid parameter specified.",         //fsInvalidParameter
    "Nonexistent drive.",                   //fsInvalidDrive
    "Invalid path specified.",              //fsInvalidPath
    "Drive is uninitialized.",              //fsUninitializedDrive
    "Read/write error.",                    //fsDriverError
    "Media error.",                         //fsMediaError
    "No media, or not initialized.",        //fsNoMedia
    "File system is not formatted.",        //fsNoFileSystem
    "No free space available.",             //fsNoFreeSpace
    "Requested file not found.",            //fsFileNotFound
    "The directory is not empty.",          //fsDirNotEmpty
    "Too many open files.",                 //fsTooManyOpenFiles
    "File or directory already exists.",    //fsAlreadyExists
    "Path is not a directory."              //fsNotDirectory
 };

static bool cid_read = false;
static fsCID_Register cid;

//*************************************************************************************************
// Локальные прототипы функций
//*************************************************************************************************
static void MakeDir( void );
static Status SDDetect( void );
static char *LowerCase( char *str );
static char *FsStatDescr( fsStatus stat );

//*********************************************************************************************
// Монтирование карты и файловой системы
//*********************************************************************************************
void SDInit( void ) {

    char *buff;
    
    buff = GetBuffer();
    memset( buff, 0x00, GetBuffSize() );
    SDMount( buff );
    UARTSendStr( buff );
    if ( sd_mount == SUCCESS )
        MakeDir();
 }

//*********************************************************************************************
// Монтирование карты и файловой системы
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт ответа в buff
//*********************************************************************************************
uint32_t SDMount( char *buff ) {

    int32_t id;
    fsStatus fstat;
    uint32_t ser_num = 0;
    char *ptr, label[32];

    ptr = buff;
    if ( buff == NULL )
        return 0;
    if ( !SDDetect() ) 
        return sprintf( ptr, "%s", MessageSd( MSG_SD_NOT_INSERT ) );
    memset( label, 0x00, sizeof( label ) );
    //инициализация файловой системы
    ptr += sprintf( ptr, "%s", MessageSd( MSG_SD_INIT_SYSTEM ) );
    fstat = finit( SD_DRIVE );
    ptr += sprintf( ptr, "%s %s\r\n", SD_DRIVE, FsStatDescr( fstat ) );
    if ( fstat != fsOK )
        return strlen( buff );
    //монтируем файловую систему
    ptr += sprintf( ptr, "%s", MessageSd( MSG_SD_MOUNT ) );
    fstat = fmount( SD_DRIVE );
    ptr += sprintf( ptr, "%s\r\n", FsStatDescr( fstat ) );
    if ( fstat != fsOK )
        return strlen( buff );
    sd_mount = SUCCESS;
    //читаем параметры SD карты
    ptr += sprintf( ptr, "%s", MessageSd( MSG_SD_CHECK_DRV ) );
    id = fs_ioc_get_id( SD_DRIVE );
    if ( id < 0 ) {
        cid_read = false;
        return sprintf( ptr, "%s", MessageSd( MSG_SD_ERROR ) );
       }
    fstat = fs_ioc_lock( id );
    ptr += sprintf( ptr, MessageSd( MSG_SD_BLC_ACCESS ), SD_DRIVE, FsStatDescr( fstat ) );
    if ( fstat != fsOK )
        return strlen( buff );
    fstat = fs_ioc_device_ctrl( id, fsDevCtrlCodeGetCID, &cid );
    ptr += sprintf( ptr, MessageSd( MSG_SD_IOC_ACCESS ), SD_DRIVE, FsStatDescr( fstat ) );
    if ( fstat != fsOK ) {
        memset( &cid, 0x00, sizeof( cid ) );
        return strlen( buff );
       }
    cid_read = true;
    fstat = fs_ioc_unlock( id );
    ptr += sprintf( ptr, MessageSd( MSG_SD_UNLOCK ), SD_DRIVE, FsStatDescr( fstat ) );
    if ( fstat != fsOK )
        return strlen( buff );
    //параметры диска
    fstat = fvol( SD_DRIVE, label, &ser_num );
    ptr += sprintf( ptr, MessageSd( MSG_SD_VOLUME_READ ), FsStatDescr( fstat ) );
    if ( fstat != fsOK )
        return strlen( buff );
    if ( strlen( label ) )
        ptr += sprintf( ptr, MessageSd( MSG_SD_VOLUME ), label );
    ptr += sprintf( ptr, MessageSd( MSG_SD_VOLUME_SER ), ser_num );
    return strlen( buff );
 }

//*********************************************************************************************
// Размонитрование карты и файловой системы
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт ответа в buff
//*********************************************************************************************
uint32_t SDUnMount( char *buff ) {

    fsStatus fstat;
    
    if ( buff == NULL )
        return 0;
    //проверка состояния карты
    if ( SDStatCheck( buff ) )
        return strlen( buff );
    fstat = funmount( SD_DRIVE );
    if ( fstat == fsOK )
        sd_mount = ERROR;
    sprintf( buff, MessageSd( MSG_SD_UNMOUNT ), SD_DRIVE, FsStatDescr( fstat ) );
    return strlen( buff );
 }

//*********************************************************************************************
// Вывод информации о SD карте
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт ответа в buff
//*********************************************************************************************
uint32_t SDCid( char *buff ) {

    char *ptr;

    if ( buff == NULL )
        return 0;
    ptr = buff;
    if ( cid_read == false )
        return sprintf( ptr, "%s", MessageSd( MSG_SD_CID_ERROR ) );
    ptr += sprintf( ptr, "%s", MessageSd( MSG_SD_ABOUT ) );
    ptr += sprintf( ptr, MessageSd( MSG_SD_ID_VENDOR ), cid.MID, cid.MID );
    ptr += sprintf( ptr, MessageSd( MSG_SD_OEMID ), cid.OID >> 8, cid.OID & 0xFF );
    ptr += sprintf( ptr, MessageSd( MSG_SD_NAMEPROD ), cid.PNM[0], cid.PNM[1], cid.PNM[2], cid.PNM[3], cid.PNM[4] );
    ptr += sprintf( ptr, MessageSd( MSG_SD_REVISION ), cid.PRV >> 4, cid.PRV & 0x0F );
    ptr += sprintf( ptr, MessageSd( MSG_SD_SERIAL2 ), cid.PSN );
    ptr += sprintf( ptr, MessageSd( MSG_SD_DATE ), cid.MDT & 0x0F, cid.MDT >> 4 );
    return strlen( buff );
 }

//*********************************************************************************************
// Возвращает статус наличия SD карты и смонтированной файловой системы
// return = true  - SD карты установлена, файловая система смонтирована
//          false - карты нет или файловая система не смонтирована
//*********************************************************************************************
Status SDStatus( void ) {

    if ( SDDetect() == ERROR )
        return ERROR;   //карты нет
    if ( sd_mount == SUCCESS )
        return SUCCESS; //SD не смонтирована
    return ERROR;
 }

//*********************************************************************************************
// Возвращает расшифровку статуса SD карты
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт ответа в buff
//*********************************************************************************************
uint32_t SDStatCheck( char *buff ) {

    if ( SDDetect() == ERROR )
        sprintf( buff, "%s", MessageSd( MSG_SD_NOT_INSERT ) );
    if ( sd_mount == ERROR )
        sprintf( buff, "%s", MessageSd( MSG_SD_NOT_MOUNT ) );
    return 0;
 }

//*********************************************************************************************
// Форматирование SD карты
// char *buff - указатель на буфер для размещения результата
// return     - кол-во байт ответа в buff
//*********************************************************************************************
uint32_t SDFormat( char *buff ) {

    fsStatus fstat;
    
    if ( buff == NULL )
        return 0;
    //проверка состояния карты
    if ( SDStatCheck( buff ) )
        return strlen( buff );
    fstat = fformat( SD_DRIVE, "/L HMI /FAT32" );
    sprintf( buff, MessageSd( MSG_SD_FORMAT ), SD_DRIVE, FsStatDescr( fstat ) );
    return strlen( buff );
 }
 
//*********************************************************************************************
// Создание каталогов
//*********************************************************************************************
static void MakeDir( void ) {

    fmkdir( "\\HTML" );
    fmkdir( "\\MPPT" );
    fmkdir( "\\BATMON" );
    fmkdir( "\\CHARGER" );
    fmkdir( "\\INV" );
    fmkdir( "\\GEN" );
    fmkdir( "\\ALT" );
    fmkdir( "\\TRC" );
    fmkdir( "\\EXECUTE" );
 }

//*********************************************************************************************
// Удаление файла:
// TEST.TXT            - Delete a file from current drive
// M0:\Workdir\log.txt - Delete a file from sub-folder
// M0:\Working folder\ - Delete all files within a directory
// char *buff      - указатель на буфер для размещения результата
// char *file_name - имя файла
// return          - кол-во байт ответа в buff
//*********************************************************************************************
uint32_t FileDelete( char *buff, char const *file_name ) {

    fsStatus fstat;

    if ( buff == NULL )
        return 0;
    if ( file_name == NULL || !strlen( file_name ) ) {
        sprintf( buff, "%s", MessageSd( MSG_FILE_NOT_SPECF ) );
        return strlen( buff ); //имя не указано
       }
    fstat = fdelete( file_name, NULL );
    sprintf( buff, MessageSd( MSG_FILE_DELETED ), file_name, FsStatDescr( fstat ) );
    return strlen( buff );
 }

//*************************************************************************************************
// Удаление каталога
// char *buff     - указатель на буфер для размещения результата
// char *dir_name - имя каталога
// return         - кол-во байт ответа в buff
//*************************************************************************************************
uint32_t DirDelete( char *buff, char *dir_name ) {

    fsStatus fstat;

    if ( buff == NULL )
        return 0;
    if ( dir_name == NULL || !strlen( dir_name ) ) {
        sprintf( buff, "%s", MessageSd( MSG_DIR_NOT_SPECF ) );
        return strlen( buff ); //имя не указано
       }
    fstat = frmdir( dir_name, NULL );
    sprintf( buff, MessageSd( MSG_DIR_DELETED ), dir_name, FsStatDescr( fstat ) );
    return strlen( buff );
 }

//*********************************************************************************************
// Переименование файла
// TEST.TXT            - Delete a file from current drive
// M0:\Workdir\log.txt - Delete a file from sub-folder
// M0:\Working folder\ - Delete all files within a directory
// char *buff      - указатель на буфер для размещения результата
// char *file_name - имя файла для переименования
// char *new_name  - новое имя файла
// return          - кол-во байт ответа в buff
//*********************************************************************************************
uint32_t FileRename( char *buff, char *file_name, char *new_name ) {

    fsStatus fstat;

    if ( buff == NULL )
        return 0;
    if ( file_name == NULL || !strlen( file_name ) ) {
        sprintf( buff, "%s", MessageSd( MSG_FILE_SRC_NOT_SPECF ) );
        return strlen( buff ); //имя не указано
       }
    if ( new_name == NULL || !strlen( new_name ) ) {
        sprintf( buff, "%s", MessageSd( MSG_FILE_NEW_NOT_SPECF ) );
        return strlen( buff ); //имя не указано
       }
    fstat = frename( file_name, new_name );
    sprintf( buff, MessageSd( MSG_FILE_RENAME ), file_name, new_name, FsStatDescr( fstat ) );
    return strlen( buff );
 }

//*********************************************************************************************
// Вывод содержимого SD карты
//*********************************************************************************************
uint32_t SDDir( char *buff, char *mask ) {

    uint64_t fsize;
    uint32_t i, files, dirs, find_ok = 0;
    char *ptr, temp[32], path[32], ch;
    fsStatus fstat;
    fsFileInfo info;

    if ( buff == NULL )
        return 0;
    ptr = buff;
    //проверка состояния карты
    if ( SDStatCheck( buff ) )
        return strlen( buff );
    if ( !strlen( mask ) )
        strcpy( path, "*.*" ); //параметра нет, добавим маску
    else strcpy( path, mask );
    ptr += sprintf( ptr, MessageSd( MSG_FILE_LIST_FILES ), SD_DRIVE, path );
    ptr += sprintf( ptr, "%s", Message( CONS_MSG_HEADER ) );
    info.fileID  = 0;
    files = dirs = fsize = 0;
    while ( ( fstat = ffind( path, &info ) ) == fsOK ) {
        find_ok++;
        if ( info.attrib & FS_FAT_ATTR_DIRECTORY ) {
            //вывод атрибутов каталога
            i = 0;
            while ( strlen((const char *)info.name+i ) > 41 ) {
                ch = info.name[i+41];
                info.name[i+41] = 0;
                ptr += sprintf( ptr, "%-41s", &info.name[i] );
                info.name[i+41] = ch;
                i += 41;
               }
            ptr += sprintf( ptr, MessageSd( MSG_DIR_INFO ), &info.name[i] );
            ptr += sprintf( ptr, " %02d.%02d.%04d  %02d:%02d\r\n", info.time.day, info.time.mon, info.time.year, info.time.hr, info.time.min );
            dirs++;
           }
        else {
            //вывод размера файла
            FormatDot( info.size, temp );
            i = 0;
            while ( strlen( (const char *)info.name+i ) > 41 ) {
                ch = info.name[i+41];
                info.name[i+41] = 0;
                ptr += sprintf( ptr, "%-41s", LowerCase( &info.name[i] ) );
                info.name[i+41] = ch;
                i += 41;
               }
            ptr += sprintf( ptr, "%-41s %14s", LowerCase( &info.name[i] ), temp );
            ptr += sprintf( ptr, " %02d.%02d.%04d  %02d:%02d\r\n", info.time.day, info.time.mon, info.time.year, info.time.hr, info.time.min );
            fsize += info.size;
            files++;
           }
       }
    if ( find_ok ) {
        if ( info.fileID == 0 ) {
            ptr += sprintf( ptr, "%s", Message( CONS_MSG_HEADER ) );
            ptr += sprintf( ptr, "%s", MessageSd( MSG_NO_FILES ) );
           } 
        else {
            FormatDot( fsize, temp );
            ptr += sprintf( ptr, "%s", Message( CONS_MSG_HEADER ) );
            ptr += sprintf( ptr, MessageSd( MSG_FILES_TOTAL ), files, temp );
           }
        FormatDot( ffree( path ), temp );
        if ( dirs ) {
            ptr += sprintf( ptr, "%s", Message( CONS_MSG_HEADER ) );
            ptr += sprintf( ptr, MessageSd( MSG_DIR_TOTAL ), dirs, temp );
           }
        else {
            ptr += sprintf( ptr, "%s", Message( CONS_MSG_HEADER ) );
            ptr += sprintf( ptr, MessageSd( MSG_DIR_FREE ), temp );
           }
      }
    else ptr += sprintf( ptr, MessageSd( MSG_FILE_FFIND ), FsStatDescr( fstat ) );
    return strlen( buff );
 }

//*********************************************************************************************
// Вывод файла в текстовом режиме
// char *file_name - имя файла 
// uint16_t page   - номер страницы для вывода на экран, по умолчанию выводиться весь файл
// CR (\r) = 0x0D 
// LF (\n) = 0x0A
//*********************************************************************************************
uint32_t FileType( char *buff, char *file_name, uint16_t page ) {

    char *ptr;
    FILE *fview;
    uint32_t read, fsize;
    
    if ( buff == NULL )
        return 0;
    ptr = buff;
    if ( file_name == NULL || !strlen( file_name ) )
        return sprintf( buff, "%s", Message( CONS_MSG_ERR_FILE ) );
    fview = fopen( file_name, "r" );               
    if ( fview == NULL )
        return sprintf( buff, Message( CONS_MSG_NOT_OPEN ), file_name );
    fsize = GetFileSize( fview );
    ptr += sprintf( ptr, Message( CONS_MSG_FILE1 ), file_name, fsize );
    ptr += sprintf( ptr, "%s", Message( CONS_MSG_HEADER ) );
    read = fread( ptr, sizeof( char ), GetBuffSize() - 1024, fview );
    fclose( fview );
    sprintf( ptr + read, Message( CONS_MSG_FILE2 ), MessageSd( MSG_FILE_EOF ), fsize, read );
    return strlen( buff );
 }

//*************************************************************************************************
// Возвращает фактический размер файла
// FILE *file - указатель на файл
// return     - размер файла
//*************************************************************************************************
uint32_t GetFileSize( FILE *file ) {

    fpos_t prev, npos;

    if ( file == NULL )
        return 0;
    fgetpos( file, &prev );
    fseek( file, 0, SEEK_END );
    fgetpos( file, &npos );
    fseek( file, prev.__pos, SEEK_SET );
    return npos.__pos;
 }

//*************************************************************************************************
// Форматированный вывод числа с разделителями по группам
// uint64_t value - значение для форматирования
// char *dest     - указатель для размещения результата
//*************************************************************************************************
void FormatDot( uint64_t value, char *dest ) {

    if ( value >= (uint64_t)1e12 ) {
        dest += sprintf( dest, "%d,", (uint32_t)( value/(uint64_t)1e12) );
        value %= (uint64_t)1e12;
        dest += sprintf( dest, "%03d,", (uint32_t)( value/(uint64_t)1e9) );
        value %= (uint64_t)1e9;
        dest += sprintf( dest, "%03d,", (uint32_t)( value/(uint64_t)1e6) );
        value %= (uint64_t)1e6;
        sprintf( dest, "%03d,%03d", (uint32_t)( value/1000),(uint32_t)( value%1000 ) );
        return;
       }
    if ( value >= (uint64_t)1e9 ) {
        dest += sprintf( dest, "%d,", (uint32_t)( value/(uint64_t)1e9) );
        value %= (uint64_t)1e9;
        dest += sprintf( dest, "%03d,", (uint32_t)( value/(uint64_t)1e6) );
        value %= (uint64_t)1e6;
        sprintf( dest, "%03d,%03d", (uint32_t)( value/1000),(uint32_t)( value%1000 ) );
        return;
       }
    if ( value >= (uint64_t)1e6 ) {
        dest += sprintf( dest, "%d,", (uint32_t)( value/(uint64_t)1e6));
        value %= (uint64_t)1e6;
        sprintf( dest,"%03d,%03d", (uint32_t)( value/1000 ),(uint32_t)( value%1000 ) );
        return;
       }
    if ( value >= 1000 ) {
        sprintf( dest, "%d,%03d", (uint32_t)( value/1000 ),(uint32_t)( value%1000 ) );
        return;
       }
    sprintf( dest, "%d", (uint32_t)(value) );
 }

//*********************************************************************************************
// Преобразует строку в нижний регистр
//*********************************************************************************************
static char *LowerCase( char *str ) {

    char *result;
    
    result = str;
    while ( *str ) {
        *str = MkLower( *str );
        str++;
       }
    return result;
 }

//***************************************************************************************** ****
// Возвращает расшифровку ошибки операции с SD картой
//*********************************************************************************************
static char *FsStatDescr( fsStatus stat ) {

    return (char *)fsStatus_desc[stat];
 }

//*********************************************************************************************
// Возвращает наличия установленной SD карты
// return = ERROR   - карты нет
//          SUCCESS - карта установлена
//*********************************************************************************************
static Status SDDetect( void ) {

    if ( GPIO_PinRead( 1, 13 ) )
        return ERROR;
    return SUCCESS;
 }
