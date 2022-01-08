/******************************************************************************/
/* @File            : InverterCom.h                                    			  */
/*                                                                            */
/* @Designed by     : Ammar Shahin                         @Date : 29-6-2020   */
/*                                                                            */
/* @Designed by     : Ammar Shahin                         @Date : 29-6-2020   */
/*                                                                            */
/* @Coding language : C                                                       */
/*                                                                            */
/* @COPYRIGHT 2020 El-ARABY Research and development center.                  */
/* all rights reserved                                                        */
/******************************************************************************/
#ifndef _INVERTERCOM_H__
#define _INVERTERCOM_H__

/************************************************************************/
/*                       Files Included                                 */
/************************************************************************/

/************************************************************************/
/*                  Public Macros & configurations                      */
/************************************************************************/
#define TxComplete() sw_uart_TxComplete()
#define RxComplete() sw_uart_RxComplete()
#define TxFlagClr() sw_uart_TxFlagClr()
#define RxFlagClr() sw_uart_RxFlagClr()
#define Send(data) sw_uart_send(data)
#define Read() sw_uart_read()

#define Normal_OPERATION_COMPRESSOR_OFF 0x0000
#define Normal_OPERATION_COMPRESSOR_ON 0x0001
#define NOT_DEFINED_SPEED_COMPRESSOR_OFF 0x0080
#define NOT_DEFINED_SPEED_COMPRESSOR_ON 0x0081
#define Err_STARTUP_VALUE 0xF002  // Current value error at compressor start-up (also when not connected)
#define Err_VOLTAGE 0xF004        // Compressor speed drop error or voltage error (overvoltage, under voltage)
#define Err_STARTUP_2 0xF008      // Compressor start-up error 2
#define Err_IPM_PROTECTION 0xF010 // IPM protection function is executed
#define Err_IPM_HighTemp 0xF020   // IPM thermistor high temperature
#define Err_IPM_CircuitErr 0xF040 // Inverter circuit error (IMP thermistor error, op-amp error)

/* Communication Errors */
#define Err_COM_DataErr 0xF100    // Data error
#define Err_COM_CrcErr 0xF200     // Checksum Error
#define Err_COM_CommandErr 0xF400 // Command Error
/************************************************************************/
/*                        Public Enum Typedefs                          */
/************************************************************************/
typedef enum
{
    NORMAL_OPERATION_OFF = 0,
    NORMAL_OPERATION_ON,
    ERROR_OPERATION
} tEnum_CompressorStatus; // Error ??? Needs Review

/************************************************************************/
/*                           APIs PROTOTYPES                            */
/************************************************************************/
/**
 * Function :  app_inverter_comm_init
 * Description: This function is to initialize the Inverter Communiation Module
 * @return nothing 
 */
void app_inverter_comm_init(void);

/**
 * Function : app_inverter_comm_Update
 * Description: This function is to update the Communiation Module
 * @return nothing 
 */
void app_inverter_comm_update(void *args);

/**
 * Function :  app_inverter_comm_setSpeed
 * Description: This function is to Set the compressor speed to be sent to the Inverter
 * @return nothing 
 */
void app_inverter_comm_setSpeed(uint16_t);

/**
 * Function : app_inverter_comm_getSetSpeed
 * Description: This function is to Get the compressor speed That was sent to the Inverter
 * @return The Setup compressor speed
 */
uint16_t app_inverter_comm_getSetSpeed(void);

/**
 * Function : app_inverter_comm_getActualSpeed
 * Description: This function is to Get the Actual compressor speed
 * @return The actual compressor speed
 */
uint16_t app_inverter_comm_getActualSpeed(void);

/**
 * Function : app_inverter_comm_getInputVoltage
 * Description: This function is to Get the Input Voltage to the inverter compressor
 * @return The Input Voltage
 */
uint16_t app_inverter_comm_getInputVoltage(void);

/**
 * Function : app_inverter_comm_getStatus
 * Description: This function is to Get the status of the compressor
 * @return The status of the compressor
 */
uint16_t app_inverter_comm_getStatus(void);

/**
 * Function : app_inverter_comm_voltageDoubler
 * Description: This function is to Get the voltage doubler.
 * @return The voltage doubler.
 */
uint16_t app_inverter_comm_voltageDoubler(void);

/**
 * Function : app_inverter_comm_ipmThrData
 * Description: This function is to Get the IPM thermistor data
 * @return The IPM thermistor data
 */
uint16_t app_inverter_comm_ipmThrData(void);

/**
 * Function : app_inverter_comm_getVersionData
 * Description: This function is to Get the Version Data
 * @return The Version Data
 */
uint16_t app_inverter_comm_getVersionData(void);

#endif /* _INVERTERCOM_H__ */