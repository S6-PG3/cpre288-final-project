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

void scan_full(scan_t *get_scan);

void scan_sendData(scan_t *get_scan, int angle);

void scan_objects(scan_t *get_scan, object *objectArray);

void scan_sendObjects();

#endif /* SCAN_H_ */
