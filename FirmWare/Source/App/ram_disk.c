
//*************************************************************************************************
//
// Модуль управления электронным (RAM) диском
//
//*************************************************************************************************

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "gui.h"
#include "dialog.h"
#include "window.h"

#include "crc16.h"
#include "rl_fs.h"
#include "ram_disk.h"
#include "sdcard.h"
#include "system.h"
#include "uart.h"
#include "message.h"
#include "dev_param.h"
#include "ext_ram_addr.h"

//*********************************************************************************************
// Локальные константы
//*********************************************************************************************
#define ALIGMENT16( size )      ( ( size + 0x10 ) & 0xFFFFFFF0 )    //выравнивание по границе 16 байт

//*********************************************************************************************
// Локальные переменные
//*********************************************************************************************
static uint8_t *ptr_addr = NULL;
static RAMDISK_HEADER ram_header;

static char * const file_load[] = {
    "image\\error.bmp",
    "image\\info.bmp",
    "image\\quest.bmp",
    "image\\warn.bmp",
    "image\\null.bmp",
    "image\\alt.bmp",
    //инвертора
    "image\\ts10.bmp",
    "image\\ts11.bmp",
    "image\\ts30.bmp",
    "image\\ts31.bmp",
    //генератор, АВР, основной ввод
    "image\\gen0.bmp",
    "image\\gen1.bmp",
    "image\\lep0.bmp",
    "image\\lep1.bmp",
    "image\\alt.bmp",
    //параметры
    "image\\sound.bmp",
    "image\\ethernet.bmp",
    "image\\param.bmp",
    "image\\graph.bmp",
    //состояние АКБ
    "image\\bat_0.bmp",
    "image\\bat_20.bmp",
    "image\\bat_30.bmp",
    "image\\bat_50.bmp",
    "image\\bat_60.bmp",
    "image\\bat_70.bmp",
    "image\\bat_80.bmp",
    "image\\bat_90.bmp",
    "image\\bat_100.bmp",
    //контроллеры заряда, трекер, SPA
    "image\\charge0.bmp",
    "image\\charge1.bmp",
    "image\\mppt0.bmp",
    "image\\mppt1.bmp",
    "image\\sun.bmp",
    "image\\trc0.bmp",
    "image\\trc1.bmp",
    //связи между объектами, схема без АВР
    "image\\pb_mppt_link1.bmp",
    "image\\bat_link1.bmp",
    //связи между объектами, схема c АВР
    "image\\gen_link20.bmp",
    "image\\gen_link21.bmp",
    "image\\lep_link20.bmp",
    "image\\lep_link21.bmp",
    "image\\pb_link21.bmp",
    "image\\pb_link20.bmp",
    "image\\alt_link.bmp",
    "image\\ts_link2.bmp",
    "image\\bat_link2.bmp",
    "image\\mppt_link2.bmp"
 };

//*********************************************************************************************
// Прототипы локальных функций
//*********************************************************************************************
static uint8_t *NextFree( void );
static char *LastDelim( char *str );
static bool EmptyHeader( RAMDISK_HEADER *addr );
static bool ValidHeader( RAMDISK_HEADER *addr, bool chk_file );

//*********************************************************************************************
// Инициализация файловой системы
//*********************************************************************************************
void RamDiskInit( void ) {

    uint8_t i;
    char temp1[32], temp2[32], str[80];
    
    ptr_addr = (uint8_t *)RAMDISK_ADDR;
    //очистка заголовка первого файла
    memset( ptr_addr, 0x00, RAMDISK_SIZE );
    FormatDot( RamDiskTotal(), temp1 ); 
    FormatDot( RamDiskFree(), temp2 );
    sprintf( str, MessageRdisk( MSG_RDISK_INIT ), temp1, temp2 );
    UARTSendStr( str );
    //загрузка файлов
    for ( i = 0; i < SIZE_ARRAY( file_load ); i++ )
        RamDiskLoad( file_load[i] );
 }

//*********************************************************************************************
// Загрузить файл в электронный диск
// char *file_name      - имя файла
// return = RamDiskStat - результат загрузки
//*********************************************************************************************
RamDiskStat RamDiskLoad( char *file_name ) {

    FILE *fload;
    fpos_t pos;
    volatile size_t load;
    char temp[32], str[80];
    uint8_t *adr_hdr, *adr_file;

    if ( SDStatus() == false ) {
        sprintf( str, "%s", MessageRdisk( MSG_RDISK_NO_SDCARD ) );
        UARTSendStr( str );
        return RAMDISK_LOAD_NO_SDCARD;
       }
    fload = fopen( file_name, "rb" );
    if ( fload == NULL ) {
        sprintf( str, MessageRdisk( MSG_RDISK_NOT_FoUND ), file_name );
        UARTSendStr( str );
        return RAMDISK_LOAD_NOTFOUND;
       }
    //получаем адрес для записи
    adr_hdr = NextFree();
    if ( adr_hdr == NULL ) {
        fclose( fload );
        sprintf( str, MessageRdisk( MSG_RDISK_NO_FREE ), file_name );
        UARTSendStr( str );
        return RAMDISK_LOAD_NOTSPACE;
       }
    adr_file = adr_hdr + sizeof( ram_header );
    memset( &ram_header, 0x00, sizeof( ram_header ) );
    //имя файла храним без пути
    strcpy( ram_header.name_file, LastDelim( file_name ) );
    //определяем размер файла
    fseek( fload, 0, SEEK_END );
    fgetpos( fload, &pos );
    ram_header.size_file = pos.__pos;
    fseek( fload, 0, SEEK_SET );
    //проверим наличии свободного места
    if ( ram_header.size_file >= RamDiskFree() ) {
        fclose( fload );
        sprintf( str, MessageRdisk( MSG_RDISK_NOT_SPACE ), file_name );
        UARTSendStr( str );
        return RAMDISK_LOAD_NOTSPACE; //выход за границы диска
       }
    //загрузка файла
    if ( !fread( (uint8_t *)adr_file, sizeof( uint8_t ), ram_header.size_file, fload ) ) {
        fclose( fload );
        sprintf( str, MessageRdisk( MSG_RDISK_UPL_ERROR ), file_name );
        UARTSendStr( str );
        return RAMDISK_LOAD_ERRLOAD; //выход за границы диска
       }
    fclose( fload );
    //файл прочитан в память, считаем КС и сохраняем заголовок
    ram_header.addr_file = adr_file;
    ram_header.crc_file = CalcCRC16( adr_file, ram_header.size_file );
    ram_header.next_file = adr_hdr + sizeof( ram_header ) + ALIGMENT16( ram_header.size_file );
    ram_header.crc_header = CalcCRC16( (uint8_t *)&ram_header, sizeof( ram_header ) - sizeof( ram_header.crc_header ) );
    //сохраним заголовок
    memcpy( adr_hdr, (uint8_t *)&ram_header, sizeof( ram_header ) );
    FormatDot( ram_header.size_file, temp );
    sprintf( str, MessageRdisk( MSG_RDISK_UPLOAD ), file_name, temp );
    UARTSendStr( str );
    return RAMDISK_LOAD_OK;
 }

//*********************************************************************************************
// Возвращает фактический адрес файла на электронном диске
// char *file_name    - имя файла
// uint32_t *filesize - указатель на размер файла
// return != NULL     - указатель на файл 
//         = NULL     - файл не найден или нарушение КС заголовка/файла
//*********************************************************************************************
uint8_t *RamDiskFile( char *file_name, uint32_t *filesize ) {

    uint8_t *addr_ram;
    RAMDISK_HEADER ram_file;

    *filesize = 0;
    addr_ram = ptr_addr;
    while ( true ) {
        //загрузим заголовок
        memset( &ram_file, 0x00, sizeof( ram_file ) );
        memcpy( &ram_file, addr_ram, sizeof( ram_file ) );
        if ( EmptyHeader( &ram_file ) )
            return NULL; //данных нет или закончились блоки
        if ( ValidHeader( &ram_file, true ) == ERROR )
            return NULL; //целостность данных нарушена
        //сравнение имени файла
        if ( strcmp( ram_file.name_file, ( file_name ) ) ) {
            //имя файла не совпало, проверим наличие следующего блока
            if ( ram_file.next_file == NULL )
                return NULL; //это был последний файл
            addr_ram = ram_file.next_file;
            if ( addr_ram >= (uint8_t *)RAMDISK_MAX )
                return NULL; //выход за границы диска
           }
        else { 
            //файл нашли
            *filesize = ram_file.size_file;
            return addr_ram + sizeof( ram_file );
           }
       }
 }

//*********************************************************************************************
// Вывод списка загруженных файлов на электронном диске
//*********************************************************************************************
uint32_t RamDiskDir( char *buff ) {

    char temp1[32], temp2[32], *ptr;
    uint32_t files_size = 0;
    uint8_t cnt_file = 0, *adr_hdr;
    RAMDISK_HEADER ram_dir;
    
    if ( buff == NULL )
        return 0;
    ptr = buff;
    adr_hdr = ptr_addr;
    ptr += sprintf( ptr, "%s", MessageRdisk( MSG_RDISK_HDR2 ) );
    ptr += sprintf( ptr, "%s", MessageRdisk( MSG_RDISK_HDR1 ) );
    while ( true ) {
        //загрузим заголовок
        memset( &ram_dir, 0x00, sizeof( ram_dir ) );
        memcpy( &ram_dir, adr_hdr, sizeof( ram_dir ) );
        if ( EmptyHeader( &ram_dir ) ) {
            if ( !cnt_file ) {
                FormatDot( RamDiskFree(), temp1 );
                ptr += sprintf( ptr, MessageRdisk( MSG_RDISK_NO_FILES ), temp1 );
                return strlen( buff );
               }
            else break; //файлы закончились
           }
        if ( ValidHeader( &ram_dir, true ) == SUCCESS ) {
            cnt_file++;
            files_size += ram_dir.size_file + sizeof( ram_dir );
            //вывод данных по файлу
            FormatDot( ram_dir.size_file, temp1 );
            ptr += sprintf( ptr, "%-32s %9s  0x%08X/0x%08X\r\n", ram_dir.name_file, temp1, (uint32_t)adr_hdr, (uint32_t)ram_dir.addr_file );
            if ( ram_dir.next_file == NULL )
                break;
            adr_hdr = ram_dir.next_file;
           }
      }
    ptr += sprintf( ptr, "%s", MessageRdisk( MSG_RDISK_HDR2 ) );
    FormatDot( files_size, temp1 );
    FormatDot( RamDiskFree(), temp2 );
    ptr += sprintf( ptr, MessageRdisk( MSG_RDISK_FILES ), cnt_file, temp1, temp2 );
    return strlen( buff );
 }

//*********************************************************************************************
// Возвращает общий объем электронного диска
//*********************************************************************************************
uint32_t RamDiskTotal( void ) {

    return RAMDISK_SIZE;
 }

//*********************************************************************************************
// Возвращает объем доступного места на электронном диске
// return - объем в байтах
//*********************************************************************************************
uint32_t RamDiskFree( void ) {

    uint8_t *addr;
        
    addr = NextFree();
    if ( addr == NULL )
        return 0;
    return RAMDISK_MAX - (uint32_t)addr;
 }

//*********************************************************************************************
// Проверка файловой системы на электронном диске
// RamDiskMess mess - тип проверки с выводом сообщения/без вывода сообщений
// return = SUCCESS - файловая система доступна
//        = ERROR   - файловая система содержит ошибки
//*********************************************************************************************
Status RamDiskCheck( RamDiskMess message ) {

    char temp[32], str[80];
    Status valid = SUCCESS;
    uint8_t cnt_file = 0, *adr_hdr;
    RAMDISK_HEADER ram_disk;

    adr_hdr = ptr_addr;
    while ( true ) {
        //загрузим заголовок
        memset( &ram_disk, 0x00, sizeof( ram_disk ) );
        memcpy( &ram_disk, adr_hdr, sizeof( ram_disk ) );
        if ( EmptyHeader( &ram_disk ) )
            break; //файлы закончились
        if ( ValidHeader( &ram_disk, true ) == SUCCESS ) {
            cnt_file++;
            if ( ram_disk.next_file == NULL )
                break;
            adr_hdr = ram_disk.next_file;
           }
        else {
            valid = ERROR;
            break;
           }
      }
    //вывод результата
    if ( message == RAMDISK_CHK_MESSAGE && valid == ERROR ) {
        sprintf( str, MessageRdisk( MSG_RDISK_VIOLATION ), (uint32_t)adr_hdr );
        UARTSendStr( str );
       }
    if ( message == RAMDISK_CHK_MESSAGE && valid == SUCCESS ) {
        FormatDot( RamDiskFree(), temp );
        sprintf( str, MessageRdisk( MSG_RDISK_TOTAL_FREE ), RAMDISK_SIZE, temp, cnt_file );
        UARTSendStr( str );
       }
    return valid;
 }

//*********************************************************************************************
// Возвращает адрес следующего свободного блока памяти для размещения файла
// return - адрес блока или NULL в случае отсутствия места
//*********************************************************************************************
static uint8_t *NextFree( void ) {

    uint8_t *addr_ram;
    RAMDISK_HEADER ram_next;
    
    addr_ram = ptr_addr;
    //поиск блоков
    while ( true ) {
        //загрузим заголовок
        memset( &ram_next, 0x00, sizeof( ram_next ) );
        memcpy( &ram_next, addr_ram, sizeof( ram_next ) );
        if ( EmptyHeader( &ram_next ) )
            return addr_ram; //начало свободного блока
        //блок занят, ищем следующий
        addr_ram = ram_next.next_file;
        if ( addr_ram >= (uint8_t *)RAMDISK_MAX )
            return NULL; //выход за границы диска
       }
 }

//*********************************************************************************************
// Проверяет RAMDISK_HEADER на наличие не "нулевых" данных
// return = true  - заголовок пустой
//        = false - заголовок содержит данные
//*********************************************************************************************
static bool EmptyHeader( RAMDISK_HEADER *addr ) {

    uint8_t i, data = 0;
    
    for ( i = 0; i < sizeof( RAMDISK_HEADER ); i++ )
        data |= *( (uint8_t *)addr + i );
    if ( data )
        return false;
    else return true;
 }

//*********************************************************************************************
// Проверка целостности заголовка и файла
// RAMDISK_HEADER *addr - указатель на структуру блока заголовка
// bool chk_file        - проверять дополнительно КС файла
// return = SUCCESS     - целостность данных не нарушена
//        = ERROR       - есть ошибки в заголовке или в файле
//*********************************************************************************************
static bool ValidHeader( RAMDISK_HEADER *addr, bool chk_file ) {

    uint16_t crc;
    
    //проверка заголовка
    crc = CalcCRC16( (uint8_t *)addr, sizeof( RAMDISK_HEADER ) - sizeof( addr->crc_header ) );
    if ( crc != addr->crc_header )
        return ERROR; //КС заголовка не совпали
    if ( !strlen( addr->name_file ) )
        return ERROR; //имя файла не указано
    //проверка файла
    if ( chk_file == false )
        return SUCCESS;
    //дополнительно проверим КС файла
    crc = CalcCRC16( addr->addr_file, addr->size_file );
    if ( crc != addr->crc_file )
        return ERROR; //КС файла не совпали
    return SUCCESS;
 }
 
//*********************************************************************************************
// Возвращает указатель на первый символ после последнего разделителя '\' в строке
// char *str - указатель на строку
//*********************************************************************************************
static char *LastDelim( char *str ) {

    char *tmp;
    
    tmp = str + strlen( str );
    while ( tmp-- > str  ) {
        if ( *tmp == '\\' )
            return ++tmp;
       }
    return str;
 }
