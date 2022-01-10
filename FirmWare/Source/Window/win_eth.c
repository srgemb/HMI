
//*************************************************************************************************
//
// Форма ввода параметров сетевого интерфейса
//
//*************************************************************************************************

#include "window.h"
#include "dialog.h"
#include "messagebox.h"

//*************************************************************************************************
// Внешние переменные
//*************************************************************************************************
extern WM_HWIN hwin0;

extern CONFIG_HMI cfg_hmi;
extern CONFIG_LCD cfg_lcd;

//*************************************************************************************************
// Размер окна
//*************************************************************************************************
#define WIN_SIZE_X                  540                                      //размер окна по Х
#define WIN_SIZE_Y                  380                                      //размер окна по Y

//*************************************************************************************************
// Определение идентификаторов объектов в форме
//*************************************************************************************************
#define ID_FRAMEWIN_0               (GUI_ID_USER + 12)

#define ID_BUTTON_CLS               (GUI_ID_USER + 0x10 + 1)
#define ID_BUTTON_SAVE              (GUI_ID_USER + 0x10 + 2)

#define ID_BUTTON_MAC               (GUI_ID_USER + 0x20 + 0)
#define ID_BUTTON_IP                (GUI_ID_USER + 0x20 + 1)
#define ID_BUTTON_SUBM              (GUI_ID_USER + 0x20 + 2)
#define ID_BUTTON_GATE              (GUI_ID_USER + 0x20 + 3)
#define ID_BUTTON_DNS1              (GUI_ID_USER + 0x20 + 4)
#define ID_BUTTON_DNS2              (GUI_ID_USER + 0x20 + 5)
#define ID_BUTTON_HOST              (GUI_ID_USER + 0x20 + 6)
#define ID_BUTTON_SOC1              (GUI_ID_USER + 0x20 + 7)
#define ID_BUTTON_SOC2              (GUI_ID_USER + 0x20 + 8)
#define ID_BUTTON_HTTP              (GUI_ID_USER + 0x20 + 9)
#define ID_BUTTON_FTP               (GUI_ID_USER + 0x20 + 10)
#define ID_BUTTON_TCP               (GUI_ID_USER + 0x20 + 11)
#define ID_BUTTON_MADR              (GUI_ID_USER + 0x20 + 12)
#define ID_BUTTON_MPORT             (GUI_ID_USER + 0x20 + 13)
#define ID_BUTTON_MFLG              (GUI_ID_USER + 0x20 + 14)
#define ID_BUTTON_MUSER             (GUI_ID_USER + 0x20 + 15)
#define ID_BUTTON_MPASS             (GUI_ID_USER + 0x20 + 16)
#define ID_BUTTON_MFROM             (GUI_ID_USER + 0x20 + 17)
#define ID_BUTTON_MTO               (GUI_ID_USER + 0x20 + 18)
#define ID_BUTTON_MCC               (GUI_ID_USER + 0x20 + 19)
#define ID_BUTTON_DOFF              (GUI_ID_USER + 0x20 + 20)

#define ID_BUTTON_HTTP1             (GUI_ID_USER + 0x20 + 21)
#define ID_BUTTON_FTP1              (GUI_ID_USER + 0x20 + 22)
#define ID_BUTTON_TCP1              (GUI_ID_USER + 0x20 + 23)

#define ID_NAME_00                  (GUI_ID_USER + 0x40 + 0)
#define ID_NAME_01                  (GUI_ID_USER + 0x40 + 1)
#define ID_NAME_02                  (GUI_ID_USER + 0x40 + 2)
#define ID_NAME_03                  (GUI_ID_USER + 0x40 + 3)
#define ID_NAME_04                  (GUI_ID_USER + 0x40 + 4)
#define ID_NAME_05                  (GUI_ID_USER + 0x40 + 5)
#define ID_NAME_06                  (GUI_ID_USER + 0x40 + 6)
#define ID_NAME_07                  (GUI_ID_USER + 0x40 + 7)
#define ID_NAME_08                  (GUI_ID_USER + 0x40 + 8)
#define ID_NAME_09                  (GUI_ID_USER + 0x40 + 9)
#define ID_NAME_10                  (GUI_ID_USER + 0x40 + 10)
#define ID_NAME_11                  (GUI_ID_USER + 0x40 + 11)
#define ID_NAME_12                  (GUI_ID_USER + 0x40 + 12)
#define ID_NAME_13                  (GUI_ID_USER + 0x40 + 13)
#define ID_NAME_14                  (GUI_ID_USER + 0x40 + 14)
#define ID_NAME_15                  (GUI_ID_USER + 0x40 + 15)
#define ID_NAME_16                  (GUI_ID_USER + 0x40 + 16)
#define ID_NAME_17                  (GUI_ID_USER + 0x40 + 17)
#define ID_NAME_18                  (GUI_ID_USER + 0x40 + 18)
#define ID_NAME_19                  (GUI_ID_USER + 0x40 + 19)
#define ID_NAME_20                  (GUI_ID_USER + 0x40 + 20)
#define ID_NAME_21                  (GUI_ID_USER + 0x40 + 21)

#define ID_CHECKBOX_DHCP            (GUI_ID_USER + 0x80 + 0)
#define ID_EDIT_MAC                 (GUI_ID_USER + 0x80 + 1)
#define ID_EDIT_IP                  (GUI_ID_USER + 0x80 + 2)
#define ID_EDIT_SUBM                (GUI_ID_USER + 0x80 + 3)
#define ID_EDIT_GATE                (GUI_ID_USER + 0x80 + 4)
#define ID_EDIT_DNS1                (GUI_ID_USER + 0x80 + 5)
#define ID_EDIT_DNS2                (GUI_ID_USER + 0x80 + 6)
#define ID_EDIT_HOST                (GUI_ID_USER + 0x80 + 7)
#define ID_EDIT_SOC1                (GUI_ID_USER + 0x80 + 8)
#define ID_EDIT_SOC2                (GUI_ID_USER + 0x80 + 9)
#define ID_EDIT_HTTP                (GUI_ID_USER + 0x80 + 10)
#define ID_EDIT_FTP                 (GUI_ID_USER + 0x80 + 11)
#define ID_EDIT_TCP                 (GUI_ID_USER + 0x80 + 12)
#define ID_EDIT_MADR                (GUI_ID_USER + 0x80 + 13)
#define ID_EDIT_MPORT               (GUI_ID_USER + 0x80 + 14)
#define ID_EDIT_MFLG                (GUI_ID_USER + 0x80 + 15)
#define ID_EDIT_MUSER               (GUI_ID_USER + 0x80 + 16)
#define ID_EDIT_MPASS               (GUI_ID_USER + 0x80 + 17)
#define ID_EDIT_MFROM               (GUI_ID_USER + 0x80 + 18)
#define ID_EDIT_MTO                 (GUI_ID_USER + 0x80 + 19)
#define ID_EDIT_MCC                 (GUI_ID_USER + 0x80 + 20)
#define ID_EDIT_DOFF                (GUI_ID_USER + 0x80 + 21)

//*************************************************************************************************
// Локальные константы
//*************************************************************************************************
#define MAX_LEN_MAC_ADDR            17                                       //кол-во символов поля MAC адреса
#define MAX_LEN_IP_ADDR             15                                       //кол-во символов поля IP адресов
#define MAX_LEN_SOCKET              5                                        //кол-во символов поля номера порта

#define ETH_FIELD_SIZE_Y            20                                       //высота поля параметров
#define ETH_NAME_POS1_X             TEXT_DISTANCE                            //отступ от левого края
#define ETH_NAME_POS2_X             260                                      //позиция Y вывода полей во второй колонке
#define DISTANCE_SIZE_Y             10                                       //расстояние между полями по Y

#define ETH_NAME_SIZE_X             80                                       //размер по X поля наименования параметра
#define ETH_NAME_POS_Y( numb )      ( numb * 15 + ( numb - 1 ) * DISTANCE_SIZE_Y )

#define ETH_VALUE_SIZE_X            120                                      //размер по X поля параметра
#define ETH_VALUE_POS1_X            ETH_NAME_POS1_X + ETH_NAME_SIZE_X        //позиция вывода первой колонки параметров
#define ETH_VALUE_POS2_X            ETH_NAME_POS2_X + ETH_NAME_SIZE_X        //позиция вывода второй колонки параметров

#define ETH_BUTTON_POS1_X           ETH_VALUE_POS1_X + ETH_VALUE_SIZE_X + 10 //позиция вывода первой колонки кнопок редактирования
#define ETH_BUTTON_POS2_X           ETH_VALUE_POS2_X + ETH_VALUE_SIZE_X + 10 //позиция вывода второй колонки кнопок редактирования

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
static int org_x, org_y;

static const GUI_WIDGET_CREATE_INFO DialogCreate[] = {
    { FRAMEWIN_CreateIndirect, "Настройка параметров сетевого интерфейса", ID_FRAMEWIN_0, WIN_POS_X( WIN_SIZE_X ), WIN_POS_Y( WIN_SIZE_Y ), WIN_SIZE_X, WIN_SIZE_Y },

    { BUTTON_CreateIndirect, "Закрыть",   ID_BUTTON_CLS,  BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "Сохранить", ID_BUTTON_SAVE, BUTTON_POS_X( 2, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },

    { BUTTON_CreateIndirect, "...", ID_BUTTON_MAC,   ETH_BUTTON_POS1_X, ETH_NAME_POS_Y( 2 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_IP,    ETH_BUTTON_POS1_X, ETH_NAME_POS_Y( 3 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_SUBM,  ETH_BUTTON_POS1_X, ETH_NAME_POS_Y( 4 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_GATE,  ETH_BUTTON_POS1_X, ETH_NAME_POS_Y( 5 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_DNS1,  ETH_BUTTON_POS1_X, ETH_NAME_POS_Y( 6 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_DNS2,  ETH_BUTTON_POS1_X, ETH_NAME_POS_Y( 7 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_HOST,  ETH_BUTTON_POS1_X, ETH_NAME_POS_Y( 8 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_SOC1,  ETH_BUTTON_POS1_X, ETH_NAME_POS_Y( 9 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_SOC2,  ETH_BUTTON_POS1_X, ETH_NAME_POS_Y( 10 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_DOFF,  ETH_BUTTON_POS1_X, ETH_NAME_POS_Y( 12 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },

    { BUTTON_CreateIndirect, "...", ID_BUTTON_HTTP,  ETH_BUTTON_POS2_X, ETH_NAME_POS_Y( 2 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_FTP,   ETH_BUTTON_POS2_X, ETH_NAME_POS_Y( 3 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_TCP,   ETH_BUTTON_POS2_X, ETH_NAME_POS_Y( 4 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "*",   ID_BUTTON_HTTP1, ETH_BUTTON_POS2_X+30, ETH_NAME_POS_Y( 2 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "*",   ID_BUTTON_FTP1,  ETH_BUTTON_POS2_X+30, ETH_NAME_POS_Y( 3 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "*",   ID_BUTTON_TCP1,  ETH_BUTTON_POS2_X+30, ETH_NAME_POS_Y( 4 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },

    { BUTTON_CreateIndirect, "...", ID_BUTTON_MADR , ETH_BUTTON_POS2_X, ETH_NAME_POS_Y( 5 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_MPORT, ETH_BUTTON_POS2_X, ETH_NAME_POS_Y( 6 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_MFLG , ETH_BUTTON_POS2_X, ETH_NAME_POS_Y( 7 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_MUSER, ETH_BUTTON_POS2_X, ETH_NAME_POS_Y( 8 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_MPASS, ETH_BUTTON_POS2_X, ETH_NAME_POS_Y( 9 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_MFROM, ETH_BUTTON_POS2_X, ETH_NAME_POS_Y( 10 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_MTO  , ETH_BUTTON_POS2_X, ETH_NAME_POS_Y( 11 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTTON_MCC  , ETH_BUTTON_POS2_X, ETH_NAME_POS_Y( 12 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },

    { TEXT_CreateIndirect, "DCHP client", ID_NAME_00, ETH_NAME_POS1_X, ETH_NAME_POS_Y( 1 ),  ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "MAC address", ID_NAME_01, ETH_NAME_POS1_X, ETH_NAME_POS_Y( 2 ),  ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "IP address",  ID_NAME_02, ETH_NAME_POS1_X, ETH_NAME_POS_Y( 3 ),  ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Subnet mask", ID_NAME_03, ETH_NAME_POS1_X, ETH_NAME_POS_Y( 4 ),  ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "IP Gateway",  ID_NAME_04, ETH_NAME_POS1_X, ETH_NAME_POS_Y( 5 ),  ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Primary DNS", ID_NAME_05, ETH_NAME_POS1_X, ETH_NAME_POS_Y( 6 ),  ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Second DNS",  ID_NAME_06, ETH_NAME_POS1_X, ETH_NAME_POS_Y( 7 ),  ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Host name",   ID_NAME_07, ETH_NAME_POS1_X, ETH_NAME_POS_Y( 8 ),  ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "TCP socket1", ID_NAME_08, ETH_NAME_POS1_X, ETH_NAME_POS_Y( 9 ),  ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "TCP socket2", ID_NAME_09, ETH_NAME_POS1_X, ETH_NAME_POS_Y( 10 ), ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Display off", ID_NAME_21, ETH_NAME_POS1_X, ETH_NAME_POS_Y( 12 ), ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    
    { TEXT_CreateIndirect, "User HTTP",   ID_NAME_10, ETH_NAME_POS2_X, ETH_NAME_POS_Y( 2 ), ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "User FTP",    ID_NAME_11, ETH_NAME_POS2_X, ETH_NAME_POS_Y( 3 ), ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "User TCP",    ID_NAME_12, ETH_NAME_POS2_X, ETH_NAME_POS_Y( 4 ), ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Mail addr",   ID_NAME_13, ETH_NAME_POS2_X, ETH_NAME_POS_Y( 5 ), ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Mail port",   ID_NAME_14, ETH_NAME_POS2_X, ETH_NAME_POS_Y( 6 ), ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Mail flags",  ID_NAME_15, ETH_NAME_POS2_X, ETH_NAME_POS_Y( 7 ), ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Mail user",   ID_NAME_16, ETH_NAME_POS2_X, ETH_NAME_POS_Y( 8 ), ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Mail passwd", ID_NAME_17, ETH_NAME_POS2_X, ETH_NAME_POS_Y( 9 ), ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Mail from",   ID_NAME_18, ETH_NAME_POS2_X, ETH_NAME_POS_Y( 10 ), ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Mail to",     ID_NAME_19, ETH_NAME_POS2_X, ETH_NAME_POS_Y( 11 ), ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Mail cc",     ID_NAME_20, ETH_NAME_POS2_X, ETH_NAME_POS_Y( 12 ), ETH_NAME_SIZE_X, ETH_FIELD_SIZE_Y },

    { CHECKBOX_CreateIndirect, "", ID_CHECKBOX_DHCP, ETH_VALUE_POS1_X, ETH_NAME_POS_Y( 1 ), ETH_VALUE_SIZE_X, ETH_FIELD_SIZE_Y, 0, 0, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_MAC,   ETH_VALUE_POS1_X, ETH_NAME_POS_Y( 2 ),  ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, MAX_LEN_MAC_ADDR, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_IP,    ETH_VALUE_POS1_X, ETH_NAME_POS_Y( 3 ),  ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, MAX_LEN_IP_ADDR, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_SUBM,  ETH_VALUE_POS1_X, ETH_NAME_POS_Y( 4 ),  ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, MAX_LEN_IP_ADDR, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_GATE,  ETH_VALUE_POS1_X, ETH_NAME_POS_Y( 5 ),  ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, MAX_LEN_IP_ADDR, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_DNS1,  ETH_VALUE_POS1_X, ETH_NAME_POS_Y( 6 ),  ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, MAX_LEN_IP_ADDR, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_DNS2,  ETH_VALUE_POS1_X, ETH_NAME_POS_Y( 7 ),  ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, MAX_LEN_IP_ADDR, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_HOST,  ETH_VALUE_POS1_X, ETH_NAME_POS_Y( 8 ),  ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, NET_HOSTNAME_LEN, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_SOC1,  ETH_VALUE_POS1_X, ETH_NAME_POS_Y( 9 ),  ETH_VALUE_SIZE_X/2, ETH_FIELD_SIZE_Y, 0, MAX_LEN_SOCKET, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_SOC2,  ETH_VALUE_POS1_X, ETH_NAME_POS_Y( 10 ), ETH_VALUE_SIZE_X/2, ETH_FIELD_SIZE_Y, 0, MAX_LEN_SOCKET, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_DOFF,  ETH_VALUE_POS1_X, ETH_NAME_POS_Y( 12 ), ETH_VALUE_SIZE_X/2, ETH_FIELD_SIZE_Y, 0, MAX_LEN_SOCKET, 0 },

    { EDIT_CreateIndirect, "", ID_EDIT_HTTP,  ETH_VALUE_POS2_X, ETH_NAME_POS_Y( 2 ), ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, MAIL_PARAM_LENGTH, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_FTP,   ETH_VALUE_POS2_X, ETH_NAME_POS_Y( 3 ), ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, MAIL_PARAM_LENGTH, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_TCP,   ETH_VALUE_POS2_X, ETH_NAME_POS_Y( 4 ), ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, MAIL_PARAM_LENGTH, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_MADR,  ETH_VALUE_POS2_X, ETH_NAME_POS_Y( 5 ), ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, MAIL_PARAM_LENGTH, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_MPORT, ETH_VALUE_POS2_X, ETH_NAME_POS_Y( 6 ), ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, MAX_LEN_SOCKET, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_MFLG,  ETH_VALUE_POS2_X, ETH_NAME_POS_Y( 7 ), ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, MAX_LEN_SOCKET, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_MUSER, ETH_VALUE_POS2_X, ETH_NAME_POS_Y( 8 ), ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, MAIL_PARAM_LENGTH, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_MPASS, ETH_VALUE_POS2_X, ETH_NAME_POS_Y( 9 ), ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, MAIL_PARAM_LENGTH, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_MFROM, ETH_VALUE_POS2_X, ETH_NAME_POS_Y( 10 ), ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, MAIL_PARAM_LENGTH, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_MTO,   ETH_VALUE_POS2_X, ETH_NAME_POS_Y( 11 ), ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, MAIL_PARAM_LENGTH, 0 },
    { EDIT_CreateIndirect, "", ID_EDIT_MCC,   ETH_VALUE_POS2_X, ETH_NAME_POS_Y( 12 ), ETH_VALUE_SIZE_X,   ETH_FIELD_SIZE_Y, 0, MAIL_PARAM_LENGTH, 0 }
 };

//*************************************************************************************************
// Обновление полей в форме
//*************************************************************************************************
void UpdateFields( WM_HWIN hwnd ) {

    if ( CHECKBOX_GetState( WM_GetDialogItem( hwnd, ID_CHECKBOX_DHCP ) ) ) {
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_EDIT_IP ) );
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_EDIT_SUBM ) );
        WM_DisableWindow( WM_GetDialogItem( hwnd, ID_EDIT_GATE ) );
       }
    else {
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_EDIT_IP ) );
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_EDIT_SUBM ) );
        WM_EnableWindow( WM_GetDialogItem( hwnd, ID_EDIT_GATE ) );
       }
 }

//*************************************************************************************************
// Сохранение параметров
//*************************************************************************************************
static void SaveConfig( WM_HWIN hwnd ) {

    char data[MAIL_PARAM_LENGTH];
    uint8_t ip4_addr[NET_ADDR_IP4_LEN], mac_addr[NET_ADDR_ETH_LEN];
    
    if ( CHECKBOX_GetState( WM_GetDialogItem( hwnd, ID_CHECKBOX_DHCP ) ) )
        cfg_hmi.used_dhcp = 1;
    else cfg_hmi.used_dhcp = 0;
    //сохраним данные из полей формы в структуру для сохранения в EEPROM
    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_MAC ), data, sizeof( data ) );
    netMAC_aton( data, &mac_addr[0] );
    memcpy( cfg_hmi.mac_addr, mac_addr, sizeof( cfg_hmi.mac_addr ) );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_IP ), data, sizeof( data ) );
    netIP_aton( data, NET_ADDR_IP4, &ip4_addr[0] );
    memcpy( cfg_hmi.ip_addr, ip4_addr, sizeof( cfg_hmi.ip_addr ) );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_SUBM ), data, sizeof( data ) );
    netIP_aton( data, NET_ADDR_IP4, &ip4_addr[0] );
    memcpy( cfg_hmi.ip_mask, ip4_addr, sizeof( cfg_hmi.ip_mask ) );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_GATE ), data, sizeof( data ) );
    netIP_aton( data, NET_ADDR_IP4, &ip4_addr[0] );
    memcpy( cfg_hmi.ip_gate, ip4_addr, sizeof( cfg_hmi.ip_gate ) );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_DNS1 ), data, sizeof( data ) );
    netIP_aton( data, NET_ADDR_IP4, &ip4_addr[0] );
    memcpy( cfg_hmi.ip_dns1, ip4_addr, sizeof( cfg_hmi.ip_dns1 ) );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_DNS2 ), data, sizeof( data ) );
    netIP_aton( data, NET_ADDR_IP4, &ip4_addr[0] );
    memcpy( cfg_hmi.ip_dns2, ip4_addr, sizeof( cfg_hmi.ip_dns2 ) );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_HOST ), data, sizeof( data ) );
    strcpy( cfg_hmi.host_name, data );
    
    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_SOC1 ), data, sizeof( data ) );
    cfg_hmi.ip_port1 = atoi( data );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_SOC2 ), data, sizeof( data ) );
    cfg_hmi.ip_port2 = atoi( data );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_DOFF ), data, sizeof( data ) );
    cfg_lcd.disp_off = atoi( data );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_HTTP ), data, sizeof( data ) );
    strcpy( cfg_hmi.user_name1, data );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_FTP ), data, sizeof( data ) );
    strcpy( cfg_hmi.user_name2, data );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_TCP ), data, sizeof( data ) );
    strcpy( cfg_hmi.user_name3, data );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_MADR ), data, sizeof( data ) );
    strcpy( cfg_hmi.mail_addr, data );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_MPORT ), data, sizeof( data ) );
    cfg_hmi.mail_port = atoi( data );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_MFLG ), data, sizeof( data ) );
    cfg_hmi.mail_flags = atoi( data );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_MUSER ), data, sizeof( data ) );
    strcpy( cfg_hmi.mail_user, data );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_MPASS ), data, sizeof( data ) );
    strcpy( cfg_hmi.mail_passwd, data );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_MFROM ), data, sizeof( data ) );
    strcpy( cfg_hmi.mail_from, data );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_MTO ), data, sizeof( data ) );
    strcpy( cfg_hmi.mail_to, data );

    EDIT_GetText( WM_GetDialogItem( hwnd, ID_EDIT_MCC ), data, sizeof( data ) );
    strcpy( cfg_hmi.mail_cc, data );
    //PS поля passwd_hash1, passwd_hash2, passwd_hash3 - уже содержатся в структуре
    ConfigHmiSave( &cfg_hmi, NULL );
    ConfigLcdSave( &cfg_lcd, NULL );
 }

//*************************************************************************************************
// Функция обратного вызова, обработка сообщений
//*************************************************************************************************
static void cb_Dialog( WM_MESSAGE * pMsg ) {

    WM_MESSAGE msg;
    char ascii[80], *result;
    int id_widg, ntf_code;
    uint8_t ip4_addr[NET_ADDR_IP4_LEN], mac_addr[NET_ADDR_ETH_LEN];

    switch ( pMsg->MsgId ) {
        case WM_INIT_DIALOG:
            //сообщим указатель (HWND) модального окна, окну из которого оно было вызвано
            msg.Data.v = DialogCreate[0].Id;
            msg.MsgId = WM_MESSAGE_MODAL_OPEN;              //тип сообщения
            msg.hWinSrc = WM_GetClientWindow( pMsg->hWin ); //указатель (HWND) модального окна
            WM_SendMessage( hwin0, &msg );
            WM_MakeModal( pMsg->hWin );
            WM_DisableWindow( WM_GetDialogItem( pMsg->hWin, ID_CHECKBOX_DHCP ) );
            WM_SetWindowPos( pMsg->hWin, org_x + DialogCreate[0].x0, org_y + DialogCreate[0].y0, DialogCreate[0].xSize, DialogCreate[0].ySize );
            
            netIF_GetOption( NET_IF_CLASS_ETH, netIF_OptionMAC_Address, mac_addr, sizeof( mac_addr ) );
            netMAC_ntoa( &mac_addr[0], ascii, sizeof( ascii ) );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MAC ), ascii );

            netIF_GetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_Address, ip4_addr, sizeof( ip4_addr ) );
            netIP_ntoa( NET_ADDR_IP4, ip4_addr, ascii, sizeof( ascii ) );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_IP ), ascii );

            netIF_GetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_SubnetMask, ip4_addr, sizeof( ip4_addr ) );
            netIP_ntoa( NET_ADDR_IP4, ip4_addr, ascii, sizeof( ascii ) );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_SUBM ), ascii );

            netIF_GetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_DefaultGateway, ip4_addr, sizeof( ip4_addr ) );
            netIP_ntoa( NET_ADDR_IP4, ip4_addr, ascii, sizeof( ascii ) );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_GATE ), ascii );

            netIF_GetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_PrimaryDNS, ip4_addr, sizeof( ip4_addr ) );
            netIP_ntoa( NET_ADDR_IP4, ip4_addr, ascii, sizeof( ascii ) );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_DNS1 ), ascii );

            netIF_GetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_SecondaryDNS, ip4_addr, sizeof( ip4_addr ) );
            netIP_ntoa( NET_ADDR_IP4, ip4_addr, ascii, sizeof( ascii ) );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_DNS2 ), ascii );

            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_HOST ), netSYS_GetHostName() );

            sprintf( ascii, "%d", cfg_hmi.ip_port1 );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_SOC1 ), ascii );
            sprintf( ascii, "%d", cfg_hmi.ip_port2 );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_SOC2 ), ascii );
            sprintf( ascii, "%u", cfg_lcd.disp_off );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_DOFF ), ascii );
            
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_HTTP ), cfg_hmi.user_name1 );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_FTP ), cfg_hmi.user_name2 );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TCP ), cfg_hmi.user_name3 );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MADR ), cfg_hmi.mail_addr );
            sprintf( ascii, "%d", cfg_hmi.mail_port );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MPORT ), ascii );
            sprintf( ascii, "%d", cfg_hmi.mail_flags );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MFLG ), ascii );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MUSER ), cfg_hmi.mail_user );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MPASS ), cfg_hmi.mail_passwd );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MFROM ), cfg_hmi.mail_from );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MTO ), cfg_hmi.mail_to );
            EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MCC ), cfg_hmi.mail_cc );
            break;
        case WM_NOTIFY_PARENT:
            id_widg = WM_GetId( pMsg->hWinSrc );
            ntf_code = pMsg->Data.v;
            //закрытие формы
            if ( id_widg == ID_BUTTON_CLS && ntf_code == WM_NOTIFICATION_CLICKED ) {
                //пользователь закрыл модальное окно
                msg.Data.v = DialogCreate[0].Id;
                msg.MsgId = WM_MESSAGE_MODAL_END;
                msg.hWinSrc = WM_GetClientWindow( pMsg->hWin );
                //сообщение окну из которого было вызвано модальное окно
                WM_SendMessage( hwin0, &msg );
                GUI_EndDialog( pMsg->hWin, 0 ); //закрываем форму
               }
            //закрытие формы с сохранением параметров
            if ( id_widg == ID_BUTTON_SAVE && ntf_code == WM_NOTIFICATION_CLICKED ) {
                SaveConfig( pMsg->hWin );
                //пользователь закрыл модальное окно
                msg.Data.v = DialogCreate[0].Id;
                msg.MsgId = WM_MESSAGE_MODAL_END;
                msg.hWinSrc = WM_GetClientWindow( pMsg->hWin );
                //сообщение окну из которого было вызвано модальное окно
                WM_SendMessage( hwin0, &msg );
                GUI_EndDialog( pMsg->hWin, 1 );
               }
            if ( id_widg == ID_CHECKBOX_DHCP && ntf_code == WM_NOTIFICATION_CLICKED )
                UpdateFields( pMsg->hWin );
            //MAC адрес
            if ( id_widg == ID_BUTTON_MAC && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MAC ), ascii, sizeof( ascii ) );
                result = WinKeyPad( ascii, MAX_LEN_MAC_ADDR, KEY_GRP_MAC, 0, 0 );
                if ( result != NULL ) {
                    if ( netMAC_aton( result, &mac_addr[0] ) )
                        EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MAC ), result );
                    else MessageBox( MESSAGE_WARNING, MessageGui( MSG_GUI_FORMAT_ERR ) );
                   }
               }
            //IP адрес
            if ( id_widg == ID_BUTTON_IP && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_IP ), ascii, sizeof( ascii ) );
                result = WinKeyPad( ascii, MAX_LEN_IP_ADDR, KEY_GRP_IP, 0, 0 );
                if ( result != NULL ) {
                    if ( netIP_aton( result, NET_ADDR_IP4, &ip4_addr[0] ) )
                        EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_IP ), result );
                    else MessageBox( MESSAGE_WARNING, MessageGui( MSG_GUI_FORMAT_ERR ) );
                   }
               }
            //маска подсети
            if ( id_widg == ID_BUTTON_SUBM && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_SUBM ), ascii, sizeof( ascii ) );
                result = WinKeyPad( ascii, MAX_LEN_IP_ADDR, KEY_GRP_IP, 0, 0 );
                if ( result != NULL ) {
                    if ( netIP_aton( result, NET_ADDR_IP4, &ip4_addr[0] ) )
                        EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_SUBM ), result );
                    else MessageBox( MESSAGE_WARNING, MessageGui( MSG_GUI_FORMAT_ERR ) );
                   }
               }
            //адрес шлюза
            if ( id_widg == ID_BUTTON_GATE && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_GATE ), ascii, sizeof( ascii ) );
                result = WinKeyPad( ascii, MAX_LEN_IP_ADDR, KEY_GRP_IP, 0, 0 );
                if ( result != NULL ) {
                    if ( netIP_aton( result, NET_ADDR_IP4, &ip4_addr[0] ) )
                        EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_GATE ), result );
                    else MessageBox( MESSAGE_WARNING, MessageGui( MSG_GUI_FORMAT_ERR ) );
                   }
               }
            //адрес DNS1
            if ( id_widg == ID_BUTTON_DNS1 && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_DNS1 ), ascii, sizeof( ascii ) );
                result = WinKeyPad( ascii, MAX_LEN_IP_ADDR, KEY_GRP_IP, 0, 0 );
                if ( result != NULL ) {
                    if ( netIP_aton( result, NET_ADDR_IP4, &ip4_addr[0] ) )
                        EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_DNS1 ), result );
                    else MessageBox( MESSAGE_WARNING, MessageGui( MSG_GUI_FORMAT_ERR ) );
                   }
               }
            //адрес DNS2
            if ( id_widg == ID_BUTTON_DNS2 && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_DNS2 ), ascii, sizeof( ascii ) );
                result = WinKeyPad( ascii, MAX_LEN_IP_ADDR, KEY_GRP_IP, 0, 0 );
                if ( result != NULL ) {
                    if ( netIP_aton( result, NET_ADDR_IP4, &ip4_addr[0] ) )
                        EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_DNS2 ), result );
                    else MessageBox( MESSAGE_WARNING, MessageGui( MSG_GUI_FORMAT_ERR ) );
                   }
               }
            //имя хоста
            if ( id_widg == ID_BUTTON_HOST && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_HOST ), ascii, sizeof( ascii ) );
                result = WinKeyBrd( ascii, NET_HOSTNAME_LEN );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_HOST ), result );
               }
            //номер порта для удаленного подключения (командное управление)
            if ( id_widg == ID_BUTTON_SOC1 && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_SOC1 ), ascii, sizeof( ascii ) );
                result = WinKeyPad( ascii, MAX_LEN_SOCKET, KEY_GRP_PORT, 1024, 65535 );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_SOC1 ), result );
               }
            //номер порта для удаленного подключения (с помощью вспомогательного ПО)
            if ( id_widg == ID_BUTTON_SOC2 && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_SOC2 ), ascii, sizeof( ascii ) );
                result = WinKeyPad( ascii, MAX_LEN_SOCKET, KEY_GRP_PORT, 1024, 65535 );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_SOC2 ), result );
               }
            //время отключения дисплея
            if ( id_widg == ID_BUTTON_DOFF && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_DOFF ), ascii, sizeof( ascii ) );
                result = WinKeyPad( ascii, 5, KEY_DIGITAL, 60, 65535 );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_DOFF ), result );
               }
            //имя пользователя при подключении по HTTP
            if ( id_widg == ID_BUTTON_HTTP && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_HTTP ), ascii, sizeof( ascii ) );
                result = WinKeyBrd( ascii, MAIL_PARAM_LENGTH );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_HTTP ), result );
               }
            //имя пользователя при подключении по FTP
            if ( id_widg == ID_BUTTON_FTP && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_FTP ), ascii, sizeof( ascii ) );
                result = WinKeyBrd( ascii, MAIL_PARAM_LENGTH );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_FTP ), result );
               }
            //имя пользователя при подключении по TCP
            if ( id_widg == ID_BUTTON_TCP && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TCP ), ascii, sizeof( ascii ) );
                result = WinKeyBrd( ascii, MAIL_PARAM_LENGTH );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TCP ), result );
               }
            //пароль пользователя для подключении по HTTP
            if ( id_widg == ID_BUTTON_HTTP1 && ntf_code == WM_NOTIFICATION_CLICKED ) {
                memset( ascii, 0x00, sizeof( ascii ) );
                result = WinKeyBrd( ascii, MAIL_PARAM_LENGTH );
                if ( result != NULL )
                    CalcMD5( result, cfg_hmi.passwd_hash1 );
               }
            //пароль пользователя для подключении по FTP
            if ( id_widg == ID_BUTTON_FTP1 && ntf_code == WM_NOTIFICATION_CLICKED ) {
                memset( ascii, 0x00, sizeof( ascii ) );
                result = WinKeyBrd( ascii, MAIL_PARAM_LENGTH );
                if ( result != NULL )
                    CalcMD5( result, cfg_hmi.passwd_hash2 );
               }
            //пароль пользователя для подключении по TCP
            if ( id_widg == ID_BUTTON_TCP1 && ntf_code == WM_NOTIFICATION_CLICKED ) {
                memset( ascii, 0x00, sizeof( ascii ) );
                result = WinKeyBrd( ascii, MAIL_PARAM_LENGTH );
                if ( result != NULL )
                    CalcMD5( result, cfg_hmi.passwd_hash3 );
               }
            //адрес почтового сервера
            if ( id_widg == ID_BUTTON_MADR && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MADR ), ascii, sizeof( ascii ) );
                result = WinKeyBrd( ascii, MAIL_PARAM_LENGTH );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MADR ), result );
               }
            //номер порта почтового сервера
            if ( id_widg == ID_BUTTON_MPORT && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MPORT ), ascii, sizeof( ascii ) );
                result = WinKeyPad( ascii, MAX_LEN_SOCKET, KEY_GRP_PORT, 0, 65535 );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MPORT ), result );
               }
            //флаг почтового сервера
            if ( id_widg == ID_BUTTON_MFLG && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MFLG ), ascii, sizeof( ascii ) );
                result = WinKeyPad( ascii, MAX_LEN_SOCKET, KEY_GRP_PORT, 0, 65535 );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MFLG ), result );
               }
            //имя пользователя почтового сервера
            if ( id_widg == ID_BUTTON_MUSER && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MUSER ), ascii, sizeof( ascii ) );
                result = WinKeyBrd( ascii, MAIL_PARAM_LENGTH );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MUSER ), result );
               }
            //пароль пользователя почтового сервера
            if ( id_widg == ID_BUTTON_MPASS && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MPASS ), ascii, sizeof( ascii ) );
                result = WinKeyBrd( ascii, MAIL_PARAM_LENGTH );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MPASS ), result );
               }
            //адрес отправителя
            if ( id_widg == ID_BUTTON_MFROM && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MFROM ), ascii, sizeof( ascii ) );
                result = WinKeyBrd( ascii, MAIL_PARAM_LENGTH );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MFROM ), result );
               }
            //адрес получателя
            if ( id_widg == ID_BUTTON_MTO && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MTO ), ascii, sizeof( ascii ) );
                result = WinKeyBrd( ascii, MAIL_PARAM_LENGTH );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MTO ), result );
               }
            //адрес получателя копии
            if ( id_widg == ID_BUTTON_MCC && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MCC ), ascii, sizeof( ascii ) );
                result = WinKeyBrd( ascii, MAIL_PARAM_LENGTH );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MCC ), result );
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
void WinEth( void ) {

    GUI_GetOrg( &org_x, &org_y ); //координаты текущего виртуального окна
    GUI_ExecDialogBox( DialogCreate, GUI_COUNTOF( DialogCreate ), &cb_Dialog, 0, 0, 0 );
 } 
