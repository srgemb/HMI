
#ifndef __EXT_RAM_ADDR_H
#define __EXT_RAM_ADDR_H

//*************************************************************************************************
// Адрес буфера для дисплея (отдельно от памяти RAM1 - GUI_LIB)
// Video RAM Size = LCD_XSIZE x LCD_YSIZE x LCD_BITSPERPIXEL / 8 * NUM_SCREENS
// Video RAM Size = ( 640 * 480 * 24 ) / 8 * 4 = 3,686,400 (3.5Mb) => 4Mb = 0x00400000
//*************************************************************************************************
#define VRAM_ADDR               0xA0800000                      //адрес буфера GLCD, 
#define VRAM_SIZE               0x00400000                      //размер буфера GLCD 

//*************************************************************************************************
// параметры электронного диска
//*************************************************************************************************
#define RAMDISK_ADDR            0xA0C00000                      //адрес блока памяти RAMDISK
#define RAMDISK_SIZE            0x00800000                      //размер RAMDISK 8M
#define RAMDISK_MAX             RAMDISK_ADDR + RAMDISK_SIZE     //максимальный размер памяти диска

//*************************************************************************************************
// параметры буфера передачи UART
//*************************************************************************************************
#define UARTBUF_ADDR            0xA1400000                      //адрес блока памяти буфера
#define UARTBUF_SIZE            0x00010000                      //размер 64Kb
    
//*************************************************************************************************
// Адреса размещение больших массивов
//*************************************************************************************************
#define PARSE_RAM_ADDRES        0xA1410000                      //param_list[MAX_CNT_PARAM][MAX_LEN_PARAM]
#define JOBS_RAM_ADDRES         0xA1410400                      //jobs[MAX_JOBS]            (sheduler.c)
#define JBUF_RAM_ADDRES         0xA1412400                      //job_buff[SIZE_BUFFER]     (sheduler.c)
#define JTMP_RAM_ADDRES         0xA1412800                      //job_temp[SIZE_BUFFER]     (sheduler.c)
#define JEXE_RAM_ADDRES         0xA1412C00                      //job_exeс[SIZE_BUFFER]     (sheduler.c)
#define EDIT_RAM_ADDRES         0xA1413000                      //result[SIZE_EDIT_TEXT]    (win_keybrd.c)
#define NETD_RAM_ADDRES         0xA1416800                      //net_data_send[4096]       (net_data.c)
#define SCRN_RAM_ADDRES         0xA1417800                      //screen_buff[640*480*4]    (screen.c)
#define SROW_RAM_ADDRES         0xA1543800                      //screen_temp[640*4]        (screen.c)

#endif
