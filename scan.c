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
}

/*
 * Method used to communicate data colleted from sensors to the GUI interface
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
 *              3. Radial widith
 *              4. Linear width
 */
void scan_objects(scan_t *get_scan, object *objectArray)
{
    int numObjects = 0; //no objects detected yet
    double previous_distance = 0.0; //previous distance starts at 0
    int current_angle = 0; //current angle should be at 0


    scan(get_scan, 0);

    while (current_angle <= 180)
    {
        scan(get_scan, current_angle);

        float IR_distance = get_scan->IR_dist; //Variable for get_scan->IR_dist to reduce program line length

        if ((IR_distance) < 55) //Check to see if the IR distance is less than 55 cm
        {
            if (abs(IR_distance - previous_distance) > 30) //Popuate information on object if IR_distance - previous_distance is greater than 30 cm
            {
                if (numObjects != 0) //and if the number of objects does not equal to 0
                {
                    objectArray++; //Shift over the array and "create" new object
                }

                //New object detected
                objectArray->startAngle = current_angle;                //Starting Angle of new object is the current angle reading
                objectArray->distance = get_scan->sound_dist;           //Object Distance = Ping sensor reading
                objectArray->radialWidth = 0;                           //Radial width of new object is currently 0
                objectArray->linearWidth = 0;                           //Linear width of new object is currently 0
                numObjects++;                                           //Increment number of objects by 1 after creating new object
            }

            else
            {
                objectArray->radialWidth++; //same object has been detected: increment the radial width by 1
            }
            previous_distance = IR_distance; //Store the previous distance to determine new objects based on line 87
        }

        else
        {
            previous_distance = 200; //Maximized reading of IR sensor in case of too high of a reading when not detecting anything
        }
        current_angle++; //When done, increment the angle by 1 degree and repeat the steps above
    }


}


/*
 * Similar function to the scan_sendData method in which
 * this method sends the collected information of the
 * objects detected on the field to the GUI.
 */
void scan_sendObjects(object *objectArray){
    char temp[50];

    int i;
    for(i = 0; i < sizeof(objectArray) / sizeof(*objectArray); i++){
        sprintf(temp, "%f,%d,%f;", objectArray[i].distance, objectArray[i].startAngle, objectArray[i].linearWidth); //TODO: Do we need linear width?
        uart_sendStr(temp);
    }
}


