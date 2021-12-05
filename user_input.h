/*
 *  user_inptut.h
 *
 *  Parses and executes commands received from user input
 *  
 *  Created on: Nov 30, 2021
 *  Author: Ainara Machargo, James Minardi
 */

#ifndef USER_INPUT_H_
#define USER_INPUT_H_

#include <stdio.h>
#include <stdlib.h>
#include "open_interface.h"
#include "movement.h"
#include "scan.h"
#include "uart.h"
#include "object.h"

object objectArray[20];

/* 
 * Parses given char pointer by the following formats:
 * 		"[Function],[Parameter]\0"
 *		"[Function]\0"
 * [Function] is a single char representing a function to complete: W,A,S,D,M,Z
 * [Parameter] is an integer value to give to the desired function: Distance or Angle
 * 
 *  @param command - Null terminated char pointer to parse
 */
void ui_parse(oi_t *sensor_data, scan_t *get_scan, char *command);

/*
 * Execute desired function with given integer parameter.
 * 		w: Move forward, s: move backward, a: rotate counter clockwise, d: rotate clockwise
 *		m: scan 0-180 degrees or specified degrees
 *		z: finish program
 *	
 * @param function - Single char to represent desired instructions to execute
 * 		  parameter - Integer value to give the function call as a distance or angle
 */
void ui_execute(oi_t *sensor_data, scan_t *get_scan, char function, int parameter);

#endif /* USER_INPUT_H_ */
