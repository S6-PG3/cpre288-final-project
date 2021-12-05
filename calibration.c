/*
 *  calibration.c
 *
 *  Collection of small programs to calibrate different cyBots.
 *  Currently supports semi-atomatic calibration of the servo motor
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

#include "calibration.h"

// Clock cycles to increment for fast and slow modes
#define FAST_INCREMENT 512
#define SLOW_INCREMENT 128

void bot_calibration(void)
{

}

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
void servo_calibration(void)
{
    // Initializations for servo calibration
    timer_init();
    lcd_init();
    servo_init();
    button_init();

    // Local variables to display each calibration value on the lcd
    int right_value;
    int left_value;

    // fast=0 increments slow; fast=1 increments fast
    int fast = 0;

    // right=1 calibrating right (0 degrees); right=0 calibrating left (180 degrees)
    int right = 1;

    //Sets servo to the theoretical 0 degrees position.
    //Must disable and reenable the timer b before adjusting the match values
    int match = 0x4A380;     // Theoretical 0 degrees: 304,000 (1ms = 0 degrees)
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
                timer_waitMillis(1000); // Wait 1s to avoid the user from accidentally completing the program by pressing too long
            }
            else if (right == 0)
            {
                // Break from loop when left value is confirmed by user
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
 *      b and c are typcally negative
 *
 *  Configuration in main.c:
 *      _SERVO_CALIBRATION 0
 *      _IR_CALIBRATION 1
 *      _MAIN 0
 */
void IR_calibration(void)
{

    servo_init();
    adc_init();
    ping_init();
    lcd_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    servo_move(90);
    while (1)
    {

        float sound_dist = ping_read();
        int IR_raw_sample = adc_read();
        float IR_dist = adc_to_cm(adc_read());

        lcd_printf("sound: %.4f\nraw: %d\nir: %.4f\nbattery: %.2f%%", sound_dist, IR_raw_sample, adc_to_cm(IR_raw_sample), (float) (sensor_data->batteryCharge * (1.0)));
                   /// sensor_data->batteryCapacity) * 100.0);
        timer_waitMillis(1000);

    }
} // END IR_calibration

