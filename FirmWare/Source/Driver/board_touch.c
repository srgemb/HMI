
//*************************************************************************************************
//
// Обработка сигналов сенсорного экрана
//
//*************************************************************************************************

#include <stdint.h>
#include <stdbool.h>

#include "lpc407x_8x_177x_8x.h"
#include "lpc_timer.h"
#include "lpc_gpio.h"
#include "lpc_adc.h"
#include "pin_lpc40xx.h"
#include "gpio_lpc40xx.h"

#include "cmsis_os2.h"

#include "lcd.h"
#include "lcd_def.h"
#include "ports.h"
#include "board_touch.h"
#include "gpio.h"

//*************************************************************************************************
// Локальные константы
//*************************************************************************************************
#define	COORDINATE_X            0
#define	COORDINATE_Y            1

#define	RATE_X                  ( (float)1024/(float)C_GLCD_H_SIZE )
#define	RATE_Y                  ( (float)1024/(float)C_GLCD_V_SIZE )

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
typedef enum {
    TS_INTR_SETUP_DLY = 0, 
    TS_WAIT_FOR_TOUCH,
    TS_X1_SETUP_DLY, 
    TS_X1_MEASURE,
    TS_X2_SETUP_DLY, 
    TS_X2_MEASURE,
    TS_Y1_SETUP_DLY, 
    TS_Y1_MEASURE,
    TS_Y2_SETUP_DLY, 
    TS_Y2_MEASURE,
 } TouchScrState_t;

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
static TouchScrState_t ts_step;
static uint8_t touch, touch_temp, samples, touch_ready; 
static uint16_t x_result, y_result, x_temp, y_temp;

//*************************************************************************************************
// Touch screen sampling algorithm
// 1. Set Y1, Y2 to output 1, Set X1 - ADC input (ch 1), X2 input with pull-down
// 2. Wait setup delay
// 3. Check X2 state - if high the screen is touched set Y0 to low and wait setup delay
//                     else wait until interrupt by rising edge from X2
//    When interrupt arise Y0 to low and wait setup delay
// 4. Star conversion
// 5. After TS_samples change Y1 high Y2 low and result calculate  1023 - ADC
// 6. After TS_samples change Y1 to ADC  input Y2 input without pulls X1 and X2
//    output in low and high level
// 7. Same like X
// 8. Apply sample delay and return back to poin 1
//*************************************************************************************************

//*************************************************************************************************
// Инициализация сенсорного экрана. Вызов выполняется автоматически при инициализации 
// графики из модуля GUI_X_Touch.c (подключается к проекту) 
// файл расположен: ..\ARM\PACK\Keil\MDK-Middleware\x.xx.xx\emWin\Sample\GUI_X\
// return = 0 - function succeeded
//         -1 - function failed
//*************************************************************************************************
int32_t Touch_Initialize( void ) {

    TIM_TIMERCFG_Type timer1_cfg;
    TIM_MATCHCFG_Type timer1_mat;
    
    //Init variables
    x_result = y_result = 0;
    ts_step = TS_INTR_SETUP_DLY;
    touch_ready = touch_temp = touch = false;

    //Инициализация GPIOs
    //TS_TOCH_X1 вход ADC0 disable pulls
    GPIO_SetDir( PORT0, TS_X1_PIN, GPIO_DIR_INPUT );
    PIN_Configure( PORT0, TS_X1_PIN, GPIO_ADC_FUNCTION | IOCON_ANALOG_MODE );
    
    //TS_TOCH_X2 вход enable pull-down
    GPIO_SetDir( PORT0, TS_X2_PIN, GPIO_DIR_INPUT );
    PIN_Configure( PORT0, TS_X2_PIN, GPIO_PIN_FUNCTION | IOCON_DIGITIAL_MODE | IOCON_MODE_PULLDOWN );
    
    //TS_TOCH_Y1 выход "1" disable pulls (P0.25)
    GPIO_SetDir( PORT0, TS_Y1_PIN, GPIO_DIR_OUTPUT );
    PIN_Configure( PORT0, TS_Y1_PIN, GPIO_PIN_FUNCTION | IOCON_DIGITIAL_MODE );
    GPIO_OutputValue( PORT0, TS_Y1_MASK, 1 );
    
    //TS_TOCH_Y2 выход "1" disable pulls
    GPIO_SetDir( PORT0, TS_Y2_PIN, GPIO_DIR_OUTPUT );
    PIN_Configure( PORT0, TS_Y2_PIN, GPIO_PIN_FUNCTION | IOCON_DIGITIAL_MODE );
    GPIO_OutputValue( PORT0, TS_Y2_MASK, 1 );

    //установка прерывания на X2 по фронту
    GPIO_IntCmd( PORT0, TS_X2_MASK, GPIO_INTERRUPT_RISING );

    //включаем прерывание от GPIO
    NVIC_EnableIRQ( GPIO_IRQn );
    
    //Инициализация ADC
    ADC_Init( LPC_ADC, 16000 );
    ADC_ChannelCmd( LPC_ADC, 0, ENABLE );
    NVIC_EnableIRQ( ADC_IRQn );

    //Конфигурация таймера 1
    timer1_cfg.PrescaleOption = TIM_PRESCALE_USVAL; //предделитель в микросекундах
    timer1_cfg.PrescaleValue = 10; //usec
    timer1_mat.MatchChannel = 0;
    timer1_mat.IntOnMatch = true;
    timer1_mat.StopOnMatch = true;      //однократный отсчет
    timer1_mat.ResetOnMatch = true;
    timer1_mat.MatchValue = TS_SETUP_DELAY;
    TIM_ConfigMatch( LPC_TIM1, &timer1_mat );
    //сброс флагов прерывания
    TIM_ClearIntPending( LPC_TIM1, TIM_MR0_INT );
    //Инициализация режима таймера Timer Mode: every rising PCLK edge
    //отсчет по каждому фронту PCLK
    TIM_Init( LPC_TIM1, TIM_TIMER_MODE, &timer1_cfg );
    NVIC_EnableIRQ( TIMER1_IRQn );
    //запуск таймера
    TIM_Cmd( LPC_TIM1, ENABLE );
    return 0;
 }

//*************************************************************************************************
// Деинициализация сенсорного экрана. Вызов выполняется автоматически из модуля GUI_X_Touch.c 
// (подключается к проекту) 
// файл расположен: ..\ARM\PACK\Keil\MDK-Middleware\x.xx.xx\emWin\Sample\GUI_X\
// return = 0 - function succeeded
//         -1 - function failed
//*************************************************************************************************
int32_t Touch_Uninitialize( void ) {
    
    return 0;
 }
 
//*************************************************************************************************
// Возвращает статус сенсорного экрана.
// Вызов из GUI_TOUCH_Exec() (GUI_X_Touch.c), периодичность вызова из GUI - 20 ms
// TOUCH_STATE *pState - указатель на структуру TOUCH_STATE
// returns  = 0 - SUCCESS
//         != 0 - ERROR
//*************************************************************************************************
// pState->x             - координата X
// pState->y             - координата Y
// pState->pressed = 1/0 - есть нажатие/нет нажатия
//*************************************************************************************************
int32_t Touch_GetState( TOUCH_STATE *pState ) {

    if ( touch_ready ) {
        //есть обработанное нажатие
        pState->x = x_result;
        pState->y = y_result;
        pState->pressed = 1;
        touch_ready = false;
        return 0;
       }
    else {
        pState->pressed = 0;
        return 0;
       }
 }

//*************************************************************************************************
// Обработчик прерывания входа X2
//*************************************************************************************************
void GPIO_IRQHandler( void ) {

    //выключение прерывания на X2 по фронту
    GPIO_IntCmdClr( PORT0, TS_X2_MASK, GPIO_INTERRUPT_RISING );
    //сброс флага прерывания
    GPIO_ClearInt( PORT0, TS_X2_MASK );
    //Init ACD measure setup delay
    if ( ts_step == TS_WAIT_FOR_TOUCH ) {
        //Y1 = OUT:0 (P0.25)
        GPIO_OutputValue( PORT0, TS_Y1_MASK, 0 );
        //X2 Disable pull down
        PIN_Configure( PORT0, TS_X2_PIN, GPIO_PIN_FUNCTION | IOCON_DIGITIAL_MODE | IOCON_MODE_REPEATER );
        //Y2 = OUT:1 (P0.26)
        GPIO_OutputValue( PORT0, TS_Y2_MASK, 1 );
        //сброс счетчика измерений
        samples = 0;
        //Clear accumulators
        x_temp = y_temp = 0;
        if ( touch ) {
            //есть удержание нажатия, задержка перед следующим циклом измерений
            TIM_UpdateMatchValue( LPC_TIM1, 0, TS_SETUP_DELAY );
           } 
        else {
            //задержка перед началом цикла измерений
            TIM_UpdateMatchValue( LPC_TIM1, 0, TS_INIT_DELAY );
           }
        //запускаем цикл измерений по X1,X2,Y1,Y2
        ts_step = TS_X1_SETUP_DLY;
        //запуск таймера
        TIM_Cmd( LPC_TIM1, ENABLE );
       }
 }

//*************************************************************************************************
// Обработчик прерывания таймера 1
//*************************************************************************************************
void TIMER1_IRQHandler( void ) {

    //сброс флага прерывания
    TIM_ClearIntPending( LPC_TIM1, TIM_MR0_INT );
    //синхронный сброс предделителя и счетчика таймера
    TIM_ResetCounter( LPC_TIM1 );
    switch ( ts_step ) {
        case TS_WAIT_FOR_TOUCH:
        case TS_Y2_MEASURE:
        case TS_Y1_MEASURE:
        case TS_X2_MEASURE:
        case TS_X1_MEASURE:
            break;
        case TS_X1_SETUP_DLY:
        case TS_X2_SETUP_DLY:
        case TS_Y1_SETUP_DLY:
        case TS_Y2_SETUP_DLY:
            ++ts_step;
            //запуск измерений ADC
            ADC_StartCmd( LPC_ADC, ADC_START_NOW );
            break;
        case TS_INTR_SETUP_DLY:
            ++ts_step; //переход в режим ожидания нажатия TS_WAIT_FOR_TOUCH
            //сброс флага прерывания
            GPIO_ClearInt( PORT0, TS_X2_MASK );
            if ( !( GPIO_ReadValue( PORT0 ) & TS_X2_MASK ) ) {
                //в конце цикла измерений нет удержания нажатия
                touch_temp = touch = false;
                //разрешаем прерывание по X2, переход в режим ожидания нажатия
                GPIO_IntCmd( PORT0, TS_X2_MASK, GPIO_INTERRUPT_RISING );
               }
            else {
                if ( touch_temp ) {
                    //установлен флаг окончания цикла измерений
                    x_result = x_temp;
                    y_result = y_temp;
                    touch_ready = true;
                    LCDBackLigth( LCD_BACK_LIGTH_ON );
                   }
                touch = touch_temp;
                //Y1 = OUT:0 (P0.25)
                GPIO_OutputValue( PORT0, TS_Y1_MASK, 0 );
                //Y2 = OUT:1 (P0.26)
                GPIO_OutputValue( PORT0, TS_Y2_MASK, 1 );
                //X2 disable pull down 
                PIN_Configure( PORT0, TS_X2_PIN, GPIO_PIN_FUNCTION | IOCON_DIGITIAL_MODE | IOCON_MODE_REPEATER );
                //сброс счетчика измерений
                samples = 0;
                //Clear accumulators
                x_temp = y_temp = 0;
                if ( touch ) {
                    //есть удержание нажатия, задержка перед следующим циклом измерений
                    TIM_UpdateMatchValue( LPC_TIM1, 0, TS_REPEAT_DELAY );
                   }
                else {
                    //задержка после...
                    TIM_UpdateMatchValue( LPC_TIM1, 0, TS_INIT_DELAY );
                   }
                ts_step = TS_X1_SETUP_DLY;
                //запуск таймера
                TIM_Cmd( LPC_TIM1, ENABLE );
              }
            break;
        default:
            break;
    }
 }

//*************************************************************************************************
// Обработчик прерывания ADC
//*************************************************************************************************
void ADC_IRQHandler( void ) {

    uint32_t data;

    //выключаем преобразование ADC
    ADC_StartCmd( LPC_ADC, ADC_START_CONTINUOUS );
    data = ( ADC_GlobalGetData( LPC_ADC ) & 0x0000FFF0 ) >> 6;  //10 - bit result
    switch ( ts_step ) {
        case TS_WAIT_FOR_TOUCH:
        case TS_INTR_SETUP_DLY:
        case TS_X1_SETUP_DLY:
        case TS_X2_SETUP_DLY: 
        case TS_Y1_SETUP_DLY: 
        case TS_Y2_SETUP_DLY: 
            break;
        case TS_X1_MEASURE:
            //цикл измерений по X1
            y_temp += data;
            if ( ++samples >= TS_SAMPLES ) {
                //завершен цикл измерений по X1
                samples = 0;
                ts_step = TS_X2_SETUP_DLY;
                //переключаем пины для измерения по X2
                //Y2 = OUT:0 (P0.26)
                GPIO_SetDir( PORT0, TS_Y2_PIN, GPIO_DIR_OUTPUT );
                PIN_Configure( PORT0, TS_Y2_PIN, GPIO_PIN_FUNCTION | IOCON_DIGITIAL_MODE );
                GPIO_OutputValue( PORT0, TS_Y2_MASK, 0 );
                //Y1 = OUT:1 (P0.25);
                GPIO_SetDir( PORT0, TS_Y1_PIN, GPIO_DIR_OUTPUT );
                PIN_Configure( PORT0, TS_Y1_PIN, GPIO_PIN_FUNCTION | IOCON_DIGITIAL_MODE );
                GPIO_OutputValue( PORT0, TS_Y1_MASK, 1 );
                //задержка перед следующим измерением
                TIM_UpdateMatchValue( LPC_TIM1, 0, TS_SETUP_DELAY );
                //запуск таймера
                TIM_Cmd( LPC_TIM1, ENABLE );
               }
            else {
                //запуск измерений ADC
                ADC_StartCmd( LPC_ADC, ADC_START_NOW );
               }
            break;
        case TS_X2_MEASURE:
            //цикл измерений по X2
            y_temp += 1023UL - data;
            if ( ++samples >= TS_SAMPLES ) {
                //завершен цикл измерений по X1
                samples = 0;
                ts_step = TS_Y1_SETUP_DLY;
                //переключаем пины для измерения по Y1
                //X1 = OUT:0 (P0.23) disable pulls
                GPIO_SetDir( PORT0, TS_X1_PIN, GPIO_DIR_OUTPUT );
                PIN_Configure( PORT0, TS_X1_PIN, GPIO_PIN_FUNCTION | IOCON_DIGITIAL_MODE | IOCON_MODE_REPEATER );
                GPIO_OutputValue( PORT0, TS_X1_MASK, 0 );
                //X2 = OUT:1 (P0.24)
                GPIO_SetDir( PORT0, TS_X2_PIN, GPIO_DIR_OUTPUT ); 
                PIN_Configure( PORT0, TS_X2_PIN, GPIO_PIN_FUNCTION | IOCON_DIGITIAL_MODE | IOCON_MODE_PLAIN );
                GPIO_OutputValue( PORT0, TS_X2_MASK, 1 );
                //Y1 = ADC Ch2 (P0.25)
                GPIO_SetDir( PORT0, TS_Y1_PIN, GPIO_DIR_INPUT );
                PIN_Configure( PORT0, TS_Y1_PIN, GPIO_ADC_FUNCTION | IOCON_ANALOG_MODE );
                //Y2 = IN (P0.26)
                GPIO_SetDir( PORT0, TS_Y2_PIN, GPIO_DIR_INPUT );
                //Y1 = ADC Ch2 (P0.25)
                ADC_ChannelCmd( LPC_ADC, 0, DISABLE );
                ADC_ChannelCmd( LPC_ADC, 2, ENABLE );
                //задержка перед следющим измерением
                TIM_UpdateMatchValue( LPC_TIM1, 0, TS_SETUP_DELAY );
                //запуск таймера
                TIM_Cmd( LPC_TIM1, ENABLE );
               }
            else {
                //запуск измерений ADC
                ADC_StartCmd( LPC_ADC, ADC_START_NOW );
               }
            break;
        case TS_Y1_MEASURE:
            //цикл измерений по Y1
            x_temp += 1023UL - data;
            if ( ++samples >= TS_SAMPLES ) {
                //завершен цикл измерений по Y1
                samples = 0;
                ts_step = TS_Y2_SETUP_DLY;
                //переключаем пины для измерения по Y2
                //X2 = OUT:0 (P0.24)
                GPIO_OutputValue( PORT0, TS_X2_MASK, 0 );
                //X1 = OUT:1 (P0.23)
                GPIO_OutputValue( PORT0, TS_X1_MASK, 1 );
                //задержка перед следющим измерением
                TIM_UpdateMatchValue( LPC_TIM1, 0, TS_SETUP_DELAY );
                //запуск таймера
                TIM_Cmd( LPC_TIM1, ENABLE );
               }
            else {
                //запуск измерений ADC
                ADC_StartCmd( LPC_ADC, ADC_START_NOW );
               }
            break;
        case TS_Y2_MEASURE:
            //цикл измерений по Y2
            x_temp += data;
            if ( ++samples >= TS_SAMPLES ) {
                //завершен цикл измерений по Y2
                ts_step = TS_INTR_SETUP_DLY; //завершение цикла измерений
                //переключаем пины для измерения по X1
                //Y1 = OUT:1 (P0.25) disable pulls
                GPIO_SetDir( PORT0, TS_Y1_PIN, GPIO_DIR_OUTPUT );
                PIN_Configure( PORT0, TS_Y1_PIN, GPIO_PIN_FUNCTION | IOCON_DIGITIAL_MODE | IOCON_MODE_PLAIN );
                GPIO_OutputValue( PORT0, TS_Y1_MASK, 1 );
                //Y2 = OUT:1 (P0.26)
                GPIO_SetDir( PORT0, TS_Y2_PIN, GPIO_DIR_OUTPUT );
                GPIO_OutputValue( PORT0, TS_Y2_MASK, 1 );
                //X1 = ADC Ch0 (P0.23)
                GPIO_SetDir( PORT0, TS_X1_PIN, GPIO_DIR_INPUT );
                PIN_Configure( PORT0, TS_X1_PIN, GPIO_ADC_FUNCTION | IOCON_ANALOG_MODE );
                //X2 = IN with pull down (P0.24) enable pull-down
                GPIO_SetDir( PORT0, TS_X2_PIN, GPIO_DIR_INPUT );
                PIN_Configure( PORT0, TS_X2_PIN, GPIO_PIN_FUNCTION | IOCON_DIGITIAL_MODE | IOCON_MODE_PULLDOWN );
                //Выбор канала ADC Ch2
                ADC_ChannelCmd( LPC_ADC, 2, DISABLE );
                ADC_ChannelCmd( LPC_ADC, 0, ENABLE );
                //Задержка перед измерением
                TIM_UpdateMatchValue( LPC_TIM1, 0, TS_SAMPLE_DELAY );
                //запуск таймера
                TIM_Cmd( LPC_TIM1, ENABLE );
                touch_temp = true; //окончание измерений
               }
            else {
                //запуск измерений ADC
                ADC_StartCmd( LPC_ADC, ADC_START_NOW );
               }
            break;
    }
}
