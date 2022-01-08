/*
 * Comm_manger.h
 * Desc : Communication Manger
 * Created: 29-7-2020 8:27:02 AM
 *  Author: Ammar Shahin
 */

#ifndef _COMM_MANGER_H_
#define _COMM_MANGER_H_

/************************************************************************/
/*				              Files Includes                            */
/************************************************************************/

/************************************************************************/
/*                               CONSTANTS/Macros                               */
/************************************************************************/
#define MAX_REQUESTS_NUMBER 2
#define MAX_BUFFER_SIZE 5

/************************************************************************/
/*                          PRIMITIVE TYPES                             */
/************************************************************************/

/************************************************************************/
/*                               ENUMS                                  */
/************************************************************************/
/* error Enum */
typedef enum
{
    OK_t = 1,
    INVALID_ARGUMENT,
    NULLPTR,
    INVALID_COMProtocol_TYPE,
    INVALID_DATA_LENGTH,
    INVALID_REQUEST_TYPE,
    MAX_NUMBER_OF_REQUESTS
} Enm_commMangerError_t;

typedef enum
{
    None = 1,
    UART,
    SPI,
    I2C,
    SW_UART,
} Enum_commProtocol_t;

typedef enum
{
    NOT_INITALIZED = 1, // This request is not initialized yet
    REQ_SENDING,
    REQ_RECEIVING
} Enum_commMangerReqType_t;

typedef enum
{
    REQ_NOT_INITALIZED,    // This request is not initialized yet
    REQ_IDLE,              // This request is not executed yet
    REQ_OPERATING,         // This request is being executed
    REQ_OPERATION_TIMEOUT, // This request was never executed in the deadline
    REQ_OPERATION_FINISHED // This request is finished executing
} Enum_commMangerReqStatus_t;

/************************************************************************/
/*                         Public Structures                            */
/************************************************************************/
typedef struct
{
    uint32_t timeOut_MS;
    uint8_t *pReqBuffer;
    Enum_commProtocol_t CommProtocol;
    Enum_commMangerReqType_t ReqType;
    uint16_t ReqBufferSize;
    uint16_t timeInterval_MS;
} Str_commMangerGlobalStr_t;
/************************************************************************/
/*                       FUNCTION PROTOTYPES                            */
/************************************************************************/
/**
 * Function : comm_manger_init
 * Description: this function is to initialize the comm manger
 * @return Status: of the initialization according to the Error handling macros 
 **/
void comm_manger_init(void);

/**
 * Function : comm_manger_reqSetup
 * Description: This Function is used to setup the Req
 * @param Req : Configrition Struct that has all the data required to initialize the Request
 * @param ReqID : A unique Id to the request 
 * @return : Status of the Function according to the Error handling
 **/
Enm_commMangerError_t comm_manger_reqSetup(Str_commMangerGlobalStr_t Req, uint8_t *ReqID);

/**
 * Function : comm_manger_reqGetStatus
 * Description: this function is used to get the status of the comm manger
 * @param ReqID : The ID Of the Request.
 * @return the status of the Reqest. 
 **/
Enum_commMangerReqStatus_t comm_manger_reqGetStatus(uint8_t ReqID);

/**
 * Function : comm_manger_update
 * Description: this function is to update the comm manger usually in the os schedule
 * @return nothing 
 **/
void comm_manger_update(void *args);

/**
 * Function : comm_manger_reqUninstall
 * Description: This Function is used to deinit the buffer from the Requests Stack
 * @return Void
 **/
void comm_manger_reqUninstall(uint8_t job_id);

#endif /* _COMM_MANGER_H_ */