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
#include "Timer.h"
#include "Drv_Switch.h"
#include "Drv_InternalEEPROM.h"
#include "Drv_DM556.h"
#include "App_StepperLinearGuide.h"
#include "Appl_PacketHandler.h"
#include "ApplicationLayer.h"

APPL_CONFIG g_ApplConfig;
APPL_CONFIG g_Appl_NvmData;

RUN_TIME_DATA g_ApplRunTimeData;

SWITCH 		g_SW_MotorX_StartStop;
SWITCH 		g_SW_MotorY_StartStop;

SWITCH 		g_SW_MotorX_DirCW;/*clock wise*/
SWITCH 		g_SW_MotorX_DirCCW;/*Counter clock wise*/

SWITCH 		g_SW_MotorY_DirCW;/*clock wise*/
SWITCH 		g_SW_MotorY_DirCCW;/*Counter clock wise*/

TimerTimeOut g_TimerNvmRunTimeData;

extern Stepper g_StepperMotorX;
extern Stepper g_StepperMotorY;
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
		g_Appl_NvmData.m_AppMotorX.m_MtrState = STEPPER_MOTOR_STOP;
		g_Appl_NvmData.m_AppMotorX.m_OperatingMode = SYS_OPERATING_MODE_AUTO;
		g_Appl_NvmData.m_AppMotorX.u32Freq = 10U;
		g_Appl_NvmData.m_AppMotorX.u1ApplEnabled = FALSE;
		g_Appl_NvmData.m_AppMotorX.u1HomePosEnabled = TRUE;
		g_Appl_NvmData.m_AppMotorX.u32NumOfSteps = 360U;

		g_Appl_NvmData.m_AppMotorY.m_Direction = Rotate_Clockwise;
		g_Appl_NvmData.m_AppMotorY.m_MicroStep = MOTOR_MS_STEP_1_64;
		g_Appl_NvmData.m_AppMotorY.m_MtrState = STEPPER_MOTOR_STOP;
		g_Appl_NvmData.m_AppMotorY.m_OperatingMode = SYS_OPERATING_MODE_AUTO;
		g_Appl_NvmData.m_AppMotorY.u32Freq = 10U;
		g_Appl_NvmData.m_AppMotorY.u1ApplEnabled = FALSE;
		g_Appl_NvmData.m_AppMotorY.u1HomePosEnabled = TRUE;
		g_Appl_NvmData.m_AppMotorY.u32NumOfSteps = 360U;
		Drv_InternalEEPROM_Write((uint16_t)EEPROM_START_ADDR ,
				(const uint8_t*)&(g_Appl_NvmData) , sizeof(g_Appl_NvmData));

		g_ApplRunTimeData.u32MotorX_CycleCount = 0U;
		g_ApplRunTimeData.u32MotorY_CycleCount = 0U;
		Drv_InternalEEPROM_Write((uint16_t)EEPROM_START_ADDR + 64U,
				(const uint8_t*)&(g_ApplRunTimeData) , sizeof(g_ApplRunTimeData));
	}
	else
	{
		Drv_InternalEEPROM_Read((uint16_t)EEPROM_START_ADDR + 64U ,
				(uint8_t*)&(g_ApplRunTimeData) , sizeof(g_ApplRunTimeData));
	}
	memcpy(&(g_ApplConfig) , &(g_Appl_NvmData) , sizeof(g_ApplConfig));/*Copy NVM data*/
	Appl_PacketHandler_Init();
	/*Initilize Stepper linear guide app layer*/
	App_StepperLinearGuide_Init();
	ConfigSwitch(&(g_SW_MotorX_StartStop) , SW4_INP_GPIO_Port , SW4_INP_Pin);
	ConfigSwitch(&(g_SW_MotorY_StartStop) , SW2_INP_GPIO_Port , SW2_INP_Pin);

	ConfigSwitch(&(g_SW_MotorX_DirCW) , SW6_INP_GPIO_Port , SW6_INP_Pin);
	ConfigSwitch(&(g_SW_MotorX_DirCCW) , SW5_INP_GPIO_Port , SW5_INP_Pin);

	ConfigSwitch(&(g_SW_MotorY_DirCW) , SW1_INP_GPIO_Port , SW1_INP_Pin);
	ConfigSwitch(&(g_SW_MotorY_DirCCW) , SW3_INP_GPIO_Port , SW3_INP_Pin);

	TimeOut_Start(&(g_TimerNvmRunTimeData) , NVM_RUN_TIME_DATA_SAVE_INTERVEL_MS);
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
	/*Motor X start stop switch handler*/
	if(SYS_OPERATING_MODE_AUTO == g_ApplConfig.m_AppMotorX.m_OperatingMode)
	{
		if(enSw_FallingEdge == GetState_Switch(&(g_SW_MotorX_StartStop)))
		{
			if(STEPPER_MOTOR_STOP == g_ApplConfig.m_AppMotorX.m_MtrState)
			{
				g_ApplConfig.m_AppMotorX.m_MtrState = STEPPER_MOTOR_START_ENTRY;
			}
			else if(STEPPER_MOTOR_RUNNING == g_ApplConfig.m_AppMotorX.m_MtrState)/*RUNNING*/
			{
				g_ApplConfig.m_AppMotorX.m_MtrState = STEPPER_MOTOR_STOP_ENTRY;
			}
		}
	}
	else/*Manual mode*/
	{
		g_ApplRunTimeData.u32MotorX_CycleCount = 0U;
		if(enSw_FallingEdge == GetState_Switch(&(g_SW_MotorX_StartStop)))
		{
			Stop_StepperMotor(&(g_StepperMotorX));
		}

		if(enSw_FallingEdge == GetState_Switch(&(g_SW_MotorX_DirCW)))
		{
			SetDirection_Stepper(&(g_StepperMotorX) , Rotate_Clockwise);
			Rotate_StepperSteps_Freq(&(g_StepperMotorX) ,
					g_ApplConfig.m_AppMotorX.u32NumOfSteps , g_ApplConfig.m_AppMotorX.u32Freq);
		}

		if(enSw_FallingEdge == GetState_Switch(&(g_SW_MotorX_DirCCW)))
		{
			SetDirection_Stepper(&(g_StepperMotorX) , Rotate_AntiClockwise);
			Rotate_StepperSteps_Freq(&(g_StepperMotorX) ,
					g_ApplConfig.m_AppMotorX.u32NumOfSteps , g_ApplConfig.m_AppMotorX.u32Freq);
		}
	}


	/*Motor Y start stop switch handler*/
	if(SYS_OPERATING_MODE_AUTO == g_ApplConfig.m_AppMotorY.m_OperatingMode)
	{
		if(enSw_FallingEdge == GetState_Switch(&(g_SW_MotorY_StartStop)))
		{
			if(STEPPER_MOTOR_STOP == g_ApplConfig.m_AppMotorY.m_MtrState)
			{
				g_ApplConfig.m_AppMotorY.m_MtrState = STEPPER_MOTOR_START_ENTRY;
			}
			else if(STEPPER_MOTOR_RUNNING == g_ApplConfig.m_AppMotorY.m_MtrState)/*RUNNING*/
			{
				g_ApplConfig.m_AppMotorY.m_MtrState = STEPPER_MOTOR_STOP_ENTRY;
			}
		}
	}
	else/*Manual mode*/
	{
		g_ApplRunTimeData.u32MotorY_CycleCount = 0U;
		if(enSw_FallingEdge == GetState_Switch(&(g_SW_MotorY_StartStop)))
		{
			Stop_StepperMotor(&(g_StepperMotorY));
		}

		if(enSw_FallingEdge == GetState_Switch(&(g_SW_MotorY_DirCW)))
		{
			SetDirection_Stepper(&(g_StepperMotorY) , Rotate_Clockwise);
			Rotate_StepperSteps_Freq(&(g_StepperMotorY) ,
					g_ApplConfig.m_AppMotorY.u32NumOfSteps , g_ApplConfig.m_AppMotorY.u32Freq);
		}

		if(enSw_FallingEdge == GetState_Switch(&(g_SW_MotorY_DirCCW)))
		{
			SetDirection_Stepper(&(g_StepperMotorY) , Rotate_AntiClockwise);
			Rotate_StepperSteps_Freq(&(g_StepperMotorY) ,
					g_ApplConfig.m_AppMotorY.u32NumOfSteps , g_ApplConfig.m_AppMotorY.u32Freq);
		}
	}
	Appl_PacketHandler_UpdateCycles(g_ApplRunTimeData.u32MotorX_CycleCount ,
												g_ApplRunTimeData.u32MotorY_CycleCount);
	if(TRUE == TimeOut_IsTimeout(&(g_TimerNvmRunTimeData)))
	{
		Drv_InternalEEPROM_Write((uint16_t)EEPROM_START_ADDR + 64U,
				(const uint8_t*)&(g_ApplRunTimeData) , sizeof(g_ApplRunTimeData));
		TimeOut_Restart(&(g_TimerNvmRunTimeData));
	}
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
RUN_TIME_DATA* GetInstance_ApplRunTimData(void)
{
	return (&(g_ApplRunTimeData));
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
	g_Appl_NvmData.m_AppMotorX.u32Freq = pConfig->motor1.freq;
	g_Appl_NvmData.m_AppMotorX.u1ApplEnabled = FALSE;
	g_Appl_NvmData.m_AppMotorX.u1HomePosEnabled = pConfig->motor1.home;
	g_Appl_NvmData.m_AppMotorX.u32NumOfSteps = pConfig->motor1.steps;
	g_Appl_NvmData.m_AppMotorX.m_MtrState = STEPPER_MOTOR_STOP;

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
	g_Appl_NvmData.m_AppMotorY.u32Freq = pConfig->motor2.freq;
	g_Appl_NvmData.m_AppMotorY.u1ApplEnabled = FALSE;
	g_Appl_NvmData.m_AppMotorY.u1HomePosEnabled = pConfig->motor2.home;
	g_Appl_NvmData.m_AppMotorY.u32NumOfSteps = pConfig->motor2.steps;
	g_Appl_NvmData.m_AppMotorY.m_MtrState = STEPPER_MOTOR_STOP;

	Drv_InternalEEPROM_Write((uint16_t)EEPROM_START_ADDR ,
			(const uint8_t*)&(g_Appl_NvmData) , sizeof(g_Appl_NvmData));
	memcpy(&(g_ApplConfig) , &(g_Appl_NvmData) , sizeof(g_ApplConfig));/*Copy NVM data*/

	g_ApplRunTimeData.u32MotorX_CycleCount = 0U;
	g_ApplRunTimeData.u32MotorY_CycleCount = 0U;
	Drv_InternalEEPROM_Write((uint16_t)EEPROM_START_ADDR + 64U,
			(const uint8_t*)&(g_ApplRunTimeData) , sizeof(g_ApplRunTimeData));
}
/*END OF FILE*/
