
#ifndef __NET_DATA_H
#define __NET_DATA_H

#include <stdint.h>

void TaskSocketData( void *pvParameters );
void DataRecv( const uint8_t *buf, uint32_t len );

#endif

