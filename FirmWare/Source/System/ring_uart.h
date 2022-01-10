

#ifndef __RING_UART_H
#define __RING_UART_H

#include <stdint.h>
#include <stdbool.h>

void RingClear( void );
void RingAddStr( char *str );
void RingAddStrLen( char *str, uint16_t len );
uint16_t RingGetCount( void );
bool RingGetChar( char *ch );
uint16_t RingGetFree( void );
uint16_t RingGetSize( void );
void RingCheckFree( void );
bool RingGetAdd( uint16_t size );
uint8_t RingGetUsed( void );

void RingPtr( uint16_t *head, uint16_t *tail );

#endif
