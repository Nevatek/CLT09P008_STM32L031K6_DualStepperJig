/*********************************.FILE_HEADER.*******************************
<Copyright Notice>
.File : Application.c
.Summary : Application layer main header file
.Note :

Author Date Description
------------------------------------------------------------------------------
<Author name> <DDMMMYYYY> <changes made>*/
/*Aldrin.Rebellow 29/03/2026 Initial Release*/
#include "main.h"
#include "string.h"
#include "Datatype.h"
#include "Drv_InternalEEPROM.h"
#include "Drv_DM556.h"
#include "App_StepperLinearGuide.h"
#include "Appl_PacketHandler.h"
#include "ApplicationLayer.h"

APPL_CONFIG g_ApplConfig;
APPL_CONFIG g_Appl_NvmData;
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ApplicationLayer_Init(void)
{
	Drv_InternalEEPROM_Init();
	/*Read NVM*/
	Drv_InternalEEPROM_Read((uint16_t)EEPROM_START_ADDR ,
			(uint8_t*)&(g_Appl_NvmData) , sizeof(g_Appl_NvmData));
	if(FLASH_INIT_FLAG != g_Appl_NvmData.u8FlashInitFlag)/*Init data to buffers*/
	{
		g_Appl_NvmData.u8FlashInitFlag = FLASH_INIT_FLAG;
		g_Appl_NvmData.m_SystemMotorOperatingMode = SYS_MOTOR_INDEPENDENT;

		g_Appl_NvmData.m_AppMotorX.m_Direction = Rotate_Clockwise;
		g_Appl_NvmData.m_AppMotorX.m_MicroStep = MOTOR_MS_STEP_1_64;
		g_Appl_NvmData.m_AppMotorX.m_MtrState = STEPPER_MOTOR_IDLE;
		g_Appl_NvmData.m_AppMotorX.m_OperatingMode = SYS_OPERATING_MODE_AUTO;
		g_Appl_NvmData.m_AppMotorX.u16Rpm = 10U;
		g_Appl_NvmData.m_AppMotorX.u1ApplEnabled = FALSE;
		g_Appl_NvmData.m_AppMotorX.u1HomePosEnabled = TRUE;
		g_Appl_NvmData.m_AppMotorX.u32NumOfSteps = 360U;

		g_Appl_NvmData.m_AppMotorY.m_Direction = Rotate_Clockwise;
		g_Appl_NvmData.m_AppMotorY.m_MicroStep = MOTOR_MS_STEP_1_64;
		g_Appl_NvmData.m_AppMotorY.m_MtrState = STEPPER_MOTOR_IDLE;
		g_Appl_NvmData.m_AppMotorY.m_OperatingMode = SYS_OPERATING_MODE_AUTO;
		g_Appl_NvmData.m_AppMotorY.u16Rpm = 10U;
		g_Appl_NvmData.m_AppMotorY.u1ApplEnabled = FALSE;
		g_Appl_NvmData.m_AppMotorY.u1HomePosEnabled = TRUE;
		g_Appl_NvmData.m_AppMotorY.u32NumOfSteps = 360U;
		Drv_InternalEEPROM_Write((uint16_t)EEPROM_START_ADDR ,
				(const uint8_t*)&(g_Appl_NvmData) , sizeof(g_Appl_NvmData));
	}
	memcpy(&(g_ApplConfig) , &(g_Appl_NvmData) , sizeof(g_ApplConfig));/*Copy NVM data*/
	Appl_PacketHandler_Init();
	/*Initilize Stepper linear guide app layer*/
	App_StepperLinearGuide_Init();
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ApplicationLayer_Exe(void)
{
	Appl_PacketHandler_Exe();
	/*Execute Stepper linear guide app layer*/
	App_StepperLinearGuide_Exe();

	/*Switch*/
	/*Auto mode*/
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
APPL_CONFIG* GetInstance_ApplConfig(void)
{
	return (&(g_ApplConfig));
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Callback_Appl_ConfigUpdated(SystemCofig_t *pConfig)
{
	/*Turn OFF all motors*/
	App_Disable_AllMotors();

	/*Save params to NVM*/
	g_Appl_NvmData.u8FlashInitFlag = FLASH_INIT_FLAG;
	g_Appl_NvmData.m_SystemMotorOperatingMode = (SYS_MOTOR_MODE)pConfig->sync;
	/*MOTOR X*/
	g_Appl_NvmData.m_AppMotorX.m_Direction = (ROTATION_DIRECTION)pConfig->motor1.dir;
	switch(pConfig->motor1.ustep)
	{
		default:
		case (0U):
		{
			g_Appl_NvmData.m_AppMotorX.m_MicroStep = MOTOR_MS_FULL_STEP;
		}break;
		case (1U):
		{
			g_Appl_NvmData.m_AppMotorX.m_MicroStep = MOTOR_MS_HALF_STEP;
		}break;
		case (2U):
		{
			g_Appl_NvmData.m_AppMotorX.m_MicroStep = MOTOR_MS_STEP_1_4;
		}break;
		case (3U):
		{
			g_Appl_NvmData.m_AppMotorX.m_MicroStep = MOTOR_MS_STEP_1_8;
		}break;
		case (4U):
		{
			g_Appl_NvmData.m_AppMotorX.m_MicroStep = MOTOR_MS_STEP_1_16;
		}break;
		case (5U):
		{
			g_Appl_NvmData.m_AppMotorX.m_MicroStep = MOTOR_MS_STEP_1_32;
		}break;
		case (6U):
		{
			g_Appl_NvmData.m_AppMotorX.m_MicroStep = MOTOR_MS_STEP_1_64;
		}break;
		case (7U):
		{
			g_Appl_NvmData.m_AppMotorX.m_MicroStep = MOTOR_MS_STEP_1_128;
		}break;
		case (8U):
		{
			g_Appl_NvmData.m_AppMotorX.m_MicroStep = MOTOR_MS_STEP_1_256;
		}break;
	}
	g_Appl_NvmData.m_AppMotorX.m_OperatingMode = (SYS_OPERATING_MODE)pConfig->motor1.mode;
	g_Appl_NvmData.m_AppMotorX.u16Rpm = pConfig->motor1.freq;
	g_Appl_NvmData.m_AppMotorX.u1ApplEnabled = FALSE;
	g_Appl_NvmData.m_AppMotorX.u1HomePosEnabled = pConfig->motor1.home;
	g_Appl_NvmData.m_AppMotorX.u32NumOfSteps = pConfig->motor1.steps;
	g_Appl_NvmData.m_AppMotorX.m_MtrState = STEPPER_MOTOR_IDLE;

	/*MOTOR Y*/
	g_Appl_NvmData.m_AppMotorY.m_Direction = (ROTATION_DIRECTION)pConfig->motor2.dir;
	switch(pConfig->motor2.ustep)
	{
		default:
		case (0U):
		{
			g_Appl_NvmData.m_AppMotorY.m_MicroStep = MOTOR_MS_FULL_STEP;
		}break;
		case (1U):
		{
			g_Appl_NvmData.m_AppMotorY.m_MicroStep = MOTOR_MS_HALF_STEP;
		}break;
		case (2U):
		{
			g_Appl_NvmData.m_AppMotorY.m_MicroStep = MOTOR_MS_STEP_1_4;
		}break;
		case (3U):
		{
			g_Appl_NvmData.m_AppMotorY.m_MicroStep = MOTOR_MS_STEP_1_8;
		}break;
		case (4U):
		{
			g_Appl_NvmData.m_AppMotorY.m_MicroStep = MOTOR_MS_STEP_1_16;
		}break;
		case (5U):
		{
			g_Appl_NvmData.m_AppMotorY.m_MicroStep = MOTOR_MS_STEP_1_32;
		}break;
		case (6U):
		{
			g_Appl_NvmData.m_AppMotorY.m_MicroStep = MOTOR_MS_STEP_1_64;
		}break;
		case (7U):
		{
			g_Appl_NvmData.m_AppMotorY.m_MicroStep = MOTOR_MS_STEP_1_128;
		}break;
		case (8U):
		{
			g_Appl_NvmData.m_AppMotorY.m_MicroStep = MOTOR_MS_STEP_1_256;
		}break;
	}
	g_Appl_NvmData.m_AppMotorY.m_OperatingMode = (SYS_OPERATING_MODE)pConfig->motor2.mode;
	g_Appl_NvmData.m_AppMotorY.u16Rpm = pConfig->motor2.freq;
	g_Appl_NvmData.m_AppMotorY.u1ApplEnabled = FALSE;
	g_Appl_NvmData.m_AppMotorY.u1HomePosEnabled = pConfig->motor2.home;
	g_Appl_NvmData.m_AppMotorY.u32NumOfSteps = pConfig->motor2.steps;
	g_Appl_NvmData.m_AppMotorY.m_MtrState = STEPPER_MOTOR_IDLE;

	Drv_InternalEEPROM_Write((uint16_t)EEPROM_START_ADDR ,
			(const uint8_t*)&(g_Appl_NvmData) , sizeof(g_Appl_NvmData));
	memcpy(&(g_ApplConfig) , &(g_Appl_NvmData) , sizeof(g_ApplConfig));/*Copy NVM data*/
}
/*END OF FILE*/
