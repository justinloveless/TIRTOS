/*
 * Team 3: James Cockrell & Justin Loveless
 * Multi-File version of complete code
 *
 * File:
 * 		multi_Motors.h
 * Description:
 * 		Header file that contains all motor related function prototypes.
 * Date:
 * 		12/08/15
 *
 * */

#ifndef MULTI_MOTORS_H_
#define MULTI_MOTORS_H_
#include "includes.h"

void Motor_init();
void motor_RightTurn();
void motor_Uturn();
void motor_setReverse();
void motor_setForward();
void adjustMotors(int PID_level);



#endif /* MULTI_MOTORS_H_ */
