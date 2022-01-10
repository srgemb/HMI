
//*************************************************************************************************
//
// Управление отображением объектов и параметров на экране
//
//*************************************************************************************************

#include "rtx_config.h"

#include "dialog.h"
#include "window.h"

//*************************************************************************************************
// Внешние переменные
//*************************************************************************************************
extern GUI_FONT GUI_FontTahoma16b;

//*************************************************************************************************
// Определение идентификаторов объектов в форме
//*************************************************************************************************
#define ID_WINDOW_0             (GUI_ID_USER + 0)
#define ID_WINDOW_1             (GUI_ID_USER + 1)
#define ID_WINDOW_2             (GUI_ID_USER + 2)

//иконки уст-в
#define ID_IMAGE_BAT            (GUI_ID_USER + 0x10 + 0)
#define ID_IMAGE_TS1            (GUI_ID_USER + 0x10 + 1)
#define ID_IMAGE_TS3            (GUI_ID_USER + 0x10 + 2)
#define ID_IMAGE_PB             (GUI_ID_USER + 0x10 + 3)
#define ID_IMAGE_MPPT           (GUI_ID_USER + 0x10 + 4)
#define ID_IMAGE_ALT            (GUI_ID_USER + 0x10 + 5)
#define ID_IMAGE_GEN            (GUI_ID_USER + 0x10 + 6)
#define ID_IMAGE_SPA            (GUI_ID_USER + 0x10 + 7)
#define ID_IMAGE_TRC            (GUI_ID_USER + 0x10 + 8)
#define ID_IMAGE_SND            (GUI_ID_USER + 0x10 + 9)
#define ID_IMAGE_LEP            (GUI_ID_USER + 0x10 + 10)
#define ID_IMAGE_GRAPH          (GUI_ID_USER + 0x10 + 11)
//иконки параметров
#define ID_IMAGE_PAR            (GUI_ID_USER + 0x10 + 12)
#define ID_IMAGE_ETH            (GUI_ID_USER + 0x10 + 13)

//связи между объектами, схема без АВР
#define ID_IMAGE_PB_MPPT_LINK1  (GUI_ID_USER + 0x40 + 0)
#define ID_IMAGE_BAT_LINK1      (GUI_ID_USER + 0x40 + 1)

//связи между объектами, схема с АВР
#define ID_IMAGE_GEN_LINK2      (GUI_ID_USER + 0x40 + 2)
#define ID_IMAGE_LEP_LINK2      (GUI_ID_USER + 0x40 + 3)
#define ID_IMAGE_ALT_LINK       (GUI_ID_USER + 0x40 + 4)
#define ID_IMAGE_TS_LINK2       (GUI_ID_USER + 0x40 + 5)
#define ID_IMAGE_BAT_LINK2      (GUI_ID_USER + 0x40 + 6)
#define ID_IMAGE_PB_LINK2       (GUI_ID_USER + 0x40 + 7)
#define ID_IMAGE_MPPT_LINK2     (GUI_ID_USER + 0x40 + 8)

//вывод значений
#define ID_TIME                 (GUI_ID_USER + 0x20 + 0)
#define ID_DATE                 (GUI_ID_USER + 0x20 + 1)
#define ID_LINK1                (GUI_ID_USER + 0x20 + 2)
#define ID_BAT_VOL              (GUI_ID_USER + 0x20 + 3)
#define ID_BAT_CUR              (GUI_ID_USER + 0x20 + 4)
#define ID_BAT_SOC              (GUI_ID_USER + 0x20 + 5)
#define ID_BAT_TTG              (GUI_ID_USER + 0x20 + 6)
#define ID_PB_CUR               (GUI_ID_USER + 0x20 + 7)
#define ID_TS1_PWR              (GUI_ID_USER + 0x20 + 8)
#define ID_TS1_PRC              (GUI_ID_USER + 0x20 + 9)
#define ID_TS3_PWR              (GUI_ID_USER + 0x20 + 10)
#define ID_TS3_PRC              (GUI_ID_USER + 0x20 + 11)
#define ID_MPPT_VOL             (GUI_ID_USER + 0x20 + 12)
#define ID_MPPT_CUR             (GUI_ID_USER + 0x20 + 13)
#define ID_ALT_STAT             (GUI_ID_USER + 0x20 + 14)

//*************************************************************************************************
// Локальные константы
//*************************************************************************************************
//размеры окон
#define X_SIZE_DISP             C_GLCD_H_SIZE
#define Y_SIZE_DISP             C_GLCD_V_SIZE

//логические координаты для окна мнемосхемы без АВР
#define X0_POS                  0
#define Y0_POS                  0

//логические координаты для окна мнемосхемы с АВР
#define X1_POS                  0
#define Y1_POS                  C_GLCD_V_SIZE

typedef enum bat2_status {
    BAT_NOT_CONNECT = 0,            //АКБ не подключена
    BAT_LEVEL_20 = 20,              //уровень заряда 20%
    BAT_LEVEL_30 = 30,              //уровень заряда 30%
    BAT_LEVEL_50 = 50,              //уровень заряда 50%
    BAT_LEVEL_60 = 60,              //уровень заряда 60%
    BAT_LEVEL_70 = 70,              //уровень заряда 70%
    BAT_LEVEL_80 = 80,              //уровень заряда 80%
    BAT_LEVEL_90 = 90,              //уровень заряда 90%
    BAT_LEVEL_100 = 100             //уровень заряда 100%
 } BAT_STATUS2;

#define VALUE_SIZE_X            128                 //размер поля по X для вывода значений
//экран #1
#define PB_VALUE_POS_X          5                   //по X вывод значений PB-1000-224
#define PB_VALUE_POS_Y          138                 //по Y вывод значений PB-1000-224

#define MPPT_VALUE_POS_X        5                   //по X вывод значений MPPT
#define MPPT_VALUE_POS_Y        378                 //по Y вывод значений MPPT

#define BAT_VALUE_POS1_X        251                 //по X вывод значений BATMON
#define BAT_VALUE_POS1_Y        305                 //по Y вывод значений BATMON

#define TS1_VALUE_POS1_X        507                 //по X вывод значений TS-1000-224
#define TS1_VALUE_POS1_Y        138                 //по Y вывод значений TS-1000-224
#define TS3_VALUE_POS1_X        507                 //по X вывод значений TS-3000-224
#define TS3_VALUE_POS1_Y        378                 //по Y вывод значений TS-3000-224
//экран #2
#define ALT_VALUE_POS_X         424                 //по X вывод значений 
#define ALT_VALUE_POS_Y         138                 //по Y вывод значений 

#define BAT_VALUE_POS2_X        175                 //по X вывод значений BATMON
#define BAT_VALUE_POS2_Y        380                 //по Y вывод значений BATMON

#define TS1_VALUE_POS2_X        359                 //по X вывод значений TS-1000-224
#define TS1_VALUE_POS2_Y        380                 //по Y вывод значений TS-1000-224
#define TS3_VALUE_POS2_X        496                 //по X вывод значений TS-3000-224
#define TS3_VALUE_POS2_Y        380                 //по Y вывод значений TS-3000-224

#define POS_Y( start, numb )    ( start + ( 20 * ( numb - 1 ) ) )

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
//Вариант мнемосхемы без АВР
static const GUI_WIDGET_CREATE_INFO DialogCreate1[] = {
    { WINDOW_CreateIndirect, NULL, ID_WINDOW_1, X0_POS, Y0_POS, X_SIZE_DISP, Y_SIZE_DISP, WM_CF_SHOW | WM_CF_MEMDEV, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_BAT,  251, 171, 128, 128, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_TS1,  507, 5,   128, 128, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_TS3,  507, 244, 128, 128, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_PB,   5,   5,   128, 128, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_SPA,  70,  180, 64,  64,  0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_TRC,  5,   180, 64,  64,  0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_MPPT, 5,   245, 128, 128, 0, 0, 0 },
    //
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_PAR, 3,   428, 48, 48, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_ETH, 55,  428, 48, 48, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_SND, 107, 428, 48, 48, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_GRAPH, 380, 428, 48, 48, 0, 0, 0 },
    //
    { TEXT_CreateIndirect, NULL, ID_TIME,  530, 440, 110, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_DATE,  530, 460, 110, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_LINK1, 435, 460, 100, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    //
    { TEXT_CreateIndirect, NULL, ID_PB_CUR,   PB_VALUE_POS_X,   POS_Y( PB_VALUE_POS_Y, 1 ),   VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_MPPT_VOL, MPPT_VALUE_POS_X, POS_Y( MPPT_VALUE_POS_Y, 1 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_MPPT_CUR, MPPT_VALUE_POS_X, POS_Y( MPPT_VALUE_POS_Y, 2 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_BAT_VOL,  BAT_VALUE_POS1_X, POS_Y( BAT_VALUE_POS1_Y, 1 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_BAT_CUR,  BAT_VALUE_POS1_X, POS_Y( BAT_VALUE_POS1_Y, 2 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_BAT_SOC,  BAT_VALUE_POS1_X, POS_Y( BAT_VALUE_POS1_Y, 3 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_BAT_TTG,  BAT_VALUE_POS1_X, POS_Y( BAT_VALUE_POS1_Y, 4 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_TS1_PWR,  TS1_VALUE_POS1_X, POS_Y( TS1_VALUE_POS1_Y, 1 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_TS1_PRC,  TS1_VALUE_POS1_X, POS_Y( TS1_VALUE_POS1_Y, 2 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_TS3_PWR,  TS3_VALUE_POS1_X,  POS_Y( TS3_VALUE_POS1_Y, 1 ),  VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_TS3_PRC,  TS3_VALUE_POS1_X,  POS_Y( TS3_VALUE_POS1_Y, 2 ),  VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    //связи между объектами
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_PB_MPPT_LINK1, 145, 70, 97, 269, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_BAT_LINK1,     395, 55, 99, 269, 0, 0, 0 }
};

//Вариант мнемосхемы с АВР
static const GUI_WIDGET_CREATE_INFO DialogCreate2[] = {
    { WINDOW_CreateIndirect, NULL, ID_WINDOW_2, X1_POS, Y1_POS, X_SIZE_DISP, Y_SIZE_DISP, WM_CF_SHOW | WM_CF_MEMDEV, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_BAT,  175, 245, 128, 128, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_TS1,  359, 244, 128, 128, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_TS3,  496, 244, 128, 128, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_PB,   5,   5,   128, 128, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_SPA,  70,  180, 64,  64,  0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_TRC,  5,   180, 64,  64,  0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_MPPT, 5,   245, 128, 128, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_ALT,  424, 7,   128, 128, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_LEP,  282, 5,   64,  64,  0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_GEN,  282, 73,  64,  64,  0, 0, 0 },
    //
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_PAR,   3,   428, 48, 48, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_ETH,   55,  428, 48, 48, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_SND,   107, 428, 48, 48, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_GRAPH, 380, 428, 48, 48, 0, 0, 0 },
    //
    { TEXT_CreateIndirect, NULL, ID_TIME,  530, 440, 110, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_DATE,  530, 460, 110, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_LINK1, 435, 460, 100, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    //
    { TEXT_CreateIndirect, NULL, ID_PB_CUR,   PB_VALUE_POS_X,   POS_Y( PB_VALUE_POS_Y, 1 ),   VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_MPPT_VOL, MPPT_VALUE_POS_X, POS_Y( MPPT_VALUE_POS_Y, 1 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_MPPT_CUR, MPPT_VALUE_POS_X, POS_Y( MPPT_VALUE_POS_Y, 2 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_BAT_VOL,  BAT_VALUE_POS2_X, POS_Y( BAT_VALUE_POS2_Y, 1 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_BAT_CUR,  BAT_VALUE_POS2_X, POS_Y( BAT_VALUE_POS2_Y, 2 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_BAT_SOC,  BAT_VALUE_POS2_X, POS_Y( BAT_VALUE_POS2_Y, 3 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_BAT_TTG,  BAT_VALUE_POS2_X, POS_Y( BAT_VALUE_POS2_Y, 4 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_TS1_PWR,  TS1_VALUE_POS2_X, POS_Y( TS1_VALUE_POS2_Y, 1 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_TS1_PRC,  TS1_VALUE_POS2_X, POS_Y( TS1_VALUE_POS2_Y, 2 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_TS3_PWR,  TS3_VALUE_POS2_X, POS_Y( TS3_VALUE_POS2_Y, 1 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_TS3_PRC,  TS3_VALUE_POS2_X, POS_Y( TS3_VALUE_POS2_Y, 2 ), VALUE_SIZE_X, TEXT_UNIT_SIZE_Y, 0, 0, 0 },
    { TEXT_CreateIndirect, NULL, ID_ALT_STAT, ALT_VALUE_POS_X,  POS_Y( ALT_VALUE_POS_Y, 1 ),  VALUE_SIZE_X, TEXT_UNIT_SIZE_Y*2, 0, 0, 0 },
    //связи между объектами
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_GEN_LINK2,  353, 82,  69,  39,  0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_LEP_LINK2,  353, 16,  69,  39,  0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_ALT_LINK,   557, 40,  79,  55,  0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_TS_LINK2,   414, 175, 155, 69,  0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_BAT_LINK2,  299, 290, 44,  40,  0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_PB_LINK2,   162, 71,  99,  170, 0, 0, 0 },
    { IMAGE_CreateIndirect, NULL, ID_IMAGE_MPPT_LINK2, 140, 290, 44,  40,  0, 0, 0 }
};

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
STACK_HWND stack_hwnd[10], stc_hwnd;
WM_HWIN hwin0, hwin1, hwin2;
static uint8_t bat_stat2 = 0, bat_prev2 = 0;
static BAT_STATUS2 bat_stat1 = BAT_NOT_CONNECT, bat_prev1 = BAT_NOT_CONNECT; 
static char * const link_state[] = { "LINK NO", "LINK ERROR", "LINK OK" };
static bool pb_flg = false, mppt_flg = false, inv1_flg = false, inv2_flg = false;
static uint8_t alt_stat = 0, charge_stat = 0, mppt_stat = 0, lep_stat = 0; 
static uint8_t trc_stat = 0, inv1_stat = 0, inv2_stat = 0, gen_stat = 0, genac_stat = 0;
static uint8_t alt_prev = 0, charge_prev = 0, mppt_prev = 0, lep_prev = 0;
static uint8_t trc_prev = 0, inv1_prev = 0, inv2_prev = 0, gen_prev = 0, genac_prev = 0;

//*************************************************************************************************
// Прототипы локальных функций
//*************************************************************************************************
static void StackHwndInit( void );
static void StackHwndPush( WM_HWIN hWin, int id_frame );
static void StackHwndPop( STACK_HWND *result );
static void CheckAlt( void );
static void cb_Dialog1( WM_MESSAGE *pMsg );
static void cb_Dialog2( WM_MESSAGE *pMsg );
static BAT_STATUS2 BatStat2( void );

//*************************************************************************************************
// Функция обновления данных, времени и даты, текстовых значений V/I/P уст-в
//*************************************************************************************************
static void UpdateWidget( WM_HWIN hwnd ) {

    char text[40];
    uint8_t stat;
    
    if ( hwnd == NULL )
        return;
    TEXT_SetText( WM_GetDialogItem( hwnd, ID_TIME ), RTCGetTime( NULL ) );
    TEXT_SetText( WM_GetDialogItem( hwnd, ID_DATE ), RTCGetDowDate( NULL, DOW_RUS_LANG ) );
    TEXT_SetText( WM_GetDialogItem( hwnd, ID_LINK1 ), link_state[LinkStat()] );
    //состояние связи
    stat = LinkStat();
    if ( stat == CAN_LINK_OK )
        TEXT_SetTextColor( WM_GetDialogItem( hwnd, ID_LINK1 ), GUI_GREEN );
    if ( stat == CAN_LINK_NO )
        TEXT_SetTextColor( WM_GetDialogItem( hwnd, ID_LINK1 ), GUI_RED );
    if ( stat == CAN_LINK_ERROR )
        TEXT_SetTextColor( WM_GetDialogItem( hwnd, ID_LINK1 ), GUI_ORANGE );
    //монитор АКБ
    sprintf( text, "V = %s V", ParamGetForm( ID_DEV_BATMON, MON_VOLTAGE, PARAM_VALUE ) );
    TEXT_SetText( WM_GetDialogItem( hwnd, ID_BAT_VOL ), text );
    sprintf( text, "I = %s A", ParamGetForm( ID_DEV_BATMON, MON_CURRENT, PARAM_VALUE ) );
    TEXT_SetText( WM_GetDialogItem( hwnd, ID_BAT_CUR ), text );
    sprintf( text, "SOC = %s %%", ParamGetForm( ID_DEV_BATMON, MON_SOC, PARAM_VALUE ) );
    TEXT_SetText( WM_GetDialogItem( hwnd, ID_BAT_SOC ), text );
    sprintf( text, "TTG = %s", ParamGetForm( ID_DEV_BATMON, MON_TTG, PARAM_VALUE ) );
    TEXT_SetText( WM_GetDialogItem( hwnd, ID_BAT_TTG ), text );
    //блок АВР
    if ( alt_stat ) {
        sprintf( text, "%s", ParamGetForm( ID_DEV_ALT, ALT_POWER_SRC, PARAM_VALUE ) );
        if ( strlen( text ) > 5 )
            sprintf( text, "Питание нагрузки от: %s", ParamGetForm( ID_DEV_ALT, ALT_POWER_SRC, PARAM_VALUE ) );
        TEXT_SetText( WM_GetDialogItem( hwin2, ID_ALT_STAT ), text );
       }
    //ток заряда PB-1000-224
    if ( pb_flg ) {
        sprintf( text, "I = %s A", ParamGetForm( ID_DEV_CHARGER, CHARGE_CURRENT, PARAM_VALUE ) );
        TEXT_SetText( WM_GetDialogItem( hwnd, ID_PB_CUR ), text );
       }
    //контроллер MPPT: ток, напряжение 
    if ( mppt_flg ) {
        sprintf( text, "V = %s V", ParamGetForm( ID_DEV_MPPT, MPPT_OUT_VOLTAGE, PARAM_VALUE ) );
        TEXT_SetText( WM_GetDialogItem( hwnd, ID_MPPT_VOL ), text );
        sprintf( text, "I = %s A", ParamGetForm( ID_DEV_MPPT, MPPT_OUT_CURRENT, PARAM_VALUE ) );
        TEXT_SetText( WM_GetDialogItem( hwnd, ID_MPPT_CUR ), text );
       }
    //инвертор TS-1000-224
    if ( inv1_flg ) {
        sprintf( text, "P = %s W", ParamGetForm( ID_DEV_INV1, INV_POWER_WATT, PARAM_VALUE ) );
        TEXT_SetText( WM_GetDialogItem( hwnd, ID_TS1_PWR ), text );
        sprintf( text, "P = %s %%", ParamGetForm( ID_DEV_INV1, INV_POWER_PERC, PARAM_VALUE ) );
        TEXT_SetText( WM_GetDialogItem( hwnd, ID_TS1_PRC ), text );
       }
    //инвертор TS-3000-224
    if ( inv2_flg ) {
        sprintf( text, "P = %s W", ParamGetForm( ID_DEV_INV2, INV_POWER_WATT, PARAM_VALUE ) );
        TEXT_SetText( WM_GetDialogItem( hwnd, ID_TS3_PWR ), text );
        sprintf( text, "P = %s %%", ParamGetForm( ID_DEV_INV2, INV_POWER_PERC, PARAM_VALUE ) );
        TEXT_SetText( WM_GetDialogItem( hwnd, ID_TS3_PRC ), text );
       }
 }
 
//*************************************************************************************************
// Проверка изменения состояния уст-в, при изменении состояния активируем/деактивируем
// соответствующее уст-во
//*************************************************************************************************
static void CheckStatusDev( void ) {

    U32 FileSize;
    char fname[80];
    const void *pData;
    
    //текущее состояние уст-в
    bat_stat1 = BatStat2();
    bat_stat2 = ParamGetVal( ID_DEV_PORTS, BAT_CONNECT ).uint8;
    mppt_stat = ParamGetVal( ID_DEV_MPPT, MPPT_POWER ).uint8;
    trc_stat = ParamGetVal( ID_DEV_TRC, TRC_LINK ).uint8;
    inv1_stat = ParamGetVal( ID_DEV_INV1, INV_DC_CONNECT ).uint8;
    inv2_stat = ParamGetVal( ID_DEV_INV2, INV_DC_CONNECT ).uint8;
    gen_stat = ParamGetVal( ID_DEV_GEN, GEN_PAR_CONN ).uint8;
    lep_stat = ParamGetVal( ID_DEV_ALT, ALT_MAIN_AC ).uint8;
    genac_stat = ParamGetVal( ID_DEV_ALT, ALT_GEN_ON ).uint8;
    charge_stat = ParamGetVal( ID_DEV_CHARGER, CHARGE_MODE ).uint8;
    //проверим состояние уст-в
    if ( bat_prev1 != bat_stat1 ) {
        //изменение состояние АКБ
        bat_prev1 = bat_stat1;
        sprintf( fname, "bat_%u.bmp", bat_stat1 );
        pData = RamDiskFile( fname, (uint32_t *)&FileSize );
        IMAGE_SetBMP( WM_GetDialogItem( hwin1, ID_IMAGE_BAT ), pData, FileSize );
        IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_BAT ), pData, FileSize );
       }
    if ( bat_prev2 != bat_stat2 ) {
        //изменение состояние АКБ
        bat_prev2 = bat_stat2;
        if ( bat_stat2 ) {
            pData = RamDiskFile( "pb_link21.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_PB_LINK2 ), pData, FileSize );
           }
        else {
            pData = RamDiskFile( "pb_link20.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_PB_LINK2 ), pData, FileSize );
           }
       }
    if ( charge_prev != charge_stat ) {
        //статус зарядного уст-ва изменился
        if ( !charge_prev && charge_stat ) {
            //уст-во доступно (включено)
            pb_flg = true;
            pData = RamDiskFile( "charge1.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin1, ID_IMAGE_PB ), pData, FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_PB ), pData, FileSize );
           }
        if ( charge_prev && !charge_stat ) {
            //уст-во не доступно (выключено)
            pb_flg = false;
            pData = RamDiskFile( "charge0.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin1, ID_IMAGE_PB ), pData, FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_PB ), pData, FileSize );
            TEXT_SetText( WM_GetDialogItem( hwin1, ID_PB_CUR ), "" );
            TEXT_SetText( WM_GetDialogItem( hwin2, ID_PB_CUR ), "" );
           }
        charge_prev = charge_stat;
        LCDBackLigth( LCD_BACK_LIGTH_ON );
       }
    if ( mppt_prev != mppt_stat ) {
        //статус MPPT контроллера заряда изменился
        if ( !mppt_prev && mppt_stat ) {
            //уст-во доступно (включено)
            mppt_flg = true;
            pData = RamDiskFile( "mppt1.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin1, ID_IMAGE_MPPT ), pData, FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_MPPT ), pData, FileSize );
           }
        if ( mppt_prev && !mppt_stat ) {
            //уст-во не доступно (выключено)
            mppt_flg = false;
            pData = RamDiskFile( "mppt0.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin1, ID_IMAGE_MPPT ), pData, FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_MPPT ), pData, FileSize );
            TEXT_SetText( WM_GetDialogItem( hwin1, ID_MPPT_VOL ), "" );
            TEXT_SetText( WM_GetDialogItem( hwin1, ID_MPPT_CUR ), "" );
            TEXT_SetText( WM_GetDialogItem( hwin2, ID_MPPT_VOL ), "" );
            TEXT_SetText( WM_GetDialogItem( hwin2, ID_MPPT_CUR ), "" );
           }
        mppt_prev = mppt_stat; 
        LCDBackLigth( LCD_BACK_LIGTH_ON );
       }
    if ( trc_prev != trc_stat ) {
        //статус трекера
        if ( !trc_prev && trc_stat ) {
            //уст-во доступно (включено)
            pData = RamDiskFile( "trc1.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin1, ID_IMAGE_TRC ), pData, FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_TRC ), pData, FileSize );
           }
        if ( trc_prev && !trc_stat ) {
            //уст-во не доступно (выключено)
            pData = RamDiskFile( "trc0.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin1, ID_IMAGE_TRC ), pData, FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_TRC ), pData, FileSize );
           }
        trc_prev = trc_stat; 
        LCDBackLigth( LCD_BACK_LIGTH_ON );
       }
    if ( inv1_prev != inv1_stat ) {
        //статус инвертора TS-1000-224
        if ( !inv1_prev && inv1_stat ) {
            //уст-во доступно (включено)
            inv1_flg = true;
            pData = RamDiskFile( "ts11.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin1, ID_IMAGE_TS1 ), pData, FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_TS1 ), pData, FileSize );
           }
        if ( inv1_prev && !inv1_stat ) {
            //уст-во не доступно (выключено)
            inv1_flg = false;
            pData = RamDiskFile( "ts10.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin1, ID_IMAGE_TS1 ), pData, FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_TS1 ), pData, FileSize );
            TEXT_SetText( WM_GetDialogItem( hwin1, ID_TS1_PWR ), "" );
            TEXT_SetText( WM_GetDialogItem( hwin1, ID_TS1_PRC ), "" );
            TEXT_SetText( WM_GetDialogItem( hwin2, ID_TS1_PWR ), "" );
            TEXT_SetText( WM_GetDialogItem( hwin2, ID_TS1_PRC ), "" );
           }
        inv1_prev = inv1_stat; 
        LCDBackLigth( LCD_BACK_LIGTH_ON );
       }
    if ( inv2_prev != inv2_stat ) {
        //статус инвертора TS-3000-224
        if ( !inv2_prev && inv2_stat ) {
            //уст-во доступно (включено)
            inv2_flg = true;
            pData = RamDiskFile( "ts31.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin1, ID_IMAGE_TS3 ), pData, FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_TS3 ), pData, FileSize );
           }
        if ( inv2_prev && !inv2_stat ) {
            //уст-во не доступно (выключено)
            inv2_flg = false;
            pData = RamDiskFile( "ts30.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin1, ID_IMAGE_TS3 ), pData, FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_TS3 ), pData, FileSize );
            TEXT_SetText( WM_GetDialogItem( hwin1, ID_TS3_PWR ), "" );
            TEXT_SetText( WM_GetDialogItem( hwin1, ID_TS3_PRC ), "" );
            TEXT_SetText( WM_GetDialogItem( hwin2, ID_TS3_PWR ), "" );
            TEXT_SetText( WM_GetDialogItem( hwin2, ID_TS3_PRC ), "" );
           }
        inv2_prev = inv2_stat; 
        LCDBackLigth( LCD_BACK_LIGTH_ON );
       }
    if ( lep_prev != lep_stat ) {
        //статус зарядного уст-ва изменился
        if ( !lep_prev && lep_stat ) {
            //уст-во доступно (включено)
            pData = RamDiskFile( "lep1.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_LEP ), pData, FileSize );
            pData = RamDiskFile( "lep_link21.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_LEP_LINK2 ), pData, FileSize );
           }
        if ( lep_prev && !lep_stat ) {
            //уст-во не доступно (выключено)
            pData = RamDiskFile( "lep0.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_LEP ), pData, FileSize );
            pData = RamDiskFile( "lep_link20.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_LEP_LINK2 ), pData, FileSize );
           }
        lep_prev = lep_stat; 
        LCDBackLigth( LCD_BACK_LIGTH_ON );
       }
    if ( gen_prev != gen_stat ) {
        //состояние подключения генератора
        if ( !gen_prev && gen_stat ) {
            //уст-во доступно (включено)
            pData = RamDiskFile( "gen1.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_GEN ), pData, FileSize );
           }
        if ( gen_prev && !gen_stat ) {
            //уст-во не доступно (выключено)
            pData = RamDiskFile( "gen0.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_GEN ), pData, FileSize );
           }
        gen_prev = gen_stat; 
        LCDBackLigth( LCD_BACK_LIGTH_ON );
       }
    if ( genac_prev != genac_stat ) {
        //состояние наличия AC сети от генератора
        if ( !genac_prev && genac_stat ) {
            //уст-во доступно (включено)
            pData = RamDiskFile( "gen_link21.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_GEN ), pData, FileSize );
           }
        if ( genac_prev && !genac_stat ) {
            //уст-во не доступно (выключено)
            pData = RamDiskFile( "gen_link20.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( WM_GetDialogItem( hwin2, ID_IMAGE_GEN ), pData, FileSize );
           }
        genac_prev = genac_stat; 
        LCDBackLigth( LCD_BACK_LIGTH_ON );
       }
 }

//*************************************************************************************************
// Функция обратного вызова окна, мнемосхема без АВР
//*************************************************************************************************
static void cb_Dialog1( WM_MESSAGE *pMsg ) {

    const void *pData;
    WM_HWIN hItem;
    int id_widg, ntf_code;
    U32 FileSize;

    switch ( pMsg->MsgId ) {
        case WM_INIT_DIALOG:
            WINDOW_SetBkColor( pMsg->hWin, GUI_BLACK );
            //BAT
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_BAT );
            pData = RamDiskFile( "bat_0.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //TS-1000-224'
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_TS1 );
            pData = RamDiskFile( "ts10.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //TS-3000-224'
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_TS3 );
            pData = RamDiskFile( "ts30.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //PB-1000-224
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_PB );
            pData = RamDiskFile( "charge0.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //MPPT
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_MPPT );
            pData = RamDiskFile( "mppt0.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //SPA
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_SPA );
            pData = RamDiskFile( "sun.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //трекер
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_TRC );
            pData = RamDiskFile( "trc0.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //параметры
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_PAR );
            pData = RamDiskFile( "param.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_ETH );
            pData = RamDiskFile( "ethernet.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_SND );
            pData = RamDiskFile( "sound.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_GRAPH );
            pData = RamDiskFile( "graph.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //связи между объектами
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_PB_MPPT_LINK1 );
            pData = RamDiskFile( "pb_mppt_link1.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_BAT_LINK1 );
            pData = RamDiskFile( "bat_link1.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //параметры АКБ
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BAT_VOL ), &GUI_FontTahoma16b );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BAT_CUR ), &GUI_FontTahoma16b );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BAT_SOC ), &GUI_FontTahoma16b );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BAT_TTG ), &GUI_FontTahoma16b );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_BAT_VOL ), GUI_WHITE );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_BAT_CUR ), GUI_WHITE );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_BAT_SOC ), GUI_WHITE );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_BAT_TTG ), GUI_WHITE );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_BAT_VOL ), GUI_TA_HCENTER );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_BAT_CUR ), GUI_TA_HCENTER );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_BAT_SOC ), GUI_TA_HCENTER );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_BAT_TTG ), GUI_TA_HCENTER );
            //дата - время
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_TIME ), GUI_TA_HCENTER );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_DATE ), GUI_TA_HCENTER );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_TIME ), &GUI_FontTahoma16b );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_DATE ), &GUI_FontTahoma16b );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_TIME ), GUI_WHITE );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_DATE ), GUI_WHITE );
            //состояние связи
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_LINK1 ), GUI_TA_HCENTER );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_LINK1 ), &GUI_FontTahoma16b );
            //состояние зарядного уст-ва
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_PB_CUR ), &GUI_FontTahoma16b );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_PB_CUR ), GUI_WHITE );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_PB_CUR ), GUI_TA_HCENTER );
            //состояние инверторов
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_TS1_PWR ), &GUI_FontTahoma16b );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_TS1_PRC ), &GUI_FontTahoma16b );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_TS3_PWR ), &GUI_FontTahoma16b );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_TS3_PRC ), &GUI_FontTahoma16b );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_TS1_PWR ), GUI_WHITE );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_TS1_PRC ), GUI_WHITE );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_TS3_PWR ), GUI_WHITE );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_TS3_PRC ), GUI_WHITE );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_TS1_PWR ), GUI_TA_HCENTER );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_TS1_PRC ), GUI_TA_HCENTER );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_TS3_PWR ), GUI_TA_HCENTER );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_TS3_PRC ), GUI_TA_HCENTER );
            //состояние контроллера MPPT
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_MPPT_VOL ), &GUI_FontTahoma16b );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_MPPT_CUR ), &GUI_FontTahoma16b );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_MPPT_VOL ), GUI_WHITE );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_MPPT_CUR ), GUI_WHITE );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_MPPT_VOL ), GUI_TA_HCENTER );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_MPPT_CUR ), GUI_TA_HCENTER );
            break;
        case WM_NOTIFY_PARENT:
            id_widg = WM_GetId( pMsg->hWinSrc ); //Id of widget
            ntf_code = pMsg->Data.v;             //Notification code
            //Вызов формы "монитор АКБ"
            if ( id_widg == ID_IMAGE_BAT && ntf_code == WM_NOTIFICATION_CLICKED )
                WinBatMon();
            //Вызов формы "контроллер заряда"
            if ( id_widg == ID_IMAGE_PB && ntf_code == WM_NOTIFICATION_CLICKED )
                WinCharge();
            //Вызов формы "контроль положения солнца"
            if ( id_widg == ID_IMAGE_SPA && ntf_code == WM_NOTIFICATION_CLICKED )
                WinSpa();
            //Вызов формы "управление солнечным трекером"
            if ( id_widg == ID_IMAGE_TRC && ntf_code == WM_NOTIFICATION_CLICKED )
                WinTrc();
            //Вызов формы "солнечный контроллер заряда"
            if ( id_widg == ID_IMAGE_MPPT && ntf_code == WM_NOTIFICATION_CLICKED )
                WinMppt();
            //Вызов формы "инвертора"
            if ( ( id_widg == ID_IMAGE_TS1 || id_widg == ID_IMAGE_TS3 ) && ntf_code == WM_NOTIFICATION_CLICKED )
                WinInv();
            //Вызов формы "контроль связи"
            if ( id_widg == ID_LINK1 && ntf_code == WM_NOTIFICATION_CLICKED )
                WinHmi();
            //Вызов формы "настройка параметров"
            if ( id_widg == ID_IMAGE_PAR && ntf_code == WM_NOTIFICATION_CLICKED )
                WinParam();
            //Вызов формы "настройка сетевого интерфейса"
            if ( id_widg == ID_IMAGE_ETH && ntf_code == WM_NOTIFICATION_CLICKED )
                WinEth();
            //Вызов формы "управление звуком и голосовым информатором"
            if ( id_widg == ID_IMAGE_SND && ntf_code == WM_NOTIFICATION_CLICKED )
                WinSnd();
            //Вызов формы "статистика"
            if ( id_widg == ID_IMAGE_GRAPH && ntf_code == WM_NOTIFICATION_CLICKED )
                MessageBox( MESSAGE_INFO, MessageGui( MSG_GUI_UNDER_DEV ) );
            //Вызов формы "установка часов/даты"
            if ( ( id_widg == ID_TIME || id_widg == ID_DATE ) && ntf_code == WM_NOTIFICATION_CLICKED )
                WinDtime();
            break;
        default:
            WM_DefaultProc( pMsg );
        break;
       }
 }

//*************************************************************************************************
// Функция обратного вызова окна, мнемосхема с АВР
//*************************************************************************************************
static void cb_Dialog2( WM_MESSAGE *pMsg ) {

    const void *pData;
    WM_HWIN hItem;
    int id_widg, ntf_code;
    U32 FileSize;

    switch ( pMsg->MsgId ) {
        case WM_INIT_DIALOG:
            WINDOW_SetBkColor( pMsg->hWin, GUI_BLACK );
            //BAT
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_BAT );
            pData = RamDiskFile( "bat_0.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //TS-1000-224
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_TS1 );
            pData = RamDiskFile( "ts10.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //TS-3000-224
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_TS3 );
            pData = RamDiskFile( "ts30.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //PB-1000-224
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_PB );
            pData = RamDiskFile( "charge0.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //MPPT
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_MPPT );
            pData = RamDiskFile( "mppt0.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //SPA
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_SPA );
            pData = RamDiskFile( "sun.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //трекер
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_TRC );
            pData = RamDiskFile( "trc0.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //генератор
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_ALT );
            pData = RamDiskFile( "alt.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //АВР
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_GEN );
            pData = RamDiskFile( "gen0.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //основная сеть
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_LEP );
            pData = RamDiskFile( "lep0.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //параметры
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_PAR );
            pData = RamDiskFile( "param.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_ETH );
            pData = RamDiskFile( "ethernet.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_SND );
            pData = RamDiskFile( "sound.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_GRAPH );
            pData = RamDiskFile( "graph.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //связи между объектами
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_GEN_LINK2 );
            pData = RamDiskFile( "gen_link20.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_LEP_LINK2 );
            pData = RamDiskFile( "lep_link20.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_ALT_LINK );
            pData = RamDiskFile( "alt_link.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_TS_LINK2 );
            pData = RamDiskFile( "ts_link2.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_BAT_LINK2 );
            pData = RamDiskFile( "bat_link2.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_PB_LINK2 );
            pData = RamDiskFile( "pb_link20.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            hItem = WM_GetDialogItem( pMsg->hWin, ID_IMAGE_MPPT_LINK2 );
            pData = RamDiskFile( "mppt_link2.bmp", (uint32_t *)&FileSize );
            IMAGE_SetBMP( hItem, pData, FileSize );
            //параметры АКБ
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BAT_VOL ), &GUI_FontTahoma16b );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BAT_CUR ), &GUI_FontTahoma16b );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BAT_SOC ), &GUI_FontTahoma16b );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_BAT_TTG ), &GUI_FontTahoma16b );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_BAT_VOL ), GUI_WHITE );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_BAT_CUR ), GUI_WHITE );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_BAT_SOC ), GUI_WHITE );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_BAT_TTG ), GUI_WHITE );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_BAT_VOL ), GUI_TA_HCENTER );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_BAT_CUR ), GUI_TA_HCENTER );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_BAT_SOC ), GUI_TA_HCENTER );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_BAT_TTG ), GUI_TA_HCENTER );
            //дата - время
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_TIME ), GUI_TA_HCENTER );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_DATE ), GUI_TA_HCENTER );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_TIME ), &GUI_FontTahoma16b );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_DATE ), &GUI_FontTahoma16b );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_TIME ), GUI_WHITE );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_DATE ), GUI_WHITE );
            //состояние связи
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_LINK1 ), GUI_TA_HCENTER );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_LINK1 ), &GUI_FontTahoma16b );
            //состояние зарядного уст-ва
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_PB_CUR ), &GUI_FontTahoma16b );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_PB_CUR ), GUI_WHITE );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_PB_CUR ), GUI_TA_HCENTER );
            //состояние инверторов
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_TS1_PWR ), &GUI_FontTahoma16b );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_TS1_PRC ), &GUI_FontTahoma16b );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_TS3_PWR ), &GUI_FontTahoma16b );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_TS3_PRC ), &GUI_FontTahoma16b );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_TS1_PWR ), GUI_WHITE );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_TS1_PRC ), GUI_WHITE );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_TS3_PWR ), GUI_WHITE );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_TS3_PRC ), GUI_WHITE );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_TS1_PWR ), GUI_TA_HCENTER );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_TS1_PRC ), GUI_TA_HCENTER );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_TS3_PWR ), GUI_TA_HCENTER );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_TS3_PRC ), GUI_TA_HCENTER );
            //состояние контроллера MPPT
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_MPPT_VOL ), &GUI_FontTahoma16b );
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_MPPT_CUR ), &GUI_FontTahoma16b );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_MPPT_VOL ), GUI_WHITE );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_MPPT_CUR ), GUI_WHITE );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_MPPT_VOL ), GUI_TA_HCENTER );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_MPPT_CUR ), GUI_TA_HCENTER );
            //источник питания нагрузки
            TEXT_SetFont( WM_GetDialogItem( pMsg->hWin, ID_ALT_STAT ), &GUI_FontTahoma16b );
            TEXT_SetTextColor( WM_GetDialogItem( pMsg->hWin, ID_ALT_STAT ), GUI_WHITE );
            TEXT_SetTextAlign( WM_GetDialogItem( pMsg->hWin, ID_ALT_STAT ), GUI_TA_HCENTER );
            TEXT_SetWrapMode( WM_GetDialogItem( pMsg->hWin, ID_ALT_STAT ), GUI_WRAPMODE_WORD );
            break;
        case WM_NOTIFY_PARENT:
            id_widg = WM_GetId( pMsg->hWinSrc );
            ntf_code = pMsg->Data.v;
            //Вызов формы "монитор АКБ"
            if ( id_widg == ID_IMAGE_BAT && ntf_code == WM_NOTIFICATION_CLICKED )
                WinBatMon();
            //Вызов формы "контроллер заряда"
            if ( id_widg == ID_IMAGE_PB && ntf_code == WM_NOTIFICATION_CLICKED )
                WinCharge();
            //Вызов формы "контроль положения солнца"
            if ( id_widg == ID_IMAGE_SPA && ntf_code == WM_NOTIFICATION_CLICKED )
                WinSpa();
            //Вызов формы "управление солнечным трекером"
            if ( id_widg == ID_IMAGE_TRC && ntf_code == WM_NOTIFICATION_CLICKED )
                WinTrc();
            //Вызов формы "солнечный контроллер заряда"
            if ( id_widg == ID_IMAGE_MPPT && ntf_code == WM_NOTIFICATION_CLICKED )
                WinMppt();
            //Вызов формы "инвертора"
            if ( ( id_widg == ID_IMAGE_TS1 || id_widg == ID_IMAGE_TS3 ) && ntf_code == WM_NOTIFICATION_CLICKED )
                WinInv();
            //Вызов формы "генератор"
            if ( id_widg == ID_IMAGE_GEN && ntf_code == WM_NOTIFICATION_CLICKED )
                WinGen();
            //Вызов формы "АВР"
            if ( id_widg == ID_IMAGE_ALT && ntf_code == WM_NOTIFICATION_CLICKED )
                WinAlt();
            //Вызов формы "контроль связи"
            if ( id_widg == ID_LINK1 && ntf_code == WM_NOTIFICATION_CLICKED )
                WinHmi();
            //Вызов формы "настройка параметров"
            if ( id_widg == ID_IMAGE_PAR && ntf_code == WM_NOTIFICATION_CLICKED )
                WinParam();
            //Вызов формы "настройка сетевого интерфейса"
            if ( id_widg == ID_IMAGE_ETH && ntf_code == WM_NOTIFICATION_CLICKED )
                WinEth();
            //Вызов формы "статистика"
            if ( id_widg == ID_IMAGE_GRAPH && ntf_code == WM_NOTIFICATION_CLICKED )
                MessageBox( MESSAGE_INFO, MessageGui( MSG_GUI_UNDER_DEV ) );
            //Вызов формы "управление звуком и голосовым информатором"
            if ( id_widg == ID_IMAGE_SND && ntf_code == WM_NOTIFICATION_CLICKED )
                WinSnd();
            //Вызов формы "установка часов/даты"
            if ( ( id_widg == ID_TIME || id_widg == ID_DATE ) && ntf_code == WM_NOTIFICATION_CLICKED )
                WinDtime();
            break;
        default:
            WM_DefaultProc( pMsg );
        break;
       }
 }

//*************************************************************************************************
// Функция отслеживает изменение состояния подключение блока АВР
// при обнаружении изменений происходит переключение вида мнемосхемы
// Вызов по таймеру технологического окна
//*************************************************************************************************
static void CheckAlt( void ) {

    WM_MESSAGE msg;
    
    //текущее состояние уст-ва
    alt_stat = ParamGetVal( ID_DEV_ALT, ALT_CONN ).uint8;
    if ( alt_prev != alt_stat ) {
        //произошло изменение статуса
        do {
            //закрываем все модальные окно, если они открыты и активны
            StackHwndPop( &stc_hwnd );
            if ( WM_IsVisible( stc_hwnd.hwnd ) && stc_hwnd.hwnd != NULL ) {
                msg.MsgId = WM_MESSAGE_MODAL_CLOSE;
                WM_SendMessage( stc_hwnd.hwnd, &msg );
               }
           } while ( stc_hwnd.hwnd != NULL );
        if ( !alt_prev && alt_stat ) {
            GUI_SetOrg( X1_POS, Y1_POS );
            WM_SetFocus( hwin2 );
           }
        if ( alt_prev && !alt_stat ) {
            GUI_SetOrg( X0_POS, Y0_POS );
            WM_SetFocus( hwin1 );
           }
        alt_prev = alt_stat;
        LCDBackLigth( LCD_BACK_LIGTH_ON );
       }
 }
 
//*************************************************************************************************
// Функция обратного вызова управляющего окна
//*************************************************************************************************
static void cb_Win( WM_MESSAGE *pMsg ) {

    switch ( pMsg->MsgId ) {
        case WM_TIMER:
            CheckAlt();
            WM_RestartTimer( pMsg->Data.v, TIME_UPD_MAIN_WINDOW );
            UpdateWidget( hwin1 );
            UpdateWidget( hwin2 );
            CheckStatusDev();
            break;
        case WM_MESSAGE_MODAL_OPEN:
            //сообщение от модального окна "окно открыто"
            StackHwndPush( pMsg->hWinSrc, pMsg->Data.v );
            break;
        case WM_MESSAGE_MODAL_END:
            //сообщение от модального окна "окно закрыто пользователем"
            StackHwndPop( &stc_hwnd );
            break;
       }
 }

//*************************************************************************************************
// Основная функция
//*************************************************************************************************
void TaskWindow( void *pvParameters ) {

    StackHwndInit();
    //начальное состояние АВР
    alt_stat = ParamGetVal( ID_DEV_ALT, ALT_CONN ).uint8;
    //создание окна для размещения таймера обновления информации
    UARTSendStr( MessageGui( MSG_GUI_INIT_WIN0 ) );
    hwin0 = WM_CreateWindow( 0, 0, 1, 1, WM_CF_HIDE, cb_Win, 0 );
    if ( hwin0 != NULL ) {
        UARTSendStr( MessageGui( MSG_GUI_OK ) );
        WM_CreateTimer( hwin0, 0, TIME_UPD_MAIN_WINDOW, 0 );
       }
    else UARTSendStr( MessageGui( MSG_GUI_ERROR ) );
    //создание окна 1 без блока АВР
    UARTSendStr( MessageGui( MSG_GUI_INIT_WIN1 ) );
    hwin1 = GUI_CreateDialogBox( DialogCreate1, GUI_COUNTOF( DialogCreate1 ), cb_Dialog1, 0, 0, 0 );
    if ( hwin1 != NULL )
        UARTSendStr( MessageGui( MSG_GUI_OK ) );
    else UARTSendStr( MessageGui( MSG_GUI_ERROR ) );
    //создание окна 2 с блоком АВР
    UARTSendStr( MessageGui( MSG_GUI_INIT_WIN2 ) );
    hwin2 = GUI_CreateDialogBox( DialogCreate2, GUI_COUNTOF( DialogCreate2 ), cb_Dialog2, 0, 0, 0 );
    if ( hwin2 != NULL )
        UARTSendStr( MessageGui( MSG_GUI_OK ) );
    else UARTSendStr( MessageGui( MSG_GUI_ERROR ) );
    //если инициализация окон не выполнена - приостановка задачи
    if ( hwin0 == NULL || hwin1 == NULL || hwin2 == NULL ) 
        osThreadSuspend( osThreadGetId() );
    //обновим начальное состояние для каждого окна
    UpdateWidget( hwin1 );
    UpdateWidget( hwin2 );
    CheckStatusDev(); //Проверка изменения состояния уст-в
    UARTSendStr( MessageGui( MSG_GUI_TASK_STARTUP ) );
    //инициализация модулей завершена, вывод приглашения в консоль
    UARTSendStr( Message( CONS_MSG_PROMPT ) );
    ReqConfig(); //разрешить запрос параметров настроек управляющего контроллера
    //начальное состояние - выбор мнемосхемы
    if ( !alt_stat ) {
        //без блока АВР
        GUI_SetOrg( X0_POS, Y0_POS );
        WM_SetFocus( hwin1 );
       }
    else {
        //с блоком АВР
        GUI_SetOrg( X1_POS, Y1_POS );
        WM_SetFocus( hwin2 );
       }
    for ( ;; ) {
        GUI_Exec();
        GUI_X_ExecIdle();
        #if ( OS_ROBIN_ENABLE == 0 )
        osThreadYield();
        #endif
       }
 }

//*************************************************************************************************
// Инициализация стека для хранения WM_HWIN
//*************************************************************************************************
static void StackHwndInit( void ) {

    memset( &stack_hwnd, 0x00, sizeof( stack_hwnd ) );
 }
 
//*************************************************************************************************
// Поместить в стек элемент WM_HWIN
// WM_HWIN hWin - указатель на окно
// int id_frame - номер окна
//*************************************************************************************************
static void StackHwndPush( WM_HWIN hWin, int id_frame ) {

    uint8_t ind;
    
    for ( ind = 0; ind < sizeof( stack_hwnd )/sizeof( stack_hwnd[0] ); ind++ ) {
        if ( stack_hwnd[ind].hwnd != NULL )
            continue;
        stack_hwnd[ind].hwnd = hWin;
        stack_hwnd[ind].id_frame = id_frame;
        break;
       }
 }

//*************************************************************************************************
// Извлечь из стека один элемент WM_HWIN
// return - указатель на переменную STACK_HWND
// если стек пустой, значения переменных STACK_HWND равно NULL и 0 соответственно
//*************************************************************************************************
static void StackHwndPop( STACK_HWND *result ) {

    uint8_t ind;
    result->hwnd = NULL;
    result->id_frame = 0;
    
    for ( ind = sizeof( stack_hwnd )/sizeof( stack_hwnd[0] ); ind; ind-- ) {
        if ( stack_hwnd[ind-1].hwnd != NULL ) {
            result->hwnd = stack_hwnd[ind-1].hwnd;
            result->id_frame = stack_hwnd[ind-1].id_frame;
            stack_hwnd[ind-1].hwnd = NULL;
            stack_hwnd[ind-1].id_frame = 0;
            break;
           }
       }
 }

//*************************************************************************************************
// Возвращает условный статус состояния АКБ (подключен/уровень заряда)
// Уровень заряда определяется по уровню SOC
// return = BAT_STATUS2
//*************************************************************************************************
static BAT_STATUS2 BatStat2( void ) {

    uint8_t value;
    
    if ( !ParamGetVal( ID_DEV_PORTS, BAT_CONNECT ).uint8 )
        return BAT_NOT_CONNECT; //АКБ не подключена
    value = (uint8_t)ParamGetVal( ID_DEV_BATMON, MON_SOC ).flt;
    if ( value >= 91 )
        return BAT_LEVEL_100;
    if ( value >= 80 && value < 95 )
        return BAT_LEVEL_90;
    if ( value >= 70 && value < 80 )
        return BAT_LEVEL_80;
    if ( value >= 60 && value < 70 )
        return BAT_LEVEL_70;
    if ( value >= 50 && value < 60 )
        return BAT_LEVEL_60;
    if ( value >= 30 && value < 50 )
        return BAT_LEVEL_50;
    if ( value >= 20 && value < 30 )
        return BAT_LEVEL_30;
    if ( value < 20 )
        return BAT_LEVEL_20;
    return BAT_NOT_CONNECT;
 }
 
