/*
 * Team 3: James Cockrell & Justin Loveless
 * Multi-File version of complete code
 *
 * File:
 * 		multi_Globals.c
 * Description:
 * 		Source file that defines all global variables.
 * Date:
 * 		12/08/15
 *
 * */
#include "multi_Globals.h"

unsigned int drive_state = 0;
unsigned int prev_follow_state = 0;
unsigned int speed_r = 1;
unsigned int speed_l = 1;
unsigned int wall_det = 1;
unsigned int line1_det = 0;
unsigned int line2_det = 0;
unsigned int stop_det = 0;
unsigned int front_wall_det = 0;
int black_count = 0;
int error = 0;
char CmdResponseBuffer[] = {'\0'};
char LocalResponseBuffer[] = {'\0'};
char GlobalCmdBuffer[] = {'\0'};
char RxBuffer[] = {'\0'};
char TxBuffer[] = {'\0'};
char FullBuffer[] = {'\0'};
char ID[] = {'\0'};
char command[] = {'\0'};
char hwiBuffer[] = {'\0'};
char pingPong0[] = {'\0'};
char pingPong1[] = {'\0'};
char *pingptr0 = pingPong0;
char *pingptr1 = pingPong1;
char *LocalResponseBufferPtr = LocalResponseBuffer;
char *CmdResponseBufferPtr = CmdResponseBuffer;
char *GlobalCmdBufferPtr = GlobalCmdBuffer;
char *RxBufferPtr = RxBuffer;
char *TxBufferPtr = TxBuffer;
char *FullBufferPtr = FullBuffer;
int RxBufferPosition = 0;
unsigned int lightPtr = 0;
int32_t read = 0;
int lrc = 0;
int lrc_in = 0;
int bufferPos = 0;
int pingFlag = 0;
int wasWhite = 1;
int adState = 0;
int numChars = 0;
int noLrc = 0;
int RMS = 0;
int RMS_cnt = 0;
