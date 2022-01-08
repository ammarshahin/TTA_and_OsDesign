/******************************************************************************/
/* @File            : InverterCom.c                                    		  */
/*                                                                            */
/* @Designed by     : Ammar Shahin                         @Date : 29-6-2020  */
/*                                                                            */
/* @Designed by     : Ammar Shahin                         @Date : 29-6-2020  */
/*                                                                            */
/* @Coding language : C                                                       */
/*                                                                            */
/* @COPYRIGHT 2020 El-ARABY Research and development center.                  */
/* all rights reserved                                                        */
/******************************************************************************/

/************************************************************************/
/*                            Includes                                  */
/************************************************************************/
#include "std_types.h"
#include "utils.h"
#include "sw_uart.h"
#include "comm_manger.h"
#include "config.h"
#include "app_inverter_comm.h"

#define debug 0

#if debug
#include "uart.h"
static uint8_t buff[10] = {0};
#endif

/************************************************************************/
/*                         Private MACROS                               */
/************************************************************************/
#define INVERTER_COMMUNICATION_TIMING_MS (5000)

#define INTERVAL_TIME_MS 13
#define TIMEOUT_TIME_MS 2500

/* ID - Command - LowByte - HighByte - CRC */

#define MAIN_MCU_ID 0xB7
#define INV_MCU_ID 0x7B

/* Set Speed frame strucure */
#define SETSPEED_COMMAND 0xD2

#define SETSPEED_Rx_COMMAND 0xA4

/* Read Setup Speed frame strucure */
#define READSETUPSPEED_COMMAND 0x2D
#define READSETUPSPEED_LowByte 0xA1
#define READSETUPSPEED_HighByte 0x01
#define READSETUPSPEED_CRC 0x7A

#define READSETUPSPEED_Rx_COMMAND 0xA1

/* Read Actual Speed frame strucure */
#define READACTUALSPEED_COMMAND 0x2D
#define READACTUALSPEED_LowByte 0xA2
#define READACTUALSPEED_HighByte 0x01
#define READACTUALSPEED_CRC 0x79

#define READACTUALSPEED_Rx_COMMAND 0xA2

/* Read Input Voltage frame strucure */
#define READINPUTVOLTAGE_COMMAND 0x2D
#define READINPUTVOLTAGE_LowByte 0xA3
#define READINPUTVOLTAGE_HighByte 0x01
#define READINPUTVOLTAGE_CRC 0x78

#define READINPUTVOLTAGE_Rx_COMMAND 0xA3

/* Read Status frame strucure */
#define READSTATUS_COMMAND 0x2D
#define READSTATUS_LowByte 0xA4
#define READSTATUS_HighByte 0x01
#define READSTATUS_CRC 0x77

#define READSTATUS_Rx_COMMAND 0xA4

/* Read PWB Check(1) frame strucure */
#define PWBCHECK1_COMMAND 0x71
#define PWBCHECK1_LowByte 0x00
#define PWBCHECK1_HighByte 0x00
#define PWBCHECK1_CRC 0xD8

#define PWBCHECK1_Rx_COMMAND 0x17

/* Read PWB Check(2) frame strucure */
#define PWBCHECK2_COMMAND 0x72
#define PWBCHECK2_LowByte 0x00
#define PWBCHECK2_HighByte 0x00
#define PWBCHECK2_CRC 0xD7

#define PWBCHECK2_Rx_COMMAND 0x27

/* Read Version frame strucure */
#define READVERSION_COMMAND 0x2D
#define READVERSION_LowByte 0xA5
#define READVERSION_HighByte 0x01
#define READVERSION_CRC 0x76

#define READVERSION_Rx_COMMAND 0xA5

/* CRC Factors Calc */
#define CRC_FACTOR1 0x100
#define CRC_FACTOR2 0x0ff

#define CRCGenerator(data) ((CRC_FACTOR1) - ((data) & (CRC_FACTOR2)))

/************************************************************************/
/*                           Private Enums                              */
/************************************************************************/
/* Main Inverter State Machine */
typedef enum
{
    INV_SET_SPEED_STATE = 0,     // A
    INV_READ_SETSPEED_STATE,     // B
    INV_READ_ACTUALSPEED_STATE,  // C
    INV_READ_INPUTVOLTAGE_STATE, // D
    INV_READ_STATUS_STATE,       // E
    INV_PWB_CHECK1_STATE,        // F
    INV_PWB_CHECK2_STATE,        // G
    INV_READ_VERSION_STATE,      // H
    INV_STATES_NUM
} tEnum_InverterComState;

/* Inner State Machine */
typedef enum
{
    INVCOM_PROSESSING = 1,
    INVCOM_SENDING,
    INVCOM_RECEIVING
} tEnum_InnerState;

typedef enum
{
    NO_CHANGE = 1,
    SPEED_CHANGE_1st,
    SPEED_CHANGE_2nd,
    ON_OFF_1ST,
    ON_OFF_2ND
} tEnum_CompressorChange;

/************************************************************************/
/*                           Private Structs                            */
/************************************************************************/
typedef struct
{
    uint8_t Main_Command;
    uint8_t Main_LowByte;
    uint8_t Main_HighByte;
    uint8_t Main_CRC;
    uint8_t Inv_Command;
    uint8_t Inv_LowByte;
    uint8_t Inv_HighByte;
    uint8_t Inv_CRC;
} tStruct_ComCfgType;
/************************************************************************/
/*                   GLOBAL STATIC(Private) VARIABLES                   */
/************************************************************************/
static uint16_t gu16_inverterCommMainTimer;

static bool_t gbool_stateInProgress_flag;

static uint16_t gu16_userSetSpeed;
static uint16_t gu16_oldUserSetSpeed;

static uint16_t gu16_receivedSetSpeed;
static uint16_t gu16_receivedActualSpeed;

static uint16_t gu16_receivedInputVoltage;

static uint16_t gu16_receivedVoltageDoubler;
static uint16_t gu16_receivedIpmThrData;

static uint16_t gu16_receivedInverterVersion;

static tEnum_CompressorStatus gu16_receivedCompressorState;
static tEnum_CompressorStatus gu16_receivedOldCompressorState;

static tEnum_InverterComState g_inverterModuleCurrentState;
static tEnum_InverterComState g_inverterModuleNextState;

static tEnum_CompressorChange g_compSpeedChange_flag;

static tStruct_ComCfgType gArr_inverterCommCfgArray[INV_STATES_NUM] = {
    {SETSPEED_COMMAND, 0, 0, 0, SETSPEED_Rx_COMMAND, 0, 0, 0},
    {READSETUPSPEED_COMMAND, READSETUPSPEED_LowByte, READSETUPSPEED_HighByte, READSETUPSPEED_CRC, READSETUPSPEED_Rx_COMMAND, 0, 0, 0},
    {READACTUALSPEED_COMMAND, READACTUALSPEED_LowByte, READACTUALSPEED_HighByte, READACTUALSPEED_CRC, READACTUALSPEED_Rx_COMMAND, 0, 0, 0},
    {READINPUTVOLTAGE_COMMAND, READINPUTVOLTAGE_LowByte, READINPUTVOLTAGE_HighByte, READINPUTVOLTAGE_CRC, READINPUTVOLTAGE_Rx_COMMAND, 0, 0, 0},
    {READSTATUS_COMMAND, READSTATUS_LowByte, READSTATUS_HighByte, READSTATUS_CRC, READSTATUS_Rx_COMMAND, 0, 0, 0},
    {PWBCHECK1_COMMAND, PWBCHECK1_LowByte, PWBCHECK1_HighByte, PWBCHECK1_CRC, PWBCHECK1_Rx_COMMAND, 0, 0, 0},
    {PWBCHECK2_COMMAND, PWBCHECK2_LowByte, PWBCHECK2_HighByte, PWBCHECK2_CRC, PWBCHECK2_Rx_COMMAND, 0, 0, 0},
    {READVERSION_COMMAND, READVERSION_LowByte, READVERSION_HighByte, READVERSION_CRC, READVERSION_Rx_COMMAND, 0, 0, 0}};

/* Com manger variables */
static uint8_t gu18Arr_TxBuffer[MAX_BUFFER_SIZE];
static uint8_t gu18Arr_RxBuffer[MAX_BUFFER_SIZE];

static Str_commMangerGlobalStr_t gStr_TxBuffer;
static Str_commMangerGlobalStr_t gStr_RxBuffer;
/************************************************************************/
/*                     Private Functions prototypes                     */
/************************************************************************/

/************** Main states transient function ***********/
static void app_inverter_comm_stateTransient(void);

/************** Main state machine state functions ***********/
static void app_inverter_comm_setSpeedData_stateProcess(void);
static void app_inverter_comm_readSetupSpeedData_stateProcess(void);
static void app_inverter_comm_readActualSpeedData_stateProcess(void);
static void app_inverter_comm_readInputVoltage_stateProcess(void);
static void app_inverter_comm_readStatus_stateProcess(void);
static void app_inverter_comm_pwbCheck1_stateProcess(void);
static void app_inverter_comm_pwbCheck2_stateProcess(void);
static void app_inverter_comm_readVersionData_stateProcess(void);

/************** Inner State machine for every main inverter state ***********/
static void app_inverter_comm_stateExecution(void);

/************** Compressor state check function ***********/
static void app_inverter_comm_checkForComperssorState(void);

/************************************************************************/
/*                           APIs(Public) Functions                      */
/************************************************************************/
/**
 * Function :  app_inverter_comm_init
 * Description: This function is to initialize the Inverter Communiation Module
 * @return nothing 
 */
void app_inverter_comm_init(void)
{
    /* Global Variables initialization */
    gu16_userSetSpeed = 0;
    gu16_oldUserSetSpeed = 0;
    gu16_receivedSetSpeed = 0;
    gu16_receivedActualSpeed = 0;
    gu16_receivedInputVoltage = 0;
    gu16_receivedVoltageDoubler = 0;
    gu16_receivedIpmThrData = 0;
    gu16_receivedInverterVersion = 0;
    gu16_receivedCompressorState = NORMAL_OPERATION_OFF;
    gu16_receivedOldCompressorState = NORMAL_OPERATION_OFF;
    g_inverterModuleCurrentState = INV_READ_VERSION_STATE;
    g_inverterModuleNextState = INV_READ_VERSION_STATE;
    gbool_stateInProgress_flag = FALSE;
    g_compSpeedChange_flag = NO_CHANGE;
    /* Init the Sw Timer Inverter and Start it */
    gu16_inverterCommMainTimer = 0;
}

/**
 * Function : app_inverter_comm_Update
 * Description: This function is to update the Communication Module and implement the main state machine
 * @return nothing 
 */
void app_inverter_comm_update(void *args)
{
    gu16_inverterCommMainTimer += INVERTER_COMM_UPDATE_PERIOD_MS;
    /* check for time */
    if (gu16_inverterCommMainTimer >= INVERTER_COMMUNICATION_TIMING_MS)
    {
        g_inverterModuleCurrentState = g_inverterModuleNextState;
        gbool_stateInProgress_flag = TRUE;
        gu16_inverterCommMainTimer = 0;
    }
    else
    {
        // Do Nothing
    }

    /* Call the state transient function */
    app_inverter_comm_stateTransient();

    /* Excute the current state */
    switch (g_inverterModuleCurrentState)
    {
    case INV_SET_SPEED_STATE:
        app_inverter_comm_setSpeedData_stateProcess();
        break;
    case INV_READ_SETSPEED_STATE:
        app_inverter_comm_readSetupSpeedData_stateProcess();
        break;
    case INV_READ_ACTUALSPEED_STATE:
        app_inverter_comm_readActualSpeedData_stateProcess();
        break;
    case INV_READ_STATUS_STATE:
        app_inverter_comm_readStatus_stateProcess();
        break;
    case INV_READ_INPUTVOLTAGE_STATE:
        app_inverter_comm_readInputVoltage_stateProcess();
        break;
    case INV_PWB_CHECK1_STATE:
        app_inverter_comm_pwbCheck1_stateProcess();
        break;
    case INV_PWB_CHECK2_STATE:
        app_inverter_comm_pwbCheck2_stateProcess();
        break;
    case INV_READ_VERSION_STATE:
        app_inverter_comm_readVersionData_stateProcess();
        break;
    default:
        // Do Nothing
        break;
    }
}

/************************************************************************/
/*                 Private Functions Implementation                     */
/************************************************************************/

/**
 * Function : app_inverter_comm_stateTransient
 * Description: This function is to run the Main States Transient.
 * @return Nothing
 */
static void app_inverter_comm_stateTransient(void)
{

    /* Main States Transient */
    /*  1- check for the ON-OFF or OFF/ON Transient
        and Synchronous transmission every 5 seconds use a sw timer to determine the time
        and the Current state is finished 
        >> Go to SetSpeed State 
        2- check for any speed change while the Current Speed != 0 
        and Synchronous transmission every 5 seconds use a sw timer to determine the time
        and the Current state is finished
        >> Go to Read Status State  */
    if (((gu16_userSetSpeed != gu16_oldUserSetSpeed) && (gu16_oldUserSetSpeed == 0)) &&
        (gbool_stateInProgress_flag == FALSE)) // OFF_____------ON Transient
    {
        /* Go to the Set Speed State */
        g_inverterModuleNextState = INV_SET_SPEED_STATE;
        /* Set the Old Speed to be equal to the New one */
        gu16_oldUserSetSpeed = gu16_userSetSpeed;
        /* Set the New speed in the global array */
        gArr_inverterCommCfgArray[INV_SET_SPEED_STATE].Main_LowByte = (uint8_t)gu16_userSetSpeed;
        gArr_inverterCommCfgArray[INV_SET_SPEED_STATE].Main_HighByte = (uint8_t)(gu16_userSetSpeed >> 8);
        /* set the transient On_off flag to the first SetSpeed state */
        g_compSpeedChange_flag = ON_OFF_1ST;
    }
    else if (((gu16_userSetSpeed != gu16_oldUserSetSpeed) && (gu16_userSetSpeed == 0)) &&
             (gbool_stateInProgress_flag == FALSE)) // ON-----_____OFF Transient
    {
        /* Go to the Set Speed State */
        g_inverterModuleNextState = INV_SET_SPEED_STATE;
        /* Set the Old Speed to be equal to the New one */
        gu16_oldUserSetSpeed = gu16_userSetSpeed;
        /* Set the New speed in the global array */
        gArr_inverterCommCfgArray[INV_SET_SPEED_STATE].Main_LowByte = (uint8_t)gu16_userSetSpeed;
        gArr_inverterCommCfgArray[INV_SET_SPEED_STATE].Main_HighByte = (uint8_t)(gu16_userSetSpeed >> 8);
        /* set the transient On_off flag to the first SetSpeed state */
        g_compSpeedChange_flag = ON_OFF_1ST;
    }
    else if (((gu16_userSetSpeed != gu16_oldUserSetSpeed) && (gu16_oldUserSetSpeed != 0) && (gu16_userSetSpeed != 0)) &&
             (gbool_stateInProgress_flag == FALSE))
    {
        /* Go to the Read Status State */
        g_inverterModuleNextState = INV_READ_STATUS_STATE;
        /* Set the Old Speed to be equal to the New one */
        gu16_oldUserSetSpeed = gu16_userSetSpeed;
        /* Set the New speed in the global array */
        gArr_inverterCommCfgArray[INV_SET_SPEED_STATE].Main_LowByte = (uint8_t)gu16_userSetSpeed;
        gArr_inverterCommCfgArray[INV_SET_SPEED_STATE].Main_HighByte = (uint8_t)(gu16_userSetSpeed >> 8);
        /* set the compressor speed change flag to the SPEED_CHANGE */
        g_compSpeedChange_flag = SPEED_CHANGE_1st;
    }
    else
    {
        // Do Nothing
    }
}

/**
 * Function : app_inverter_comm_setSpeedData_stateProcess
 * Description: This function is to run the set speed state and run it's state machine.
 * @return Nothing
 */
static void app_inverter_comm_setSpeedData_stateProcess(void)
{
    /*
        1- Synchronous transmission every 5 seconds use a sw timer to determine the time
        and the Current state is finished
        >> Go to Read Setup Speed State 
        2- the Current state isn't finished
        >> Call The State Execution function
    */
    if ((g_compSpeedChange_flag == NO_CHANGE) && (gbool_stateInProgress_flag == FALSE))
    {
        g_inverterModuleNextState = INV_READ_SETSPEED_STATE;
    }
    else if (gbool_stateInProgress_flag == TRUE)
    {
        app_inverter_comm_stateExecution();
        if (gbool_stateInProgress_flag == FALSE)
        {
            if (g_compSpeedChange_flag == ON_OFF_1ST)
            {
                gu16_receivedOldCompressorState = (uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_LowByte | ((uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_HighByte << 8);
                g_compSpeedChange_flag = ON_OFF_2ND;
            }
            else if (g_compSpeedChange_flag == ON_OFF_2ND)
            {
                gu16_receivedCompressorState = (uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_LowByte | ((uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_HighByte << 8);
                app_inverter_comm_checkForComperssorState(); // Should compare the Old Compressor State with the new one
                g_compSpeedChange_flag = NO_CHANGE;
            }
            else if (g_compSpeedChange_flag == SPEED_CHANGE_2nd)
            {
                gu16_receivedCompressorState = (uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_LowByte | ((uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_HighByte << 8);
                app_inverter_comm_checkForComperssorState(); // Should compare the Old Compressor State with the new one
                g_compSpeedChange_flag = NO_CHANGE;
            }
            else
            {
                gu16_receivedCompressorState = (uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_LowByte | ((uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_HighByte << 8);
            }
        }
        else
        {
            // Do Nothing
        }
    }
    else
    {
        // Do Nothing
    }
}

/**
 * Function : app_inverter_comm_readSetupSpeedData_stateProcess
 * Description: This function is to run the read set speed state and run it's state machine.
 * @return Nothing
 */
static void app_inverter_comm_readSetupSpeedData_stateProcess(void)
{
    /*
        1- Synchronous transmission every 5 seconds use a sw timer to determine the time
        and the Current state is finished
        >> Go to Read Setup Speed State 
        2- the Current state isn't finished
        >> Call The State Execution function
    */
    if ((g_compSpeedChange_flag == NO_CHANGE) && (gbool_stateInProgress_flag == FALSE))
    {
        g_inverterModuleNextState = INV_READ_STATUS_STATE;
    }
    else if (gbool_stateInProgress_flag == TRUE)
    {
        app_inverter_comm_stateExecution();
        if (gbool_stateInProgress_flag == FALSE)
        {
            gu16_receivedSetSpeed = (uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_LowByte | ((uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_HighByte << 8);
        }
        else
        {
            // Do Nothing
        }
    }
    else
    {
        // Do Nothing
    }
}

/**
 * Function : app_inverter_comm_readActualSpeedData_stateProcess
 * Description: This function is to run the read Actual speed state and run it's state machine.
 * @return Nothing
 */
static void app_inverter_comm_readActualSpeedData_stateProcess(void)
{
    /*
        1- Synchronous transmission every 5 seconds use a sw timer to determine the time
        and the Current state is finished
        >> Go to Read Setup Speed State 
        2- the Current state isn't finished
        >> Call The State Execution function
    */
    if ((g_compSpeedChange_flag == NO_CHANGE) && (gbool_stateInProgress_flag == FALSE))
    {
        g_inverterModuleNextState = INV_READ_INPUTVOLTAGE_STATE;
    }
    else if (gbool_stateInProgress_flag == TRUE)
    {
        app_inverter_comm_stateExecution();
        if (gbool_stateInProgress_flag == FALSE)
        {
            gu16_receivedActualSpeed = (uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_LowByte | ((uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_HighByte << 8);
        }
        else
        {
            // Do Nothing
        }
    }
    else
    {
        // Do Nothing
    }
}

/**
 * Function : app_inverter_comm_readInputVoltage_stateProcess
 * Description: This function is to run the Read Input Voltage state and run it's state machine.
 * @return Nothing
 */
static void app_inverter_comm_readInputVoltage_stateProcess(void)
{
    /*
        1- Synchronous transmission every 5 seconds use a sw timer to determine the time
        and the Current state is finished
        >> Go to Read Setup Speed State 
        2- the Current state isn't finished
        >> Call The State Execution function
    */
    if ((g_compSpeedChange_flag == NO_CHANGE) && (gbool_stateInProgress_flag == FALSE))
    {
        g_inverterModuleNextState = INV_READ_VERSION_STATE;
    }
    else if (gbool_stateInProgress_flag == TRUE)
    {
        app_inverter_comm_stateExecution();
        if (gbool_stateInProgress_flag == FALSE)
        {
            gu16_receivedInputVoltage = (uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_LowByte | ((uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_HighByte << 8);
        }
        else
        {
            // Do Nothing
        }
    }
    else
    {
        // Do Nothing
    }
}

/**
 * Function : app_inverter_comm_readStatus_stateProcess
 * Description: This function is to run the Read Status state and run it's state machine.
 * @return Nothing
 */
static void app_inverter_comm_readStatus_stateProcess(void)
{
    /*
        1- Synchronous transmission every 5 seconds use a sw timer to determine the time
        and the Current state is finished
        >> Go to Read Setup Speed State 
        2- the Current state isn't finished
        >> Call The State Execution function
    */
    if ((g_compSpeedChange_flag == NO_CHANGE) && (gbool_stateInProgress_flag == FALSE))
    {
        g_inverterModuleNextState = INV_READ_ACTUALSPEED_STATE;
    }
    else if (gbool_stateInProgress_flag == TRUE)
    {
        app_inverter_comm_stateExecution();
        if (gbool_stateInProgress_flag == FALSE)
        {
            if (g_compSpeedChange_flag == SPEED_CHANGE_1st)
            {
                gu16_receivedOldCompressorState = (uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_LowByte | ((uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_HighByte << 8);
                g_inverterModuleNextState = INV_SET_SPEED_STATE;
                g_compSpeedChange_flag = SPEED_CHANGE_2nd;
            }
            else
            {
                gu16_receivedCompressorState = (uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_LowByte | ((uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_HighByte << 8);
            }
        }
        else
        {
            // Do Nothing
        }
    }
    else
    {
        // Do Nothing
    }
}

/**
 * Function : app_inverter_comm_pwbCheck1_stateProcess
 * Description: This function is to run the PWB Cheack(1) state and run it's state machine.
 * @return Nothing
 */
static void app_inverter_comm_pwbCheck1_stateProcess(void)
{
    /*  
        1- check for the ON-OFF or OFF/ON Transient
        and Synchronous transmission every 5 seconds use a sw timer to determine the time
        and the Current state is finished 
        >> 
        2- check for any speed change while the Compressor is on
        and Synchronous transmission every 5 seconds use a sw timer to determine the time
        and the Current state is finished
        3- Synchronous transmission every 5 seconds use a sw timer to determine the time
        and the Current state is finished
        4- the Current state isn't finished
    */
}

/**
 * Function : app_inverter_comm_pwbCheck2_stateProcess
 * Description: This function is to run the PWB Cheack(2) state and run it's state machine.
 * @return Nothing
 */
static void app_inverter_comm_pwbCheck2_stateProcess(void)
{
    /*  
        1- check for the ON-OFF or OFF/ON Transient
        and Synchronous transmission every 5 seconds use a sw timer to determine the time
        and the Current state is finished 
        >> 
        2- check for any speed change while the Compressor is on
        and Synchronous transmission every 5 seconds use a sw timer to determine the time
        and the Current state is finished
        3- Synchronous transmission every 5 seconds use a sw timer to determine the time
        and the Current state is finished
        4- the Current state isn't finished
    */
}

/**
 * Function : app_inverter_comm_readVersionData_stateProcess
 * Description: This function is to run the Read Version state and run it's state machine.
 * @return Nothing
 */
static void app_inverter_comm_readVersionData_stateProcess(void)
{
    /*
        1- Synchronous transmission every 5 seconds use a sw timer to determine the time
        and the Current state is finished
        >> Go to Read Setup Speed State 
        2- the Current state isn't finished
        >> Call The State Execution function
    */
    if ((g_compSpeedChange_flag == NO_CHANGE) && (gbool_stateInProgress_flag == FALSE))
    {
        g_inverterModuleNextState = INV_SET_SPEED_STATE;
    }
    else if (gbool_stateInProgress_flag == TRUE)
    {
        app_inverter_comm_stateExecution();
        if (gbool_stateInProgress_flag == FALSE)
        {
            gu16_receivedInverterVersion = (uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_LowByte | ((uint16_t)gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_HighByte << 8);
        }
        else
        {
            // Do Nothing
        }
    }
    else
    {
        // Do Nothing
    }
}

/*****************************************************************************/
/*                      Inner States Implementation                           */
/*****************************************************************************/
#if 0
static void app_inverter_comm_stateExecution(void)
{
    uint8_t tempVar = 0;
    static uint8_t CRC_Code = 0;
    static tEnum_InnerState InnerStateIterator = SEND_IDCode;
    // Inner state machine
    switch (InnerStateIterator)
    {
    case SEND_IDCode:
        tempVar = MAIN_MCU_ID;
        Send(tempVar);
        CRC_Code = tempVar;
        InnerStateIterator = SEND_Command;
        break;
    case SEND_Command:
        if (TxComplete() == TRUE)
        {
            TxFlagClr();
            tempVar = gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Main_Command;
            Send(tempVar);
            CRC_Code += tempVar;
            InnerStateIterator = SEND_LowByte;
        }
        break;
    case SEND_LowByte:
        if (TxComplete() == TRUE)
        {
            TxFlagClr();
            tempVar = gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Main_LowByte;
            Send(tempVar);
            CRC_Code += tempVar;
            InnerStateIterator = SEND_HighByte;
        }
        break;
    case SEND_HighByte:
        if (TxComplete() == TRUE)
        {
            TxFlagClr();
            tempVar = gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Main_HighByte;
            Send(tempVar);
            CRC_Code += tempVar;
            InnerStateIterator = SEND_CRC;
        }
        break;
    case SEND_CRC:
        if (TxComplete() == TRUE)
        {
            TxFlagClr();
            tempVar = CRCGenerator(CRC_Code);
            Send(tempVar);
            InnerStateIterator = RECEIVE_IDCode;
        }
        break;
    case RECEIVE_IDCode:
        if ((RxComplete() == TRUE) && (TxComplete() == TRUE))
        {
            RxFlagClr();
            TxFlagClr();
            tempVar = Read();
            if (tempVar != INV_MCU_ID)
            {
                // LOG_Error(INVALID_INV_ID);
            }
            CRC_Code = tempVar;
            InnerStateIterator = RECEIVE_Command;
        }
        break;
    case RECEIVE_Command:
        if (RxComplete() == TRUE)
        {
            RxFlagClr();
            tempVar = Read();
            if (tempVar != gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_Command)
            {
                // LOG_Error(INV_InvalidCommand);
            }
            CRC_Code += tempVar;
            InnerStateIterator = RECEIVE_LowByte;
        }
        break;
    case RECEIVE_LowByte:
        if (RxComplete() == TRUE)
        {
            RxFlagClr();
            tempVar = Read();
            gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_LowByte = tempVar;
            CRC_Code += tempVar;
            InnerStateIterator = RECEIVE_HighByte;
        }
        break;
    case RECEIVE_HighByte:
        if (RxComplete() == TRUE)
        {
            RxFlagClr();
            tempVar = Read();
            gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_HighByte = tempVar;
            CRC_Code += tempVar;
            InnerStateIterator = RECEIVE_CRC;
        }
        break;
    case RECEIVE_CRC:
        if (RxComplete() == TRUE)
        {
            RxFlagClr();
            tempVar = Read();
            CRC_Code = CRCGenerator(CRC_Code);
            if (tempVar != CRC_Code) // CRC Miss Matching
            {
                // LOG_Error(INV_WrongCRC);
            }
            InnerStateIterator = SEND_IDCode;
            gbool_stateInProgress_flag = FALSE;
            CRC_Code = 0;
        }
        break;
    default:
        break;
    }
}
#endif

/**
 * Function : app_inverter_comm_stateExecution
 * Description: This function is to Excute the current state.
 * @return Nothing
 */
static void app_inverter_comm_stateExecution(void)
{
    uint8_t internalIterator;
    Enm_commMangerError_t commMangerErrStatus;
    static uint8_t Txjob_ID = 0, Rxjob_ID = 0;
    static uint8_t CRC_Code = 0;
    static tEnum_InnerState InnerStateIterator = INVCOM_SENDING;
    if (InnerStateIterator == INVCOM_SENDING)
    {
        gu18Arr_TxBuffer[0] = MAIN_MCU_ID;
        gu18Arr_TxBuffer[1] = gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Main_Command;
        gu18Arr_TxBuffer[2] = gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Main_LowByte;
        gu18Arr_TxBuffer[3] = gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Main_HighByte;
        CRC_Code = 0;
        for (internalIterator = 0; internalIterator < 4; internalIterator++)
        {
            CRC_Code += gu18Arr_TxBuffer[internalIterator];
        }
        CRC_Code = CRCGenerator(CRC_Code);
        gu18Arr_TxBuffer[4] = CRC_Code;
        gStr_TxBuffer.pReqBuffer = gu18Arr_TxBuffer;
        gStr_TxBuffer.ReqBufferSize = MAX_BUFFER_SIZE;
        gStr_TxBuffer.CommProtocol = SW_UART;
        gStr_TxBuffer.ReqType = REQ_SENDING;
        gStr_TxBuffer.timeInterval_MS = INTERVAL_TIME_MS;
        gStr_TxBuffer.timeOut_MS = TIMEOUT_TIME_MS;
        commMangerErrStatus = comm_manger_reqSetup(gStr_TxBuffer, &Txjob_ID);
        if (commMangerErrStatus != OK_t)
        {
            // Error handling
        }
        InnerStateIterator = INVCOM_RECEIVING;
        CRC_Code = 0;
    }
    else if ((InnerStateIterator == INVCOM_RECEIVING) && (comm_manger_reqGetStatus(Txjob_ID) == REQ_OPERATION_FINISHED))
    {
        comm_manger_reqUninstall(Txjob_ID);
        gStr_RxBuffer.pReqBuffer = gu18Arr_RxBuffer;
        gStr_RxBuffer.ReqBufferSize = MAX_BUFFER_SIZE;
        gStr_RxBuffer.CommProtocol = SW_UART;
        gStr_RxBuffer.ReqType = REQ_RECEIVING;
        gStr_RxBuffer.timeInterval_MS = INTERVAL_TIME_MS;
        gStr_RxBuffer.timeOut_MS = TIMEOUT_TIME_MS;
        commMangerErrStatus = comm_manger_reqSetup(gStr_RxBuffer, &Rxjob_ID);
        if (commMangerErrStatus != OK_t)
        {
            // Error handling
        }
        InnerStateIterator = INVCOM_PROSESSING;
    }
    else if ((InnerStateIterator == INVCOM_PROSESSING) && (comm_manger_reqGetStatus(Rxjob_ID) == REQ_OPERATION_FINISHED))
    {
        comm_manger_reqUninstall(Rxjob_ID);
        /* For the data received it's fully inverted so we invert it back to process it */
        for (internalIterator = 0; internalIterator < 5; internalIterator++)
        {
            gu18Arr_RxBuffer[internalIterator] = ~gu18Arr_RxBuffer[internalIterator];
        }

#if debug
        uart_data_put(UART_1, "\n\r");
        for (internalIterator = 0; internalIterator < 5; internalIterator++)
        {

            utils_uitoa((uint32_t)(gu18Arr_RxBuffer[internalIterator]), buff);
            uart_data_put(UART_1, "\n\r");
            uart_data_put(UART_1, buff);
        }
        uart_data_put(UART_1, "\n\r");
#endif

        if (gu18Arr_RxBuffer[0] != INV_MCU_ID)
        {
            // LOG_Error(INVALID_INV_ID);
#if debug
            uart_data_put(UART_1, "INV_ID");
            uart_data_put(UART_1, "\n\r");
#endif
        }
        else
        {
            // Do Nothing
        }

        if (gu18Arr_RxBuffer[1] != gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_Command)
        {
            // LOG_Error(INV_InvalidCommand);
#if debug
            uart_data_put(UART_1, "INVCommand");
            uart_data_put(UART_1, "\n\r");
#endif
        }
        gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_LowByte = gu18Arr_RxBuffer[2];
        gArr_inverterCommCfgArray[g_inverterModuleCurrentState].Inv_HighByte = gu18Arr_RxBuffer[3];

        for (internalIterator = 0; internalIterator < 4; internalIterator++)
        {
            CRC_Code += gu18Arr_RxBuffer[internalIterator];
        }
        CRC_Code = CRCGenerator(CRC_Code);

        if (gu18Arr_RxBuffer[4] != CRC_Code)
        {
            // LOG_Error(INV_WrongCRC);
#if debug
            uart_data_put(UART_1, "\n\r");
            uart_data_put(UART_1, "WrongCRC=");
            utils_uitoa((uint32_t)CRC_Code, buff);
            uart_data_put(UART_1, buff);
            uart_data_put(UART_1, "\n\r");
#endif
        }
        gbool_stateInProgress_flag = FALSE;
        InnerStateIterator = INVCOM_SENDING;
        Txjob_ID = 0;
        Rxjob_ID = 0;
        CRC_Code = 0;
    }
    else if ((comm_manger_reqGetStatus(Txjob_ID) == REQ_OPERATION_TIMEOUT) || (comm_manger_reqGetStatus(Rxjob_ID) == REQ_OPERATION_TIMEOUT))
    {
        // LOG_Error(INV_RequestTimeOut);
#if debug
        uart_data_put(UART_1, "ReqTimeOut");
        uart_data_put(UART_1, "\n\r");
#endif
        comm_manger_reqUninstall(Txjob_ID);
        comm_manger_reqUninstall(Rxjob_ID);
        gbool_stateInProgress_flag = FALSE;
        InnerStateIterator = INVCOM_SENDING;
        Txjob_ID = 0;
        Rxjob_ID = 0;
        CRC_Code = 0;
    }
    else
    {
        // Do Nothing
    }
}

/*****************************************************************************/
/*                          Utility Functions                                */
/*****************************************************************************/
/**
 * Function : app_inverter_comm_checkForComperssorState 
 * Description: This function is used to check for compressor change state and error checking
 * @return Nothing
 */
static void app_inverter_comm_checkForComperssorState(void)
{
    /* Error Checking According to the Documentation Table */
    switch (gu16_receivedCompressorState)
    {
    case Normal_OPERATION_COMPRESSOR_OFF:
        // No Error
        break;
    case Normal_OPERATION_COMPRESSOR_ON:
        // No Error
        break;
    case NOT_DEFINED_SPEED_COMPRESSOR_OFF:
        // Error >> NOT_DEFINED_SPEED_COMPRESSOR_OFF
        break;
    case NOT_DEFINED_SPEED_COMPRESSOR_ON:
        // Error >> NOT_DEFINED_SPEED_COMPRESSOR_ON
        break;
    case Err_STARTUP_VALUE:
        // Error >> Err_STARTUP_VALUE
        break;
    case Err_VOLTAGE:
        // Error >> Err_VOLTAGE
        break;
    case Err_STARTUP_2:
        // Error >> Err_STARTUP_2
        break;
    case Err_IPM_PROTECTION:
        // Error >> Err_IPM_PROTECTION
        break;
    case Err_IPM_HighTemp:
        // Error >> Err_IPM_HighTemp
        break;
    case Err_IPM_CircuitErr:
        // Error >> Err_IPM_CircuitErr
        break;
    case Err_COM_DataErr:
        // Error >> Err_COM_DataErr
        break;
    case Err_COM_CrcErr:
        // Error >> Err_COM_CrcErr
        break;
    case Err_COM_CommandErr:
        // Error >> Err_COM_CommandErr
        break;
    default:
        break;
    }

    /* Error Checking Not Expected Status */
    if (g_compSpeedChange_flag == SPEED_CHANGE_2nd) // Expected >> new == old [NORMAL_ON]
    {
        if (gu16_receivedCompressorState != gu16_receivedOldCompressorState)
        {
            // Error
        }
        else
        {
            // Do Nothing
        }
    }
    else if (g_compSpeedChange_flag == ON_OFF_2ND) // Expected >> new != old [ON -> OFF || OFF -> ON]
    {
        if (gu16_receivedCompressorState == gu16_receivedOldCompressorState)
        {
            // Error
        }
        else
        {
            gu16_receivedOldCompressorState = gu16_receivedCompressorState;
        }
    }
    else
    {
        // Do Nothing
    }
}



/******************************************* Apis [Geters and Seters] *********************************/
/**
 * Function :  app_inverter_comm_setSpeed
 * Description: This function is to Set the compressor speed to be sent to the Inverter
 * @return nothing 
 */
void app_inverter_comm_setSpeed(uint16_t speed)
{
    gu16_userSetSpeed = speed;
}

/**
 * Function : app_inverter_comm_GetSetSpeed
 * Description: This function is to Get the compressor speed That was sent to the Inverter
 * @return The Setup compressor speed
 */
uint16_t app_inverter_comm_getSetSpeed(void)
{
    return gu16_receivedSetSpeed;
}

/**
 * Function : app_inverter_comm_GetActualSpeed
 * Description: This function is to Get the Actual compressor speed
 * @return The actual compressor speed
 */
uint16_t app_inverter_comm_getActualSpeed(void)
{
    return gu16_receivedActualSpeed;
}

/**
 * Function : app_inverter_comm_GetInputVoltage
 * Description: This function is to Get the Input Voltage to the inverter compressor
 * @return The Input Voltage
 */
uint16_t app_inverter_comm_getInputVoltage(void)
{
    return gu16_receivedInputVoltage;
}

/**
 * Function : app_inverter_comm_GetStatus
 * Description: This function is to Get the status of the compressor
 * @return The status of the compressor
 */
uint16_t app_inverter_comm_getStatus(void)
{
    return gu16_receivedCompressorState;
}

/**
 * Function : app_inverter_comm_VoltageDoubler
 * Description: This function is to Get the voltage doubler.
 * @return The voltage doubler.
 */
uint16_t app_inverter_comm_voltageDoubler(void)
{
    return gu16_receivedVoltageDoubler;
}

/**
 * Function : app_inverter_comm_IpmThrData
 * Description: This function is to Get the IPM thermistor data
 * @return The IPM thermistor data
 */
uint16_t app_inverter_comm_ipmThrData(void)
{
    return gu16_receivedIpmThrData;
}
/**
 * Function : app_inverter_comm_getVersionData
 * Description: This function is to Get the Version Data
 * @return The Version Data
 */
uint16_t app_inverter_comm_getVersionData(void)
{
    return gu16_receivedInverterVersion;
}