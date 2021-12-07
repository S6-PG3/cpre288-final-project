/*
 *  ping.h
 *
 *  Created on: Oct 26, 2021
 *  Author: James Minardi, Danny Cao
 */

#ifndef PING_H_
#define PING_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include "lcd.h"

enum states
{
    LOW, HIGH, DONE
}; // Set by ISR

extern int BOT; // main.c

volatile enum states state;
/*
 * Pulse start time set by ISR
 */
volatile unsigned int rising_time;
/*
 * Pulse end time set by ISR
 */
volatile unsigned int falling_time;

/*
 * Method used to initialize ping sensor's GPIO registers and timer
 * registers based on settings such as capture mode, selecting timer#,
 * selecting both positive and negative edge detection, etc.
 */
void ping_init(void);

/*
 * Method to initialize and start up timer interrupt registers.
 */
void ping_interrupt_init(void);

/*
 * Called when rising or falling event occurs for Timer
 */
void ping_timer3b_handler(void);

/*
 * Finds the time it took for the ping to transmit and receive. Converts that value into distance (cm).
 *
 * @return distance - distance in cm
 */
float ping_read(void);

/*
 * Method used to send an output signal or "trigger"
 * in order for signal to bounce off of object
 */
void ping_send_trigger(void);

#endif /* PING_H_ */
