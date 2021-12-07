/*
 *  servo.c
 *
 *  Servo motor functionality
 *
 *  Created on: Nov 2, 2021
 *  Author: James Minardi
 *
 */

#include "servo.h"

/*
 *  Initialize servo motor using Port B pin 5, and Timer 1B
 *  Timer 1B is set to split 16-bit count down, periodic, PWM timer
 */
void servo_init(void)
{
    // System enable
    SYSCTL_RCGCGPIO_R |= 0b000010;      // enable port B clock
    SYSCTL_RCGCTIMER_R |= 0b000010;     // enable Timer 1 clock

    // GPIO Init
    GPIO_PORTB_AFSEL_R |= 0b100000;     // Sets PB5 to alternate function
    GPIO_PORTB_PCTL_R |= 0x00700000;    // Selects timer 1 (T1CCP1)
    GPIO_PORTB_PCTL_R &= 0xFF7FFFFF;
    GPIO_PORTB_DEN_R |= 0b00100000;     // enable PB5 digital function
    GPIO_PORTB_DIR_R |= 0b00100000;     // Set PB5 to output

    // Timer Init
    TIMER1_CTL_R &= 0b011111111;        // disable timer1B to config
    TIMER1_CFG_R |= 0x4;                //Use split 16-bit timers
    TIMER1_TBMR_R |= 0b01010;           //Set as count down, periodic, PWM mode
    TIMER1_TBMR_R &= 0b01010;
    TIMER1_TBPR_R = 0x04;               // Loads MSBs of 0x4E200 (320,000)
    TIMER1_TBILR_R = 0xE200;            // Loads 8 LSBs of 0x4E200 (Period = 320,000 clock cycles)

    TIMER1_CTL_R |= 0b100000000;        // Enable timer b

    servo_move(0);                      // Initialize to theoretical 0 degrees
    timer_waitMillis(1000);
} // END servo_init

/*
 * Moves servo motor to specified degrees.
 *
 * @param angle - Degrees to move servo motor to
 * @return match - Value for the specified degrees for debugging
 */
int servo_move(int angle)
{
    if (angle < 0)    // For situations where input degrees may be less than 0
    {
        angle = 0;
    }
    if (angle > 180)  // For situations where input degrees may be more than 0
    {
        angle = 180;
    }

    int match = angle_to_match(angle); // Convert degrees to a timer match value

    TIMER1_CTL_R &= 0b011111111;        // Disable timer
    TIMER1_TBMATCHR_R = match;          // Set match value for specified degrees
    TIMER1_TBPMR_R = match >> 16;       // Set prescale value
    TIMER1_CTL_R |= 0b100000000;        // Enable timer

    // Experimental formula to determine time to wait depending on how far the servo has to move
    timer_waitMillis(4 * abs(angle - current_angle) + 20); // Increase b value (ax+b) if having troubles

    // Update currentAngle
    current_angle = angle;
    return match;
}  // END servo_move

/**
 * Converts degrees to match value for the timer. Uses the left_calibration_value and right_calibration_value set by the user by using
 * servo_calibration() in scan.h
 *
 * @param angle - Degrees to convert to timer match value
 * @return timer - Match value for @param degrees
 */
int angle_to_match(int angle)
{
    float slope = (left_calibration_value - right_calibration_value) / 180.0;
    return (slope * angle) + right_calibration_value;
}  // END degrees_to_match

