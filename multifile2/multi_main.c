/*
 * Team 3: James Cockrell & Justin Loveless
 * Multi-File version of complete code
 *
 * File:
 * 		multi_main.h
 * Description:
 * 		Source file that contains main and init functions.
 * Date:
 * 		12/08/15
 *
 * */
#include "multi_main.h"

int main(void){
	// Call init functions.
    Board_initGeneral();
    Board_initGPIO();
    Board_initPWM();
    Board_initADC();
    Board_initUART();
    Motor_init(); //PWM seems to be causing issues

    //set direction control for motor control
    //pin 4 high -> right motor forward
    GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_DIR_MODE_OUT);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);
    //ping 7 high -> left motor forward
    GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_7, GPIO_DIR_MODE_OUT);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, GPIO_PIN_7);
    // Turn on blue LED -> indicate BIOS running
    GPIO_write(Board_LED0, Board_LED_ON);
    /* Start BIOS */
    BIOS_start();

    return (0);
}

void Board_initADC(){
	//Enable ADC0. Just replace 0 with 1 for ADC1.
	//To select both use: (SYSCTL_PERIPH_ADC0 | SYSCTL_PERIPH_ADC1)
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	//Reset to Apply values.
   SysCtlPeripheralReset(SYSCTL_PERIPH_ADC0);
	//Disable ADC0 sequencer 3 before before configuring settings.
	ADCSequenceDisable(ADC0_BASE,2);
	//Configure the ADC sequencer.
	//Use ADC0, select sample sequencer 3, trigger processor,
	//		input channel select | Enable interupts | End readings.
	ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 2, 0,
			ADC_CTL_CH0 /*AIN0: right wall*/);
	ADCSequenceStepConfigure(ADC0_BASE, 2, 1,
			ADC_CTL_CH1 /*AIN1: front wall*/ | ADC_CTL_IE | ADC_CTL_END);

	//Enable port E. Set up Port E pin 3 as ADC input (AIN0)
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE | SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3); //right Distance sensor
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2); //front Distance sensor

	//Enable ADC sequencer 2.
	ADCSequenceEnable(ADC0_BASE, 2);


}
