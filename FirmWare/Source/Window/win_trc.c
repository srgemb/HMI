
//*************************************************************************************************
//
// Форма управления трекером
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
#define WIN_SIZE_X                  435
#define WIN_SIZE_Y                  440

//*************************************************************************************************
// Определение идентификаторов объектов в форме
//*************************************************************************************************
#define ID_FRAMEWIN_0       (GUI_ID_USER + 8)

#define ID_BUTTON_CLS       (GUI_ID_USER + 0x10 + 0)

#define ID_BUTTON_ACT0      (GUI_ID_USER + 0x10 + 1)
#define ID_BUTTON_ACT1      (GUI_ID_USER + 0x10 + 2)
#define ID_BUTTON_SAVE      (GUI_ID_USER + 0x10 + 3)
#define ID_BUTTON_REST      (GUI_ID_USER + 0x10 + 4)
#define ID_BUTTON_STOP      (GUI_ID_USER + 0x10 + 5)
#define ID_BUTTON_RESET     (GUI_ID_USER + 0x10 + 6)
#define ID_BUTTON_CMD       (GUI_ID_USER + 0x10 + 7)
#define ID_BUTTON_INT       (GUI_ID_USER + 0x10 + 8)
#define ID_BUTTON_VER       (GUI_ID_USER + 0x10 + 9)
#define ID_BUTTON_HOR       (GUI_ID_USER + 0x10 + 10)
#define ID_BUTTON_VERE      (GUI_ID_USER + 0x10 + 11)
#define ID_BUTTON_HORE      (GUI_ID_USER + 0x10 + 12)

#define ID_EDIT_VER         (GUI_ID_USER + 0x10 + 13)
#define ID_EDIT_HOR         (GUI_ID_USER + 0x10 + 14)

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
#define ID_NAME_10          (GUI_ID_USER + 0x20 + 10)
#define ID_NAME_11          (GUI_ID_USER + 0x20 + 11)
#define ID_NAME_12          (GUI_ID_USER + 0x20 + 12)
#define ID_NAME_13          (GUI_ID_USER + 0x20 + 13)

#define ID_VALUE_00         (GUI_ID_USER + 0x40 + 0)
#define ID_VALUE_01         (GUI_ID_USER + 0x40 + 1)
#define ID_VALUE_02         (GUI_ID_USER + 0x40 + 2)
#define ID_VALUE_03         (GUI_ID_USER + 0x40 + 3)
#define ID_VALUE_04         (GUI_ID_USER + 0x40 + 4)
#define ID_VALUE_05         (GUI_ID_USER + 0x40 + 5)
#define ID_VALUE_06         (GUI_ID_USER + 0x40 + 6)
#define ID_VALUE_07         (GUI_ID_USER + 0x40 + 7)
#define ID_VALUE_08         (GUI_ID_USER + 0x40 + 8)
#define ID_VALUE_09         (GUI_ID_USER + 0x40 + 9)
#define ID_VALUE_10         (GUI_ID_USER + 0x40 + 10)
#define ID_VALUE_11         (GUI_ID_USER + 0x40 + 11)

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
#define ID_UNIT_10          (GUI_ID_USER + 0x80 + 10)
#define ID_UNIT_11          (GUI_ID_USER + 0x80 + 11)

//*************************************************************************************************
// Локальные константы
//*************************************************************************************************
#define TEXT_NAME_SIZE1_X           190
#define TEXT_VALUE_POS1_X           TEXT_NAME_SIZE1_X + 20
#define PAR_BUTTON_POS1_X           TEXT_VALUE_POS1_X + 60      //позиция вывода колонки кнопок редактирования

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
static int org_x, org_y;

static const GUI_WIDGET_CREATE_INFO DialogCreate[] = {
    { FRAMEWIN_CreateIndirect, "Состояние солнечного трекера", ID_FRAMEWIN_0, WIN_POS_X( WIN_SIZE_X ), WIN_POS_Y( WIN_SIZE_Y ), WIN_SIZE_X, WIN_SIZE_Y },

    { BUTTON_CreateIndirect, "Закрыть",  ID_BUTTON_CLS, BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },

    { BUTTON_CreateIndirect, "Комн.реж. ВЫКЛ", ID_BUTTON_INT,   BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y-35 ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "Комн.реж. ВКЛ",  ID_BUTTON_CMD,   BUTTON_POS_X( 2, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y-35 ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "Перезапуск",     ID_BUTTON_RESET, BUTTON_POS_X( 3, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y-35 ), BUTTON_SIZE_X, BUTTON_SIZE_Y },

    { BUTTON_CreateIndirect, "Останов",      ID_BUTTON_STOP, BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y-70 ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "Восстановить", ID_BUTTON_REST, BUTTON_POS_X( 2, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y-70 ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "Сохранить",    ID_BUTTON_SAVE, BUTTON_POS_X( 3, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y-70 ), BUTTON_SIZE_X, BUTTON_SIZE_Y },

    { BUTTON_CreateIndirect, "Пит.акт. ВЫКЛ", ID_BUTTON_ACT0, BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y-105 ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "Пит.акт. ВКЛ", ID_BUTTON_ACT1, BUTTON_POS_X( 2, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y-105 ), BUTTON_SIZE_X, BUTTON_SIZE_Y },

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

    { TEXT_CreateIndirect, "Новое значение по вертикали",   ID_NAME_12, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 14 ), TEXT_NAME_SIZE1_X, TEXT_NAME_SIZE_Y },
    { TEXT_CreateIndirect, "Новое значение по горизонтали", ID_NAME_13, TEXT_NAME_POS_X, TEXT_NAME_POS_Y( 16 ), TEXT_NAME_SIZE1_X, TEXT_NAME_SIZE_Y },
    { EDIT_CreateIndirect, "", ID_EDIT_VER, TEXT_VALUE_POS1_X, TEXT_NAME_POS_Y( 14 ), 40, 20, 0, 5, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_HOR, TEXT_VALUE_POS1_X, TEXT_NAME_POS_Y( 16 ), 40, 20, 0, 5, 0 },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_VERE, PAR_BUTTON_POS1_X, TEXT_NAME_POS_Y( 14 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_HORE, PAR_BUTTON_POS1_X, TEXT_NAME_POS_Y( 16 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "Установить", ID_BUTTON_VER, BUTTON_POS_X( 1, WIN_SIZE_X ), TEXT_NAME_POS_Y( 14 ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "Установить", ID_BUTTON_HOR, BUTTON_POS_X( 1, WIN_SIZE_X ), TEXT_NAME_POS_Y( 16 ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
 };

//*************************************************************************************************
// Функция обновления данных в форме
//*************************************************************************************************
static void UpdateWidget( WM_HWIN hwnd ) {

    uint8_t id, rel_id;
    
    //вывод значений параметров
    for ( id = 0; id < DevParamCnt( ID_DEV_TRC, CNT_HMI ); id++ ) {
        rel_id = DevParamRelat( ID_DEV_TRC, id );
        TEXT_SetText( WM_GetDialogItem( hwnd, ID_VALUE_00 + id ), ParamGetForm( ID_DEV_TRC, rel_id, PARAM_VALUE ) );
       }
    if ( ParamGetVal( ID_DEV_TRC, TRC_LINK ).uint8 ) {
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_ACT0 ) );
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_ACT1 ) );
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_SAVE ) );
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_REST ) );
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_STOP ) );
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_RESET ) );
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_CMD ) );
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_INT ) );
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_VER ) );
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_HOR ) );
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_VERE ) );
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_HORE ) );
       }
    else {
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_ACT0 ) );
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_ACT1 ) );
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_SAVE ) );
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_REST ) );
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_STOP ) );
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_RESET ) );
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_CMD ) );
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_INT ) );
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_VER ) );
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_HOR ) );
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_VERE ) );
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_BUTTON_HORE ) );
       }
 }

//*************************************************************************************************
// Функция обратного вызова, обработка сообщений
//*************************************************************************************************
static void cb_Dialog( WM_MESSAGE * pMsg ) {

    CAN_TRC trc;
    double value;
    WM_MESSAGE msg;
    uint8_t id, rel_id;
    char str[10], warn[120], *result;
    int id_widg, ntf_code;

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
            for ( id = 0; id < DevParamCnt( ID_DEV_TRC, CNT_HMI ); id++ ) {
                rel_id = DevParamRelat( ID_DEV_TRC, id );
                //вывод наименования параметра
                TEXT_SetText( WM_GetDialogItem( pMsg->hWin, ID_NAME_00 + id ), ParamGetForm( ID_DEV_TRC, rel_id, PARAM_DESC ) );
                //единицы измерения параметра
                TEXT_SetText( WM_GetDialogItem( pMsg->hWin, ID_UNIT_00 + id ), ParamGetForm( ID_DEV_TRC, rel_id, PARAM_UNIT ) );
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
            //управление реле питания актуаторов
            if ( id_widg == ID_BUTTON_ACT0 && ntf_code == WM_NOTIFICATION_CLICKED ) {
                trc.ctrl = CAN_TRC_POWER;
                trc.power_act = TRC_ACT_OFF;
                SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) );
               }
            if ( id_widg == ID_BUTTON_ACT1 && ntf_code == WM_NOTIFICATION_CLICKED ) {
                trc.ctrl = CAN_TRC_POWER;
                trc.power_act = TRC_ACT_ON;
                SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) );
               }
            //сохранить текущие значения положения в EEPROM
            if ( id_widg == ID_BUTTON_SAVE && ntf_code == WM_NOTIFICATION_CLICKED ) {
                trc.ctrl = CAN_TRC_SAVE;
                SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) );
               }
            //восстановить текущие значения положения из EEPROM
            if ( id_widg == ID_BUTTON_REST && ntf_code == WM_NOTIFICATION_CLICKED ) {
                trc.ctrl = CAN_TRC_REST;
                SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) );
               }
            //командный режим выключить
            if ( id_widg == ID_BUTTON_INT && ntf_code == WM_NOTIFICATION_CLICKED ) {
                trc.ctrl = CAN_TRC_INT;
                SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) );
               }
            //командный режим включить
            if ( id_widg == ID_BUTTON_CMD && ntf_code == WM_NOTIFICATION_CLICKED ) {
                trc.ctrl = CAN_TRC_CMD;
                SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) );
               }
            //остановка позиционирования трекера
            if ( id_widg == ID_BUTTON_STOP && ntf_code == WM_NOTIFICATION_CLICKED ) {
                trc.ctrl = CAN_TRC_STOP;
                SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) );
               }
            //перезапуск контроллера
            if ( id_widg == ID_BUTTON_RESET && ntf_code == WM_NOTIFICATION_CLICKED ) {
                trc.ctrl = CAN_TRC_RESET;
                SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) );
               }
            //позиционирование вертикального актуатора
            if ( id_widg == ID_BUTTON_VER && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_VER ), str, sizeof( str ) );
                trc.ctrl = CAN_TRC_POS;
                trc.type_act = TRC_POS_VERTICAL;
                //определим тип значения параметра позиционирования
                if ( strchr( str, '.' ) == NULL )
                    trc.type_value = TRC_POS_LENGTH;  //значение в мм
                else trc.type_value = TRC_POS_DEGREE; //значение в град.
                trc.value = atof( str );
                SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) );
               }
            //позиционирование горизонтального актуатора
            if ( id_widg == ID_BUTTON_HOR && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_HOR ), str, sizeof( str ) );
                trc.ctrl = CAN_TRC_POS;
                trc.type_act = TRC_POS_HORIZONTAL;
                //определим тип значения параметра позиционирования
                if ( strchr( str, '.' ) == NULL )
                    trc.type_value = TRC_POS_LENGTH;  //значение в мм
                else trc.type_value = TRC_POS_DEGREE; //значение в град.
                trc.value = atof( str );
                SendCommand( ID_DEV_TRC, (uint8_t *)&trc, sizeof( trc ) );
               }
            //корректировка новых параметров
            if ( id_widg == ID_BUTTON_VERE && ntf_code == WM_NOTIFICATION_CLICKED ) {
                //в форму корректировки значения передаем текущее значение
                sprintf( str, "%u", ParamGetVal( ID_DEV_TRC, TRC_VERT_MM ).uint16 );
                //вызов формы корректировки значения
                result = WinKeyPad( str, 5, KEY_DIGITAL | KEY_DOT, MIN_VALUE_VERT, MAX_VALUE_VERT );
                //проверка введенного значения
                if ( result != NULL && strlen( result ) ) {
                    if ( strchr( result, '.' ) == NULL ) {
                        //значение в мм
                        value = atoi( result );
                        if ( value > MAX_VALUE_VERT ) {
                            sprintf( warn, MessageGui( MSG_GUI_ERR_RANGE_MM ), result );
                            MessageBox( MESSAGE_WARNING, warn );
                            break;
                           }
                        EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_VER ), result );
                       }
                    else {
                        //значение в градусах
                        value = atof( result );
                        if ( value < 0 || value > 90 ) {
                            sprintf( warn, MessageGui( MSG_GUI_ERR_RANGE_DEG1 ), result );
                            MessageBox( MESSAGE_WARNING, warn );
                            break;
                           }
                        sprintf( str, "%u", StemVert( value ) );
                        EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_VER ), str );
                        MessageBox( MESSAGE_INFO, MessageGui( MSG_GUI_CALC_DEG_MM ) );
                       }
                   }
               }
            if ( id_widg == ID_BUTTON_HORE && ntf_code == WM_NOTIFICATION_CLICKED ) {
                //в форму корректировки значения передаем текущее значение
                sprintf( str, "%u", ParamGetVal( ID_DEV_TRC, TRC_HORZ_MM ).uint16 );
                //вызов формы корректировки значения
                result = WinKeyPad( str, 5, KEY_DIGITAL | KEY_DOT, MIN_VALUE_HORZ, MAX_VALUE_HORZ );
                //проверка введенного значения
                if ( result != NULL && strlen( result ) ) {
                    if ( strchr( result, '.' ) == NULL ) {
                        //значение в мм
                        value = atoi( result );
                        if ( value > MAX_VALUE_HORZ ) {
                            sprintf( warn, MessageGui( MSG_GUI_ERR_RANGE_MM ), result );
                            MessageBox( MESSAGE_WARNING, warn );
                            break;
                           }
                        EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_HOR ), result );
                       }
                    else {
                        //значение в градусах
                        value = atof( result );
                        if ( value < MIN_ANGLE_AZIMUT || value > MAX_ANGLE_AZIMUT ) {
                            sprintf( warn, MessageGui( MSG_GUI_ERR_RANGE_DEG2 ), result, (float)MIN_ANGLE_AZIMUT, (float)MAX_ANGLE_AZIMUT );
                            MessageBox( MESSAGE_WARNING, warn );
                            break;
                           }
                        sprintf( str, "%u", StemHorz( value ) );
                        EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_HOR ), str );
                        MessageBox( MESSAGE_INFO, MessageGui( MSG_GUI_CALC_DEG_MM ) );
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
void WinTrc( void ) {

    GUI_GetOrg( &org_x, &org_y ); //координаты текущего виртуального окна
    GUI_ExecDialogBox( DialogCreate, GUI_COUNTOF( DialogCreate ), &cb_Dialog, 0, 0, 0 );
 } 
