/*
 *  ping.c
 *
 *  Created on: Oct 26, 2021
 *  Author: James Minardi, Danny Cao
 */

#include "ping.h"
#include "lcd.h"

/*
 * Method used to initialize ping sensor's GPIO registers and timer
 * registers based on settings such as capture mode, selecting timer#,
 * selecting both positive and negative edge detection, etc.
 */
void ping_init(void)
{
    state = LOW;
    //GPIO Initialization
    SYSCTL_RCGCGPIO_R |= 0b000010;          //enable port B clock
    SYSCTL_RCGCTIMER_R |= 0b001000;         // enable Timer clock

    GPIO_PORTB_AFSEL_R &= 0b11110111;       //disable PB3 as alternate function, set to GPIO

    // Sets PB3 to GPIO output
    GPIO_PORTB_PCTL_R |= 0x00007000;        // Selects timer (T3CCP1)
    GPIO_PORTB_PCTL_R &= 0xFFFF7FFF;

    GPIO_PORTB_DEN_R |= 0b00001000;         // enable PB3 digital function
    GPIO_PORTB_DIR_R |= 0b00001000;         // Set PB3 to output

    TIMER3_CTL_R &= 0b011111111;            // disable timer3 B to config interrupts

    //Initialize Timer Registers
    TIMER3_CTL_R |= 0b110000000000;         //Enable both edge trigger for T(b)EVENT
    TIMER3_CFG_R |= 0x4;                    //Use split 16-bit timers
    TIMER3_TBPR_R |= 0xFF;
    TIMER3_TBMR_R |= 0b0111;                //Set for Capture mode
    TIMER3_TBMR_R &= 0b0111;
    TIMER3_TBILR_R |= 0xFFFF;               //Read from both TBPS and TBR and shift

    ping_interrupt_init();

} // END ping_init

/*
 * Method to initilize and start up timer interrupt registers.
 */
void ping_interrupt_init(void)
{
    TIMER3_ICR_R |= 0b010000000000;         // Clears interrupt flags
    TIMER3_IMR_R |= 0b010000000000;         // enable capture interrupt for timer B

    NVIC_EN1_R |= 0b00010000;               // Enable 16/32bit timer3B interrupt in NVIC_EN#_R (int# 36)

    NVIC_PRI9_R |= 0x20;

    IntRegister(INT_TIMER3B, ping_timer3b_handler); // Bind ISR
    IntMasterEnable();

    TIMER3_CTL_R |= 0b100000000;            // Reenable timer3 B

} // END ping_interrupt_init

/*
 * Called when rising or falling event occurs for Timer
 */
void ping_timer3b_handler(void)
{
    if (state == LOW)                       // If current state is low, set state to high and store start time
    {
        rising_time = TIMER3_TBR_R;
        state = HIGH;
    }
    else if (state == HIGH)                 // If current state is high, set state to done and store end time
    {
        falling_time = TIMER3_TBR_R;
        state = DONE;
    }

    TIMER3_ICR_R |= 0b010000000000;         // Clear timer3B capture mode event interrupt

} // END ping_timer3b_handler

/*
 * Finds the time it took for the ping to transmit and receive. Converts that value into distance (cm).
 *
 * @return distance - distance in cm
 */
float ping_read(void)
{
    ping_send_trigger(); // Send ping
    while (1)
    {
        if (state == DONE)
        {
            float distance = 0;
            long delta_clocks = 0;
            state = LOW;

            if (falling_time > rising_time) // Overflow occurred
            {
                delta_clocks += (long) 1 << 24;
            }
            delta_clocks += (rising_time - falling_time); // change in time in clock cycles

            if (BOT == 6 || BOT == 11) // Hand calibrated sonar distance for BOT 06
            {
                distance = (delta_clocks - 1599.24) / 926.907;
            }
            if else (BOT == 8) {
                distance = (delta_Clocks - 959.653) / 938.964)
            }
            else // Theoretical distance when speed of sound is 340 m/s
            {
                float delta_time = (float) delta_clocks * 0.0000625; // change in time in milliseconds
                distance = (delta_time * ((340.0 * 100.0) / 1000.0) / 2.0); // distance in centimeters
            }

//            lcd_printf("dCC: %d\nDist: %.4f", (int)delta_clocks, distance); // Debugging print method

            return distance;

        }
    }

} // END ping_read

/*
 * Method used to send an output signal or "trigger"
 * in order for signal to bounce off of object
 */
void ping_send_trigger(void)
{
    TIMER3_IMR_R &= 0b101111111111;         // Mask Timer B Capture Mode Event Interrupt (disable)

    // Set PB3 to GPIO output device
    GPIO_PORTB_AFSEL_R &= 0b11110111;       //disable PB3 as alternate function, set to GPIO
    GPIO_PORTB_DIR_R |= 0b00001000;         // Set PB3 to output

    // TRIGGER
    GPIO_PORTB_DATA_R |= 0b00001000;        //PB3 set to high (1)
    timer_waitMillis(0.01);                 //wait 10 microseconds
    GPIO_PORTB_DATA_R &= 0b11110111;        //PB3 set to low (0)

    // Set PB3 back to TIMER input device
    GPIO_PORTB_AFSEL_R |= 0b00001000;       // set PB3 to alternate function (Timer)
    GPIO_PORTB_DIR_R &= 0b11110111;         // Set PB3 to input

    TIMER3_ICR_R |= 0b010000000000;         // Clear timer3B capture mode event interrupt
    TIMER3_IMR_R |= 0b010000000000;         // Unmask timer3B capture mode event interrupt

} // END ping_send_trigger
