
#ifndef __RAM_DISK_H
#define __RAM_DISK_H

#include <stdint.h>
#include <stdbool.h>
#include "lpc_types.h"


typedef enum {
    RAMDISK_LOAD_OK,                        //файл загружен
    RAMDISK_LOAD_NO_SDCARD,                 //Нет SD карты
    RAMDISK_LOAD_NOTFOUND,                  //Файл не найден или не открыт
    RAMDISK_LOAD_NOTSPACE,                  //нет места для размещения файлов
    RAMDISK_LOAD_ERRLOAD                    //выход за границы диска
 } RamDiskStat;
 
typedef enum {
    RAMDISK_CHK_MESSAGE,                    //проверка RAMDISK с сообщением
    RAMDISK_CHK_QUIET                       //проверка RAMDISK без сообщением
 } RamDiskMess;

#pragma pack( push, 1 )

typedef struct {
    char name_file[32];                     //имя файла
    uint8_t *addr_file;                     //указатель на блок данными файла
    uint8_t *next_file;                     //указатель на блок со следующим файлом
    uint32_t size_file;                     //фактический размер файла
    uint16_t crc_file;                      //контрольная сумма файла
    uint16_t crc_header;                    //контрольная сумма блока (заголовка)
 } RAMDISK_HEADER;

#pragma pack( pop )

void RamDiskInit( void );
Status RamDiskCheck( RamDiskMess mess );
uint32_t RamDiskDir( char *buff );

uint32_t RamDiskFree( void );
uint32_t RamDiskTotal( void );
RamDiskStat RamDiskLoad( char *file_name );
uint8_t *RamDiskFile( char *file_name, uint32_t *filesize );
                       
#endif

