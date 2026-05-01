/*
 * ApplicationLayer.h
 *
 *  Created on: 29-Mar-2026
 *      Author: aldrin.rebellow
 */

#ifndef APPLICATION_APPLICATIONLAYER_H_
#define APPLICATION_APPLICATIONLAYER_H_

#define FLASH_INIT_FLAG 	(0xAA)
#define FLASH_DEFAULT_FLAG 	(0xFF)

#define NVM_RUN_TIME_DATA_SAVE_INTERVEL_MS (5000U)

typedef enum
{
	SYS_OPERATING_MODE_AUTO = 0U,
	SYS_OPERATING_MODE_MANUAL,
}SYS_OPERATING_MODE;

typedef enum
{
	SYS_MOTOR_INDEPENDENT = 0U,
	SYS_MOTOR_SYNC,
}SYS_MOTOR_MODE;

typedef struct
{
	uint32_t u32NumOfSteps;
	uint32_t u1HomePosEnabled : 1U;
	uint32_t u1ApplEnabled : 1U;/*Motor on/off*/
	uint32_t u16StrokeDelayMs : 16U;/*Delay between each stroke in MS*/
	uint32_t : 14U;
	uint32_t u32Rpm;
	SYS_OPERATING_MODE m_OperatingMode;
	MOTOR_MICRO_STEP_SEL m_MicroStep;
	MOTOR_STEP_ANGLE_SEL m_MtrAngleSel;
	ROTATION_DIRECTION m_Direction;
	MOTOR_STATE m_MtrState;
}MOTOR_APP_CONFIG;

typedef struct
{
	uint8_t u8FlashInitFlag;
	SYS_MOTOR_MODE m_SystemMotorOperatingMode;
	MOTOR_APP_CONFIG m_AppMotorX;
	MOTOR_APP_CONFIG m_AppMotorY;
}APPL_CONFIG;

typedef struct
{
	uint32_t u32MotorX_CycleCount;
	uint32_t u32MotorY_CycleCount;
}RUN_TIME_DATA;

void ApplicationLayer_Init(void);
void ApplicationLayer_Exe(void);
APPL_CONFIG* GetInstance_ApplConfig(void);
RUN_TIME_DATA* GetInstance_ApplRunTimData(void);
void Callback_Appl_ConfigUpdated(SystemCofig_t *pConfig);
#endif /* APPLICATION_APPLICATIONLAYER_H_ */
