
//*************************************************************************************************
//
// Параметры дисплея и контроллера дисплея
//
//*************************************************************************************************

#ifndef __LCD_DEF_H
#define __LCD_DEF_H

//*************************************************************************************************
// Биты конфигурации регистра LCD_CTRL управления контроллера
//*************************************************************************************************
#define LCD_CTRL_LCDEN          0           //LCD enable control bit
#define LCD_CTRL_LCDBPP         1           //LCD bits per pixel
#define LCD_CTRL_LCDBW          4           //STN LCD monochrome/color selection
#define LCD_CTRL_LCDTFT         5           //LCD panel TFT type selection
#define LCD_CTRL_LCDMONO8       6           //Monochrome LCD interface width
#define LCD_CTRL_LCDDUAL        7           //Single or Dual LCD panel selection
#define LCD_CTRL_BGR            8           //Color format selection RGB/BGR
#define LCD_CTRL_BEBO           9           //Big-endian Byte Order
#define LCD_CTRL_BEPO           10          //Big-Endian Pixel Ordering
#define LCD_CTRL_LCDPWR         11          //LCD power enable
#define LCD_CTRL_LCDVCOMP       12          //LCD Vertical Compare Interrupt
#define LCD_CTRL_WATERMARK      16          //LCD DMA FIFO watermark level

//*************************************************************************************************
// Биты конфигурации регистра LCD_TIMH горизонтальной развертки
//*************************************************************************************************
#define LCD_TIMH_PPL            2           //Pixels-per-line
#define LCD_TIMH_HSW            8           //Horizontal synchronization pulse width
#define LCD_TIMH_HFP            16          //Horizontal front porch
#define LCD_TIMH_HBP            24          //Horizontal back porch

//*************************************************************************************************
// Биты конфигурации регистра LCD_TIMV вертикальной развертки
//*************************************************************************************************
#define LCD_TIMV_LPP            0           //Lines per panel
#define LCD_TIMV_VSW            10          //Vertical synchronization pulse width
#define LCD_TIMV_VFP            16          //Vertical front porch
#define LCD_TIMV_VBP            24          //Vertical back porch

//*************************************************************************************************
// Биты конфигурации регистра LCD_POL полярности управляющих сигналов 
//*************************************************************************************************
#define LCD_POL_PCD_LO          0           //Lower five bits of panel clock divisor
#define LCD_POL_CLKSEL          5           //Clock Select
#define LCD_POL_ACB             6           //AC bias pin frequency
#define LCD_POL_IVS             11          //Invert vertical synchronization
#define LCD_POL_IHS             12          //Invert horizontal synchronization
#define LCD_POL_IPC             13          //Invert panel clock
#define LCD_POL_IOE             14          //Invert output enable
#define LCD_POL_CPL             16          //Clocks per line
#define LCD_POL_BCD             26          //Bypass pixel clock divider
#define LCD_POL_PCD_HI          27          //Upper five bits of panel clock divisor

//*************************************************************************************************
// Параметры работы дисплея
//*************************************************************************************************
#define C_GLCD_REFRESH_FREQ     50

#define C_GLCD_H_SIZE           640         //пикселей по горизонтали
#define C_GLCD_H_PULSE          10 
#define C_GLCD_H_FRONT_PORCH    16
#define C_GLCD_H_BACK_PORCH     134U

#define C_GLCD_V_SIZE           480         //пикселей по вертикале
#define C_GLCD_V_PULSE          2
#define C_GLCD_V_FRONT_PORCH    32
#define C_GLCD_V_BACK_PORCH     10

#define C_GLCD_PWR_ENA_DIS_DLY  50000
#define C_GLCD_ENA_DIS_DLY      50000

#define LCD_BITSPERPIXEL        24

#define CRSR_PIX_32             0
#define CRSR_PIX_64             1
#define CRSR_ASYNC              0
#define CRSR_FRAME_SYNC         2

//размер экрана в пикселях
#define C_GLCD_SIZE_PIXEL       ( C_GLCD_H_SIZE * C_GLCD_V_SIZE )

//размер экрана в байтах
#define C_GLCD_SIZE_BYTES       ( C_GLCD_H_SIZE * C_GLCD_V_SIZE * 4 )

#define C_GLCD_CLK_PER_LINE     ( C_GLCD_H_SIZE + C_GLCD_H_PULSE + C_GLCD_H_FRONT_PORCH + C_GLCD_H_BACK_PORCH )
#define C_GLCD_LINES_PER_FRAME  ( C_GLCD_V_SIZE + C_GLCD_V_PULSE + C_GLCD_V_FRONT_PORCH + C_GLCD_V_BACK_PORCH )
#define C_GLCD_PIX_CLK          ( C_GLCD_CLK_PER_LINE * C_GLCD_LINES_PER_FRAME * C_GLCD_REFRESH_FREQ )

#endif
