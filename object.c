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
void copy_object(object* source, object* target) {

    target->center_angle = source->center_angle;
    target->distance = source->distance;
    target->radial_width = source->radial_width;
    target->linear_width = source->linear_width;

}

