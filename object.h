/*
 * object.h
 *
 *  Created on: Oct 13, 2021
 *      Author: jminardi
 */

#ifndef OBJECT_H_
#define OBJECT_H_


typedef struct
{
    int center_angle;
    float distance;
    int radial_width;
    float linear_width;
} object;

/*
 * Copies object source to target
 */
void copy_object(object* source, object* target);



#endif /* OBJECT_H_ */
