
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "uart.h"
#include "dialog.h"
#include "calibrate.h"

#define NUM_CALIB_POINTS  5  //кол-во точек калибровки

//*********************************************************************************************
// Локальные переменные
//*********************************************************************************************
static int _aRefX[NUM_CALIB_POINTS];
static int _aRefY[NUM_CALIB_POINTS];
static int _aSamX[NUM_CALIB_POINTS];
static int _aSamY[NUM_CALIB_POINTS];

//*********************************************************************************************
// Прототипы локальных функций
//*********************************************************************************************
static void Explain( void );
static void GetPhysValues( uint16_t LogX, uint16_t LogY, uint16_t *pPhysX, uint16_t *pPhysY, const char *pString );
static void DispStringCentered( const char *pString );
static void WaitForPressedState( int Pressed );

//*********************************************************************************************
// Локальные константы
//*********************************************************************************************
static const char * _acPos[] = {   
    "(upper left position)",   
    "(lower right position)"   
 };   
   

//*********************************************************************************************
// Калибровка сенсора дисплея, вариант 1 по 5 точкам
//*********************************************************************************************
void Calibrate( void ) {

    uint16_t i;//, pMinx, pMiny, pMaxx, pMaxy;
    char str[40];
    uint16_t xSize, ySize;
    GUI_PID_STATE TouchState;

    xSize = LCD_GetXSize();
    ySize = LCD_GetYSize();
    //расчет координат для точек калибровки
    //--------------------
    //| P0            P1 |
    //|                  |
    //|        P4        |
    //|                  |
    //| P3            P2 |
    //--------------------
    _aRefX[0] = (xSize * 5) / 100;
    _aRefX[1] = xSize - (xSize * 5) / 100;
    _aRefX[2] = _aRefX[1];
    _aRefX[3] = _aRefX[0];
    _aRefX[4] = xSize / 2;

    _aRefY[0] = (ySize * 5) / 100;
    _aRefY[1] = _aRefY[0];
    _aRefY[2] = ySize - (ySize * 5) / 100;
    _aRefY[3] = _aRefY[2];
    _aRefY[4] = ySize / 2;
    // Draw reference points on LCD
    //GUI_Clear();
    GUI_TOUCH_GetState( &TouchState );
    TouchState.Pressed = 0;
    GUI_SetPenSize( 3 );
    for ( i = 0; i < NUM_CALIB_POINTS; i++ ) {
        //GUI_Clear();
        sprintf( str, "Please touch the point, step %u of %u", i+1, NUM_CALIB_POINTS );
        GUI_DispStringHCenterAt( str, LCD_GetXSize() / 2, LCD_GetYSize() / 2 - 60 );
        GUI_DrawCircle( _aRefX[i], _aRefY[i], 5 );
        while ( TouchState.Pressed != 1 ) {
            //ждем нажатия
            GUI_Delay( 250 );
            GUI_TOUCH_GetState( &TouchState );
           }
        if ( TouchState.Pressed == 1 ) {
            //Store sample points
            _aSamX[i] = GUI_TOUCH_GetxPhys();
            _aSamY[i] = GUI_TOUCH_GetyPhys();
            sprintf( str, "Touch: %u  X = %u Y = %u", i+1, TouchState.x, TouchState.y );
            GUI_DispStringAt( str, 10, 40 + ( i * 20 ) );
           }
        TouchState.Pressed = 0;
        GUI_Delay( 250 );
       }
    // Pass measured points to emWin
    GUI_TOUCH_CalcCoefficients( NUM_CALIB_POINTS, _aRefX, _aRefY, _aSamX, _aSamY, xSize, ySize );
    //вывод значений
    for ( i = 0; i < NUM_CALIB_POINTS; i++ ) {
        sprintf( str, "Calibr: %u  X = %u Y = %u", i+1, _aSamX[i], _aSamY[i] );
        GUI_DispStringAt( str, 10, 250 + ( i * 20 ) );
       }
    //GUI_TOUCH_GetCalData( GUI_COORD_X, &pMinx, &pMaxx );
    //GUI_TOUCH_GetCalData( GUI_COORD_Y, &pMiny, &pMaxy );
    //sprintf( str, "Xmin = %u Xmax = %u Ymin = %u Ymax = %u", pMinx, pMaxx, pMiny, pMaxy );
    //GUI_DispStringAt( str, 250, 350 );

    //GUI_CURSOR_Show();
    BUTTON_Create( 100, 100, 200, 50, 1234, WM_CF_SHOW );  // A button to be touched
    GUI_DispStringAt( "End calibraion.", 10, 400 );
 }

//*********************************************************************************************
// Калибровка сенсора дисплея, вариант 2 по 2 точкам
//*********************************************************************************************
void Calibrate2( void ) {   

    char str[40];
    uint16_t calX0, calX1, calY0, calY1;
    uint16_t aPhysX[2], aPhysY[2], aLogX[2], aLogY[2], i;

    do {
        GUI_SetBkColor( GUI_WHITE );
        GUI_Clear();
        GUI_SetColor( GUI_BLACK );
        Explain();
        //Set the logical values
        aLogX[0] = 15;
        aLogY[0] = 15;
        aLogX[1] = LCD_GetXSize() - 20;
        aLogY[1] = LCD_GetYSize() - 20;
        //Get the physical values of the AD converter for 2 positions
        for (i = 0; i < 2; i++)
            GetPhysValues( aLogX[i], aLogY[i], &aPhysX[i], &aPhysY[i], _acPos[i] );
        //Use the physical values to calibrate the touch screen
        GUI_TOUCH_Calibrate( 0, aLogX[0], aLogX[1], aPhysX[0], aPhysX[1] ); //Calibrate X-axis
        GUI_TOUCH_Calibrate( 1, aLogY[0], aLogY[1], aPhysY[0], aPhysY[1] ); //Calibrate Y-axis
         
        //calculate and display values for configuration file
        GUI_Clear();
           
        GUI_TOUCH_GetCalData( GUI_COORD_X, &calX0, &calX1 );
        GUI_TOUCH_GetCalData( GUI_COORD_Y, &calY0, &calY1 );
           
        GUI_DispStringAt( "calX0: ", 0, 0 );    GUI_DispDec( calX0, 4 );        GUI_DispNextLine();
        GUI_DispString( "calX1: " );            GUI_DispDec( calX1, 4 );        GUI_DispNextLine();
        GUI_DispString( "calY0: " );            GUI_DispDec( calY0, 4 );        GUI_DispNextLine();
        GUI_DispString( "calY1: " );            GUI_DispDec( calY1, 4 );
             
        GUI_DispStringAt( "lcdx0: ", 0, 200 );  GUI_DispDec( aLogX[0], 4 );     GUI_DispNextLine();
        GUI_DispString( "lcdx1: " );            GUI_DispDec( aLogX[1], 4 );     GUI_DispNextLine();
        GUI_DispString( "lcdy0: " );            GUI_DispDec( aLogY[0], 4 );     GUI_DispNextLine();
        GUI_DispString( "lcdy1: " );            GUI_DispDec( aLogY[1], 4 );     GUI_DispNextLine();
           
        GUI_DispString( "tscX0: " );            GUI_DispDec( aPhysX[0], 4 );    GUI_DispNextLine();
        GUI_DispString( "tscX1: " );            GUI_DispDec( aPhysX[1], 4 );    GUI_DispNextLine();
        GUI_DispString( "tscY0: " );            GUI_DispDec( aPhysY[0], 4 );    GUI_DispNextLine();
        GUI_DispString( "tscY1: " );            GUI_DispDec( aPhysY[1], 4 );    GUI_DispNextLine();
           
        sprintf( str, "calX0: %u calX1: %u calY0: %u calY1: %u\r", calX0, calX1, calY0, calY1 );
        UARTSendStr( str );
        
        GUI_DispString( "Please touch display to continue..." );
        GUI_Delay( 1000 );
           
        WaitForPressedState( 1 );
        WaitForPressedState( 0 );
    } while ( 1 );
 }

//*********************************************************************************************
// Ожидание нажатия
//*********************************************************************************************
static void WaitForPressedState( int Pressed ) {   

    GUI_PID_STATE State;   
    
    //Wait until touch is pressed
    do {   
        GUI_TOUCH_GetState(&State);
        if ( State.Pressed == Pressed )
            break;
        GUI_Delay( 200 );
    } while ( 1 );
 }   

//*********************************************************************************************
// Вывод строки в центре дисплея
// const char * pString - указатель на строку для вывода
//*********************************************************************************************
static void DispStringCentered( const char * pString ) {   

    GUI_RECT Rect;

    Rect.x0 = Rect.y0 = 0;
    Rect.x1 = LCD_GetXSize() - 1;
    Rect.y1 = LCD_GetYSize() - 1;
    GUI_DispStringInRect( pString, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER );
 }   

//*********************************************************************************************
// Отрисовка контрольных точек калибровки
// int LogX, LogY       - адреса физических координат на дисплее (для отображения точки касания)
// int *pPhysX, *pPhysY - указатель на массив значения точек касания (логических)
// const char * pString - указатель на строку для вывода
//*********************************************************************************************
static void GetPhysValues( uint16_t LogX, uint16_t LogY, uint16_t *pPhysX, uint16_t *pPhysY, const char *pString ) {   

    GUI_RECT Rect;   
    uint16_t FontSizeY, Align;
    char acText[] = "Press here";

    FontSizeY = GUI_GetFontSizeY();
    GUI_Clear();
    GUI_SetColor( GUI_BLACK );
    DispStringCentered( "Runtime calibration,\n"
                        "please touch the screen\n"
                        "at the center of the ring." );

    //Calculate the rectangle for the string
    Rect.y0 = LogY - FontSizeY;
    Rect.y1 = LogY + FontSizeY;
    if ( LogX < LCD_GetXSize() / 2 ) {
        Rect.x0 = LogX + 15;
        Rect.x1 = LCD_GetXSize();
        Align = GUI_TA_LEFT;
       } 
    else {
        Rect.x0 = 0;
        Rect.x1 = LogX - 15;
        Align = GUI_TA_RIGHT;
       }
    //Show the text nearby the ring
    GUI_DispStringInRect( acText, &Rect, Align | GUI_TA_TOP );
    GUI_DispStringInRect( pString, &Rect, Align | GUI_TA_BOTTOM );
    // Draw the ring
    GUI_FillCircle( LogX, LogY, 10 );
    GUI_SetColor( GUI_WHITE );
    GUI_FillCircle(LogX, LogY, 5 );
    GUI_SetColor( GUI_BLACK );
    //Wait until touch is pressed
    WaitForPressedState( 1 );
    *pPhysX = GUI_TOUCH_GetxPhys();
    *pPhysY = GUI_TOUCH_GetyPhys();
    //Wait until touch is released
    WaitForPressedState( 0 );
 }   
   
//*********************************************************************************************
// Вывод приглашения и ожидание нажатия
//*********************************************************************************************
static void Explain( void ) {

    DispStringCentered( "This sample shows how\n"
                        "a touch screen can be\n"
                        "calibrated at run time.\n"
                        "Please press the touch\n"
                        "screen to continue..." );
    GUI_DispStringHCenterAt( "TOUCH_Calibrate", LCD_GetXSize()/2, 5 );
    WaitForPressedState( 1 );
    WaitForPressedState( 0 );
}   
   
