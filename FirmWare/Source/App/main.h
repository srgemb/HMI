#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>
#include <stdbool.h>

//Маски источника сброса контроллера
#define RESET_POR           ( 1 << 0 )
#define RESET_EXTR          ( 1 << 1 )
#define RESET_WDTR          ( 1 << 2 )
#define RESET_BODR          ( 1 << 3 )
#define RESET_SYSRESET      ( 1 << 4 )
#define RESET_LOCKUP        ( 1 << 5 )

//Время перезапуска от WDT 2 сек
#define WDT_TIMEOUT         2000000

//расчет множителя значения таймеров для получения секундных интервалов
#define SEC_TO_TICK         (int)( (float)1000*(float)1000/(float)osKernelGetTickFreq() )

//расчет множителя значения таймеров для получения минутных интервалов
#define MIN_TO_TICK         (int)( (float)60*1000*(float)1000/(float)osKernelGetTickFreq() )

void WDTReset( void );

#endif
