
//*************************************************************************************************
//
// Форма ввода цифровых значений
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
#define WIN_SIZE_X          250
#define WIN_SIZE_Y          300

//*************************************************************************************************
// Определение идентификаторов объектов в форме
//*************************************************************************************************
#define ID_FRAMEWIN_0       (GUI_ID_USER + 16)

#define ID_BUTTON_CLS       (GUI_ID_USER + 0x10 + 1)
#define ID_BUTTON_BSP       (GUI_ID_USER + 0x10 + 2)
#define ID_BUTTON_OK        (GUI_ID_USER + 0x10 + 3)

#define ID_EDIT_00          (GUI_ID_USER + 0x10 + 4)

#define ID_BUTTON_0         (GUI_ID_USER + 0x20 + 0)
#define ID_BUTTON_1         (GUI_ID_USER + 0x20 + 1)
#define ID_BUTTON_2         (GUI_ID_USER + 0x20 + 2)
#define ID_BUTTON_3         (GUI_ID_USER + 0x20 + 3)
#define ID_BUTTON_4         (GUI_ID_USER + 0x20 + 4)
#define ID_BUTTON_5         (GUI_ID_USER + 0x20 + 5)
#define ID_BUTTON_6         (GUI_ID_USER + 0x20 + 6)
#define ID_BUTTON_7         (GUI_ID_USER + 0x20 + 7)
#define ID_BUTTON_8         (GUI_ID_USER + 0x20 + 8)
#define ID_BUTTON_9         (GUI_ID_USER + 0x20 + 9)
#define ID_BUTTON_A         (GUI_ID_USER + 0x20 + 10)
#define ID_BUTTON_B         (GUI_ID_USER + 0x20 + 11)
#define ID_BUTTON_C         (GUI_ID_USER + 0x20 + 12)
#define ID_BUTTON_D         (GUI_ID_USER + 0x20 + 13)
#define ID_BUTTON_E         (GUI_ID_USER + 0x20 + 14)
#define ID_BUTTON_F         (GUI_ID_USER + 0x20 + 15)
#define ID_BUTTON_DOT       (GUI_ID_USER + 0x20 + 16)
#define ID_BUTTON_PLS       (GUI_ID_USER + 0x20 + 17)
#define ID_BUTTON_MIN       (GUI_ID_USER + 0x20 + 18)
#define ID_BUTTON_SLH       (GUI_ID_USER + 0x20 + 19)
#define ID_BUTTON_STR       (GUI_ID_USER + 0x20 + 20)

//*************************************************************************************************
// Локальные константы
//*************************************************************************************************
#define BUTTON_MASK         0xFFFFFFE0
#define BUTTON_MASK_ID      0x0000001F

#define BUTTON_SIZE1_X      40          //размер обычной кнопки
#define BUTTON_SIZE1_Y      40

#define BUTTON_SIZE2_X      80          //размер увеличенной кнопки
#define BUTTON_SIZE2_Y      40

#define BEGIN_DISTANCE1     10          //позиция начала размещения объекта
#define PAD_DISTANCE2       5           //расстояние между соседними объектами

#define DIGITAL_POS_X( n )  ( BUTTON_SIZE1_X * ( n - 1 ) + BEGIN_DISTANCE1 + PAD_DISTANCE2 * ( n - 1 ) )
#define DIGITAL_POS_Y( n )  ( BUTTON_SIZE1_Y * n ) + PAD_DISTANCE2 * ( n - 1 )

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
static int org_x, org_y;
static char *in_value, result[100], warn[80];
static uint8_t in_len, in_mode;
static int32_t in_min, in_max;

static char symbols[] = { "0123456789ABCDEF.+-/*" };

static const GUI_WIDGET_CREATE_INFO DialogCreate[] = {
    { FRAMEWIN_CreateIndirect, "Цифровая клавиатура", ID_FRAMEWIN_0, WIN_POS_X( WIN_SIZE_X ), WIN_POS_Y( WIN_SIZE_Y ), WIN_SIZE_X, WIN_SIZE_Y },

    { EDIT_CreateIndirect, "", ID_EDIT_00, TEXT_DISTANCE, TEXT_DISTANCE, 220, 20, 0, 100, 0 },

    { BUTTON_CreateIndirect, "A",   ID_BUTTON_A,   DIGITAL_POS_X( 1 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "B",   ID_BUTTON_B,   DIGITAL_POS_X( 2 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "C",   ID_BUTTON_C,   DIGITAL_POS_X( 3 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "D",   ID_BUTTON_D,   DIGITAL_POS_X( 4 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "BSP", ID_BUTTON_BSP, DIGITAL_POS_X( 5 ), DIGITAL_POS_Y( 1 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
                                                                                                                         
    { BUTTON_CreateIndirect, "7",   ID_BUTTON_7,   DIGITAL_POS_X( 1 ), DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "8",   ID_BUTTON_8,   DIGITAL_POS_X( 2 ), DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "9",   ID_BUTTON_9,   DIGITAL_POS_X( 3 ), DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "E",   ID_BUTTON_E,   DIGITAL_POS_X( 4 ), DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "*",   ID_BUTTON_STR, DIGITAL_POS_X( 5 ), DIGITAL_POS_Y( 2 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
                                                                                                                         
    { BUTTON_CreateIndirect, "4",   ID_BUTTON_4,   DIGITAL_POS_X( 1 ), DIGITAL_POS_Y( 3 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "5",   ID_BUTTON_5,   DIGITAL_POS_X( 2 ), DIGITAL_POS_Y( 3 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "6",   ID_BUTTON_6,   DIGITAL_POS_X( 3 ), DIGITAL_POS_Y( 3 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "F",   ID_BUTTON_F,   DIGITAL_POS_X( 4 ), DIGITAL_POS_Y( 3 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "/",   ID_BUTTON_SLH, DIGITAL_POS_X( 5 ), DIGITAL_POS_Y( 3 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
                                                                                                                         
    { BUTTON_CreateIndirect, "1",   ID_BUTTON_1,   DIGITAL_POS_X( 1 ), DIGITAL_POS_Y( 4 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "2",   ID_BUTTON_2,   DIGITAL_POS_X( 2 ), DIGITAL_POS_Y( 4 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "3",   ID_BUTTON_3,   DIGITAL_POS_X( 3 ), DIGITAL_POS_Y( 4 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "+",   ID_BUTTON_PLS, DIGITAL_POS_X( 4 ), DIGITAL_POS_Y( 4 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "OK",  ID_BUTTON_OK,  DIGITAL_POS_X( 5 ), DIGITAL_POS_Y( 4 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
                                                                                                                         
    { BUTTON_CreateIndirect, "0",   ID_BUTTON_0,   DIGITAL_POS_X( 1 ), DIGITAL_POS_Y( 5 ), BUTTON_SIZE2_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, ".",   ID_BUTTON_DOT, DIGITAL_POS_X( 3 ), DIGITAL_POS_Y( 5 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "-",   ID_BUTTON_MIN, DIGITAL_POS_X( 4 ), DIGITAL_POS_Y( 5 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y },
    { BUTTON_CreateIndirect, "CLS", ID_BUTTON_CLS, DIGITAL_POS_X( 5 ), DIGITAL_POS_Y( 5 ), BUTTON_SIZE1_X, BUTTON_SIZE1_Y }
 };

//*************************************************************************************************
// Функция обратного вызова, обработка сообщений
//*************************************************************************************************
static void cb_Dialog( WM_MESSAGE * pMsg ) {

    WM_MESSAGE msg;
    uint32_t ud, index;
    int value, id_widg, ntf_code;

    switch ( pMsg->MsgId ) {
        case WM_INIT_DIALOG:
            //сообщим указатель (HWND) модального окна, окну из которого оно было вызвано
            msg.Data.v = DialogCreate[0].Id;
            msg.MsgId = WM_MESSAGE_MODAL_OPEN;              //тип сообщения
            msg.hWinSrc = WM_GetClientWindow( pMsg->hWin ); //указатель (HWND) модального окна
            WM_SendMessage( hwin0, &msg );
            WM_MakeModal( pMsg->hWin );
            WM_SetWindowPos( pMsg->hWin, org_x + DialogCreate[0].x0, org_y + DialogCreate[0].y0, DialogCreate[0].xSize, DialogCreate[0].ySize );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_00 ), in_value );
            EDIT_SetMaxLen( WM_GetDialogItem( pMsg->hWin, ID_EDIT_00 ), in_len );
            EDIT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_EDIT_00 ), &GUI_FontTahoma16b );
            EDIT_EnableBlink( WM_GetDialogItem( pMsg->hWin, ID_EDIT_00 ), 10, 1 );
            //
            for ( index = ID_BUTTON_0; index <= ID_BUTTON_STR; index++ )
                BUTTON_SetFont( WM_GetDialogItem( pMsg->hWin, index ), &GUI_FontTahoma16b );
            BUTTON_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_BSP ), &GUI_FontTahoma16b );
            BUTTON_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_PLS ), &GUI_FontTahoma16b );
            BUTTON_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_OK ), &GUI_FontTahoma16b );
            BUTTON_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_CLS ), &GUI_FontTahoma16b );
            //
            if ( !( in_mode & KEY_DIGITAL ) ) {
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_0 ) );
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_1 ) );
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_2 ) );
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_3 ) );
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_4 ) );
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_5 ) );
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_6 ) );
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_7 ) );
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_8 ) );
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_9 ) );
               }
            if ( !( in_mode & KEY_HEX ) ) {
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_A ) );
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_B ) );
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_C ) );
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_D ) );
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_E ) );
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_F ) );
               }
            if ( !( in_mode & KEY_STR ) )
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_STR ) );
            if ( !( in_mode & KEY_SLH ) )
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_SLH ) );
            if ( !( in_mode & KEY_PLS ) )
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_PLS ) );
            if ( !( in_mode & KEY_DOT ) )
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_DOT ) );
            if ( !( in_mode & KEY_MIN ) )
                WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_MIN ) );
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
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_00 ), result, sizeof( result ) );
                //проверка введенного значения
                if ( in_min || in_max ) {
                    value = atoi( result );
                    if ( value < in_min || value > in_max ) {
                        sprintf( warn, MessageGui( MSG_GUI_ERR_RANGE ), result, in_min, in_max );
                        MessageBox( MESSAGE_WARNING, warn );
                        break;
                       }
                   }
                //пользователь закрыл модальное окно
                msg.Data.v = DialogCreate[0].Id;
                msg.MsgId = WM_MESSAGE_MODAL_END;
                msg.hWinSrc = WM_GetClientWindow( pMsg->hWin );
                //сообщение окну из которого было вызвано модальное окно
                WM_SendMessage( hwin0, &msg );
                GUI_EndDialog( pMsg->hWin, 1 ); //закрываем форму
               }
            if ( id_widg == ID_BUTTON_BSP && ntf_code == WM_NOTIFICATION_CLICKED )
                EDIT_AddKey( WM_GetDialogItem( pMsg->hWin, ID_EDIT_00 ), GUI_KEY_BACKSPACE );
            //цифровые-символьные кнопки
            if ( ( id_widg & BUTTON_MASK ) == ID_BUTTON_0 && ntf_code == WM_NOTIFICATION_CLICKED ) {
                ud = id_widg & BUTTON_MASK_ID;
                if ( ud < sizeof( symbols ) )
                    EDIT_AddKey( WM_GetDialogItem( pMsg->hWin, ID_EDIT_00 ), symbols[ud] );
               }
            break;
        case WM_MESSAGE_MODAL_CLOSE:
            //сообщение от основного окна на принудительное закрытие модального окна
            GUI_EndDialog( pMsg->hWin, 0 );
            break;
        default:
            WM_DefaultProc(pMsg);
            break;
       }
 }

//*************************************************************************************************
// Основная функция вызова формы цифровой клавиатуры
// char *value      - исходное значение строки
// uint8_t max_len  - максимальное кол-во символов
// uint8_t mode     - режим отображения кнопок
// uint32_t val_min - минимальное значение (для контроля)
// uint32_t val_max - максимальное значение (для контроля)
// Контроль введенного значения выполняется только, если одно из значений val_min или val_max 
// больше нуля и только для целый чисел
//*************************************************************************************************
char *WinKeyPad( char *value, uint8_t max_len, uint8_t mode, int32_t val_min, int32_t val_max ) {

    in_value = value;
    in_len = max_len;
    in_mode = mode;
    in_min = val_min;
    in_max = val_max;
    GUI_GetOrg( &org_x, &org_y ); //координаты текущего виртуального окна
    if ( GUI_ExecDialogBox( DialogCreate, GUI_COUNTOF( DialogCreate ), &cb_Dialog, 0, 0, 0 ) )
        return result;
    else return NULL;
 } 
