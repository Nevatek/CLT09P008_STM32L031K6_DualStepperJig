/*********************************.FILE_HEADER.*******************************
<Copyright Notice>
.File : Drv_Uart.h
.Summary : Drv_Uart header file
.Note :

*****************************************************************************/
#ifndef DRV_UART_H_
#define DRV_UART_H_

#define MAX_RX_SIZE	32
#define MAX_TX_SIZE	16


uint16_t uart_GetRecievedSize(void);

void uart_Init();
void uart_Receive(uint8_t *data, uint16_t size);
void uart_Transmit(const uint8_t *pData, uint16_t Size);

void uart_TransmitCallback(UART_HandleTypeDef *Handle);
void uart_ReceiveCallback(UART_HandleTypeDef *Handle, uint16_t size);


#endif /* DRV_UART_H_ */
