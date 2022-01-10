
#ifndef __SDCARD_H
#define __SDCARD_H

#include <lpc_types.h>
#include <stdbool.h>
#include <stdio.h>

//*************************************************************************************************
// Функции управления
//*************************************************************************************************
void SDInit( void );
uint32_t SDMount( char *buff );
uint32_t SDUnMount( char *buff );
uint32_t SDCid( char *buff );
uint32_t SDFormat( char *buff );
uint32_t SDDir( char *buff, char *mask );

//*************************************************************************************************
// Функции статуса/состояния
//*************************************************************************************************
uint32_t SDStatCheck( char *buff );
Status SDStatus( void );
void FormatDot( uint64_t value, char *dest );
uint32_t GetFileSize( FILE *file );
uint32_t FileDelete( char *buff, char const *file_name );
uint32_t DirDelete( char *buff, char *dir_name );
uint32_t FileRename( char *buff, char *file_name, char *new_name );
uint32_t FileType( char *buff, char *file_name, uint16_t page );
char *NumDotFormat( uint64_t val, char *sp );

#endif 
