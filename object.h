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
    int startAngle;
    float distance;
    int radialWidth;
    float linearWidth;
} object;

/*
 * Copies object source to target
 */
void objectCopy(object* source, object* target);



#endif /* OBJECT_H_ */
