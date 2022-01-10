
#ifndef __PRIORITY_H
#define __PRIORITY_H

//CMSIS NXP
#define PRIORITY_TIMER          1

#define SUB_PRIORITY_TIMER0     0
#define SUB_PRIORITY_TIMER1     1
#define SUB_PRIORITY_TIMER2     2

//CMSIS Keil
#define PRIORITY_USART          3

#define SUB_PRIORITY_USART0     0
#define SUB_PRIORITY_USART1     1
#define SUB_PRIORITY_USART2     2
#define SUB_PRIORITY_USART3     3
#define SUB_PRIORITY_USART4     4

//CMSIS Keil
#define PRIORITY_SSP            2

#define SUB_PRIORITY_SSP0       0
#define SUB_PRIORITY_SSP1       1
#define SUB_PRIORITY_SSP2       2
#define SUB_PRIORITY_CAN        3       //CAN1

//CMSIS NXP
#define PRIORITY_RTC            5
#define PRIORITY_GPIO           6

#endif

