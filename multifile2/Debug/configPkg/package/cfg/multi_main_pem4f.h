/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-A71
 */

#include <xdc/std.h>

#include <ti/sysbios/knl/Clock.h>
extern const ti_sysbios_knl_Clock_Handle DataClock;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle TxDataSema;

#include <ti/sysbios/hal/Timer.h>
extern const ti_sysbios_hal_Timer_Handle timer1;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle DataSema;

#include <ti/sysbios/hal/Timer.h>
extern const ti_sysbios_hal_Timer_Handle timer2;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle LightSema;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle DriveSema;

#include <ti/sysbios/knl/Clock.h>
extern const ti_sysbios_knl_Clock_Handle DriveClock;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle Drive;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle Uturn_Sema;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle Rturn_Sema;

#include <ti/sysbios/hal/Timer.h>
extern const ti_sysbios_hal_Timer_Handle Rturn_timer;

#include <ti/sysbios/hal/Timer.h>
extern const ti_sysbios_hal_Timer_Handle Uturn_timer;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle CmdBuffer_RWLock;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle NewCmdSema;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle TX_RWLock;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle TxResponseSema;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle UARTDoneSema;

#include <ti/sysbios/hal/Hwi.h>
extern const ti_sysbios_hal_Hwi_Handle hwi_readChar;

#include <ti/sysbios/knl/Swi.h>
extern const ti_sysbios_knl_Swi_Handle swi_procChar;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle AcquireData;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle CmdInt;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle ReadLight;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle RxCmd;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle TxData;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle TxResponse;

#define TI_DRIVERS_WIFI_INCLUDED 0

extern int xdc_runtime_Startup__EXECFXN__C;

extern int xdc_runtime_Startup__RESETFXN__C;

#ifndef ti_sysbios_knl_Task__include
#ifndef __nested__
#define __nested__
#include <ti/sysbios/knl/Task.h>
#undef __nested__
#else
#include <ti/sysbios/knl/Task.h>
#endif
#endif

extern ti_sysbios_knl_Task_Struct TSK_idle;

