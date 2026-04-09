/*********************************.FILE_HEADER.*******************************
<Copyright Notice>
.File : Appl_PacketHandler.c
.Summary : Appl_PacketHandler layer file
.Note :

Author Date Description
------------------------------------------------------------------------------
<Author name> <DDMMMYYYY> <changes made>*/
/*Muhamed.Afil 08/04/2026 Initial Release*/

/***********************includes *********************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "Drv_Uart.h"
#include "Datatype.h"
#include "Appl_PacketHandler.h"

/*********************** globals *********************************************/
static SystemCofig_t g_Config;

PacketHandlerState_t PacketHandler_state;

uint8_t g_PacketBuffer[PACKET_LEN]= {0};
uint8_t g_BufferSize = 0;
static uint32_t g_CountSentTime = 0;

int motor1_Count = 0, motor2_Count = 0;
extern void Callback_Appl_ConfigUpdated(SystemCofig_t *pConfig);
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
static uint16_t Packet_crc16(uint8_t *data, uint16_t len)
{
    uint16_t crc = 0xFFFF;

    for (uint16_t i = 0; i < len; i++)
    {
        crc ^= (data[i] << 8);

        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void PacketHandler_Send(uint8_t cmd, uint8_t *data, uint8_t len)
{
    uint8_t buf[300];
    uint16_t idx = 0;

    buf[idx++] = PACKET_SOP;
    buf[idx++] = cmd;
    buf[idx++] = len;

    for (uint8_t i = 0; i < len; i++)
        buf[idx++] = data[i];

    uint16_t crc = Packet_crc16(&buf[1], 2 + len); // CMD+LEN+DATA

    buf[idx++] = (crc >> 8) & 0xFF;
    buf[idx++] = crc & 0xFF;

    uart_Transmit(buf, idx);
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
bool PacketHandler_Decode(uint8_t *data, uint8_t len)
{
    if (PAYLOAD_LEN != g_PacketBuffer[2])
        return false;

    // M1
    g_Config.motor1.steps =data[0] | (data[1] << 8);
    g_Config.motor1.dir   =data[2];
    g_Config.motor1.home  =data[3];
    g_Config.motor1.mode  =data[4];
    g_Config.motor1.ustep =data[5];
    g_Config.motor1.freq  =data[6] | (data[7] << 8);

    // M2
    g_Config.motor2.steps =data[8]  | (data[9] << 8);
    g_Config.motor2.dir   =data[10];
    g_Config.motor2.home  =data[11];
    g_Config.motor2.mode  =data[12];
    g_Config.motor2.ustep =data[13];
    g_Config.motor2.freq  =data[14] | (data[15] << 8);

    // Sync
    g_Config.sync =data[16];

    return true;
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
static void send_ack(void)
{
	PacketHandler_Send(PACKET_CMD_ACK, 0, 0);
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
static void send_error(uint8_t err)
{
	PacketHandler_Send(PACKET_CMD_ERR, &err, 1);
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
static void send_cycles(uint32_t m1, uint32_t m2)
{
    uint8_t d[8];

    d[0] = (m1 >> 0);
    d[1] = (m1 >> 8);
    d[2] = (m1 >> 16);
    d[3] = (m1 >> 24);

    d[4] = (m2 >> 0);
    d[5] = (m2 >> 8);
    d[6] = (m2 >> 16);
    d[7] = (m2 >> 24);

    PacketHandler_Send(PACKET_CMD_CYC, d, 8);
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Appl_PacketHandler_UpdateCycles(uint32_t m1, uint32_t m2)
{
	motor1_Count = m1;
	motor2_Count = m2;
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
static uint8_t validate_cfg(SystemCofig_t *cfg)
{
    if (cfg->motor1.steps == 0 || cfg->motor2.steps == 0)
        return PACKET_ERR_INVALID_STEPS;

    if (cfg->motor1.freq == 0 || cfg->motor2.freq == 0)
        return PACKET_ERR_INVALID_FREQ;

    return 0;
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void PacketHandler_Process(uint8_t *data)
{
	    switch (data[1])
	    {
	        case PACKET_CMD_CFG:
	        {
	            if (!PacketHandler_Decode(&(data[3]), g_BufferSize))
	            {
	                send_error(PACKET_ERR_GENERIC);
	                return;
	            }

	            uint8_t err = validate_cfg(&g_Config);
	            if (err)
	            {
	                send_error(err);
	                return;
	            }

	            send_ack();

				g_CountSentTime = HAL_GetTick();

	            break;
	        }

	        default:
	            send_error(PACKET_ERR_GENERIC);
	            break;
	    }
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Appl_PacketHandler_Init(void)
{
	uart_Init();

	uart_Receive(g_PacketBuffer, PACKET_LEN);

	PacketHandler_state	= HANDLER_IDLE;
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Appl_PacketHandler_Exe(void)
{
	if((HAL_GetTick() - g_CountSentTime) >= TICK_1_SEC)
	{
		g_CountSentTime = HAL_GetTick();
		send_cycles(motor1_Count, motor2_Count);
	}

	switch (PacketHandler_state) {
	case HANDLER_IDLE:
	{
		g_BufferSize = uart_GetRecievedSize();
		if(g_BufferSize)
		{
			PacketHandler_state = HANDLER_COPY_DATA;
		}
		break;
	}

	case HANDLER_COPY_DATA:
	{
		uart_Receive(g_PacketBuffer, g_BufferSize);

		if(g_PacketBuffer[0] == PACKET_SOP)
		{
			PacketHandler_state = HANDLER_PROCESS_DATA;
		}
		else
		{
			PacketHandler_state = HANDLER_IDLE;
		}

		break;
	}

	case HANDLER_PROCESS_DATA:
	{

		PacketHandler_Process(g_PacketBuffer);
		Callback_Appl_ConfigUpdated(&g_Config);

		PacketHandler_state = HANDLER_IDLE;

		break;
	}

	case HANDLER_ERROR:
	{
		PacketHandler_state = HANDLER_IDLE;
		break;
	}
	}
}
