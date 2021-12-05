/*
 * object.c
 *
 *  Created on: Oct 12, 2021
 *      Author: jminardi
 */


#include "object.h"

/*
 * Copies object source to target
 */
void objectCopy(object* source, object* target) {

    target->startAngle = source->startAngle;
    target->distance = source->distance;
    target->radialWidth = source->radialWidth;
    target->linearWidth = source->linearWidth;

}

