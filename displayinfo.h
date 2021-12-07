/*
 *  displayinfo.h
 *
 *  Parses and executes commands received from user input
 *
 *  Created on: Dec 04, 2021
 *  Author: Ainara Machargo
 */

#ifndef DISPLAYINFO_H_
#define DISPLAYINFO_H_

 #include "lcd.h"
 #include "open_interface.h"
 #include "movement.h"
 #include "Timer.h"

/*
 * Function to display a variety of information: Battery percentage, Bumper status, Cliff signal, and distance traveled from user input on the LCD screen
 * @PARAM oi_t sensor data from open interface
 * @PARAM parameter  distance that the roomba will travel input by the user on the GUI
 */
 void displayStats(oi_t *sensor_data, int parameter);

#endif /* DISPLAYINFO_H_*/
