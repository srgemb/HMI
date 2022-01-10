
//*************************************************************************************************
//
// Форма контроля состояния контроллера MPPT
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
#define WIN_SIZE_X      435
#define WIN_SIZE_Y      355

//*************************************************************************************************
// Определение идентификаторов объектов в форме
//*************************************************************************************************
#define ID_FRAMEWIN_0   (GUI_ID_USER + 9)

#define ID_BUTTON_CLS   (GUI_ID_USER + 0x10 + 1)
#define ID_BUTTON_MODE  (GUI_ID_USER + 0x10 + 2)
#define ID_BUTTON_PV    (GUI_ID_USER + 0x10 + 3)

#define ID_NAME_00      (GUI_ID_USER + 0x20 + 0)
#define ID_NAME_01      (GUI_ID_USER + 0x20 + 1)
#define ID_NAME_02      (GUI_ID_USER + 0x20 + 2)
#define ID_NAME_03      (GUI_ID_USER + 0x20 + 3)
#define ID_NAME_04      (GUI_ID_USER + 0x20 + 4)
#define ID_NAME_05      (GUI_ID_USER + 0x20 + 5)
#define ID_NAME_06      (GUI_ID_USER + 0x20 + 6)
#define ID_NAME_07      (GUI_ID_USER + 0x20 + 7)
#define ID_NAME_08      (GUI_ID_USER + 0x20 + 8)
#define ID_NAME_09      (GUI_ID_USER + 0x20 + 9)
#define ID_NAME_10      (GUI_ID_USER + 0x20 + 10)
#define ID_NAME_11      (GUI_ID_USER + 0x20 + 11)
#define ID_NAME_12      (GUI_ID_USER + 0x20 + 12)
#define ID_NAME_13      (GUI_ID_USER + 0x20 + 13)
#define ID_NAME_14      (GUI_ID_USER + 0x20 + 14)
#define ID_NAME_15      (GUI_ID_USER + 0x20 + 15)
#define ID_NAME_16      (GUI_ID_USER + 0x20 + 16)
#define ID_NAME_17      (GUI_ID_USER + 0x20 + 17)

#define ID_VALUE_00     (GUI_ID_USER + 0x40 + 0)
#define ID_VALUE_01     (GUI_ID_USER + 0x40 + 1)
#define ID_VALUE_02     (GUI_ID_USER + 0x40 + 2)
#define ID_VALUE_03     (GUI_ID_USER + 0x40 + 3)
#define ID_VALUE_04     (GUI_ID_USER + 0x40 + 4)
#define ID_VALUE_05     (GUI_ID_USER + 0x40 + 5)
#define ID_VALUE_06     (GUI_ID_USER + 0x40 + 6)
#define ID_VALUE_07     (GUI_ID_USER + 0x40 + 7)
#define ID_VALUE_08     (GUI_ID_USER + 0x40 + 8)
#define ID_VALUE_09     (GUI_ID_USER + 0x40 + 9)
#define ID_VALUE_10     (GUI_ID_USER + 0x40 + 10)
#define ID_VALUE_11     (GUI_ID_USER + 0x40 + 11)
#define ID_VALUE_12     (GUI_ID_USER + 0x40 + 12)
#define ID_VALUE_13     (GUI_ID_USER + 0x40 + 13)
#define ID_VALUE_14     (GUI_ID_USER + 0x40 + 14)
#define ID_VALUE_15     (GUI_ID_USER + 0x40 + 15)
#define ID_VALUE_16     (GUI_ID_USER + 0x40 + 16)
#define ID_VALUE_17     (GUI_ID_USER + 0x40 + 17)

#define ID_UNIT_00      (GUI_ID_USER + 0x80 + 0)
#define ID_UNIT_01      (GUI_ID_USER + 0x80 + 1)
#define ID_UNIT_02      (GUI_ID_USER + 0x80 + 2)
#define ID_UNIT_03      (GUI_ID_USER + 0x80 + 3)
#define ID_UNIT_04      (GUI_ID_USER + 0x80 + 4)
#define ID_UNIT_05      (GUI_ID_USER + 0x80 + 5)
#define ID_UNIT_06      (GUI_ID_USER + 0x80 + 6)
#define ID_UNIT_07      (GUI_ID_USER + 0x80 + 7)
#define ID_UNIT_08      (GUI_ID_USER + 0x80 + 8)
#define ID_UNIT_09      (GUI_ID_USER + 0x80 + 9)
#define ID_UNIT_10      (GUI_ID_USER + 0x80 + 10)
#define ID_UNIT_11      (GUI_ID_USER + 0x80 + 11)
#define ID_UNIT_12      (GUI_ID_USER + 0x80 + 12)
#define ID_UNIT_13      (GUI_ID_USER + 0x80 + 13)
#define ID_UNIT_14      (GUI_ID_USER + 0x80 + 14)
#define ID_UNIT_15      (GUI_ID_USER + 0x80 + 15)
#define ID_UNIT_16      (GUI_ID_USER + 0x80 + 16)
#define ID_UNIT_17      (GUI_ID_USER + 0x80 + 17)

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
static int org_x, org_y;

static const GUI_WIDGET_CREATE_INFO DialogCreate[] = {
    { FRAMEWIN_CreateIndirect, "Солнечный контроллер заряда", ID_FRAMEWIN_0, WIN_POS_X( WIN_SIZE_X ), WIN_POS_Y( WIN_SIZE_Y ), WIN_SIZE_X, WIN_SIZE_Y },
    { BUTTON_CreateIndirect, "Закрыть",  ID_BUTTON_CLS, BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "ПОСЛ режим",  ID_BUTTON_MODE, BUTTON_POS_X( 2, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "ВКЛ панели",  ID_BUTTON_PV, BUTTON_POS_X( 3, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },

    { TEXT_CreateIndirect, "", ID_NAME_00, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 1 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_01, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 2 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_02, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 3 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_03, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 4 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_04, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 5 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_05, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 6 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_06, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 7 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_07, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 8 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_08, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 9 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_09, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 10 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_10, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 11 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_11, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 12 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_12, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 13 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_13, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 14 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_14, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 15 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_15, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 16 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_16, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 17 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_17, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 18 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },

    { TEXT_CreateIndirect, "", ID_VALUE_00, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 1 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_01, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 2 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_02, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 3 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_03, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 4 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_04, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 5 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_05, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 6 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_06, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 7 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_07, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 8 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_08, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 9 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_09, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 10 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_10, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 11 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_11, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 12 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_12, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 13 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_13, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 14 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_14, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 15 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_15, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 16 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_16, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 17 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_17, TEXT_VALUE_POS_X, TEXT_NAME_POS_Y( 18 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },

    { TEXT_CreateIndirect, "", ID_UNIT_00, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 1 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_01, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 2 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_02, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 3 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_03, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 4 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_04, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 5 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_05, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 6 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_06, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 7 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_07, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 8 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_08, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 9 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_09, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 10 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_10, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 11 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_11, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 12 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_12, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 13 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_13, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 14 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_14, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 15 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_15, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 16 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_16, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 17 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_UNIT_17, TEXT_UNIT_POS_X, TEXT_NAME_POS_Y( 18 ), TEXT_UNIT_SIZE_X, TEXT_UNIT_SIZE_Y }
 };

//*************************************************************************************************
// Функция обновления данных в форме
//*************************************************************************************************
static void UpdateWidget( WM_HWIN hwnd ) {

    uint8_t id, rel_id;

    //вывод значений параметров
    for ( id = 0; id < DevParamCnt( ID_DEV_MPPT, CNT_HMI ); id++ ) {
        rel_id = DevParamRelat( ID_DEV_MPPT, id );
        TEXT_SetText( WM_GetDialogItem( hwnd, ID_VALUE_00 + id ), ParamGetForm( ID_DEV_MPPT, rel_id, PARAM_VALUE ) );
       }
    if ( ParamGetVal( ID_DEV_MPPT, MPPT_POWER ).uint8 ) {
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_PV ) );
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_MODE ) );
       }
    else {
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_PV ) );
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_MODE ) );
       }
    //управление подключением панелей
    if ( ParamGetVal( ID_DEV_MPPT, MPPT_PVON ).uint8 == PV_STAT_ON )
        BUTTON_SetText( WM_GetDialogItem( hwnd, ID_BUTTON_PV ), "ВЫКЛ панели" );
    else BUTTON_SetText( WM_GetDialogItem( hwnd, ID_BUTTON_PV ), "ВКЛ панели" );
    //управление режимом панелей
    if ( ParamGetVal( ID_DEV_MPPT, MPPT_PVMODE ).uint8 == PV_MODE_PAR )
        BUTTON_SetText( WM_GetDialogItem( hwnd, ID_BUTTON_MODE ), "ПОСЛ. режим" );
    else BUTTON_SetText( WM_GetDialogItem( hwnd, ID_BUTTON_MODE ), "ПАРЛ. режим" );
 }

//*************************************************************************************************
// Функция обратного вызова, обработка сообщений
//*************************************************************************************************
static void cb_Dialog( WM_MESSAGE * pMsg ) {

    uint8_t rel_id;
    CAN_PV can_pv;
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
            for ( id = 0; id < DevParamCnt( ID_DEV_MPPT, CNT_HMI ); id++ ) {
                rel_id = DevParamRelat( ID_DEV_MPPT, id );
                //вывод наименования параметра
                TEXT_SetText( WM_GetDialogItem( pMsg->hWin, ID_NAME_00 + id ), ParamGetForm( ID_DEV_MPPT, rel_id, PARAM_DESC ) );
                //вывод единиц измерения параметра
                TEXT_SetText( WM_GetDialogItem( pMsg->hWin, ID_UNIT_00 + id ), ParamGetForm( ID_DEV_MPPT, rel_id, PARAM_UNIT ) );
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
            if ( id_widg == ID_BUTTON_PV && ntf_code == WM_NOTIFICATION_CLICKED ) {
                //вкл/выкл панелей
                if ( ParamGetVal( ID_DEV_MPPT, MPPT_PVON ).uint8 ) {
                    can_pv.mask = CAN_PV_CTRL;
                    can_pv.ctrl = PV_CTRL_OFF;
                   }
                else {
                    can_pv.mask = CAN_PV_CTRL;
                    can_pv.ctrl = PV_CTRL_ON;
                   }
                SendCommand( ID_DEV_PV, (uint8_t *)&can_pv, sizeof( can_pv ) );
               }
            if ( id_widg == ID_BUTTON_MODE && ntf_code == WM_NOTIFICATION_CLICKED ) {
                //переключение режима панелей
                if ( ParamGetVal( ID_DEV_MPPT, MPPT_PVMODE ).uint8 ) {
                    can_pv.mask = CAN_PV_MODE;
                    can_pv.mode = PV_MODE_SER;
                   }
                else {
                    can_pv.mask = CAN_PV_MODE;
                    can_pv.mode = PV_MODE_PAR;
                   }
                SendCommand( ID_DEV_PV, (uint8_t *)&can_pv, sizeof( can_pv ) );
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
void WinMppt( void ) {

    GUI_GetOrg( &org_x, &org_y ); //координаты текущего виртуального окна
    GUI_ExecDialogBox( DialogCreate, GUI_COUNTOF( DialogCreate ), &cb_Dialog, 0, 0, 0 );
 } 
