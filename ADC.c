// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0 to be triggered by
// software and trigger a conversion, wait for it to finish,
// and return the result.
// Put your name here 
// Put the date here



// comments from Hadil; please update your comments to reflect what is going on in the system. 
// I have changed the setting to PE2, channel 1 instead of PD2
#include "ADC.h"
#include "tm4c123gh6pm.h"

volatile unsigned long delay;


// This function should initialize ADC sampling at the lowest possible rate
// use a sequencer and step of your choice

void ADC_Init(void){ volatile unsigned long delay;
// using portD pin 1
	int x = 0; 
	SYSCTL_RCGC2_R |= 0x10; 
	x++; 
	GPIO_PORTE_DEN_R &= ~0x01; // disable for analog use
	GPIO_PORTE_DIR_R &= ~ 0x01; // write as 0 for input use 
	GPIO_PORTE_AFSEL_R |= 0x01; // enable for analog modulation 
	GPIO_PORTE_AMSEL_R |= 0x01; // enable for alternate functions 
	// USE ANY SEQUENCER FOR SAMPLING 
	// WE WILL USE SAMPLE SEQUENCER 0 WHICH HAS THE HIGHEST PRIOROTY
	
	SYSCTL_RCGC0_R |= 0x00010000; // ACTIVATE ADC0, USE BIT 16 ACCORDING TO MAP
	delay = 0x10; 
	delay=0x0a;
	delay =0x0f;
//	ADC1_PC_R = 0x1; 
	SYSCTL_RCGC0_R &= ~0x300; // CONFIGURE FOR 125K SAMPLES/SEC
	ADC0_SSPRI_R &= ~0x3000; // FOR PRIORITY 0, THE HIGHEST
	ADC0_ACTSS_R &= ~0x0008; 
	ADC0_EMUX_R &= ~0xF000; // SOFTWARE TRIGGER FOR EN3
	ADC0_SSMUX3_R |= 0x0003; //CLEAR SAMPLE SEQUENCER FIELD
	//ADC0_SSMUX3_R += 0;// SET CHANNEL 6-see analog inputs pin table
	ADC0_SSCTL3_R |= 0x0006; 
	//ADC0_IM_R &= ~0x0008;
	ADC0_ACTSS_R |= 0x08; 
	// NEED TO SET EN3 TO ENSURE NO UNPREDICTABLE BEHAVIOR


}

//------------ADC_In------------
// Busy-wait Analog to di5gital conversion
// Input: none
// Output: 12-bit result of ADC conversion
	
	//unsigned long ADC_In(void){
	//unsigned long Data;
	//ADC0_PSSI_R = 0x0008;
	//while ((ADC0_RIS_R&0x08) == 0){}; 
	//	Data = ADC0_SSFIFO3_R&0xFFF;
		//ADC0_ISC_R = 0x0008;
		//return Data; 
	//}

	unsigned long ADC_In(void) {
    unsigned long Data;

    ADC0_PSSI_R |= 0x08; // initiate SS3

    // wait until conversion is done:
    while ((ADC0_RIS_R & 0x08) == 0) {}
    Data = ADC0_SSFIFO3_R & 0xFFF; // mask with 12-bit binary
    ADC0_ISC_R |= 0x08;              // clear the flag to start capturing another sample
    return Data;                   // return the value of result
}


