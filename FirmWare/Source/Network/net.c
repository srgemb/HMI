
//*************************************************************************************************
//
// Инициализация сетевого интерфейса
// Управление обменом данными через TCP сокеты
//
//*************************************************************************************************

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "cmsis_os2.h"
#include "rtx_config.h"
#include "rl_net.h"
#include "gui.h"

#include "system.h"
#include "rtc.h"
#include "uart.h"
#include "sdcard.h"
#include "net.h"
#include "eeprom.h"
#include "command.h"
#include "ftp.h"
#include "telnet.h"
#include "net_data.h"
#include "dev_param.h"
#include "message.h"
#include "events.h"

//*********************************************************************************************
// Локальные константы
//*********************************************************************************************
typedef enum {
    SEGMENT_NULL,                               //тип отправки не определен
    SEGMENT_FIRST,                              //много сегментная отправка
    SEGMENT_LAST                                //отправка единственного сегмента
 } TypeSegment;

typedef enum {
    SOCKET_OPEN,
    SOCKET_CONNECT
 } SocketStep;

#define MAX_DATA_LEN( len, max )        ( len < max ? len : max )

//*************************************************************************************************
// Внешние переменные
//*************************************************************************************************
extern CONFIG_HMI cfg_hmi;

//*************************************************************************************************
// Переменные с внешним доступом
//*************************************************************************************************
osMessageQueueId_t data_queue = NULL, cmd_queue = NULL;
osMessageQueueId_t sock1_queue = NULL, sock2_queue = NULL;

//*************************************************************************************************
// Локальные переменные
//*************************************************************************************************
static char ping_addr[40];
static char prompt[20];

static char * const state_net[] = { 
    "Operation succeeded.",
    "Process is busy.",
    "Unspecified error.",
    "Invalid parameter specified.",
    "Wrong state error.",
    "Driver error.",
    "Server error.",
    "User authentication failed.",
    "DNS host resolver failed.",
    "File not found or file r/w error.",
    "Operation timeout."
 };

//*************************************************************************************************
// Атрибуты объектов RTOS
//*************************************************************************************************
static const osThreadAttr_t attr_sock1 = {
    .name = "Socket1",
    .attr_bits = osThreadDetached,
    .stack_size = 512,
    .priority = osPriorityNormal
 };

static const osThreadAttr_t attr_sock2 = {
    .name = "Socket2",
    .attr_bits = osThreadDetached,
    .stack_size = 512,
    .priority = osPriorityNormal
 };

static const osThreadAttr_t attr_tcpdata = {
    .name = "TcpData",
    .attr_bits = osThreadDetached,
    .stack_size = 768,
    .priority = osPriorityNormal
 };

static const osThreadAttr_t attr_tcpcmd = {
    .name = "TcpCmnd",
    .attr_bits = osThreadDetached,
    .stack_size = 1024,
    .priority = osPriorityNormal
 };

static const osMessageQueueAttr_t attr_mes1 = { .name = "SocketCmd" };
static const osMessageQueueAttr_t attr_mes2 = { .name = "SocketData" };
static const osMessageQueueAttr_t attr_mes3 = { .name = "Socket1" };
static const osMessageQueueAttr_t attr_mes4 = { .name = "Socket2" };

//*********************************************************************************************
// Прототипы локальных функций
//*********************************************************************************************
static void ClearCRLF( char *str, uint32_t len );
static void ping_callback( netPing_Event event );
static uint32_t tcp_cb_func1( int32_t socket, netTCP_Event event, const NET_ADDR *addr, const uint8_t *buf, uint32_t len );
static uint32_t tcp_cb_func2( int32_t socket, netTCP_Event event, const NET_ADDR *addr, const uint8_t *buf, uint32_t len );

//*************************************************************************************************
// Инициализация сетевого интерфейса MAC, IP, MASK ...
//*************************************************************************************************
void NetInit( void ) {

    cmd_queue = osMessageQueueNew( 8, sizeof( QUEUE_CMD ), &attr_mes1 );
    data_queue = osMessageQueueNew( 16, sizeof( QUEUE_DATA ), &attr_mes2 );
    sock1_queue = osMessageQueueNew( 8, sizeof( QUEUE_SOCKET ), &attr_mes3 );
    sock2_queue = osMessageQueueNew( 8, sizeof( QUEUE_SOCKET ), &attr_mes4 );

    if ( netInitialize() == netOK ) {
        NetConfig();
        UARTSendStr( MessageNet( MSG_NET_INIT_OK ) );
        GetEthInfo( GetBuffer() );
        UARTSendStr( GetBuffer() );
        Telnet();
        FTPServer();
        osThreadNew( TaskSocket1, NULL, &attr_sock1 );
        osThreadNew( TaskSocket2, NULL, &attr_sock2 );
        osThreadNew( TaskSocketData, NULL, &attr_tcpdata );
        osThreadNew( TaskSocketCmd, NULL, &attr_tcpcmd );
       }
    else UARTSendStr( MessageNet( MSG_NET_INIT_ERR ) );
 }

//*************************************************************************************************
// Установка параметров сетевого интерфейса MAC, IP, MASK ...
//*************************************************************************************************
void NetConfig( void ) {

    char str[80];
    netStatus stat;
    uint8_t user_cid[] = { 0xFF, 1, 2, 3, 4, 0x33, 0x71 };

    netSYS_SetHostName( cfg_hmi.host_name );
    memset( prompt, 0x00, sizeof( prompt ) );
    strcat( prompt, cfg_hmi.host_name );
    strcat( prompt, ">" );
    stat = netIF_SetOption( NET_IF_CLASS_ETH, netIF_OptionMAC_Address, cfg_hmi.mac_addr, sizeof( cfg_hmi.mac_addr ) );
    sprintf( str, MessageNet( MSG_NET_SET_OPTION ), NetStateDesc( stat ) );
    UARTSendStr( str );
    if ( cfg_hmi.used_dhcp ) {
        stat = netDHCP_Disable( NET_IF_CLASS_ETH );
        sprintf( str, MessageNet( MSG_NET_DHCP_DISABLE ), NetStateDesc( stat ) );
        UARTSendStr( str );
        stat = netDHCP_SetOption( NET_IF_CLASS_ETH, NET_DHCP_OPTION_CLIENT_ID, user_cid, sizeof( user_cid ) );
        sprintf( str, MessageNet( MSG_NET_DHCP_OPTION ), NetStateDesc( stat ) );
        UARTSendStr( str );
        if ( stat == netOK ) {
            if ( netDHCP_Enable( NET_IF_CLASS_ETH ) == netOK ) {
                cfg_hmi.used_dhcp = false;
                netDHCP_Disable( NET_IF_CLASS_ETH );
               }
           }
        else cfg_hmi.used_dhcp = false;
       }
    if ( cfg_hmi.used_dhcp == false ) {
        netIF_SetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_Address, cfg_hmi.ip_addr, sizeof( cfg_hmi.ip_addr ) );
        netIF_SetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_SubnetMask, cfg_hmi.ip_mask, sizeof( cfg_hmi.ip_mask ) );
        netIF_SetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_DefaultGateway, cfg_hmi.ip_gate, sizeof( cfg_hmi.ip_gate ) );
        netIF_SetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_PrimaryDNS, cfg_hmi.ip_dns1, sizeof( cfg_hmi.ip_dns1 ) );
        netIF_SetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_SecondaryDNS, cfg_hmi.ip_dns2, sizeof( cfg_hmi.ip_dns2 ) );
       }
 }

//*************************************************************************************************
// Функция PING удаленного узла
//*************************************************************************************************
uint32_t PingEcho( char *buff, char *host ) {

    char *ptr;
    netStatus result;
    uint8_t ip4_addr[NET_ADDR_IP4_LEN];
    NET_ADDR addr;

    if ( buff == NULL )
        return 0;
    memset( ping_addr, 0x00, sizeof( ping_addr ) );
    strcpy( ping_addr, host );
    if ( strlen( host ) > sizeof( ping_addr ) ) {
        ptr += sprintf( ptr, "%s", MessageNet( MSG_NET_PIMG_EXCEED ) );
        return strlen( buff );
       }
    //определим тип адреса - IP/имя хоста
    if ( netIP_aton( host, NET_ADDR_IP4, &ip4_addr[0] ) ) {
        //PING по адресу
        addr.port = 0;
        addr.addr_type = NET_ADDR_IP4;
        memcpy( addr.addr, ip4_addr, sizeof( ip4_addr ) ); 
        if ( netPing_Echo( &addr, ping_callback ) == netOK )
            ptr += sprintf( ptr, MessageNet( MSG_NET_PING_START1 ), host );
        else ptr += sprintf( ptr, "%s", MessageNet( MSG_NET_PING_NOT_READY ) );
       }
    else {
        //PING по имени хоста
        ptr += sprintf( ptr, MessageNet( MSG_NET_PING_START2 ), host );
        result = netPing_EchoX( ping_addr, 0 );
        ptr += sprintf( ptr, "%s\r\n", NetStateDesc( result ) );
       }
    return strlen( buff );
 }
 
//*************************************************************************************************
// Функция обратного вызова для PingEcho
//*************************************************************************************************
static void ping_callback( netPing_Event event ) {

    char str[80];
    
    switch ( event ) {
        case netPing_EventSuccess:
            sprintf( str, MessageNet( MSG_NET_PING_NO_RESP1 ), ping_addr );
            UARTSendStr( str );
            break;
        case netPing_EventTimeout:
            sprintf( str, MessageNet( MSG_NET_PING_NO_RESP2 ), ping_addr );
            UARTSendStr( str );
            break;
       }
 }

//*********************************************************************************************
// Задача - создание сокетов (ip_port1) по мере подключения/отключения клиентов
//*********************************************************************************************
void TaskSocket1( void *pvParameters ) {

    char str[80];
    int32_t tcp_sock;
    osStatus_t stat_mess;
    QUEUE_SOCKET msg;
    netStatus stat_list;
    SocketStep step = SOCKET_OPEN;
    
    for ( ;; ) {
        if ( step == SOCKET_OPEN ) {
            //создаем сокет
            tcp_sock = netTCP_GetSocket( tcp_cb_func1 );
            if ( tcp_sock < 0 ) {
                sprintf( str, MessageNet( MSG_NET_SOCKET_ERROR ), cfg_hmi.ip_port1 );
                UARTSendStr( str );
                osThreadSuspend( osThreadGetId() ); //сокет не открыт, задачу останавливаем
               }
            sprintf( str, MessageNet( MSG_NET_SOCKET_OPEN ), cfg_hmi.ip_port1 );
            UARTSendStr( str );
            //открываем сокет для прослушивания
            stat_list = netTCP_Listen( tcp_sock, cfg_hmi.ip_port1 );
            if ( stat_list != netOK ) {
                //вывод ошибки
                sprintf( str, MessageNet( MSG_NET_SOCKET_LISTEN ), cfg_hmi.ip_port1, NetStateDesc( stat_list ) );
                UARTSendStr( str );
                osThreadSuspend( osThreadGetId() ); //задачу останавливаем
               }
            sprintf( str, MessageNet( MSG_NET_SOCKET_LISTEN ), cfg_hmi.ip_port1, NetStateDesc( stat_list ) );
            UARTSendStr( str );
            step = SOCKET_CONNECT;
           }
        if ( step == SOCKET_CONNECT ) {
            //ждем сообщения подтверждения подключение клиента от tcp_cb_func1()
            stat_mess = osMessageQueueGet( sock1_queue, &msg, NULL, osWaitForever );
            if ( stat_mess == osOK && msg.id_msg == MSG_SOCKET_CONNECT ) {
                //отправим приглашение клиенту
                Prompt( msg.socket );
                step = SOCKET_OPEN; //подключение выполнено, переход на создание нового сокета
               }
           }
        #if ( OS_ROBIN_ENABLE == 0 )
        osThreadYield();
        #endif
       }
 }

//*********************************************************************************************
// CallBack функция для обработки TCP socket соединения (ip_port1).
// return = 0 - соединения не подтверждаем
//        = 1 - подтвердить установку соедиения
//*********************************************************************************************
static uint32_t tcp_cb_func1( int32_t socket, netTCP_Event event, const NET_ADDR *addr, const uint8_t *buf, uint32_t len ) {

    char str[160], ip_ascii[40];
    QUEUE_SOCKET msg_conn;
    QUEUE_CMD msg_cmd;
    
    netIP_ntoa( NET_ADDR_IP4, addr->addr, ip_ascii, sizeof( ip_ascii ) );
    
    if ( event == netTCP_EventEstablished ) {
        //отправка приглашения клиенту
        msg_conn.socket = socket;
        msg_conn.id_msg = MSG_SOCKET_CONNECT;
        osMessageQueuePut( sock1_queue, &msg_conn, 0, 0 );
        sprintf( str, MessageNet( MSG_NET_PORT_CONN ), cfg_hmi.ip_port1, ip_ascii, addr->port, socket );
        UARTSendStr( str );
       }
    if ( event == netTCP_EventClosed || event == netTCP_EventAborted ) {
        //соединение закрыто или прервано, удаление сокеты выполняется автоматически
        if ( event == netTCP_EventClosed )
            sprintf( str, MessageNet( MSG_NET_PORT_CLOSE ), cfg_hmi.ip_port1, ip_ascii, addr->port, socket );
        if ( event == netTCP_EventAborted )
            sprintf( str, MessageNet( MSG_NET_PORT_ABORT ), cfg_hmi.ip_port1, ip_ascii, addr->port, socket );
        UARTSendStr( str );
       }
    if ( event == netTCP_EventACK ) {
        msg_cmd.socket = socket;           //ID сокета
        msg_cmd.id_msg = MSG_ACK_SEND_TCP; //источник данных
        msg_cmd.ptr_cmd = NULL;
        msg_cmd.len = 0;
        osMessageQueuePut( cmd_queue, &msg_cmd, 0, 0 );
       }
    if ( event == netTCP_EventData ) {
        //принят новый пакет данных в RAW формате
        ClearCRLF( (char *)buf, len );
        sprintf( str, MessageNet( MSG_NET_PORT_DATA1 ), cfg_hmi.ip_port1, ip_ascii, addr->port, len, buf );
        UARTSendStr( str );
        //отправим сообщение в TaskCommand() для последующей обработки
        msg_cmd.socket = socket;          //ID сокета
        msg_cmd.id_msg = MSG_COMMAND_TCP; //источник данных
        msg_cmd.ptr_cmd = (char *)buf;
        msg_cmd.len = strlen( (char *)buf );
        osMessageQueuePut( cmd_queue, &msg_cmd, 0, 0 );
       }
    return 1;
 }

//*********************************************************************************************
// Задача - создание сокетов (ip_port2) по мере подключения/отключения клиентов
//*********************************************************************************************
void TaskSocket2( void *pvParameters ) {

    char str[80];
    int32_t tcp_sock;
    osStatus_t stat_mess;
    QUEUE_SOCKET msg;
    netStatus stat_list;
    SocketStep step = SOCKET_OPEN;
    
    for ( ;; ) {
        if ( step == SOCKET_OPEN ) {
            //создаем сокет
            tcp_sock = netTCP_GetSocket( tcp_cb_func2 );
            if ( tcp_sock < 0 ) {
                sprintf( str, MessageNet( MSG_NET_SOCKET_ERROR ), cfg_hmi.ip_port2 );
                UARTSendStr( str );
                osThreadSuspend( osThreadGetId() ); //сокет не открыт, задачу останавливаем
               }
            sprintf( str, MessageNet( MSG_NET_SOCKET_OPEN ), cfg_hmi.ip_port2 );
            UARTSendStr( str );
            //открываем сокет для прослушивания
            stat_list = netTCP_Listen( tcp_sock, cfg_hmi.ip_port2 );
            if ( stat_list != netOK ) {
                //вывод ошибки
                sprintf( str, MessageNet( MSG_NET_SOCKET_LISTEN ), cfg_hmi.ip_port2, NetStateDesc( stat_list ) );
                UARTSendStr( str );
                osThreadSuspend( osThreadGetId() ); //задачу останавливаем
               }
            sprintf( str, MessageNet( MSG_NET_SOCKET_LISTEN ), cfg_hmi.ip_port2, NetStateDesc( stat_list ) );
            UARTSendStr( str );
            step = SOCKET_CONNECT;
           }
        if ( step == SOCKET_CONNECT ) {
            //ждем сообщения подтверждения подключение клиента от tcp_cb_func2()
            stat_mess = osMessageQueueGet( sock2_queue, &msg, NULL, osWaitForever );
            if ( stat_mess == osOK && msg.id_msg == MSG_SOCKET_CONNECT )
                step = SOCKET_OPEN; //подключение выполнено, переход на создание нового сокета
           }
        #if ( OS_ROBIN_ENABLE == 0 )
        osThreadYield();
        #endif
       }
 }

//*********************************************************************************************
// CallBack функция для обработки TCP socket соединения (ip_port2).
// return = 0 - соединения не подтверждаем
//        = 1 - подтвердить установку соединения
//*********************************************************************************************
static uint32_t tcp_cb_func2( int32_t socket, netTCP_Event event, const NET_ADDR *addr, const uint8_t *buf, uint32_t len ) {

    char str[100], ip_ascii[40];
    QUEUE_SOCKET msg_conn;
    QUEUE_DATA msg_data;
    
    netIP_ntoa( NET_ADDR_IP4, addr->addr, ip_ascii, sizeof( ip_ascii ) );
    
    if ( event == netTCP_EventEstablished ) {
        msg_conn.socket = socket;
        msg_conn.id_msg = MSG_SOCKET_CONNECT;
        osMessageQueuePut( sock2_queue, &msg_conn, 0, 0 );
        sprintf( str, MessageNet( MSG_NET_PORT_CONN ), cfg_hmi.ip_port2, ip_ascii, addr->port, socket );
        UARTSendStr( str );
       }
    if ( event == netTCP_EventClosed || event == netTCP_EventAborted ) {
        //соединение закрыто или прервано, удаление сокеты выполняется автоматически
        if ( event == netTCP_EventClosed )
            sprintf( str, MessageNet( MSG_NET_PORT_CLOSE ), cfg_hmi.ip_port2, ip_ascii, addr->port, socket );
        if ( event == netTCP_EventAborted )
            sprintf( str, MessageNet( MSG_NET_PORT_ABORT ), cfg_hmi.ip_port2, ip_ascii, addr->port, socket );
        UARTSendStr( str );
       }
    if ( event == netTCP_EventData ) {
        //принят новый блок данных в двоичном формате
        //sprintf( str, "Port: %u, Data recv from: %s port: %u size: %u\r\n", cfg_hmi.ip_port2, ip_ascii, addr->port, len );
        sprintf( str, MessageNet( MSG_NET_PORT_DATA2 ), cfg_hmi.ip_port2, ip_ascii, addr->port, len );
        UARTSendStr( str );
        msg_data.socket = socket;            //ID сокета
        msg_data.id_msg = MSG_TCP_DATA_RECV; //источник данных
        msg_data.ptr_data = (uint8_t *)buf;  //указатель на данные
        msg_data.len = len;                  //размер данных
        osMessageQueuePut( data_queue, &msg_data, 0, 0 );
       }
    return 1;
 }

//*********************************************************************************************
// Задача обработки команды по TCP и отправки результата
//*********************************************************************************************
void TaskSocketCmd( void *pvParameters ) {

    char *ptr_buff;
    QUEUE_CMD msg;
    uint8_t *sendbuf;
    osStatus_t stat_mess;
    uint32_t len, buf_len, offset;
    TypeSegment segment = SEGMENT_NULL;

    for ( ;; ) {
        stat_mess = osMessageQueueGet( cmd_queue, &msg, NULL, 0U );
        if ( stat_mess == osOK ) {
            //обработка сообщения
            if ( msg.id_msg == MSG_COMMAND_TCP && segment == SEGMENT_NULL ) {
                //ждем пока освободится блокировка
                osMutexAcquire( command_mutex, osWaitForever );
                //захват семафора
                osMutexAcquire( command_mutex, 0 );
                offset = 0;
                //указатель на буфер с результатом выполнения команды
                ptr_buff = GetBuffer();
                len = ExecCommand( SOURCE_CMD_TCP, msg.ptr_cmd, ptr_buff ); //выполнение команды
                sprintf( ptr_buff + len, MessageNet( MSG_NET_PROMPT ), cfg_hmi.host_name ); //добавляем приглашение
                buf_len = netTCP_GetMaxSegmentSize( msg.socket );
                while ( !netTCP_SendReady( msg.socket ) );
                //готовность к передаче
                sendbuf = netTCP_GetBuffer( buf_len );
                len = strlen( ptr_buff + offset );
                if ( len <= buf_len )
                    segment = SEGMENT_LAST; //отправка единственного сегмента
                else {
                    //результат весь не помещается, отправляем по частям
                    segment = SEGMENT_FIRST; //много сегментная отправка
                    len = buf_len;
                    offset += len; //сохраним смещение для следующего сегмента
                   }
                //сброс очереди
                osMessageQueueReset( cmd_queue );
                //отправка сегмента
                memcpy( sendbuf, ptr_buff, len );
                netTCP_Send( msg.socket, sendbuf, len );
               }
            if ( msg.id_msg == MSG_ACK_SEND_TCP && segment == SEGMENT_FIRST ) {
                //обработка подтверждения получения пакета
                while ( !netTCP_SendReady( msg.socket ) );
                sendbuf = netTCP_GetBuffer( buf_len );
                len = strlen( ptr_buff + offset );
                if ( len <= buf_len ) {
                    segment = SEGMENT_LAST; //отправка последнего сегмента
                    memcpy( sendbuf, ptr_buff + offset, len );
                    netTCP_Send( msg.socket, sendbuf, len );
                   }
                else {
                    //результат весь не помещается, отправляем по частям
                    memcpy( sendbuf, ptr_buff + offset, buf_len );
                    offset += buf_len; //сохраним смещение для следующего сегмента
                    netTCP_Send( msg.socket, sendbuf, buf_len );
                   }
               }
            if ( msg.id_msg == MSG_ACK_SEND_TCP && segment == SEGMENT_LAST ) {
                //передача завершена, освободим блокировку
                segment = SEGMENT_NULL; 
                osMutexRelease( command_mutex );
               }
           }
        #if ( OS_ROBIN_ENABLE == 0 )
        osThreadYield();
        #endif
       }
 }

//*********************************************************************************************
// Вывод приглашения, имя хоста + ">", в сокет
// int32_t socket - ID сокета
//*********************************************************************************************
void Prompt( int32_t socket ) {

    char msg[40];
    uint8_t *sendbuf;
    netStatus status;
    
    if ( socket < 0 )
        return;
    //отправим приглашение клиенту
    sendbuf = netTCP_GetBuffer( strlen( prompt ) );
    strcpy( (char *)sendbuf, prompt );
    while ( !netTCP_SendReady( socket ) );
    status = netTCP_Send( socket, sendbuf, strlen( prompt ) );
    if ( status != netOK ) {
        sprintf( msg, MessageNet( MSG_NET_SOCK_ERR_PROMPT ), socket, NetStateDesc( status ) );
        UARTSendStr( msg );
       }
 }

//*********************************************************************************************
// Возвращает рашифровку статуса выполнения сетевых функций
// netStatus state - ID сообщения
// return == NULL  - выход за пределы массива 
//        != NULL  - указатель на строку
//*********************************************************************************************
char *NetStateDesc( netStatus state ) {

    if ( state >= SIZE_ARRAY( state_net ) )
        return NULL; //выход за границы массива
    else return state_net[state];
 }

//*********************************************************************************************
// Убираем коды со значением меньше 32 начиная от позиции указанной в len
// char *str    - указатель на исходную строку
// uint32_t len - фактическая длина команды
// return       - указатель на результирующую строку
//*********************************************************************************************
static void ClearCRLF( char *str, uint32_t len ) {

    char *tmp;
    
    tmp = str + len;
    while ( tmp-- > str  ) {
        if ( *tmp < 32 )
            *tmp = 0x00;
        else break;
       }
 }
