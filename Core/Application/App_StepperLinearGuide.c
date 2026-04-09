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
#include "Appl_PacketHandler.h"
#include "App_StepperLinearGuide.h"
#include "ApplicationLayer.h"

Stepper g_StepperMotorX;
Stepper g_StepperMotorY;
MTR_APP_PROCESS g_MtrAppProcess_X;
MTR_APP_PROCESS g_MtrAppProcess_Y;
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

//	Rotate_StepperSteps(&(g_StepperMotorY) , 15210 , 30);
//	StartContinous_StepperMotor(&(g_Stepp0erMotorY) , 230);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void App_Disable_AllMotors(void)
{
	Stop_StepperMotor(&(g_StepperMotorX));
	Stop_StepperMotor(&(g_StepperMotorY));
	DisableStepper(&(g_StepperMotorX));
	DisableStepper(&(g_StepperMotorY));
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
	switch(pApplCfg->m_AppMotorX.m_MtrState)
	{
		case (STEPPER_MOTOR_START_ENTRY):
		{
			if(SYS_MOTOR_SYNC == pApplCfg->m_SystemMotorOperatingMode)
			{
				g_MtrAppProcess_X = MTR_APP_MOVE_TO_HOME;/*Motor X move to home first*/
				g_MtrAppProcess_Y = MTR_APP_IDLE;
				pApplCfg->m_AppMotorX.m_MtrState = STEPPER_MOTOR_RUNNING;
				pApplCfg->m_AppMotorY.m_MtrState = STEPPER_MOTOR_RUNNING;
			}
			else/*INDEPENDED MODE*/
			{
				g_MtrAppProcess_X = MTR_APP_MOVE_TO_HOME;
				pApplCfg->m_AppMotorX.m_MtrState = STEPPER_MOTOR_RUNNING;
			}

		}break;

		case (STEPPER_MOTOR_STOP_ENTRY):
		{
			if(SYS_MOTOR_SYNC == pApplCfg->m_SystemMotorOperatingMode)
			{
				g_MtrAppProcess_X = MTR_APP_IDLE;
				Stop_StepperMotor(&(g_StepperMotorX));
				g_MtrAppProcess_Y = MTR_APP_IDLE;
				Stop_StepperMotor(&(g_StepperMotorY));
				pApplCfg->m_AppMotorX.m_MtrState = STEPPER_MOTOR_STOP;
				pApplCfg->m_AppMotorY.m_MtrState = STEPPER_MOTOR_STOP;
			}
			else/*INDEPENDED MODE*/
			{
				g_MtrAppProcess_X = MTR_APP_IDLE;
				Stop_StepperMotor(&(g_StepperMotorX));
				pApplCfg->m_AppMotorX.m_MtrState = STEPPER_MOTOR_STOP;

			}
		}break;

		case (STEPPER_MOTOR_STOP):
		{
			/*NOP*/
		}break;

		case (STEPPER_MOTOR_RUNNING):
		{
			switch(g_MtrAppProcess_X)
			{
				case (MTR_APP_IDLE):
				{
					/*NOP*/
				}break;
				case (MTR_APP_MOVE_TO_HOME):
				{
					SetDirection_Stepper(&(g_StepperMotorX) , pApplCfg->m_AppMotorX.m_Direction);
					if(TRUE == pApplCfg->m_AppMotorX.u1HomePosEnabled)
					{
						Rotate_StepperSteps_Freq(&(g_StepperMotorX) ,
								pApplCfg->m_AppMotorX.u32NumOfSteps + DEFAULT_HOME_POS_OFFSET_STEPS ,
								pApplCfg->m_AppMotorX.u32Freq);
					}
					else/*If home positon is not enabled*/
					{
						Rotate_StepperSteps_Freq(&(g_StepperMotorX) ,
								pApplCfg->m_AppMotorX.u32NumOfSteps ,
								pApplCfg->m_AppMotorX.u32Freq);
					}
					g_MtrAppProcess_X = MTR_APP_MOVE_TO_HOME_WAIT;
				}break;
				case (MTR_APP_MOVE_TO_HOME_WAIT):
				{
					if(STEPPER_MOTOR_STOP == HI_GetMotorState((&(g_StepperMotorX))))
					{
						if(SYS_MOTOR_SYNC == pApplCfg->m_SystemMotorOperatingMode)
						{
							g_MtrAppProcess_Y = MTR_APP_MOVE_TO_HOME;/*Motor Y move to home*/
							g_MtrAppProcess_X = MTR_APP_IDLE;
						}
						else/*INDEPENDEND*/
						{
							/*When motor is stopped*/
							g_MtrAppProcess_X = MTR_APP_MOVE_TO_STEP;
						}
					}
				}break;
				case (MTR_APP_MOVE_TO_STEP):
				{
					SetDirection_Stepper(&(g_StepperMotorX) , !pApplCfg->m_AppMotorX.m_Direction);
					Rotate_StepperSteps_Freq(&(g_StepperMotorX) ,
							pApplCfg->m_AppMotorX.u32NumOfSteps ,
							pApplCfg->m_AppMotorX.u32Freq);
					g_MtrAppProcess_X = MTR_APP_MOVE_TO_STEP_WAIT;
				}break;
				case (MTR_APP_MOVE_TO_STEP_WAIT):
				{
					if(STEPPER_MOTOR_STOP == HI_GetMotorState((&(g_StepperMotorX))))
					{
						if(0xFFFFFFFF > GetInstance_ApplRunTimData()->u32MotorX_CycleCount)
						{
							GetInstance_ApplRunTimData()->u32MotorX_CycleCount++;
						}
						if(SYS_MOTOR_SYNC == pApplCfg->m_SystemMotorOperatingMode)
						{
							g_MtrAppProcess_Y = MTR_APP_MOVE_TO_STEP;/*Motor Y move to step*/
							g_MtrAppProcess_X = MTR_APP_IDLE;
						}
						else/*INDEPENDEND*/
						{
							/*When motor is stopped*/
							g_MtrAppProcess_X = MTR_APP_MOVE_TO_HOME;
						}
					}
				}break;
				default:
					break;
			}
		}break;
		default:
			break;
	}
	/*
	 * MOTOR X - END*
	 */


	/*
	 * MOTOR Y - START*
	 */
	switch(pApplCfg->m_AppMotorY.m_MtrState)
	{
		case (STEPPER_MOTOR_START_ENTRY):
		{
			if(SYS_MOTOR_SYNC == pApplCfg->m_SystemMotorOperatingMode)
			{
				g_MtrAppProcess_X = MTR_APP_MOVE_TO_HOME;/*Motor X move to home first*/
				g_MtrAppProcess_Y = MTR_APP_IDLE;
				pApplCfg->m_AppMotorX.m_MtrState = STEPPER_MOTOR_RUNNING;
				pApplCfg->m_AppMotorY.m_MtrState = STEPPER_MOTOR_RUNNING;
			}
			else/*INDEPENDED MODE*/
			{
				g_MtrAppProcess_Y = MTR_APP_MOVE_TO_HOME;
				pApplCfg->m_AppMotorY.m_MtrState = STEPPER_MOTOR_RUNNING;
			}
		}break;

		case (STEPPER_MOTOR_STOP_ENTRY):
		{
			if(SYS_MOTOR_SYNC == pApplCfg->m_SystemMotorOperatingMode)
			{
				g_MtrAppProcess_Y = MTR_APP_IDLE;
				Stop_StepperMotor(&(g_StepperMotorY));
				g_MtrAppProcess_X = MTR_APP_IDLE;
				Stop_StepperMotor(&(g_StepperMotorX));
				pApplCfg->m_AppMotorX.m_MtrState = STEPPER_MOTOR_STOP;
				pApplCfg->m_AppMotorY.m_MtrState = STEPPER_MOTOR_STOP;
			}
			else/*INDEPENDED MODE*/
			{
				g_MtrAppProcess_Y = MTR_APP_IDLE;
				Stop_StepperMotor(&(g_StepperMotorY));
				pApplCfg->m_AppMotorY.m_MtrState = STEPPER_MOTOR_STOP;
			}
		}break;

		case (STEPPER_MOTOR_STOP):
		{
			/*NOP*/
		}break;

		case (STEPPER_MOTOR_RUNNING):
		{
			switch(g_MtrAppProcess_Y)
			{
				case (MTR_APP_IDLE):
				{
					/*NOP*/
				}break;
				case (MTR_APP_MOVE_TO_HOME):
				{
					SetDirection_Stepper(&(g_StepperMotorY) , pApplCfg->m_AppMotorY.m_Direction);
					if(TRUE == pApplCfg->m_AppMotorY.u1HomePosEnabled)
					{
						Rotate_StepperSteps_Freq(&(g_StepperMotorY) ,
								pApplCfg->m_AppMotorY.u32NumOfSteps + DEFAULT_HOME_POS_OFFSET_STEPS ,
								pApplCfg->m_AppMotorY.u32Freq);
					}
					else/*If home positon is not enabled*/
					{
						Rotate_StepperSteps_Freq(&(g_StepperMotorY) ,
								pApplCfg->m_AppMotorY.u32NumOfSteps ,
								pApplCfg->m_AppMotorY.u32Freq);
					}
					g_MtrAppProcess_Y = MTR_APP_MOVE_TO_HOME_WAIT;
				}break;
				case (MTR_APP_MOVE_TO_HOME_WAIT):
				{
					if(STEPPER_MOTOR_STOP == HI_GetMotorState((&(g_StepperMotorY))))
					{
						if(SYS_MOTOR_SYNC == pApplCfg->m_SystemMotorOperatingMode)
						{
							g_MtrAppProcess_X = MTR_APP_MOVE_TO_STEP;/*Motor Y move to home*/
							g_MtrAppProcess_Y = MTR_APP_IDLE;
						}
						else/*INDEPENDEND*/
						{
							/*When motor is stopped*/
							g_MtrAppProcess_Y = MTR_APP_MOVE_TO_STEP;
						}
					}
				}break;
				case (MTR_APP_MOVE_TO_STEP):
				{
					SetDirection_Stepper(&(g_StepperMotorY) , !pApplCfg->m_AppMotorY.m_Direction);
					Rotate_StepperSteps_Freq(&(g_StepperMotorY) ,
							pApplCfg->m_AppMotorY.u32NumOfSteps ,
							pApplCfg->m_AppMotorY.u32Freq);
					g_MtrAppProcess_Y = MTR_APP_MOVE_TO_STEP_WAIT;
				}break;
				case (MTR_APP_MOVE_TO_STEP_WAIT):
				{
					if(STEPPER_MOTOR_STOP == HI_GetMotorState((&(g_StepperMotorY))))
					{
						if(0xFFFFFFFF > GetInstance_ApplRunTimData()->u32MotorY_CycleCount)
						{
							GetInstance_ApplRunTimData()->u32MotorY_CycleCount++;
						}
						if(SYS_MOTOR_SYNC == pApplCfg->m_SystemMotorOperatingMode)
						{
							g_MtrAppProcess_X = MTR_APP_MOVE_TO_HOME;/*Motor Y move to step*/
							g_MtrAppProcess_Y = MTR_APP_IDLE;
						}
						else/*INDEPENDEND*/
						{
							/*When motor is stopped*/
							g_MtrAppProcess_Y = MTR_APP_MOVE_TO_HOME;
						}
					}
				}break;
				default:
					break;
			}
		}break;
		default:
			break;
	}
	/*
	 * MOTOR Y - END*
	 */
}

