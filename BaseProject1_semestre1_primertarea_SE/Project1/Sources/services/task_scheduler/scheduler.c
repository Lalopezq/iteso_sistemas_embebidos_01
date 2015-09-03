/****************************************************************************************************/
/**
\file       scheduler.c
\brief      Multi-thread Task scheduler.
\author     Abraham Tezmol
\version    1.0
\date       31/10/2008
*/
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

/** Scheduler function prototypes definitions */
#include    "scheduler.h"

/*****************************************************************************************************
* Definition of  VARIABLEs - 
*****************************************************************************************************/
UINT8 gu8Scheduler_Status;
UINT8 gu8Scheduler_Counter;
UINT8 gu8Scheduler_Thread_ID;
UINT8 gu8Scheduler_Thread_ID_Backup;
UINT8 u8_10ms_Counter;
UINT8 u8_50ms_Counter;
UINT8 u8_100ms_Counter;

/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/

/*****************************************************************************************************
* Code of module wide FUNCTIONS
*****************************************************************************************************/

/****************************************************************************************************/
/**
* \brief    Scheduler - Initialization
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo     
*/
void vfnScheduler_Init( void  )
{
    /* Periodic Interrupt Timer Low Level Initialization */
    vfnPIT0_Init(vfnScheduler_Callback, __S12_CORE);

    /* Init Global and local Task Scheduler variables */
    gu8Scheduler_Counter   = 0u;
    gu8Scheduler_Thread_ID = NO_TASK_PENDING;
    u8_10ms_Counter        = 0u;
    u8_50ms_Counter        = 0u;
    u8_100ms_Counter       = 0u;
    gu8Scheduler_Status    = TASK_SCHEDULER_INIT;
}

/***************************************************************************************************/
/**
* \brief    Scheduler Start - Once time base is armed, start execution of   \n
            Multi-thread Round Robin scheduling scheme.                     \n
            This function requires prior execution of "vfnScheduler_Init"
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo     
*/
void vfnScheduler_Start( void  )
{
    /* Perform Low level PIT start */
    vfnPIT0_Start();
    
    /* Update Scheduler Status */
    gu8Scheduler_Status = TASK_SCHEDULER_RUNNING;
}

/***************************************************************************************************/
/**
* \brief    Scheduler Stop - stop execution of Multi-thread Round Robin scheduling scheme.
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo     
*/
void vfnScheduler_Stop( void  )
{
    /* Perform Low level PIT stop */
    vfnPIT_Stop();
    
    /* Update scheduler status accordingly */
    gu8Scheduler_Status = TASK_SCHEDULER_HALTED;
}

/***************************************************************************************************/
/**
* \brief    Multi-thread round robin task Scheduler  (non-preemtive)        \n
            It calls the different tasks based on the status of             \n   
            "gu8Scheduler_Thread_ID". This variable is modified by          \n
            ISR "vfnPIT_Channel0_Isr"                                        \n
            List of tasks shall be defined @ "tasks.h" file
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo     
*/
void vfnTask_Scheduler( void  )
{
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*  1ms execution thread - used to derive two execution threads:                */
    /*  a) 1ms thread (high priority tasks)                                         */
    /*  b) 100ms thread (lowest priority tasks)                                     */
    /*  As any other thread on this scheduler, all tasks must be executed in <=500us*/
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    if( ( gu8Scheduler_Thread_ID == TASKS_1_MS )
         || ( gu8Scheduler_Thread_ID == TASKS_100_MS ) )
    {
        /* Make a copy of scheduled task ID */
        gu8Scheduler_Thread_ID_Backup = gu8Scheduler_Thread_ID;

        EXECUTE_1MS_TASKS ()
        if( gu8Scheduler_Thread_ID == TASKS_100_MS )
        {
            EXECUTE_100MS_TASKS ()
        }
        /* Verify that thread execution took less than 500 us */
        if( gu8Scheduler_Thread_ID_Backup == gu8Scheduler_Thread_ID )
        {
             /* In case execution of all thread took less than 500us */
            gu8Scheduler_Thread_ID = NO_TASK_PENDING;
        }
        else
        {
            gu8Scheduler_Status = TASK_SCHEDULER_OVERLOAD_1MS;
        }
    }
    else
    {
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*  2ms execution thread - used to derive two execution threads:                */
        /*  a) 2ms group A thread (high priority tasks)                                 */
        /*  b) 50ms thread (second lowest priority tasks)                               */
        /*  As any other thread on this scheduler, all tasks must be executed in <=500us*/
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        if( ( gu8Scheduler_Thread_ID == TASKS_2_MS_A )
             || ( gu8Scheduler_Thread_ID == TASKS_50_MS ) )
        {
            /* Make a copy of scheduled task ID */
            gu8Scheduler_Thread_ID_Backup = gu8Scheduler_Thread_ID;

            EXECUTE_2MS_A_TASKS ()
            if( gu8Scheduler_Thread_ID == TASKS_50_MS )
            {
                EXECUTE_50MS_TASKS ()
            }
            /* Verify that thread execution took less than 500 us */
            if( gu8Scheduler_Thread_ID_Backup == gu8Scheduler_Thread_ID )
            {
                 /* In case execution of all thread took less than 500us */
                gu8Scheduler_Thread_ID = NO_TASK_PENDING;
            }
            else
            {
                gu8Scheduler_Status = TASK_SCHEDULER_OVERLOAD_2MS_A;
            }
        }
        else
        {
            /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
            /*  2ms execution thread - used to derive two execution threads:                */
            /*  a) 2ms group B thread (high priority tasks)                                 */
            /*  b) 10ms thread (medium priority tasks)                                      */
            /*  As any other thread on this scheduler, all tasks must be executed in <=500us*/
            /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
            if( ( gu8Scheduler_Thread_ID == TASKS_2_MS_B )
                 || ( gu8Scheduler_Thread_ID == TASKS_10_MS ) )
            {
                /* Make a copy of scheduled task ID */
                gu8Scheduler_Thread_ID_Backup = gu8Scheduler_Thread_ID;

                EXECUTE_2MS_B_TASKS ()
                if( gu8Scheduler_Thread_ID == TASKS_10_MS )
                {
                    EXECUTE_10MS_TASKS ()
                }
                 /* Verify that thread execution took less than 500 us */
                if( gu8Scheduler_Thread_ID_Backup == gu8Scheduler_Thread_ID )
                {
                    /* In case execution of all thread took less than 500us */
                    gu8Scheduler_Thread_ID = NO_TASK_PENDING;
                }
                else
                {
                    gu8Scheduler_Status = TASK_SCHEDULER_OVERLOAD_2MS_B;
                }
            }
        }
    }
}

/***************************************************************************************************/
/**
* \brief    Periodic callback function to be called by the main timer ISR       \n
            This interrupt is the core of the task scheduler.                   \n
            It is executed every 500us                                          \n
            It defines 3 basic threads from which other 3 threads are derived:  \n
            a) 1ms thread (basic) ->  100ms thread (derived)                    \n
            b) 2ms A thread (basic)-> 50ms thread (derived)                     \n
            c) 2ms B thread (basic)-> 10ms thread (derived)                     \n
            It partitions core execution time into time slices (500us each one).\n 
            This arrangement assures core will have equal task loading across time.\n   
            For more information on how time slice is assigned to each thread,  \n
            refer to file "S12X Task Scheduler Layout.xls"
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo
*/

void vfnScheduler_Callback( void  )
{
    /*-- Update scheduler control variable --*/
    gu8Scheduler_Counter++;

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /*  1ms execution thread - used to derive two execution threads:                */
    /*  a) 1ms thread (highest priority tasks)                                      */
    /*  b) 100ms thread (lowest priority tasks)                                     */
    /*  As any other thread on this scheduling scheme,                              */
    /*  all tasks must be executed in <= 500us                                      */
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    if( ( gu8Scheduler_Counter & 0x01u ) == 0x01u )
    {
        u8_100ms_Counter++;
        /*-- Allow 100 ms periodic tasks to be executed --*/
        if( u8_100ms_Counter >= 100u )
        {
            gu8Scheduler_Thread_ID = TASKS_100_MS;
            u8_100ms_Counter       = 0u;
        }
        /*-- Allow 1 ms periodic tasks to be executed --*/
        else
        {
            gu8Scheduler_Thread_ID = TASKS_1_MS;
        }
    }
    else
    {
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /*  2ms execution thread - used to derive two execution threads:                */
        /*  a) 2ms group A thread (high priority tasks)                                 */
        /*  b) 50ms thread (second lowest priority tasks)                               */
        /*  As any other thread on this scheduling scheme,                              */
        /*  all tasks must be executed in <= 500us                                      */
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        if( ( gu8Scheduler_Counter & 0x02u ) == 0x02u )
        {
            u8_50ms_Counter++;
            /*-- Allow 50 ms periodic tasks to be executed --*/
            if( u8_50ms_Counter >= 25u )
            {
                gu8Scheduler_Thread_ID = TASKS_50_MS;
                u8_50ms_Counter        = 0u;
            }
            /*-- Allow 2 ms group A periodic tasks to be executed --*/
            else
            {
                gu8Scheduler_Thread_ID = TASKS_2_MS_A;
            }
        }
        else
        {
            /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
            /*  2ms execution thread - used to derive two execution threads:                */
            /*  a) 2ms group B thread (high priority tasks)                                 */
            /*  b) 10ms thread (medium priority tasks)                                      */
            /*  As any other thread on this scheduling scheme,                              */
            /*  all tasks must be executed in <= 500us                                      */
            /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
            if( ( gu8Scheduler_Counter & 0x03u ) == 0x00u )
            {
                u8_10ms_Counter++;
                /*-- Allow 10 ms periodic tasks to be executed --*/
                if( u8_10ms_Counter >= 5u )
                {
                    gu8Scheduler_Thread_ID = TASKS_10_MS;
                    u8_10ms_Counter        = 0u;
                }
                /*-- Allow 2 ms group B periodic tasks to be executed --*/
                else
                {
                    gu8Scheduler_Thread_ID = TASKS_2_MS_B;
                }
            }
        }
    }
}
/***************************************************************************************************/
