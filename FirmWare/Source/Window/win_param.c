
//*************************************************************************************************
//
// Форма настройки параметров управляющего контроллера
//
//*************************************************************************************************

#include "window.h"
#include "dialog.h"

//*************************************************************************************************
// Внешние переменные
//*************************************************************************************************
extern WM_HWIN hwin0;
extern CONFIG_LCD  cfg_lcd;

//*************************************************************************************************
// Размер окна
//*************************************************************************************************
#define WIN_SIZE_X                  580
#define WIN_SIZE_Y                  450

//*************************************************************************************************
// Определение идентификаторов объектов в форме
//*************************************************************************************************
#define ID_FRAMEWIN_0               (GUI_ID_USER + 13)

#define ID_BUTTON_CLS               (GUI_ID_USER + 0x10 + 1)
#define ID_BUTTON_APPLY             (GUI_ID_USER + 0x10 + 2)
#define ID_BUTTON_SAVE              (GUI_ID_USER + 0x10 + 3)
#define ID_MULTIPAGE_0              (GUI_ID_USER + 0x10 + 4)

//описания параметров
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
#define ID_NAME_22                  (GUI_ID_USER + 0x40 + 22)
#define ID_NAME_23                  (GUI_ID_USER + 0x40 + 23)
#define ID_NAME_24                  (GUI_ID_USER + 0x40 + 24)
#define ID_NAME_25                  (GUI_ID_USER + 0x40 + 25)
#define ID_NAME_26                  (GUI_ID_USER + 0x40 + 26)
#define ID_NAME_27                  (GUI_ID_USER + 0x40 + 27)
#define ID_NAME_28                  (GUI_ID_USER + 0x40 + 28)
#define ID_NAME_29                  (GUI_ID_USER + 0x40 + 29)
#define ID_NAME_30                  (GUI_ID_USER + 0x40 + 30)
#define ID_NAME_31                  (GUI_ID_USER + 0x40 + 31)
#define ID_NAME_32                  (GUI_ID_USER + 0x40 + 32)
#define ID_NAME_33                  (GUI_ID_USER + 0x40 + 33)
#define ID_NAME_34                  (GUI_ID_USER + 0x40 + 34)
#define ID_NAME_35                  (GUI_ID_USER + 0x40 + 35)
#define ID_NAME_36                  (GUI_ID_USER + 0x40 + 36)
#define ID_NAME_37                  (GUI_ID_USER + 0x40 + 37)
#define ID_NAME_38                  (GUI_ID_USER + 0x40 + 38)
#define ID_NAME_39                  (GUI_ID_USER + 0x40 + 39)
#define ID_NAME_40                  (GUI_ID_USER + 0x40 + 40)
#define ID_NAME_41                  (GUI_ID_USER + 0x40 + 41)
#define ID_NAME_42                  (GUI_ID_USER + 0x40 + 42)
#define ID_NAME_43                  (GUI_ID_USER + 0x40 + 43)
#define ID_NAME_44                  (GUI_ID_USER + 0x40 + 44)

//единицы измерения параметров
#define ID_UNIT_00                  (GUI_ID_USER + 0x400 + 0)
#define ID_UNIT_01                  (GUI_ID_USER + 0x400 + 1)
#define ID_UNIT_02                  (GUI_ID_USER + 0x400 + 2)
#define ID_UNIT_03                  (GUI_ID_USER + 0x400 + 3)
#define ID_UNIT_04                  (GUI_ID_USER + 0x400 + 4)
#define ID_UNIT_05                  (GUI_ID_USER + 0x400 + 5)
#define ID_UNIT_06                  (GUI_ID_USER + 0x400 + 6)
#define ID_UNIT_07                  (GUI_ID_USER + 0x400 + 7)
#define ID_UNIT_08                  (GUI_ID_USER + 0x400 + 8)
#define ID_UNIT_09                  (GUI_ID_USER + 0x400 + 9)
#define ID_UNIT_10                  (GUI_ID_USER + 0x400 + 10)
#define ID_UNIT_11                  (GUI_ID_USER + 0x400 + 11)
#define ID_UNIT_12                  (GUI_ID_USER + 0x400 + 12)
#define ID_UNIT_13                  (GUI_ID_USER + 0x400 + 13)
#define ID_UNIT_14                  (GUI_ID_USER + 0x400 + 14)

//основные параметры
#define ID_EDIT_SCR                 (GUI_ID_USER + 0x80 + 10)
#define ID_EDIT_JOB1                (GUI_ID_USER + 0x80 + 11)
#define ID_EDIT_JOB2                (GUI_ID_USER + 0x80 + 12)
#define ID_EDIT_MSYS                (GUI_ID_USER + 0x80 + 13)
#define ID_EDIT_LCHRG               (GUI_ID_USER + 0x80 + 14)
#define ID_EDIT_CRSTOP              (GUI_ID_USER + 0x80 + 15)
#define ID_EDIT_STINV               (GUI_ID_USER + 0x80 + 16)
#define ID_EDIT_SPINV               (GUI_ID_USER + 0x80 + 17)
#define ID_CHECKBOX_MLOG            (GUI_ID_USER + 0x80 + 18)
//кнопки редактирования параметров
#define ID_BUTT_SCR                 (ID_EDIT_SCR    - 0x80)  
#define ID_BUTT_JOB1                (ID_EDIT_JOB1   - 0x80) 
#define ID_BUTT_JOB2                (ID_EDIT_JOB2   - 0x80)  
#define ID_BUTT_MSYS                (ID_EDIT_MSYS   - 0x80)
#define ID_BUTT_CRSTOP              (ID_EDIT_CRSTOP - 0x80)
#define ID_BUTT_STINV               (ID_EDIT_STINV  - 0x80)
#define ID_BUTT_SPINV               (ID_EDIT_SPINV  - 0x80)

//параметры логирования данных
#define ID_EDIT_PB                  (GUI_ID_USER + 0x80 + 10)
#define ID_EDIT_MPPT                (GUI_ID_USER + 0x80 + 11)
#define ID_EDIT_BMON                (GUI_ID_USER + 0x80 + 12)
#define ID_EDIT_TS                  (GUI_ID_USER + 0x80 + 13)
#define ID_EDIT_TRC                 (GUI_ID_USER + 0x80 + 14)
#define ID_CHECKBOX_PV              (GUI_ID_USER + 0x80 + 15)
#define ID_CHECKBOX_PB              (GUI_ID_USER + 0x80 + 16)
#define ID_CHECKBOX_MPPT            (GUI_ID_USER + 0x80 + 17)
#define ID_CHECKBOX_TS              (GUI_ID_USER + 0x80 + 18)
#define ID_CHECKBOX_BMON            (GUI_ID_USER + 0x80 + 19)
#define ID_CHECKBOX_GEN             (GUI_ID_USER + 0x80 + 20)
#define ID_CHECKBOX_ALT             (GUI_ID_USER + 0x80 + 21)
#define ID_CHECKBOX_TRC             (GUI_ID_USER + 0x80 + 22)
//кнопки редактирования параметров
#define ID_BUTT_PB                  (ID_EDIT_PB   - 0x80 + 10)
#define ID_BUTT_MPPT                (ID_EDIT_MPPT - 0x80 + 11)
#define ID_BUTT_BMON                (ID_EDIT_BMON - 0x80 + 12)
#define ID_BUTT_TS                  (ID_EDIT_TS   - 0x80 + 13)
#define ID_BUTT_TRC                 (ID_EDIT_TRC  - 0x80 + 14)

//параметры генератора
#define ID_EDIT_DSTART              (GUI_ID_USER + 0x80 + 10)
#define ID_EDIT_DSTOP               (GUI_ID_USER + 0x80 + 11)
#define ID_EDIT_CSTART              (GUI_ID_USER + 0x80 + 12)
#define ID_EDIT_CHRUN               (GUI_ID_USER + 0x80 + 13)
#define ID_EDIT_BSTART              (GUI_ID_USER + 0x80 + 14)
#define ID_EDIT_TSTART              (GUI_ID_USER + 0x80 + 15)
#define ID_EDIT_TRUN                (GUI_ID_USER + 0x80 + 16)
#define ID_EDIT_TSLP                (GUI_ID_USER + 0x80 + 17)
#define ID_EDIT_TEST                (GUI_ID_USER + 0x80 + 18)
#define ID_EDIT_LDAY                (GUI_ID_USER + 0x80 + 19)
#define ID_CHECKBOX_AUTO            (GUI_ID_USER + 0x80 + 20)
//кнопки редактирования параметров
#define ID_BUTT_DSTART              (ID_EDIT_DSTART - 0x80)
#define ID_BUTT_DSTOP               (ID_EDIT_DSTOP  - 0x80)
#define ID_BUTT_CSTART              (ID_EDIT_CSTART - 0x80)
#define ID_BUTT_CHRUN               (ID_EDIT_CHRUN  - 0x80)
#define ID_BUTT_BSTART              (ID_EDIT_BSTART - 0x80)
#define ID_BUTT_TSTART              (ID_EDIT_TSTART - 0x80)
#define ID_BUTT_TRUN                (ID_EDIT_TRUN   - 0x80)
#define ID_BUTT_TSLP                (ID_EDIT_TSLP   - 0x80)
#define ID_BUTT_TEST                (ID_EDIT_TEST   - 0x80)

//параметры SPA
#define ID_EDIT_TZ                  (GUI_ID_USER + 0x80 + 10)
#define ID_EDIT_LAT                 (GUI_ID_USER + 0x80 + 11)
#define ID_EDIT_LNG                 (GUI_ID_USER + 0x80 + 12)
#define ID_EDIT_ELV                 (GUI_ID_USER + 0x80 + 13)
#define ID_EDIT_PRS                 (GUI_ID_USER + 0x80 + 14)
#define ID_EDIT_TEMP                (GUI_ID_USER + 0x80 + 15)
#define ID_EDIT_SLP                 (GUI_ID_USER + 0x80 + 16)
#define ID_EDIT_AZM                 (GUI_ID_USER + 0x80 + 17)
//кнопки редактирования параметров
#define ID_BUTT_TZ                  (ID_EDIT_TZ   - 0x80)
#define ID_BUTT_LAT                 (ID_EDIT_LAT  - 0x80)
#define ID_BUTT_LNG                 (ID_EDIT_LNG  - 0x80)
#define ID_BUTT_ELV                 (ID_EDIT_ELV  - 0x80)
#define ID_BUTT_PRS                 (ID_EDIT_PRS  - 0x80)
#define ID_BUTT_TEMP                (ID_EDIT_TEMP - 0x80)
#define ID_BUTT_SLP                 (ID_EDIT_SLP  - 0x80)
#define ID_BUTT_AZM                 (ID_EDIT_AZM  - 0x80)

//параметры тестирования
#define ID_EDIT_00                  (GUI_ID_USER + 0x80 + 10)
#define ID_EDIT_01                  (GUI_ID_USER + 0x80 + 11)
#define ID_EDIT_02                  (GUI_ID_USER + 0x80 + 12)
#define ID_EDIT_03                  (GUI_ID_USER + 0x80 + 13)
#define ID_EDIT_04                  (GUI_ID_USER + 0x80 + 14)
#define ID_EDIT_05                  (GUI_ID_USER + 0x80 + 15)
#define ID_EDIT_06                  (GUI_ID_USER + 0x80 + 16)
#define ID_EDIT_07                  (GUI_ID_USER + 0x80 + 17)
//кнопки редактирования параметров тестирования
#define ID_BUTT_00                  (ID_EDIT_00 - 0x80)
#define ID_BUTT_01                  (ID_EDIT_01 - 0x80)
#define ID_BUTT_02                  (ID_EDIT_02 - 0x80)
#define ID_BUTT_03                  (ID_EDIT_03 - 0x80)
#define ID_BUTT_04                  (ID_EDIT_04 - 0x80)
#define ID_BUTT_05                  (ID_EDIT_05 - 0x80)
#define ID_BUTT_06                  (ID_EDIT_06 - 0x80)
#define ID_BUTT_07                  (ID_EDIT_07 - 0x80)

//*************************************************************************************************
// Локальные константы
//*************************************************************************************************
#define PAR_FIELD_SIZE_Y            20                                       //высота поля параметров
#define PAR_NAME_POS1_X             TEXT_DISTANCE                            //отступ от левого края
#define PAR_NAME_POS2_X             260                                      //позиция Y вывода полей во второй колонке
#define DISTANCE_SIZE_Y             10                                       //расстояние между полями по Y

#define PAR_NAME_SIZE_X             390                                      //размер по X поля наименования параметра
#define PAR_NAME_POS_Y( numb )      ( numb * 15 + ( numb - 1 ) * DISTANCE_SIZE_Y )

#define LEN_EDIT_TSTART             19

#define PAR_VALUE_FN_SIZE_X         120                                     //размер по X поля параметра (имя файла)
#define PAR_VALUE_T19_SIZE_X        115                                     //размер по X поля параметра (текст 19 знаков)
#define PAR_VALUE_N1_SIZE_X         15                                      //размер по X поля параметра (число N)
#define PAR_VALUE_N2_SIZE_X         25                                      //размер по X поля параметра (число NN)
#define PAR_VALUE_N3_SIZE_X         30                                      //размер по X поля параметра (число NNN)
#define PAR_VALUE_N4_SIZE_X         40                                      //размер по X поля параметра (число NNNN)
#define PAR_VALUE_N5_SIZE_X         50                                      //размер по X поля параметра (число NNNNN)
#define PAR_VALUE_D_SIZE_X          75                                      //размер по X поля параметра (дата)
#define PAR_VALUE_FL_SIZE_X         80                                      //размер по X поля параметра (число float)
#define PAR_VALUE_LFN_SIZE_X        340                                     //размер по X поля параметра (длинное имя файла)
#define PAR_VALUE_POS1_X            PAR_NAME_POS1_X + PAR_NAME_SIZE_X       //позиция вывода первой колонки параметров
#define PAR_VALUE_POS2_X            PAR_NAME_POS2_X + PAR_NAME_SIZE_X       //позиция вывода второй колонки параметров
#define PAR_VALUE_POS3_X            PAR_NAME_POS1_X + PAR_NAME_SIZE_X       //позиция вывода первой колонки параметров
#define PAR_VALUE_POS5_X            40                                      //
#define PAR_UNIT_SIZE_X             40                                      //размер по X поля единиц измерения параметра

#define PAR_BUTTON_POS1_X           PAR_NAME_SIZE_X + PAR_VALUE_FN_SIZE_X + 20  //позиция вывода колонки кнопок редактирования
#define PAR_BUTTON_POS2_X           PAR_NAME_SIZE_X + PAR_VALUE_FN_SIZE_X + 20  //позиция вывода колонки кнопок редактирования
#define PAR_BUTTON_POS3_X           PAR_NAME_SIZE_X + PAR_VALUE_T19_SIZE_X + 20 //позиция вывода колонки кнопок редактирования
#define PAR_BUTTON_POS4_X           PAR_NAME_SIZE_X + PAR_VALUE_FL_SIZE_X + 20  //позиция вывода колонки кнопок редактирования
#define PAR_BUTTON_POS5_X           PAR_NAME_SIZE_X + PAR_VALUE_LFN_SIZE_X + 10 //позиция вывода колонки кнопок редактирования
#define PAR_UNIT_POS1_X             PAR_NAME_SIZE_X + PAR_VALUE_N5_SIZE_X  //позиция вывода колонки единиц измерения параметра
#define PAR_UNIT_POS2_X             PAR_NAME_SIZE_X + PAR_VALUE_N5_SIZE_X  //позиция вывода колонки единиц измерения параметра
#define PAR_UNIT_POS3_X             PAR_NAME_SIZE_X + PAR_VALUE_N5_SIZE_X + 20  //позиция вывода колонки единиц измерения параметра

#define MULTI_PAGE_SIZE_X           WIN_SIZE_X - 16
#define MULTI_PAGE_SIZE_Y           BUTTON_POS_Y( WIN_SIZE_Y ) - 10

#define WINDOW_PAGE_SIZE_X          MULTI_PAGE_SIZE_X
#define WINDOW_PAGE_SIZE_Y          MULTI_PAGE_SIZE_Y

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
static int32_t min, max;
static int org_x, org_y;
static CONFIG conf_save;
static WM_HWIN hMultiPage, hDialog1, hDialog2, hDialog3, hDialog4;

static const GUI_WIDGET_CREATE_INFO DialogCreate[] = {
    { FRAMEWIN_CreateIndirect, "Настройка параметров", ID_FRAMEWIN_0, WIN_POS_X( WIN_SIZE_X ), WIN_POS_Y( WIN_SIZE_Y ), WIN_SIZE_X, WIN_SIZE_Y },
    { MULTIPAGE_CreateIndirect, "", ID_MULTIPAGE_0, 3, 3, MULTI_PAGE_SIZE_X, MULTI_PAGE_SIZE_Y, 0, 0, 0 },
    { BUTTON_CreateIndirect, "Закрыть",   ID_BUTTON_CLS,   BUTTON_POS_X( 1, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "Применить", ID_BUTTON_APPLY, BUTTON_POS_X( 2, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y },
    { BUTTON_CreateIndirect, "Сохранить", ID_BUTTON_SAVE,  BUTTON_POS_X( 3, WIN_SIZE_X ), BUTTON_POS_Y( WIN_SIZE_Y ), BUTTON_SIZE_X, BUTTON_SIZE_Y }
 };

// Закладка 1 - общие параметры
static const GUI_WIDGET_CREATE_INFO DialogPage1[] = {
    { WINDOW_CreateIndirect, "", 0, 0, 0, WINDOW_PAGE_SIZE_X, WINDOW_PAGE_SIZE_Y },
    { TEXT_CreateIndirect, "Имя файла экрана",                                          ID_NAME_00, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 1 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Имя файла заданий (main)",                                  ID_NAME_01, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 2 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Имя файла заданий (test)",                                  ID_NAME_02, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 3 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Режим работы системы",                                      ID_NAME_03, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 4 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Дата подзарядки от основной сети",                          ID_NAME_04, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 5 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Ток выключения зарядки от PB-1000-224",                     ID_NAME_05, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 6 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Задержка вкл инверторов при отключении основной сети",      ID_NAME_06, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 7 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Задержка выкл инверторов при восстановлении основной сети", ID_NAME_07, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 8 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Режим логирования файлов [..\\Файл]/[..\\YYYYMM\\файл]",    ID_NAME_08, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 9 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    //
    { EDIT_CreateIndirect, NULL, ID_EDIT_SCR,    PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 1 ), PAR_VALUE_FN_SIZE_X, PAR_FIELD_SIZE_Y, 0, sizeof( config.scr_file ), 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_JOB1,   PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 2 ), PAR_VALUE_FN_SIZE_X, PAR_FIELD_SIZE_Y, 0, sizeof( config.job_file ), 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_JOB2,   PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 3 ), PAR_VALUE_FN_SIZE_X, PAR_FIELD_SIZE_Y, 0, sizeof( config.job_test ), 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_MSYS,   PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 4 ), PAR_VALUE_N1_SIZE_X, PAR_FIELD_SIZE_Y, 0, 1, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_LCHRG,  PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 5 ), PAR_VALUE_D_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 10, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_CRSTOP, PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 6 ), PAR_VALUE_N2_SIZE_X, PAR_FIELD_SIZE_Y, 0, 2, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_STINV,  PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 7 ), PAR_VALUE_N3_SIZE_X, PAR_FIELD_SIZE_Y, 0, 3, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_SPINV,  PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 8 ), PAR_VALUE_N3_SIZE_X, PAR_FIELD_SIZE_Y, 0, 3, 0 },
    { CHECKBOX_CreateIndirect, "", ID_CHECKBOX_MLOG, PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 9 ), 20, PAR_FIELD_SIZE_Y, 0, 0, 0 },
    //
    { BUTTON_CreateIndirect, "...", ID_BUTT_SCR,    PAR_BUTTON_POS1_X, PAR_NAME_POS_Y( 1 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_JOB1,   PAR_BUTTON_POS1_X, PAR_NAME_POS_Y( 2 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_JOB2,   PAR_BUTTON_POS1_X, PAR_NAME_POS_Y( 3 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_MSYS,   PAR_BUTTON_POS1_X, PAR_NAME_POS_Y( 4 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_CRSTOP, PAR_BUTTON_POS1_X, PAR_NAME_POS_Y( 6 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_STINV,  PAR_BUTTON_POS1_X, PAR_NAME_POS_Y( 7 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_SPINV,  PAR_BUTTON_POS1_X, PAR_NAME_POS_Y( 8 ), BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    //
    { TEXT_CreateIndirect, "А",   ID_UNIT_00, PAR_UNIT_POS1_X, PAR_NAME_POS_Y( 6 ), PAR_UNIT_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "сек", ID_UNIT_01, PAR_UNIT_POS1_X, PAR_NAME_POS_Y( 7 ), PAR_UNIT_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "сек", ID_UNIT_02, PAR_UNIT_POS1_X, PAR_NAME_POS_Y( 8 ), PAR_UNIT_SIZE_X, PAR_FIELD_SIZE_Y }
 };
 
// Закладка 2 - логирование
static const GUI_WIDGET_CREATE_INFO DialogPage2[] = {
    { WINDOW_CreateIndirect, "", 0, 0, 0, WINDOW_PAGE_SIZE_X, WINDOW_PAGE_SIZE_Y },
    { TEXT_CreateIndirect, "Период записи данных PB-1000-224",       ID_NAME_10, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 1 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Период записи данных MPPT",              ID_NAME_11, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 2 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Период записи данных монитора АКБ",      ID_NAME_12, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 3 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Период записи данных инверторов",        ID_NAME_13, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 4 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Период записи данных трекера",           ID_NAME_14, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 5 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Команды управление солнечными панелями", ID_NAME_15, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 6 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Команды зарядного уст-ва PB-1000-224",   ID_NAME_16, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 7 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Команды/данные контроллера заряда MPPT", ID_NAME_17, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 8 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Команды/данные инверторов",              ID_NAME_18, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 9 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Данных монитора АКБ",                    ID_NAME_19, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 10 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Команды генератора",                     ID_NAME_20, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 11 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Команды блока АВР",                      ID_NAME_21, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 12 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Команды трекера",                        ID_NAME_22, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 13 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    //
    { EDIT_CreateIndirect, NULL, ID_EDIT_PB,   PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 1 ), PAR_VALUE_N3_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 3, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_MPPT, PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 2 ), PAR_VALUE_N3_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 3, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_BMON, PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 3 ), PAR_VALUE_N3_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 3, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_TS,   PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 4 ), PAR_VALUE_N3_SIZE_X, PAR_FIELD_SIZE_Y, 0, 3, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_TRC,  PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 5 ), PAR_VALUE_N3_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 3, 0 },
    { CHECKBOX_CreateIndirect, NULL, ID_CHECKBOX_PV,   PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 6 ), 20, PAR_FIELD_SIZE_Y, 0, 0, 0 },
    { CHECKBOX_CreateIndirect, NULL, ID_CHECKBOX_PB,   PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 7 ), 20, PAR_FIELD_SIZE_Y, 0, 0, 0 },
    { CHECKBOX_CreateIndirect, NULL, ID_CHECKBOX_MPPT, PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 8 ), 20, PAR_FIELD_SIZE_Y, 0, 0, 0 },
    { CHECKBOX_CreateIndirect, NULL, ID_CHECKBOX_TS,   PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 9 ), 20, PAR_FIELD_SIZE_Y, 0, 0, 0 },
    { CHECKBOX_CreateIndirect, NULL, ID_CHECKBOX_BMON, PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 10 ), 20, PAR_FIELD_SIZE_Y, 0, 0, 0 },
    { CHECKBOX_CreateIndirect, NULL, ID_CHECKBOX_GEN,  PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 11 ), 20, PAR_FIELD_SIZE_Y, 0, 0, 0 },
    { CHECKBOX_CreateIndirect, NULL, ID_CHECKBOX_ALT,  PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 12 ), 20, PAR_FIELD_SIZE_Y, 0, 0, 0 },
    { CHECKBOX_CreateIndirect, NULL, ID_CHECKBOX_TRC,  PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 13 ), 20, PAR_FIELD_SIZE_Y, 0, 0, 0 },
    //
    { BUTTON_CreateIndirect, "...", ID_BUTT_PB,   PAR_BUTTON_POS2_X, PAR_NAME_POS_Y( 1 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_MPPT, PAR_BUTTON_POS2_X, PAR_NAME_POS_Y( 2 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_BMON, PAR_BUTTON_POS2_X, PAR_NAME_POS_Y( 3 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_TS,   PAR_BUTTON_POS2_X, PAR_NAME_POS_Y( 4 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_TRC,  PAR_BUTTON_POS2_X, PAR_NAME_POS_Y( 5 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    //
    { TEXT_CreateIndirect, "сек", ID_UNIT_03, PAR_UNIT_POS2_X, PAR_NAME_POS_Y( 1 ), PAR_UNIT_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "сек", ID_UNIT_04, PAR_UNIT_POS2_X, PAR_NAME_POS_Y( 2 ), PAR_UNIT_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "сек", ID_UNIT_05, PAR_UNIT_POS2_X, PAR_NAME_POS_Y( 3 ), PAR_UNIT_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "сек", ID_UNIT_06, PAR_UNIT_POS2_X, PAR_NAME_POS_Y( 4 ), PAR_UNIT_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "сек", ID_UNIT_07, PAR_UNIT_POS2_X, PAR_NAME_POS_Y( 5 ), PAR_UNIT_SIZE_X, PAR_FIELD_SIZE_Y },
 };
 
// Закладка 3 - генератор
static const GUI_WIDGET_CREATE_INFO DialogPage3[] = {
    { WINDOW_CreateIndirect, "", 0, 0, 0, WINDOW_PAGE_SIZE_X, WINDOW_PAGE_SIZE_Y },
    { TEXT_CreateIndirect, "Задержка запуска после отключения основной сети",        ID_NAME_23, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 1 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Задержка выключения после восстановления основной сети", ID_NAME_24, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 2 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Кол-во попыток запуска генератора",                      ID_NAME_25, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 3 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Ожидание сигнала запуска генератора",                    ID_NAME_26, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 4 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Пауза между запусками",                                  ID_NAME_27, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 5 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Продолжительность запуска для каждой попытки",           ID_NAME_28, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 6 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Максимальная продолжительность работы генератора",       ID_NAME_29, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 7 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Продолжительность паузы между длительными работами",     ID_NAME_30, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 8 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Продолжительность тестирования генератора",              ID_NAME_31, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 9 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Дата последнего включения генератора",                   ID_NAME_32, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 10 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Автоматический режим запуска при отключении сети",       ID_NAME_33, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 11 ), PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    //
    { EDIT_CreateIndirect, NULL, ID_EDIT_DSTART, PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 1 ), PAR_VALUE_N4_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 4, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_DSTOP,  PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 2 ), PAR_VALUE_N4_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 4, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_CSTART, PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 3 ), PAR_VALUE_N1_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 1, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_CHRUN,  PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 4 ), PAR_VALUE_N2_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 2, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_BSTART, PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 5 ), PAR_VALUE_N2_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 2, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_TSTART, PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 6 ), PAR_VALUE_T19_SIZE_X, PAR_FIELD_SIZE_Y, 0, LEN_EDIT_TSTART, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_TRUN,   PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 7 ), PAR_VALUE_N5_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 5, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_TSLP,   PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 8 ), PAR_VALUE_N4_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 4, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_TEST,   PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 9 ), PAR_VALUE_N3_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 3, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_LDAY,   PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 10 ), PAR_VALUE_D_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 10, 0 },
    { CHECKBOX_CreateIndirect, "", ID_CHECKBOX_AUTO, PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 11 ), 20, PAR_FIELD_SIZE_Y, 0, 0, 0 },
    //
    { BUTTON_CreateIndirect, "...", ID_BUTT_DSTART, PAR_BUTTON_POS3_X, PAR_NAME_POS_Y( 1 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_DSTOP,  PAR_BUTTON_POS3_X, PAR_NAME_POS_Y( 2 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_CSTART, PAR_BUTTON_POS3_X, PAR_NAME_POS_Y( 3 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_CHRUN,  PAR_BUTTON_POS3_X, PAR_NAME_POS_Y( 4 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_BSTART, PAR_BUTTON_POS3_X, PAR_NAME_POS_Y( 5 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_TSTART, PAR_BUTTON_POS3_X, PAR_NAME_POS_Y( 6 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_TRUN,   PAR_BUTTON_POS3_X, PAR_NAME_POS_Y( 7 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_TSLP,   PAR_BUTTON_POS3_X, PAR_NAME_POS_Y( 8 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_TEST,   PAR_BUTTON_POS3_X, PAR_NAME_POS_Y( 9 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    //
    { TEXT_CreateIndirect, "сек", ID_UNIT_08, PAR_UNIT_POS3_X, PAR_NAME_POS_Y( 1 ), PAR_UNIT_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "сек", ID_UNIT_09, PAR_UNIT_POS3_X, PAR_NAME_POS_Y( 2 ), PAR_UNIT_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "сек", ID_UNIT_10, PAR_UNIT_POS3_X, PAR_NAME_POS_Y( 4 ), PAR_UNIT_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "сек", ID_UNIT_11, PAR_UNIT_POS3_X, PAR_NAME_POS_Y( 5 ), PAR_UNIT_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "сек", ID_UNIT_12, PAR_UNIT_POS3_X, PAR_NAME_POS_Y( 7 ), PAR_UNIT_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "сек", ID_UNIT_13, PAR_UNIT_POS3_X, PAR_NAME_POS_Y( 8 ), PAR_UNIT_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "сек", ID_UNIT_14, PAR_UNIT_POS3_X, PAR_NAME_POS_Y( 9 ), PAR_UNIT_SIZE_X, PAR_FIELD_SIZE_Y }
 };

// Закладка 4 - SPA
static const GUI_WIDGET_CREATE_INFO DialogPage4[] = {
    { WINDOW_CreateIndirect, "", 0, 0, 0, WINDOW_PAGE_SIZE_X, WINDOW_PAGE_SIZE_Y },
    { TEXT_CreateIndirect, "Часовой пояс",                      ID_NAME_36, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 1 ),  PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Широта места",                      ID_NAME_37, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 2 ),  PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Долгота места",                     ID_NAME_38, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 3 ),  PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Высота места",                      ID_NAME_39, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 4 ),  PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Среднегодовое местное давление",    ID_NAME_40, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 5 ),  PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Среднегодовая местная температура", ID_NAME_41, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 6 ),  PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Наклон поверхности",                ID_NAME_42, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 7 ),  PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },
    { TEXT_CreateIndirect, "Вращение поверхности азимута",      ID_NAME_43, PAR_NAME_POS1_X, PAR_NAME_POS_Y( 8 ),  PAR_NAME_SIZE_X, PAR_FIELD_SIZE_Y },

    { EDIT_CreateIndirect, NULL, ID_EDIT_TZ,   PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 1 ), PAR_VALUE_N2_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 2, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_LAT,  PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 2 ), PAR_VALUE_FL_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 9, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_LNG,  PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 3 ), PAR_VALUE_FL_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 9, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_ELV,  PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 4 ), PAR_VALUE_FL_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 5, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_PRS,  PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 5 ), PAR_VALUE_FL_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 3, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_TEMP, PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 6 ), PAR_VALUE_FL_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 5, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_SLP,  PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 7 ), PAR_VALUE_FL_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 5, 0 },
    { EDIT_CreateIndirect, NULL, ID_EDIT_AZM,  PAR_VALUE_POS1_X, PAR_NAME_POS_Y( 8 ), PAR_VALUE_FL_SIZE_X,  PAR_FIELD_SIZE_Y, 0, 5, 0 },
    //
    { BUTTON_CreateIndirect, "...", ID_BUTT_TZ,   PAR_BUTTON_POS4_X, PAR_NAME_POS_Y( 1 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_LAT,  PAR_BUTTON_POS4_X, PAR_NAME_POS_Y( 2 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_LNG,  PAR_BUTTON_POS4_X, PAR_NAME_POS_Y( 3 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_ELV,  PAR_BUTTON_POS4_X, PAR_NAME_POS_Y( 4 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_PRS,  PAR_BUTTON_POS4_X, PAR_NAME_POS_Y( 5 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_TEMP, PAR_BUTTON_POS4_X, PAR_NAME_POS_Y( 6 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_SLP,  PAR_BUTTON_POS4_X, PAR_NAME_POS_Y( 7 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y },
    { BUTTON_CreateIndirect, "...", ID_BUTT_AZM,  PAR_BUTTON_POS4_X, PAR_NAME_POS_Y( 8 ),  BUTT_EDT_SIZE_X, BUTT_EDT_SIZE_Y }
 };

//*************************************************************************************************
// Локальные функции
//*************************************************************************************************
static void ApplyConfig( void );
static void cb_Dialog1( WM_MESSAGE *pMsg );
static void cb_Dialog2( WM_MESSAGE *pMsg );
static void cb_Dialog3( WM_MESSAGE *pMsg );
static void cb_Dialog4( WM_MESSAGE *pMsg );

//*************************************************************************************************
// Обновление данных в 1 закладке
//*************************************************************************************************
void UpdateFields1( WM_HWIN hwnd ) {

    char value[5];
    
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_SCR ), config.scr_file );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_JOB1 ), config.job_file );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_JOB2 ), config.job_test );
    sprintf( value, "%u", ParamGetVal( ID_CONFIG, CFG_MODE_SYS ).uint8 );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_MSYS ), value );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_LCHRG ), ParamGetForm( ID_CONFIG, CFG_LAST_CHARGE, PARAM_VALUE ) );
    WM_DisableWindow( WM_GetDialogItem( hwnd, ID_EDIT_LCHRG ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_CRSTOP ), ParamGetForm( ID_CONFIG, CFG_PB_CURRENT_STOP, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_STINV ), ParamGetForm( ID_CONFIG, CFG_DELAY_START_INV, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_SPINV ), ParamGetForm( ID_CONFIG, CFG_DELAY_STOP_INV, PARAM_VALUE ) );
    CHECKBOX_SetState( WM_GetDialogItem( hwnd, ID_CHECKBOX_MLOG ), ParamGetVal( ID_CONFIG, CFG_MODE_LOGGING ).uint8 );
 }

//*************************************************************************************************
// Обновление данных во 2 закладке (логирование данных)
//*************************************************************************************************
void UpdateFields2( WM_HWIN hwnd ) {

    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_PB ), ParamGetForm( ID_CONFIG, CFG_DATLOG_UPD_CHARGE, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_MPPT ), ParamGetForm( ID_CONFIG, CFG_DATLOG_UPD_MPPT, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_BMON ), ParamGetForm( ID_CONFIG, CFG_DATLOG_UPD_BMON, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_TS ), ParamGetForm( ID_CONFIG, CFG_DATLOG_UPD_INV, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_TRC ), ParamGetForm( ID_CONFIG, CFG_DATLOG_UPD_TRC, PARAM_VALUE ) );
    CHECKBOX_SetState( WM_GetDialogItem( hwnd, ID_CHECKBOX_PV ), ParamGetVal( ID_CONFIG, CFG_LOG_ENABLE_PV ).uint8 );
    CHECKBOX_SetState( WM_GetDialogItem( hwnd, ID_CHECKBOX_PB ), ParamGetVal( ID_CONFIG, CFG_LOG_ENABLE_CHARGE ).uint8 );
    CHECKBOX_SetState( WM_GetDialogItem( hwnd, ID_CHECKBOX_MPPT ), ParamGetVal( ID_CONFIG, CFG_LOG_ENABLE_MPPT ).uint8 );
    CHECKBOX_SetState( WM_GetDialogItem( hwnd, ID_CHECKBOX_TS ), ParamGetVal( ID_CONFIG, CFG_LOG_ENABLE_INV ).uint8 );
    CHECKBOX_SetState( WM_GetDialogItem( hwnd, ID_CHECKBOX_BMON ), ParamGetVal( ID_CONFIG, CFG_LOG_ENABLE_BMON ).uint8 );
    CHECKBOX_SetState( WM_GetDialogItem( hwnd, ID_CHECKBOX_GEN ), ParamGetVal( ID_CONFIG, CFG_LOG_ENABLE_GEN ).uint8 );
    CHECKBOX_SetState( WM_GetDialogItem( hwnd, ID_CHECKBOX_ALT ), ParamGetVal( ID_CONFIG, CFG_LOG_ENABLE_ALT ).uint8 );
    CHECKBOX_SetState( WM_GetDialogItem( hwnd, ID_CHECKBOX_TRC ), ParamGetVal( ID_CONFIG, CFG_LOG_ENABLE_TRC ).uint8 );
 }
 
//*************************************************************************************************
// Обновление данных в 3 закладке (генератор)
//*************************************************************************************************
void UpdateFields3( WM_HWIN hwnd ) {

    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_DSTART ), ParamGetForm( ID_CONFIG, CFG_GEN_DELAY_START, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_DSTOP ), ParamGetForm( ID_CONFIG, CFG_GEN_DELAY_STOP, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_CSTART ), ParamGetForm( ID_CONFIG, CFG_GEN_CNT_START, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_CHRUN ), ParamGetForm( ID_CONFIG, CFG_GEN_DELAY_CHK_RUN, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_BSTART ), ParamGetForm( ID_CONFIG, CFG_GEN_BEFORE_START, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_TSTART ), ParamGetForm( ID_CONFIG, CFG_GEN_TIME_START, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_TRUN ), ParamGetForm( ID_CONFIG, CFG_GEN_TIME_RUN, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_TSLP ), ParamGetForm( ID_CONFIG, CFG_GEN_TIME_SLEEP, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_TEST ), ParamGetForm( ID_CONFIG, CFG_GEN_TIME_TEST, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_LDAY ), ParamGetForm( ID_CONFIG, CFG_GEN_LAST_RUN, PARAM_VALUE ) );
    WM_DisableWindow( WM_GetDialogItem( hwnd, ID_EDIT_LDAY ) );
    CHECKBOX_SetState( WM_GetDialogItem( hwnd, ID_CHECKBOX_AUTO ), ParamGetVal( ID_CONFIG, CFG_GEN_AUTO_MODE ).uint8 );
 }

//*************************************************************************************************
// Обновление данных в 4 закладке (SPA)
//*************************************************************************************************
void UpdateFields4( WM_HWIN hwnd ) {

    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_TZ ), ParamGetForm( ID_CONFIG, CFG_SPA_TIMEZONE, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_LAT ), ParamGetForm( ID_CONFIG, CFG_SPA_LATITUDE, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_LNG ), ParamGetForm( ID_CONFIG, CFG_SPA_LONGITUDE, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_ELV ), ParamGetForm( ID_CONFIG, CFG_SPA_ELEVATION, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_PRS ), ParamGetForm( ID_CONFIG, CFG_SPA_PRESSURE, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_TEMP ), ParamGetForm( ID_CONFIG, CFG_SPA_TEMPERATURE, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_SLP ), ParamGetForm( ID_CONFIG, CFG_SPA_SLOPE, PARAM_VALUE ) );
    EDIT_SetText( WM_GetDialogItem( hwnd, ID_EDIT_AZM ), ParamGetForm( ID_CONFIG, CFG_SPA_AZM_ROTATION, PARAM_VALUE ) );
 }

//*************************************************************************************************
// Функция обратного вызова, обработка сообщений
//*************************************************************************************************
static void cb_Dialog( WM_MESSAGE * pMsg ) {

    uint8_t cmd;
    WM_MESSAGE msg;
    int id_widg, ntf_code;

    switch ( pMsg->MsgId ) {
        case WM_INIT_DIALOG:
            //сообщим указатель (HWND) модального окна, окну из которого оно было вызвано
            msg.Data.v = DialogCreate[0].Id;
            msg.MsgId = WM_MESSAGE_MODAL_OPEN;              //тип сообщения
            msg.hWinSrc = WM_GetClientWindow( pMsg->hWin ); //указатель (HWND) модального окна
            WM_SendMessage( hwin0, &msg );
            WM_MakeModal( pMsg->hWin );
            hMultiPage = WM_GetDialogItem( pMsg->hWin, ID_MULTIPAGE_0 );
            WM_SetWindowPos( pMsg->hWin, org_x + DialogCreate[0].x0, org_y + DialogCreate[0].y0, DialogCreate[0].xSize, DialogCreate[0].ySize );
            //закладка общих параметров
            hDialog1 = GUI_CreateDialogBox( DialogPage1, GUI_COUNTOF( DialogPage1 ), &cb_Dialog1, WM_UNATTACHED, 0, 0 );
            MULTIPAGE_AddPage( hMultiPage, hDialog1, "Основные" );
            //закладка параметров логирования
            hDialog2 = GUI_CreateDialogBox( DialogPage2, GUI_COUNTOF( DialogPage2 ), &cb_Dialog2, WM_UNATTACHED, 0, 0 );
            MULTIPAGE_AddPage( hMultiPage, hDialog2, "Логирование данных" );
            //закладка параметров генератора
            hDialog3 = GUI_CreateDialogBox( DialogPage3, GUI_COUNTOF( DialogPage3 ), &cb_Dialog3, WM_UNATTACHED, 0, 0 );
            MULTIPAGE_AddPage( hMultiPage, hDialog3, "Генератор" );
            //закладка параметров SPA
            hDialog4 = GUI_CreateDialogBox( DialogPage4, GUI_COUNTOF( DialogPage4 ), &cb_Dialog4, WM_UNATTACHED, 0, 0 );
            MULTIPAGE_AddPage( hMultiPage, hDialog4, "SPA (Solar position algorithm)" );
            MULTIPAGE_SelectPage( hMultiPage, 0 );
            //обновим данные во всех полях закладок
            UpdateFields1( hDialog1 );
            UpdateFields2( hDialog2 );
            UpdateFields3( hDialog3 );
            UpdateFields4( hDialog4 );
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
            if ( id_widg == ID_BUTTON_APPLY && ntf_code == WM_NOTIFICATION_CLICKED )
                ApplyConfig(); //применение значений параметров
            if ( id_widg == ID_BUTTON_SAVE && ntf_code == WM_NOTIFICATION_CLICKED ) {
                //команда сохранения значений параметров в EEPROM
                if ( MessageBox( MESSAGE_QUESTION, MessageGui( MSG_GUI_EEPROM ) ) == MB_YES ) {
                    cmd = CAN_CONFIG_SAVE;
                    SendCommand( ID_CONFIG, &cmd, sizeof( cmd ) );
                   }
                //пользователь закрыл модальное окно
                msg.Data.v = DialogCreate[0].Id;
                msg.MsgId = WM_MESSAGE_MODAL_END;
                msg.hWinSrc = WM_GetClientWindow( pMsg->hWin );
                //сообщение окну из которого было вызвано модальное окно
                WM_SendMessage( hwin0, &msg );
                GUI_EndDialog( pMsg->hWin, 1 ); //закрываем форму
               }
            break;
        case WM_MESSAGE_MODAL_CLOSE:
            //сообщение от основного окна на принудительное закрытие модального окна
            GUI_EndDialog( pMsg->hWin, 0 );
            break;
        default:
            WM_DefaultProc( pMsg );
            break;
       }
 }

//*************************************************************************************************
// Функция обратного вызова, обработка сообщений, основные настройки
//*************************************************************************************************
static void cb_Dialog1( WM_MESSAGE * pMsg ) {

    char data[80], *result;
    int id_widg, ntf_code;

    switch ( pMsg->MsgId ) {
        case WM_INIT_DIALOG:
            break;
        case WM_NOTIFY_PARENT:
            id_widg = WM_GetId( pMsg-> hWinSrc );
            ntf_code = pMsg->Data.v;
            //Имя файла экрана
            if ( id_widg == ID_BUTT_SCR && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_SCR ), data, sizeof( data ) );
                result = WinKeyBrd( data, sizeof( config.scr_file ) );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_SCR ), result );
               }
            //Имя файла заданий (main)
            if ( id_widg == ID_BUTT_JOB1 && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_JOB1 ), data, sizeof( data ) );
                result = WinKeyBrd( data, sizeof( config.job_file ) );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_JOB1 ), result );
               }
            //Имя файла заданий (test)
            if ( id_widg == ID_BUTT_JOB2 && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_JOB2 ), data, sizeof( data ) );
                result = WinKeyBrd( data, sizeof( config.job_test ) );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_JOB2 ), result );
               }
            //Режим работы системы
            if ( id_widg == ID_BUTT_MSYS && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MSYS ), data, sizeof( data ) );
                ConfigLimit( CFG_MODE_SYS, &min, &max );
                result = WinKeyPad( data, 1, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MSYS ), result );
               }
            //Ток выключения зарядки от PB-1000-224
            if ( id_widg == ID_BUTT_CRSTOP && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_CRSTOP ), data, sizeof( data ) );
                ConfigLimit( CFG_PB_CURRENT_STOP, &min, &max );
                result = WinKeyPad( data, 2, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_CRSTOP ), result );
               }
            //Задержка вкл инверторов при отключении основной сети
            if ( id_widg == ID_BUTT_STINV && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_STINV ), data, sizeof( data ) );
                ConfigLimit( CFG_DELAY_START_INV, &min, &max );
                result = WinKeyPad( data, 3, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_STINV ), result );
               }
            //Задержка выкл инверторов при восстановлении основной сети
            if ( id_widg == ID_BUTT_SPINV && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_SPINV ), data, sizeof( data ) );
                ConfigLimit( CFG_DELAY_STOP_INV, &min, &max );
                result = WinKeyPad( data, 3, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_SPINV ), result );
               }
            break;
        default:
            WM_DefaultProc( pMsg );
            break;
       }
 }

//*************************************************************************************************
// Функция обратного вызова, обработка сообщений, параметры логирования
//*************************************************************************************************
static void cb_Dialog2( WM_MESSAGE * pMsg ) {

    char data[80], *result;
    int id_widg, ntf_code;

    switch ( pMsg->MsgId ) {
        case WM_INIT_DIALOG:
            break;
        case WM_NOTIFY_PARENT:
            id_widg = WM_GetId( pMsg-> hWinSrc );
            ntf_code = pMsg->Data.v;
            //Период записи данных PB-1000-224
            if ( id_widg == ID_BUTT_PB && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_PB ), data, sizeof( data ) );
                ConfigLimit( CFG_DATLOG_UPD_CHARGE, &min, &max );
                result = WinKeyPad( data, 3, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_PB ), result );
               }
            //Период записи данных MPPT
            if ( id_widg == ID_BUTT_MPPT && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MPPT ), data, sizeof( data ) );
                ConfigLimit( CFG_DATLOG_UPD_MPPT, &min, &max );
                result = WinKeyPad( data, 3, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_MPPT ), result );
               }
            //Период записи данных монитора АКБ
            if ( id_widg == ID_BUTT_BMON && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_BMON ), data, sizeof( data ) );
                ConfigLimit( CFG_DATLOG_UPD_BMON, &min, &max );
                result = WinKeyPad( data, 3, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_BMON ), result );
               }
            //Период записи данных инверторов
            if ( id_widg == ID_BUTT_TS && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TS ), data, sizeof( data ) );
                ConfigLimit( CFG_DATLOG_UPD_INV, &min, &max );
                result = WinKeyPad( data, 3, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TS ), result );
               }
            //Период записи данных трекера
            if ( id_widg == ID_BUTT_TRC && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TRC ), data, sizeof( data ) );
                ConfigLimit( CFG_DATLOG_UPD_TRC, &min, &max );
                result = WinKeyPad( data, 3, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TRC ), result );
               }
            break;
        default:
            WM_DefaultProc( pMsg );
            break;
       }
 }

//*************************************************************************************************
// Функция обратного вызова, обработка сообщений, параметры генератора
//*************************************************************************************************
static void cb_Dialog3( WM_MESSAGE * pMsg ) {

    char data[80], *result;
    int id_widg, ntf_code;

    switch ( pMsg->MsgId ) {
        case WM_INIT_DIALOG:
            break;
        case WM_NOTIFY_PARENT:
            id_widg = WM_GetId( pMsg-> hWinSrc );
            ntf_code = pMsg->Data.v;
            //Задержка запуска после отключения основной сети
            if ( id_widg == ID_BUTT_DSTART && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_DSTART ), data, sizeof( data ) );
                ConfigLimit( CFG_GEN_DELAY_START, &min, &max );
                result = WinKeyPad( data, 4, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_DSTART ), result );
               }
            //Задержка выключения после восстановления основной сети
            if ( id_widg == ID_BUTT_DSTOP && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_DSTOP ), data, sizeof( data ) );
                ConfigLimit( CFG_GEN_DELAY_STOP, &min, &max );
                result = WinKeyPad( data, 4, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_DSTOP ), result );
               }
            //Кол-во попыток запуска генератора
            if ( id_widg == ID_BUTT_CSTART && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_CSTART ), data, sizeof( data ) );
                ConfigLimit( CFG_GEN_CNT_START, &min, &max );
                result = WinKeyPad( data, 1, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_CSTART ), result );
               }
            //Ожидание сигнала запуска генератора
            if ( id_widg == ID_BUTT_CHRUN && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_CHRUN ), data, sizeof( data ) );
                ConfigLimit( CFG_GEN_DELAY_CHK_RUN, &min, &max );
                result = WinKeyPad( data, 1, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_CHRUN ), result );
               }
            //Пауза между запусками
            if ( id_widg == ID_BUTT_BSTART && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_BSTART ), data, sizeof( data ) );
                ConfigLimit( CFG_GEN_BEFORE_START, &min, &max );
                result = WinKeyPad( data, 2, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_BSTART ), result );
               }
            //Продолжительность запуска для каждой попытки
            if ( id_widg == ID_BUTT_TSTART && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TSTART ), data, sizeof( data ) );
                result = WinKeyBrd( data, LEN_EDIT_TSTART );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TSTART ), result );
               }
            //Максимальная продолжительность работы генератора
            if ( id_widg == ID_BUTT_TRUN && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TRUN ), data, sizeof( data ) );
                ConfigLimit( CFG_GEN_TIME_RUN, &min, &max );
                result = WinKeyPad( data, 5, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TRUN ), result );
               }
            //Продолжительность паузы между длительными работами
            if ( id_widg == ID_BUTT_TSLP && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TSLP ), data, sizeof( data ) );
                ConfigLimit( CFG_GEN_TIME_SLEEP, &min, &max );
                result = WinKeyPad( data, 4, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TSLP ), result );
               }
            //Продолжительность тестирования генератора
            if ( id_widg == ID_BUTT_TEST && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TEST ), data, sizeof( data ) );
                ConfigLimit( CFG_GEN_TIME_TEST, &min, &max );
                result = WinKeyPad( data, 3, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TEST ), result );
               }
            break;
        default:
            WM_DefaultProc( pMsg );
            break;
       }
 }

//*************************************************************************************************
// Функция обратного вызова, обработка сообщений, закладка SPA
//*************************************************************************************************
static void cb_Dialog4( WM_MESSAGE * pMsg ) {

    char data[80], *result;
    int id_widg, ntf_code;

    switch ( pMsg->MsgId ) {
        case WM_INIT_DIALOG:
            break;
        case WM_NOTIFY_PARENT:
            id_widg = WM_GetId( pMsg-> hWinSrc );
            ntf_code = pMsg->Data.v;
            //Часовой пояс
            if ( id_widg == ID_BUTT_TZ && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TZ ), data, sizeof( data ) );
                ConfigLimit( CFG_SPA_TIMEZONE, &min, &max );
                result = WinKeyPad( data, 2, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TZ ), result );
               }
            //Широта места
            if ( id_widg == ID_BUTT_LAT && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_LAT ), data, sizeof( data ) );
                ConfigLimit( CFG_SPA_LATITUDE, &min, &max );
                result = WinKeyPad( data, 9, KEY_GRP_FLOAT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_LAT ), result );
               }
            //Долгота места
            if ( id_widg == ID_BUTT_LNG && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_LNG ), data, sizeof( data ) );
                ConfigLimit( CFG_SPA_LONGITUDE, &min, &max );
                result = WinKeyPad( data, 9, KEY_GRP_FLOAT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_LNG ), result );
               }
            //Высота места
            if ( id_widg == ID_BUTT_ELV && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_ELV ), data, sizeof( data ) );
                ConfigLimit( CFG_SPA_ELEVATION, &min, &max );
                result = WinKeyPad( data, 4, KEY_GRP_FLOAT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_ELV ), result );
               }
            //Среднегодовое местное давление
            if ( id_widg == ID_BUTT_PRS && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_PRS ), data, sizeof( data ) );
                ConfigLimit( CFG_SPA_PRESSURE, &min, &max );
                result = WinKeyPad( data, 3, KEY_GRP_INT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_PRS ), result );
               }
            //Среднегодовая местная температура
            if ( id_widg == ID_BUTT_TEMP && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TEMP ), data, sizeof( data ) );
                ConfigLimit( CFG_SPA_PRESSURE, &min, &max );
                result = WinKeyPad( data, 5, KEY_GRP_FLOAT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_TEMP ), result );
               }
            //Наклон поверхности (измеряется от горизонтальной плоскости)
            if ( id_widg == ID_BUTT_SLP && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_SLP ), data, sizeof( data ) );
                ConfigLimit( CFG_SPA_TEMPERATURE, &min, &max );
                result = WinKeyPad( data, 5, KEY_GRP_FLOAT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_SLP ), result );
               }
            //Вращение поверхности азимут (с юга на проекции нормали)
            if ( id_widg == ID_BUTT_AZM && ntf_code == WM_NOTIFICATION_CLICKED ) {
                EDIT_GetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_AZM ), data, sizeof( data ) );
                ConfigLimit( CFG_SPA_AZM_ROTATION, &min, &max );
                result = WinKeyPad( data, 5, KEY_GRP_FLOAT, min, max );
                if ( result != NULL )
                    EDIT_SetText( WM_GetDialogItem( pMsg->hWin, ID_EDIT_AZM ), result );
               }
            break;
        default:
            WM_DefaultProc( pMsg );
            break;
       }
 }

//*************************************************************************************************
// Основная функция вызова формы
//*************************************************************************************************
void WinParam( void ) {

    GUI_GetOrg( &org_x, &org_y ); //координаты текущего виртуального окна
    GUI_ExecDialogBox( DialogCreate, GUI_COUNTOF( DialogCreate ), &cb_Dialog, 0, 0, 0 );
 } 

//*************************************************************************************************
// Функция применения параметров (без сохранения в EEPROM)
//*************************************************************************************************
static void ApplyConfig( void ) {

    char data[80];
    ConfigValSet cfg_value;
    
    if ( MessageBox( MESSAGE_QUESTION, MessageGui( MSG_GUI_APPLY ) ) == MB_NO )
        return;
    memset( &conf_save, 0x00, sizeof( conf_save ) );
    //не изменяемые параметры
    conf_save.gen_last_run = config.gen_last_run;
    conf_save.last_charge = config.last_charge;
    //основные параметры
    EDIT_GetText( WM_GetDialogItem( hDialog1, ID_EDIT_SCR ), data, sizeof( data ) );
    strncpy( conf_save.scr_file, data, sizeof( conf_save.scr_file ) );
    EDIT_GetText( WM_GetDialogItem( hDialog1, ID_EDIT_JOB1 ), data, sizeof( data ) );
    strncpy( conf_save.job_file, data, sizeof( conf_save.job_file ) );
    EDIT_GetText( WM_GetDialogItem( hDialog1, ID_EDIT_JOB2 ), data, sizeof( data ) );
    strncpy( conf_save.job_test, data, sizeof( conf_save.job_test ) );
    EDIT_GetText( WM_GetDialogItem( hDialog1, ID_EDIT_MSYS ), data, sizeof( data ) );
    conf_save.mode_sys = atoi( data );
    EDIT_GetText( WM_GetDialogItem( hDialog1, ID_EDIT_CRSTOP ), data, sizeof( data ) );
    conf_save.pb_current_stop = atoi( data );
    EDIT_GetText( WM_GetDialogItem( hDialog1, ID_EDIT_STINV ), data, sizeof( data ) );
    conf_save.delay_start_inv = atoi( data );
    EDIT_GetText( WM_GetDialogItem( hDialog1, ID_EDIT_SPINV ), data, sizeof( data ) );
    conf_save.delay_stop_inv = atoi( data );
    conf_save.mode_logging = CHECKBOX_GetState( WM_GetDialogItem( hDialog1, ID_CHECKBOX_MLOG ) );
    //параметры логирования
    EDIT_GetText( WM_GetDialogItem( hDialog2, ID_EDIT_PB ), data, sizeof( data ) );
    conf_save.datlog_upd_chrg = atoi( data );
    EDIT_GetText( WM_GetDialogItem( hDialog2, ID_EDIT_MPPT ), data, sizeof( data ) );
    conf_save.datlog_upd_mppt = atoi( data );
    EDIT_GetText( WM_GetDialogItem( hDialog2, ID_EDIT_BMON ), data, sizeof( data ) );
    conf_save.datlog_upd_bmon = atoi( data );
    EDIT_GetText( WM_GetDialogItem( hDialog2, ID_EDIT_TS ), data, sizeof( data ) );
    conf_save.datlog_upd_inv = atoi( data );
    EDIT_GetText( WM_GetDialogItem( hDialog2, ID_EDIT_TRC ), data, sizeof( data ) );
    conf_save.datlog_upd_trc = atoi( data );
    conf_save.log_enable_pv = CHECKBOX_GetState( WM_GetDialogItem( hDialog2, ID_CHECKBOX_PV ) );
    conf_save.log_enable_chrg = CHECKBOX_GetState( WM_GetDialogItem( hDialog2, ID_CHECKBOX_PB ) );
    conf_save.log_enable_mppt = CHECKBOX_GetState( WM_GetDialogItem( hDialog2, ID_CHECKBOX_MPPT ) );
    conf_save.log_enable_inv = CHECKBOX_GetState( WM_GetDialogItem( hDialog2, ID_CHECKBOX_TS ) );
    conf_save.log_enable_bmon = CHECKBOX_GetState( WM_GetDialogItem( hDialog2, ID_CHECKBOX_BMON ) );
    conf_save.log_enable_gen = CHECKBOX_GetState( WM_GetDialogItem( hDialog2, ID_CHECKBOX_GEN ) );
    conf_save.log_enable_alt = CHECKBOX_GetState( WM_GetDialogItem( hDialog2, ID_CHECKBOX_ALT ) );
    conf_save.log_enable_trc = CHECKBOX_GetState( WM_GetDialogItem( hDialog2, ID_CHECKBOX_TRC ) );
    //параметры генератора
    EDIT_GetText( WM_GetDialogItem( hDialog3, ID_EDIT_DSTART ), data, sizeof( data ) );
    conf_save.gen_delay_start = atoi( data );
    EDIT_GetText( WM_GetDialogItem( hDialog3, ID_EDIT_DSTOP ), data, sizeof( data ) );
    conf_save.gen_delay_stop = atoi( data );
    EDIT_GetText( WM_GetDialogItem( hDialog3, ID_EDIT_CSTART ), data, sizeof( data ) );
    conf_save.gen_cnt_start = atoi( data );
    EDIT_GetText( WM_GetDialogItem( hDialog3, ID_EDIT_CHRUN ), data, sizeof( data ) );
    conf_save.gen_delay_chk_run = atoi( data );
    EDIT_GetText( WM_GetDialogItem( hDialog3, ID_EDIT_BSTART ), data, sizeof( data ) );
    conf_save.gen_before_start = atoi( data );
    EDIT_GetText( WM_GetDialogItem( hDialog3, ID_EDIT_TSTART ), data, sizeof( data ) );
    //преобразование значения в массив для параметра CFG_GEN_TIME_START
    StrToConfigVal( CFG_GEN_TIME_START, data, &cfg_value );
    memcpy( (uint8_t *)&conf_save.gen_time_start, (uint8_t *)cfg_value.uint8_array, sizeof( cfg_value.uint8_array ) );
    EDIT_GetText( WM_GetDialogItem( hDialog3, ID_EDIT_TRUN ), data, sizeof( data ) );
    conf_save.gen_time_run = atoi( data );
    EDIT_GetText( WM_GetDialogItem( hDialog3, ID_EDIT_TSLP ), data, sizeof( data ) );
    conf_save.gen_time_sleep = atoi( data );
    EDIT_GetText( WM_GetDialogItem( hDialog3, ID_EDIT_TEST ), data, sizeof( data ) );
    conf_save.gen_time_test = atoi( data );
    conf_save.gen_auto_mode = CHECKBOX_GetState( WM_GetDialogItem( hDialog3, ID_CHECKBOX_AUTO ) );
    //параметры SPA
    EDIT_GetText( WM_GetDialogItem( hDialog4, ID_EDIT_TZ ), data, sizeof( data ) );
    conf_save.spa_timezone = atof( data );
    EDIT_GetText( WM_GetDialogItem( hDialog4, ID_EDIT_LAT ), data, sizeof( data ) );
    conf_save.spa_latitude = atof( data );
    EDIT_GetText( WM_GetDialogItem( hDialog4, ID_EDIT_LNG ), data, sizeof( data ) );
    conf_save.spa_longitude = atof( data );
    EDIT_GetText( WM_GetDialogItem( hDialog4, ID_EDIT_ELV ), data, sizeof( data ) );
    conf_save.spa_elevation = atof( data );
    EDIT_GetText( WM_GetDialogItem( hDialog4, ID_EDIT_PRS ), data, sizeof( data ) );
    conf_save.spa_pressure = atof( data );
    EDIT_GetText( WM_GetDialogItem( hDialog4, ID_EDIT_TEMP ), data, sizeof( data ) );
    conf_save.spa_temperature = atof( data );
    EDIT_GetText( WM_GetDialogItem( hDialog4, ID_EDIT_SLP ), data, sizeof( data ) );
    conf_save.spa_slope = atof( data );
    EDIT_GetText( WM_GetDialogItem( hDialog4, ID_EDIT_AZM ), data, sizeof( data ) );
    conf_save.spa_azm_rotation = atof( data );
    //общие параметры
    if ( strcmp( conf_save.scr_file, config.scr_file ) != 0 ) {
        cfg_value.ptr = conf_save.scr_file;
        SendConfig( CFG_SCR_FILE, cfg_value );
       }
    if ( strcmp( conf_save.job_file, config.job_file ) != 0 ) {
        cfg_value.ptr = conf_save.job_file;
        SendConfig( CFG_JOB_FILE, cfg_value );
       }
    if ( strcmp( conf_save.job_test, config.job_test ) != 0 ) {
        cfg_value.ptr = conf_save.job_test;
        SendConfig( CFG_JOB_TEST, cfg_value );
       }
    if ( conf_save.mode_sys != config.mode_sys ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint8 = conf_save.mode_sys;
        SendConfig( CFG_MODE_SYS, cfg_value );
       }
    if ( conf_save.pb_current_stop != config.pb_current_stop ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint8 = conf_save.pb_current_stop;
        SendConfig( CFG_PB_CURRENT_STOP, cfg_value );
       }
    if ( conf_save.delay_start_inv != config.delay_start_inv ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.delay_start_inv;
        SendConfig( CFG_DELAY_START_INV, cfg_value );
       }
    if ( conf_save.delay_stop_inv != config.delay_stop_inv ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.delay_stop_inv;
        SendConfig( CFG_DELAY_STOP_INV, cfg_value );
       }
    if ( conf_save.mode_logging != config.mode_logging ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint8 = conf_save.mode_logging;
        SendConfig( CFG_MODE_LOGGING, cfg_value );
       }
    //параметры логирования
    if ( conf_save.datlog_upd_chrg != config.datlog_upd_chrg ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.datlog_upd_chrg;
        SendConfig( CFG_DATLOG_UPD_CHARGE, cfg_value );
       }
    if ( conf_save.datlog_upd_mppt != config.datlog_upd_mppt ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.datlog_upd_mppt;
        SendConfig( CFG_DATLOG_UPD_MPPT, cfg_value );
       }
    if ( conf_save.datlog_upd_bmon != config.datlog_upd_bmon ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.datlog_upd_bmon;
        SendConfig( CFG_DATLOG_UPD_BMON, cfg_value );
       }
    if ( conf_save.datlog_upd_inv != config.datlog_upd_inv ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.datlog_upd_inv;
        SendConfig( CFG_DATLOG_UPD_INV, cfg_value );
       }
    if ( conf_save.datlog_upd_trc != config.datlog_upd_trc ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.datlog_upd_trc;
        SendConfig( CFG_DATLOG_UPD_TRC, cfg_value );
       }
    if ( conf_save.log_enable_pv != config.log_enable_pv ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.log_enable_pv;
        SendConfig( CFG_LOG_ENABLE_PV, cfg_value );
       }
    if ( conf_save.log_enable_chrg != config.log_enable_chrg ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint8 = conf_save.log_enable_chrg;
        SendConfig( CFG_LOG_ENABLE_CHARGE, cfg_value );
       }
    if ( conf_save.log_enable_mppt != config.log_enable_mppt ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint8 = conf_save.log_enable_mppt;
        SendConfig( CFG_LOG_ENABLE_MPPT, cfg_value );
       }
    if ( conf_save.log_enable_inv != config.log_enable_inv ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint8 = conf_save.log_enable_inv;
        SendConfig( CFG_LOG_ENABLE_INV, cfg_value );
       }
    if ( conf_save.log_enable_bmon != config.log_enable_bmon ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint8 = conf_save.log_enable_bmon;
        SendConfig( CFG_LOG_ENABLE_BMON, cfg_value );
       }
    if ( conf_save.log_enable_gen != config.log_enable_gen ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint8 = conf_save.log_enable_gen;
        SendConfig( CFG_LOG_ENABLE_GEN, cfg_value );
       }
    if ( conf_save.log_enable_alt != config.log_enable_alt ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint8 = conf_save.log_enable_alt;
        SendConfig( CFG_LOG_ENABLE_ALT, cfg_value );
       }
    if ( conf_save.log_enable_trc != config.log_enable_trc ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint8 = conf_save.log_enable_trc;
        SendConfig( CFG_LOG_ENABLE_TRC, cfg_value );
       }
    //параметры генератора
    if ( conf_save.gen_delay_start != config.gen_delay_start ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.gen_delay_start;
        SendConfig( CFG_GEN_DELAY_START, cfg_value );
       }
    if ( conf_save.gen_delay_stop != config.gen_delay_stop ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.gen_delay_stop;
        SendConfig( CFG_GEN_DELAY_STOP, cfg_value );
       }
    if ( conf_save.gen_cnt_start != config.gen_cnt_start ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint8 = conf_save.gen_cnt_start;
        SendConfig( CFG_GEN_CNT_START, cfg_value );
       }
    if ( conf_save.gen_delay_chk_run != config.gen_delay_chk_run ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.gen_delay_chk_run;
        SendConfig( CFG_GEN_DELAY_CHK_RUN, cfg_value );
       }
    if ( conf_save.gen_before_start != config.gen_before_start ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.gen_before_start;
        SendConfig( CFG_GEN_BEFORE_START, cfg_value );
       }
    if ( memcmp( (uint8_t *)&conf_save.gen_time_start, (uint8_t *)config.gen_time_start, sizeof( cfg_value.uint8_array ) ) != 0 ) {
        memcpy( (uint8_t *)&cfg_value.uint8_array, (uint8_t *)(uint8_t *)&conf_save.gen_time_start, sizeof( cfg_value.uint8_array ) );
        SendConfig( CFG_GEN_TIME_START, cfg_value );
       }
    if ( conf_save.gen_time_run != config.gen_time_run ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.gen_time_run;
        SendConfig( CFG_GEN_TIME_RUN, cfg_value );
       }
    if ( conf_save.gen_time_sleep != config.gen_time_sleep ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.gen_time_sleep;
        SendConfig( CFG_GEN_TIME_SLEEP, cfg_value );
       }
    if ( conf_save.gen_time_test != config.gen_time_test ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.gen_time_test;
        SendConfig( CFG_GEN_TIME_TEST, cfg_value );
       }
    if ( conf_save.gen_auto_mode != config.gen_auto_mode ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint8 = conf_save.gen_auto_mode;
        SendConfig( CFG_GEN_AUTO_MODE, cfg_value );
       }
    //параметры SPA
    if ( conf_save.spa_timezone != config.spa_timezone ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint8 = conf_save.spa_timezone;
        SendConfig( CFG_SPA_TIMEZONE, cfg_value );
       }
    if ( conf_save.spa_latitude != config.spa_latitude ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.dbl = conf_save.spa_latitude;
        SendConfig( CFG_SPA_LATITUDE, cfg_value );
       }
    if ( conf_save.spa_longitude != config.spa_longitude ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.dbl = conf_save.spa_longitude;
        SendConfig( CFG_SPA_LONGITUDE, cfg_value );
       }
    if ( conf_save.spa_elevation != config.spa_elevation ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.int16 = conf_save.spa_elevation;
        SendConfig( CFG_SPA_ELEVATION, cfg_value );
       }
    if ( conf_save.spa_pressure != config.spa_pressure ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.spa_pressure;
        SendConfig( CFG_SPA_PRESSURE, cfg_value );
       }
    if ( conf_save.spa_temperature != config.spa_temperature ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint8 = conf_save.spa_temperature;
        SendConfig( CFG_SPA_TEMPERATURE, cfg_value );
       }
    if ( conf_save.spa_slope != config.spa_slope ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.spa_slope;
        SendConfig( CFG_SPA_SLOPE, cfg_value );
       }
    if ( conf_save.spa_azm_rotation != config.spa_azm_rotation ) {
        memset( (uint8_t *)&cfg_value.uint8_array, 0x00, sizeof( cfg_value.uint8_array ) );
        cfg_value.uint16 = conf_save.spa_azm_rotation;
        SendConfig( CFG_SPA_AZM_ROTATION, cfg_value );
       }
    MessageBox( MESSAGE_INFO, MessageGui( MSG_GUI_VALUE_CHANGE ) );
 }
