
#ifndef __COMMAND_H
#define __COMMAND_H

#include <stdint.h>
#include <stdbool.h>

//код источника вызова для ExecCommand()
typedef enum {
    SOURCE_CMD_UART,                        //команда передана из UART
    SOURCE_CMD_TELNET,                      //команда передана из TELNET
    SOURCE_CMD_TCP,                         //команда передана по TCP (RAW формат)
    SOURCE_CMD_JOB                          //команда передана из планировщика заданий
 } SourceCmd;
 
//структура очереди для обработки команд UART,TELNET,TCP(raw)
typedef struct {
    uint32_t id_msg;                        //ID сообщения
    int32_t  socket;                        //ID сокета источника/получателя
    char     *ptr_cmd;                      //указатель на буфер содержащий команду
    uint16_t len;                           //длинна команды
} QUEUE_CMD;                            

//*************************************************************************************************
// Функции управления
//*************************************************************************************************
void CommandInit( void );
char *GetBuffer( void );
uint32_t GetBuffSize( void );
void TaskCommand( void *pvParameters );
char *ExecuteJob( char *cmnd );
uint32_t ExecCommand( SourceCmd src_call, char *cmnd, char *buff );
uint32_t ExecTelnet( SourceCmd src_call, const char *cmd, char *buf, uint32_t buf_len, uint32_t *pvar );

#endif
