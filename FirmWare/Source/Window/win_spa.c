
//*************************************************************************************************
//
// Форма параметров положения солнца
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
#define WIN_SIZE_X      330
#define WIN_SIZE_Y      160

//*************************************************************************************************
// Определение идентификаторов объектов в форме
//*************************************************************************************************
#define ID_FRAMEWIN_0   (GUI_ID_USER + 7)

#define ID_BUTTON_CLS   (GUI_ID_USER + 0x10 + 1)

#define ID_NAME_00      (GUI_ID_USER + 0x20 + 0)
#define ID_NAME_01      (GUI_ID_USER + 0x20 + 1)
#define ID_NAME_02      (GUI_ID_USER + 0x20 + 2)
#define ID_NAME_03      (GUI_ID_USER + 0x20 + 3)
#define ID_NAME_04      (GUI_ID_USER + 0x20 + 4)

#define ID_VALUE_00     (GUI_ID_USER + 0x40 + 0)
#define ID_VALUE_01     (GUI_ID_USER + 0x40 + 1)
#define ID_VALUE_02     (GUI_ID_USER + 0x40 + 2)
#define ID_VALUE_03     (GUI_ID_USER + 0x40 + 3)
#define ID_VALUE_04     (GUI_ID_USER + 0x40 + 4)

#define ID_UNIT_00      (GUI_ID_USER + 0x80 + 0)
#define ID_UNIT_01      (GUI_ID_USER + 0x80 + 1)
#define ID_UNIT_02      (GUI_ID_USER + 0x80 + 2)
#define ID_UNIT_03      (GUI_ID_USER + 0x80 + 3)
#define ID_UNIT_04      (GUI_ID_USER + 0x80 + 4)

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
static int org_x, org_y;

static const GUI_WIDGET_CREATE_INFO DialogCreate[] = {
    { FRAMEWIN_CreateIndirect, "Параметры положения солнца", ID_FRAMEWIN_0, WIN_POS_X( WIN_SIZE_X ), WIN_POS_Y( WIN_SIZE_Y ), WIN_SIZE_X, WIN_SIZE_Y },
    { BUTTON_CreateIndirect, "Закрыть",  ID_BUTTON_CLS, BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },

    { TEXT_CreateIndirect, "", ID_NAME_00, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 1 ), SPA_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_01, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 2 ), SPA_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_02, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 3 ), SPA_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_03, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 4 ), SPA_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_04, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 5 ), SPA_NAME_SIZE_X, TEXT_NAME_SIZE_Y },

    { TEXT_CreateIndirect, "", ID_VALUE_00, SPA_VALUE_POS_X, TEXT_NAME_POS_Y( 1 ), SPA_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_01, SPA_VALUE_POS_X, TEXT_NAME_POS_Y( 2 ), SPA_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_02, SPA_VALUE_POS_X, TEXT_NAME_POS_Y( 3 ), SPA_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_03, SPA_VALUE_POS_X, TEXT_NAME_POS_Y( 4 ), SPA_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_04, SPA_VALUE_POS_X, TEXT_NAME_POS_Y( 5 ), SPA_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },

    { TEXT_CreateIndirect, "", ID_UNIT_00, SPA_UNIT_POS_X, TEXT_NAME_POS_Y( 1 ), SPA_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_01, SPA_UNIT_POS_X, TEXT_NAME_POS_Y( 2 ), SPA_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_02, SPA_UNIT_POS_X, TEXT_NAME_POS_Y( 3 ), SPA_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_03, SPA_UNIT_POS_X, TEXT_NAME_POS_Y( 4 ), SPA_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_04, SPA_UNIT_POS_X, TEXT_NAME_POS_Y( 5 ), SPA_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
 };

//*************************************************************************************************
// Функция обновления данных в форме
//*************************************************************************************************
static void UpdateWidget( WM_HWIN hwnd ) {

    uint8_t id, rel_id;
    
    //вывод значений параметров
    for ( id = 0; id < DevParamCnt( ID_DEV_SPA, CNT_HMI ); id++ ) {
        rel_id = DevParamRelat( ID_DEV_SPA, id );
        TEXT_SetText( WM_GetDialogItem( hwnd, ID_VALUE_00 + id ), ParamGetForm( ID_DEV_SPA, rel_id, PARAM_VALUE ) );
       }
 }

//*************************************************************************************************
// Функция обратного вызова, обработка сообщений
//*************************************************************************************************
static void cb_Dialog( WM_MESSAGE * pMsg ) {

    uint8_t rel_id;
    WM_MESSAGE msg;
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
            for ( id = 0; id < DevParamCnt( ID_DEV_SPA, CNT_HMI ); id++ ) {
                rel_id = DevParamRelat( ID_DEV_SPA, id );
                //вывод наименования параметра
                TEXT_SetText( WM_GetDialogItem( pMsg->hWin, ID_NAME_00 + id ), ParamGetForm( ID_DEV_SPA, rel_id, PARAM_DESC ) );
                //вывод единиц измерения параметра
                TEXT_SetText( WM_GetDialogItem( pMsg->hWin, ID_UNIT_00 + id ), ParamGetForm( ID_DEV_SPA, rel_id, PARAM_UNIT ) );
               }
            UpdateWidget( pMsg->hWin );
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
                GUI_EndDialog( pMsg->hWin, 0 ); //закрываем форму
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
void WinSpa( void ) {

    GUI_GetOrg( &org_x, &org_y ); //координаты текущего виртуального окна
    GUI_ExecDialogBox( DialogCreate, GUI_COUNTOF( DialogCreate ), &cb_Dialog, 0, 0, 0 );
 } 
