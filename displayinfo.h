/*
 *  displayinfo.h
 *
 *  Parses and executes commands received from user input
 *
 *  Created on: Nov 30, 2021
 *  Author: Ainara Machargo, James Minardi
 */

#ifndef DISPLAYINFO_H_
#define DISPLAYINFO_H_

 #include "lcd.h"
 #include "open_interface.h"
 #include "movement.h"
 #include "Timer.h"

 void displayStats(oi_t *sensor_data, int parameter);

#endif /* DISPLAYINFO_H_*/
