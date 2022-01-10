
//*************************************************************************************************
//
// Данных полученные по CAN шине
//
//*************************************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "device.h"
#include "dev_data.h"
#include "dev_param.h"

#include "can_data.h"
#include "can_def.h"

#include "data.h"
#include "command.h"
#include "events.h"

//*************************************************************************************************
// Переменные с внешним доступом
//*************************************************************************************************
PORTS                   ports;
ALT                     alt;
MPPT                    mppt;
CHARGER                 charger;
BATMON                  batmon;
INVERTER                inv1, inv2;
GEN                     gen, *gen_ptr = NULL;
SUNPOS                  sunpos;
RTC                     rtc;
TRACKER                 tracker;
VOICE                   voice;
CONFIG                  config;
    
//*********************************************************************************************
// Локальные переменные
//*********************************************************************************************
static CAN_BATMON1      can_batmon1;
static CAN_BATMON2      can_batmon2;
static CAN_BATMON3      can_batmon3;
static CAN_BATMON4      can_batmon4;
static CAN_BATMON5      can_batmon5;
static CAN_BATMON6      can_batmon6;
static CAN_BATMON7      can_batmon7;
    
static CAN_MPPT1        can_mppt1;
static CAN_MPPT2        can_mppt2;
static CAN_MPPT3        can_mppt3;
static CAN_MPPT4        can_mppt4;
static CAN_MPPT5        can_mppt5;
static CAN_MPPT6        can_mppt6;
    
static CAN_CHARGER1     can_charger1;
    
static CAN_INV1         can_1inv1; 
static CAN_INV2         can_2inv1;
    
static CAN_INV1         can_1inv2;
static CAN_INV2         can_2inv2;
    
static CAN_GEN1         can_gen1;
static CAN_GEN2         can_gen2;
    
static CAN_TRC1         can_trc1;
static CAN_TRC2         can_trc2;
    
static CAN_SPA1         can_spa1;
static CAN_SPA2         can_spa2;
static CAN_SPA3         can_spa3;
    
static CAN_CONFIG1      can_config1;
static CAN_CONFIG2      can_config2;
static CAN_CONFIG3      can_config3;
static CAN_CONFIG4      can_config4;
static CAN_CONFIG5      can_config5;
static CAN_CONFIG6      can_config6;
static CAN_CONFIG7      can_config7;
static CAN_CONFIG8      can_config8;
static CAN_CONFIG9      can_config9;
static CAN_CONFIG10     can_config10;
static CAN_CONFIG11     can_config11;
static CAN_CONFIG12     can_config12;
static CAN_CONFIG13     can_config13;
static CAN_CONFIG14     can_config14;
static CAN_CONFIG15     can_config15;

//*************************************************************************************************
// Прототипы локальных функций
//*************************************************************************************************
static void CanDataPorts( uint8_t *ptr );
static void CanDataBatmon( uint8_t sub_id, uint8_t *ptr );
static void CanDataMppt( uint8_t sub_id, uint8_t *ptr );
static void CanDataCharger( uint8_t sub_id, uint8_t *ptr );
static void CanDataInv1( uint8_t sub_id, uint8_t *ptr );
static void CanDataInv2( uint8_t sub_id, uint8_t *ptr );
static void CanDataGen( uint8_t sub_id, uint8_t *ptr );
static void CanDataTrc( uint8_t sub_id, uint8_t *ptr );
static void CanDataSpa( uint8_t sub_id, uint8_t *ptr );
static void CanDataAlt( uint8_t *ptr );
static void CanDataRtc( uint8_t *ptr );
static void CanDataVoice( uint8_t *ptr );
static void CanDataConfig( uint8_t sub_id, uint8_t *ptr );

//*************************************************************************************************
// Разбор принятых данных и заполнение структур
//*************************************************************************************************
void CanData( MSGQUEUE_CAN data ) {

    if ( data.dev_id == ID_DEV_PORTS )
        CanDataPorts( data.data );
    if ( data.dev_id == ID_DEV_RTC )
        CanDataRtc( data.data );
    if ( data.dev_id == ID_DEV_BATMON )
        CanDataBatmon( data.sub_pack_id, data.data );
    if ( data.dev_id == ID_DEV_MPPT )
        CanDataMppt( data.sub_pack_id, data.data );
    if ( data.dev_id == ID_DEV_CHARGER )
        CanDataCharger( data.sub_pack_id, data.data );
    if ( data.dev_id == ID_DEV_INV1 )
        CanDataInv1( data.sub_pack_id, data.data );
    if ( data.dev_id == ID_DEV_INV2 )
        CanDataInv2( data.sub_pack_id, data.data );
    if ( data.dev_id == ID_DEV_ALT )
        CanDataAlt( data.data );
    if ( data.dev_id == ID_DEV_GEN )
        CanDataGen( data.sub_pack_id, data.data );
    if ( data.dev_id == ID_DEV_TRC )
        CanDataTrc( data.sub_pack_id, data.data );
    if ( data.dev_id == ID_DEV_SPA )
        CanDataSpa( data.sub_pack_id, data.data );
    if ( data.dev_id == ID_DEV_VOICE )
        CanDataVoice( data.data );
    if ( data.dev_id == ID_CONFIG )
        CanDataConfig( data.sub_pack_id, data.data );
 }

//*************************************************************************************************
// Обнуление данных устройств
//*************************************************************************************************
void ClearData( void ) {

    memset( (uint8_t *)&ports, 0x00, sizeof( ports ) );
    memset( (uint8_t *)&alt, 0x00, sizeof( alt ) );
    memset( (uint8_t *)&mppt, 0x00, sizeof( mppt ) );
    memset( (uint8_t *)&charger, 0x00, sizeof( charger ) );
    memset( (uint8_t *)&batmon, 0x00, sizeof( batmon ) );
    memset( (uint8_t *)&inv1, 0x00, sizeof( inv1 ) );
    memset( (uint8_t *)&inv2, 0x00, sizeof( inv2 ) );
    memset( (uint8_t *)&gen, 0x00, sizeof( gen ) );
    memset( (uint8_t *)&sunpos, 0x00, sizeof( sunpos ) );
    memset( (uint8_t *)&rtc, 0x00, sizeof( rtc ) );
    memset( (uint8_t *)&tracker, 0x00, sizeof( tracker ) );
    memset( (uint8_t *)&voice, 0x00, sizeof( voice ) );
 }
 
//*************************************************************************************************
// Заполняет структуры данными портов входа
// uint8_t *ptr   - указатель на данные
//*************************************************************************************************
static void CanDataPorts( uint8_t *ptr ) {

    memcpy( (uint8_t *)&ports, ptr, sizeof( ports ) );
 }

//*************************************************************************************************
// Заполняет структуры данными блока АВР
// uint8_t *ptr   - указатель на данные
//*************************************************************************************************
static void CanDataAlt( uint8_t *ptr ) {

    memcpy( (uint8_t *)&alt, ptr, sizeof( alt ) );
 }

//*************************************************************************************************
// Заполняет структуры данными часов
// uint8_t *ptr   - указатель на данные
//*************************************************************************************************
static void CanDataRtc( uint8_t *ptr ) {

    memcpy( (uint8_t *)&rtc, ptr, sizeof( rtc ) );
 }

//*************************************************************************************************
// Заполняет структуры данными голосового информатора
// uint8_t *ptr   - указатель на данные
//*************************************************************************************************
static void CanDataVoice( uint8_t *ptr ) {

    memcpy( (uint8_t *)&voice, ptr, sizeof( voice ) );
 }

//*************************************************************************************************
// Заполняет структуры данными монитора АКБ
// uint8_t sub_id - ID блока данных
// uint8_t *ptr   - указатель на данные
//*************************************************************************************************
static void CanDataBatmon( uint8_t sub_id, uint8_t *ptr ) {

    if ( sub_id == 1 ) {
        memcpy( (uint8_t *)&can_batmon1, ptr, sizeof( can_batmon1 ) );
        batmon.link = can_batmon1.link;
        batmon.alarm = can_batmon1.alarm;
        batmon.relay = can_batmon1.relay;
        batmon.alarm_mode = can_batmon1.alarm_mode;
        batmon.ttg = can_batmon1.ttg;
        batmon.h4 = can_batmon1.h4;
        batmon.h5 = can_batmon1.h5;
       }
    if ( sub_id == 2 ) {
        memcpy( (uint8_t *)&can_batmon2, ptr, sizeof( can_batmon2 ) );
        batmon.voltage = can_batmon2.voltage;
        batmon.current = can_batmon2.current;
       }
    if ( sub_id == 3 ) {
        memcpy( (uint8_t *)&can_batmon3, ptr, sizeof( can_batmon3 ) );
        batmon.cons_energy = can_batmon3.cons_energy;
        batmon.soc = can_batmon3.soc;
       }
    if ( sub_id == 4 ) {
        memcpy( (uint8_t *)&can_batmon4, ptr, sizeof( can_batmon4 ) );
        batmon.h1 = can_batmon4.h1;
        batmon.h2 = can_batmon4.h2;
       }
    if ( sub_id == 5 ) {
        memcpy( (uint8_t *)&can_batmon5, ptr, sizeof( can_batmon5 ) );
        batmon.h3 = can_batmon5.h3;
        batmon.h6 = can_batmon5.h6;
       }
    if ( sub_id == 6 ) {
        memcpy( (uint8_t *)&can_batmon6, ptr, sizeof( can_batmon6 ) );
        batmon.h7 = can_batmon6.h7;
        batmon.h8 = can_batmon6.h8;
       }
    if ( sub_id == 7 ) {
        memcpy( (uint8_t *)&can_batmon7, ptr, sizeof( can_batmon7 ) );
        batmon.h9 = can_batmon7.h9;
        batmon.h11 = can_batmon7.h11;
       }
 }

//*************************************************************************************************
// Заполняет структуры данными контроллера MPPT
// uint8_t sub_id - ID блока данных
// uint8_t *ptr   - указатель на данные
//*************************************************************************************************
static void CanDataMppt( uint8_t sub_id, uint8_t *ptr ) {

    if ( sub_id == 1 ) {
        memcpy( (uint8_t *)&can_mppt1, ptr, sizeof( can_mppt1 ) );
        mppt.power = can_mppt1.power;
        mppt.connect = can_mppt1.connect;
        mppt.link = can_mppt1.link;
        mppt.pv_stat = can_mppt1.pv_stat;
        mppt.pv_mode = can_mppt1.pv_mode;
        mppt.u08_charge_mode = can_mppt1.u08_charge_mode;
        mppt.u12_soc = can_mppt1.u12_soc;
        mppt.u07_time_flt = can_mppt1.u07_time_flt;
        mppt.time_charge = can_mppt1.time_charge;
       }
    if ( sub_id == 2 ) {
        memcpy( (uint8_t *)&can_mppt2, ptr, sizeof( can_mppt2 ) );
        mppt.u01_in_voltage = can_mppt2.u01_in_voltage;
        mppt.u02_in_current = can_mppt2.u02_in_current;
       }
    if ( sub_id == 3 ) {
        memcpy( (uint8_t *)&can_mppt3, ptr, sizeof( can_mppt3 ) );
        mppt.u03_out_voltage = can_mppt3.u03_out_voltage;
        mppt.u04_out_current = can_mppt3.u04_out_current;
       }
    if ( sub_id == 4 ) {
        memcpy( (uint8_t *)&can_mppt4, ptr, sizeof( can_mppt4 ) );
        mppt.u13_bat_current = can_mppt4.u13_bat_current;
       }
    if ( sub_id == 5 ) {
        memcpy( (uint8_t *)&can_mppt5, ptr, sizeof( can_mppt5 ) );
        mppt.u05_energy1 = can_mppt5.u05_energy1;
        mppt.u05_energy2 = can_mppt5.u05_energy2;
       }
    if ( sub_id == 6 ) {
        memcpy( (uint8_t *)&can_mppt6, ptr, sizeof( can_mppt6 ) );
        mppt.u15_bat_temp = can_mppt6.u15_bat_temp;
        mppt.u11_mppt_temp = can_mppt6.u11_mppt_temp;
       }
 }

//*************************************************************************************************
// Заполняет структуры данными 
// uint8_t sub_id - ID блока данных
// uint8_t *ptr   - указатель на данные
//*************************************************************************************************
static void CanDataCharger( uint8_t sub_id, uint8_t *ptr ) {

    if ( sub_id == 1 ) {
        memcpy( (uint8_t *)&can_charger1, ptr, sizeof( can_charger1 ) );
        charger.connect_ac = can_charger1.connect_ac;
        charger.device_ok = can_charger1.device_ok;
        charger.charge_end = (ChargeCompl)can_charger1.charge_end;
        charger.charge_mode = can_charger1.charge_mode;
        charger.charge_exec = can_charger1.charge_exec;
        charger.current = can_charger1.current;
       }
 }

//*************************************************************************************************
// Заполняет структуры данными инвертора TS-1000-224
// uint8_t sub_id - ID блока данных
// uint8_t *ptr   - указатель на данные
//*************************************************************************************************
static void CanDataInv1( uint8_t sub_id, uint8_t *ptr ) {

    if ( sub_id == 1 ) {
        memcpy( (uint8_t *)&can_1inv1, ptr, sizeof( can_1inv1 ) );
        inv1.dc_conn = can_1inv1.dc_conn;
        inv1.mode = can_1inv1.mode;
        inv1.cycle_step = can_1inv1.cycle_step;
        inv1.ctrl_error = can_1inv1.ctrl_error;
        inv1.dev_error = can_1inv1.dev_error;
        inv1.ac_out = can_1inv1.ac_out;
        inv1.power_watt = can_1inv1.power_watt;
        inv1.power_perc = can_1inv1.power_perc;
       }
    if ( sub_id == 2 ) {
        memcpy( (uint8_t *)&can_2inv1, ptr, sizeof( can_2inv1 ) );
        inv1.dc_in = can_2inv1.dc_in;
        inv1.temperature = can_2inv1.temperature;
       }
 }

//*************************************************************************************************
// Заполняет структуры данными инвертора TS3000-224
// uint8_t sub_id - ID блока данных
// uint8_t *ptr   - указатель на данные
//*************************************************************************************************
static void CanDataInv2( uint8_t sub_id, uint8_t *ptr ) {

    if ( sub_id == 1 ) {
        memcpy( (uint8_t *)&can_1inv2, ptr, sizeof( can_1inv2 ) );
        inv2.dc_conn = can_1inv2.dc_conn;
        inv2.mode = can_1inv2.mode;
        inv2.cycle_step = can_1inv2.cycle_step;
        inv2.ctrl_error = can_1inv2.ctrl_error;
        inv2.dev_error = can_1inv2.dev_error;
        inv2.ac_out = can_1inv2.ac_out;
        inv2.power_watt = can_1inv2.power_watt;
        inv2.power_perc = can_1inv2.power_perc;
       }
    if ( sub_id == 2 ) {
        memcpy( (uint8_t *)&can_2inv2, ptr, sizeof( can_2inv2 ) );
        inv2.dc_in = can_2inv2.dc_in;
        inv2.temperature = can_2inv2.temperature;
       }
 }

//*************************************************************************************************
// Заполняет структуры данными контроллера генератора
// uint8_t sub_id - ID блока данных
// uint8_t *ptr   - указатель на данные
//*************************************************************************************************
static void CanDataGen( uint8_t sub_id, uint8_t *ptr ) {

    gen_ptr = &gen;
    //выбор источника данных: локальный/подключен удаленно
    if ( sub_id == 1 ) {
        memcpy( (uint8_t *)&can_gen1, ptr, sizeof( can_gen1 ) );
        gen.remote = can_gen1.remote;
        gen.connect = can_gen1.connect;
        gen.mode = can_gen1.mode;
        gen.stat = can_gen1.stat;
        gen.cycle1 = can_gen1.cycle1;
        gen.cycle2 = can_gen1.cycle2;
        gen.error = can_gen1.error;
        gen.timer_run_inc = can_gen1.timer_run_inc;
        gen.timer_run_dec = can_gen1.timer_run_dec;
       }
    if ( sub_id == 2 ) {
        memcpy( (uint8_t *)&can_gen2, ptr, sizeof( can_gen2 ) );
        gen.timer_lost_acmain = can_gen2.timer_lost_acmain;
        gen.timer_rest_acmain = can_gen2.timer_rest_acmain;
        gen.timer_sleep = can_gen2.timer_sleep;
       }
 }

//*************************************************************************************************
// Заполняет структуры данными трекера
// uint8_t sub_id - ID блока данных
// uint8_t *ptr   - указатель на данные
//*************************************************************************************************
static void CanDataTrc( uint8_t sub_id, uint8_t *ptr ) {

    if ( sub_id == 1 ) {
        memcpy( (uint8_t *)&can_trc1, ptr, sizeof( can_trc1 ) );
        tracker.link = can_trc1.link;
        tracker.pwr_trc = can_trc1.pwr_trc;
        tracker.pwr_act = can_trc1.pwr_act;
        tracker.pwr_fuse = can_trc1.pwr_fuse;
        tracker.stat = can_trc1.stat;
        tracker.time_on = can_trc1.time_on;
       }
    if ( sub_id == 2 ) {
        memcpy( (uint8_t *)&can_trc2, ptr, sizeof( can_trc2 ) );
        tracker.act_pos_vert = can_trc2.act_pos_vert;
        tracker.act_pos_horz = can_trc2.act_pos_horz;
        tracker.act_vert_eep = can_trc2.act_vert_eep;
        tracker.act_horz_eep = can_trc2.act_horz_eep;
       }
 }

//*************************************************************************************************
// Заполняет структуры данными положения солнца
// uint8_t sub_id - ID блока данных
// uint8_t *ptr   - указатель на данные
//*************************************************************************************************
static void CanDataSpa( uint8_t sub_id, uint8_t *ptr ) {

    if ( sub_id == 1 ) {
        memcpy( (uint8_t *)&can_spa1, ptr, sizeof( can_spa1 ) );
        sunpos.sunrise = can_spa1.sunrise;
        sunpos.sunset = can_spa1.sunset;
       }
    if ( sub_id == 2 ) {
        memcpy( (uint8_t *)&can_spa2, ptr, sizeof( can_spa2 ) );
        sunpos.zenith = can_spa2.zenith;
        sunpos.azimuth = can_spa2.azimuth;
       }
    if ( sub_id == 3 ) {
        memcpy( (uint8_t *)&can_spa3, ptr, sizeof( can_spa3 ) );
        sunpos.duration = can_spa3.duration;
        sunpos.error = can_spa3.error;
       }
 }

//*************************************************************************************************
// Заполняет структуры данными параметрами настроек
// uint8_t sub_id - ID блока данных
// uint8_t *ptr   - указатель на данные
//*************************************************************************************************
static void CanDataConfig( uint8_t sub_id, uint8_t *ptr ) {

    if ( sub_id == 1 ) {
        memcpy( (uint8_t *)&can_config1, ptr, sizeof( can_config1 ) );
        memcpy( (uint8_t *)config.scr_file, (uint8_t *)can_config1.scr_file, 8 );
       }
    if ( sub_id == 2 ) {
        memcpy( (uint8_t *)&can_config2, ptr, sizeof( can_config2 ) );
        memcpy( (uint8_t *)config.scr_file + 8, (uint8_t *)can_config2.scr_file, 8 );
       }
    if ( sub_id == 3 ) {
        memcpy( (uint8_t *)&can_config3, ptr, sizeof( can_config3 ) );
        memcpy( (uint8_t *)config.job_file, (uint8_t *)can_config3.job_file, 8 );
       }
    if ( sub_id == 4 ) {
        memcpy( (uint8_t *)&can_config4, ptr, sizeof( can_config4 ) );
        memcpy( (uint8_t *)config.job_file + 8, (uint8_t *)can_config4.job_file, 8 );
       }
    if ( sub_id == 5 ) {
        memcpy( (uint8_t *)&can_config5, ptr, sizeof( can_config5 ) );
        memcpy( (uint8_t *)config.job_test, (uint8_t *)can_config5.job_test, 8 );
       }
    if ( sub_id == 6 ) {
        memcpy( (uint8_t *)&can_config6, ptr, sizeof( can_config6 ) );
        memcpy( (uint8_t *)config.job_test + 8, (uint8_t *)can_config6.job_test, 8 );
       }
    if ( sub_id == 7 ) {
        memcpy( (uint8_t *)&can_config7, ptr, sizeof( can_config7 ) );
        config.mode_sys = can_config7.mode_sys;
        config.log_enable_pv = can_config7.log_enable_pv;
        config.log_enable_chrg = can_config7.log_enable_chrg;
        config.log_enable_mppt = can_config7.log_enable_mppt;
        config.log_enable_inv = can_config7.log_enable_inv;
        config.log_enable_bmon = can_config7.log_enable_bmon;
        config.log_enable_gen = can_config7.log_enable_gen;
        config.log_enable_alt = can_config7.log_enable_alt;
        config.log_enable_trc = can_config7.log_enable_trc;
        config.mode_logging = can_config7.mode_logging;
        config.gen_auto_mode = can_config7.gen_auto_mode;
        config.pb_current_stop = can_config7.pb_current_stop;
        config.delay_start_inv = can_config7.delay_start_inv;
        config.delay_stop_inv = can_config7.delay_stop_inv;
       }
    if ( sub_id == 8 ) {
        memcpy( (uint8_t *)&can_config8, ptr, sizeof( can_config8 ) );
        config.last_charge = can_config8.last_charge;
        config.datlog_upd_chrg = can_config8.datlog_upd_pb;
        config.datlog_upd_mppt = can_config8.datlog_upd_mppt;
       }
    if ( sub_id == 9 ) {
        memcpy( (uint8_t *)&can_config9, ptr, sizeof( can_config9 ) );
        config.datlog_upd_bmon = can_config9.datlog_upd_bmon;
        config.datlog_upd_inv = can_config9.datlog_upd_ts;
        config.datlog_upd_trc = can_config9.datlog_upd_trc;
        config.gen_time_run = can_config9.gen_time_run;
       }
    if ( sub_id == 10 ) {
        memcpy( (uint8_t *)&can_config10, ptr, sizeof( can_config10 ) );
        config.gen_delay_start = can_config10.gen_delay_start;
        config.gen_delay_stop = can_config10.gen_delay_stop;
        config.gen_cnt_start = can_config10.gen_cnt_start;
        config.gen_delay_chk_run = can_config10.gen_delay_chk_run;
        config.gen_before_start = can_config10.gen_before_start;
        config.gen_time_sleep = can_config10.gen_time_sleep;
       }
    if ( sub_id == 11 ) {
        memcpy( (uint8_t *)&can_config11, ptr, sizeof( can_config11 ) );
        memcpy( (uint8_t *)config.gen_time_start, (uint8_t *)can_config11.gen_time_start, sizeof( config.gen_time_start ) );
       }
    if ( sub_id == 12 ) {
        memcpy( (uint8_t *)&can_config12, ptr, sizeof( can_config12 ) );
        config.gen_time_test = can_config12.gen_time_test;
        config.gen_last_run = can_config12.gen_last_run;
        config.spa_timezone = can_config12.spa_timezone;
        config.spa_temperature = can_config12.spa_temperature;
       }
    if ( sub_id == 13 ) {
        memcpy( (uint8_t *)&can_config13, ptr, sizeof( can_config13 ) );
        config.spa_elevation = can_config13.spa_elevation;
        config.spa_pressure = can_config13.spa_pressure;
        config.spa_slope = can_config13.spa_slope;
        config.spa_azm_rotation = can_config13.spa_azm_rotation;
       }
    if ( sub_id == 14 ) {
        memcpy( (uint8_t *)&can_config14, ptr, sizeof( can_config14 ) );
        config.spa_latitude = can_config14.spa_latitude;
       }
    if ( sub_id == 15 ) {
        memcpy( (uint8_t *)&can_config15, ptr, sizeof( can_config15 ) );
        config.spa_longitude = can_config15.spa_longitude;
       }
 }
