/*
 *  movement.c
 *
 *
 *  Collection of programs used to navigate robot and peform some obsacle detection.
 *
 *  Created on: Nov 16, 2021
 *  Author: Danny Cao, James Minardi, Ainara Machargo
 */

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
        if (obstacle != 0)
        {
            //sum -= 50;
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
float move_backward(oi_t *sensor_data, int centimeters)
{
    centimeters *= 10;

    double sum = 0;
    oi_update(sensor_data);
    oi_setWheels(-100, -100); //Move backwards, "-500 is max speed"
    while (sum < (centimeters))
    {
        oi_update(sensor_data);

        sum -= sensor_data->distance;
    }

    oi_setWheels(0, 0);
    return sum / 10;

}

/*
 *  Method used to rotate robot clockwise.
 *  User gives an angle to which the robot will
 *  continue to rotate right until "sum" is greater
 *  than the given input angle
 *
 */
float rotate_clockwise(oi_t *sensor_data, int degrees)
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
float rotate_counterClockwise(oi_t *sensor_data, int degrees)
{
    double sum = 0;
    oi_setWheels(60, -60); //Rotate counter clockwise
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
void send_distanceTraveled(float distance, int obstacle)
{
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
void send_angleRotated(float angle, int obstacle)
{
    char temp[20];
    sprintf(temp, "%f,%d", angle, obstacle);
    uart_sendStr(temp);
}

/*
 *  Detects obstacles during cyBot navigation.
 *  Utilizes bump sensors and cliff sensors to detect holes,
 *  boundaries, and low-profile objects. When an obstacle is
 *  detected, the robot stops moving, plays a quick sound,
 *  and moves backwards a small ammount.
 *
 *     Method Returns:
 *          0 : nothing
 *          1 : Left & right bump sensor    -> object
 *          2 : Left bump sensor            -> object
 *          3 : Right bump sensor           -> object
 *          4 : Left cliff sensor           -> boundary
 *          5 : Front left cliff sensor     -> boundary
 *          6 : Front right cliff sensor    -> boundary
 *          7 : Right cliff sensor          -> boundary
 *          8 : Left cliff sensor           -> hole
 *          9 : Front left cliff sensor     -> hole
 *         10 : Front right cliff sensor    -> hole
 *         11 : Right cliff sensor          -> hole
 *
 *         "{Distance or Angle traveled},{obstacle val}\0"
 */
int obstacle_check(oi_t *sensor_data)
{

//**************BUMPER SENSORS*******************************************
    if (sensor_data->bumpLeft && sensor_data->bumpRight)
    {
        oi_setWheels(0, 0);
        bump_play(sensor_data);
        move_backward(sensor_data, 5);
        return 1; //return a 1 if left AND right bump sensor = true
    }
    if (sensor_data->bumpLeft)
    {
        oi_setWheels(0, 0); //STOP robot as soon as bump sensor = TRUE
        bump_play(sensor_data);
        move_backward(sensor_data, 5);
        return 2; //return a 2 if left bump sensor = true
    }
    if (sensor_data->bumpRight)
    {
        oi_setWheels(0, 0); //STOP robot as soon as bumpRight = TRUE
        bump_play(sensor_data);
        move_backward(sensor_data, 5);
        return 3; //returns a 3 if right bump sensor = true
    }

//************BOUNDARY SENSOR***********************************************
    if (sensor_data->cliffLeftSignal > 2650)
    {
        oi_setWheels(0, 0);
        boundary_play(sensor_data);
        move_backward(sensor_data, 5);
        return 4;
    }
    if (sensor_data->cliffFrontLeftSignal > 2650)
    {
        oi_setWheels(0, 0);
        boundary_play(sensor_data);
        move_backward(sensor_data, 5);
        return 5;
    }
    if (sensor_data->cliffFrontRightSignal > 2650)
    {
        oi_setWheels(0, 0);
        boundary_play(sensor_data);
        move_backward(sensor_data, 5);
        return 6;
    }
    if (sensor_data->cliffRightSignal > 2650)
    {
        oi_setWheels(0, 0);
        boundary_play(sensor_data);
        move_backward(sensor_data, 5);
        return 7;
    }

//************CLIFF BOOLEAN & SIGNAL SENSOR***********************************************
    if (sensor_data->cliffLeft || sensor_data->cliffRightSignal < 500)
    {
        oi_setWheels(0, 0);
        hole_play(sensor_data);
        move_backward(sensor_data, 5);
        return 8;
    }
    if (sensor_data->cliffFrontLeft || sensor_data->cliffRightSignal < 500)
    {
        oi_setWheels(0, 0);
        hole_play(sensor_data);
        move_backward(sensor_data, 5);
        return 9;
    }
    if (sensor_data->cliffFrontRight || sensor_data->cliffRightSignal < 500)
    {
        oi_setWheels(0, 0);
        hole_play(sensor_data);
        move_backward(sensor_data, 5);
        return 10;
    }
    if (sensor_data->cliffRight || sensor_data->cliffRightSignal < 500)
    {
        oi_setWheels(0, 0);
        hole_play(sensor_data);
        move_backward(sensor_data, 5);
        return 11;
    }

//    if (sensor_data->cliffLeft)
//    {
//        oi_setWheels(0, 0);
//        hole_play(sensor_data);
//        move_backward(sensor_data, 5);
//        return 8;
//    }
//    if (sensor_data->cliffFrontLeft)
//    {
//        oi_setWheels(0, 0);
//        hole_play(sensor_data);
//        move_backward(sensor_data, 5);
//        return 9;
//    }
//    if (sensor_data->cliffFrontRight)
//    {
//        oi_setWheels(0, 0);
//        hole_play(sensor_data);
//        move_backward(sensor_data, 5);
//        return 10;
//    }
//    if (sensor_data->cliffRight)
//    {
//        oi_setWheels(0, 0);
//        hole_play(sensor_data);
//        move_backward(sensor_data, 5);
//        return 11;
//    }

    return 0;
}
