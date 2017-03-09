#ifndef __INFO_LIST_H
#define __INFO_LIST_H

#define ROBOTS 8
#define GROBOTS 8

#define CONFIG_ROBOT1

#ifdef CONFIG_ROBOT1 
//mag sensor para
#define rbID            (1)
#define MAG_SENSOR_X    (-15)
#define MAG_SENSOR_Y    (-165)
#elif defined(CONFIG_ROBOT2)
#define rbID            (2)
#define MAG_SENSOR_X    (200)
#define MAG_SENSOR_Y    (793)
#endif

#define FRONT2X            (63) 

#endif