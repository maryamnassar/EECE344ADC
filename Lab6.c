// Lab6.c
// this lab creates a position measurment software that outputs the result on LCD
// The software samples analog input signal and use calibration to output the position
// Runs on LM4F120 or TM4C123
// Use the SysTick timer to request interrupts at a particular period.
// Maryam Nassar 
// 2 May, 2020

// 

#include "Nokia5110.h"
#include "PLL.h"
#include "ADC.h"
#include "tm4c123gh6pm.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode
void SystickInit (void); 

// declaire any global variable here
unsigned long Data;
unsigned long Distance; 
unsigned long Convert(); 
unsigned long ADCMail; 
unsigned long ADCStatus; 
unsigned long sample; 
// main1 is used to test and debug the ADC 
// At any point one function only should be name main
// rename the main function you intend to use
// single step this program and look at Data output in the simulator

int main (void){ 

	PLL_Init(); 		// adjust PLL.c to run the system at a frequency of your choice (50MHz-80MHz)	
	ADC_Init();
	EnableInterrupts(); 
	SystickInit();
	Nokia5110_Init(); 


	while(1){
	
		 if(ADCStatus == 1) { // if the status flag is  set
				Distance = Convert(ADCMail); //convert the value sampled and stored in adc mail 
				Nokia5110_Clear();
			 Nokia5110_OutString("Dist:"); 
				Nokia5110_OutUDec(Distance/1000);
				Nokia5110_OutChar('.');
				Nokia5110_OutUDec(Distance%1000); 
				Nokia5110_OutString("cm"); 
				ADCStatus =0; 
			 
							}	
		
				}

	}	


int main1 (void){ 
  PLL_Init();        
  ADC_Init();     
  while(1){  
  Data= Convert(ADC_In());
  }
}   
unsigned long Convert(unsigned long DigitalInput){
	
	Distance = (((DigitalInput*125) + 86 + 128 )/256); 
	
	//graph eqn: y = .4484x + .3377 

	// Precision is 2^-8
	// 128 is 256/2 --- this is to account for issue of overflow
	// 256 --- max decimal value with 8 bits 
  return Distance; 
}


void SystickInit() {
	
	NVIC_ST_CTRL_R = 0; // disable for initial setup 
	NVIC_ST_RELOAD_R = 21000000; // need to calculate ACTUAL reload value
	NVIC_ST_CURRENT_R = 0;// any write to 0 clears it 
	NVIC_PRI0_R = (NVIC_PRI3_R & 0x0FFFFFF) | 0x20000000; 
	NVIC_ST_CTRL_R = 0x07; // enable once again 

}

// ADCMail
// ADCStatus 
void SysTick_Handler(void) {
	
	GPIO_PORTE_DATA_R ^=0x02;//toggle bit
	ADCMail = ADC_In(); // save our sample data into adc mail
	Distance = Convert(ADCMail); // convert our sampled adc mail data using distance function  
	ADCStatus = 1; //set flag 
	GPIO_PORTE_DATA_R ^= 0x02;
	
	
	return; 

}





 
