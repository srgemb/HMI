

#ifndef __WINDOW_H
#define __WINDOW_H

#include "dialog.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "rl_net.h"
#include "cmsis_os2.h"
#include "lpc_rtc.h"

#include "lcd.h"
#include "rtc.h"
#include "can.h"
#include "can_data.h"
#include "uart.h"
#include "lcd_def.h"
#include "ram_disk.h"
#include "can_def.h"
#include "eeprom.h"
#include "message.h"
#include "command.h"
#include "dev_param.h"
#include "trc_calc.h"
#include "tracker_ext.h"
#include "ext_ram_addr.h"

//*************************************************************************************************
// 
//*************************************************************************************************

//Расчет координат X/Y для размещения окна по центу экрана            
#define WIN_POS_X( size_x )             ( ( C_GLCD_H_SIZE - size_x ) / 2 )
#define WIN_POS_Y( size_y )             ( ( C_GLCD_V_SIZE - size_y ) / 2 )

//Размер стандарных кнопок
#define BUTTON_SIZE_X                   110
#define BUTTON_SIZE_Y                   25

//Размер кнопок вызова формы редактирования параметра
#define BUTT_EDT_SIZE_X                 20
#define BUTT_EDT_SIZE_Y                 20


#define END_DISTANCE1                   20      //отступ от правого края окна до края кнопки (выравнивание вправо)
#define BUTTON_DISTANCE2                5       //расстояние между соседними кнопками по горизонтали

//Координаты позиционирования кнопок
//numb  - номер кнопки: 1...
//win_* - размер окна X/Y
#define BUTTON_POS_X( numb, win_x )     ( win_x - ( BUTTON_SIZE_X * numb + BUTTON_DISTANCE2 * ( numb - 1 ) + END_DISTANCE1 ) )
#define BUTTON_POS_Y( win_y )           ( win_y - BUTTON_SIZE_Y - 35 )

//Расстояние между соседними строками текста по вертикали
#define TEXT_DISTANCE                   10

//Координата X/Y начала вывода названия параметра
//numb - номер параметра 1... 
#define TEXT_NAME_POS_X                 TEXT_DISTANCE       //отступ от левого края
#define TEXT_NAME_POS_Y( numb )         ( numb * TEXT_DISTANCE + ( ( numb - 1 ) * 5 ) )

//Размер полей названий параметров
#define TEXT_NAME_SIZE_X                255
#define TEXT_NAME_SIZE_Y                15

//Размер полей значений параметров
#define TEXT_VALUE_POS_X                TEXT_NAME_SIZE_X + ( TEXT_DISTANCE * 2 ) //275     
#define TEXT_VALUE_SIZE_X               80
#define TEXT_VALUE_SIZE_Y               15

//Размер полей единиц параметров
#define TEXT_UNIT_POS_X                 TEXT_VALUE_POS_X + TEXT_VALUE_SIZE_X + TEXT_DISTANCE //365
#define TEXT_UNIT_SIZE_X                50
#define TEXT_UNIT_SIZE_Y                15

//Индивидуальные параметры
//Размер полей параметров для формы HMI
#define HMI_NAME_SIZE_X                 180
#define HMI_UNIT_POS_X                  HMI_NAME_SIZE_X + ( TEXT_DISTANCE * 2 )
#define HMI_UNIT_SIZE_X                 170

//Размер полей параметров для формы SPA
#define SPA_NAME_SIZE_X                 160
#define SPA_VALUE_SIZE_X                50
#define SPA_VALUE_POS_X                 SPA_NAME_SIZE_X + ( TEXT_DISTANCE * 2 )
#define SPA_UNIT_SIZE_X                 50
#define SPA_UNIT_POS_X                  SPA_VALUE_POS_X + SPA_UNIT_SIZE_X + TEXT_DISTANCE

//Размер полей параметров для формы TS
#define TS_NAME_SIZE_X                  200

#define TS_VALUE1_SIZE_X                150
#define TS_VALUE1_POS_X                 TS_NAME_SIZE_X + ( TEXT_DISTANCE * 2 )

#define TS_VALUE2_SIZE_X                150
#define TS_VALUE2_POS_X                 TS_VALUE1_POS_X + TS_VALUE1_SIZE_X + TEXT_DISTANCE

#define TS_UNIT_SIZE_X                  30
#define TS_UNIT_POS_X                   TS_VALUE2_POS_X + TS_VALUE2_SIZE_X + TEXT_DISTANCE

#define TS_VALUE_SIZE_Y2                50          //размер поля для двух отдельных полей

//интервал обновления информации
#define TIME_UPD_MAIN_WINDOW            500         //в основном экране
#define TIME_UPD_PARAM                  500         //для всех параметров
#define TIME_UPD_HMI                    500         //состояния связи

#define WM_MESSAGE_MODAL_OPEN           (WM_USER + 0x0001)  //открытие модального окна
#define WM_MESSAGE_MODAL_END            (WM_USER + 0x0002)  //пользователь закрыл модальное окно
#define WM_MESSAGE_MODAL_CLOSE          (WM_USER + 0x0003)  //принудительное закрытие модального окна

//Тип окна MessageBox
#define MESSAGE_INFO                    0           //информационное сообщение
#define MESSAGE_WARNING                 1           //сообщение - предупреждение
#define MESSAGE_ERROR                   2           //сообщение об ошибке
#define MESSAGE_QUESTION                3           //запрос - подтверждение

//Коды результата окна MessageBox
#define MB_OK                           0
#define MB_NO                           0
#define MB_CLOSE                        0
#define MB_YES                          1

//Маски доступных видов клавиш для цифровой клавиатуры
#define KEY_DIGITAL                     0x01        //цифровые кнопки (0-9)
#define KEY_HEX                         0x02        //кнопки для ввода шестнадцатеричных значений (ABCDEF)
#define KEY_STR                         0x04        //кнопка "*"
#define KEY_SLH                         0x08        //кнопка "/"
#define KEY_PLS                         0x10        //кнопка "+"
#define KEY_DOT                         0x20        //кнопка "."
#define KEY_MIN                         0x40        //кнопка "-"

//Групповые виды масок для разных типов значений
#define KEY_GRP_INT                     KEY_DIGITAL
#define KEY_GRP_HEX                     KEY_DIGITAL | KEY_HEX
#define KEY_GRP_FLOAT                   KEY_DIGITAL | KEY_DOT | KEY_MIN | KEY_PLS
#define KEY_GRP_MAC                     KEY_DIGITAL | KEY_HEX | KEY_MIN
#define KEY_GRP_IP                      KEY_DIGITAL | KEY_DOT
#define KEY_GRP_PORT                    KEY_DIGITAL
#define KEY_GRP_ALL                     KEY_DIGITAL | KEY_HEX | KEY_STR | KEY_SLH | KEY_PLS | KEY_DOT | KEY_MIN

#define COUNT_PARAM( dim )              ( sizeof( dim )/sizeof( dim[0] ) )

//*************************************************************************************************
// Структура стека открытых окон
//*************************************************************************************************
typedef struct {
    WM_HWIN hwnd;
    int id_frame;
   } STACK_HWND;

//*************************************************************************************************
// Прототипы функций
//*************************************************************************************************
void WindowInit( void );
void WinOutStr( char *str );

void TaskWindow( void *pvParameters );

void WinBatMon( void );
void WinAlt( void );
void WinHmi( void );
void WinBatMon( void );
void WinSpa( void );
void WinMppt( void );
void WinGen( void );
void WinInv( void );
void WinCharge( void );
void WinTrc( void );
void WinParam( void );
void WinEth( void );
void WinDtime( void );
void WinSnd( void );
char *WinKeyPad( char *value, uint8_t max_len, uint8_t mode, int32_t val_min, int32_t val_max );
char *WinKeyBrd( char *value, uint8_t max_len );

uint8_t MessageBox( uint8_t type, char *text );

#endif
