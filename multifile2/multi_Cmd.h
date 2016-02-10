/*
 * Team 3: James Cockrell & Justin Loveless
 * Multi-File version of complete code
 *
 * File:
 * 		multi_Cmd.h
 * Description:
 * 		Header file that declares all command related function prototypes.
 * Date:
 * 		12/08/15
 *
 * */

#ifndef MULTI_CMD_H_
#define MULTI_CMD_H_
#include "includes.h"

void RxCmdFn ();
char * getCmdFn();
char * StopDrivingFn();
char * StartDrivingFn();
char *ReadIRFn ();
char *InvalidFn();
char *SetSpdFn(int spd1);
void CmdIntFn ();
char* StopAcquireData();
char* StartAcquireData();
char* readLightData();
void AcquireDataFn();
char* shutdownMotors();


#endif /* MULTI_CMD_H_ */
