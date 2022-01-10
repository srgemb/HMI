
#ifndef __MESSAGE_H
#define __MESSAGE_H

#include "mess_log.h"

//*************************************************************************************************
// ID сообщений консоли
//*************************************************************************************************
typedef enum {
    CONS_MSG_PROMPT,                        //>
    CONS_MSG_OK,                            //OK
    CONS_MSG_CRLF,                          //\r\n
    CONS_MSG_ERR_CMND,                      //Invalid command.
    CONS_MSG_ERR_NOJOB,                     //Command will not be executed from the scheduler.
    CONS_MSG_ERR_PARAM,                     //Invalid parameter.
    CONS_MSG_ERR_NOPARAM,                   //Parameter not specified.
    CONS_MSG_ERR_SEND,                      //There is no communication with the controller.
    CONS_MSG_ERR_FOPEN,                     //File open error.
    CONS_MSG_ERR_FILE,                      //File name not specified.
    CONS_MSG_FILE_JOBS,                     //Loading a job file: %s ... 
    CONS_MSG_ERR_LIMITSTR,                  //Exceeded line limit.
    CONS_MSG_ERR_JOB,                       //Task not specified.
    CONS_MSG_ERR_NO_JOB,                    //There are no tasks in the file.
    CONS_MSG_ERR_JOB_OPEN,                  //Temporary file: jobs~ does not open.
    CONS_MSG_ERR_JOB_RENAME,                //Temporary file: jobs~ not renamed.
    CONS_MSG_FILE,                          //File: 
    CONS_MSG_FILE1,                         //File name: %s size: %u bytes.
    CONS_MSG_FILE2,                         //%sFile size: %u read: %u bytes.
    CONS_MSG_NOT_DELETED,                   //File not deleted.
    CONS_MSG_NOT_OPEN,                      //File: %s does not open.
    CONS_MSG_CHARGE_DAY,                    //Only %d days have passed from a full battery charge.
    CONS_MSG_JOB_HDR1,                      //====================================================================
    CONS_MSG_JOB_HDR2,                      //        ..........1.........2.........3.........4.........5.........
    CONS_MSG_JOB_HDR3,                      //        012345678901234567890123456789012345678901234567890123456789
    CONS_MSG_JOB_HDR4,                      //--------------------------------------------------------------------
    CONS_MSG_HEADER,                        //--------------------------------- ...
    CONS_MSG_TASK_HDR1,                     //Name thread     Priority  State      Stack Unused
    CONS_MSG_TASK_HDR2,                     //----------------------------------------------------
    CONS_MSG_APP_VER,                       //Application version ......... %s
    CONS_MSG_BUILD_VER,                     //Build version ............... %s %s
    CONS_MSG_CPU_CLOCK,                     //CPU clock ................... %s Hz
    CONS_MSG_CLOCK_PER,                     //Clock periphery ............. %s Hz
    CONS_MSG_CLOCK_KERNEL,                  //Clock of the kernel tick .... %s Hz
    CONS_MSG_CLOCK_SYSTIMER,                //Clock of the system timer ... %s Hz
    CONS_MSG_KERNEL_INFO,                   //Kernel information .......... %s
    CONS_MSG_KERNEL_VER,                    //Kernel version .............. %d.%d.%d
    CONS_MSG_KERNEL_API                     //Kernel API version .......... %d.%d.%d
 } ConsMessage;

//*************************************************************************************************
// ID сообщений функционала работы с SD картой
//*************************************************************************************************
typedef enum {
    MSG_SD_NOT_INSERT,                      //SD card not inserted.
    MSG_SD_NOT_MOUNT,                       //SD card not mounted.
    MSG_SD_INIT_SYSTEM,                     //Initialize File System ...
    MSG_SD_MOUNT,                           //Mount the file system ...
    MSG_SD_CHECK_DRV,                       //Check if valid drive is specified ...
    MSG_SD_ERROR,                           //Error.
    MSG_SD_BLC_ACCESS,                      //Block media access: %s %s
    MSG_SD_IOC_ACCESS,                      //IOC device control access: %s %s
    MSG_SD_UNLOCK,                          //Unlock drive and allow media access to the upper layer: %s %s
    MSG_SD_VOLUME_READ,                     //Read volume label and serial number: %s
    MSG_SD_VOLUME,                          //Volume in drive: %s
    MSG_SD_VOLUME_SER,                      //Volume serial number is: %X
    MSG_SD_UNMOUNT,                         //Unmount drive: %s %s
    MSG_SD_ABOUT,                           //Information about SDCard:
    MSG_SD_ID_VENDOR,                       //    Manufacturer ID: %d (0x%.2X)
    MSG_SD_OEMID,                           //             OEM ID: %c%c
    MSG_SD_NAMEPROD,                        //       Product name: %c%c%c%c%c
    MSG_SD_REVISION,                        //   Product revision: %d.%d
    MSG_SD_SERIAL2,                         //      Serial number: 0x%X
    MSG_SD_DATE,                            //Date of manufacture: %d/%.2d
    MSG_SD_CID_ERROR,                       //CID register not read.
    MSG_SD_FORMAT,                          //Format drive: %s %s
    MSG_FILE_NOT_SPECF,                     //File name not specified.
    MSG_FILE_SRC_NOT_SPECF,                 //Source file name not specified.
    MSG_FILE_NEW_NOT_SPECF,                 //New file name not specified.
    MSG_FILE_LIST_FILES,                    //List of files %s:%s
    MSG_FILE_FFIND,                         //Function ffind(): %s
    MSG_FILE_DELETED,                       //Deleting a file: %s %s
    MSG_DIR_DELETED,                        //Removing a directory: %s %s
    MSG_DIR_NOT_SPECF,                      //Directory name not specified.
    MSG_FILE_RENAME,                        //Rename a file or directory: %s to: %s %s
    MSG_DIR_INFO,                           //%-41s    <directory>"
    MSG_NO_FILES,                           //No files.
    MSG_FILES_TOTAL,                        //%9d files  %21s bytes
    MSG_DIR_TOTAL,                          //%9d directory   %21s byte aviable.
    MSG_DIR_FREE,                           //%56s byte aviable.
    MSG_FILE_EOF                            //<EOF>
 } SdMessId;

//*************************************************************************************************
// ID сообщений функционала работы с электронным диском
//*************************************************************************************************
typedef enum {
    MSG_RDISK_HDR1,                         //Header1
    MSG_RDISK_HDR2,                         //Header2
    MSG_RDISK_INIT,                         //RAM DISK INIT: total %s bytes.\r\nRAM DISK: free %s bytes.
    MSG_RDISK_NO_SDCARD,                    //SD card not found.
    MSG_RDISK_NOT_FoUND,                    //File: %s not found.
    MSG_RDISK_NO_FREE,                      //File: %s no free blocks.
    MSG_RDISK_NOT_SPACE,                    //File: %s no upload space.
    MSG_RDISK_UPL_ERROR,                    //File: %s upload error.
    MSG_RDISK_UPLOAD,                       //File: %s upload: %s bytes.
    MSG_RDISK_NO_FILES,                     //There are no files. Available: %s bytes.
    MSG_RDISK_FILES,                        //Files: %u used: %s available: %s bytes.
    MSG_RDISK_VIOLATION,                    //Violation of data integrity, address: 0x%08X
    MSG_RDISK_TOTAL_FREE                    //RAM DISK: total %u bytes.\r\nRAM DISK: free %s bytes.\r\nFiles: %u
 } RdiskMessId;


typedef enum {
    MSG_FTP_START,                          //Start the FTP server: %s\r\n
    MSG_FTP_ACCEPT,                         //FTP server accept authentication from: %u.%u.%u.%u\r\n
    MSG_FTP_USER,                           //FTP user name: \"%s\" %s\r\n
    MSG_FTP_PASSWD,                         //FTP user ID: %d, password: \"%s\" %s\r\n
    MSG_FTP_FILE_NAME,                      //FTP user ID: %d, file name: %s, file access: 0x%02X %s\r\n
    MSG_FTP_LOG_ERROR                       //The ftp_server.log file is not open.\r\n
 } FtpMessId;

//*************************************************************************************************
// ID сообщений функционала FTP сервера
//*************************************************************************************************
typedef enum {
    MSG_NET_PROMPT,                         //%s>
    MSG_NET_INIT_OK,                        //Network Core initialized successfully.
    MSG_NET_INIT_ERR,                       //Failed to initialize the Network Core.
    MSG_NET_SET_OPTION,                     //Set interface option: %s
    MSG_NET_DHCP_DISABLE,                   //Disable DHCP: %s
    MSG_NET_DHCP_OPTION,                    //Set DHCP option: %s
    MSG_NET_PIMG_EXCEED,                    //Hostname exceeded length.
    MSG_NET_PING_START1,                    //Ping host: %s started.
    MSG_NET_PING_NOT_READY,                 //Ping not ready or bad parameters.
    MSG_NET_PING_START2,                    //Ping host: %s started ... 
    MSG_NET_PING_NO_RESP1,                  //Remote host: %s responded to ping.
    MSG_NET_PING_NO_RESP2,                  //Remote host: %s, ping timeout, no response.
    MSG_NET_SOCKET_OPEN,                    //Allocate a free TCP socket: %u
    MSG_NET_SOCKET_LISTEN,                  //Open TCP socket ID: %u for incoming connection: %s"
    MSG_NET_SOCKET_ERROR,                   //Get socket ID: %u failed.
    MSG_NET_PORT_CONN,                      //Port: %u, Connect from: %s port: %u socket ID: %d
    MSG_NET_PORT_CLOSE,                     //Port: %u, Close connect from: %s port: %u socket ID: %u
    MSG_NET_PORT_ABORT,                     //Port: %u, Abort connect from: %s port: %u socket ID: %u
    MSG_NET_PORT_DATA1,                     //Port: %u, Data recv from: %s port: %u size: %u "%s"
    MSG_NET_PORT_DATA2,                     //Port: %u, Data recv from: %s port: %u size: %u
    MSG_NET_SOCK_ERR_PROMPT,                //Prompt error send, socket: %u %s
    MSG_TELNET_START,                       //Start the Telnet server: %s
    MSG_TELNET_WELCOME,                     //Welcome to the %s telnet server.
    MSG_TELNET_PROMPT,                      //CMD> 
    MSG_TELNET_LOGIN,                       //%s embedded telnet server, please login ...
    MSG_TELNET_USER,                        //Username: 
    MSG_TELNET_PASSWD,                      //Password: 
    MSG_TELNET_LOGIN_ERR,                   //Login incorrect.
    MSG_TELNET_LOGIN_ERR1,                  //Telnet login incorrect: %s
    MSG_TELNET_TIMEOUT,                     //Login timeout.
    MSG_TELNET_END,                         //OK. Telnet session ended.
    MSG_TELNET_ENT_COMMAND,                 //Enter command !
    MSG_TELNET_ACCEPT,                      //Telnet accept authentication from: %u.%u.%u.%u
    MSG_TELNET_USER_OUT,                    //Telnet user name: "%s" %s
    MSG_TELNET_USER_PASS,                   //Telnet user ID: %d, password: "%s" %s
    MSG_TELNET_TELLOG_SD,                   //TelnetLog: SD card not inserted. #
    MSG_TELNET_TELLOG_LOG,                  //The telnet_server.log file is not open. #
    MSG_TELNET_PASS_ACCEPT,                 //password accepted.
    MSG_TELNET_PASS_INVALID,                //invalid password.
    MSG_NET_USER_ACCEPT,                    //access is allowed.
    MSG_NET_USER_DENIED,                    //access denied.
    MSG_NET_PASS_ACCEPT,                    //password accepted.
    MSG_NET_PASS_DENIED                     //invalid password.
 } NetMessId;

//*************************************************************************************************
// ID сообщений функционала графического интерфейса
//*************************************************************************************************
typedef enum {
    MSG_GUI_LOAD_FILE,                      //Загрузка графических файлов ... %d%%, ошибок: %u
    MSG_GUI_LOAD_OK,                        //Графические файлы загружены.
    MSG_GUI_INIT_WIN,                       //Инициализация экраных форм ...
    MSG_GUI_ERROR,                          //error.
    MSG_GUI_OK,                             //OK.
    MSG_GUI_APPLY,                          //Применить значения параметров без сохранения в EEPROM ?
    MSG_GUI_EEPROM,                         //Сохранить значения параметров в EEPROM ?
    MSG_GUI_UNDER_DEV,                      //Функционал находится в стадии разработки.
    MSG_GUI_INIT_WIN0,                      //Create window 0 ...
    MSG_GUI_INIT_WIN1,                      //Create window 1 ...
    MSG_GUI_INIT_WIN2,                      //Create window 2 ...
    MSG_GUI_TASK_STARTUP,                   //TaskWindow start.
    MSG_GUI_CALC_DEG_MM,                    //Выполнен пересчет угла в градусах в миллиметры позиционирования.
    MSG_GUI_ERR_RANGE_MM,                   //Введенное значение %s выходит за пределы допустимого диапазона: 0 - 900 мм
    MSG_GUI_ERR_RANGE_DEG1,                 //Введенное значение %s выходит за пределы допустимого диапазона: 0 - 90 °
    MSG_GUI_ERR_RANGE_DEG2,                 //Введенное значение %s выходит за пределы допустимого диапазона: %.2f - %.2f °
    MSG_GUI_FORMAT_ERR,                     //Ошибка при проверки формата. Введенное значение не соответствует требованиям.
    MSG_GUI_ERR_RANGE,                      //Введенное значение %s выходит за пределы установленного диапазона: %d ... %d
    MSG_GUI_VALUE_CHANGE                    //Значения параметров изменены.
 } GuiMessId;

//*************************************************************************************************
// Функции статуса/состояния
//*************************************************************************************************
char *MessageHelp( void );
char *Message( ConsMessage id_mess );
char *MessageLog( LogMessId id_mess );
char *MessageSd( SdMessId id_mess );
char *MessageRdisk( RdiskMessId id_mess );
char *MessageFtp( FtpMessId id_mess );
char *MessageNet( NetMessId id_mess );
char *MessageGui( GuiMessId id_mess );

#endif

