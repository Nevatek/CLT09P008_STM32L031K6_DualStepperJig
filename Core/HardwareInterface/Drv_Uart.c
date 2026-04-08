/*********************************.FILE_HEADER.*******************************
<Copyright Notice>
.File : Drv_Uart.c
.Summary : Drv_Uart layer file
.Note :

Author Date Description
------------------------------------------------------------------------------
<Author name> <DDMMMYYYY> <changes made>*/
/*Muhamed.Afil 08/04/2026 Initial Release*/
#include "main.h"
#include <string.h>
#include "Drv_Uart.h"

uint8_t uart_rxBuffer[MAX_RX_SIZE] = {0};
uint8_t uart_txBuffer[MAX_TX_SIZE] = {0};

uint16_t uart_rxBufferSize;
uint16_t uart_txBufferSize;

UART_HandleTypeDef *uart_Handle;

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as uart initialization
.Returns :
.Note : use this function for all major initialization
******************************************************************************/
void uart_Init()
{
	uart_Handle = GetInstance_UART2();
	uint8_t fake_data;
	uart_Receive(&fake_data, 0);
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as uart Receive
.Returns :
.Note : the data will be copied to the passed buffer
******************************************************************************/
void uart_Receive(uint8_t* data, uint16_t size)
{
	memcpy(data, uart_rxBuffer, size);

	uart_rxBufferSize = 0;
	memset(uart_rxBuffer, 0, MAX_RX_SIZE);

	HAL_UARTEx_ReceiveToIdle_IT(uart_Handle, uart_rxBuffer, MAX_RX_SIZE);
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as uart Transmit
.Returns :
.Note : the data will be copied from the passed buffer
******************************************************************************/
void uart_Transmit(const uint8_t *pData, uint16_t Size)
{
	uart_txBufferSize = Size;
	memcpy(uart_txBuffer , pData, uart_txBufferSize);

	HAL_UART_Transmit_IT(uart_Handle, uart_txBuffer, uart_txBufferSize);
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as uart check
.Returns : The size of data received
.Note :
******************************************************************************/
uint16_t uart_GetRecievedSize(void)
{
	return uart_rxBufferSize;
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void inline uart_ReceiveCallback(UART_HandleTypeDef *Handle, uint16_t size)
{
	if((Handle == NULL) ||
			(Handle != uart_Handle) ||
			(0U == size))
	{
		uint8_t fake_data;
		uart_Receive(&fake_data, 0);
	}
	else
	{
		uart_rxBufferSize = size;
	}
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void uart_TransmitCallback(UART_HandleTypeDef *Handle)
{
	if((Handle == NULL) ||
			(Handle != uart_Handle))
	{
		uart_txBufferSize = 0;
		memset(uart_txBuffer, 0, MAX_TX_SIZE);
	}
}
