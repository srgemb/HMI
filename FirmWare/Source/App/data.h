
#ifndef __DATA_H
#define __DATA_H

#include <stdint.h>
#include <stdbool.h>

#include "device.h"
#include "dev_param.h"
#include "can_def.h"

//*************************************************************************************************
// Функции управления
//*************************************************************************************************
void ClearData( void );
void CanData( MSGQUEUE_CAN data );

#endif
