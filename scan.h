/*
 *  scan.h
 *
 *  Created on: Nov 27, 2021
 *  Author: James Minardi
 *
 */

#ifndef SCAN_H_
#define SCAN_H_

#include "adc.h"
#include "ping.h"
#include "servo.h"
#include "uart.h"
#include "object.h"

// Scan value
typedef struct{
    float sound_dist;   // Distance from Ping SensorS
    float IR_dist;        // Distance from IR sensor
} scan_t;


/*
 * Moves servo motor to angle, and gets IR and Sonar distances for that angle
 *
 * @params
 *      angle - Direction to point the sensors to
 *      getScan - Location of a scan_t struct to store scan values
 */
void scan(scan_t *get_scan, int angle);

/*
 * Method used to perform a "scan" from 0 to 180 degrees.
 * When called, the servo is set a 0 degrees and returns
 * the IR_distance and the Sound_distance. It increments
 * by one degree and collects the same information.
 */
void scan_full(scan_t *get_scan);

/*
 * Method used to communicate data colleted from sensors to the GUI interface
 * for user analysis. The method sends the angle and the IR distance to the GUI.
 */
void scan_sendData(scan_t *get_scan, int angle);

/*
 * Called when users needs to scan for objects present on the field.
 * Similar to "scan_full" the method scans from 0 to 180 degrees, but
 * collects information based on:
 *              1. Object starting angle
 *              2. Distance (based on sound distance)
 *              3. Radial widith
 *              4. Linear width
 */
void scan_objects(scan_t *get_scan, object *objectArray);


/*
 * Similar function to the scan_sendData method in which
 * this method sends the collected information of the
 * objects detected on the field to the GUI.
 */
void scan_sendObjects(object *objectArray);

#endif /* SCAN_H_ */
