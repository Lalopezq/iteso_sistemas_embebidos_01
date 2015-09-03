/****************************************************************************************************/
/**
\file       scheduler.h
\brief      Task scheduler function prototypes
\author     Abraham Tezmol
\version    1.0
\date       31/10/2008
*/
/****************************************************************************************************/

#ifndef __SCHEDULER_H        /*prevent duplicated includes*/ 
#define    __SCHEDULER_H    

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

/** Core Modules */
/** Configuration Options */
#include    "configuration.h"
/** S12X derivative information */
#include    __MCU_DERIVATIVE
/** Variable types and common definitions */
#include    "typedefs.h"

/** Tasks definition */
#include    "tasks.h"
/** PIT definitions and prototypes */
#include    "pit.h"

/*****************************************************************************************************
* Declaration of module wide TYPES
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide MACROS / #DEFINE-CONSTANTS 
*****************************************************************************************************/

/* Task Scheduler state machine status definitions */
#define    NO_TASK_PENDING                  0x00u
#define    TASKS_1_MS                       0x01u
#define    TASKS_2_MS_A                     0x02u
#define    TASKS_2_MS_B                     0x03u
#define    TASKS_10_MS                      0x04u
#define    TASKS_50_MS                      0x05u
#define    TASKS_100_MS                     0x06u

/* Global Task Scheduler Status definitions */
#define    TASK_SCHEDULER_INIT              0x00u
#define    TASK_SCHEDULER_RUNNING           0x01u
#define    TASK_SCHEDULER_OVERLOAD_1MS      0x02u
#define    TASK_SCHEDULER_OVERLOAD_2MS_A    0x03u
#define    TASK_SCHEDULER_OVERLOAD_2MS_B    0x04u
#define    TASK_SCHEDULER_HALTED            0xAAu

/*****************************************************************************************************
* Declaration of module wide FUNCTIONS
*****************************************************************************************************/

/** Sheduler Initalization (arming) */
void vfnScheduler_Init( void  );

/** Scheduler kick-off function */
void vfnScheduler_Start( void  );

/** Scheduler stop function */
void vfnScheduler_Stop( void  );

/** Multi-thread round robin task scheduler */
void vfnTask_Scheduler( void  );

/** ISR that provides time base to MTRR Task Scheduler - */
//#pragma CODE_SEG __NEAR_SEG NON_BANKED
void vfnScheduler_Callback( void  );
//#pragma CODE_SEG DEFAULT


#endif /* __SCHEDULER_H */ 