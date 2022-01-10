
//*************************************************************************************************
//
// Форма управления голосовым и звуковым информатором
//
//*************************************************************************************************

#include "window.h"
#include "dialog.h"
#include "messagebox.h"

//*************************************************************************************************
// Внешние переменные
//*************************************************************************************************
extern WM_HWIN hwin0;
extern GUI_FONT GUI_FontTahoma16;

//*************************************************************************************************
// Размер окна
//*************************************************************************************************
#define WIN_SIZE_X          310
#define WIN_SIZE_Y          250

//*************************************************************************************************
// Определение идентификаторов объектов в форме
//*************************************************************************************************
#define ID_FRAMEWIN_0       (GUI_ID_USER + 15)

#define ID_BUTTON_CLS       (GUI_ID_USER + 0x10 + 1)
#define ID_BUTTON_SND       (GUI_ID_USER + 0x10 + 2)
#define ID_BUTTON_VOC       (GUI_ID_USER + 0x10 + 3)
#define ID_BUTTON_VOL       (GUI_ID_USER + 0x10 + 4)

#define ID_NAME_00          (GUI_ID_USER + 0x20 + 0)
#define ID_NAME_01          (GUI_ID_USER + 0x20 + 1)
#define ID_NAME_02          (GUI_ID_USER + 0x20 + 2)
#define ID_NAME_03          (GUI_ID_USER + 0x20 + 3)
#define ID_NAME_04          (GUI_ID_USER + 0x20 + 4)

#define ID_VALUE_00         (GUI_ID_USER + 0x40 + 0)
#define ID_VALUE_01         (GUI_ID_USER + 0x40 + 1)
#define ID_VALUE_02         (GUI_ID_USER + 0x40 + 2)
#define ID_VALUE_03         (GUI_ID_USER + 0x40 + 3)

#define ID_DROPDOWN_0       (GUI_ID_USER + 0x80 + 0)
#define ID_DROPDOWN_1       (GUI_ID_USER + 0x80 + 1)
#define ID_SPINBOX_VOL      (GUI_ID_USER + 0x80 + 2)

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
static int org_x, org_y;

static const GUI_WIDGET_CREATE_INFO DialogCreate[] = {
    { FRAMEWIN_CreateIndirect, "Звуковое/голосовое информирование", ID_FRAMEWIN_0, WIN_POS_X( WIN_SIZE_X ), WIN_POS_Y( WIN_SIZE_Y ), WIN_SIZE_X, WIN_SIZE_Y },

    { BUTTON_CreateIndirect, "Закрыть",       ID_BUTTON_CLS, BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ),     BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "Воспроизвести", ID_BUTTON_SND, BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y-35 ),  BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "Воспроизвести", ID_BUTTON_VOC, BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y-70 ),  BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "Установить",    ID_BUTTON_VOL, BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y-105 ), BUTTON_SIZE_X, BUTTON_SIZE_Y },

    { TEXT_CreateIndirect, "Громкость", ID_NAME_04, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 6 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },

    { TEXT_CreateIndirect, "", ID_NAME_00, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 1 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_01, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 2 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_02, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 3 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_NAME_03, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 4 ), TEXT_NAME_SIZE_X, TEXT_NAME_SIZE_Y },

    { TEXT_CreateIndirect, "", ID_VALUE_00, TEXT_VALUE_POS_X-50, TEXT_NAME_POS_Y( 1 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_01, TEXT_VALUE_POS_X-50, TEXT_NAME_POS_Y( 2 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_02, TEXT_VALUE_POS_X-50, TEXT_NAME_POS_Y( 3 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },
    { TEXT_CreateIndirect, "", ID_VALUE_03, TEXT_VALUE_POS_X-50, TEXT_NAME_POS_Y( 4 ), TEXT_VALUE_SIZE_X, TEXT_VALUE_SIZE_Y },

    { DROPDOWN_CreateIndirect, "", ID_DROPDOWN_0, 10, WIN_SIZE_Y-105-23, 150, 20, 0, 0, 0 },
    { DROPDOWN_CreateIndirect, "", ID_DROPDOWN_1, 10, WIN_SIZE_Y-70-23, 150, 20, 0, 0, 0 },
    { SPINBOX_CreateIndirect,  "", ID_SPINBOX_VOL, 100, 86, 60, 20, 0, 0, 0 }

 };

//*************************************************************************************************
// Функция обновления данных в форме
//*************************************************************************************************
static void UpdateWidget( WM_HWIN hwnd ) {

    uint8_t id, rel_id;
    
    //вывод значений параметров
    for ( id = 0; id < DevParamCnt( ID_DEV_VOICE, CNT_HMI ); id++ ) {
        rel_id = DevParamRelat( ID_DEV_VOICE, id );
        TEXT_SetText( WM_GetDialogItem( hwnd, ID_VALUE_00 + id ), ParamGetForm( ID_DEV_VOICE, rel_id, PARAM_VALUE ) );
       }
 }

//*************************************************************************************************
// Функция обратного вызова, обработка сообщений
//*************************************************************************************************
static void cb_Dialog( WM_MESSAGE * pMsg ) {

    CAN_INFO info;
    WM_HWIN hItem;
    WM_MESSAGE msg;
    int id_widg, ntf_code;
    uint8_t id, snd, voc, vol, rel_id;

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
            //наименование параметра
            for ( id = 0; id < DevParamCnt( ID_DEV_VOICE, CNT_HMI ); id++ ) {
                rel_id = DevParamRelat( ID_DEV_VOICE, id );
                TEXT_SetText( WM_GetDialogItem( pMsg->hWin, ID_NAME_00 + id ), ParamGetForm( ID_DEV_VOICE, rel_id, PARAM_DESC ) );
               }
            //
            SPINBOX_SetFont( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_VOL ), &GUI_FontTahoma16 );
            SPINBOX_SetEdge( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_VOL ), SPINBOX_EDGE_CENTER );
            SPINBOX_SetRange( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_VOL ), 0, 9 );
            SPINBOX_SetValue( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_VOL ), ParamGetVal( ID_DEV_VOICE, VOICE_PAR_VOL ).uint8 );
            //список голосовых сообщений
            hItem = WM_GetDialogItem( pMsg->hWin, ID_DROPDOWN_0 );
            DROPDOWN_SetFont( hItem, &GUI_FontTahoma16 );
            DROPDOWN_SetAutoScroll( hItem, 1 );
            DROPDOWN_SetListHeight( hItem, 120 );
            DROPDOWN_SetUpMode( hItem, 1 );
            DROPDOWN_SetScrollbarWidth( hItem, 20 );
            DROPDOWN_AddString( hItem, "INFO" );
            DROPDOWN_AddString( hItem, "LOW_CHARGE" );
            DROPDOWN_AddString( hItem, "LEVEL_CHARGE" );
            DROPDOWN_AddString( hItem, "POWER_INV" );
            DROPDOWN_AddString( hItem, "TTGO" );
            DROPDOWN_AddString( hItem, "CHARGE" );
            DROPDOWN_AddString( hItem, "BAT_FULL" );
            DROPDOWN_AddString( hItem, "POWER_AC" );
            DROPDOWN_AddString( hItem, "POWER_TS" );
            DROPDOWN_AddString( hItem, "POWER_GEN" );
            DROPDOWN_AddString( hItem, "AC_OFF" );
            DROPDOWN_AddString( hItem, "AC_ON" );
            DROPDOWN_AddString( hItem, "POWER_CTRL" );
            DROPDOWN_AddString( hItem, "BAT_NOCONN" );
            DROPDOWN_AddString( hItem, "CHARGE_END" );
            DROPDOWN_AddString( hItem, "GEN_RUN" );
            DROPDOWN_AddString( hItem, "GEN_OFF" );
            DROPDOWN_AddString( hItem, "GEN_DISCON" );
            DROPDOWN_AddString( hItem, "GEN_CHECK" );
            DROPDOWN_AddString( hItem, "PV_ON" );
            DROPDOWN_AddString( hItem, "PV_OFF" );
            DROPDOWN_AddString( hItem, "TRC_ON" );
            DROPDOWN_AddString( hItem, "TRC_OFF" );
            DROPDOWN_AddString( hItem, "TEST" );
            DROPDOWN_AddString( hItem, "TS_START" );
            DROPDOWN_AddString( hItem, "VOICE" );
            //список звуковых сообщений
            hItem = WM_GetDialogItem( pMsg->hWin, ID_DROPDOWN_1 );
            DROPDOWN_SetFont( hItem, &GUI_FontTahoma16 );
            DROPDOWN_SetAutoScroll( hItem, 1 );
            DROPDOWN_SetListHeight( hItem, 150 );
            DROPDOWN_SetUpMode( hItem, 1 );
            DROPDOWN_SetScrollbarWidth( hItem, 20 );
            DROPDOWN_AddString( hItem, "BATCRT" );
            DROPDOWN_AddString( hItem, "BATLOW" );
            DROPDOWN_AddString( hItem, "CRTSTP" );
            DROPDOWN_AddString( hItem, "ERROR" );
            DROPDOWN_AddString( hItem, "EXCL" );
            DROPDOWN_AddString( hItem, "HDFAIL" );
            DROPDOWN_AddString( hItem, "HDINST" );
            DROPDOWN_AddString( hItem, "HDREM" );
            DROPDOWN_AddString( hItem, "LOGOFF" );
            DROPDOWN_AddString( hItem, "LOGON" );
            DROPDOWN_AddString( hItem, "NOTIFY" );
            DROPDOWN_AddString( hItem, "PRINT" );
            DROPDOWN_AddString( hItem, "SHUTDN" );
            DROPDOWN_AddString( hItem, "STARTU" );
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
            if ( id_widg == ID_BUTTON_VOC && ntf_code == WM_NOTIFICATION_CLICKED ) {
                //воспроизведение голосового сообщения
                voc = DROPDOWN_GetSel( WM_GetDialogItem( pMsg->hWin, ID_DROPDOWN_0 ) );
                info.ctrl = CAN_VOICE;
                info.voice = (VoiceId)( voc + 1 );
                SendCommand( ID_DEV_VOICE, (uint8_t *)&info, sizeof( info ) );
               }
            if ( id_widg == ID_BUTTON_SND && ntf_code == WM_NOTIFICATION_CLICKED ) {
                //воспроизведение звукового сообщения
                snd = DROPDOWN_GetSel( WM_GetDialogItem( pMsg->hWin, ID_DROPDOWN_1 ) );
                info.ctrl = CAN_SOUND;
                info.sound = (SoundId)( snd + 1 );
                SendCommand( ID_DEV_VOICE, (uint8_t *)&info, sizeof( info ) );
               }
            if ( id_widg == ID_BUTTON_VOL && ntf_code == WM_NOTIFICATION_CLICKED ) {
                //установка громкости голосового информатора
                vol = SPINBOX_GetValue( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_VOL ) );
                info.ctrl = CAN_VOLUME;
                info.volume = (Volume)vol;
                SendCommand( ID_DEV_VOICE, (uint8_t *)&info, sizeof( info ) );
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
void WinSnd( void ) {

    GUI_GetOrg( &org_x, &org_y ); //координаты текущего виртуального окна
    GUI_ExecDialogBox( DialogCreate, GUI_COUNTOF( DialogCreate ), &cb_Dialog, 0, 0, 0 );
 } 
