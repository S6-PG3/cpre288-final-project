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
 
void displayStats(oi_t *sensor_data, int parameter){

    //oi_t *sensor_data = oi_alloc(); // Allocating space for open interface struct
    //oi_init(sensor_data); // Filling sensor data struct with initial data

    lcd_init();
    timer_init();
    lcd_clear();
    oi_update(sensor_data);

    char bump [5];
    int distTravelled;
    while (1){
        lcd_clear();
        oi_update(sensor_data);

        //to display bump sensing

        //when left bumper is detected
        if (sensor_data->bumpLeft == 1 ){
            sprintf(bump, "Left");
            //lcd_printf("left");
        }
        //when right bumper is detected
        else if (sensor_data->bumpRight == 1 ){
        sprintf(bump, "Right");
        //d_printf("right");
        }
        //when both bumpers is detected
        else if (sensor_data->bumpLeft == 1 && sensor_data->bumpRight == 1){
        sprintf(bump, "Both");

    //  lcd_printf("both");
        }
        //when no bumper is detected
        else {
        sprintf(bump, "None");
        }


//  for distance travelled
//      while (parameter > 0){
//
//       distTravelled = 0;
//
//      if (!sensor_data->bumpRight == 1 || !sensor_data->bumpLeft == 1  || !(sensor_data->bumpLeft == 1 && sensor_data->bumpRight == 1)){
//          oi_update(sensor_data);
//           distTravelled += sensor_data->distance;
//
//              if (distTravelled == parameter){
//                   distTravelled =0;
//                  }
//      }
//      else {
//           distTravelled =0;
//      }
//
//      }
        char stats [100];


        sprintf(stats, "Battery: %.2f%%     Bump: %s          Cliff: %d           Distance: %d", (float) (sensor_data->batteryCharge * (1.0) / sensor_data->batteryCapacity), bump, sensor_data->cliffFrontLeftSignal,distTravelled);


      lcd_clear();
      lcd_puts(stats);

      timer_waitMillis(100);
    }
}
