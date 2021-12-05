/*
 *  scan.c
 *
 *  Created on: Nov 27, 2021
 *  Author: James Minardi
 *
 */

#include "scan.h"

extern volatile unsigned int rising_time;
extern volatile unsigned int falling_time;


void scan(scan_t *get_scan, int angle)
{
    servo_move(angle);
    int IR_raw_sample = adc_read();
    get_scan->IR_dist = adc_to_cm(IR_raw_sample);
    get_scan->sound_dist = ping_read();


}

void scan_full(scan_t *get_scan)
{
    int current_angle = 0;
    scan(get_scan, 0);

    while (current_angle <= 180)
    {
        scan(get_scan, current_angle);
        scan_sendData(get_scan, current_angle);
        current_angle++;
    }
}

void scan_sendData(scan_t *get_scan, int angle)
{
    char temp[50];
    sprintf(temp, "%d,%lf;",angle, get_scan->IR_dist);
    uart_sendStr(temp);
}

void scan_objects(scan_t *get_scan){
    // "{center angle},{distance},{linearwidth};" "50,2.6435345.863060;
}
