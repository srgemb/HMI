
#ifndef __EEPROM_H
#define __EEPROM_H

#include <stdint.h>
#include <stdbool.h>

#include "rl_net.h"
#include "lpc_types.h"
#include "dev_data.h"

#define MD5_HASH_LENGTH         16
#define MAIL_PARAM_LENGTH       32

#define LOAD_FROM_EEPROM        true
#define LOAD_FROM_DEFAULT       false

#define EEPROM_READ_PAGE        0
#define EEPROM_CLEAR_PAGE       1

//*********************************************************************************************
// Структура для хранения настроек модуля HMI
//*********************************************************************************************
#pragma pack( push, 1 )

typedef struct {
    uint8_t  mac_addr[NET_ADDR_ETH_LEN];        //МАС адрес
    bool     used_dhcp;                         //включить DHCP
    uint8_t  ip_addr[NET_ADDR_IP4_LEN];         //IP адрес
    uint8_t  ip_mask[NET_ADDR_IP4_LEN];         //маска сети
    uint8_t  ip_gate[NET_ADDR_IP4_LEN];         //IP шлюз
    uint8_t  ip_dns1[NET_ADDR_IP4_LEN];         //IP DNS1
    uint8_t  ip_dns2[NET_ADDR_IP4_LEN];         //IP DNS2
    char     host_name[NET_HOSTNAME_LEN];       //имя в сети
    uint16_t ip_port1;                          //номер порта для связи по TCP
    uint16_t ip_port2;                          //номер порта для связи по TCP
    char     user_name1[NET_USERNAME_LEN];      //имя пользователя для HTTP
    uint8_t  passwd_hash1[MD5_HASH_LENGTH];     //хэш-сумма пароля для HTTP
    char     user_name2[NET_USERNAME_LEN];      //имя пользователя для FTP
    uint8_t  passwd_hash2[MD5_HASH_LENGTH];     //хэш-сумма пароля для FTP
    char     user_name3[NET_USERNAME_LEN];      //имя пользователя для TELNET
    uint8_t  passwd_hash3[MD5_HASH_LENGTH];     //хэш-сумма пароля для TELNET
    char     mail_addr[MAIL_PARAM_LENGTH];      //Server address (FQDN or IP address)
    uint16_t mail_port;                         //Server port number, can be 0
    uint16_t mail_flags;                        //Service control flags
    char     mail_user[MAIL_PARAM_LENGTH];      //Account user name, can be NULL
    char     mail_passwd[MAIL_PARAM_LENGTH];    //Account password, can be NULL
    char     mail_from[MAIL_PARAM_LENGTH];      //Sender address, can be NULL
    char     mail_to[MAIL_PARAM_LENGTH];        //Recipient(s), can be NULL
    char     mail_cc[MAIL_PARAM_LENGTH];        //Carbon copy recipient(s), can be NULL
    uint16_t crc_config;                        //контрольная сумма всего блока данных
 } CONFIG_HMI;

//структура для хранения параметров конфигурации модулей
//typedef struct {
//    GLB_PARAM param;                            //набор параметров
//    uint16_t  crc_param;                        //КС набор параметров
// } CONFIG_PAR;

//структура для хранения параметров тестирования
//typedef struct {
//    uint8_t  select;                            //номер выбранного файла
//    char     file_name[8][48];                  //имена файла
//    uint16_t crc_config;                        //контрольная сумма всего блока данных
// } CONFIG_TEST;

//структура для хранения параметров экрана
typedef struct {
    uint16_t touch_x_min;
    uint16_t touch_x_max;
    uint16_t touch_y_min;
    uint16_t touch_y_max;
    uint16_t disp_off;                          //время отключения дисплея
    uint16_t crc;
 } CONFIG_LCD;

#pragma pack( pop )

//*********************************************************************************************
//Функции управления
//*********************************************************************************************
void EepromInit( void );
uint32_t Eeprom( char *buff, uint8_t page, uint8_t mode );

Status ConfigHmiLoad( CONFIG_HMI *ptr_cfg, bool type );
//bool ConfigTestLoad( CONFIG_TEST *ptr_test, bool type );
Status ConfigLcdLoad( CONFIG_LCD *ptr_lcd, bool type );
//bool ConfigParLoad( CONFIG_PAR *ptr_par, bool type );

uint32_t ConfigHmiSave( CONFIG_HMI *ptr_cfg, char *buff );
//uint32_t ConfigTestSave( CONFIG_TEST *ptr_test, char *buff );
uint32_t ConfigLcdSave( CONFIG_LCD *ptr_lcd, char *buff );
//uint32_t ConfigParSave( CONFIG_PAR *ptr_par, char *buff );

uint32_t GetEthInfo( char *buff );
uint32_t GetNetInfo( char *buff );
uint32_t GetGlbPar( char *buff );

void CalcMD5( const char *str, uint8_t *md5 );
Status CheckMD5( const char *str, uint8_t *md5 );
char *MD5ToStr( uint8_t *md5, char *buf, uint8_t buff_len );

#endif
