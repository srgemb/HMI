
#ifndef __PARSE_H
#define __PARSE_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_CNT_PARAM       20          //максимальное кол-во параметров, включая команду
#define MAX_LEN_PARAM       16          //максимальный размер (длинна) параметра в командной строке

//индексы для получения параметров команды
#define IND_PAR_CMND        0           //команда
#define IND_PARAM1          1           //параметр 1
#define IND_PARAM2          2           //параметр 2
#define IND_PARAM3          3           //параметр 3
#define IND_PARAM4          4           //параметр 4
#define IND_PARAM5          5           //параметр 5
#define IND_PARAM6          6           //параметр 6
#define IND_PARAM7          7           //параметр 7
#define IND_PARAM8          8           //параметр 8
#define IND_PARAM9          9           //параметр 9
#define IND_PARAM10         10          //параметр 10

uint8_t ParseCommand( char *src );
uint8_t GetParamCnt( void );
char *GetParamVal( uint8_t index );
char *GetParamList( void );

#endif
