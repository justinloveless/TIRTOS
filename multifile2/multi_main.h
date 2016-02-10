/*
 * Team 3: James Cockrell & Justin Loveless
 * Multi-File version of complete code
 *
 * File:
 * 		multi_main.h
 * Description:
 * 		Header file that contains all critical libraries.
 * Date:
 * 		12/08/15
 *
 * */

#ifndef MULTI_MAIN_H_
#define MULTI_MAIN_H_

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>


/* BIOS Header files */
#include <ti/sysbios/BIOS.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PWM.h>

/* Example/Board Header files */
#include "Board.h"
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include <driverlib/uart.h>
#include "driverlib/pin_map.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/adc.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Swi.h>

#include <math.h>

#include "includes.h"

void Motor_init();
void Board_initADC();

#endif /* MULTI_MAIN_H_ */
