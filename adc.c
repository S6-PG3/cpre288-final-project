/*
 *  adc.h
 *
 * 	ADC and IR sensor functionality. Includes initialization and reading of ADC samples
 * 	as well as conversion methods from raw IR samples to distance in centimeters
 *
 *  Created on: Oct 19, 2021
 *  Author: James Minardi
 */

#include "adc.h"

/*
 * Initialize ADC using GPIO port B pin 4, ADC Module 0, and SS1
 */
void adc_init(void)
{

    // GPIO Initialization
    SYSCTL_RCGCGPIO_R |= 0b000010; 			// Enable GPIO port B
    SYSCTL_RCGCADC_R |= 0x1; 				// Enable ADC module 0

    GPIO_PORTB_AFSEL_R |= 0b00010000; 		// Set PB4 to alternate function
    GPIO_PORTB_DEN_R &= 0b11101111; 		// disable PB4 digital function
    GPIO_PORTB_AMSEL_R |= 0b00010000; 		// Enables PB4 for analog function
    GPIO_PORTB_DIR_R &= 0b11101111; 		// Set PB4 to input

    // Initialize ADC Module 0 - ADC0
    GPIO_PORTB_ADCCTL_R = 0x00; 			// Default

    // Initialize ADC Registers
    ADC0_ACTSS_R &= 0b1101; 				// Disable SS1 to stop sampling
    ADC0_EMUX_R = 0x0; 						// Default
    ADC0_SSMUX1_R |= 0x000A; 				// First sample AIN10
    ADC0_SSCTL1_R |= 0x00006; 				// Only need to sample once
    ADC0_SAC_R = 0b101;						// 16x sample averaging
    ADC0_ACTSS_R |= 0b0010; 			    // enable SS1, disables SS0, SS2, SS3

} // END adc_init

/*
 * Complete ADC conversion and assign sample to IR_raw_sample
 *
 * @return Raw sample for scan_ to convert using adc_to_cm
 */
int adc_read(void)
{

    ADC0_PSSI_R |= 0b0010; 					// Enable SS1 sampling

    while ((ADC0_RIS_R & 0b0010) == 0)      // Poll wait for ADC conversion to be done
    {
    }

    int IR_raw_sample = ADC0_SSFIFO1_R;         // Read sample from AIN10 and set to IR_raw_sample
    ADC0_ISC_R |= 0b0010; 	                // Clear the interrupt in RIS

    return IR_raw_sample;

} // END adc_read

/*
 * Converts a raw IR sample to the distance in centimeters
 * using the specified bot's unique conversion equation.
 *
 * @param raw - Raw IR sample to be converted to distance
 * @return distance - distance in centimeters
 */
float adc_to_cm(int raw)
{
    float distance;
    switch (BOT)
    {
    case 1:
        distance = adc_to_cm_1(raw);
        break;
    case 2:
        distance = adc_to_cm_2(raw);
        break;
    case 3:
        distance = adc_to_cm_3(raw);
        break;
    case 4:
        distance = adc_to_cm_4(raw);
        break;
    case 5:
        distance = adc_to_cm_5(raw);
        break;
    case 6:
        distance = adc_to_cm_6(raw);
        break;
    case 7:
        distance = adc_to_cm_7(raw);
        break;
    case 8:
        distance = adc_to_cm_8(raw);
        break;
    case 9:
        distance = adc_to_cm_9(raw);
        break;
    case 10:
        distance = adc_to_cm_10(raw);
        break;
    case 11:
        distance = adc_to_cm_11(raw);
        break;
    case 12:
        distance = adc_to_cm_12(raw);
        break;
    case 13:
        distance = adc_to_cm_13(raw);
        break;
    case 14:
        distance = adc_to_cm_14(raw);
        break;
    case 15:
        distance = adc_to_cm_15(raw);
        break;
    default:
        distance = -1;
        break;

    }
    return distance;
} // END adc_to_cm


/*
 * Specific bot's conversion equation called by adc_to_cm.
 * Distance is capped at 100cm
 * @param raw - raw IR sample to convert to distance
 * @return distance - distance in centimeters
 */
float adc_to_cm_1(int raw)
{
    return -1;
}
float adc_to_cm_2(int raw)
{
    return -1;
}
float adc_to_cm_3(int raw)
{
    return -1;
}
float adc_to_cm_4(int raw)
{
    return (12993.9 / (raw - 579.346)); // NOT TESTED
}
float adc_to_cm_5(int raw)
{
    return -1;
}
float adc_to_cm_6(int raw)
{
    if (raw < 438)
    {
        return 100.0;
    }
    else
    {
        return ((31103.3 / (raw - 115.697)) - 2.09641);
    }
} // END adc_to_cm_6

float adc_to_cm_7(int raw)
{
    return -1;
}
float adc_to_cm_8(int raw)
{
    if (raw < 430)
    {
        return 100.0;
    }
    else
    {
        return ((32751.8 / (raw - 111.277)) - 2.16436);
    }
}
float adc_to_cm_9(int raw)
{
    return -1;
}
float adc_to_cm_10(int raw)
{
    return -1;
}
float adc_to_cm_11(int raw)
{
    return ((21431.4 / (raw + 110.399)) - 2.48678); // NOT TESTED
}
float adc_to_cm_12(int raw)
{
    return -1;
}

float adc_to_cm_13(int raw)
{
    if (raw < 420)
    {
        return 100.0;
    }
    else
    {
        return ((31103.3 / (raw - 115.697)) - 2.09641);
    }
} // END adc_to_cm_13

float adc_to_cm_14(int raw)
{
    return -1;
}
float adc_to_cm_15(int raw)
{
    return -1;
}
