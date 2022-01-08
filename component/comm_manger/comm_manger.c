/*
 * Comm_manger.c
 * Desc : Communication Manger
 * Created: 29-7-2020 8:27:02 AM
 *  Author: Ammar Shahin
 */
/************************************************************************/
/*				              Files Includes                            */
/************************************************************************/
#include "std_types.h"
#include "utils.h"
#include "sw_uart.h"
#include "comm_manger.h"

/************************************************************************/
/*                               LOCAL MACROS                           */
/************************************************************************/

#define NULL ((void *)0)

#define debug 0

#if debug
#include "uart.h"
#endif

/************************************************************************/
/*                         LOCAL Structures                            */
/************************************************************************/
typedef struct
{
    uint32_t timeOut_MS;
    uint32_t timeOutTimer;
    uint8_t *pReqBuffer;
    Enum_commProtocol_t CommProtocol;
    Enum_commMangerReqStatus_t ReqStatus;
    Enum_commMangerReqType_t ReqType;
    uint16_t ReqBufferSize;
    uint16_t timeInterval_MS;
    uint16_t intervalTimer;
    uint16_t ReqIteratorIndex;
    uint8_t ReqID;
    uint8_t ReqLocalBuffer[MAX_BUFFER_SIZE];
} Str_ComMangerLocalStr_t;

/************************************************************************/
/*							Local Enums                                 */
/************************************************************************/
typedef enum
{
    PROTOCOL_IDLE,     // This CommProtocol is IDLE
    PROTOCOL_OPERATING // This CommProtocol is OPERATING
} Enum_ComProtocolStatus_t;

/************************************************************************/
/*                       GLOBAL STATIC VARIABLES                        */
/************************************************************************/
static Str_ComMangerLocalStr_t gArr_RequestsArrary[MAX_REQUESTS_NUMBER];

static Enum_ComProtocolStatus_t gEnum_CommProtocolStatus_SWUART;
//Enum_ComProtocolStatus_t gEnum_CommProtocolStatus_UART;
//Enum_ComProtocolStatus_t gEnum_CommProtocolStatus_SPI;
//Enum_ComProtocolStatus_t gEnum_CommProtocolStatus_I2C;

/************************************************************************/
/*			            Private[Static] functions                       */
/************************************************************************/
static void comm_manger_Send(Enum_commProtocol_t CommProtocol, uint8_t data);
static uint8_t comm_manger_read(Enum_commProtocol_t CommProtocol);

static void comm_manger_txDispatcher(uint8_t req_id);
static void comm_manger_rxDispatcher(uint8_t req_id);


static Enum_ComProtocolStatus_t comm_manger_commProtocolGetStatus(Enum_commProtocol_t CommProtocol);
static void comm_manger_commProtocolSetStatus(Enum_commProtocol_t CommProtocol, Enum_ComProtocolStatus_t status);

static bool_t comm_manger_commProtocolTxFlagGet(Enum_commProtocol_t CommProtocol);
static bool_t comm_manger_commProtocolRxFlagGet(Enum_commProtocol_t CommProtocol);

static void comm_manger_commProtocolTxFlagClr(Enum_commProtocol_t CommProtocol);
static void comm_manger_commProtocolRxFlagClr(Enum_commProtocol_t CommProtocol);

/************************************************************************/
/*                           APIs IMPLEMENTATION                        */
/************************************************************************/

/**
 * Function : comm_manger_init
 * Description: this function is to initialize the comm manger
 * @return nothing 
 **/
void comm_manger_init(void)
{
    uint8_t localReqIterator, localBuffIterator;
    /* Initialize The Requests Array */
    for (localReqIterator = 0; localReqIterator < MAX_REQUESTS_NUMBER; localReqIterator++)
    {
        gArr_RequestsArrary[localReqIterator].CommProtocol = None;
        gArr_RequestsArrary[localReqIterator].ReqID = 0;
        gArr_RequestsArrary[localReqIterator].ReqType = NOT_INITALIZED;
        gArr_RequestsArrary[localReqIterator].pReqBuffer = NULL;
        gArr_RequestsArrary[localReqIterator].ReqBufferSize = 0;
        for (localBuffIterator = 0; localBuffIterator < MAX_BUFFER_SIZE; localBuffIterator++)
        {
            gArr_RequestsArrary[localReqIterator].ReqLocalBuffer[localBuffIterator] = 0;
        }
        gArr_RequestsArrary[localReqIterator].ReqIteratorIndex = 0;
        gArr_RequestsArrary[localReqIterator].ReqStatus = REQ_NOT_INITALIZED;
        gArr_RequestsArrary[localReqIterator].timeInterval_MS = 0;
        gArr_RequestsArrary[localReqIterator].timeOut_MS = 0;
        gArr_RequestsArrary[localReqIterator].intervalTimer = 0;
        gArr_RequestsArrary[localReqIterator].timeOutTimer = 0;
    }

    /* Initialize the Communcation protocols used in the Comm Manger */
    //sw_uart_init();
    //mcal_uart_init();
    //mcal_spi_init();

    /* Set the Status(Key) of each Communication Protocols to be Idle(available) */
    gEnum_CommProtocolStatus_SWUART = PROTOCOL_IDLE;
    //gEnum_CommProtocolStatus_Uart = PROTOCOL_IDLE;
    //gEnum_CommProtocolStatus_SPI = PROTOCOL_IDLE;
}

/**
 * Function : comm_manger_reqSetup
 * Description: This Function is used to setup the Req
 * @param Req : Configurations Struct that has all the data required to initialize the Request
 * @param ReqID : A unique Id to the request 
 * @return : Status of the Function according to the Error handling
 **/
Enm_commMangerError_t comm_manger_reqSetup(Str_commMangerGlobalStr_t Req, uint8_t *ReqID)
{
    Enm_commMangerError_t api_status = OK_t;
    uint8_t localIterator = 0, tempVar = 0;
    for (localIterator = 0; localIterator < MAX_REQUESTS_NUMBER; localIterator++)
    {
        if (gArr_RequestsArrary[localIterator].ReqStatus == REQ_NOT_INITALIZED)
        {
            tempVar = localIterator;
            break;
        }
    }

    if (localIterator == MAX_REQUESTS_NUMBER)
    {
        api_status = MAX_NUMBER_OF_REQUESTS;
    }
    else if ((Req.CommProtocol != UART) && (Req.CommProtocol != SW_UART) &&
             (Req.CommProtocol != SPI) && (Req.CommProtocol != I2C))
    {
        api_status = INVALID_COMProtocol_TYPE;
    }
    else if (Req.pReqBuffer == NULL)
    {
        api_status = NULLPTR;
    }
    else if (Req.ReqBufferSize == 0)
    {
        api_status = INVALID_DATA_LENGTH;
    }
    else if ((Req.ReqType != REQ_SENDING) && (Req.ReqType != REQ_RECEIVING))
    {
        api_status = INVALID_REQUEST_TYPE;
    }
    else
    {
        gArr_RequestsArrary[tempVar].CommProtocol = Req.CommProtocol;
        gArr_RequestsArrary[tempVar].ReqType = Req.ReqType;
        gArr_RequestsArrary[tempVar].pReqBuffer = Req.pReqBuffer;
        gArr_RequestsArrary[tempVar].ReqBufferSize = Req.ReqBufferSize;

        gArr_RequestsArrary[tempVar].ReqID = tempVar;

        for (localIterator = 0; localIterator < MAX_BUFFER_SIZE; localIterator++)
        {
            gArr_RequestsArrary[tempVar].ReqLocalBuffer[localIterator] = Req.pReqBuffer[localIterator];
        }

        gArr_RequestsArrary[tempVar].ReqIteratorIndex = 0;
        gArr_RequestsArrary[tempVar].ReqStatus = REQ_IDLE;

        gArr_RequestsArrary[tempVar].timeInterval_MS = Req.timeInterval_MS;
        gArr_RequestsArrary[tempVar].timeOut_MS = Req.timeOut_MS;
        gArr_RequestsArrary[tempVar].intervalTimer = 0;
        gArr_RequestsArrary[tempVar].timeOutTimer = 0;
    }
    return api_status;
}

/**
 * Function : comm_manger_update
 * Description: this function is to update the comm mangerusually in the os schedule
 * @return nothing 
 **/
void comm_manger_update(void *args)
{
    uint8_t localIteratorReq = 0;
    for (localIteratorReq = 0; localIteratorReq < MAX_REQUESTS_NUMBER; localIteratorReq++)
    {
        if (gArr_RequestsArrary[localIteratorReq].ReqStatus == REQ_NOT_INITALIZED)
        {
            continue;
        }
        else if (gArr_RequestsArrary[localIteratorReq].ReqStatus == REQ_IDLE)
        {
            if (comm_manger_commProtocolGetStatus(gArr_RequestsArrary[localIteratorReq].CommProtocol) == PROTOCOL_IDLE)
            {
                comm_manger_commProtocolSetStatus(gArr_RequestsArrary[localIteratorReq].CommProtocol, PROTOCOL_OPERATING);
                gArr_RequestsArrary[localIteratorReq].ReqStatus = REQ_OPERATING;
            }
            else
            {
                // Do Nothing
            }

            /* Timeout check */
            if (++gArr_RequestsArrary[localIteratorReq].timeOutTimer >= gArr_RequestsArrary[localIteratorReq].timeOut_MS)
            {
                gArr_RequestsArrary[localIteratorReq].ReqStatus = REQ_OPERATION_TIMEOUT;
                comm_manger_commProtocolSetStatus(gArr_RequestsArrary[localIteratorReq].CommProtocol, PROTOCOL_IDLE);
            }
        }
        else if (gArr_RequestsArrary[localIteratorReq].ReqStatus == REQ_OPERATING)
        {
            if (gArr_RequestsArrary[localIteratorReq].ReqType == REQ_SENDING)
            {
                comm_manger_txDispatcher(localIteratorReq);
            }
            else if (gArr_RequestsArrary[localIteratorReq].ReqType == REQ_RECEIVING)
            {
                comm_manger_rxDispatcher(localIteratorReq);
            }
            else
            {
                // Do Nothing
            }

            /* Timeout check */
            if (++gArr_RequestsArrary[localIteratorReq].timeOutTimer >= gArr_RequestsArrary[localIteratorReq].timeOut_MS)
            {
                gArr_RequestsArrary[localIteratorReq].ReqStatus = REQ_OPERATION_TIMEOUT;
                comm_manger_commProtocolSetStatus(gArr_RequestsArrary[localIteratorReq].CommProtocol, PROTOCOL_IDLE);
            }
        }
        else
        {
            // Do Nothing
        }
    }

    /* call all the protocols update functions */
    //sw_uart_update();
    //mcal_uart_update();
}

/**
 * Function : comm_manger_reqUninstall
 * Description: This Function is used to deinit the buffer from the Requests Stack
 * @return Void
 **/
void comm_manger_reqUninstall(uint8_t req_id)
{
    uint8_t localIterator2 = 0;
    gArr_RequestsArrary[req_id].CommProtocol = None;
    gArr_RequestsArrary[req_id].ReqID = 0;
    gArr_RequestsArrary[req_id].ReqType = NOT_INITALIZED;
    gArr_RequestsArrary[req_id].pReqBuffer = NULL;
    gArr_RequestsArrary[req_id].ReqBufferSize = 0;
    for (localIterator2 = 0; localIterator2 < MAX_BUFFER_SIZE; localIterator2++)
    {
        gArr_RequestsArrary[req_id].ReqLocalBuffer[localIterator2] = 0;
    }
    gArr_RequestsArrary[req_id].ReqIteratorIndex = 0;
    gArr_RequestsArrary[req_id].ReqStatus = REQ_NOT_INITALIZED;
    gArr_RequestsArrary[req_id].intervalTimer = 0;
    gArr_RequestsArrary[req_id].timeOutTimer = 0;
    gArr_RequestsArrary[req_id].timeInterval_MS = 0;
    gArr_RequestsArrary[req_id].timeOut_MS = 0;
}

/**
 * Function : comm_manger_reqGetStatus
 * Description: this function is used to get the status of the comm manger
 * @param ReqID : The ID Of the Request.
 * @return the status of the Reqest. 
 **/
Enum_commMangerReqStatus_t comm_manger_reqGetStatus(uint8_t ReqID)
{
    return gArr_RequestsArrary[ReqID].ReqStatus;
}

/************************************************************************/
/*             Local(Static) functions IMPLEMENTATION                   */
/************************************************************************/
/**
 * Function : comm_manger_txDispatcher
 * Description: This Function is used to handle the Sending in the run time
 * @return Void
 **/
static void comm_manger_txDispatcher(uint8_t req_id)
{
    if (comm_manger_commProtocolTxFlagGet(gArr_RequestsArrary[req_id].CommProtocol) == TRUE)
    {
        if (gArr_RequestsArrary[req_id].intervalTimer >= gArr_RequestsArrary[req_id].timeInterval_MS)
        {
            gArr_RequestsArrary[req_id].intervalTimer = 0;
            comm_manger_commProtocolTxFlagClr(gArr_RequestsArrary[req_id].CommProtocol);
            comm_manger_Send(gArr_RequestsArrary[req_id].CommProtocol, gArr_RequestsArrary[req_id].ReqLocalBuffer[gArr_RequestsArrary[req_id].ReqIteratorIndex++]);
            if (gArr_RequestsArrary[req_id].ReqIteratorIndex == gArr_RequestsArrary[req_id].ReqBufferSize)
            {
                gArr_RequestsArrary[req_id].ReqStatus = REQ_OPERATION_FINISHED;
                comm_manger_commProtocolSetStatus(gArr_RequestsArrary[req_id].CommProtocol, PROTOCOL_IDLE);
                //comm_manger_reqUninstall(req_id); // Needs Further Discution >> Will The Comm manger DeInit the buffer
            }
            else
            {
                // Do Nothing
            }
        }
        else
        {
            gArr_RequestsArrary[req_id].intervalTimer++;
        }
    }
    else
    {
        // Do Nothing
    }
}

/**
 * Function : comm_manger_rxDispatcher
 * Description: This Function is used to handle the Receiving in the run time
 * @return Void
 **/
static void comm_manger_rxDispatcher(uint8_t req_id)
{
#if debug
    uint8_t buff[10] = {0};
#endif
    uint8_t localIterator = 0;
    if (comm_manger_commProtocolRxFlagGet(gArr_RequestsArrary[req_id].CommProtocol) == TRUE)
    {
        comm_manger_commProtocolRxFlagClr(gArr_RequestsArrary[req_id].CommProtocol);
        gArr_RequestsArrary[req_id].ReqLocalBuffer[gArr_RequestsArrary[req_id].ReqIteratorIndex++] = comm_manger_read(gArr_RequestsArrary[req_id].CommProtocol);
        if (gArr_RequestsArrary[req_id].ReqIteratorIndex == gArr_RequestsArrary[req_id].ReqBufferSize) // The Buffer requested is completely received
        {
#if debug
            uart_data_put(UART_1, "\n\r");
            uart_data_put(UART_1, "*****************Data received*******************");
#endif
            /* Now copy the local buffer to the remote one */
            for (localIterator = 0; localIterator < gArr_RequestsArrary[req_id].ReqBufferSize; localIterator++)
            {
                gArr_RequestsArrary[req_id].pReqBuffer[localIterator] = gArr_RequestsArrary[req_id].ReqLocalBuffer[localIterator];
#if debug
                utils_uitoa((uint32_t)(~gArr_RequestsArrary[req_id].ReqLocalBuffer[localIterator]), buff);
                uart_data_put(UART_1, "\n\r");
                uart_data_put(UART_1, "Data received = ");
                uart_data_put(UART_1, buff);
#endif
            }
#if debug
            uart_data_put(UART_1, "\n\r");
            uart_data_put(UART_1, "*****************Data received*******************");
#endif
            gArr_RequestsArrary[req_id].ReqStatus = REQ_OPERATION_FINISHED;
            comm_manger_commProtocolSetStatus(gArr_RequestsArrary[req_id].CommProtocol, PROTOCOL_IDLE);
            //comm_manger_reqUninstall(req_id); // Needs Further Discution >> Will The Comm manger DeInit the buffer
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

/*
 * Function : comm_manger_Send
 * Description : This Function is used to Send data through a specified protocol
 * @param CommProtocol : a specified protocol
 * @param data : data to be sent
 * @ return : Nothing
*/
static void comm_manger_Send(Enum_commProtocol_t CommProtocol, uint8_t data)
{
    switch (CommProtocol)
    {
    case SW_UART:
        sw_uart_send(data);
        break;
    case UART:
        //uart_data_put(UART_1,data);
        break;
    default:
        break;
    }
}

/**
 * Function : comm_manger_read
 * Description: This Function is used to Read data from a specified protocol
 * @param CommProtocol : a specified protocol
 * @return : the data received
 **/
static uint8_t comm_manger_read(Enum_commProtocol_t CommProtocol)
{
    uint8_t tempVar = 0;
    switch (CommProtocol)
    {
    case SW_UART:
        tempVar = sw_uart_read();
        break;
    case UART:
        //mcal_uart_read();
        break;
    default:
        break;
    }
    return tempVar;
}

/**
 * Function : comm_manger_commProtocolGetStatus
 * Description: This Function is used to Get the status of a specified protocol
 * @param CommProtocol : a specified protocol
 * @return : the status[IDLE or OPERATING]
 **/
static Enum_ComProtocolStatus_t comm_manger_commProtocolGetStatus(Enum_commProtocol_t CommProtocol)
{
    Enum_ComProtocolStatus_t tempVar = PROTOCOL_OPERATING;
    switch (CommProtocol)
    {
    case SW_UART:
        tempVar = gEnum_CommProtocolStatus_SWUART;
        break;
    case UART:
        //tempVar = gEnum_CommProtocolStatus_Uart;
        break;
    default:
        break;
    }
    return tempVar;
}

/**
 * Function : comm_manger_commProtocolSetStatus
 * Description: This Function is used to Set the status of a specified protocol
 * @param CommProtocol : a specified protocol
 * @param status : statsu
 * @return Nothing
 **/
static void comm_manger_commProtocolSetStatus(Enum_commProtocol_t CommProtocol, Enum_ComProtocolStatus_t status)
{
    switch (CommProtocol)
    {
    case SW_UART:
        gEnum_CommProtocolStatus_SWUART = status;
        break;
    case UART:
        //gEnum_CommProtocolStatus_UART = status;
        break;
    default:
        break;
    }
}

/**
 * Function : comm_manger_commProtocolTxFlagGet
 * Description: This Function is used to get the status of the TxComplete Flag of a specified protocol
 * @param CommProtocol : a specified protocol
 * @return the status of the TxComplete Flag [TRUE or FALSE]
 **/
static bool_t comm_manger_commProtocolTxFlagGet(Enum_commProtocol_t CommProtocol)
{
    bool_t tempVar = FALSE;
    switch (CommProtocol)
    {
    case SW_UART:
        tempVar = sw_uart_TxComplete();
        break;
    case UART:
        //tempVar = mcal_uart_TxComplete();
        break;
    default:
        break;
    }
    return tempVar;
}

/**
 * Function : comm_manger_commProtocolTxFlagGet
 * Description: This Function is used to get the status of the RxComplete Flag of a specified protocol
 * @param CommProtocol : a specified protocol
 * @return the status of the TxComplete Flag [TRUE or FALSE]
 **/
static bool_t comm_manger_commProtocolRxFlagGet(Enum_commProtocol_t CommProtocol)
{
    bool_t tempVar = FALSE;
    switch (CommProtocol)
    {
    case SW_UART:
        tempVar = sw_uart_RxComplete();
        break;
    case UART:
        //tempVar = mcal_uart_RxComplete();
        break;
    default:
        break;
    }
    return tempVar;
}

/**
 * Function : comm_manger_commProtocolSetStatus
 * Description: This Function is used to Set the status of a specified protocol
 * @param CommProtocol : a specified protocol
 * @return Nothing
 **/
static void comm_manger_commProtocolTxFlagClr(Enum_commProtocol_t CommProtocol)
{
    switch (CommProtocol)
    {
    case SW_UART:
        sw_uart_TxFlagClr();
        break;
    case UART:
        //mcal_uart_TxFlagClr();
        break;
    default:
        break;
    }
}

/**
 * Function : comm_manger_commProtocolSetStatus
 * Description: This Function is used to Set the status of a specified protocol
 * @param CommProtocol : a specified protocol
 * @return Nothing
 **/
static void comm_manger_commProtocolRxFlagClr(Enum_commProtocol_t CommProtocol)
{
    switch (CommProtocol)
    {
    case SW_UART:
        sw_uart_RxFlagClr();
        break;
    case UART:
        //mcal_uart_RxFlagClr();
        break;
    default:
        break;
    }
}