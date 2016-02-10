/*
 * Team 3: James Cockrell & Justin Loveless
 * Multi-File version of complete code
 *
 * File:
 * 		multi_Drive.h
 * Description:
 * 		Header file that declares all Drive related function prototypes.
 * 		This includes light reading	and data acquisition related function in some cases.
 * Date:
 * 		12/08/15
 *
 * */

#ifndef MULTI_DRIVE_H_
#define MULTI_DRIVE_H_
#include "includes.h"


void ReadLightW(Semaphore_Handle Sema, unsigned int *results);
void DataClockFn();
void lightSetInputFn();
void lightReadFn();
void ReadLightFn ();
unsigned int ReadWall_IR ();
void setSpeed (unsigned int final_speed, uint32_t motor);
void pid_reset();
unsigned int ReadFront_IR ();
void PID_Algorithm();


#endif /* MULTI_DRIVE_H_ */
