/*
 *  displayinfo.h
 *
 *  Parses and executes commands received from user input
 *
 *  Created on: Dec 05, 2021
 *  Author: Ainara Machargo
 */

#ifndef DISPLAYINFO_H_
#define DISPLAYINFO_H_

 #include "lcd.h"
 #include "open_interface.h"
 #include "movement.h"
 #include "Timer.h"

/*
 * Method used to display stats on Roomba's LCD
 * This method will display battery percentage, cliff's front left signal, bump status (left, right, both or none) and distance travveled
 * PARAM: sensor data
 * PARAM: distance desired to travel
 */
 void displayStats(oi_t *sensor_data, int parameter);

#endif /* DISPLAYINFO_H_*/
