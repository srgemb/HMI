
#ifndef __CAN_H
#define __CAN_H

#include <stdint.h>
#include <stdbool.h>

#include "device.h"
#include "dev_param.h"

//состояние связи по CAN шине
typedef enum {
    CAN_LINK_NO,                            //связи нет
    CAN_LINK_ERROR,                         //есть ошибки
    CAN_LINK_OK                             //связь без ошибок
 } CANLinkStat;

//*************************************************************************************************
// Функции управления
//*************************************************************************************************
void CANInit( void );
void ReqConfig( void );
Status SendCommand( Device dev_id, uint8_t *ptr_data, uint8_t len_data );
Status SendConfig( ConfigParam conf_id, ConfigValSet value );

//*************************************************************************************************
// Функции статуса/состояния
//*************************************************************************************************
CANLinkStat LinkStat( void );
ValueParam LinkGetValue( ParamHmi id_param );

#endif
