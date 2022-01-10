
//*************************************************************************************************
//
// Модуль сохранения копии экрана в файл формата BMP
//
//*************************************************************************************************

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "dialog.h"

#include "cmsis_os2.h"

#include "uart.h"
#include "sdcard.h"
#include "lcd_def.h"
#include "message.h"
#include "ext_ram_addr.h"

//*********************************************************************************************
// Локальные константы
//*********************************************************************************************
#define LCD_H_SIZE_BYTES            C_GLCD_H_SIZE * 4       //размер одной строки LCD в байтах

//тип сжатия изображения
typedef enum {
   BI_RGB       = 0x0000,
   BI_RLE8      = 0x0001,
   BI_RLE4      = 0x0002,
   BI_BITFIELDS = 0x0003,
   BI_JPEG      = 0x0004,
   BI_PNG       = 0x0005,
   BI_CMYK      = 0x000B,
   BI_CMYKRLE8  = 0x000C,
   BI_CMYKRLE4  = 0x000D
 } Compression;
 
//*********************************************************************************************
// Локальные переменные
//*********************************************************************************************

//временный буфер копии экрана
static uint8_t screen_buff[C_GLCD_SIZE_BYTES] __attribute__ ((at(SCRN_RAM_ADDRES)));
static uint8_t screen_temp[LCD_H_SIZE_BYTES] __attribute__ ((at(SROW_RAM_ADDRES)));

#pragma pack( push, 1 )

//заголовок файла BMP
typedef struct tagBITMAPFILEHEADER {
   uint16_t     bfType;                 //тип файла 'BM'
   uint32_t     bfSize;                 //размер самого файла в байтах
   uint16_t     bfReserved1;
   uint16_t     bfReserved2;
   uint32_t     bfOffBits;              //начало битового массива относительно начала файла
 } BITMAPFILEHEADER;
 
//заголовок параметров файла BMP
typedef struct tagBITMAPINFOHEADER {
   uint32_t     biSize;                 //размер самой структуры
   uint32_t     biWidth;                //ширина картинки в пикселях
   uint32_t     biHeight;               //высота картинки в пикселях
   uint16_t     biPlanes;               //количество плоскостей
   uint16_t     biBitCount;             //количество бит на один пиксель
   uint32_t     biCompression;          //тип сжатия
   uint32_t     biSizeImage;            //обозначает размер картинки в байтах. Если изображение не сжато 
                                        //(то есть предыдущее поле установлено в BI_RGB), то = 0
   int32_t      biXPelsPerMeter;        //горизонтальное разрешение (в пикселях на метр)
   int32_t      biYPelsPerMeter;        //вертикальное разрешение (в пикселях на метр)
   uint32_t     biClrUsed;              //количество используемых цветов из таблицы
   uint32_t     biClrImportant;         //количество важных цветов
 } BITMAPINFOHEADER;

//цвета растра для файла BMP
typedef struct {
   uint8_t      rgbBlue;                //синий
   uint8_t      rgbGreen;               //зеленый
   uint8_t      rgbRed;                 //красный
   uint8_t      rgbReserved;
 } RGB_BMP;
 
//цвета растра LCD экрана
typedef struct {
   uint8_t      rgbRed;                 //красный
   uint8_t      rgbGreen;               //зеленый
   uint8_t      rgbBlue;                //синий
   uint8_t      rgbReserved;
 } RGB_LCD;
 
#pragma pack( pop )

//*********************************************************************************************
// Формирование копии экрана в формате BMP. Файл сохраняется в корневом каталоге 
// на SD карте, имя файла: screen_NNN.bmp, где NNN - номер переданный через параметр index.
// uint8_t index - номер файла для записи
//*********************************************************************************************
void CrtScreen( uint8_t index ) {

    FILE *scr;
    uint32_t row;
    RGB_BMP rgb_bmp;
    RGB_LCD rgb_lcd;
    int org_x, org_y;
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih; 
    char str[80], file_name[40];
    uint32_t pix_cnt, *ptr_buff;
    uint8_t *ptr_scr, *row_data, *scr_data;
 
    memset( &bfh, 0, sizeof( bfh ) );
    //обозначим, что это тип файла bmp 'BM'
    bfh.bfType = 0x4D42;
    //адрес по которому находится информация о пикселях
    bfh.bfOffBits = sizeof( bfh ) + sizeof( bih );
    //посчитаем размер конечного файла
    bfh.bfSize = sizeof( bfh ) + sizeof( bih ) + C_GLCD_SIZE_BYTES;
    memset( &bih, 0, sizeof( bih ) );
    bih.biSize = sizeof( bih );   //размер структуры BITMAPINFOHEADER
    bih.biHeight = C_GLCD_V_SIZE; //пикселей по вертикале
    bih.biWidth = C_GLCD_H_SIZE;  //пикселей по горизонтали
    bih.biPlanes = 1;             //должно быть 1
    bih.biBitCount = 32;          //24 бит на пиксель
    bih.biCompression = BI_RGB;   //без сжатия
    bih.biClrUsed = 0;
    bih.biSizeImage = 0;
    //координаты текущего виртуального окна для расчета 
    //смещения в реальной памяти графического буфера
    GUI_GetOrg( &org_x, &org_y );
    if ( !org_x && !org_y )
        ptr_scr = (uint8_t *)VRAM_ADDR; //первый экран
    else ptr_scr = (uint8_t *)( VRAM_ADDR + C_GLCD_SIZE_BYTES ); //второй экран
    //делаем полную копию экрана
    memcpy( screen_buff, (uint8_t *)ptr_scr, C_GLCD_SIZE_BYTES );
    //корректировка размещения цветов для формата BMP
    ptr_buff = (uint32_t *)screen_buff;
    for ( pix_cnt = 0; pix_cnt < C_GLCD_SIZE_PIXEL; pix_cnt++, ptr_buff++ ) {
        memcpy( (uint8_t *)&rgb_lcd, (uint8_t *)ptr_buff, sizeof( rgb_lcd ) );
        rgb_bmp.rgbRed = rgb_lcd.rgbRed;
        rgb_bmp.rgbGreen = rgb_lcd.rgbGreen;
        rgb_bmp.rgbBlue = rgb_lcd.rgbBlue;
        rgb_bmp.rgbReserved = rgb_lcd.rgbReserved;
        memcpy( (uint8_t *)ptr_buff, (uint8_t *)&rgb_bmp, sizeof( rgb_bmp ) );
       }
    //адрес верхней строки
    scr_data = (uint8_t *)screen_buff;
    //адрес нижней строки
    row_data = (uint8_t *)( screen_buff + sizeof( screen_buff ) - LCD_H_SIZE_BYTES );
    //формируем изображение для записи в файл, перестановка строк, снизу вверх
    for ( row = 0; row < ( C_GLCD_V_SIZE/2 ); row++, scr_data += LCD_H_SIZE_BYTES, row_data -= LCD_H_SIZE_BYTES ) {
        //сохраним перемещаемую строку во временный буфер
        memcpy( screen_temp, row_data, LCD_H_SIZE_BYTES );
        //меняем строки местами, сверху - вниз
        memcpy( row_data, scr_data, LCD_H_SIZE_BYTES );
        memcpy( scr_data, screen_temp, LCD_H_SIZE_BYTES );
       }
    //формируем файл
    sprintf( file_name, "screen_%03u.bmp", index );
    scr = fopen( file_name, "wb" );
    if ( scr == NULL ) {
        sprintf( str, Message( CONS_MSG_NOT_OPEN ), file_name );
        UARTSendStr( str );
        return;
       }
    //запишем заголовки
    fwrite( &bfh, sizeof( uint8_t ), sizeof( bfh ), scr ); 
    fwrite( &bih, sizeof( uint8_t ), sizeof( bih ), scr ); 
    //запись данных растра
    fwrite( screen_buff, sizeof( uint8_t ), C_GLCD_SIZE_BYTES, scr );
    fclose( scr );
    sprintf( str, "\r\n%s%s saved.\r\n", Message( CONS_MSG_FILE ), file_name );
    UARTSendStr( str );
 }
