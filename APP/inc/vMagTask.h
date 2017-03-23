/**
 * @file vMagTask.h
 * @brief Mag sensor parameters calibration.
 * @since March 5, 2017
 * @author Jinlin CHEN
 **/
#ifndef __VLEDTASK_H
#define __VLEDTASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "mrslib.h"
#include "vInfoList.h"

extern void vMagTask( void *pvParameters );
extern void vBCastMagTask( void *pvParameters );
extern void magSenPara();
#endif

