/*
 * Team 3: James Cockrell & Justin Loveless
 * Multi-File version of complete code
 *
 * File:
 * 		multi_Comm.c
 * Description:
 * 		Source file that contains all communication related functions.
 * Date:
 * 		12/08/15
 *
 * */
#include "multi_Comm.h"



void hwi_readChar_ISR (){//this hwi is triggered when data is received from bluetooth
	//store received data
	hwiBuffer[bufferPos++] = /*(int32_t)*/(char )UARTCharGet(UART1_BASE);
	//allow swi to run
	Swi_post(swi_procChar);

}

void swi_procChar_ISR (){
	int bufferIndex = 0;
	while (RxBufferPosition < bufferPos){
		read = (char)hwiBuffer[bufferIndex++];
		if (read == 0x0a || read == '\n') {	//line feed or carriage return/line feed combo.
			//end of message
			lrc_in = 0;
			//The LRC already got added in as it was recieved, so subtract it out.
			lrc -= RxBuffer[RxBufferPosition-1];
			lrc -= RxBuffer[RxBufferPosition-2];
			lrc -= RxBuffer[RxBufferPosition-3];
			//Grab the LRC that was sent, convert from hexascii to int,
			//and multiply 16 for high byte.
			lrc_in += hexToInt(RxBuffer[RxBufferPosition-3]);
			lrc_in *= 16;
			lrc_in += hexToInt(RxBuffer[RxBufferPosition-2]);
			RxBuffer[RxBufferPosition++] = read;			//add in the final linefeed.
			RxBuffer[RxBufferPosition] = '\0';				//null terminate
			RxBufferPosition = 0;
			bufferPos = 0;
			lrc *= -1;
			lrc = (char)(lrc);		//Trim LRC to one byte.

			//finished, disable interrupts, post UARTDoneSema.
			Hwi_disableInterrupt(22);
			Hwi_disable();
			Semaphore_post(UARTDoneSema);

			return;
		} else if (read == 0x3a) {		//start ':'
			//reset the buffer.
			RxBufferPosition = 0;
		} else {
			RxBuffer[RxBufferPosition++] = read; //Nothing special, add it in.
			lrc += read;	//keep computing the LRC as values come in.
		}

	}
}

char *readFrame(){
	read = 0;
	lrc = 0;
	lrc_in = 0;
	//enable UART interrupts
	Hwi_enable();
	Hwi_enableInterrupt(22);	//enable interrupts for UART1
	Semaphore_pend(UARTDoneSema, BIOS_WAIT_FOREVER);
	if (lrc_in == 0 || (lrc - lrc_in) == 0){ // frame correct
		return RxBufferPtr;
	}
	else if (lrc - lrc_in != 0){	//frame incorrect

		return "\0";
	}
	return NULL;
}

void TxDataFn(){
	while(1){
		//wait for data and access.
		Semaphore_pend(TxDataSema, BIOS_WAIT_FOREVER);
		Semaphore_pend(TX_RWLock, BIOS_WAIT_FOREVER);
		//Copy the fullbuffer to transmit buffer.
		strcpy(TxBufferPtr, FullBufferPtr);
		//TXBuffer is ready to be sent.
		Semaphore_post(TxResponseSema);
	}
}

int generateLRC(char *message) {		//Takes in a nul terminated string.
	int lrc = 0;
	//Go through the response message and calculate the LRC.
	while (*message != '\0') {
		lrc += *message;
		message++;
	}
	//Take two's compliment and truncate to lowest significant byte.
	lrc *= -1;
	lrc = (char)(lrc);
	return lrc;
}

void writeFrame (char *ID, char* data){
	char str[BUFFER_MAX*2], *strPtr;
	char final[BUFFER_MAX*3];
	*str = '\0';
	strPtr = final;
	if (noLrc) {	//Check if we are sending data.
		final[0] = '\0';
	} else {	//If command response use the :
		final[0] = ':';
		final[1] = '\0';
	}
	char lrc[5];
	//Append the command id then the data.
	strcat(str, ID);
	strcat(str, data);
	//Compute the LRC.
	int lrcval = generateLRC(str);
	hexToStr(lrcval, lrc, 2);
	//sprintf(lrc, "%02x", lrcval);
	lrc[2] = 0x0d;	//CR
	lrc[3] = 0x0a;	//LF
	lrc[4] = '\0';
	if (!noLrc) {	//If command response use the LRC.
		strcat(str, lrc);
	}
	else {
		noLrc = 0;
	}
	//Put the result into the final string.
	strcat(final, str);

	//Send it out.
	while(*strPtr != '\0'){
		UARTCharPutNonBlocking(UART1_BASE, *strPtr);
		strPtr++;
	}
}

void TxResponseFn (){
	while(1) {
		Semaphore_pend(TxResponseSema, BIOS_WAIT_FOREVER);
		//Once the response is ready, send it out.
		writeFrame(ID, TxBufferPtr);
		Semaphore_post(TX_RWLock);
	}
}

int hexToInt(char hex) {
	//Convert hexascii char to int.
	int val = -1;
	if (hex >= '0' && hex <= '9')
		val = hex - '0';
	else if (hex >= 'a' && hex <= 'f')
		val = hex - 'a' + 10;
	else if (hex >= 'A' && hex <= 'F')
		val = hex - 'A' + 10;
	return val;
}

// reverses a string 'str' of length 'len'. used for the int to str.
void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
    reverse(str, i);
    str[i] = '\0';
    return i;
}
char* hexToStr(int x, char str[], int d) {
	//Convert an integer to a hex ascii string.
	int i = 0;
	int temp;
	while (x && (i < d))
	{
		temp = (x%16);
		if (temp < 10)
			str[i++] = temp+'0';
		else
			str[i++] = temp+'A'-10;
		x = x/16;
	}
	// If number of digits required is more, then
	// add 0s at the beginning
	while (i < d)
		str[i++] = '0';
	reverse(str, i);
	str[i] = '\0';
	return str;
}

