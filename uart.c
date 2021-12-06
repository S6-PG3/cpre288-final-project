/*
 *   uart.c
 *
 *   UART capability using 115200 baud rate
 *
 *   Created on: Nov 30, 2021
 *   Author: James Minardi, Danny Cao
 */

#include "uart.h"

/*
 * Initialize UART functionality using GPIO Port B wires 0:1 and UART1
 */
void uart_init(void)
{
    SYSCTL_RCGCGPIO_R |= 0b00000010;        // Enable GPIO Port B clock
    SYSCTL_RCGCUART_R |= 0b00000010;        // Enable UART 1 clock
    GPIO_PORTB_AFSEL_R = 0b00000011;        // Set Port B Wires 0:1 as alternate function (peripherals)
    GPIO_PORTB_PCTL_R  = 0x00000011;        // Use PB0 as UART receive and PB1 as UART transmit
    GPIO_PORTB_DEN_R   = 0b00000011;        // Enable PB0 and PB1
    GPIO_PORTB_DIR_R   = 0b00000001;        // Set PB0 as input and PB1 as output

    // Compute baud registers using 16MHz clock cycle, and 115200 baud rate
    double fbrd;
    int ibrd;
    fbrd = 16000000.0 / (16.0 * 115200.0);
    ibrd = (int) fbrd;
    fbrd = (int) ((fbrd - ibrd) * 64 + 0.5);

    UART1_CTL_R &= 0xFE;                    // Disable UART1 for configuration
    UART1_IBRD_R = ibrd;                    // Integer part of BRD into IBRD
    UART1_FBRD_R = fbrd;                    // Fractional part of BRD into FBRD
    UART1_LCRH_R = 0b01100000;              // Line control: 8 Bit, no parity
    UART1_CC_R = 0b0000;                    // Use system clock as clock source
    UART1_CTL_R |= 0b1100000001;            // Renable UART1
    // TODO: UART1_DR_R = 0x00; Doesn't work? Look into this

    uart_interrupt_init();                  // Enable receive interrupt

} // END uart_init

/*
 * Initializes UART interrupts for receive
 */
void uart_interrupt_init(void)
{
    UART1_CTL_R &= 0b0111111110;            // Disable UART1 for configuration
    UART1_ICR_R |= 0b11111110000;           // Clear interrupt flags
    UART1_IM_R  |= 0b010000;                // Enable interrupt on receive
    NVIC_EN0_R  &= 0x00000040;              // Enable UART 1 interrupts
    NVIC_EN0_R  |= 0x00000040;              // ""

    // Find the vector number of UART1 in table 2-9 ! UART1 is 22 from vector number page 104
    IntRegister(INT_UART1, uart_interrupt_handler); // Set interrupt handler to uart_interrupt_handler
    IntMasterEnable();

    UART1_CTL_R |= 0b1100000001;            // Renable UART1

} // END uart_interrupt_init

/*
 * Interrupt handler for UART 1
 */
void uart_interrupt_handler(void)
{
    if (UART1_MIS_R & 0b010000)                 // Check for UART receive interrupt
    {
        uart_data = (char) (UART1_DR_R & 0xFF); // Store received data
        uart_receive_flag = 1;                  // Data received flag enable
        UART1_ICR_R = UART1_ICR_R | 010000;     // Clear receive bit interrupt
    }
} // END uart_interrupt_handler

/*
 * Transmits data through UART 1
 *
 * @param data - 8 bits of data to send
 */
void uart_sendChar(char data)
{
    while (UART1_FR_R & 0b00100000 == 0)        // Wait while transmit FIFO is full
    {
    }
    UART1_DR_R = data;                          // Send data
    timer_waitMillis(2);                        // Does not work without

} // END uart_sendChar

/*
 * Receives data from UART 1
 *
 * @return data - 8 bits of data to receive
 */
char uart_receive(void)
{
    char data = 0;
    while (UART1_FR_R & 0b00010000)             // Wait while receive FIFO is empty
    {
    }

    data = (char) (UART1_DR_R & 0xFF);          // Mask 4 error bits and grab only 8 data bits

    return data;

} // END uart_receive

/*
 * Transmit a string to UART1
 * Terminates at null character
 * Similar to lcd_puts from lcd.c
 *
 * @param data - String pointer
 */
void uart_sendStr(const char *data)
{
    while (*data != '\0')                       // Terminate at null character
    {
        uart_sendChar(*data);
        data++;                                 // Move to next char
    }
} // END uart_sendStr
