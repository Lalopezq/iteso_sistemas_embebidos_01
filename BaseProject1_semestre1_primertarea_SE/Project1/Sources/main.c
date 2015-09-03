/****************************************************************************************************/
/**
  \mainpage
  \n 
  \brief        Main application (main module)
  \author       Abraham Tezmol Otero, M.S.E.E
  \project      Omicron 
  \version      1.0
  \date         26/Jun/2009
   
  Program compiled with CW for HCS12(X) v5.0, tested on DEMO9S12XEP100 board
*/
/****************************************************************************************************/

/** Core modules */
/** Omicron Configuration Options */
#include     "configuration.h"
/** Variable types and common definitions */
#include     "typedefs.h"
/** MCU derivative information */
#include     __MCU_DERIVATIVE
#pragma LINK_INFO DERIVATIVE __MCU_LINK_DERIVATIVE
/** Common defines and macros */
#include     <hidef.h>

/** Used modules */
/** PLL definitions and prototypes*/
#include     "pll.h"
/** Scheduler function prototypes definitions */
#include     "scheduler.h"
/** COP watchdog functions and services */
#include     "watchdog.h"
/** Interrupts module functions and services */
#include     "interrupt.h"
/** Microcontroller input output low-level pins initialization */
#include     "io.h"

/*****************************************************************************************************
* Definition of module wide VARIABLEs 
*****************************************************************************************************/

/****************************************************************************************************
* Declaration of module wide FUNCTIONs 
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of global wide MACROs / #DEFINE-CONSTANTs
*****************************************************************************************************/

/*****************************************************************************************************
* Declaration of module wide TYPEs 
*****************************************************************************************************/

/******************************************************************************************************
* Definition of module wide VARIABLEs 
******************************************************************************************************/

/******************************************************************************************************
* Code of module wide FUNCTIONS
******************************************************************************************************/

void vfnBackgroundSubsystemTasks(void);

/*~~~~~~~ Main Code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void main(void) 
{       
    /* Disable interrupts */
    vfnInterrupt_Disable(); 
    /* Initialize Interrupt Module */
    vfnInterrupt_Init();          
    /* PLL initialization to the desired target frequency */
    vfnPLL_Init();
    /* Microcontroller Input/Output pins initialization */
    vfnInputs_Outputs_Init();
    /* Initialize Task Scheduler */ 
    vfnScheduler_Init();
    /* Start execution of task scheduler */
    vfnScheduler_Start();
    /* Arm COP watchdog */
    vfnCOPWatchdog_Init();
    /* Once all initializations are complete, enable interrupts */
    vfnInterrupt_Enable();     
    
    /*-- Loop through all the periodic tasks from Task Scheduler --*/
    for(;;)
    {
        /* Perform all scheduled tasks */
        vfnTask_Scheduler();
        /* Perform all background susbystem control tasks */
        vfnBackgroundSubsystemTasks();
                   
    }
}
/*~~~~~~~ End of Main Code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* Background tasks related to Subsystem control */
void vfnBackgroundSubsystemTasks(void) 
{;}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/