
#ifndef __PORTS_H
#define __PORTS_H


//*********************************************************************************************
// Выходы управления
//*********************************************************************************************
//Номера портов
#define PORT0               0
#define PORT1               1
#define PORT2               2
#define PORT3               3
#define PORT4               4
#define PORT5               5

//Внешние сигналы интерфейса связи (SPI)
#define EXT_INT             19
#define EXT_BUSY            21

//Номера пинов управления
#define USB_UP_LED2         13          //индикатор состояния
#define LCD_ADJ_BRG         0           //пин управление подсветкой LCD
#define PWM1_OUT            1           //альтернативная функция для LCD_ADJ_BRG

void PortsInit( void );
      
#endif


