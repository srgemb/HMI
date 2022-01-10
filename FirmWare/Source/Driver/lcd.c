
//*************************************************************************************************
//
// Инициализация графического контроллера, управление подсветкой дисплея
//
//*************************************************************************************************

#include <stdio.h>
#include <stdlib.h>

#include "lpc407x_8x_177x_8x.h"
#include "pin_lpc40xx.h"
#include "gpio_lpc40xx.h"
#include "lpc_wwdt.h"

#include "cmsis_os2.h"
#include "rtx_config.h"

#include "main.h"
#include "ports.h"
#include "events.h"
#include "lpc_pwm.h"

#include "lcd.h"
#include "lcd_def.h"
#include "eeprom.h"
#include "ext_ram_addr.h"

//*************************************************************************************************
// Внешние переменные
//*************************************************************************************************
extern CONFIG_LCD cfg_lcd;

//*************************************************************************************************
// Локальные константы
//*************************************************************************************************
#define PWM_VALUE_0             0                   //значение ШИМ - подсветка выключена (0%)
#define PWM_VALUE_100           100                 //значение ШИМ - подсветка включена (100%)

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
static osTimerId_t timer_backligth;
static osMessageQueueId_t backligth_queue = NULL;

//*************************************************************************************************
// Атрибуты объектов RTOS
//*************************************************************************************************
static const osThreadAttr_t attr_lcd = {
    .name = "BackLigth",
    .priority = osPriorityNormal
 };

static const osTimerAttr_t attr_tmr = { .name = "BackLigth" };
static const osMessageQueueAttr_t attr_mes = { .name = "BackLigth" };

//*************************************************************************************************
// Прототипы локальных функций
//*************************************************************************************************
static void LCDBackLigthInit( void );
static void TimerBackLigth( void *arg );
static void TaskBackLigth( void *pvParameters );

//*************************************************************************************************
// Инициализация контроллера дисплея
//*************************************************************************************************
void LCDInit( void ) {

    LPC_IOCON->P0_4  = 0x27;
    LPC_IOCON->P0_5  = 0x27;
    LPC_IOCON->P4_28 = 0x27;
    LPC_IOCON->P4_29 = 0x27;
    LPC_IOCON->P2_6  = 0x27;
    LPC_IOCON->P2_7  = 0x27;
    LPC_IOCON->P2_8  = 0x27;
    LPC_IOCON->P2_9  = 0x27;

    LPC_IOCON->P0_6  = 0x27;
    LPC_IOCON->P0_7  = 0x27;
    LPC_IOCON->P1_20 = 0x27;
    LPC_IOCON->P1_21 = 0x27;
    LPC_IOCON->P1_22 = 0x27;
    LPC_IOCON->P1_23 = 0x27;
    LPC_IOCON->P1_24 = 0x27;
    LPC_IOCON->P1_25 = 0x27;

    LPC_IOCON->P0_8  = 0x27;
    LPC_IOCON->P0_9  = 0x27;
    LPC_IOCON->P2_12 = 0x27;
    LPC_IOCON->P2_13 = 0x27;
    LPC_IOCON->P1_26 = 0x27;
    LPC_IOCON->P1_27 = 0x27;
    LPC_IOCON->P1_28 = 0x27;
    LPC_IOCON->P1_29 = 0x27;

    LPC_IOCON->P2_2  = 0x27;         //LCD_DCLK_IOCON
    LPC_IOCON->P2_3  = 0x27;         //LCD_VS_IOCON
    LPC_IOCON->P2_4  = 0x27;         //LCD_ENAB_IOCON
    LPC_IOCON->P2_5  = 0x27;         //LCD_HS_IOCON

    //init GLCD cotroller
    LPC_LCD->CTRL = ( 0 << LCD_CTRL_LCDEN )   |     //disable GLCD controller
                    ( 5 << LCD_CTRL_LCDBPP )  |     //101 = 24 bpp (TFT panel only)
                                                    //STN LCD monochrome/color selection. 0 = STN LCD is color./1 = STN LCD is monochrome.
                    ( 1 << LCD_CTRL_LCDTFT )  |     //0 = LCD is an STN display. Use gray scaler/1 = LCD is a TFT display. Do not use gray scaler
                    ( 0 << LCD_CTRL_LCDDUAL ) |     //single panel
                    ( 0 << LCD_CTRL_BGR )     |     //0 = RGB: normal output/1 = BGR: red and blue swapped
                    ( 0 << LCD_CTRL_BEBO )    |     //little endian byte order
                    ( 0 << LCD_CTRL_BEPO )    |     //little endian pix order
                    ( 0 << LCD_CTRL_LCDPWR );       //disable power

    //частота обновления одного пиксела
    LPC_SC->LCD_CFG = (uint32_t)( SystemCoreClock / (uint32_t)C_GLCD_PIX_CLK );

    LPC_LCD->POL = ( 1 << LCD_POL_BCD )    |                  //bypass inrenal clk divider
                   ( 0 << LCD_POL_CLKSEL ) |                  //clock source for the LCD block is HCLK
                   ( 1 << LCD_POL_IVS )    |                  //LCDFP pin is active LOW and inactive HIGH
                   ( 1 << LCD_POL_IHS )    |                  //LCDLP pin is active LOW and inactive HIGH
                   ( 0 << LCD_POL_IPC )    |                  //data is driven out into the LCD on the rising edge
                   ( 0 << LCD_POL_IOE )    |                  //active high
                   ( ( C_GLCD_H_SIZE - 1 ) << LCD_POL_CPL );  //Clocks per line

    //init Horizontal Timing
    LPC_LCD->TIMH = ( ( C_GLCD_H_BACK_PORCH - 1 ) << LCD_TIMH_HBP )  |  //Horizontal back porch
                    ( ( C_GLCD_H_FRONT_PORCH - 1 ) << LCD_TIMH_HFP ) |  //Horizontal front porch
                    ( ( C_GLCD_H_PULSE - 1 ) << LCD_TIMH_HSW )       |  //Horizontal synchronization pulse width
                    ( ( ( C_GLCD_H_SIZE/16 ) - 1 ) << LCD_TIMH_PPL);    //Pixels-per-line

    //init Vertical Timing
    LPC_LCD->TIMV = ( C_GLCD_V_BACK_PORCH << LCD_TIMV_VBP ) |   //Vertical back porch
                    ( C_GLCD_V_FRONT_PORCH << LCD_TIMV_VFP )|   //Vertical front porch
                    ( C_GLCD_V_PULSE << LCD_TIMV_VSW )      |   //Vertical synchronization pulse width
                    ( (C_GLCD_V_SIZE - 1) << LCD_TIMV_LPP );    //Lines per panel

    // Frame Base Address doubleword aligned
    LPC_LCD->UPBASE = VRAM_ADDR & ~7UL;
    LPC_LCD->LPBASE = VRAM_ADDR & ~7UL;

    LPC_LCD->CTRL |= ( 1 << LCD_CTRL_LCDEN );    //LCD enable control bit
    LPC_LCD->CTRL |= ( 1 << LCD_CTRL_LCDPWR );   //LCD power enable
    //инициализация задачи подсветки
    LCDBackLigthInit();
 }

//*************************************************************************************************
// Установка адреса видео буфера
// uint32_t *addr - адрес видео буфера
//*************************************************************************************************
void LCDSetAddress( uint32_t addr ) {

    LPC_LCD->UPBASE = addr & ~7UL;
    LPC_LCD->LPBASE = addr & ~7UL;
 }
 
//*************************************************************************************************
// Инициализация управленния ШИМ подсветкой дисплея
//*************************************************************************************************
static void LCDBackLigthInit( void ) {

    PWM_TIMERCFG_Type PWM_Timer;
    
    //P2.0 - вывод альтернативной функции PWM1
    PIN_Configure( PORT2, LCD_ADJ_BRG, PWM1_OUT | IOCON_MODE_PLAIN | IOCON_DIGITIAL_MODE );

    PWM_Timer.PrescaleOption = PWM_TIMER_PRESCALE_USVAL;
    PWM_Timer.PrescaleValue = 50; //(us)
    PWM_Init( PWM_1, PWM_MODE_TIMER, &PWM_Timer );
    
    PWM_MatchUpdate( PWM_1, 0, 100, PWM_MATCH_UPDATE_NOW );
    PWM_MatchUpdate( PWM_1, 1, 100, PWM_MATCH_UPDATE_NOW );
    
    PWM_ChannelCmd( PWM_1, 1, ENABLE );
    PWM_ResetCounter( PWM_1 );
    PWM_CounterCmd( PWM_1, ENABLE );
    PWM_Cmd( PWM_1, ENABLE );

    //инициализация таймера задержки выключения
    timer_backligth = osTimerNew( TimerBackLigth, osTimerOnce, NULL, &attr_tmr );
    //задача плавного вкл/выкл подсветки
    osThreadNew( TaskBackLigth, NULL, &attr_lcd );
    //таймер выключения подсветки дисплея
    osTimerStart( timer_backligth, cfg_lcd.disp_off * SEC_TO_TICK );
    backligth_queue = osMessageQueueNew( 16, sizeof( uint8_t ), &attr_mes );
 }

//*************************************************************************************************
// Функция управление подсветкой дисплея
// BackLigth mode - режим подсветки
//*************************************************************************************************
void LCDBackLigth( BackLigth mode ) {

    uint32_t events;
    
    if ( mode == LCD_BACK_LIGTH_OFF ) {
        //выключение подсветки
        PWM_MatchUpdate( PWM_1, 1, PWM_VALUE_0, PWM_MATCH_UPDATE_NEXT_RST );
        //остановка таймера времени подсветки
        osTimerStop( timer_backligth );
       }
    if ( mode == LCD_BACK_LIGTH_ON ) {
        //останавливаем плавное выключение если оно уже началось
        events = MSG_LCD_BACK_SLOW_STOP;
        osMessageQueuePut( backligth_queue, &events, 0, 0 );
       }
    if ( mode == LCD_BACK_LIGTH_SLOW_ON ) {
        events = MSG_LCD_BACK_SLOW_ON;
        osMessageQueuePut( backligth_queue, &events, 0, 0 );
       }
    if ( mode == LCD_BACK_LIGTH_SLOW_OFF ) {
        events = MSG_LCD_BACK_SLOW_OFF;
        osMessageQueuePut( backligth_queue, &events, 0, 0 );
       }
 }

//*********************************************************************************************
// Задача управления подсветкой дисплея
//*********************************************************************************************
static void TaskBackLigth( void *pvParameters ) {

    uint16_t msg;
    osStatus_t status;
    uint8_t level, mode = 0;

    for ( ;; ) {
        status = osMessageQueueGet( backligth_queue, &msg, NULL, 0 );
        if ( status == osOK ) {
            //обработка сообщения на плавное вкл/выкл/останов вык.
            if ( msg == MSG_LCD_BACK_SLOW_ON ) {
                level = PWM_VALUE_0;
                mode = LCD_BACK_LIGTH_SLOW_ON;
               }
            if ( msg == MSG_LCD_BACK_SLOW_OFF ) {
                level = PWM_VALUE_100;
                mode = LCD_BACK_LIGTH_SLOW_OFF;
               }
            if ( msg == MSG_LCD_BACK_SLOW_STOP ) {
                //выключаем плавное вкл/выкл и включаем
                level = PWM_VALUE_100;
                mode = LCD_BACK_LIGTH_ON;
                PWM_MatchUpdate( PWM_1, 1, level, PWM_MATCH_UPDATE_NEXT_RST );
                //перезапуск таймера времени подсветки
                osTimerStart( timer_backligth, cfg_lcd.disp_off * SEC_TO_TICK );
               }
           }
        //плавное включение
        if ( mode == LCD_BACK_LIGTH_SLOW_ON ) {
            PWM_MatchUpdate( PWM_1, 1, level, PWM_MATCH_UPDATE_NEXT_RST );
            level++;
            if ( level > PWM_VALUE_100 )
                mode = 0;
           }
        //плавное выключение
        if ( mode == LCD_BACK_LIGTH_SLOW_OFF ) {
            level--;
            if ( level == PWM_VALUE_0 )
                mode = 0;
            PWM_MatchUpdate( PWM_1, 1, level, PWM_MATCH_UPDATE_NEXT_RST );
           }
        osDelay( 20 );
        #if ( OS_ROBIN_ENABLE == 0 )
        osThreadYield();
        #endif
       }
 }

//*********************************************************************************************
// Функция обратного вызова таймера выключения подсветки дисплея
//*********************************************************************************************
static void TimerBackLigth( void *arg ) {

    //отсчет таймера завершен
    LCDBackLigth( LCD_BACK_LIGTH_SLOW_OFF );
 }

