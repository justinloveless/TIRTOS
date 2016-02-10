/*
 * Team 3: James Cockrell & Justin Loveless
 * Multi-File version of complete code
 *
 * File:
 * 		multi_Globals.h
 * Description:
 * 		Header file that declares global variables and prototypes.
 * Date:
 * 		12/08/15
 *
 * */

#ifndef MULTI_GLOBALS_H_
#define MULTI_GLOBALS_H_
#include "includes.h"

#define MAX_VAL_F   	1850		// change this value experimentally
#define MIN_VAL_R		1200		// change this value experimentally
#define MOTOR_RF_OUT 	PWM_OUT_3
#define MOTOR_LF_OUT 	PWM_OUT_0
#define MOTOR_RF_EN  	PWM_OUT_3_BIT
#define MOTOR_LF_EN  	PWM_OUT_0_BIT
#define BUFFER_MAX 		20
#define START			0
#define FOLLOW1     	1
#define FOLLOW2     	2
#define FOLLOW3     	3
#define INTERSECTION 	4
#define UTURN       	5
#define STOP        	6
#define OFF				7

extern unsigned int drive_state;
extern unsigned int prev_follow_state;
extern unsigned int speed_r;
extern unsigned int speed_l;
extern unsigned int wall_det;
extern int black_count;

extern char command[3];
extern int RxBufferPosition;
extern char FullBuffer[BUFFER_MAX];
extern char TxBuffer[BUFFER_MAX];
extern char *FullBufferPtr;
extern char *TxBufferPtr;
extern char RxBuffer[BUFFER_MAX];
extern char *RxBufferPtr;
extern char GlobalCmdBuffer[BUFFER_MAX];
extern char *GlobalCmdBufferPtr;
extern char ID[BUFFER_MAX];
extern char CmdResponseBuffer[BUFFER_MAX];
extern char *CmdResponseBufferPtr;
extern char LocalResponseBuffer[BUFFER_MAX];
extern char *LocalResponseBufferPtr;
extern unsigned int lightPtr;

extern unsigned int line1_det;
extern unsigned int line2_det;
extern unsigned int stop_det;
extern unsigned int front_wall_det;
extern unsigned int dist_r;
extern int error;
extern char pingPong0[BUFFER_MAX];
extern char pingPong1[BUFFER_MAX];
extern char *pingptr0;
extern char *pingptr1;
extern int pingFlag;

extern char cmdTable[6][3];
extern char *(*fnTable[7])(void);

extern int32_t read;
extern int lrc;
extern int lrc_in;
extern char hwiBuffer[BUFFER_MAX];
extern int bufferPos;
extern int pingFlag;
extern int wasWhite;
extern int adState;

extern int numChars;
extern int noLrc;
extern int RMS;
extern int RMS_cnt;


#endif /* MULTI_GLOBALS_H_ */
