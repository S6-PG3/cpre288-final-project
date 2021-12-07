/*
 *  calibration.h
 *
 *  Collection of small programs to calibrate different cyBots.
 *  Currently supports semi-automatic calibration of the servo motor
 *  and manual calibration of the IR sensor.
 *
 *  Each method is a standalone program meant to be ran independent of others.
 *
 *  To enable a program, set one of the the macros in main.c to 1 and leave all other 0:
 *      _SERVO_CALIBRATION 0
 *      _IR_CALIBRATION 0
 *      _MAIN 0
 *
 *  Created on: Nov 30, 2021
 *  Author: James Minardi
 */

#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include "button.h"
#include "lcd.h"
#include "servo.h"
#include "Timer.h"
#include "ping.h"
#include "adc.h"
#include "open_interface.h"
#include "scan.h"

/*
 *  Small program to calibrate the servo motor.
 *  User moves servo to 0 and 180 degrees and sets those values to
 *  right_calibration_value and left_calibration_value in main.c
 *
 *  Configuration in main.c:
 *      _SERVO_CALIBRATION 1
 *      _IR_CALIBRATION 0
 *      _MAIN 0
 *
 */
void servo_calibration(void);

/*
 *  Small program to calibrate the IR sensor.
 *  User moves cyBot in front of flat surface and records the IR_raw_value at increments
 *  of 5-10cm from 0 to 50cm. Using a graphing calculator, the user will find a best fit
 *  inverse function. They will then enable and implement the equation in scan.c to the
 *  corresponding bot.
 *
 *  Input the IR_raw_value as the y-axis and distance (cm) as the x-axis in a graphing calculator like desmos.
 *
 *  In Desmos, use this equation:
 *
 *      y1 ~ a/(x1+b) + c
 *          y1 is distance in cm
 *          x1 is raw ir value
 *      b and c are typically negative
 *
 *  Configuration in main.c:
 *      _SERVO_CALIBRATION 0
 *      _IR_CALIBRATION 1
 *      _MAIN 0
 */
void IR_calibration(void);

#endif /* CALIBRATION_H_ */
