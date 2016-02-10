/*
 * Team 3: James Cockrell & Justin Loveless
 * Multi-File version of complete code
 *
 * File:
 * 		multi_Drive.c
 * Description:
 * 		Source file that contains all Drive related functions. This includes light reading
 * 		and data acquisition related function in some cases.
 * Date:
 * 		12/08/15
 *
 * */
#include "multi_Drive.h"

//PID algorithm formula constants.

//Proportional constant. (Target Power) / (aprx. max error)
const float Kp = 100.0/1500.0;
//Integral Constant.
const float Ki = 0.007;
//Derivative constant. Experimentally determined
const float Kd = .08;
//Reference distance. Experimentally determined
const int desired_distance = 2200;
//Prevent integral wind up.
const int i_max = 15;
const int i_min = -15;

//PID algorithm variables.

//Current distance from wall. Range aprx: 1500 - 3700. 200/1100
int current_distance = 0;
//previous distance from wall.
int old_distance = 0;
//Error from wall. Range aprx: 0 - 1500.
int old_error = 0;

int i_partial = 0;
float p_calc = 0;
float i_calc = 0;
float d_calc = 0;
float d_old = 0;
float i_sum = 0;
int pid_old = 0;
int pid = 0;
int error_hist [4] = {0,0,0,0};
int hist_index = 0;


void DriveClockFn (){
	Semaphore_post(DriveSema);
}


void setSpeed (unsigned int final_speed, uint32_t motor){
	//Takes in the speed to set and the motor to apply it too.
	unsigned int speed = PWMPulseWidthGet(PWM0_BASE, motor);
	//Find out if speed is less than or greater than the new speed
	//		and accelerate/decelerate to it.
	while (speed > final_speed){
		if (speed > final_speed){
			speed -= 5;
		}
		else { speed = final_speed;}
		PWMPulseWidthSet(PWM0_BASE, motor, speed);
	}
	while (speed < final_speed ){
		if (speed < final_speed){
			speed += 5;
		}
		else { speed = final_speed;}
		PWMPulseWidthSet(PWM0_BASE, motor, speed);
	}
}



unsigned int ReadWall_IR () {
	uint32_t resultRight, resultFront;
	int i;
	ADCProcessorTrigger(ADC0_BASE, 2);
	//Wait for ADC to finish converting.
	for ( i = 10000; i>0; i--);	//delay a bit
	ADCSequenceDataGet(ADC0_BASE, 2, &resultRight);
	ADCSequenceDataGet(ADC0_BASE, 2, &resultFront);
	//Check if the front wall is close.
	if (resultFront > MAX_VAL_F){
		front_wall_det = 1;
	}
	else {
		front_wall_det = 0;
	}
	//Check if the right wall is close.
	if (resultRight < MIN_VAL_R){
		wall_det = 0;
	}
	else {
		wall_det = 1;
	}

	return resultRight;
}

unsigned int ReadFront_IR () {
	uint32_t resultRight, resultFront;
	int i;
	ADCProcessorTrigger(ADC0_BASE, 2);
	//Wait for ADC to finish converting.
	for ( i = 10000; i>0; i--);	//delay a bit
	ADCSequenceDataGet(ADC0_BASE, 2, &resultRight);
	ADCSequenceDataGet(ADC0_BASE, 2, &resultFront);
	//Check if the front wall is close.
	if (resultFront > MAX_VAL_F){
		front_wall_det = 1;
	}
	else {
		front_wall_det = 0;
	}
	//Check if the right wall is close.
	if (resultRight < MIN_VAL_R){
		wall_det = 0;
	}
	else {
		wall_det = 1;
	}

	return resultFront;
}




void PID_Algorithm() {
	//Get the current Error.
	current_distance = ReadWall_IR();
	error = desired_distance - current_distance;
	error_hist[hist_index] = error;
	//Calculating the P term.
	p_calc = Kp * error;
	//Calculating the I term.
	i_partial = (.5 * i_partial) + error;
	if (i_partial > i_max)
		i_partial = i_max;
	else if (i_partial < i_min)
		i_partial = i_min;
	i_calc += Ki * i_partial;
	//Calculating the D term.
	d_calc = Kd * ((error_hist[(4+hist_index)%4]
					+ 3*error_hist[(3+hist_index)%4]
					- 3*error_hist[(2+hist_index)%4]
					- error_hist[(1 + hist_index)%4])/6);

	pid = (int) (p_calc  + i_calc + d_calc);

	if (pid > 125)
		pid = 125;
	else if (pid < -125)
		pid = -125;

	hist_index = (hist_index++)%4;
	adjustMotors(pid);	//TargetPower = 200 (50%).

}

void pid_reset() {
	//Return all the values to default.
	old_error = 0;		//Error from wall. Range aprx: 0 - 1500.
	i_partial = 0;
	p_calc = 0;
	i_calc = 0;
	d_calc = 0;
	d_old = 0;
	i_sum = 0;
	pid_old = 0;
	error_hist[0] = 0;
	error_hist[1] = 0;
	error_hist[2] = 0;
	error_hist[3] = 0;
}

void DriveFn (){
	static int lightFlag = 0;
	while(1){
		Semaphore_pend(DriveSema, BIOS_WAIT_FOREVER);
		switch (drive_state) {
			case START:
				//enable motors forward.
				PWMOutputState(PWM0_BASE, MOTOR_LF_EN , true);
				PWMOutputState(PWM0_BASE, MOTOR_RF_EN , true);
				//accelerate to speed
				setSpeed (200, MOTOR_RF_EN);
				setSpeed (200, MOTOR_LF_EN);
				drive_state = FOLLOW1;
				GPIO_write(Board_LED0, Board_LED_ON);
				break;
				//Follow1,2 and 3 grouped together since they do the same thing.
				//They do progress depending on the lines the robot has passed, but that
				//is handled in another area.
			case FOLLOW1:
			case FOLLOW2:
			case FOLLOW3:
				prev_follow_state = drive_state;
				if (lightFlag == 0)
					//acquire data
					lightFlag = !lightFlag;

				if (!wall_det) {
					drive_state = INTERSECTION;
				}
				else if (front_wall_det == 1){
					drive_state = UTURN;		//stop if front wall detected
				}
				//read right wall distance
				PID_Algorithm();

				break;

			case INTERSECTION:
				if (Clock_isActive(DataClock) ){	//if collecting data
					Clock_stop(DataClock);	//don't collect data in intersection
					motor_RightTurn();
					Clock_start(DataClock);
				}
				else	//if not collecting data
					motor_RightTurn();	//move along

				break;
			case UTURN:
				if (Clock_isActive(DataClock) ){	//if collecting data
					Clock_stop(DataClock);	//don't collect data in uturn
					motor_Uturn();
					Clock_start(DataClock);
				}
				else	//if not collecting data
					motor_Uturn();	//move along

				break;
			case STOP:
				//decelerate to a stop
				setSpeed (0, MOTOR_LF_EN);
				setSpeed (0, MOTOR_RF_EN);
				StopDrivingFn();
				drive_state = OFF;
				break;
			case OFF:
				//disable motors
				PWMOutputState(PWM0_BASE, MOTOR_LF_EN , false);
				PWMOutputState(PWM0_BASE, MOTOR_RF_EN , false);
				break;
		}
	}
}




//light function///////////////////////////////////////////////////////////////


void ReadLightW(Semaphore_Handle Sema, unsigned int *results){
		char RMS_str[5];
		char RMS_cnt_str[5];
		//Set pins as output and set light sensor high.
		GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1);
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);
		Timer_start(timer1);
		//Turn on timer1 for 12 microseconds.
		//Pend on LightSema
		Semaphore_pend(Sema, BIOS_WAIT_FOREVER);

		switch (adState) {
		case 0: // no line dtected
			if (*results && (black_count >= 20)){ //make sure it is a full line.
				Clock_start(DataClock);
				wasWhite = 0;	//record state.
				adState = 1;	//next state.
			}
			break;
		case 1: //first rising edge detected

			//falling edge of first line
			if (!(*results) && !wasWhite && (black_count >= 20)) {
				wasWhite = 1;
				adState = 2;
				if ((black_count <= 40) && (black_count >= 20)) {	//single line detected
					if (drive_state == FOLLOW1 || prev_follow_state == FOLLOW1){
						//First black line found, progress to follow2
						//		and start acquiring data.
						drive_state = FOLLOW2;
						prev_follow_state = FOLLOW2;
						RMS_cnt = 0;
						RMS = 0;
						Clock_start(DataClock);
					}
					else if (drive_state == FOLLOW2 || prev_follow_state == FOLLOW2){
						//Second black line found, progress to follow3
						//		and stop acquiring data.
						prev_follow_state = FOLLOW3;
						drive_state = FOLLOW3;
						Clock_stop(DataClock);
						//RMS /= RMS_cnt;		//finalize RMS
						//RMS = sqrt(RMS);
						intToStr(RMS_cnt, RMS_cnt_str, 4);
						strcpy(FullBufferPtr, "\0");
						strcpy(FullBufferPtr, "R=\0");	//Clear the fullBuffer.
						intToStr(RMS, RMS_str, 4);
						strcat(FullBufferPtr, RMS_str);
						strcat(FullBufferPtr, "\nC=\0");
						strcat(FullBufferPtr, RMS_cnt_str);
						strcat(FullBufferPtr, "\n\0");
						writeFrame("",FullBufferPtr);
						//Semaphore_post(TxDataSema);		//Let writeFrame continue.
						//Clock_stop(DataClock);
					}
				}else if (black_count > 40){ //double line detected
					StopDrivingFn();
					Clock_stop(DataClock);
				}
				black_count = 0;

			}
			break;
		case 2: //first falling edge detected
			if (*results && wasWhite && (black_count >= 20)){
				wasWhite = 0;
				adState = 3;
			}

			break;
		case 3: //second rising edge detected

			//falling edge of second line
			if (!(*results) && !wasWhite && (black_count >= 20)) {
				wasWhite = 1;
				adState = 4;
				if ((black_count <= 40) && (black_count >= 20)) {	//single line detected
					if (drive_state == FOLLOW1 || prev_follow_state == FOLLOW1){
						//First black line found, progress to follow2
						//		and start acquiring data.
						drive_state = FOLLOW2;
						prev_follow_state = FOLLOW2;
						RMS_cnt = 0;
						RMS = 0;
						Clock_start(DataClock);
					}
					else if (drive_state == FOLLOW2 || prev_follow_state == FOLLOW2){
						//First black line found, progress to follow3
						//		and stop acquiring data.
						prev_follow_state = FOLLOW3;
						drive_state = FOLLOW3;
						//Clock_stop(DataClock);
						Clock_stop(DataClock);
						//RMS /= RMS_cnt;		//finalize RMS
						//RMS = sqrt(RMS);
						intToStr(RMS_cnt, RMS_cnt_str, 4);
						strcpy(FullBufferPtr, "\0");
						strcpy(FullBufferPtr, "R=\0");	//Clear the fullBuffer.
						intToStr(RMS, RMS_str, 4);
						strcat(FullBufferPtr, RMS_str);
						strcat(FullBufferPtr, "\nC=\0");
						strcat(FullBufferPtr, RMS_cnt_str);
						strcat(FullBufferPtr, "\n\0");
						writeFrame("",FullBufferPtr);
						//Semaphore_post(TxDataSema);		//Let writeFrame continue.
					}
				}else if (black_count > 40){//double line detected
					//StopDrivingFn();
					Clock_stop(DataClock);
				}
				black_count = 0;
			}
			break;
		case 4: //second falling edge detected. Done collecting data.
			//Clock_stop(DataClock);
			//StopAcquireData();

			Clock_stop(DataClock);
			//RMS /= RMS_cnt;		//finalize RMS
			//RMS = sqrt(RMS);
			intToStr(RMS_cnt, RMS_cnt_str, 4);
			strcpy(FullBufferPtr, "\0");
			strcpy(FullBufferPtr, "R=\0");	//Clear the fullBuffer.
			intToStr(RMS, RMS_str, 4);
			strcat(FullBufferPtr, RMS_str);
			strcat(FullBufferPtr, "\nC=\0");
			strcat(FullBufferPtr, RMS_cnt_str);
			strcat(FullBufferPtr, "\n\0");
			writeFrame("",FullBufferPtr);
			//Semaphore_post(TxDataSema);		//Let writeFrame continue.
			//push whats remaining in the buffer out to transfer it.
			strcpy(FullBufferPtr, "\0");
			strcat(pingptr1, "\n\0");
			strcpy(FullBufferPtr, pingptr1);
			Semaphore_post(TxDataSema);
			strcpy(pingptr1, "\0");
			numChars = 0;
			wasWhite = 0;
			adState = 0;
			break;
		default:
			break;
		}

}

void lightSetInputFn() {
	//return pin to input and start the second timer.
	Timer_stop(timer1);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1);
	Timer_start(timer2);
}
void ReadLightFn () {
	while(1){
		//Get the up to date light value.
		ReadLightW(LightSema, &lightPtr);
		Task_sleep(2);
	}

}
void lightReadFn() {
	//Read the pin then switch the port to output and post the LightSema.
	Timer_stop(timer2);
	if (GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1)){
		lightPtr = 1; //Dark if output of pin is high
		black_count++;
	}
	else {
		lightPtr = 0; //Light if output of pin is low
	}
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1);
	Semaphore_post(LightSema);
}

void DataClockFn() {
	Semaphore_post(DataSema);
}


