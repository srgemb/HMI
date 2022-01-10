
//*************************************************************************************************
//
// Форма ввода текстовых значений
//
//*************************************************************************************************

#include "window.h"
#include "dialog.h"
#include "messagebox.h"

//*************************************************************************************************
// Внешние переменные
//*************************************************************************************************
extern WM_HWIN hwin0;
extern GUI_FONT GUI_FontTahoma16b;

//*************************************************************************************************
// Размер окна
//*************************************************************************************************
#define WIN_SIZE_X          605
#define WIN_SIZE_Y          375

//*************************************************************************************************
// Определение идентификаторов объектов в форме
//*************************************************************************************************
#define ID_FRAMEWIN_0       (GUI_ID_USER + 17)

#define ID_BUTTON_CLS       (GUI_ID_USER + 0x10 + 3)
#define ID_BUTTON_OK        (GUI_ID_USER + 0x10 + 4)

#define ID_BUTTON_BSP       (GUI_ID_USER + 0x10 + 5)
#define ID_BUTTON_TAB       (GUI_ID_USER + 0x10 + 6)
#define ID_BUTTON_CAPS      (GUI_ID_USER + 0x10 + 7)
#define ID_BUTTON_SHT1      (GUI_ID_USER + 0x10 + 8)
#define ID_BUTTON_SHT2      (GUI_ID_USER + 0x10 + 9)
#define ID_BUTTON_ENT       (GUI_ID_USER + 0x10 + 10)
#define ID_BUTTON_SPC       (GUI_ID_USER + 0x10 + 11)
#define ID_BUTTON_LANG      (GUI_ID_USER + 0x10 + 12)

#define ID_MULTIEDIT_00     (GUI_ID_USER + 0x10 + 13)

#define ID_BUTTON_00        (GUI_ID_USER + 0x40 + 0)
#define ID_BUTTON_01        (GUI_ID_USER + 0x40 + 1)
#define ID_BUTTON_02        (GUI_ID_USER + 0x40 + 2)
#define ID_BUTTON_03        (GUI_ID_USER + 0x40 + 3)
#define ID_BUTTON_04        (GUI_ID_USER + 0x40 + 4)
#define ID_BUTTON_05        (GUI_ID_USER + 0x40 + 5)
#define ID_BUTTON_06        (GUI_ID_USER + 0x40 + 6)
#define ID_BUTTON_07        (GUI_ID_USER + 0x40 + 7)
#define ID_BUTTON_08        (GUI_ID_USER + 0x40 + 8)
#define ID_BUTTON_09        (GUI_ID_USER + 0x40 + 9)
#define ID_BUTTON_10        (GUI_ID_USER + 0x40 + 10)
#define ID_BUTTON_11        (GUI_ID_USER + 0x40 + 11)
#define ID_BUTTON_12        (GUI_ID_USER + 0x40 + 12)
#define ID_BUTTON_13        (GUI_ID_USER + 0x40 + 13)
#define ID_BUTTON_14        (GUI_ID_USER + 0x40 + 14)
#define ID_BUTTON_15        (GUI_ID_USER + 0x40 + 15)
#define ID_BUTTON_16        (GUI_ID_USER + 0x40 + 16)
#define ID_BUTTON_17        (GUI_ID_USER + 0x40 + 17)
#define ID_BUTTON_18        (GUI_ID_USER + 0x40 + 18)
#define ID_BUTTON_19        (GUI_ID_USER + 0x40 + 19)
#define ID_BUTTON_20        (GUI_ID_USER + 0x40 + 20)
#define ID_BUTTON_21        (GUI_ID_USER + 0x40 + 21)
#define ID_BUTTON_22        (GUI_ID_USER + 0x40 + 22)
#define ID_BUTTON_23        (GUI_ID_USER + 0x40 + 23)
#define ID_BUTTON_24        (GUI_ID_USER + 0x40 + 24)
#define ID_BUTTON_25        (GUI_ID_USER + 0x40 + 25)
#define ID_BUTTON_26        (GUI_ID_USER + 0x40 + 26)
#define ID_BUTTON_27        (GUI_ID_USER + 0x40 + 27)
#define ID_BUTTON_28        (GUI_ID_USER + 0x40 + 28)
#define ID_BUTTON_29        (GUI_ID_USER + 0x40 + 29)
#define ID_BUTTON_30        (GUI_ID_USER + 0x40 + 30)
#define ID_BUTTON_31        (GUI_ID_USER + 0x40 + 31)
#define ID_BUTTON_32        (GUI_ID_USER + 0x40 + 32)
#define ID_BUTTON_33        (GUI_ID_USER + 0x40 + 33)
#define ID_BUTTON_34        (GUI_ID_USER + 0x40 + 34)
#define ID_BUTTON_35        (GUI_ID_USER + 0x40 + 35)
#define ID_BUTTON_36        (GUI_ID_USER + 0x40 + 36)
#define ID_BUTTON_37        (GUI_ID_USER + 0x40 + 37)
#define ID_BUTTON_38        (GUI_ID_USER + 0x40 + 38)
#define ID_BUTTON_39        (GUI_ID_USER + 0x40 + 39)
#define ID_BUTTON_40        (GUI_ID_USER + 0x40 + 40)
#define ID_BUTTON_41        (GUI_ID_USER + 0x40 + 41)
#define ID_BUTTON_42        (GUI_ID_USER + 0x40 + 42)
#define ID_BUTTON_43        (GUI_ID_USER + 0x40 + 43)
#define ID_BUTTON_44        (GUI_ID_USER + 0x40 + 44)
#define ID_BUTTON_45        (GUI_ID_USER + 0x40 + 45)
#define ID_BUTTON_46        (GUI_ID_USER + 0x40 + 46)

//*************************************************************************************************
// Локальные константы
//*************************************************************************************************
#define BUTTON_MASK         0xFFFFFFC0
#define BUTTON_MASK_ID      0x0000003F

#define BUTTON_SIZE1_X      35          //размер обычной кнопки
#define BUTTON_SIZE1_Y      35

#define BUTTON_SIZE2_X      55          //размер кнопки TAB
#define BUTTON_SIZE3_X      65          //размер кнопки CAPS
#define BUTTON_SIZE4_X      85          //размер кнопки SHIFT
#define BUTTON_SIZE5_X      85          //размер кнопки ENTER
#define BUTTON_SIZE6_X      195         //размер кнопки SPACE
#define BUTTON_SIZE7_X      60          //размер кнопки LANG

#define BEGIN_DISTANCE1     10          //позиция начала размещения объекта по X
#define PAD_DISTANCE2       5           //расстояние между соседними кнопками
#define BEGIN_FIRST_STR     70


#define DIGITAL_POS_X( n )  ( BUTTON_SIZE1_X * ( n - 1 ) + BEGIN_DISTANCE1 + PAD_DISTANCE2 * ( n - 1 ) )
#define DIGITAL_POS_Y( n )  ( BUTTON_SIZE1_Y * n ) + PAD_DISTANCE2 * ( n - 1 ) + BEGIN_FIRST_STR

#define SIZE_EDIT_TEXT      4096        //размер текстового буфера для ID_MULTIEDIT_00

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
static int org_x, org_y;
static uint8_t in_len;
static char *in_value, *ptr;
static bool caps, shift, lang;
static char result[SIZE_EDIT_TEXT] __attribute__ ((at(EDIT_RAM_ADDRES)));

static char * const smode0 = "`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./"; //ENG
static char * const smode1 = "~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?"; //ENG+SHIFT
static char * const smode2 = "`1234567890-=QWERTYUIOP[]\\ASDFGHJKL;'ZXCVBNM,./"; //ENG+CAPS
static char * const smode3 = "~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?"; //ENG+CAPS+SHIFT
static char * const smode4 = "ё1234567890-=йцукенгшщзхъ\\фывапролджэячсмитьбю."; //RUS
static char * const smode5 = "Ё!\"№;%:?*()_+ЙЦУКЕНГШЩЗХЪ/ФЫВАПРОЛДЖЭЯЧСМИТЬБЮ,"; //RUS+SHIFT
static char * const smode6 = "Ё1234567890-=ЙЦУКЕНГШЩЗХЪ\\ФЫВАПРОЛДЖЭЯЧСМИТЬБЮ."; //RUS+CAPS
static char * const smode7 = "ё!\"№;%:?*()_+йцукенгшщзхъ/фывапролджэячсмитьбю,"; //RUS+CAPS+SHIFT

static const GUI_WIDGET_CREATE_INFO DialogCreate[] = {
    { FRAMEWIN_CreateIndirect, "Клавиатура", ID_FRAMEWIN_0, WIN_POS_X( WIN_SIZE_X ), WIN_POS_Y( WIN_SIZE_Y ), WIN_SIZE_X, WIN_SIZE_Y },
    { MULTIEDIT_CreateIndirect, "", ID_MULTIEDIT_00, TEXT_DISTANCE, TEXT_DISTANCE, 575, 80, 0, SIZE_EDIT_TEXT, 0 },
    { BUTTON_CreateIndirect, "Закрыть", ID_BUTTON_CLS,  BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "OK",      ID_BUTTON_OK,   BUTTON_POS_X( 2, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    //дополнительные кнопки
    { BUTTON_CreateIndirect, "BSP",   ID_BUTTON_BSP,  DIGITAL_POS_X( 14 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE2_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "TAB",   ID_BUTTON_TAB,  DIGITAL_POS_X( 1 ),  DIGITAL_POS_Y( 2 ), BUTTON_SIZE2_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "CAPS",  ID_BUTTON_CAPS, DIGITAL_POS_X( 1 ),  DIGITAL_POS_Y( 3 ), BUTTON_SIZE3_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "SHIFT", ID_BUTTON_SHT1, DIGITAL_POS_X( 1 ),  DIGITAL_POS_Y( 4 ), BUTTON_SIZE4_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "ENTER", ID_BUTTON_ENT,  DIGITAL_POS_X( 14 ) - PAD_DISTANCE2 * 2, DIGITAL_POS_Y( 3 ), BUTTON_SIZE3_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "SHIFT", ID_BUTTON_SHT2, DIGITAL_POS_X( 13 ) + PAD_DISTANCE2 * 2, DIGITAL_POS_Y( 4 ), BUTTON_SIZE4_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "SPACE", ID_BUTTON_SPC,  DIGITAL_POS_X( 5 ),  DIGITAL_POS_Y( 5 ), BUTTON_SIZE6_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "RUS",   ID_BUTTON_LANG, DIGITAL_POS_X( 14 ), DIGITAL_POS_Y( 5 ), BUTTON_SIZE7_X, BUTTON_SIZE1_Y },
    //первая строка кнопок
    { BUTTON_CreateIndirect, "00", ID_BUTTON_00, DIGITAL_POS_X( 1 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "01", ID_BUTTON_01, DIGITAL_POS_X( 2 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "02", ID_BUTTON_02, DIGITAL_POS_X( 3 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "03", ID_BUTTON_03, DIGITAL_POS_X( 4 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "04", ID_BUTTON_04, DIGITAL_POS_X( 5 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "05", ID_BUTTON_05, DIGITAL_POS_X( 6 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "06", ID_BUTTON_06, DIGITAL_POS_X( 7 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "07", ID_BUTTON_07, DIGITAL_POS_X( 8 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "08", ID_BUTTON_08, DIGITAL_POS_X( 9 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "09", ID_BUTTON_09, DIGITAL_POS_X( 10 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "10", ID_BUTTON_10, DIGITAL_POS_X( 11 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "11", ID_BUTTON_11, DIGITAL_POS_X( 12 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "12", ID_BUTTON_12, DIGITAL_POS_X( 13 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    //вторая строка кнопок
    { BUTTON_CreateIndirect, "13", ID_BUTTON_13, DIGITAL_POS_X( 1 ) + BUTTON_SIZE2_X + PAD_DISTANCE2, DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "14", ID_BUTTON_14, DIGITAL_POS_X( 2 ) + BUTTON_SIZE2_X + PAD_DISTANCE2, DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "15", ID_BUTTON_15, DIGITAL_POS_X( 3 ) + BUTTON_SIZE2_X + PAD_DISTANCE2, DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "16", ID_BUTTON_16, DIGITAL_POS_X( 4 ) + BUTTON_SIZE2_X + PAD_DISTANCE2, DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "17", ID_BUTTON_17, DIGITAL_POS_X( 5 ) + BUTTON_SIZE2_X + PAD_DISTANCE2, DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "18", ID_BUTTON_18, DIGITAL_POS_X( 6 ) + BUTTON_SIZE2_X + PAD_DISTANCE2, DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "19", ID_BUTTON_19, DIGITAL_POS_X( 7 ) + BUTTON_SIZE2_X + PAD_DISTANCE2, DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "20", ID_BUTTON_20, DIGITAL_POS_X( 8 ) + BUTTON_SIZE2_X + PAD_DISTANCE2, DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "21", ID_BUTTON_21, DIGITAL_POS_X( 9 ) + BUTTON_SIZE2_X + PAD_DISTANCE2, DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "22", ID_BUTTON_22, DIGITAL_POS_X( 10 ) + BUTTON_SIZE2_X + PAD_DISTANCE2, DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "23", ID_BUTTON_23, DIGITAL_POS_X( 11 ) + BUTTON_SIZE2_X + PAD_DISTANCE2, DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "24", ID_BUTTON_24, DIGITAL_POS_X( 12 ) + BUTTON_SIZE2_X + PAD_DISTANCE2, DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "25", ID_BUTTON_25, DIGITAL_POS_X( 13 ) + BUTTON_SIZE2_X + PAD_DISTANCE2, DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    //третья строка кнопок
    { BUTTON_CreateIndirect, "26", ID_BUTTON_26, DIGITAL_POS_X( 1 ) + BUTTON_SIZE3_X + PAD_DISTANCE2, DIGITAL_POS_Y( 3 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "27", ID_BUTTON_27, DIGITAL_POS_X( 2 ) + BUTTON_SIZE3_X + PAD_DISTANCE2, DIGITAL_POS_Y( 3 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "28", ID_BUTTON_28, DIGITAL_POS_X( 3 ) + BUTTON_SIZE3_X + PAD_DISTANCE2, DIGITAL_POS_Y( 3 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "29", ID_BUTTON_29, DIGITAL_POS_X( 4 ) + BUTTON_SIZE3_X + PAD_DISTANCE2, DIGITAL_POS_Y( 3 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "30", ID_BUTTON_30, DIGITAL_POS_X( 5 ) + BUTTON_SIZE3_X + PAD_DISTANCE2, DIGITAL_POS_Y( 3 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "31", ID_BUTTON_31, DIGITAL_POS_X( 6 ) + BUTTON_SIZE3_X + PAD_DISTANCE2, DIGITAL_POS_Y( 3 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "32", ID_BUTTON_32, DIGITAL_POS_X( 7 ) + BUTTON_SIZE3_X + PAD_DISTANCE2, DIGITAL_POS_Y( 3 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "33", ID_BUTTON_33, DIGITAL_POS_X( 8 ) + BUTTON_SIZE3_X + PAD_DISTANCE2, DIGITAL_POS_Y( 3 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "34", ID_BUTTON_34, DIGITAL_POS_X( 9 ) + BUTTON_SIZE3_X + PAD_DISTANCE2, DIGITAL_POS_Y( 3 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "35", ID_BUTTON_35, DIGITAL_POS_X( 10 ) + BUTTON_SIZE3_X + PAD_DISTANCE2, DIGITAL_POS_Y( 3 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "36", ID_BUTTON_36, DIGITAL_POS_X( 11 ) + BUTTON_SIZE3_X + PAD_DISTANCE2, DIGITAL_POS_Y( 3 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    //четвертая строка кнопок
    { BUTTON_CreateIndirect, "37", ID_BUTTON_37, DIGITAL_POS_X( 1 ) + BUTTON_SIZE4_X + PAD_DISTANCE2, DIGITAL_POS_Y( 4 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "38", ID_BUTTON_38, DIGITAL_POS_X( 2 ) + BUTTON_SIZE4_X + PAD_DISTANCE2, DIGITAL_POS_Y( 4 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "39", ID_BUTTON_39, DIGITAL_POS_X( 3 ) + BUTTON_SIZE4_X + PAD_DISTANCE2, DIGITAL_POS_Y( 4 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "40", ID_BUTTON_40, DIGITAL_POS_X( 4 ) + BUTTON_SIZE4_X + PAD_DISTANCE2, DIGITAL_POS_Y( 4 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "41", ID_BUTTON_41, DIGITAL_POS_X( 5 ) + BUTTON_SIZE4_X + PAD_DISTANCE2, DIGITAL_POS_Y( 4 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "42", ID_BUTTON_42, DIGITAL_POS_X( 6 ) + BUTTON_SIZE4_X + PAD_DISTANCE2, DIGITAL_POS_Y( 4 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "43", ID_BUTTON_43, DIGITAL_POS_X( 7 ) + BUTTON_SIZE4_X + PAD_DISTANCE2, DIGITAL_POS_Y( 4 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "44", ID_BUTTON_44, DIGITAL_POS_X( 8 ) + BUTTON_SIZE4_X + PAD_DISTANCE2, DIGITAL_POS_Y( 4 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "45", ID_BUTTON_45, DIGITAL_POS_X( 9 ) + BUTTON_SIZE4_X + PAD_DISTANCE2, DIGITAL_POS_Y( 4 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "46", ID_BUTTON_46, DIGITAL_POS_X( 10 ) + BUTTON_SIZE4_X + PAD_DISTANCE2, DIGITAL_POS_Y( 4 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y }
 };

//*************************************************************************************************
// Функция возвращает адрес массива char содержащего символы для клавиш клавиатуры
// в зависимости от значений SHIFT, CAPS, LANGUAGE
// ------0-1-2-3-4-5-6-7
// SHIFT 0 1 0 1 0 1 0 1
// CAPS  0 0 1 1 0 0 1 1
// RUS   0 0 0 0 1 1 1 1
//*************************************************************************************************
static const char *GetSymbolMode( void ) {

    uint8_t value = 0;

    if ( shift )
        value |= 0x01;
    if ( caps )
        value |= 0x02;
    if ( lang )
        value |= 0x04;
    switch ( value ) {
        case 0:  return smode0;
        case 1:  return smode1;
        case 2:  return smode2;
        case 3:  return smode3;
        case 4:  return smode4;
        case 5:  return smode5;
        case 6:  return smode6;
        case 7:  return smode7;
        default: return smode0;
       }
 }

//*************************************************************************************************
// Функция обновления данных в форме
//*************************************************************************************************
static void UpdateWidget( WM_HWIN hwnd ) {

    char tmp[3];
    uint32_t id_btn;
    
    memset( tmp, 0x00, sizeof( tmp ) );
    ptr = (char *)GetSymbolMode();
    //установка значений для кнопок
    for ( id_btn = 0; id_btn <= ( ID_BUTTON_46 - ID_BUTTON_00 ); id_btn++ ) {
        tmp[0] = *( ptr + id_btn );
        BUTTON_SetText( WM_GetDialogItem( hwnd, id_btn + ID_BUTTON_00 ), tmp );
      }
 }

//*************************************************************************************************
// Функция обратного вызова, обработка сообщений
//*************************************************************************************************
static void cb_Dialog( WM_MESSAGE *pMsg ) {

    uint32_t index;
    WM_HWIN hMulti;
    WM_MESSAGE msg;
    int id_widg, ntf_code;

    hMulti = WM_GetDialogItem( pMsg->hWin, ID_MULTIEDIT_00 );
    switch ( pMsg->MsgId ) {
        case WM_INIT_DIALOG:
            //сообщим указатель (HWND) модального окна, окну из которого оно было вызвано
            msg.Data.v = DialogCreate[0].Id;
            msg.MsgId = WM_MESSAGE_MODAL_OPEN;              //тип сообщения
            msg.hWinSrc = WM_GetClientWindow( pMsg->hWin ); //указатель (HWND) модального окна
            WM_SendMessage( hwin0, &msg );
            WM_MakeModal( pMsg->hWin );
            WM_SetWindowPos( pMsg->hWin, org_x + DialogCreate[0].x0, org_y + DialogCreate[0].y0, DialogCreate[0].xSize, DialogCreate[0].ySize );
            MULTIEDIT_SetText( hMulti, in_value );
            MULTIEDIT_SetMaxNumChars( hMulti, in_len );
            MULTIEDIT_SetFont( hMulti, &GUI_FontTahoma16b );
            MULTIEDIT_SetWrapWord( hMulti );
            MULTIEDIT_SetAutoScrollV( hMulti, 1 );
            MULTIEDIT_SetInsertMode( hMulti, 1 );
            MULTIEDIT_EnableBlink( hMulti, 10, 1 );
            //установка шрифта для символьных кнопок
            for ( index = ID_BUTTON_00; index <= ID_BUTTON_46; index++ )
                BUTTON_SetFont( WM_GetDialogItem( pMsg->hWin, index ), &GUI_FontTahoma16b );
            BUTTON_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_BSP ), &GUI_FontTahoma16b );
            BUTTON_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_TAB ), &GUI_FontTahoma16b );
            BUTTON_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_CAPS ), &GUI_FontTahoma16b );
            BUTTON_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_SHT1 ), &GUI_FontTahoma16b );
            BUTTON_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_SHT2 ), &GUI_FontTahoma16b );
            BUTTON_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_ENT ), &GUI_FontTahoma16b );
            BUTTON_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_SPC ), &GUI_FontTahoma16b );
            BUTTON_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_LANG ), &GUI_FontTahoma16b );
            UpdateWidget( pMsg->hWin );
            break;
        case WM_NOTIFY_PARENT:
            id_widg = WM_GetId( pMsg->hWinSrc );
            ntf_code = pMsg->Data.v;
            //обработка событий кнопок
            if ( id_widg == ID_BUTTON_CLS && ntf_code == WM_NOTIFICATION_CLICKED ) {
                memset( result, 0x00, sizeof( result ) );
                //пользователь закрыл модальное окно
                msg.Data.v = DialogCreate[0].Id;
                msg.MsgId = WM_MESSAGE_MODAL_END;
                msg.hWinSrc = WM_GetClientWindow( pMsg->hWin );
                //сообщение окну из которого было вызвано модальное окно
                WM_SendMessage( hwin0, &msg );
                GUI_EndDialog( pMsg->hWin, 0 ); //закрываем форму
               }
            if ( id_widg == ID_BUTTON_OK && ntf_code == WM_NOTIFICATION_CLICKED ) {
                memset( result, 0x00, sizeof( result ) );
                MULTIEDIT_GetText( hMulti, result, sizeof( result ) );
                //пользователь закрыл модальное окно
                msg.Data.v = DialogCreate[0].Id;
                msg.MsgId = WM_MESSAGE_MODAL_END;
                msg.hWinSrc = WM_GetClientWindow( pMsg->hWin );
                //сообщение окну из которого было вызвано модальное окно
                WM_SendMessage( hwin0, &msg );
                GUI_EndDialog( pMsg->hWin, 1 ); //закрываем форму
               }
            if ( id_widg == ID_BUTTON_BSP && ntf_code == WM_NOTIFICATION_CLICKED )
                MULTIEDIT_AddKey( hMulti, GUI_KEY_BACKSPACE );
            if ( id_widg == ID_BUTTON_TAB && ntf_code == WM_NOTIFICATION_CLICKED )
                MULTIEDIT_AddKey( hMulti, GUI_KEY_TAB );
            if ( id_widg == ID_BUTTON_ENT && ntf_code == WM_NOTIFICATION_CLICKED )
                MULTIEDIT_AddKey( hMulti, GUI_KEY_ENTER );
            if ( id_widg == ID_BUTTON_SPC && ntf_code == WM_NOTIFICATION_CLICKED )
                MULTIEDIT_AddKey( hMulti, GUI_KEY_SPACE );
            //обработка нажатия клавиши CAPS
            if ( id_widg == ID_BUTTON_CAPS && ntf_code == WM_NOTIFICATION_CLICKED ) {
                caps = !caps;
                if ( caps )
                    BUTTON_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_CAPS ), BUTTON_CI_UNPRESSED, GUI_RED );
                else BUTTON_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_CAPS ), BUTTON_CI_UNPRESSED, GUI_BLACK );
                UpdateWidget( pMsg->hWin );
               }
            //обработка нажатия клавиши SHIFT
            if ( ( id_widg == ID_BUTTON_SHT1 || id_widg == ID_BUTTON_SHT2 ) && ntf_code == WM_NOTIFICATION_CLICKED ) {
                shift = !shift;
                if ( shift ) {
                    BUTTON_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_SHT1 ), BUTTON_CI_UNPRESSED, GUI_RED );
                    BUTTON_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_SHT2 ), BUTTON_CI_UNPRESSED, GUI_RED );
                   }
                else { 
                    BUTTON_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_SHT1 ), BUTTON_CI_UNPRESSED, GUI_BLACK );
                    BUTTON_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_SHT2 ), BUTTON_CI_UNPRESSED, GUI_BLACK );
                   }
                UpdateWidget( pMsg->hWin );
               }
            //обработка нажатия клавиши LANG переключение языка
            if ( id_widg == ID_BUTTON_LANG && ntf_code == WM_NOTIFICATION_CLICKED ) {
                lang = !lang;
                if ( lang )
                    BUTTON_SetText( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_LANG ), "ENG" );
                else BUTTON_SetText( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_LANG ), "RUS" );
                UpdateWidget( pMsg->hWin );
               }
            //цифровые-символьные кнопки
            if ( ( id_widg & BUTTON_MASK ) == ID_BUTTON_00 && ntf_code == WM_NOTIFICATION_CLICKED ) {
                ptr = (char *)GetSymbolMode();
                MULTIEDIT_AddKey( hMulti, *( ptr + ( id_widg & BUTTON_MASK_ID ) ) );
                shift = false;
                UpdateWidget( pMsg->hWin );
                BUTTON_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_SHT1 ), BUTTON_CI_UNPRESSED, GUI_BLACK );
                BUTTON_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_SHT2 ), BUTTON_CI_UNPRESSED, GUI_BLACK );
                UpdateWidget( pMsg->hWin );
               }
            break;
        case WM_MESSAGE_MODAL_CLOSE:
            //сообщение от основного окна на принудительное закрытие модального окна
            GUI_EndDialog( pMsg->hWin, 0 );
            break;
        default:
            WM_DefaultProc( pMsg );
            break;
       }
 }

//*************************************************************************************************
// Основная функция вызова формы
// char *value     - исходная строка для размещения
// uint8_t max_len - максимальное кол-во символов ввода
//*************************************************************************************************
char *WinKeyBrd( char *value, uint8_t max_len ) {

    caps = false;
    shift = false; 
    lang = false;
    in_value = value;
    in_len = max_len;
    GUI_GetOrg( &org_x, &org_y ); //координаты текущего виртуального окна
    if ( GUI_ExecDialogBox( DialogCreate, GUI_COUNTOF( DialogCreate ), &cb_Dialog, 0, 0, 0 ) )
        return result;
    else return NULL;
 } 
