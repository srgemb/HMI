
//*************************************************************************************************
//
// Инициализация графического интерфейса
//
//*************************************************************************************************

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "gui.h"
#include "dialog.h"

#include "window.h"
#include "calibrate.h"
#include "system.h"
#include "ram_disk.h"
#include "board_touch.h"

//*************************************************************************************************
// Внешние переменные
//*************************************************************************************************
extern GUI_FONT GUI_FontTahoma16;
extern GUI_FONT GUI_FontTahoma16b;

//*************************************************************************************************
// Атрибуты объектов RTOS
//*************************************************************************************************
static const osThreadAttr_t attr_win = {
    .name = "Windows",
    .stack_size = 3328,
    .priority = osPriorityNormal
 };
 
//*************************************************************************************************
// Инициализация графического интерфейса
//*************************************************************************************************
void WindowInit( void ) {

    GUI_Init();
    WM_SetCreateFlags( WM_CF_MEMDEV );
    //настройка экранов
    GUI_Clear();
    GUI_SetColor( GUI_WHITE );
    GUI_SetBkColor( GUI_BLACK );
    //параметры по умолчанию
    GUI_SetFont( &GUI_FontTahoma16 );
    FRAMEWIN_SetDefaultFont( &GUI_FontTahoma16 );
    TEXT_SetDefaultFont( &GUI_FontTahoma16 );
    EDIT_SetDefaultFont( &GUI_FontTahoma16 );
    MULTIPAGE_SetDefaultFont( &GUI_FontTahoma16 );
    RADIO_SetDefaultFont( &GUI_FontTahoma16 );
    BUTTON_SetDefaultFont( &GUI_FontTahoma16 );
    GUI_DispString( MessageGui( MSG_GUI_INIT_WIN ) );
    //создаем задачу управления графическим интерфейсом
    osThreadNew( TaskWindow, NULL, &attr_win );
 }
