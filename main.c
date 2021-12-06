/**
 * main.c
 */

// Run each independently of each other
#define _SERVO_CALIBRATION 0
#define _IR_CALIBRATION 0
#define _MAIN 1

#include <stdio.h>
#include "open_interface.h"
#include "scan.h"
#include "adc.h"
#include "lcd.h"
#include "Timer.h"
#include "servo.h"
#include "ping.h"
#include "calibration.h"
#include "scan.h"
#include "uart.h"
#include "user_input.h"

extern int current_angle; // servo.h
extern volatile int uart_receive_flag;

/*
 * Run _SERVO_CALIBRATION by itself to find values for these. Currently set for bot06
 */
int right_calibration_value = 311040;
int left_calibration_value = 281728;
int BOT = 8;

scan_t get_scan;
oi_t *sensor_data;

void clear_array(char* command);

int main(void)
{

#if _SERVO_CALIBRATION
    servo_calibration();
#endif // _SERVO_CALIBRATION

#if _IR_CALIBRATION
    IR_calibration();
#endif // _IR_CALIBRATRION

#if _MAIN
    timer_init();
    lcd_init();
    uart_init();
    servo_init();
    adc_init();
    ping_init();
    button_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    char command[20];
    clear_array(command);

    int cmd_index = 0;
    while (1)
    {
        if (uart_receive_flag == 1)
        {
            uart_receive_flag = 0;
            command[cmd_index] = uart_data;
            //lcd_putc(uart_data);
            if (command[cmd_index] == '\0')
            {

                lcd_printf("%s", command);
                ui_parse(sensor_data, &get_scan, command);
                clear_array(command);
                cmd_index = 0;
            }
            else {
                cmd_index++;
            }

        }
    }

#endif // _MAIN
}

/*
 * Clears given array with null characters
 * @param command - pointer to first
 */
void clear_array(char* command)
{
    int cmd_index = 0;
    while (cmd_index < 15) {
        *command = 0;
        command++;
        cmd_index++;
    }
}
