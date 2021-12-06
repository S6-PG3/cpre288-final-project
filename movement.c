/*
 * movement.c
 *
 *
 * Collection of programs used to navigate robot and peform some obsacle detection.
 *
 *  Created on: Nov 16, 2021
 *      Author: dannycao
 */

#include "open_interface.h"
#include "object.h"
#include "Timer.h"
#include "lcd.h"
#include "movement.h"

/*
 *  Method used to navigate robot forward.
 *  User gives a value (in cm) to which
 *  value gets converted in mm and is stored
 *  back in "centimeters".
 *
 */
float move_forward(oi_t *sensor_data, int centimeters)
{
    centimeters *= 10; //convert centimeters into millimeters

    double sum = 0;
    oi_update(sensor_data);
    oi_setWheels(100, 100); //Move forward, "500 is max speed"
    while (sum < (centimeters)) //Stay in loop until sum has reached target user input
    {
        oi_update(sensor_data);
        sum += sensor_data->distance;

        obstacle = obstacle_check(sensor_data);
        if (obstacle_check != 0) {
            break;
        }
    }

    oi_setWheels(0, 0); //STOP once distance has been reached
    return sum / 10;
}

/*
 *  Method used to navigate robot backwards.
 *  User gives a value (in cm) to which
 *  value gets converted in mm and is stored
 *  back in "centimeters".
 *
 */
void move_backward(oi_t *sensor_data, int centimeters)
{
    centimeters *= 10;

    double sum = 0;
    oi_update(sensor_data);
    while (sum < (centimeters))
    {
        oi_setWheels(-100, -100); //Move backwards, "-500 is max speed"
        oi_update(sensor_data);

        sum -= sensor_data->distance;
    }
    oi_setWheels(0, 0);
}

/*
 *  Method used to rotate robot clockwise.
 *  User gives an angle to which the robot will
 *  continue to rotate right until "sum" is greater
 *  than the given input angle
 *
 */
int rotate_clockwise(oi_t *sensor_data, int degrees)
{
    double sum = 0;
    oi_setWheels(-30, 30); //Set wheel speed opposite of each other to acheive clockwise rotation

    while (sum < degrees)
    {
        oi_update(sensor_data);
        sum -= (sensor_data->angle); //Continue to rotate until it has reached target degree
    }
    oi_setWheels(0, 0);
    return sum;
}

/*
 *  Method used to rotate robot counterclockwise.
 *  User gives an angle to which the robot will
 *  continue to rotate left until "sum" is greater
 *  than the given input angle
 *
 */
int rotate_counterClockwise(oi_t *sensor_data, int degrees)
{
    double sum = 0;
    oi_setWheels(30, -30); //Rotate counter clockwise
    while (sum < degrees)
    {
        oi_update(sensor_data);
        sum += (sensor_data->angle);
    }
    oi_setWheels(0, 0);
    return sum;
}

/*
 * Method to send GUI information of robot's distance traveled so far.
 *
 *      Information sent:
 *              - distance
 *              - obstacle# (if any were detected)
 */
void send_distanceTraveled(float distance, int obstacle){
    char temp[20];
    sprintf(temp, "%f,%d", distance, obstacle);
    uart_sendStr(temp);
}

/*
 * Method to send GUI information of robot's angle.
 *
 *      Information sent:
 *              - angle
 *              - obstacle# (if any were detected)
 */
void send_angleRotated(int angle, int obstacle){
    char temp[20];
    sprintf(temp, "%d,%d", angle, obstacle);
    uart_sendStr(temp);
}

/*
 *  A large method to detect obstacles during movement of the robot.
 *  Utilizes bump sensors and cliff sensors to detect obstacles such as holes,
 *  boundary lines, and small objects undetectable from IR and PING sensors. Method
 *  also includes manuevering program when sensors have been triggered to assist user
 *  with navigation.
 *
 *     Method Returns:
 *          0 : nothing
 *          1 : left bump sensor triggered
 *          2 : right bump sensor triggered
 *          3 : left cliff sensor detected boundary
 *          4 : left cliff sensor detected hole
 *          5 : Front left cliff sensor detected boundary
 *          6 : Front left cliff sensor detected hole
 *          7 : Front right cliff sensor detected boundary
 *          8 : Front right cliff sensor detected hole
 *          9 : Right cliff sensor detected boundary
 *         10 : Right cliff sensor detected hole
 *
 *     Method Returns: UPDATED?
 *          0 : nothing
 *          1 : left bump sensor
 *          2 : left & right bump sensor
 *          3 : right bump sensor
 *          4 : left cliff sensor detected boundary
 *          5 : Front left cliff sensor detected boundary
 *          6 : Front right cliff sensor detected boundary
 *          7 : Right cliff sensor detected boundary
 *          8 : left cliff sensor detected hole
 *          9 : Front left cliff sensor detected hole
 *         10 : Front right cliff sensor detected hole
 *         11 : Right cliff sensor detected hole
 *
 *         "{Distance or Angle traveled},{obstacle val}\0"
 *
 *
 */
int obstacle_check(oi_t *sensor_data)
{
//**************BUMPER SENSORS*******************************************
		//beep if the robot bumps an object
		unsigned char beep [1] = {60};
        unsigned char duration[1] =  {25};

    if (sensor_data->bumpLeft)
    {
        oi_setWheels(0, 0); //STOP robot as soon as bump sensor = TRUE
        move_backward(sensor_data, 3);
        oi_loadSong(1, 1, beep, duration);
        oi_play_song(1);
        return 1; //return a 1 if left bump sensor = true
    }
    if (sensor_data->bumpLeft && sensor_data->bumpRight) {
            oi_setWheels(0,0);
            move_backward(sensor_data, 3);
            oi_loadSong(1, 1, beep, duration);
            oi_play_song(1);
            return 2; //return a 2 if left AND right bump sensor = true
        }
    if (sensor_data->bumpRight)
    {
        oi_setWheels(0, 0); //STOP robot as soon as bumpRight = TRUE
        move_backward(sensor_data, 3);
        oi_loadSong(1, 1, beep, duration);
        oi_play_song(1);
        return 3; //returns a 3 if right bump sensor = true
    }
//************CLIFF SENSORS***********************************************
    if (sensor_data->cliffLeft > 2800 || sensor_data->cliffLeft < 1600)
    {
        //TODO: Implement flag?
        oi_setWheels(0, 0); //STOP, robot sensed white tape or hole
//        rotate_counterClockwise(sensor_data, 45); //Turn left 45 degrees
        move_backward(sensor_data, 5); //Move back 5 cm
//        rotate_clockwise(sensor_data, 45); //revert back to original angle

        if (sensor_data->cliffLeft > 2800){
            oi_update(sensor_data);
            return 4; //tape detected for CL
        }
        else{
            oi_update(sensor_data);
            return 5; //hole detected for CL
        }
    }


    if (sensor_data->cliffFrontLeft > 2800 || sensor_data->cliffFrontLeft < 1600)
    {
        //TODO: Implement flag?
        oi_setWheels(0, 0); //STOP, robot sensed white tape or hole
//        rotate_counterClockwise(sensor_data, 60); //Turn left 60 degrees
        move_backward(sensor_data, 5); //Move back 5 cm
//        rotate_clockwise(sensor_data, 60); //revert back to original angle

        if (sensor_data->cliffFrontLeft > 2800)
        {
            oi_update(sensor_data);
            return 6; //tape detected for CFL
        }
        else
        {
            oi_update(sensor_data);
            return 7; //hole detected for CFL
        }
    }

    if (sensor_data->cliffFrontRight > 2800 || sensor_data->cliffFrontRight < 1600)
    {
        //TODO: Implement flag?
                oi_setWheels(0, 0);
//                rotate_clockwise(sensor_data, 60);
                move_backward(sensor_data, 5);
//                rotate_counterClockwise(sensor_data, 60);

                if (sensor_data->cliffFrontRight > 2800)
                {
                    oi_update(sensor_data);
                    return 8; //tape detected for CFR
                }
                else
                {
                    oi_update(sensor_data);
                    return 9; //hole detected for CFR
                }
            }

    if (sensor_data->cliffRight > 2800 || sensor_data->cliffRight < 1600)
    {
        //TODO: Implement flag?
               oi_setWheels(0, 0);
//               rotate_clockwise(sensor_data, 45);
               move_backward(sensor_data, 5);
//               rotate_counterClockwise(sensor_data, 45);

               if (sensor_data->cliffRight > 2800){
                   oi_update(sensor_data);
                   return 10; //tape detected for CR
               }
               else{
                   oi_update(sensor_data);
                   return 11; //hole detected for CR
               }
           }

return 0;
}


