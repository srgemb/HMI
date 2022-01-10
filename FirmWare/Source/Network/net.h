
#ifndef __NET_H
#define __NET_H

#include <stdint.h>

#include "rl_net.h"

//структура очереди для обработки запросов данных по TCP
typedef struct {
    uint16_t id_msg;                        //ID сообщения
    int32_t  socket;                        //ID сокета источника/получателя
} QUEUE_SOCKET;

//структура очереди для обработки данных по TCP
typedef struct {
    uint32_t id_msg;                        //ID сообщения
    int32_t  socket;                        //ID сокета источника/получателя
    uint8_t  *ptr_data;                     //указатель на буфер содержащий данные
    uint32_t len;                           //размер данных
} QUEUE_DATA;                            

void NetInit( void );
void NetConfig( void );
uint32_t NetInfo( char *buf, uint32_t buf_len );

void Prompt( int32_t socket );
uint32_t PingEcho( char *buff, char *host );
void TaskSocket1( void *pvParameters );
void TaskSocket2( void *pvParameters );
void TaskSocketCmd( void *pvParameters );
void SocketSendStr( int32_t sock_dest, char *buff, uint32_t len );
char *NetStateDesc( netStatus state );

#endif

