/*
 * Team 3: James Cockrell & Justin Loveless
 * Multi-File version of complete code
 *
 * File:
 * 		multi_Comm.h
 * Description:
 * 		Header file that declares all communication related function prototypes.
 * Date:
 * 		12/08/15
 *
 * */

#ifndef MULTI_COMM_H_
#define MULTI_COMM_H_
#include "includes.h"
void hwi_readChar_ISR ();
void swi_procChar_ISR ();
char *readFrame();
void TxDataFn();
int generateLRC(char *message);
void writeFrame (char *ID, char* data);
void TxResponseFn ();
int hexToInt(char hex);
void reverse(char *str, int len);
int intToStr(int x, char str[], int d);
char* hexToStr(int x, char str[], int d);




#endif /* MULTI_COMM_H_ */
