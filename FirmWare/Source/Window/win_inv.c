
//*************************************************************************************************
//
// Форма управления инверторами
//
//*************************************************************************************************

#include "window.h"
#include "dialog.h"

//*************************************************************************************************
// Внешние переменные
//*************************************************************************************************
extern WM_HWIN hwin0;

//*************************************************************************************************
// Размер окна
//*************************************************************************************************
#define WIN_SIZE_X      590
#define WIN_SIZE_Y      300

//*************************************************************************************************
// Определение идентификаторов объектов в форме
//*************************************************************************************************
#define ID_FRAMEWIN_0       (GUI_ID_USER + 5)

#define ID_BUTTON_CLS       (GUI_ID_USER + 0x10 + 1)
#define ID_BUTTON_1_ON      (GUI_ID_USER + 0x10 + 2)
#define ID_BUTTON_1_OFF     (GUI_ID_USER + 0x10 + 3)
#define ID_BUTTON_3_ON      (GUI_ID_USER + 0x10 + 4)
#define ID_BUTTON_3_OFF     (GUI_ID_USER + 0x10 + 5)

#define ID_NAME_00          (GUI_ID_USER + 0x20 + 0)
#define ID_NAME_01          (GUI_ID_USER + 0x20 + 1)
#define ID_NAME_02          (GUI_ID_USER + 0x20 + 2)
#define ID_NAME_03          (GUI_ID_USER + 0x20 + 3)
#define ID_NAME_04          (GUI_ID_USER + 0x20 + 4)
#define ID_NAME_05          (GUI_ID_USER + 0x20 + 5)
#define ID_NAME_06          (GUI_ID_USER + 0x20 + 6)
#define ID_NAME_07          (GUI_ID_USER + 0x20 + 7)
#define ID_NAME_08          (GUI_ID_USER + 0x20 + 8)
#define ID_NAME_09          (GUI_ID_USER + 0x20 + 9)

#define ID_VALUE_1_00       (GUI_ID_USER + 0x40 + 0)
#define ID_VALUE_1_01       (GUI_ID_USER + 0x40 + 1)
#define ID_VALUE_1_02       (GUI_ID_USER + 0x40 + 2)
#define ID_VALUE_1_03       (GUI_ID_USER + 0x40 + 3)
#define ID_VALUE_1_04       (GUI_ID_USER + 0x40 + 4)
#define ID_VALUE_1_05       (GUI_ID_USER + 0x40 + 5)
#define ID_VALUE_1_06       (GUI_ID_USER + 0x40 + 6)
#define ID_VALUE_1_07       (GUI_ID_USER + 0x40 + 7)
#define ID_VALUE_1_08       (GUI_ID_USER + 0x40 + 8)
#define ID_VALUE_1_09       (GUI_ID_USER + 0x40 + 9)

#define ID_VALUE_3_00       (GUI_ID_USER + 0x50 + 0)
#define ID_VALUE_3_01       (GUI_ID_USER + 0x50 + 1)
#define ID_VALUE_3_02       (GUI_ID_USER + 0x50 + 2)
#define ID_VALUE_3_03       (GUI_ID_USER + 0x50 + 3)
#define ID_VALUE_3_04       (GUI_ID_USER + 0x50 + 4)
#define ID_VALUE_3_05       (GUI_ID_USER + 0x50 + 5)
#define ID_VALUE_3_06       (GUI_ID_USER + 0x50 + 6)
#define ID_VALUE_3_07       (GUI_ID_USER + 0x50 + 7)
#define ID_VALUE_3_08       (GUI_ID_USER + 0x50 + 8)
#define ID_VALUE_3_09       (GUI_ID_USER + 0x50 + 9)

#define ID_UNIT_00          (GUI_ID_USER + 0x80 + 0)
#define ID_UNIT_01          (GUI_ID_USER + 0x80 + 1)
#define ID_UNIT_02          (GUI_ID_USER + 0x80 + 2)
#define ID_UNIT_03          (GUI_ID_USER + 0x80 + 3)
#define ID_UNIT_04          (GUI_ID_USER + 0x80 + 4)
#define ID_UNIT_05          (GUI_ID_USER + 0x80 + 5)
#define ID_UNIT_06          (GUI_ID_USER + 0x80 + 6)
#define ID_UNIT_07          (GUI_ID_USER + 0x80 + 7)
#define ID_UNIT_08          (GUI_ID_USER + 0x80 + 8)
#define ID_UNIT_09          (GUI_ID_USER + 0x80 + 9)

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
static int org_x, org_y;

static const GUI_WIDGET_CREATE_INFO DialogCreate[] = {
    { FRAMEWIN_CreateIndirect, "Инверторы", ID_FRAMEWIN_0, WIN_POS_X( WIN_SIZE_X ), WIN_POS_Y( WIN_SIZE_Y ), WIN_SIZE_X, WIN_SIZE_Y },
    { BUTTON_CreateIndirect, "Закрыть",  ID_BUTTON_CLS, BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },

    { BUTTON_CreateIndirect, "TS-3000 Выкл",  ID_BUTTON_3_OFF, BUTTON_POS_X( 2, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "TS-1000 Выкл", ID_BUTTON_1_OFF, BUTTON_POS_X( 3, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "TS-3000 Вкл",  ID_BUTTON_3_ON, BUTTON_POS_X( 2, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y-35 ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "TS-1000 Вкл", ID_BUTTON_1_ON, BUTTON_POS_X( 3, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y-35 ), BUTTON_SIZE_X, BUTTON_SIZE_Y },

    { TEXT_CreateIndirect, "Модель инвертора", ID_NAME_00, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 1 ), TS_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_01, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 2 ), TS_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_02, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 3 ), TS_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_03, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 4 ), TS_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_04, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 5 ), TS_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_05, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 6 ), TS_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_06, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 7 ), TS_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_07, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 8 ), TS_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_08, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 9 ), TS_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_09, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 10 ), TS_NAME_SIZE_X, TEXT_NAME_SIZE_Y },

    { TEXT_CreateIndirect, "TS-1000-224", ID_VALUE_1_00, TS_VALUE1_POS_X, TEXT_NAME_POS_Y( 1 ), TS_VALUE1_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_1_01, TS_VALUE1_POS_X, TEXT_NAME_POS_Y( 2 ), TS_VALUE1_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_1_02, TS_VALUE1_POS_X, TEXT_NAME_POS_Y( 3 ), TS_VALUE1_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_1_03, TS_VALUE1_POS_X, TEXT_NAME_POS_Y( 4 ), TS_VALUE1_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_1_04, TS_VALUE1_POS_X, TEXT_NAME_POS_Y( 5 ), TS_VALUE1_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_1_05, TS_VALUE1_POS_X, TEXT_NAME_POS_Y( 6 ), TS_VALUE1_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_1_06, TS_VALUE1_POS_X, TEXT_NAME_POS_Y( 7 ), TS_VALUE1_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_1_07, TS_VALUE1_POS_X, TEXT_NAME_POS_Y( 8 ), TS_VALUE1_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_1_08, TS_VALUE1_POS_X, TEXT_NAME_POS_Y( 9 ), TS_VALUE1_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_1_09, TS_VALUE1_POS_X, TEXT_NAME_POS_Y( 10 ), TS_VALUE1_SIZE_X, TS_VALUE_SIZE_Y2 },

    { TEXT_CreateIndirect, "TS-3000-224", ID_VALUE_3_00, TS_VALUE2_POS_X, TEXT_NAME_POS_Y( 1 ), TS_VALUE2_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_3_01, TS_VALUE2_POS_X, TEXT_NAME_POS_Y( 2 ), TS_VALUE2_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_3_02, TS_VALUE2_POS_X, TEXT_NAME_POS_Y( 3 ), TS_VALUE2_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_3_03, TS_VALUE2_POS_X, TEXT_NAME_POS_Y( 4 ), TS_VALUE2_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_3_04, TS_VALUE2_POS_X, TEXT_NAME_POS_Y( 5 ), TS_VALUE2_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_3_05, TS_VALUE2_POS_X, TEXT_NAME_POS_Y( 6 ), TS_VALUE2_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_3_06, TS_VALUE2_POS_X, TEXT_NAME_POS_Y( 7 ), TS_VALUE2_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_3_07, TS_VALUE2_POS_X, TEXT_NAME_POS_Y( 8 ), TS_VALUE2_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_3_08, TS_VALUE2_POS_X, TEXT_NAME_POS_Y( 9 ), TS_VALUE2_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_3_09, TS_VALUE2_POS_X, TEXT_NAME_POS_Y( 10 ), TS_VALUE2_SIZE_X, TS_VALUE_SIZE_Y2 },

    { TEXT_CreateIndirect, "", ID_UNIT_00, TS_UNIT_POS_X, TEXT_NAME_POS_Y( 1 ), TS_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_01, TS_UNIT_POS_X, TEXT_NAME_POS_Y( 2 ), TS_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_02, TS_UNIT_POS_X, TEXT_NAME_POS_Y( 3 ), TS_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_03, TS_UNIT_POS_X, TEXT_NAME_POS_Y( 4 ), TS_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_04, TS_UNIT_POS_X, TEXT_NAME_POS_Y( 5 ), TS_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_05, TS_UNIT_POS_X, TEXT_NAME_POS_Y( 6 ), TS_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_06, TS_UNIT_POS_X, TEXT_NAME_POS_Y( 7 ), TS_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_07, TS_UNIT_POS_X, TEXT_NAME_POS_Y( 8 ), TS_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_08, TS_UNIT_POS_X, TEXT_NAME_POS_Y( 9 ), TS_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_09, TS_UNIT_POS_X, TEXT_NAME_POS_Y( 10 ), TS_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y }
 };

//*************************************************************************************************
// Функция обновления данных в форме
//*************************************************************************************************
static void UpdateWidget( WM_HWIN hwnd ) {

    uint8_t id, rel_id;

    //вывод значений параметров
    for ( id = 0; id < DevParamCnt( ID_DEV_INV1, CNT_HMI ); id++ ) {
        rel_id = DevParamRelat( ID_DEV_INV1, id );
        TEXT_SetText( WM_GetDialogItem( hwnd, ID_VALUE_1_01 + id ), ParamGetForm( ID_DEV_INV1, rel_id, PARAM_VALUE ) );
        TEXT_SetText( WM_GetDialogItem( hwnd, ID_VALUE_3_01 + id ), ParamGetForm( ID_DEV_INV2, rel_id, PARAM_VALUE ) );
       }
    if ( ParamGetVal( ID_DEV_PORTS, BAT_CONNECT ).uint8 ) {
        //АКБ подключена, можно включать инвертора
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_1_ON ) );
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_3_ON ) );
       }
    else {
        //АКБ не подключена, включение инверторов заблокировано
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_1_ON ) );
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_3_ON ) );
       }
 }

//*************************************************************************************************
// Функция обратного вызова, обработка сообщений
//*************************************************************************************************
static void cb_Dialog( WM_MESSAGE * pMsg ) {

    WM_MESSAGE msg;
    uint8_t cmd, rel_id;
    int id, id_widg, ntf_code;

    switch ( pMsg->MsgId ) {
        case WM_INIT_DIALOG:
            //сообщим указатель (HWND) модального окна, окну из которого оно было вызвано
            msg.Data.v = DialogCreate[0].Id;
            msg.MsgId = WM_MESSAGE_MODAL_OPEN;              //тип сообщения
            msg.hWinSrc = WM_GetClientWindow( pMsg->hWin ); //указатель (HWND) модального окна
            WM_SendMessage( hwin0, &msg );
            WM_MakeModal( pMsg->hWin );
            WM_CreateTimer( WM_GetClientWindow( pMsg->hWin ), 0, TIME_UPD_PARAM, 0 );
            WM_SetWindowPos( pMsg->hWin, org_x + DialogCreate[0].x0, org_y + DialogCreate[0].y0, DialogCreate[0].xSize, DialogCreate[0].ySize );
            for ( id = 0; id < DevParamCnt( ID_DEV_INV1, CNT_HMI ); id++ ) {
                rel_id = DevParamRelat( ID_DEV_INV1, id );
                //вывод наименования параметра
                TEXT_SetText( WM_GetDialogItem( pMsg->hWin, ID_NAME_01 + id ), ParamGetForm( ID_DEV_INV1, rel_id, PARAM_DESC ) );
                //вывод единиц измерения параметра
                TEXT_SetText( WM_GetDialogItem( pMsg->hWin, ID_UNIT_01 + id ), ParamGetForm( ID_DEV_INV2, rel_id, PARAM_UNIT ) );
               }
            TEXT_SetWrapMode( WM_GetDialogItem( pMsg->hWin, ID_VALUE_1_09 ), GUI_WRAPMODE_WORD );
            TEXT_SetWrapMode( WM_GetDialogItem( pMsg->hWin, ID_VALUE_3_09 ), GUI_WRAPMODE_WORD );
            break;
        case WM_TIMER:
            WM_RestartTimer( pMsg->Data.v, TIME_UPD_PARAM );
            UpdateWidget( pMsg->hWin );
            break;
        case WM_NOTIFY_PARENT:
            ntf_code = pMsg->Data.v;
            id_widg = WM_GetId( pMsg->hWinSrc );
            if ( id_widg == ID_BUTTON_CLS && ntf_code == WM_NOTIFICATION_CLICKED ) {
                //пользователь закрыл модальное окно
                msg.Data.v = DialogCreate[0].Id;
                msg.MsgId = WM_MESSAGE_MODAL_END;
                msg.hWinSrc = WM_GetClientWindow( pMsg->hWin );
                //сообщение окну из которого было вызвано модальное окно
                WM_SendMessage( hwin0, &msg );
                GUI_EndDialog( pMsg->hWin, 0 );
               }
            //включить инвертор TS-1000-224
            if ( id_widg == ID_BUTTON_1_ON && ntf_code == WM_NOTIFICATION_CLICKED ) {
                cmd = INV_CTRL_ON;
                SendCommand( ID_DEV_INV1, &cmd, sizeof( cmd ) );
               }
            //выключить инвертор TS-1000-224
            if ( id_widg == ID_BUTTON_1_OFF && ntf_code == WM_NOTIFICATION_CLICKED ) {
                cmd = INV_CTRL_OFF;
                SendCommand( ID_DEV_INV1, &cmd, sizeof( cmd ) );
               }
            //включить инвертор TS-3000-224
            if ( id_widg == ID_BUTTON_3_ON && ntf_code == WM_NOTIFICATION_CLICKED ) {
                cmd = INV_CTRL_ON;
                SendCommand( ID_DEV_INV2, &cmd, sizeof( cmd ) );
               }
            //выключить инвертор TS-3000-224
            if ( id_widg == ID_BUTTON_3_OFF && ntf_code == WM_NOTIFICATION_CLICKED ) {
                cmd = INV_CTRL_OFF;
                SendCommand( ID_DEV_INV2, &cmd, sizeof( cmd ) );
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
// Основная функция вызова формы
//*************************************************************************************************
void WinInv( void ) {

    GUI_GetOrg( &org_x, &org_y ); //координаты текущего виртуального окна
    GUI_ExecDialogBox( DialogCreate, GUI_COUNTOF( DialogCreate ), &cb_Dialog, 0, 0, 0 );
 } 
