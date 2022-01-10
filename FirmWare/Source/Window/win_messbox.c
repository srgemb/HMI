
//*************************************************************************************************
//
// Форма MessageBox
//
//*************************************************************************************************

#include "dialog.h"
#include "window.h"

//*************************************************************************************************
// Внешние переменные
//*************************************************************************************************
extern WM_HWIN hwin0;
extern GUI_FONT GUI_FontTahoma16;

//*************************************************************************************************
// Размер окна
//*************************************************************************************************
#define WIN_SIZE_X          380
#define WIN_SIZE_Y          140

//*************************************************************************************************
// Определение идентификаторов объектов в форме
//*************************************************************************************************
#define ID_FRAMEWIN_0       (GUI_ID_USER + 18)

#define ID_IMAGE_0          (GUI_ID_USER + 0x20 + 2)
#define ID_TEXT_0           (GUI_ID_USER + 0x20 + 3)
#define ID_BUTTON_MSG0      (GUI_ID_USER + 0x20 + 4)
#define ID_BUTTON_MSG1      (GUI_ID_USER + 0x20 + 5)

//*********************************************************************************************
// Локальные переменные
//*********************************************************************************************
static uint16_t id_frame;
static char *st_text;
static uint8_t st_type;
static int pos_x, pos_y, org_x, org_y;

//Заголовки окна
static char * const header[] = { "Информация", "Внимание", "Ошибка", "Подтверждение" };

//Текст кнопки 1 (слева на право)
static char * const button0[] = { "OK", "OK", "OK", "Нет"  };

//Текст кнопки 2 (слева на право)
static char * const button1[] = { "", "", "", "Да" };

static const GUI_WIDGET_CREATE_INFO DialogCreate1[] = {
    { FRAMEWIN_CreateIndirect, "", ID_FRAMEWIN_0, WIN_POS_X( WIN_SIZE_X ), WIN_POS_Y( WIN_SIZE_Y ), WIN_SIZE_X, WIN_SIZE_Y, 0, 0, 0 },
    { IMAGE_CreateIndirect, "", ID_IMAGE_0, 11, 17, 50, 50, 0, 0, 0 },
    { TEXT_CreateIndirect, "", ID_TEXT_0, 72, 9, 286, 65, 0, 0, 0 },
    { BUTTON_CreateIndirect, "",  ID_BUTTON_MSG0, BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y }
 };

static const GUI_WIDGET_CREATE_INFO DialogCreate2[] = {
    { FRAMEWIN_CreateIndirect, "", ID_FRAMEWIN_0, WIN_POS_X( WIN_SIZE_X ), WIN_POS_Y( WIN_SIZE_Y ), WIN_SIZE_X, WIN_SIZE_Y, 0, 0, 0 },
    { IMAGE_CreateIndirect, "", ID_IMAGE_0, 11, 17, 50, 50, 0, 0, 0 },
    { TEXT_CreateIndirect, "", ID_TEXT_0, 72, 9, 286, 65, 0, 0, 0 },
    { BUTTON_CreateIndirect, "",  ID_BUTTON_MSG0, BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "",  ID_BUTTON_MSG1, BUTTON_POS_X( 2, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
 };

//*********************************************************************************************
// Call-Back функция для MessageBox()
//*********************************************************************************************
static void cb_MessageBox( WM_MESSAGE * pMsg ) {

    U32 FileSize;
    WM_HWIN hItem;
    WM_MESSAGE msg;
    const void *pData;
    int id_widg, ntf_code;

    switch ( pMsg->MsgId ) {
        case WM_INIT_DIALOG:
            //сообщим указатель (HWND) модального окна, окну из которого оно было вызвано
            msg.Data.v = id_frame;
            msg.MsgId = WM_MESSAGE_MODAL_OPEN;              //тип сообщения
            msg.hWinSrc = WM_GetClientWindow( pMsg->hWin ); //указатель (HWND) модального окна
            WM_SendMessage( hwin0, &msg );
            WM_MakeModal( pMsg->hWin );
            WM_SetWindowPos( pMsg->hWin, org_x + pos_x, org_y + pos_y, WM_GetWinSizeX( pMsg->hWin ), WM_GetWinSizeY( pMsg->hWin ) );
            //смена заголовка окна
            FRAMEWIN_SetText( pMsg->hWin, header[st_type] );
            BUTTON_SetText( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_MSG0 ), button0[st_type] );
            if ( st_type == MESSAGE_QUESTION )            
                BUTTON_SetText( WM_GetDialogItem( pMsg->hWin, ID_BUTTON_MSG1 ), button1[st_type] );
            //инициализация иконок
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_0 );
            if ( st_type == MESSAGE_INFO )
                pData = RamDiskFile( "info.bmp", (uint32_t *)&FileSize );
            if ( st_type == MESSAGE_WARNING )
                pData = RamDiskFile( "warn.bmp", (uint32_t *)&FileSize );
            if ( st_type == MESSAGE_ERROR )
                pData = RamDiskFile( "error.bmp", (uint32_t *)&FileSize );
            if ( st_type == MESSAGE_QUESTION )
                pData = RamDiskFile( "quest.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( pMsg->hWin, ID_IMAGE_0 ), pData, FileSize );
            //инициализация текстового поля
            hItem = WM_GetDialogItem( pMsg->hWin, ID_TEXT_0 );
            TEXT_SetWrapMode( hItem, GUI_WRAPMODE_WORD );
            TEXT_SetFont( hItem, &GUI_FontTahoma16 );
            TEXT_SetText( hItem, st_text );
            break;
        case WM_NOTIFY_PARENT:
            //обработка нажатия клавиш
            id_widg = WM_GetId( pMsg->hWinSrc );
            ntf_code = pMsg->Data.v;
            if ( id_widg == ID_BUTTON_MSG0 && ntf_code == WM_NOTIFICATION_CLICKED ) {
                //пользователь закрыл модальное окно
                msg.Data.v = id_frame;
                msg.MsgId = WM_MESSAGE_MODAL_END;
                msg.hWinSrc = WM_GetClientWindow( pMsg->hWin );
                //сообщение окну из которого было вызвано модальное окно
                WM_SendMessage( hwin0, &msg );
                GUI_EndDialog( pMsg->hWin, MB_OK ); //закрываем форму
               }
            if ( id_widg == ID_BUTTON_MSG1 && ntf_code == WM_NOTIFICATION_CLICKED ) {
                //пользователь закрыл модальное окно
                msg.Data.v = id_frame;
                msg.MsgId = WM_MESSAGE_MODAL_END;
                msg.hWinSrc = WM_GetClientWindow( pMsg->hWin );
                //сообщение окну из которого было вызвано модальное окно
                WM_SendMessage( hwin0, &msg );
                GUI_EndDialog( pMsg->hWin, MB_YES ); //закрываем форму
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

//*********************************************************************************************
// Вызов диалога MessageBox
//*********************************************************************************************
uint8_t MessageBox( uint8_t type, char *text ) {

    int result;
    
    if ( type > COUNT_PARAM( header ) )
        type = 0;
    st_type = type;
    st_text = text;
    GUI_GetOrg( &org_x, &org_y ); //координаты текущего виртуального окна
    if ( type == MESSAGE_INFO || type == MESSAGE_WARNING || type == MESSAGE_ERROR ) {
        id_frame = DialogCreate1[0].Id;
        pos_x = DialogCreate1[0].x0; 
        pos_y = DialogCreate1[0].y0;
        result = GUI_ExecDialogBox( DialogCreate1, GUI_COUNTOF( DialogCreate1 ), cb_MessageBox, WM_HBKWIN, 0, 0 );
       }
    if ( type == MESSAGE_QUESTION ) {
        id_frame = DialogCreate2[0].Id;
        pos_x = DialogCreate2[0].x0; 
        pos_y = DialogCreate2[0].y0;
        result = GUI_ExecDialogBox( DialogCreate2, GUI_COUNTOF( DialogCreate2 ), cb_MessageBox, WM_HBKWIN, 0, 0 );
       }
    return result;
 }
 
