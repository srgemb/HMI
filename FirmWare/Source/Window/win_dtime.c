
//*************************************************************************************************
//
// Форма установки даты/времени
//
//*************************************************************************************************

#include "dialog.h"
#include "window.h"

//*********************************************************************************************
// Внешние переменные
//*********************************************************************************************
extern WM_HWIN hwin0;
extern GUI_FONT GUI_FontTahoma16;

//*************************************************************************************************
// Размер окна
//*************************************************************************************************
#define WIN_SIZE_X          400
#define WIN_SIZE_Y          180

//*************************************************************************************************
// Определение идентификаторов объектов в форме
//*************************************************************************************************
#define ID_FRAMEWIN_0       (GUI_ID_USER + 14)

#define ID_BUTTON_CLS       (GUI_ID_USER + 0x10 + 1)
#define ID_BUTTON_SET1      (GUI_ID_USER + 0x10 + 2)
#define ID_BUTTON_SET2      (GUI_ID_USER + 0x10 + 3)
#define ID_BUTTON_HC        (GUI_ID_USER + 0x10 + 4)
#define ID_BUTTON_CH        (GUI_ID_USER + 0x10 + 5)

#define ID_TEXT_0           (GUI_ID_USER + 0x20 + 0)
#define ID_TEXT_1           (GUI_ID_USER + 0x20 + 1)
#define ID_TEXT_2           (GUI_ID_USER + 0x20 + 2)
#define ID_TEXT_3           (GUI_ID_USER + 0x20 + 3)
#define ID_TEXT_4           (GUI_ID_USER + 0x20 + 4)
#define ID_TEXT_5           (GUI_ID_USER + 0x20 + 5)

#define ID_SPINBOX_HOUR     (GUI_ID_USER + 0x40 + 0)
#define ID_SPINBOX_MIN      (GUI_ID_USER + 0x40 + 1)
#define ID_SPINBOX_SEC      (GUI_ID_USER + 0x40 + 2)
#define ID_SPINBOX_DAY      (GUI_ID_USER + 0x40 + 3)
#define ID_SPINBOX_MONTH    (GUI_ID_USER + 0x40 + 4)
#define ID_SPINBOX_YEAR     (GUI_ID_USER + 0x40 + 5)

//*************************************************************************************************
// Локальные константы
//*************************************************************************************************
#define TEXT_SIZE_X         60
#define TEXT_SIZE_Y         20

#define EDIT_SIZE_X         60
#define EDIT_SIZE_Y         20

#define STR1_POS_Y          10
#define STR2_POS_Y          50

#define STR_POS0_X          1
#define STR_POS1_X          STR_POS0_X + TEXT_SIZE_X
#define STR_POS2_X          STR_POS1_X + EDIT_SIZE_X
#define STR_POS3_X          STR_POS2_X + TEXT_SIZE_X
#define STR_POS4_X          STR_POS3_X + EDIT_SIZE_X
#define STR_POS5_X          STR_POS4_X + TEXT_SIZE_X

//*********************************************************************************************
// Локальные переменные
//*********************************************************************************************
static int org_x, org_y;

static const GUI_WIDGET_CREATE_INFO DialogCreate[] = {
    { FRAMEWIN_CreateIndirect, "Установка даты/времени", ID_FRAMEWIN_0, WIN_POS_X( WIN_SIZE_X ), WIN_POS_Y( WIN_SIZE_Y ), WIN_SIZE_X, WIN_SIZE_Y },

    { BUTTON_CreateIndirect, "Закрыть",    ID_BUTTON_CLS,  BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "Уст. дату",  ID_BUTTON_SET2, BUTTON_POS_X( 2, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "Уст. время", ID_BUTTON_SET1, BUTTON_POS_X( 3, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    
    { BUTTON_CreateIndirect, "Синх. CTRL->HMI", ID_BUTTON_CH, BUTTON_POS_X( 2, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y-35 ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "Синх. HMI->CTRL", ID_BUTTON_HC, BUTTON_POS_X( 3, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y-35 ), BUTTON_SIZE_X, BUTTON_SIZE_Y },

    { TEXT_CreateIndirect, "Часы",    ID_TEXT_0,        STR_POS0_X, STR1_POS_Y, TEXT_SIZE_X, TEXT_SIZE_Y, 0, 0, 0 },
    { SPINBOX_CreateIndirect, "",     ID_SPINBOX_HOUR,  STR_POS1_X, STR1_POS_Y, EDIT_SIZE_X, EDIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, "Минуты",  ID_TEXT_1,        STR_POS2_X, STR1_POS_Y, TEXT_SIZE_X, TEXT_SIZE_Y, 0, 0, 0 },
    { SPINBOX_CreateIndirect, "",     ID_SPINBOX_MIN,   STR_POS3_X, STR1_POS_Y, EDIT_SIZE_X, EDIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, "Секунды", ID_TEXT_2,        STR_POS4_X, STR1_POS_Y, TEXT_SIZE_X, TEXT_SIZE_Y, 0, 0, 0 },
    { SPINBOX_CreateIndirect, "",     ID_SPINBOX_SEC,   STR_POS5_X, STR1_POS_Y, EDIT_SIZE_X, EDIT_SIZE_Y, 0, 0, 0 },

    { TEXT_CreateIndirect, "Число",   ID_TEXT_3,        STR_POS0_X, STR2_POS_Y, TEXT_SIZE_X, TEXT_SIZE_Y, 0, 0, 0 },
    { SPINBOX_CreateIndirect, "",     ID_SPINBOX_DAY,   STR_POS1_X, STR2_POS_Y, EDIT_SIZE_X, EDIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, "Месяц",   ID_TEXT_4,        STR_POS2_X, STR2_POS_Y, TEXT_SIZE_X, TEXT_SIZE_Y, 0, 0, 0 },
    { SPINBOX_CreateIndirect, "",     ID_SPINBOX_MONTH, STR_POS3_X, STR2_POS_Y, EDIT_SIZE_X, EDIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, "Год",     ID_TEXT_5,        STR_POS4_X, STR2_POS_Y, TEXT_SIZE_X, TEXT_SIZE_Y, 0, 0, 0 },
    { SPINBOX_CreateIndirect, "",     ID_SPINBOX_YEAR,  STR_POS5_X, STR2_POS_Y, EDIT_SIZE_X+10, EDIT_SIZE_Y, 0, 0, 0 }
 };

//*************************************************************************************************
// Функция обновления данных в форме
//*************************************************************************************************
static void UpdateWidget( WM_HWIN hwnd ) {

    //блокировка/разблокировка кнопок синхронизации часов
    if ( LinkStat() != CAN_LINK_OK ) {
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_CH ) );
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_HC ) );
       }
    else {
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_CH ) );
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_HC ) );
       }
 }

//*************************************************************************************************
// Функция обратного вызова, обработка сообщений
//*************************************************************************************************
static void cb_Dialog( WM_MESSAGE * pMsg ) {

    RTC rtc_hmi;
    WM_MESSAGE msg;
    int hour, min, sec;
    int day, month, year;
    RTC_TIME_Type time;
    char str[12], new_val[16];
    int id_widg, ntf_code, max;

    switch ( pMsg->MsgId ) {
        case WM_INIT_DIALOG:
            //сообщим указатель (HWND) модального окна, окну из которого оно было вызвано
            msg.Data.v = DialogCreate[0].Id;
            msg.MsgId = WM_MESSAGE_MODAL_OPEN;              //тип сообщения
            msg.hWinSrc = WM_GetClientWindow( pMsg->hWin ); //указатель (HWND) модального окна
            WM_SendMessage( hwin0, &msg );
            WM_MakeModal( pMsg->hWin );
            RTC_GetFullTime( LPC_RTC, &time );
            WM_CreateTimer( WM_GetClientWindow( pMsg->hWin ), 0, TIME_UPD_PARAM, 0 );
            WM_SetWindowPos( pMsg->hWin, org_x + DialogCreate[0].x0, org_y + DialogCreate[0].y0, DialogCreate[0].xSize, DialogCreate[0].ySize );
            
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_TEXT_0 ), GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_TEXT_1 ), GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_TEXT_2 ), GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_TEXT_3 ), GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_TEXT_4 ), GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_TEXT_5 ), GUI_TA_HCENTER | GUI_TA_VCENTER);

            SPINBOX_SetFont( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_HOUR ), &GUI_FontTahoma16 );
            SPINBOX_SetFont( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_MIN ), &GUI_FontTahoma16 );
            SPINBOX_SetFont( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_SEC ), &GUI_FontTahoma16 );
            SPINBOX_SetFont( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_DAY ), &GUI_FontTahoma16 );
            SPINBOX_SetFont( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_MONTH ), &GUI_FontTahoma16 );
            SPINBOX_SetFont( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_YEAR ), &GUI_FontTahoma16 );

            SPINBOX_SetEdge( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_HOUR ), SPINBOX_EDGE_CENTER );
            SPINBOX_SetEdge( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_MIN ), SPINBOX_EDGE_CENTER );
            SPINBOX_SetEdge( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_SEC ), SPINBOX_EDGE_CENTER );
            SPINBOX_SetEdge( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_DAY ), SPINBOX_EDGE_CENTER );
            SPINBOX_SetEdge( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_MONTH ), SPINBOX_EDGE_CENTER );
            SPINBOX_SetEdge( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_YEAR ), SPINBOX_EDGE_CENTER );

            SPINBOX_SetRange( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_HOUR ), 0, 23 );
            SPINBOX_SetRange( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_MIN ), 0, 59 );
            SPINBOX_SetRange( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_SEC ), 0, 59 );

            SPINBOX_SetRange( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_DAY ), 1, 31 );
            SPINBOX_SetRange( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_MONTH ), 1, 12 );
            SPINBOX_SetRange( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_YEAR ), 2000, 2040 );

            SPINBOX_SetValue( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_HOUR ), time.HOUR );
            SPINBOX_SetValue( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_MIN ), time.MIN );
            SPINBOX_SetValue( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_SEC ), time.SEC );
            SPINBOX_SetValue( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_DAY ), time.DOM );
            SPINBOX_SetValue( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_MONTH ), time.MONTH );
            SPINBOX_SetValue( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_YEAR ), time.YEAR );
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
            //установка времени
            if ( id_widg == ID_BUTTON_SET1 && ntf_code == WM_NOTIFICATION_CLICKED ) {
                hour = SPINBOX_GetValue( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_HOUR ) );
                min = SPINBOX_GetValue( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_MIN ) );
                sec = SPINBOX_GetValue( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_SEC ) );
                sprintf( str, "%02d:%02d:%02d", hour, min, sec );
                if ( LinkStat() != CAN_LINK_OK )
                    RTCSetTime( str, SET_MODE_LOCAL );
                else RTCSetTime( str, SET_MODE_REMOTE );
               }
            //установка даты
            if ( id_widg == ID_BUTTON_SET2 && ntf_code == WM_NOTIFICATION_CLICKED ) {
                day = SPINBOX_GetValue( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_DAY ) );
                month = SPINBOX_GetValue( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_MONTH ) );
                year = SPINBOX_GetValue( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_YEAR ) );
                sprintf( str, "%02d.%02d.%04d", day, month, year );
                if ( LinkStat() != CAN_LINK_OK )
                    RTCSetDate( str, SET_MODE_LOCAL );
                else RTCSetDate( str, SET_MODE_REMOTE );
               }
            //синхронизировать часы "HMI" -> "контроллер"
            if ( id_widg == ID_BUTTON_HC && ntf_code == WM_NOTIFICATION_CLICKED ) {
                RTCHmi( &rtc_hmi ); //дата/время локальных часов
                SendCommand( ID_DEV_RTC, (uint8_t *)&rtc_hmi, sizeof( RTC ) );
                //пользователь закрыл модальное окно
                msg.hWin = DialogCreate[0].Id;
                msg.MsgId = WM_MESSAGE_MODAL_END;
                msg.hWinSrc = WM_GetClientWindow( pMsg->hWin );
                //сообщение окну из которого было вызвано модальное окно
                WM_SendMessage( hwin0, &msg );
                GUI_EndDialog( pMsg->hWin, 0 ); //закрываем форму
               }
            //синхронизировать часы "контроллер" -> "HMI"
            if ( id_widg == ID_BUTTON_CH && ntf_code == WM_NOTIFICATION_CLICKED ) {
                sprintf( new_val, "%02d:%02d:%02d", rtc.hour, rtc.min, rtc.sec );
                RTCSetTime( new_val, SET_MODE_LOCAL );
                sprintf( new_val, "%02d.%02d.%04d", rtc.day, rtc.month, rtc.year );
                RTCSetDate( new_val, SET_MODE_LOCAL );
                //пользователь закрыл модальное окно
                msg.hWin = DialogCreate[0].Id;
                msg.MsgId = WM_MESSAGE_MODAL_END;
                msg.hWinSrc = WM_GetClientWindow( pMsg->hWin );
                //сообщение окну из которого было вызвано модальное окно
                WM_SendMessage( hwin0, &msg );
                GUI_EndDialog( pMsg->hWin, 0 ); //закрываем форму
               }
            //пересчет максимального числа месяца
            if ( ( id_widg == ID_SPINBOX_MONTH || id_widg == ID_SPINBOX_YEAR ) && ntf_code == WM_NOTIFICATION_VALUE_CHANGED ) {
                month = SPINBOX_GetValue( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_MONTH ) );
                year = SPINBOX_GetValue( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_YEAR ) );
                max = DaysOfMonth( month, year );
                SPINBOX_SetRange( WM_GetDialogItem( pMsg->hWin, ID_SPINBOX_DAY ), 1, max );
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
void WinDtime( void ) {

    GUI_GetOrg( &org_x, &org_y ); //координаты текущего виртуального окна
    GUI_ExecDialogBox( DialogCreate, GUI_COUNTOF( DialogCreate ), &cb_Dialog, 0, 0, 0 );
 } 
