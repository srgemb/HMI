
#include <stdint.h>
#include <stdbool.h>

#ifndef __BOARD_TOUCH_H
#define __BOARD_TOUCH_H

//*************************************************************************************************
// Пины и маски сенсорного экрана
//*************************************************************************************************
#define TS_X1_PIN           23              //P0.23, ADC0_IN0
#define TS_X1_MASK          (1UL << 23)

#define TS_X2_PIN           24              //P0.24, ADC0_IN1
#define TS_X2_MASK          (1UL << 24)

#define TS_Y1_PIN           25              //P0.25, ADC0_IN2
#define TS_Y1_MASK          (1UL << 25)

#define TS_Y2_PIN           26              //P0.26, ADC0_IN3
#define TS_Y2_MASK          (1UL << 26)

//*************************************************************************************************
// Константы временн'ых интервалов
// Множитель = 10 (us) 
//*************************************************************************************************
#define TS_SETUP_DELAY      10              //100 (usec) задержка перед каждым измерением для завершения
                                            //переходных процессов после изменения режимов X1,X2,Y1,Y2

#define TS_SAMPLE_DELAY     1000            //10 (msec) задержка после последнего измерения

#define TS_INIT_DELAY       5000            //50 (msec) задержка измерения после первого нажатия
                                            //и после отпускания

#define TS_REPEAT_DELAY     10000           //100 (msec) задержка обработки удержания нажатия
                                            
#define TS_SAMPLES          1               //кол-во измерений по каждому из каналов X1,X2,Y1,Y2

//*************************************************************************************************
//Touch state
//*************************************************************************************************
typedef struct _TOUCH_STATE {
    int16_t x;              //Position X
    int16_t y;              //Position Y
    uint8_t pressed;        //Pressed flag
    uint8_t padding;
 } TOUCH_STATE;


void test( void );

int32_t Touch_Initialize( void );
int32_t Touch_Uninitialize( void );
int32_t Touch_GetState( TOUCH_STATE *pState );

#endif
