/*
 *  scan.c
 *
 *  Small program to calibrate the servo motor.
 *  User moves servo to 0 and 180 degrees and sets those values to
 *  right_calibration_value and left_calibration_value in main.c
 *
 *  Note: Intended to run entirely independent
 *      _SERVO_CALIBRATION 1
 *      _IR_CALIBRATION 0
 *      _MAIN 0
 *
 *  Created on: Nov 27, 2021
 *  Author: James Minardi
 *
 */

#include "scan.h"

// Clock cycles to increment for fast and slow modes
#define FAST_INCREMENT 512
#define SLOW_INCREMENT 128

void servo_calibration(void)
{
    // Initializations for servo calibration
    timer_init();
    lcd_init();
    servo_init1();
    button_init();

    // Local variables to display each calibration value on the lcd
    int right_value;
    int left_value;

    // fast=0 increments slow; fast=1 increments fast
    int fast = 0;

    // right=1 calibrating right (0 degrees); right=0 calibrating left (180 degrees)
    int right = 1;

    /*
     * Sets servo to the theoretical 0 degrees position.
     * Must disable and reenable the timer b before adjusting the match values
     */
    int match = 0x4A380;                // Theoretical 0 degrees: 304,000 (1ms = 0 degrees)
    TIMER1_CTL_R &= 0b011111111;        // Disable timer b
    TIMER1_TBMATCHR_R = match;
    TIMER1_TBPMR_R = match >> 16;
    TIMER1_CTL_R |= 0b100000000;        // Enable timer b

    // Loop infinitely while user presses lcd buttons to adjust the servo motor to its instructions
    lcd_printf("1=L 2=R 3=Cont.\nRight Val: %d", match);
    while (1)
    {

        int button = button_getButton();
        if (button == 1)                // Move left
        {
            match = fast ? match - FAST_INCREMENT : match - SLOW_INCREMENT;
        }
        else if (button == 2)           // Move right
        {
            match = fast ? match + FAST_INCREMENT : match + SLOW_INCREMENT;
        }
        else if (button == 3)           // Confirm
        {
            if (right == 1)
            {
                right_value = match;
                right = 0;
                lcd_printf("1=L 2=R 3=Done\nLeft Val: %d", match);
                timer_waitMillis(1000); // Waiting 1s to not double click and accidentally finish program
            }
            else if (right == 0)
            {
                // Break when adjusting left and it is confirmed
                left_value = match;
                lcd_printf("Right: %d\nLeft: %d", right_value, left_value);
                timer_waitMillis(500);
                break;
            }
        }
        else if (button == 4)           // Swap speed to fast or slow
        {
            fast = fast ? 0 : 1;
            timer_waitMillis(500);
        }

        // Update match value
        TIMER1_CTL_R &= 0b011111111;    // Disable timer b
        TIMER1_TBMATCHR_R = match;
        TIMER1_TBPMR_R = match >> 16;
        TIMER1_CTL_R |= 0b100000000;    // Enable timer b

        // Display current calibration value
        if (right)
        {
            lcd_printf("1=L 2=R 3=Cont.\nRight Val: %d", match);
        }
        else if (right == 0)
        {
            lcd_printf("1=L 2=R 3=Done\nLeft Val: %d", match);
        }
        timer_waitMillis(200);
    } // end while(1)

    lcd_printf("Right: %d\nLeft: %d", right_value, left_value);
} // END servo_calibration

void IR_calibration(void) {

    servo_init1();
    adc_init();
    ping2_init();

    while (1) {

        float sound_dist = ping_read();
        adc_read();

        lcd_printf("sound: %f\nIR: %d\n", sound_dist, IR_raw_sample);
        timer_waitMillis(1000);

    }
} // END IR_calibration

