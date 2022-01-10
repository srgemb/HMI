
//*************************************************************************************************
//
// Управление обменом данными через порт UART
//
//*************************************************************************************************

#include <lpc407x_8x_177x_8x.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "cmsis_os2.h"
#include "rtx_config.h"

#include "driver_usart.h"
#include "rl_net.h"

#include "priority.h"
#include "uart.h"
#include "system.h"
#include "ring_uart.h"
#include "command.h"
#include "events.h"

#define CONS_RECV_BUFF      128         //размер приемного буфера

//*************************************************************************************************
// Внешние переменные
//*************************************************************************************************
extern ARM_DRIVER_USART   Driver_USART0;

//*************************************************************************************************
// Переменные с внешним доступом
//*************************************************************************************************
osSemaphoreId_t sem_wait = NULL;
osMessageQueueId_t uart_queue = NULL;

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
static uint8_t recv_ind;
static ARM_DRIVER_USART *USARTdrv;
static char recv_ch, recv_buffer[CONS_RECV_BUFF];

//*************************************************************************************************
// Атрибуты объектов RTOS
//*************************************************************************************************
const osThreadAttr_t uart_attr = {
    .name = "Uart",
    .stack_size = 256,
    .priority = osPriorityNormal,
 };

static const osSemaphoreAttr_t sem_attr = { .name = "Uart" };
static const osMessageQueueAttr_t mess_attr = { .name = "Uart" };

//*************************************************************************************************
// Прототипы локальных функций
//*************************************************************************************************
static void CallBackUsart( uint32_t event );

//*********************************************************************************************
// Инициализация консоли UART0/IRQ5
//*********************************************************************************************
void UARTInit( void ) {

    RingClear();
    UARTRecvClear();
    
    USARTdrv = &Driver_USART0;
    USARTdrv->Initialize( &CallBackUsart );
    NVIC_SetPriority( UART0_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), PRIORITY_USART, SUB_PRIORITY_USART0 ) );
    USARTdrv->PowerControl( ARM_POWER_FULL );
    USARTdrv->Control( ARM_USART_MODE_ASYNCHRONOUS | ARM_USART_DATA_BITS_8 | ARM_USART_PARITY_NONE | 
                       ARM_USART_STOP_BITS_1 | ARM_USART_FLOW_CONTROL_NONE, 115200 );
    USARTdrv->Control( ARM_USART_CONTROL_TX, 1 );
    USARTdrv->Control( ARM_USART_CONTROL_RX, 1 );    
    USARTdrv->Receive( &recv_ch, 1 );
    //создаем объекты RTOS
    sem_wait = osSemaphoreNew( 1, 0, &sem_attr );
    uart_queue = osMessageQueueNew( 16, sizeof( QUEUE_UART ), &mess_attr );
    osThreadNew( TaskUart, NULL, &uart_attr );
 }
 
//*************************************************************************************************
// Задача обработки сообщений для UART
//*************************************************************************************************
void TaskUart( void *pvParameters ) {

    QUEUE_UART msg;
    osStatus_t status;
  
    for ( ;; ) {
        //ожидание сообщения
        status = osMessageQueueGet( uart_queue, &msg, NULL, osWaitForever );
        if ( status == osOK ) {
            if ( msg.id_msg == MSG_UART_SEND_BUSY )
                UARTSendStart(); //попробуем запустит отправку еще раз
           }
        #if ( OS_ROBIN_ENABLE == 0 )
        osThreadYield();
        #endif
       }
 }
 
//*********************************************************************************************
// Обработчик событий последовательного порта
//*********************************************************************************************
static void CallBackUsart( uint32_t event ) {

    char ch;
    QUEUE_CMD mess;
   
    if ( event & ARM_USART_EVENT_RECEIVE_COMPLETE ) {
        //принят один байт
        if ( recv_ch == KEY_CR ) {
            //нажали клавишу ENTER - выполнение команды
            mess.socket = 0;
            mess.ptr_cmd = recv_buffer;
            mess.len = strlen( recv_buffer );
            mess.id_msg = MSG_COMMAND_UART; 
            osMessageQueuePut( command_queue, &mess, 0, 0 );
           }
        if ( recv_ch >= 32 ) {
            //принимаем только символы и знаки
            recv_ind += USARTdrv->GetRxCount();
            if ( recv_ind <= CONS_RECV_BUFF )
                recv_buffer[recv_ind-1] = recv_ch; //сохраним байт в буфере
            else UARTRecvClear(); //буфер переполнен, сбросим буфер
           }
        //повторная инициализация приема
        USARTdrv->Receive( &recv_ch, 1 );
        }
    if ( event & ARM_USART_EVENT_SEND_COMPLETE ) {
        //передача завершена
        if ( RingGetChar( &ch ) ) {
            USARTdrv->Send( &ch, 1 );
            RingCheckFree(); //снимем семафор, если в буфере много свободного места
           }
       }
 }
 
//*********************************************************************************************
// Очистка буфера приема
//*********************************************************************************************
void UARTRecvClear( void ) {

    recv_ind = 0;
    memset( recv_buffer, 0, sizeof( recv_buffer ) );
 }
 
//*************************************************************************************************
// Вывод строки в UART. Строка предварительно помещается в кольцевой буфер.
// char *str - указатель на строку для передач
//*************************************************************************************************
void UARTSendStr( char *str ) {

    uint16_t ind_str = 0, buf_len, str_len, tmp_len;
    
    //вывод в последовательный порт
    if ( str == NULL || !strlen( str ) )
        return;
    //проверка на превышение размера буфера
    str_len = strlen( str );
    buf_len = RingGetSize();
    if ( str_len > buf_len ) {
        //длина строки больше размера кольцевого буфера, 
        //отправлять будем по частям, по 1/2 от размера буфера
        tmp_len = buf_len / 2;
        while ( true ) {
            //запрос доступного места в кольцевом буфере
            if ( RingGetAdd( tmp_len ) ) {
                RingAddStrLen( str + ind_str, tmp_len ); //место есть
                if ( tmp_len < ( buf_len / 2 ) )
                    break; //передали последний блок
                ind_str += tmp_len;
                if ( ( str_len - tmp_len ) > tmp_len )
                    str_len -= tmp_len;             //остаток для передачи
                else tmp_len = str_len - tmp_len;   //размер последнего блока
               }
            else { 
                //ждем пока освободится семафор буфера
                osSemaphoreAcquire( sem_wait, osWaitForever );
               }
           }
        return;
       }
    //запрос доступного места в кольцевом буфере
    if ( RingGetAdd( strlen( str ) ) )
        RingAddStr( str ); //место есть
    else { 
        //ждем пока освободится семафор буфера
        osSemaphoreAcquire( sem_wait, osWaitForever );
       }
 }

//*************************************************************************************************
// Запуск вывода из кольцевого буфера в UART
// Если UART во время вызова функции занят - отправим сообщение в задачу TaskUart() для 
// повторного запуска передачи через некоторый интервал
//*************************************************************************************************
void UARTSendStart( void ) {

    char ch;
    ARM_USART_STATUS stat;
    QUEUE_UART msg_uart;
    
    stat = USARTdrv->GetStatus();
    if ( stat.tx_busy ) {
        //UART занят
        msg_uart.id_msg = MSG_UART_SEND_BUSY;
        osMessageQueuePut( uart_queue, &msg_uart, 0, 0 );
        return;
       }
    if ( RingGetChar( &ch ) )
        USARTdrv->Send( &ch, 1 );
 }

//*************************************************************************************************
// Возвращает адрес приемного буфера UART
//*************************************************************************************************
char *UARTBuffer( void ) {

    return recv_buffer;
 }

//*************************************************************************************************
// Возвращает размер приемного буфера UART
//*************************************************************************************************
uint16_t UARTBufferLen( void ) {

    return sizeof( recv_buffer );
 }
