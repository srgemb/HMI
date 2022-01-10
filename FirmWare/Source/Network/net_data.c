
//*************************************************************************************************
//
// Управление обменом двоичными данными через TCP сокет
//
//*************************************************************************************************

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "cmsis_os2.h"
#include "rtx_config.h"
#include "rl_net.h"

#include "rtc.h"
#include "net.h"
#include "crc16.h"
#include "net_data.h"
#include "uart.h"
#include "eeprom.h"
#include "events.h"
#include "message.h"
#include "ext_ram_addr.h"

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
static char net_data_send[2048] __attribute__ ((at(NETD_RAM_ADDRES)));

//*********************************************************************************************
// Прототипы локальных функций
//*********************************************************************************************
static void CreateData( void );
static uint16_t DumpHex( char *buff, uint8_t *data, uint16_t cnt );
static void SocketDataCheck( int32_t sock_src, const uint8_t *buf, uint32_t len );

//*********************************************************************************************
// Задача обработки и отправки пакетов данных по TCP
//*********************************************************************************************
void TaskSocketData( void *pvParameters ) {

    char str[80];
    int32_t maxlen;
    netStatus stat_send;
    uint8_t *sendbuf;
    osStatus_t status;
    QUEUE_DATA msg;
    
    for ( ;; ) {
        //ожидание сообщения
        status = osMessageQueueGet( data_queue, &msg, NULL, osWaitForever );
        if ( status == osOK ) {
            //обработка сообщения
            if ( msg.id_msg == MSG_TCP_DATA_RECV ) {
                //проверка принятого пакета
                SocketDataCheck( msg.socket, msg.ptr_data, msg.len );
                //ожидание готовности для передачи
                while ( !netTCP_SendReady( msg.socket ) );
                //формируем пакет данных
                CreateData();
                //максимальный пакет данных для указанного сокета
                maxlen = netTCP_GetMaxSegmentSize( msg.socket );
                //выделение блока памяти для пакета
                sendbuf = netTCP_GetBuffer( maxlen );
                if ( strlen( net_data_send ) > maxlen ) {
                    sprintf( str, "Data size %u exceeds buffer size %u\r\n", strlen( net_data_send ), maxlen );
                    UARTSendStr( str );
                   }
                else {
                    memcpy( sendbuf, (uint8_t *)&net_data_send, strlen( net_data_send ) );
                    stat_send = netTCP_Send( msg.socket, sendbuf, strlen( net_data_send ) );
                    if ( stat_send != netOK ) {
                        //что-то пошло не так, передача не состоялась
                        sprintf( str, "Send data to socket: %d, error: %s\r\n", msg.socket, NetStateDesc( stat_send ) );
                        UARTSendStr( str );
                       }
                   }
               }
           }
        #if ( OS_ROBIN_ENABLE == 0 )
        osThreadYield();
        #endif
       }
 }

//*********************************************************************************************
// Формирование пакета для передачи
//*********************************************************************************************
static void CreateData( void ) {

    char *ptr;
    
    ptr = (char *)net_data_send;
    memset( net_data_send, 0x00, sizeof( net_data_send ) );
    //данные контроллера заряда
    ptr += sprintf( ptr, "CHARGER: %u\r\n", sizeof( charger ) );
    ptr += DumpHex( ptr, (uint8_t *)&charger, sizeof( charger ) );
    //данные трекера
    ptr += sprintf( ptr, "TRC: %u\r\n", sizeof( tracker ) );
    ptr += DumpHex( ptr, (uint8_t *)&tracker, sizeof( tracker ) );
    //данные SPA
    ptr += sprintf( ptr, "SPA: %u\r\n", sizeof( sunpos ) );
    ptr += DumpHex( ptr, (uint8_t *)&sunpos, sizeof( sunpos ) );
    //данные контроллера MPPT
    ptr += sprintf( ptr, "MPPT: %u\r\n", sizeof( mppt ) );
    ptr += DumpHex( ptr, (uint8_t *)&mppt, sizeof( mppt ) );
    //данные монитора АКБ
    ptr += sprintf( ptr, "BATMON: %u\r\n", sizeof( batmon ) );
    ptr += DumpHex( ptr, (uint8_t *)&batmon, sizeof( batmon ) );
    //данные инвертора TS-1000-224
    ptr += sprintf( ptr, "INV1: %u\r\n", sizeof( inv1 ) );
    ptr += DumpHex( ptr, (uint8_t *)&inv1, sizeof( inv1 ) );
    //данные инвертора TS-3000-224
    ptr += sprintf( ptr, "INV2: %u\r\n", sizeof( inv2 ) );
    ptr += DumpHex( ptr, (uint8_t *)&inv2, sizeof( inv2 ) );
 }

//*************************************************************************************************
// Выводит блок данных в формате HEX дампа
// Вывод выполняется всего блока *data, по одной строке
// char *buff    - указатель на буфер для вывода данных
// uint8_t *data - указатель на блок данных для вывода
// uint16_t cnt  - кол-во байт для вывода
//*************************************************************************************************
static uint16_t DumpHex( char *buff, uint8_t *data, uint16_t cnt ) {

    char *ptr;
    uint16_t byte;
    
    ptr = buff;
    for ( byte = 0; byte < cnt; byte++ ) {
        ptr += sprintf( ptr, "%02X ", *( data + byte ) );
        if ( ( byte & 0x000F ) != 0x000F )
            continue; //продолжаем вывод
        ptr += sprintf( ptr, "%s", Message( CONS_MSG_CRLF ) ); 
       }
    ptr += sprintf( ptr, "%s", Message( CONS_MSG_CRLF ) );
    return strlen( buff );
 }

//*************************************************************************************************
// Функция проверки принятого пакета данных
// int32_t sock_src   - сокет отправитель пакета
// const uint8_t *buf - указатель на буфер пакета
// uint32_t len       - размер блока данных
//*************************************************************************************************
static void SocketDataCheck( int32_t sock_src, const uint8_t *buf, uint32_t len ) {

    return;
 }

