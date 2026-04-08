/*********************************.FILE_HEADER.*******************************
<Copyright Notice>
.File : Appl_PacketHandler.h
.Summary : Appl_PacketHandler header file
.Note :

*****************************************************************************/

#ifndef APPL_PACKET_HANDLER_H_
#define APPL_PACKET_HANDLER_H_

/*********************** defines *********************************************/

#define PACKET_SOP 					0xAA

#define PACKET_CMD_CFG 				0x01
#define PACKET_CMD_ACK 				0x02
#define PACKET_CMD_ERR 				0x03
#define PACKET_CMD_CYC 				0x04

#define PACKET_ERR_INVALID_STEPS  	0x01
#define PACKET_ERR_INVALID_FREQ   	0x02
#define PACKET_ERR_CRC            	0x03
#define PACKET_ERR_GENERIC        	0xFF

#define TICK_1_SEC 1000U

#define FIXED_LEN	5U
#define PAYLOAD_LEN 17U
#define PACKET_LEN	(PAYLOAD_LEN + FIXED_LEN)

#define PACKET_MAX_LEN	128

/*********************** typedef *********************************************/

typedef enum {
	HANDLER_IDLE,
	HANDLER_COPY_DATA,
	HANDLER_PROCESS_DATA,
	HANDLER_ERROR
}PacketHandlerState_t;

typedef struct
{
    uint16_t steps;
    uint8_t dir;
    uint8_t home;
    uint8_t mode;
    uint8_t ustep;
    uint16_t freq;
} MotorConfig_t;

typedef struct
{
	MotorConfig_t motor1;
	MotorConfig_t motor2;
    uint8_t sync;
} SystemCofig_t;

void Appl_PacketHandler_Init(void);
void Appl_PacketHandler_Exe(void);

#endif /* APPL_PACKET_HANDLER_H_ */
