
//*************************************************************************************************
//
// Массив текстовых сообщений
//
//*************************************************************************************************

#include <stdio.h>
#include <stdint.h>

#include "dev_param.h"

#include "message.h"

//*************************************************************************************************
// Сообщения консоли
//*************************************************************************************************
static char * const cons_msg[] = { 
    "\r\n>",                                                                        //CONS_MSG_PROMPT
    "OK\r\n",                                                                       //CONS_MSG_OK
    "\r\n",                                                                         //CONS_MSG_CRLF
    "Invalid command.\r\n",                                                         //CONS_MSG_ERR_CMND
    "Command will not be executed from the scheduler.\r\n",                         //CONS_MSG_ERR_NOJOB
    "Invalid parameter.\r\n",                                                       //CONS_MSG_ERR_PARAM
    "Parameter not specified.\r\n",                                                 //CONS_MSG_ERR_NOPARAM
    "There is no communication with the controller.\r\n",                           //CONS_MSG_ERR_SEND
    "File open error.\r\n",                                                         //CONS_MSG_ERR_FOPEN
    "File name not specified.\r\n",                                                 //CONS_MSG_ERR_FILE
    "Loading a job file: %s ...\r\n",                                               //CONS_MSG_FILE_JOBS
    "Exceeded line limit.\r\n",                                                     //CONS_MSG_ERR_LIMITSTR
    "Task not specified.\r\n",                                                      //CONS_MSG_ERR_JOB
    "There are no tasks in the file.\r\n",                                          //CONS_MSG_ERR_NO_JOB
    "Temporary file: jobs~ does not open.\r\n",                                     //CONS_MSG_ERR_JOB_OPEN
    "Temporary file: jobs~ not renamed.\r\n",                                       //CONS_MSG_ERR_JOB_RENAME
    "File: ",                                                                       //CONS_MSG_FILE
    "File name: %s size: %u bytes.\r\n",                                            //CONS_MSG_FILE1
    "%sFile size: %u read: %u bytes.",                                              //CONS_MSG_FILE2
    "File not deleted.\r\n",                                                        //CONS_MSG_NOT_DELETED
    "File: %s does not open.\r\n",                                                  //CONS_MSG_NOT_OPEN
    "Only %d days have passed from a full battery charge.\r\n",                     //CONS_MSG_CHARGE_DAY
    "====================================================================\r\n",     //CONS_MSG_JOB_HDR1
    "        ..........1.........2.........3.........4.........5.........\r\n",     //CONS_MSG_JOB_HDR2
    "        012345678901234567890123456789012345678901234567890123456789\r\n",     //CONS_MSG_JOB_HDR3
    "--------------------------------------------------------------------\r\n",     //CONS_MSG_JOB_HDR4
                                                                                    //CONS_MSG_HEADER
    "----------------------------------------------------------------------------\r\n",
    "\r\n   Name thread     Priority  State      Stack Unused\r\n",                 //CONS_MSG_TASK_HDR1
    "----------------------------------------------------\r\n",                     //CONS_MSG_TASK_HDR2
    "Application version ......... %s\r\n",                                         //CONS_MSG_APP_VER
    "Build version ............... %s %s\r\n",                                      //CONS_MSG_BUILD_VER
    "CPU clock ................... %s Hz\r\n",                                      //CONS_MSG_CPU_CLOCK
    "Clock periphery ............. %s Hz\r\n",                                      //CONS_MSG_CLOCK_PER
    "Clock of the kernel tick .... %s Hz\r\n",                                      //CONS_MSG_CLOCK_KERNEL
    "Clock of the system timer ... %s Hz\r\n",                                      //CONS_MSG_CLOCK_SYSTIMER
    "Kernel information .......... %s\r\n",                                         //CONS_MSG_KERNEL_INFO
    "Kernel version .............. %s\r\n",                                         //CONS_MSG_KERNEL_VER
    "Kernel API version .......... %s\r\n"                                          //CONS_MSG_KERNEL_API
 };

//*************************************************************************************************
// Сообщения функционала работы с SD картой
//*************************************************************************************************
static char * const sd_msg[] = { 
    "SD card not inserted.\r\n",                                                    //MSG_SD_NOT_INSERT
    "SD card not mounted.\r\n",                                                     //MSG_SD_NOT_MOUNT
    "Initialize File System ... ",                                                  //MSG_SD_INIT_SYSTEM
    "Mount the file system ... ",                                                   //MSG_SD_MOUNT
    "Check if valid drive is specified ... ",                                       //MSG_SD_CHECK_DRV
    "Error.\r\n",                                                                   //MSG_SD_ERROR
    "Block media access: %s %s\r\n",                                                //MSG_SD_BLC_ACCESS
    "IOC device control access: %s %s\r\n",                                         //MSG_SD_IOC_ACCESS
    "Unlock drive and allow media access to the upper layer: %s %s\r\n",            //MSG_SD_UNLOCK
    "Read volume label and serial number: %s\r\n",                                  //MSG_SD_VOLUME_READ
    "Volume in drive: %s\r\n",                                                      //MSG_SD_VOLUME
    "Volume serial number is: %X\r\n",                                              //MSG_SD_VOLUME_SER
    "Unmount drive: %s %s\r\n",                                                     //MSG_SD_UNMOUNT
    "Information about SDCard:\r\n",                                                //MSG_SD_ABOUT
    "    Manufacturer ID: %d (0x%.2X)\r\n",                                         //MSG_SD_ID_VENDOR
    "             OEM ID: %c%c\r\n",                                                //MSG_SD_OEMID
    "       Product name: %c%c%c%c%c\r\n",                                          //MSG_SD_NAMEPROD
    "   Product revision: %d.%d\r\n",                                               //MSG_SD_REVISION
    "      Serial number: 0x%X\r\n",                                                //MSG_SD_SERIAL2
    "Date of manufacture: %d/%.2d\r\n",                                             //MSG_SD_DATE
    "CID register not read.\r\n",                                                   //MSG_SD_CID_ERROR
    "Format drive: %s %s\r\n",                                                      //MSG_SD_FORMAT
    "File name not specified.\r\n",                                                 //MSG_FILE_NOT_SPECF
    "Source file name not specified.\r\n",                                          //MSG_FILE_SRC_NOT_SPECF
    "New file name not specified.\r\n",                                             //MSG_FILE_NEW_NOT_SPECF
    "List of files %s\\%s\r\n",                                                     //MSG_FILE_LIST_FILES
    "Function ffind(): %s\r\n",                                                     //MSG_FILE_FFIND
    "Deleting a file: %s %s\r\n",                                                   //MSG_FILE_DELETED
    "Removing a directory: %s %s\r\n",                                              //MSG_DIR_DELETED
    "Directory name not specified.\r\n",                                            //MSG_DIR_NOT_SPECF
    "Rename a file or directory: %s to: %s %s\r\n",                                 //MSG_FILE_RENAME
    "%-41s    <directory>",                                                         //MSG_DIR_INFO
    "No files.\r\n",                                                                //MSG_NO_FILES
    "%9d files  %21s bytes\r\n",                                                    //MSG_FILES_TOTAL
    "%9d directory   %21s byte aviable.\r\n",                                       //MSG_DIR_TOTAL
    "%56s byte aviable.\r\n",                                                       //MSG_DIR_FREE
    "<EOF>\r\n"                                                                     //MSG_FILE_EOF
 };

//*************************************************************************************************
// Сообщения функционала работы с электронным диском
//*************************************************************************************************
static char * const rd_msg[] = { 
    "                                  The size The address The address\r\n"        //MSG_RDISK_HDR1
    "  File name                       in bytes    header       file\r\n"
    "------------------------------------------------------------------\r\n",
    "------------------------------------------------------------------\r\n",       //MSG_RDISK_HDR2
    "RAM DISK INIT: total %s bytes.\r\nRAM DISK: free %s bytes.\r\n",               //MSG_RDISK_INIT
    "SD card not inserted.\r\n",                                                    //MSG_RDISK_NO_SDCARD
    "File: %s not found.\r\n",                                                      //MSG_RDISK_NOT_FOUND
    "File: %s no free blocks.\r\n",                                                 //MSG_RDISK_NO_FREE
    "File: %s no upload space.\r\n",                                                //MSG_RDISK_NOT_SPACE
    "File: %s upload error\r\n",                                                    //MSG_RDISK_UPL_ERROR
    "File: %s upload: %s bytes.\r\n",                                               //MSG_RDISK_UPLOAD
    "There are no files. Available: %s bytes.\r\n",                                 //MSG_RDISK_NO_FILES
    "Files: %u used: %s available: %s bytes.\r\n",                                  //MSG_RDISK_FILES
    "Violation of data integrity, address: 0x%08X\r\n",                             //MSG_RDISK_VIOLATION
    "RAM DISK: total %u bytes.\r\nRAM DISK: free %s bytes.\r\nFiles: %u\r\n"        //MSG_RDISK_TOT_FREE
 };

//*************************************************************************************************
// Сообщения для файлов логирования событий
//*************************************************************************************************
static char * const log_msg[] = { 
    // АВР
    "The load is connected to the main AC line.",                                   //LOG_MSG_PWR_AC
    "The load is connected to the inverters.",                                      //LOG_MSG_PWR_INV
    "Main AC mains restored.",                                                      //LOG_MSG_AC_RESTORE
    "AC mains disconnected.",                                                       //LOG_MSG_AC_DISCON
    // control SOC
    "Low battery charge, recharging from the mains.",                               //LOG_MSG_SOC_BAT_CHRG_AC
    "Low battery charge, recharging from a generator.",                             //LOG_MSG_SOC_BAT_CHRG_GE
    "Low battery charge, generator starting.",                                      //LOG_MSG_SOC_GEN_START
    "Recharging from the mains.",                                                   //LOG_MSG_SOC_CHRG_AC
    "Recharging from a generator.",                                                 //LOG_MSG_SOC_CHRG_GEN
    // panels
    "Solar panels are connected.",                                                  //LOG_MSG_PV_ON
    "Solar panels are off.",                                                        //LOG_MSG_PV_OFF
    "Parallel connection of panels.",                                               //LOG_MSG_PV_PAR
    "Series connection of panels.",                                                 //LOG_MSG_PV_SER
    "Error turning on panels.",                                                     //LOG_MSG_PV_ONERR
    "Panel disconnection error.",                                                   //LOG_MSG_PV_OFFERR
    // tracker
    "Actuator control is on.",                                                      //LOG_MSG_ACT_ON
    "Actuator control off.",                                                        //LOG_MSG_ACT_OFF
    "Command mode is on.",                                                          //LOG_MSG_TRC_CMD
    "Disabled command mode.",                                                       //LOG_MSG_TRC_SUN
    // charge controller
    "Management. Turn on the charge.",                                              //LOG_MSG_CHRG_START
    "Management. Turn off the charge.",                                             //LOG_MSG_CHRG_STOP
    "Recovery. Turn on the charge.",                                                //LOG_MSG_CHRG_RSTART
    "Battery charging completed by current.",                                       //LOG_MSG_CHRG_END1
    "Battery charging completed.",                                                  //LOG_MSG_CHRG_END2
    // inverters
    "TS-1000-224",                                                                  //LOG_MSG_INV1
    "TS-3000-224",                                                                  //LOG_MSG_INV2
    "TS-1000-224 ... OK.",                                                          //LOG_MSG_INV1_OK
    "TS-3000-224 ... OK.",                                                          //LOG_MSG_INV2_OK
    "Power on TS-1000-224.",                                                        //LOG_MSG_INV1_TURN_ON
    "Shutting down TS-1000-224.",                                                   //LOG_MSG_INV1_SHDN
    "Power on TS-3000-224.",                                                        //LOG_MSG_INV2_TURN_ON
    "Shutting down TS-3000-224.",                                                   //LOG_MSG_INV2_SHDN
    "TS-1000-224 is on.",                                                           //LOG_MSG_INV1_ON
    "TS-3000-224 is on.",                                                           //LOG_MSG_INV2_ON
    "Manual activation of TS-1000-224.",                                            //LOG_MSG_INV1_MAN_ON
    "Manual shutdown of TS-1000-224.",                                              //LOG_MSG_INV1_MAN_OFF
    "Manual activation of TS-3000-224.",                                            //LOG_MSG_INV2_MAN_ON
    "Manual shutdown of TS-3000-224.",                                              //LOG_MSG_INV2_MAN_OFF
    "TS-1000-224 is off.",                                                          //LOG_MSG_INV1_OFF
    "TS-3000-224 is off.",                                                          //LOG_MSG_INV2_OFF
    "Contactor control off, check inverter.",                                       //LOG_MSG_INV_CTRL_DC_OFF
    "Contactor control on.",                                                        //LOG_MSG_INV_CTRL_DC_ON
    "Off remotely, turn it on programmatically.",                                   //LOG_MSG_INV_RMTOFF_SWON
    "Contactor on.",                                                                //LOG_MSG_INV_DC_ON
    "Turn off the contactor.",                                                      //LOG_MSG_INV_DC_SHDN
    "Reading configuration.",                                                       //LOG_MSG_INV_RD_CONF
    "Reading state.",                                                               //LOG_MSG_INV_RD_STAT
    "Turn off the command.",                                                        //LOG_MSG_INV_OFF_CMD
    "Off remotely.",                                                                //LOG_MSG_INV_OFF_RMT
    "Inverter off, Contactor not off.",                                             //LOG_MSG_INV_DC_OFF_ERR
    "Load on inverter."                                                             //LOG_MSG_INV_POWER_AC
 };                                                               

//*************************************************************************************************
// Сообщения функционала FTP сервера
//*************************************************************************************************
static char * const ftp_msg[] = { 
    "Start the FTP server: %s\r\n",                                                 //MSG_FTP_START
    "FTP server accept authentication from: %u.%u.%u.%u\r\n",                       //MSG_FTP_ACCEPT
    "FTP user name: \"%s\" %s\r\n",                                                 //MSG_FTP_USER
    "FTP user ID: %d, password: \"%s\" %s\r\n",                                     //MSG_FTP_PASSWD
    "FTP user ID: %d, file name: %s, file access: 0x%02X %s\r\n",                   //MSG_FTP_FILE_NAME
    "The ftp_server.log file is not open.\r\n"                                      //MSG_FTP_LOG_ERROR
 };

//*************************************************************************************************
// Сообщения функционала FTP сервера
//*************************************************************************************************
static char * const net_msg[] = { 
    "\r\n%s>",                                                                      //MSG_NET_PROMPT
    "Network Core initialized successfully.\r\n",                                   //MSG_NET_INIT_OK
    "Failed to initialize the Network Core.\r\n",                                   //MSG_NET_INIT_ERR
    "Set interface option: %s\r\n",                                                 //MSG_NET_SET_OPTION
    "Disable DHCP: %s\r\n",                                                         //MSG_NET_DHCP_DISABLE
    "Set DHCP option: %s\r\n",                                                      //MSG_NET_DHCP_OPTION
    "Hostname exceeded length.\r\n",                                                //MSG_NET_PIMG_EXCEED
    "Ping host: %s started.\r\n",                                                   //MSG_NET_PING_START1
    "Ping not ready or bad parameters.\r\n",                                        //MSG_NET_PING_NOT_READY
    "Ping host: %s started ... ",                                                   //MSG_NET_PING_START2
    "Remote host: %s responded to ping.\r\n",                                       //MSG_NET_PING_NO_RESP1
    "Remote host: %s, ping timeout, no response.\r\n",                              //MSG_NET_PING_NO_RESP2
    "Allocate a free TCP socket ID: %u ... OK.\r\n",                                //MSG_NET_SOCKET_OPEN
    "Open TCP socket ID: %u for incoming connection: %s\r\n",                       //MSG_NET_SOCKET_LISTEN
    "Get socket ID: %u failed.\r\n",                                                //MSG_NET_SOCKET_ERROR
    "Port: %u, Connect from: %s port: %u socket ID: %d\r\n",                        //MSG_NET_PORT_CONN
    "Port: %u, Close connect from: %s port: %u socket ID: %u\r\n",                  //MSG_NET_PORT_CLOSE
    "Port: %u, Abort connect from: %s port: %u socket ID: %u\r\n",                  //MSG_NET_PORT_ABORT
    "Port: %u, Data recv from: %s port: %u size: %u \"%s\"\r\n",                    //MSG_NET_PORT_DATA1
    "Port: %u, Data recv from: %s port: %u size: %u\r\n",                           //MSG_NET_PORT_DATA2
    "Prompt error send, socket: %u %s\r\n",                                         //MSG_NET_SOCK_ERR_PROMPT
    "Start the Telnet server: %s\r\n",                                              //MSG_TELNET_START
    "\r\nWelcome to the %s telnet server.\r\n",                                     //MSG_TELNET_WELCOME
    "\r\nCMD> ",                                                                    //MSG_TELNET_PROMPT
    "\r\n%s embedded telnet server, please login ...",                              //MSG_TELNET_LOGIN
    "\r\nUsername: ",                                                               //MSG_TELNET_USER
    "\r\nPassword: ",                                                               //MSG_TELNET_PASSWD
    "\r\nLogin incorrect.",                                                         //MSG_TELNET_LOGIN_ERR
    "Login incorrect.",                                                             //MSG_TELNET_LOGIN_ERR1
    "\r\nLogin timeout.\r\n",                                                       //MSG_TELNET_TIMEOUT
    "Telnet session ended.",                                                        //MSG_TELNET_END
    "Enter command !\r\n",                                                          //MSG_TELNET_ENT_COMMAND
    "Telnet accept authentication from: %u.%u.%u.%u",                               //MSG_TELNET_ACCEPT
    "Telnet user name: \"%s\" %s",                                                  //MSG_TELNET_USER_OUT
    "Telnet user ID: %d, password: \"%s\" %s",                                      //MSG_TELNET_USER_PASS
    "TelnetLog: SD card not inserted. #",                                           //MSG_TELNET_TELLOG_SD
    "The telnet_server.log file is not open. #",                                    //MSG_TELNET_TELLOG_LOG
    "password accepted.",                                                           //MSG_TELNET_PASS_ACCEPT
    "invalid password.",                                                            //MSG_TELNET_PASS_INVALID
    "access is allowed.",                                                           //MSG_NET_USER_ACCEPT
    "access denied.",                                                               //MSG_NET_USER_DENIED
    "password accepted.",                                                           //MSG_NET_PASS_ACCEPT
    "invalid password."                                                             //MSG_NET_PASS_DENIED
 };

//*************************************************************************************************
// Сообщения функционала работы с электронным диском
//*************************************************************************************************
static char * const gui_msg[] = { 
    "Загрузка графических файлов ... %d%%, ошибок: %u",                             //MSG_GUI_LOAD_FILE
    "\r\nГрафические файлы загружены.\r\n",                                         //MSG_GUI_LOAD_OK
    "Инициализация экранных форм ...\r\n",                                          //MSG_GUI_INIT_WIN
    "error.\r\n",                                                                   //MSG_GUI_ERROR
    "OK.\r\n",                                                                      //MSG_GUI_OK
    "Применить значения параметров без сохранения в EEPROM ?",                      //MSG_GUI_APPLY
    "Сохранить значения параметров в EEPROM ?",                                     //MSG_GUI_EEPROM
    "Функционал находится в стадии разработки.",                                    //MSG_GUI_UNDER_DEV
    "Create window 0 ... ",                                                         //MSG_GUI_INIT_WIN0
    "Create window 1 ... ",                                                         //MSG_GUI_INIT_WIN1
    "Create window 2 ... ",                                                         //MSG_GUI_INIT_WIN2
    "TaskWindow start.\r\n",                                                        //MSG_GUI_TASK_STARTUP
    "Выполнен пересчет угла в градусах в миллиметры позиционирования.",             //MSG_GUI_CALC_DEG_MM
    "Введенное значение %s выходит за пределы допустимого диапазона: 0 - 900 мм",   //MSG_GUI_ERR_RANGE_MM
    "Введенное значение %s выходит за пределы допустимого диапазона: 0 - 90 °",     //MSG_GUI_ERR_RANGE_DEG1
    "Введенное значение %s выходит за пределы допустимого диапазона: %.2f - %.2f °",//MSG_GUI_ERR_RANGE_DEG2
    "Ошибка при проверки формата. Введенное значение не соответствует требованиям.",//MSG_GUI_FORMAT_ERR
    "Введенное значение %s выходит за пределы установленного диапазона: %d ... %d", //MSG_GUI_ERR_RANGE
    "Значения параметров изменены."                                                 //MSG_GUI_VALUE_CHANGE
 };

//*************************************************************************************************
// Информация о командах
//*************************************************************************************************
const static char help[] = {
    "TIME [hh:mi[:ss]]             - read/set time\r\n"
    "DATE [dd.mm.yyyy]             - read/set date\r\n"
    "DTIME                         - date-time in full format\r\n"
    "RTC HMI/CTRL                  - clock/calendar synchronization HMI->CTRL/CTRL->HMI\r\n"
    "\r\n"
    "SDCARD                        - SD card information\r\n"
    "SDCARD FORMAT                 - format SD card\r\n"
    "SDCARD MOUNT                  - mount SD card\r\n"
    "SDCARD UMOUNT                 - unmount SD card\r\n"
    "SDCARD DIR                    - displays a list of downloaded files on the SD card\r\n"
    "\r\n"
    "RAMDIR                        - displays a list of downloaded files on the electronic disk\r\n"
    "\r\n"
    "JOBS                          - reloading the job file\r\n"
    "TYPE                          - outputting a file in text mode\r\n"
    "EEPROM N                      - output the page \"N\"of EEPROM data as a HEX dump\r\n"
    "EEPROM CLR N                  - clearing the page \"N\"EEPROM data\r\n"
    "TASK                          - Displaying a list of tasks\r\n"
    "SYSTEM                        - Displaying firmware versions\r\n"
    "LINK                          - Communication status on the CAN interface\r\n"
    "\r\n"
    "CONFIG                        - display network parameters\r\n"
    "CONFIG HTTP_USER user         - username for HTTP\r\n"
    "CONFIG HTTP_PASS pass         - user password for HTTP\r\n"
    "CONFIG FTP_USER user          - FTP username\r\n"
    "CONFIG FTP_PASS pass          - FTP user password\r\n"
    "CONFIG TELNET_USER user       - username for TELNET\r\n"
    "CONFIG TELNET_PASS pass       - user password for TELNET\r\n"
    "CONFIG MAIL addr              - name (address) of the mail server\r\n"
    "CONFIG MAIL_PORT port         - port for connection\r\n"
    "CONFIG MAIL_USER user         - username\r\n"
    "CONFIG MAIL_PASS pass         - user password\r\n"
    "CONFIG MAIL_FROM from         - sender name\r\n"
    "CONFIG MAIL_TO to             - to\r\n"
    "CONFIG MAIL_CC copy           - copy to\r\n"
    "CONFIG DISPLAY_OFF time       - display off time\r\n"
    "CONFIG SAVE                   - save parameters\r\n"
    "\r\n"
    "LAN MAC XX-XX-XX-XX-XX-XX     - setting the MAC address\r\n"
    "LAN IP XXX.XXX.XXX.XXX        - setting the IP address\r\n"
    "LAN MASK XXX.XXX.XXX.XXX      - setting the subnet mask\r\n"
    "LAN GATE XXX.XXX.XXX.XXX      - setting the gateway address\r\n"
    "LAN DNS1 XXX.XXX.XXX.XXX      - setting the address of the primary DNS server\r\n"
    "LAN DNS2 XXX.XXX.XXX.XXX      - setting the address of the secondary DNS server\r\n"
    "LAN HOST XXXXXXXXXXXXXXX      - setting hostname\r\n"
    "LAN PORT [1/2] 1024-65535     - setting the port number\r\n"
    "LAN SAVE                      - saving LAN parameters to EEPROM\r\n"
    "PING IP/HOSTNAME              - check the connection to the remote host by IP or hostname\r\n"
    "\r\n"
    "INV 1/2 on/off                - inverter control\r\n"
    "ALT [ac/dc]                   - control of the AVR unit\r\n"
    "PV [on/off/par/ser]           - solar panel commutation control\r\n"
    "CHARGE [0/2/3/8 [days]]       - control of charging from the AC network, days - start if from the last charge > N days\r\n"
    "TRC [on/off]                  - control of the power relay of the actuator circuits\r\n"
    "TRC [ver/hor value]           - tracker positioning control, integer value in mm, with decimal point in degrees\r\n"
    "TRC [save/rest/stop]          - restore position in EEPROM\r\n"
    "TRC [reset/cmd/int/stop/init] - restart, on command mode, on internal control, interrupt positioning, transfer to the initial position\r\n"
    "GEN [start/stop/test]         - generator control\r\n"
    "RES 0/1/2/3 on/off/pulse      - control of additional relays\r\n"
    "EXT up/dn/lf/rt on/off/pulse  - control of additional outputs\r\n"
    "VOICE 1...N [name_voice]      - play voice message\r\n" \
    "VOLUME 0...N                  - setting the volume level of the voice message\r\n"
    "SOUND 1...N [name]            - play a sound message\r\n"
    "\r\n"
    "RESET                         - restart the controller\r\n"
    "SCR 0...255                   - save screen to file: screen_NNN.bmp\r\n"
 };
 
//*************************************************************************************************
// Возвращает указатель на строку текстового сообщения
// ConsMesage id_mess - ID сообщения
// return             - указатель на строку
//*************************************************************************************************
char *Message( ConsMessage id_mess ) {

    if ( id_mess < SIZE_ARRAY( cons_msg ) )
        return cons_msg[id_mess];
    else return NULL;
 }

//*************************************************************************************************
// Возвращает указатель на строку текстового сообщения
// LogMessId id_mess - ID сообщения
// return            - указатель на строку
//*************************************************************************************************
char *MessageLog( LogMessId id_mess ) {

    if ( id_mess < SIZE_ARRAY( log_msg ) )
        return log_msg[id_mess];
    else return NULL;
 }

//*************************************************************************************************
// Возвращает указатель на строку текстового сообщения
// LogMessId id_mess - ID сообщения
// return            - указатель на строку
//*************************************************************************************************
char *MessageSd( SdMessId id_mess ) {

    if ( id_mess < SIZE_ARRAY( sd_msg ) )
        return sd_msg[id_mess];
    else return NULL;
 }

//*************************************************************************************************
// Возвращает указатель на строку текстового сообщения
// LogMessId id_mess - ID сообщения
// return            - указатель на строку
//*************************************************************************************************
char *MessageRdisk( RdiskMessId id_mess ) {

    if ( id_mess < SIZE_ARRAY( rd_msg ) )
        return rd_msg[id_mess];
    else return NULL;
 }

//*************************************************************************************************
// Возвращает указатель на строку с описанием команд
//*************************************************************************************************
char *MessageHelp( void ) {

    return (char *)help;
 }

//*************************************************************************************************
// Возвращает указатель на строку текстового сообщения
// FtpMessId id_mess - ID сообщения
// return            - указатель на строку
//*************************************************************************************************
char *MessageFtp( FtpMessId id_mess ) {

    if ( id_mess < SIZE_ARRAY( ftp_msg ) )
        return ftp_msg[id_mess];
    else return NULL;
 }

//*************************************************************************************************
// Возвращает указатель на строку текстового сообщения
// NetMessId id_mess - ID сообщения
// return            - указатель на строку
//*************************************************************************************************
char *MessageNet( NetMessId id_mess ) {

    if ( id_mess < SIZE_ARRAY( net_msg ) )
        return net_msg[id_mess];
    else return NULL;
 }

//*************************************************************************************************
// Возвращает указатель на строку текстового сообщения
// NetMessId id_mess - ID сообщения
// return            - указатель на строку
//*************************************************************************************************
char *MessageGui( GuiMessId id_mess ) {

    if ( id_mess < SIZE_ARRAY( gui_msg ) )
        return gui_msg[id_mess];
    else return NULL;
 }

