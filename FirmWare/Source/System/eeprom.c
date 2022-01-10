
//*************************************************************************************************
//
// Управление хранением параметров в EEPROM
//
//*************************************************************************************************

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "lpc_eeprom.h"

#include "crc16.h"
#include "rtc.h"
#include "md5.h"
#include "eeprom.h"
#include "uart.h"

//*********************************************************************************************
// Локальные константы
//*********************************************************************************************
#define CONFIG_PAGE             0       //начальная страница для хранения настроек HMI
//#define PARAM_PAGE              10      //начальная страница для хранения настроек GLB
//#define CONFIG_TEST_PAGE        20      //начальная страница для хранения имен файлов тестирования
#define CONFIG_LCD_PAGE         30      //начальная страница для хранения калибровочных коэффициентов

//кол-во блоков для хранения настроек HMI
#define CONFIG_BLOCK            (uint16_t)( ( sizeof( CONFIG_HMI ) / EEPROM_PAGE_SIZE ) + 1 )
//размер промежуточного блока для обработки структуры хранения настроек
#define CONFIG_SIZE             EEPROM_PAGE_SIZE * CONFIG_BLOCK

//кол-во блоков для хранения калибровочных коэффициентов
#define CONFIG_LCD_BLOCK        (uint16_t)( ( sizeof( CONFIG_LCD ) / EEPROM_PAGE_SIZE ) + 1 )
//размер промежуточного блока для обработки структуры хранения настроек
#define CONFIG_LCD_SIZE         EEPROM_PAGE_SIZE * CONFIG_LCD_BLOCK

//*********************************************************************************************
// Глобальные параметры
//*********************************************************************************************
CONFIG_HMI  cfg_hmi;
CONFIG_LCD  cfg_lcd;

//*********************************************************************************************
// Локальные переменные
//*********************************************************************************************
static MD5_CTX md5_ctx;

//*********************************************************************************************
// Прототипы локальных функций
//*********************************************************************************************
static void EepromClear( uint8_t page );

//*********************************************************************************************
// Инициализация EEPROM памяти
//*********************************************************************************************
void EepromInit( void ) {

    char buff[80];
    
    EEPROM_Init();
    EEPROM_PowerDown( DISABLE );
    //загрузим настройки HMI
    if ( ConfigHmiLoad( &cfg_hmi, LOAD_FROM_EEPROM ) == ERROR ) {
        //если КС не совпали установим параметры по умолчанию
        ConfigHmiLoad( &cfg_hmi, LOAD_FROM_DEFAULT );
        UARTSendStr( "Загрузка конфигурации по умолчанию.\r\n" );
       }
    //загрузим параметры калибровки
    if ( ConfigLcdLoad( &cfg_lcd, LOAD_FROM_EEPROM ) == ERROR ) {
        //если КС не совпали установим параметры по умолчанию
        ConfigLcdLoad( &cfg_lcd, LOAD_FROM_DEFAULT );
        UARTSendStr( "Загрузка параметров калибровки по умолчанию.\r\n" );
        ConfigLcdSave( &cfg_lcd, buff );
       }
 }

//*********************************************************************************************
// Сохранить значения настроек модуля HMI в EEPROM
// CONFIG *ptr_cfg - Указатель на структуру CONFIG
// return uint32_t - Размер данных в буфере (~60)
//*********************************************************************************************
uint32_t ConfigHmiSave( CONFIG_HMI *ptr_cfg, char *buff ) {

    //считаем КС данных
    ptr_cfg->crc_config = CalcCRC16( (uint8_t *)ptr_cfg, sizeof( CONFIG_HMI ) - 2 );
    EEPROM_Write( 0, CONFIG_PAGE, (uint8_t *)ptr_cfg, MODE_8_BIT, sizeof( CONFIG_HMI ) );
    if ( buff != NULL ) {
        sprintf( buff, "CONFIG_SIZE = %d CONFIG_BLOCKS = %d/%d\r\n", sizeof( CONFIG_HMI ), CONFIG_BLOCK, CONFIG_SIZE );
        return strlen( buff );
       }
    return 0;
 }

//*********************************************************************************************
// Сохранить значения настроек тестирования HMI в EEPROM
// CONFIG_LCD *ptr_lcd - Указатель на структуру CONFIG_LCD
// return uint32_t     - Размер данных в буфере (~60)
//*********************************************************************************************
uint32_t ConfigLcdSave( CONFIG_LCD *ptr_lcd, char *buff ) {

    //считаем КС данных
    ptr_lcd->crc = CalcCRC16( (uint8_t *)ptr_lcd, sizeof( CONFIG_LCD ) - 2 );
    EEPROM_Write( 0, CONFIG_LCD_PAGE, (uint8_t *)ptr_lcd, MODE_8_BIT, sizeof( CONFIG_LCD ) );
    if ( buff != NULL ) {
        sprintf( buff, "CONFIG_LCD_SIZE = %d CONFIG_LCD_BLOCKS = %d/%d\r\n", sizeof( CONFIG_LCD ), CONFIG_LCD_BLOCK, CONFIG_LCD_SIZE );
        return strlen( buff );
       }
    return 0;
 }

//*********************************************************************************************
// Загрузить значения настроек модуля HMI из EEPROM
// CONFIG *ptr_cfg  - Указатель на структуру CONFIG_HMI
// bool type        - Тип загрузки данных EEPROM/DEFAULT
// return = ERROR   - КС прочитанных данных не совпадает с рассчитанной, данные могут быть не достоверны
//        = SUCCESS - Данные прочитаны без ошибок
//*********************************************************************************************
Status ConfigHmiLoad( CONFIG_HMI *ptr_cfg, bool type ) {

    uint16_t crc;
    
    if ( type == LOAD_FROM_EEPROM ) {
        EEPROM_Read( 0, CONFIG_PAGE, (uint8_t *)ptr_cfg, MODE_8_BIT, sizeof( CONFIG_HMI ) );
        //считаем КС данных
        crc = CalcCRC16( (uint8_t *)ptr_cfg, sizeof( CONFIG_HMI ) - 2 );
        if ( ptr_cfg->crc_config != crc )
            return ERROR;
        else return SUCCESS;
       }
    else {
        //загрузка параметров по умолчанию
        memset( ptr_cfg, 0x00, sizeof( CONFIG_HMI ) );
        ptr_cfg->mac_addr[0] = 0x10;                    //МАС адрес
        ptr_cfg->mac_addr[1] = 0x30;
        ptr_cfg->mac_addr[2] = 0x6C;
        ptr_cfg->mac_addr[3] = 0xA2;
        ptr_cfg->mac_addr[4] = 0x46;
        ptr_cfg->mac_addr[5] = 0x50;
        ptr_cfg->used_dhcp = 0;                         //включить DHCP
        ptr_cfg->ip_addr[0] = 192;                      //IP адрес
        ptr_cfg->ip_addr[1] = 168;
        ptr_cfg->ip_addr[2] = 1;
        ptr_cfg->ip_addr[3] = 55;
        ptr_cfg->ip_mask[0] = 255;                      //маска сети
        ptr_cfg->ip_mask[1] = 255;
        ptr_cfg->ip_mask[2] = 255;
        ptr_cfg->ip_mask[3] = 0;
        ptr_cfg->ip_gate[0] = 192;                      //IP шлюз
        ptr_cfg->ip_gate[1] = 168;
        ptr_cfg->ip_gate[2] = 1;
        ptr_cfg->ip_gate[3] = 1;
        ptr_cfg->ip_dns1[0] = 192;                      //IP DNS1
        ptr_cfg->ip_dns1[1] = 168;
        ptr_cfg->ip_dns1[2] = 1;
        ptr_cfg->ip_dns1[3] = 1;
        ptr_cfg->ip_dns2[0] = 192;                      //IP DNS2
        ptr_cfg->ip_dns2[1] = 168;
        ptr_cfg->ip_dns2[2] = 1;
        ptr_cfg->ip_dns2[3] = 1;
        strcpy( ptr_cfg->host_name, "HMI" );            //имя в сети
        ptr_cfg->ip_port1 = 49100;                      //номер порта для связи по TCP
        ptr_cfg->ip_port2 = 49101;                      //номер порта для связи по TCP
        strcpy( ptr_cfg->user_name1, "admin" );         //имя пользователя для HTTP
        memset( ptr_cfg->passwd_hash1, 0x00, sizeof( ptr_cfg->passwd_hash1 ) );    //хэш-сумма пароля для HTTP
        strcpy( ptr_cfg->user_name2, "admin" );         //имя пользователя для FTP
        memset( ptr_cfg->passwd_hash2, 0x00, sizeof( ptr_cfg->passwd_hash2 ) );     //хэш-сумма пароля для FTP
        strcpy( ptr_cfg->user_name3, "admin" );         //имя пользователя для Telnet
        memset( ptr_cfg->passwd_hash3, 0x00, sizeof( ptr_cfg->passwd_hash3 ) );    //хэш-сумма пароля для Telnet
        strcpy( ptr_cfg->mail_addr, "mail.ru" );        //Server address (FQDN or IP address)
        ptr_cfg->mail_port = 0;                         //Server port number, can be 0
        ptr_cfg->mail_flags = 0;                        //Service control flags
        strcpy( ptr_cfg->mail_user, "account" );        //Account user name, can be NULL
        strcpy( ptr_cfg->mail_passwd, "password" );     //Account password, can be NULL
        strcpy( ptr_cfg->mail_from, "from" );           //Sender address, can be NULL
        strcpy( ptr_cfg->mail_to, "to" );               //Recipient(s), can be NULL
        strcpy( ptr_cfg->mail_cc, "cc" );               //Carbon copy recipient(s), can be NULL
        return SUCCESS;
       }
 }

//*********************************************************************************************
// Загрузить значения настроек модуля HMI из EEPROM
// CONFIG_LCD *ptr_cfg - Указатель на структуру CONFIG_LCD
// bool type           - Тип загрузки данных EEPROM/DEFAULT
// return = ERROR      - КС прочитанных данных не совпадает с рассчитанной, данные могут быть не достоверны
//        = SUCCESS    - Данные прочитаны без ошибок
//*********************************************************************************************
Status ConfigLcdLoad( CONFIG_LCD *ptr_lcd, bool type ) {

    uint16_t crc;
    
    if ( type == LOAD_FROM_EEPROM ) {
        EEPROM_Read( 0, CONFIG_LCD_PAGE, (uint8_t *)ptr_lcd, MODE_8_BIT, sizeof( CONFIG_LCD ) );
        //считаем КС данных
        crc = CalcCRC16( (uint8_t *)ptr_lcd, sizeof( CONFIG_LCD ) - 2 );
        if ( ptr_lcd->crc != crc )
            return ERROR;
        else return SUCCESS;
       }
    else {
        //загрузка параметров по умолчанию
        //параметры калибровки
        ptr_lcd->touch_x_min = 192;
        ptr_lcd->touch_x_max = 1855;
        ptr_lcd->touch_y_min = 299;
        ptr_lcd->touch_y_max = 1753;
        ptr_lcd->disp_off = 240;    //время отключения дисплея
        return SUCCESS;
       }
 }

//*************************************************************************************************
// Вывод информации о параметрах интерфейса LAN
// char *buff      - указатель на буфер для размещения данных
// return uint32_t - размер данных в буфере (~200 байт)
//*************************************************************************************************
uint32_t GetEthInfo( char *buff ) {

    char *ptr, ascii[40];
    uint8_t ip4_addr[NET_ADDR_IP4_LEN], mac_addr[NET_ADDR_ETH_LEN];
 
    if ( buff == NULL )
        return 0;
    ptr = buff;
    ptr += sprintf( ptr, "DCHP client: %s\r", cfg_hmi.used_dhcp ? "Enable" : "Disable" );

    netIF_GetOption( NET_IF_CLASS_ETH, netIF_OptionMAC_Address, mac_addr, sizeof( mac_addr ) );
    netMAC_ntoa( &mac_addr[0], ascii, sizeof( ascii ) );
    ptr += sprintf( ptr, "MAC address: %s\r\n", ascii );

    netIF_GetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_Address, ip4_addr, sizeof( ip4_addr ) );
    netIP_ntoa( NET_ADDR_IP4, ip4_addr, ascii, sizeof( ascii ) );
    ptr += sprintf( ptr, " IP address: %s\r\n", ascii );

    netIF_GetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_SubnetMask, ip4_addr, sizeof( ip4_addr ) );
    netIP_ntoa( NET_ADDR_IP4, ip4_addr, ascii, sizeof( ascii ) );
    ptr += sprintf( ptr, "Subnet mask: %s\r\n", ascii );

    netIF_GetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_DefaultGateway, ip4_addr, sizeof( ip4_addr ) );
    netIP_ntoa( NET_ADDR_IP4, ip4_addr, ascii, sizeof( ascii ) );
    ptr += sprintf( ptr, " IP Gateway: %s\r\n", ascii );

    netIF_GetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_PrimaryDNS, ip4_addr, sizeof( ip4_addr ) );
    netIP_ntoa( NET_ADDR_IP4, ip4_addr, ascii, sizeof( ascii ) );
    ptr += sprintf( ptr, "Primary DNS: %s\r\n", ascii );

    netIF_GetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_SecondaryDNS, ip4_addr, sizeof( ip4_addr ) );
    netIP_ntoa( NET_ADDR_IP4, ip4_addr, ascii, sizeof( ascii ) );
    ptr += sprintf( ptr, " Second DNS: %s\r\n", ascii );
    return strlen( buff );
 }

//*********************************************************************************************
// Вывод конфигурации контроллера 
// char *buff - указатель на буфер для размещения данных
// return uint32_t - размер данных в буфере (~550 байт)
//*********************************************************************************************
uint32_t GetNetInfo( char *buff ) {

    char *ptr, md5[40];
    
    ptr = buff;
    ptr += sprintf( ptr, "       Host name: %s\r\n", netSYS_GetHostName() );
    ptr += sprintf( ptr, "     TCP socket1: %u\r\n", cfg_hmi.ip_port1 );
    ptr += sprintf( ptr, "     TCP socket2: %u\r\n", cfg_hmi.ip_port2 );
    ptr += sprintf( ptr, "  HTTP user name: %s\r\n", cfg_hmi.user_name1 );
    ptr += sprintf( ptr, "   HTTP password: %s\r\n", MD5ToStr( cfg_hmi.passwd_hash1, md5, sizeof( md5 ) ) );
    ptr += sprintf( ptr, "   FTP user name: %s\r\n", cfg_hmi.user_name2 );
    ptr += sprintf( ptr, "    FTP password: %s\r\n", MD5ToStr( cfg_hmi.passwd_hash2, md5, sizeof( md5 ) ) );
    ptr += sprintf( ptr, "TELNET user name: %s\r\n", cfg_hmi.user_name3 );
    ptr += sprintf( ptr, " TELNET password: %s\r\n", MD5ToStr( cfg_hmi.passwd_hash3, md5, sizeof( md5 ) ) );
    ptr += sprintf( ptr, "   eMail address: %s\r\n", cfg_hmi.mail_addr );
    ptr += sprintf( ptr, "      eMail port: %u\r\n", cfg_hmi.mail_port );
    ptr += sprintf( ptr, "     eMail flags: %u\r\n", cfg_hmi.mail_flags );
    ptr += sprintf( ptr, " eMail user name: %s\r\n", cfg_hmi.mail_user );
    ptr += sprintf( ptr, "  eMail password: %s\r\n", cfg_hmi.mail_passwd );
    ptr += sprintf( ptr, "      eMail from: %s\r\n", cfg_hmi.mail_from );
    ptr += sprintf( ptr, "        eMail to: %s\r\n", cfg_hmi.mail_to );
    ptr += sprintf( ptr, "      eMail copy: %s\r\n", cfg_hmi.mail_cc );
    return strlen( buff );
 }

//*************************************************************************************************
// Вывод дампа памяти EEPROM по 1 блоку (64 байта)
// char *buff   - указатель ну буфер для размещения результата
// uint8_t page - номер страницы 0 - 63
// uint8_t mode - режим работы: чтение/стирание
//*************************************************************************************************
uint32_t Eeprom( char *buff, uint8_t page, uint8_t mode ) {

    char *ptr;
    uint8_t dat[EEPROM_PAGE_SIZE];
    uint16_t hexi, hexb = 0, dati;
    
    if ( buff == NULL )
        return 0;
    if ( page > 63 )
        return sprintf( buff, "Invalid parameter.\r\n" );
    ptr = buff;
    if ( mode == EEPROM_READ_PAGE ) {
        //вывод дампа памяти EEPROM
        ptr += sprintf( ptr, "Page: %d\r\n", page );
        //читаем 1 блок
        EEPROM_Read( 0, page, (uint8_t*)&dat, MODE_8_BIT, EEPROM_PAGE_SIZE );
        for ( hexi = 0, dati = 0; hexi < 4; hexi++ ) {
            //вывод адреса
            ptr += sprintf( ptr, "%04X: ", page * EEPROM_PAGE_SIZE + hexi * 16 );
            //вывод строки данных по 16 байт в формате HEX
            for ( hexb = 0; hexb < 16; hexb++ )
                ptr += sprintf( ptr, "%02X ", dat[dati++] );
            //вывод строки данных по 16 байт в формате "символ"
            ptr += sprintf( ptr, "  " );
            for ( hexb = 0; hexb < 16; hexb++ ) {
                if ( dat[dati+hexb-16] >= 32 )
                    ptr += sprintf( ptr, "%c", dat[dati+hexb-16] );
                else ptr += sprintf( ptr, "." );
               } 
            ptr += sprintf( ptr, "\r\n" );
           } 
        return strlen( buff );
       }
    if ( mode == EEPROM_CLEAR_PAGE ) {
        //очистка блока памяти в EEPROM
        EepromClear( page );
        return sprintf( buff, "Invalid parameter.\r\n" );
       }
    return sprintf( buff, "Invalid mode.\r\n" );
 }
 
//*********************************************************************************************
// Очистить блок данных и записать в EEPROM
// uint8_t page - номер страницы (блока) 0-63
//*********************************************************************************************
static void EepromClear( uint8_t page ) {

    uint8_t clear[EEPROM_PAGE_SIZE];
                    
    if ( page > 63 )
        return;
    memset( clear, 0x00, sizeof( clear ) );
    EEPROM_Write( 0, page, clear, MODE_8_BIT, EEPROM_PAGE_SIZE );
 }

//*********************************************************************************************
// Вывод значения MD5 в текстовом виде
// uint8_t *md5 - указатель на массив с значением MD5
// char *buffer - указатель на буфер для размещения текстового значения
//                размер буфера должен быть не менее 33 байт
// return       - указатель на буфер с результатом
//*********************************************************************************************
char *MD5ToStr( uint8_t *md5, char *buf, uint8_t buff_len ) {

    char str[10];
    uint8_t index;
    
    if ( buff_len < 34 )
        return NULL;
    memset( buf, 0x00, buff_len );
    for ( index = 0; index < 16; index++ ) {
        sprintf( str, "%02X", *( md5 + index ) );
        strcat( buf, str );
       }
    return buf;
 }

//*********************************************************************************************
// Расчет контрольной суммы по алгоритму MD5
// char *str    - указатель на строку
// uint8_t *md5 - указатель на рассчитанную сумму (16 байт)
//*********************************************************************************************
void CalcMD5( const char *str, uint8_t *md5 ) {

    MD5Init( &md5_ctx, 0 );
    MD5Update( &md5_ctx, (unsigned char *)str, strlen( str ) );
    MD5Final( &md5_ctx );
    memcpy( md5, md5_ctx.digest, 16 );
 }

//*********************************************************************************************
// Проверка контрольной суммы по алгоритму MD5
// char *str        - указатель на исходную текстовую строку
// uint8_t *md5     - указатель на сумму MD5 (16 байт)
// return = SUCCESS - контрольная сумма совпала
//          ERROR   - контрольная сумма не совпала
//*********************************************************************************************
Status CheckMD5( const char *str, uint8_t *md5 ) {

    uint8_t md5_calc[16];
    
    CalcMD5( str, md5_calc );
    if ( !memcmp( md5, md5_calc, 16 ) )
        return SUCCESS;
    else return ERROR;
 }
