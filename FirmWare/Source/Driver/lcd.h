
#ifndef __LCD_H
#define __LCD_H

#include <stdint.h>

//Режим подсветки
typedef enum {
    LCD_BACK_LIGTH_OFF,                     //выключение подсветки
    LCD_BACK_LIGTH_ON,                      //включение подсветки
    LCD_BACK_LIGTH_SLOW_ON,                 //плавное включение подсветки
    LCD_BACK_LIGTH_SLOW_OFF                 //плавное выключение подсветки
 } BackLigth;

//*************************************************************************************************
// Функции управления
//*************************************************************************************************
void LCDInit( void );
void LCDBackLigth( BackLigth mode );
void LCDSetAddress( uint32_t addr );

#endif
