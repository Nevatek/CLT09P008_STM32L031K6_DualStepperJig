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
#include "Datatype.h"
#include "Drv_DM556.h"
#include "App_StepperLinearGuide.h"

Stepper g_StepperMotorX;
Stepper g_StepperMotorY;
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
inline void Callback_StepperTimer_MotX(void)
{
	g_StepperMotorX.Callback_TimerComplete((Stepper *)&(g_StepperMotorX));
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
inline void Callback_StepperTimer_MotY(void)
{
	g_StepperMotorY.Callback_TimerComplete((Stepper *)&(g_StepperMotorY));
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
inline void CallBack_HomeSensDetectMotorX(void)
{
	if(TRUE == g_StepperMotorX.bHomeSensEnable)
	{
		Stop_StepperMotor(&(g_StepperMotorX));
	}
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
inline void CallBack_HomeSensDetectMotorY(void)
{
	if(TRUE == g_StepperMotorY.bHomeSensEnable)
	{
		Stop_StepperMotor(&(g_StepperMotorY));
	}
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void App_StepperLinearGuide_Init(void)
{
	/*Config Stepper 1*/
	g_StepperMotorX.p_DirPort = MOT1_DIR__GPIO_Port;
	g_StepperMotorX.p_EnablePort = MOT1_DISABLE__GPIO_Port;
	g_StepperMotorX.p_PulsePort = MOT1_PUL__GPIO_Port;
	g_StepperMotorX.u8DirPin = MOT1_DIR__Pin;
	g_StepperMotorX.u8EnablePin = MOT1_DISABLE__Pin;
	g_StepperMotorX.u8PulsePin = MOT1_PUL__Pin;
	g_StepperMotorX.bHomeSensEnable = TRUE;/*Enable Home sensor by DEFAULT*/
	Config_StepperTimer(&(g_StepperMotorX) ,
			GetInstance_Timer2() , &Execute_PulseCallback ,
			MOTOR_MS_STEP_1_64 , TMC_STEALTH_CHOP);

	/*Config Stepper 2*/
	g_StepperMotorY.p_DirPort = MOT2_DIR__GPIO_Port;
	g_StepperMotorY.p_EnablePort = MOT2_DISABLE__GPIO_Port;
	g_StepperMotorY.p_PulsePort = MOT2_PUL__GPIO_Port;
	g_StepperMotorY.u8DirPin = MOT2_DIR__Pin;
	g_StepperMotorY.u8EnablePin = MOT2_DISABLE__Pin;
	g_StepperMotorY.u8PulsePin = MOT2_PUL__Pin;
	g_StepperMotorY.bHomeSensEnable = TRUE;/*Enable Home sensor by DEFAULT*/
	Config_StepperTimer(&(g_StepperMotorY) ,
			GetInstance_Timer21() , &Execute_PulseCallback ,
			MOTOR_MS_STEP_1_64 , TMC_STEALTH_CHOP);

	DisableStepper(&(g_StepperMotorX));
	DisableStepper(&(g_StepperMotorY));

	Rotate_StepperSteps(&(g_StepperMotorY) , 15210 , 30);
//	StartContinous_StepperMotor(&(g_StepperMotorY) , 230);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void App_StepperLinearGuide_Exe(void)
{
	APPL_CONFIG *pApplCfg = GetInstance_ApplConfig();
	/*
	 * MOTOR X - START*
	 */
	if(TRUE == pApplCfg->m_AppMotorX.u1ApplEnabled)
	{
		/*If motor is enabled and running*/
	}
}

