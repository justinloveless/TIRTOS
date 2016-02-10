/*
 * Team 3: James Cockrell & Justin Loveless
 * Multi-File version of complete code
 *
 * File:
 * 		multi_Cmd.c
 * Description:
 * 		Source file that contains all command related functions.
 * Date:
 * 		12/08/15
 *
 * */
#include "multi_Cmd.h"

char cmdTable[6][3] = {	//If index is > 2, these cmds are only valid when not driving.
		"sd\0",
		"ad\0",
		"as\0",
		"ri\0",
		"rl\0",
		"st\0"
};
char *(*fnTable[7])(void) = {
		StopDrivingFn,
		StartAcquireData,
		StopAcquireData,
		ReadIRFn,
		readLightData,
		StartDrivingFn,
		InvalidFn
};

void RxCmdFn (){
char * Frame;

	while (1){
		do {
			Frame = readFrame();
			Task_sleep(10);
		}while (Frame == NULL);
		Semaphore_pend(CmdBuffer_RWLock, BIOS_WAIT_FOREVER);
		strcpy(GlobalCmdBufferPtr, RxBufferPtr);
		Semaphore_post(NewCmdSema);
	}
}
char * getCmdFn(){
	strncpy(command, GlobalCmdBufferPtr, 2);	//get first 2 letters
	command[2] = '\0';
	return command;
}
char * StopDrivingFn() {
	Clock_stop(DriveClock);
	setSpeed (0, MOTOR_LF_EN);
	setSpeed (0, MOTOR_RF_EN);
	PWMOutputState(PWM0_BASE, MOTOR_LF_EN , false);
	PWMOutputState(PWM0_BASE, MOTOR_RF_EN , false);
	strcpy(CmdResponseBufferPtr, "Stopping\0");
	drive_state = START;
	Clock_stop(DataClock);   //stop data acquisition.
	return CmdResponseBufferPtr;
}

char * StartDrivingFn() {
	drive_state = FOLLOW1;
	setSpeed(200,MOTOR_LF_EN );
	setSpeed(200,MOTOR_RF_EN );
	PWMOutputState(PWM0_BASE, MOTOR_LF_EN , true);
	PWMOutputState(PWM0_BASE, MOTOR_RF_EN , true);
	Clock_start(DriveClock);
	strcpy(CmdResponseBufferPtr, "Starting\0");
	return CmdResponseBufferPtr;
}

char *ReadIRFn (){
	int distance;
	distance = ReadWall_IR ();
	intToStr(distance,CmdResponseBufferPtr, 4);
	return CmdResponseBufferPtr;
}

char *InvalidFn() {
	strcpy(CmdResponseBufferPtr, "Invalid cmd\0");
	return CmdResponseBufferPtr;
}
char *SetSpdFn(int spd1){ //spd1 and spd2 go from 0-99
	setSpeed (4*spd1, MOTOR_LF_EN);
	setSpeed (4*spd1, MOTOR_RF_EN);
	strcpy(CmdResponseBufferPtr, "Set Speed\0");
	return CmdResponseBufferPtr;
}
void CmdIntFn (){
	char *command;
	int i = 0;
	while(1){
		Semaphore_pend(NewCmdSema, BIOS_WAIT_FOREVER);
		*CmdResponseBufferPtr = '\0';
		noLrc = 0;
		//extract command
		command = getCmdFn();
		for ( i = 0; i < 6; i++){
			if(!strcmp(command, cmdTable[i])){	//finds match
				if (drive_state > START){	//driving
					if (i <= 2){	//valid commands
						CmdResponseBufferPtr = fnTable[i]();
						break;
					}
					else {	//invalid commands
						CmdResponseBufferPtr = fnTable[6]();
						break;
					}
				}
				else {	//not driving
					if (i > 2) {	//valid commands
						CmdResponseBufferPtr = fnTable[i]();
						break;
					}
					else {	//invalid commands
						CmdResponseBufferPtr = fnTable[6]();
						break;
					}
				}
			}
			else {
				if (i == 6)	//no commands found
					CmdResponseBufferPtr = fnTable[6]();
			}

		}
		Semaphore_pend(TX_RWLock, BIOS_WAIT_FOREVER);
		//return pointer to CmdResponseBuffer
		//copy cmdResponseBuffer to LocalResponseBuffer
		strcpy(LocalResponseBuffer, CmdResponseBuffer);
		//copy LocalResponseBufferPtr to TxBufferPtr
		strcpy(TxBufferPtr, LocalResponseBuffer);
		Semaphore_post(TxResponseSema);
		Semaphore_post(CmdBuffer_RWLock);
	}
}
void AcquireDataFn() {	//Task that relies on dataClock.
	while(1) {
		char temp[3] = "\0";
		Semaphore_pend(DataSema, BIOS_WAIT_FOREVER);
		char buffer[4] = " \0";
		//Get the latest error and trim the lower significant byte off.
		int errorval = abs(error);
		errorval /= 0xf;
		//sum the squares
		RMS += (errorval*errorval);
		RMS_cnt++;
		//Convert the value to a string.
		hexToStr(errorval,temp,2);
		strcat(buffer, temp);
		buffer[3] = '\0';
		noLrc = 1;	//notifies the writeFrame not to add the LRC.
		//Make sure the latest value wont overflow the buffer. Accounts for new line and null.
		if ((numChars + strlen(buffer) ) <= BUFFER_MAX - 3) {
			strcat(pingptr1, buffer);
			numChars+= strlen(buffer);
		}
		else {//If the value overflows push the buffer out to be written
			//		and switch to the other

			//ping pong buffer.
			numChars = 0;
			strcpy(FullBufferPtr, "\0");	//Clear the fullBuffer.
			strcat(pingptr1, "\n\0");		//Append new line.
			strcpy(FullBufferPtr, pingptr1);//Copy it to the fullBuffer.
			Semaphore_post(TxDataSema);		//Let writeFrame continue.
			//Switch buffers.
			if (pingptr1 == pingPong1)
				pingptr1 = pingPong0;
			else
				pingptr1 = pingPong1;
			//Clear buffer before adding on to it.
			strcpy(pingptr1, "\0");
			strcat(pingptr1, buffer);
			numChars+= strlen(buffer);
		}
	}
}
char* StopAcquireData() {
	char RMS_str[5];
	Clock_stop(DataClock);
	//RMS /= RMS_cnt;		//finalize RMS
	//RMS = sqrt(RMS);
	//strcpy(FullBufferPtr, "\0");	//Clear the fullBuffer.
	//strcpy(FullBufferPtr, "\nRMS = \0");
	//intToStr(RMS, RMS_str, 2);
	//strcat(FullBufferPtr, RMS_str);
	//strcat(FullBufferPtr, "\n\0");
	//Semaphore_post(TxDataSema);		//Let writeFrame continue.
	strcpy(CmdResponseBufferPtr, "StoppingAD\0");
	return CmdResponseBufferPtr;
}
char* StartAcquireData() {
	Clock_start(DataClock);
	RMS = 0;
	strcpy(CmdResponseBufferPtr, "StartingAD\0");
	return CmdResponseBufferPtr;
}

char* readLightData() {
	char buffer[7];
	if (lightPtr){
		strcpy(buffer, "dark");
	}
	else {
		strcpy(buffer, "light");
	}
	strcpy(CmdResponseBufferPtr, buffer);
	return CmdResponseBufferPtr;
}

char* shutdownMotors() {
	//Deccelerate and shutdown the motors.
	setSpeed (0, MOTOR_LF_EN);
	setSpeed (0, MOTOR_RF_EN);
	PWMOutputState(PWM0_BASE, MOTOR_LF_EN , false);
	PWMOutputState(PWM0_BASE, MOTOR_RF_EN , false);
	strcpy(CmdResponseBufferPtr, "ShutdownMotors\0");
	return CmdResponseBufferPtr;
}


