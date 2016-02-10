/*
 * Team 3: James Cockrell & Justin Loveless
 * Multi-File version of complete code
 *
 * File:
 * 		multi_Motors.c
 * Description:
 * 		Source file that contains all motor related functions.
 * Date:
 * 		12/08/15
 *
 * */
#include "multi_Motors.h"

void Motor_init(){
	//For forward.
	//Configure the motors.
	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	//set period of PWMs to 400
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 400);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, 400);
	//set PWM width to smallest possible value w/o turning them off
	PWMPulseWidthSet(PWM0_BASE, MOTOR_LF_OUT, 001);
	PWMPulseWidthSet(PWM0_BASE, MOTOR_RF_OUT, 001);
	//enable generators 0 and 1
	PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	PWMGenEnable(PWM0_BASE, PWM_GEN_1);
	//Leave motors off until the robot is ready to start.
	PWMOutputState(PWM0_BASE, MOTOR_LF_EN , false);
	PWMOutputState(PWM0_BASE, MOTOR_RF_EN , false);
	//initialize drive variables
	prev_follow_state = 0;
	drive_state = 0;
	speed_r = 1;
	speed_l = 1;
	wall_det = 1;
}

void adjustMotors(int PID_level) {
	//Used for small incremental changes to motors from pid.
	unsigned int target_power = 250;
	unsigned int speed_l = target_power + PID_level;
	unsigned int speed_r = target_power - PID_level;
	//update PWM width
	PWMPulseWidthSet(PWM0_BASE, MOTOR_LF_OUT, speed_l);
	PWMPulseWidthSet(PWM0_BASE, MOTOR_RF_OUT, speed_r);
}

void motor_setForward() {
	//turn off right motor
	PWMOutputState(PWM0_BASE, MOTOR_RF_OUT , false);
	//set pin 4 high -> forward motor control
	GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_DIR_MODE_OUT);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);
	//turn back on right motor
	PWMOutputState(PWM0_BASE, MOTOR_RF_OUT , true);
}

void motor_setReverse() {
	//turn off right motor
	PWMOutputState(PWM0_BASE, MOTOR_RF_OUT , false);
	//set pin 4 low -> reverse motor control
	GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_DIR_MODE_OUT);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, !GPIO_PIN_4);
	//turn back on right motor
	PWMOutputState(PWM0_BASE, MOTOR_RF_OUT , true);
}

void motor_Uturn() {
	//slow to stop
	setSpeed(0, MOTOR_LF_EN);
	setSpeed(0, MOTOR_RF_EN);
	//Right motor in reverse.
	motor_setReverse();
	unsigned int dist;
	//turn on purple LED -> indicate first part of uturn
	GPIO_write(Board_LED0, Board_LED_ON);
	GPIO_write(Board_LED1, Board_LED_OFF);
	GPIO_write(Board_LED2, Board_LED_ON);
	//accelerate to 50% speed, turning in place
	setSpeed(200, MOTOR_LF_OUT);
	setSpeed(200, MOTOR_RF_OUT);
	//continue turning for length of timer (in config file)
	Timer_start(Uturn_timer);
	Semaphore_pend(Uturn_Sema , BIOS_WAIT_FOREVER);
	//read front distance sensor
	dist = ReadFront_IR();
	//uturn until the front is almost all the way around.
	//decelerate to 45% speed to complete uturn
	setSpeed(180, MOTOR_LF_OUT);
	setSpeed(180, MOTOR_RF_OUT);
	//continue turning until front sensor sees no walls
	while (dist > 1000){
		dist = ReadFront_IR();
	}
	//turn off LEDs for debugging
	GPIO_write(Board_LED0, Board_LED_OFF);
	GPIO_write(Board_LED2, Board_LED_OFF);
	GPIO_write(Board_LED1, Board_LED_OFF);
	//get right wall distance
	dist = ReadWall_IR();
	//turn on red LED -> indicate second part of uturn
	GPIO_write(Board_LED2, Board_LED_ON);
	//Finish out the Uturn until the right wall is near the nominal amount for Pid.
	while (dist < (2000)) {
		dist = ReadWall_IR();
	}

	//Set motor back to forward.
	motor_setForward();
	wall_det = 1;
	//return to previos state
	drive_state = prev_follow_state;
	//turn on green LED -> indicate completed uturn
    GPIO_write(Board_LED0, Board_LED_OFF);
    GPIO_write(Board_LED2, Board_LED_OFF);
    GPIO_write(Board_LED1, Board_LED_ON);

    return;
}

void motor_RightTurn() {
	unsigned int dist = ReadWall_IR();
	//turn on LED -> indicate right turn
	GPIO_write(Board_LED0, Board_LED_ON);
	GPIO_write(Board_LED2, Board_LED_OFF);
	GPIO_write(Board_LED1, Board_LED_OFF);
	//set small turning radius
	setSpeed(210, MOTOR_LF_OUT);
	setSpeed(170, MOTOR_RF_OUT);
	//continue turning for length of timer (in config file)
	Timer_start(Rturn_timer);
	Semaphore_pend(Rturn_Sema , BIOS_WAIT_FOREVER);
	//set sharp turning radius
	setSpeed(300, MOTOR_LF_OUT);
	setSpeed(100, MOTOR_RF_OUT);
	//Turn until right is detected again.
	while (dist < 2500 ) {
		dist = ReadWall_IR();
	}
	//return to previous state
	drive_state = prev_follow_state;
	//reinitialize PID algorithm
	pid_reset();
	wall_det = 1;

	//turn off LEDs -> indicate complete right turn
	GPIO_write(Board_LED0, Board_LED_OFF);
	GPIO_write(Board_LED2, Board_LED_OFF);
	GPIO_write(Board_LED1, Board_LED_OFF);
}


void Rturn_timerFn () {
	Semaphore_post(Rturn_Sema);
}

void Uturn_timerFn(){
	Semaphore_post(Uturn_Sema);
}


