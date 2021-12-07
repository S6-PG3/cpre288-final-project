/*
 *  scan.c
 *
 *  Created on: Nov 27, 2021
 *  Author: James Minardi
 *
 */

#include "scan.h"
#include "object.h"


/*
 * Moves servo motor to angle, and gets IR and Sonar distances for that angle
 *
 * @params
 *      angle - Direction to point the sensors to
 *      getScan - Location of a scan_t struct to store scan values
 */
void scan(scan_t *get_scan, int angle)
{
    servo_move(angle);
    int IR_raw_sample = adc_read();
    get_scan->IR_dist = adc_to_cm(IR_raw_sample);
    get_scan->sound_dist = ping_read();

}

/*
 * Method used to perform a "scan" from 0 to 180 degrees.
 * When called, the servo is set a 0 degrees and returns
 * the IR_distance and the Sound_distance. It increments
 * by one degree and collects the same information.
 * @PARAM scanner data
 */
void scan_full(scan_t *get_scan)
{
    int current_angle = 0;
    scan(get_scan, 0); //servo is set to 0 degree

    while (current_angle <= 180) //loop over program until the servo has reached 180 degrees
    {
        scan(get_scan, current_angle); //Collect IR and Sound distances at the angle servo is currently at
        scan_sendData(get_scan, current_angle); //Send this information over to the GUI to process
        current_angle++; //Increment the angle by 1 degree
    }
    scan(get_scan, 0);
}

/*
 * Method used to communicate data collected from sensors to the GUI interface
 * for user analysis. The method sends the angle and the IR distance to the GUI.
 */
void scan_sendData(scan_t *get_scan, int angle)
{
    char temp[50];
    sprintf(temp, "%d,%lf;",angle, get_scan->IR_dist);
    uart_sendStr(temp);
}


/*
 * Called when users needs to scan for objects present on the field.
 * Similar to "scan_full" the method scans from 0 to 180 degrees, but
 * collects information based on:
 *              1. Object starting angle
 *              2. Distance (based on sound distance)
 *              3. Radial width
 *              4. Linear width
 * @PARAM scanner data
 * @PARAM object information
 * @RETURN number of objects
 */
int scan_objects(scan_t *get_scan, object *object_array)
{
    int num_objects = 0; //no objects detected yet
    double previous_distance = 0.0; //previous distance starts at 0
    int current_angle = 0; //current angle should be at 0


    scan(get_scan, 0);

    while (current_angle <= 180)
    {
        scan(get_scan, current_angle);

        if ((get_scan->IR_dist) < 55) //Check to see if the IR distance is less than 55 cm
        {
            if (abs(get_scan->IR_dist - previous_distance) > 30) // Populate information on object if IR_distance - previous_distance is greater than 30 cm
            {
                if (num_objects != 0)   // and if the number of objects does not equal to 0
                {
                    object_array++;      // Shift over the array and "create" new object
                }

                //New object detected
                object_array->center_angle = current_angle;                 // Starting Angle of new object is the current angle reading
                object_array->distance = get_scan->sound_dist;  // Object Distance = Ping sensor reading
                object_array->radial_width = 0;                 // Initialize radial width to 0
                object_array->linear_width = 0;                 // Initialize radial width to 0
                num_objects++; //Increment number of objects by 1 after creating new object
            }

            else
            {
                object_array->radial_width++;       //same object has been detected: increment the radial width by 1
            }
            previous_distance = get_scan->IR_dist;  //Store the previous distance to determine new objects based on line 87
        }
        else
        {
            previous_distance = 100; // Maximized reading of IR sensor in case of too high of a reading when not detecting anything
        }
        current_angle++; // When done, increment the angle by 1 degree and repeat the steps above
    }

    // Caluclate linear width
    int i = 0;
    for (i = 0; i < num_objects; i++)
    {
        (object_array + i)->linear_width = 2 * (object_array + i)->distance
                * tan((object_array + i)->radial_width * (M_PI_2 / 180));
    }

    remove_garbage(object_array, &num_objects);
    confirm_distances(get_scan, object_array, &num_objects);
    return num_objects;
}

/*
 * Removes objects with a radial width less than 3. These values are obtained by IR sensor noise.
 * Updates object_array pointer and objects pointer with the updated objects.
 */
void remove_garbage(object *object_array, int *objects) {
    object copy[20];
    int i = 0;
    for (i = 0; i < *objects; i++)
    {
        copy_object((object_array + i), &copy[i]);
    }

    int original_index = 0;
    int copy_index = 0;
    for (copy_index = 0; copy_index < *objects; copy_index++)
    {
        if (copy[copy_index].radial_width > 3)
        {
            copy_object(&copy[copy_index], (object_array + original_index));
            original_index++;
        }
        else
        { // Radial width < 4; GARBAGE
        }
    }
    *objects = original_index;
}

void confirm_distances(scan_t *get_scan, object *object_array, int *num_objects)
{
    int current_index = 0;
    while (current_index < *num_objects)
    {
        object_array->center_angle = (object_array->center_angle
                + (object_array->radial_width / 2)); // Center of object from 0 degrees
        scan(get_scan, object_array->center_angle);
        object_array->distance = get_scan->sound_dist; // Update distance with new average

        current_index++;
        object_array++;
    }
}

/*
 * Similar function to the scan_sendData method in which
 * this method sends the collected information of the
 * objects detected on the field to the GUI.
 * @PARAM object data
 */
void scan_sendObjects(object *object_array, int num_objects){
    char temp[50];
//    sprintf(temp, "%d;", num_objects);
//    uart_sendStr(temp);


    int i;
    for(i = 0; i < num_objects; i++){
        sprintf(temp, "%f,%d,%f;", (object_array+i)->distance, (object_array+i)->center_angle, (object_array+i)->linear_width);
        uart_sendStr(temp);
    }
}


