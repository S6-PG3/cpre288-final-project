/*
 * displayinfo.c
 *
 *  Created on: Dec 4, 2021
 *      Author: Ainara Machargo
 */

 #include "lcd.h"
 #include "open_interface.h"
 #include "movement.h"
 #include "Timer.h"
 #include "user_input.h"
 
/*
 * Function to display a variety of information: Battery percentage, Bumper status, Cliff signal, and distance traveled from user input on the LCD screen
 * @PARAM oi_t sensor data from open interface
 * @PARAM parameter  distance that the roomba will travel input by the user on the GUI
 */


void displayStats(oi_t *sensor_data, int parameter){

    lcd_init();
    timer_init();
    lcd_clear();
    oi_update(sensor_data);

    //keeps track of bumper status
    char bump [5];
    //keeps track of distance traveled compared to the user input on the GUI for the robot to move
    int distTravelled;


    while (1){
        lcd_clear();
        oi_update(sensor_data);

        //to display bump sensing

        //when left bumper is detected
        if (sensor_data->bumpLeft == 1 ){
            sprintf(bump, "Left");
        }

        //when right bumper is detected
        else if (sensor_data->bumpRight == 1 ){
        sprintf(bump, "Right");
        }

        //when both bumpers is detected
        else if (sensor_data->bumpLeft == 1 && sensor_data->bumpRight == 1){
        sprintf(bump, "Both");
        }

        //when no bumper is detected
        else {
        sprintf(bump, "None");
        }

/*
 // to determine distance traveled

     // when the user's input is greater than 0, this loop will execute
      while (parameter > 0){

      distTravelled = 0;

      //when there are no bumpers pressed, the distance traveled is the current distance
      if (!sensor_data->bumpRight == 1 || !sensor_data->bumpLeft == 1  || !(sensor_data->bumpLeft == 1 && sensor_data->bumpRight == 1)){
          oi_update(sensor_data);
           distTravelled += sensor_data->distance;

              //when the distance reaches the user input, the distance traveled is reset
              if (distTravelled == parameter){
                   distTravelled =0;
                  }
      }
      //if the bumpers are pressed, distance traveled is reset
      else {
           distTravelled =0;
      }

      }

*/
        /*
         * String that contains stats to be displayed on the LCD
         * Stats are the following:
         * BATTERY: Battery percentage on the roomba
         * BUMP: Bumper status determined by if statements above
         * CLIFF: Cliff Signal from the Front Left
         * DISTANCE: The distance traveled in comparison to the distance entered by user in the GUI
         */
        char stats [100];


        sprintf(stats, "Battery: %.2f%%     Bump: %s          Cliff: %d           Distance: %d", (float) (sensor_data->batteryCharge * (1.0) / sensor_data->batteryCapacity), bump, sensor_data->cliffFrontLeftSignal,distTravelled);

      //clears any previous entry and places the string that was just made with the sensor information
      lcd_clear();
      lcd_puts(stats);

      //delay so the LCD doesn't update too quickly
      timer_waitMillis(100);
    }
}
