/*
 *   uart.h
 *
 *   UART capability using 115200 baud rate
 *
 *   Created on: Nov 30, 2021
 *   Author: James Minardi, Danny Cao
 */

#ifndef UART_H_
#define UART_H_

#include "Timer.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"

/*
 * Data received from UART
 */
volatile char uart_data;

/*
 * When flag is 1, there is data to be read in uart_data
 * Must set flag to 0 when uart_data is read
 */
volatile int uart_receive_flag;

/*
 * Initialize UART functionality using GPIO Port B wires 0:1 and UART1
 */
void uart_init(void);

/*
 * Initializes UART interrupts for receive
 */
void uart_interrupt_init();

/*
 * Interrupt handler for UART 1
 */
void uart_interrupt_handler();

/*
 * Transmits data through UART 1
 *
 * @param data - 8 bits of data to send
 */
void uart_sendChar(char data);

/*
 * Receives data from UART 1
 *
 * @return data - 8 bits of data to receive
 */
char uart_receive(void);

/*
 * Transmit a string to UART1
 * Terminates at null character
 * Similar to lcd_puts from lcd.c
 *
 * @param data - String pointer
 */
void uart_sendStr(const char *data);

#endif /* UART_H_ */
