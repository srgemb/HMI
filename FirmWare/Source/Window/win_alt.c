
//*************************************************************************************************
//
// Форма управления блоком АВР
//
//*************************************************************************************************

#include "window.h"
#include "dialog.h"
#include "messagebox.h"

//*************************************************************************************************
// Внешние переменные
//*************************************************************************************************
extern WM_HWIN hwin0;

//*************************************************************************************************
// Размер окна
//*************************************************************************************************
#define WIN_SIZE_X      435
#define WIN_SIZE_Y      160

//*************************************************************************************************
// Определение идентификаторов объектов в форме
//*************************************************************************************************
#define ID_FRAMEWIN_0   (GUI_ID_USER + 10)

#define ID_BUTTON_CLS   (GUI_ID_USER + 0x10 + 1)
#define ID_BUTTON_DC    (GUI_ID_USER + 0x10 + 2)
#define ID_BUTTON_AC    (GUI_ID_USER + 0x10 + 3)

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
    { FRAMEWIN_CreateIndirect, "Блок АВР", ID_FRAMEWIN_0, WIN_POS_X( WIN_SIZE_X ), WIN_POS_Y( WIN_SIZE_Y ), WIN_SIZE_X, WIN_SIZE_Y },

    { BUTTON_CreateIndirect, "Закрыть",        ID_BUTTON_CLS, BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "Нагрузка от DC", ID_BUTTON_DC,  BUTTON_POS_X( 2, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "Нагрузка от AC", ID_BUTTON_AC,  BUTTON_POS_X( 3, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },

    { TEXT_CreateIndirect, "", ID_NAME_00, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 1 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_01, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 2 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_02, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 3 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_03, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 4 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_04, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 5 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },

    { TEXT_CreateIndirect, "", ID_VALUE_00, TEXT_VALUE_POS_X-50, TEXT_NAME_POS_Y( 1 ), TEXT_VALUE_SIZE_X+20, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_01, TEXT_VALUE_POS_X-50, TEXT_NAME_POS_Y( 2 ), TEXT_VALUE_SIZE_X+20, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_02, TEXT_VALUE_POS_X-50, TEXT_NAME_POS_Y( 3 ), TEXT_VALUE_SIZE_X+20, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_03, TEXT_VALUE_POS_X-50, TEXT_NAME_POS_Y( 4 ), TEXT_VALUE_SIZE_X+20, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_04, TEXT_VALUE_POS_X-50, TEXT_NAME_POS_Y( 5 ), TEXT_VALUE_SIZE_X+20, TEXT_VALUE_SIZE_Y },

    { TEXT_CreateIndirect, "", ID_UNIT_00, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 1 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_01, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 2 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_02, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 3 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_03, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 4 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_04, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 5 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
 };

//*************************************************************************************************
// Функция обновления данных в форме
//*************************************************************************************************
static void UpdateWidget( WM_HWIN hwnd ) {

    uint8_t id, rel_id;
    
    //вывод значений параметров
    for ( id = 0; id < DevParamCnt( ID_DEV_ALT, CNT_HMI ); id++ ) {
        rel_id = DevParamRelat( ID_DEV_ALT, id );
        TEXT_SetText( WM_GetDialogItem( hwnd, ID_VALUE_00 + id ), ParamGetForm( ID_DEV_ALT, rel_id, PARAM_VALUE ) );
       }
    //есть напряжение (внешняя сеть или генератор), можно включить питание нагрузки от сети
    if ( ParamGetVal( ID_DEV_ALT, ALT_MAIN_AC ).uint8 || ParamGetVal( ID_DEV_ALT, ALT_GEN_ON ).uint8 )
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_AC ) );
    else WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_AC ) );
    //переключать нагрузку на инвертора можно только при включенном АКБ и инверторах
    if ( ParamGetVal( ID_DEV_INV1, INV_DC_CONNECT ).uint8 || ParamGetVal( ID_DEV_INV2, INV_DC_CONNECT ).uint8 )
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_DC ) );
    else WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_DC ) );
 }

//*************************************************************************************************
// Функция обратного вызова, обработка сообщений
//*************************************************************************************************
static void cb_Dialog( WM_MESSAGE *pMsg ) {

    CAN_INFO info;
    WM_MESSAGE msg;
    int id_widg, ntf_code;
    uint8_t cmd, id, rt, rel_id;

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
            for ( id = 0; id < DevParamCnt( ID_DEV_ALT, CNT_HMI ); id++ ) {
                rel_id = DevParamRelat( ID_DEV_ALT, id );
                //наименование параметра
                TEXT_SetText( WM_GetDialogItem( pMsg->hWin, ID_NAME_00 + id ), ParamGetForm( ID_DEV_ALT, rel_id, PARAM_DESC ) );
                //единицы измерения параметра
                TEXT_SetText( WM_GetDialogItem( pMsg->hWin, ID_UNIT_00 + id ), ParamGetForm( ID_DEV_ALT, rel_id, PARAM_UNIT ) );
               }
            UpdateWidget( pMsg->hWin );
            break;
        case WM_TIMER:
            WM_RestartTimer( pMsg->Data.v, TIME_UPD_PARAM );
            UpdateWidget( pMsg->hWin );
            break;
        case WM_NOTIFY_PARENT:
            id_widg = WM_GetId( pMsg->hWinSrc );
            ntf_code = pMsg->Data.v;
            if ( id_widg == ID_BUTTON_CLS && ntf_code == WM_NOTIFICATION_CLICKED ) {
                //пользователь закрыл модальное окно
                msg.Data.v = DialogCreate[0].Id;
                msg.MsgId = WM_MESSAGE_MODAL_END;
                msg.hWinSrc = WM_GetClientWindow( pMsg->hWin );
                //сообщение окну из которого было вызвано модальное окно
                WM_SendMessage( hwin0, &msg );
                GUI_EndDialog( pMsg->hWin, 0 ); //закрываем форму
               }
            if ( id_widg == ID_BUTTON_AC && ntf_code == WM_NOTIFICATION_CLICKED ) {
                rt = MessageBox( MESSAGE_QUESTION, "Переключить нагрузку на основную сеть ?" );
                if ( rt == MB_YES ) {
                    cmd = CAN_ALT_AC;
                    SendCommand( ID_DEV_ALT, &cmd, sizeof( cmd ) );
                   }
               }
            if ( id_widg == ID_BUTTON_DC && ntf_code == WM_NOTIFICATION_CLICKED ) {
                if ( !ParamGetVal( ID_DEV_PORTS, BAT_CONNECT ).uint8 ) {
                    MessageBox( MESSAGE_WARNING, "Не включен автомат защиты АКБ." );
                    info.ctrl = CAN_VOICE;
                    info.voice = VOICE_BAT_NOCONN;
                    SendCommand( ID_DEV_VOICE, (uint8_t *)&info, sizeof( info ) );
                   }
                else {
                    rt = MessageBox( MESSAGE_QUESTION, "Переключить нагрузку АКБ ?" );
                    if ( rt == MB_YES ) {
                        cmd = CAN_ALT_DC;
                        SendCommand( ID_DEV_ALT, &cmd, sizeof( cmd ) );
                       }
                   }
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
void WinAlt( void ) {

    GUI_GetOrg( &org_x, &org_y ); //координаты текущего виртуального окна
    GUI_ExecDialogBox( DialogCreate, GUI_COUNTOF( DialogCreate ), &cb_Dialog, 0, 0, 0 );
 } 
