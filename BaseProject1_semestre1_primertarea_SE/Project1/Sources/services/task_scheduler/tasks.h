/****************************************************************************************************/
/**
\file       tasks.h
\brief      Tasks to be assigned to each execution thread of Task scheduler.
\author     Abraham Tezmol
\version    1.0
\date       31/10/2008
*/
/****************************************************************************************************/

#ifndef __TASKS_H
#define    __TASKS_H    

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

/** GPIO Indication functions */
#include    "GPIO.h"
/** COP watchdog functions and services */
#include    "watchdog.h"
/** Example1 Definitions */
#include    "pin_toggle.h"

/*****************************************************************************************************
* Definition of module wide VARIABLES
*****************************************************************************************************/

/*****************************************************************************************************
* Declaration of module wide TYPES
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide MACROS / #DEFINE-CONSTANTS 
*****************************************************************************************************/

/* List of tasks to be executed @ 1ms */
#define    EXECUTE_1MS_TASKS( )                                 \
                {;}               

/* List of tasks to be executed @ 2ms, first group */
#define    EXECUTE_2MS_A_TASKS( )                               \
                {;}        

/* List of tasks to be executed @ 2ms, second group */
#define    EXECUTE_2MS_B_TASKS( )                               \
                {;}  
                  
/* List of tasks to be executed @ 10ms */
#define    EXECUTE_10MS_TASKS( )                                \
                {;}              

/* List of tasks to be executed @ 50ms */
#define    EXECUTE_50MS_TASKS( )                                \
                {;}
                
/* List of tasks to be executed @ 100ms */                
#define    EXECUTE_100MS_TASKS( )                               \
            {                                                   \
              vfnPinToggle();                                    \
              vfnCOPWatchdog_Reset();                           \
            }
                  
/*****************************************************************************************************
* Declaration of module wide FUNCTIONS
*****************************************************************************************************/

#endif /*__TASKS_H */ 
