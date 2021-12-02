/*
 * movement.c
 *
 *
 * Collection of programs used to navigate robot and peform some obsacle detection.
 *
 *  Created on: Nov 16, 2021
 *      Author: dannycao
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

/*
 *  Method used to navigate robot forward.
 *  User gives a value (in cm) to which
 *  value gets converted in mm and is stored
 *  back in "centimeters".
 *
 */
int moveForward(oi_t *sensor_data, int centimeters);

/*
 *  Method used to navigate robot backwards.
 *  User gives a value (in cm) to which
 *  value gets converted in mm and is stored
 *  back in "centimeters".
 *
 */
void moveBackward(oi_t *sensor_data, int centimeters);

/*
 *  Method used to rotate robot clockwise.
 *  User gives an angle to which the robot will
 *  continue to rotate right until "sum" is greater
 *  than the given input angle
 *
 */
void turnRight(oi_t *sensor_data, int degrees);

/*
 *  Method used to rotate robot counterclockwise.
 *  User gives an angle to which the robot will
 *  continue to rotate left until "sum" is greater
 *  than the given input angle
 *
 */
void turnLeft(oi_t *sensor_data, int degrees);

/*
 *  A large method to detect obstacles during movement of the robot.
 *  Utilizes bump sensors and cliff sensors to detect obstacles such as holes,
 *  boundary lines, and small objects undetectable from IR and PING sensors. Method
 *  also includes manuevering program when sensors have been triggered to assist user
 *  with navigation.
 *
 *     Method Returns:
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
 */
int obstacleCheck(oi_t *sensor_data);


#endif /* MOVEMENT_H_ */
