
#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>

#include "scheduler_def.h"

//Команды управления заданиями
typedef enum {
    JOBS_VIEW,                              //просмотр заданий
    JOBS_ADD,                               //добавление задания
    JOBS_DEL,                               //удаление задания
    JOBS_ON,                                //включение задания
    JOBS_OFF,                               //выключение задания
    JOBS_RUN                                //задания готовые в выполнению (без параметров запуска)
 } JobsCmnd;

//Команды управления заданиями
typedef enum {
    JOBS_LOAD_CHECK,                        //загрузка файла с заданиями с проверкой изменения имени файла
    JOBS_LOAD_FORC                          //загрузка файла с заданиями без проверок
 } LoadMode;

//*************************************************************************************************
// Функции управления
//*************************************************************************************************
void JobInit( void );
void LoadJobs( LoadMode mode );
void JobsView( void );

#endif
