/*
 *  adc.h
 *
 * 	ADC and IR sensor functionality. Includes initialization and reading of ADC samples
 * 	as well as conversion methods from raw IR samples to distance in centimeters
 *
 *  Created on: Oct 19, 2021
 *  Author: James Minardi
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include "lcd.h"

/*
 * Current bot being used. Updated manually in main.c
 */
extern int BOT; // main.c

/*
 * Initialize ADC using GPIO port B pin 4, ADC Module 0, and SS1
 */
void adc_init(void);

/*
 * Complete ADC conversion and assign sample to IR_raw_sample
 *
 * @return Raw sample for scan_ to convert using adc_to_cm
 */
int adc_read(void);

/*
 * Converts a raw IR sample to the distance in centimeters
 * using the specified bot's unique conversion equation.
 *
 * @param raw - Raw IR sample to be converted to distance
 * @return distance - distance in centimeters
 */
float adc_to_cm(int raw);

/*
 * Specific bot's conversion equation called by adc_to_cm
 * @param raw - raw IR sample to convert to distance
 * @return distance - distance in centimeters
 */
float adc_to_cm_1(int raw);
float adc_to_cm_2(int raw);
float adc_to_cm_3(int raw);
float adc_to_cm_4(int raw);
float adc_to_cm_5(int raw);
float adc_to_cm_6(int raw);
float adc_to_cm_7(int raw);
float adc_to_cm_8(int raw);
float adc_to_cm_9(int raw);
float adc_to_cm_10(int raw);
float adc_to_cm_11(int raw);
float adc_to_cm_12(int raw);
float adc_to_cm_13(int raw);
float adc_to_cm_14(int raw);
float adc_to_cm_15(int raw);


#endif /* ADC_H_ */
