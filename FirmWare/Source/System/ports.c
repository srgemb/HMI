
//*************************************************************************************************
//
// Настройка вспомогательных портов
//
//*************************************************************************************************

#include <stdio.h>
#include <stdbool.h>

#include "pin_lpc40xx.h"
#include "gpio_lpc40xx.h"

#include "cmsis_os2.h"
#include "rtx_config.h"
#include "rl_net.h"

#include "lcd.h"
#include "main.h"
#include "ports.h"
#include "events.h"

//*************************************************************************************************
// Прототипы локальных функций
//*************************************************************************************************
static void TaskLed( void *pvParameters );

//*************************************************************************************************
// Атрибуты объектов RTOS
//*************************************************************************************************
static const osThreadAttr_t attr_led = {
    .name = "LedCheck", 
    .stack_size = 128,
    .priority = osPriorityNormal };
    
//*************************************************************************************************
// Инициализация портов
//*************************************************************************************************
void PortsInit( void ) {

    //контрольный индикатор
    GPIO_SetDir( PORT0, USB_UP_LED2, GPIO_DIR_OUTPUT );
    GPIO_PinWrite( PORT0, USB_UP_LED2, 0 );
    //подсветка LCD
    GPIO_SetDir( PORT2, LCD_ADJ_BRG, GPIO_DIR_OUTPUT );
    GPIO_PinWrite( PORT2, LCD_ADJ_BRG, 0 );
    //стартуем задачу контрольного индикатора
    osThreadNew( TaskLed, NULL, &attr_led );
 }
 
//*************************************************************************************************
// Задача, мигаем контрольным светодиодом USB_UP_LED
//*************************************************************************************************
static void TaskLed( void *pvParameters ) {

    for ( ;; ) {
        if ( GPIO_PinRead( PORT0, USB_UP_LED2 ) )
            GPIO_PinWrite( PORT0, USB_UP_LED2, 0 );
        else GPIO_PinWrite( PORT0, USB_UP_LED2, 1 );
        WDTReset();
		osDelay( 100 );
        #if ( OS_ROBIN_ENABLE == 0 )
        osThreadYield();
        #endif
       }
 }
