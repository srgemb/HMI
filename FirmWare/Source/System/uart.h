
#ifndef __UART_H
#define __UART_H

#include <stdint.h>
#include <stdbool.h>

#define KEY_CR              0x0D        //код '\r'
#define KEY_LF              0x0A        //код '\n'
#define KEY_ESC             0x1B        //код ESC
#define KEY_BS              0x08        //код Backspace

//структура очереди для проверки не отправленных данных в UART
typedef struct {
    uint16_t id_msg;                    //ID сообщения
} QUEUE_UART;


void UARTInit( void );
char *UARTBuffer( void );
void TaskUart( void *pvParameters );

void UARTRecvClear( void );
void UARTSendStr( char *str );
void UARTSendStrLen( char *str, uint16_t len );
void UARTSendStart( void );

#endif
