/*
 * DM556.c
 *
 *  Created on: Jun 16, 2024
 *      Author: Alvin
 */

#include "main.h"
#include "Datatype.h"
#include "Drv_DM556.h"

static double g_fMicroStepDividentVal = 0.0f;
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
inline void Execute_PulseCallback(Stepper *pStepper)
{
	if(FALSE == pStepper->bContinousRotationEnable &&
			pStepper->u32TotalStepCount <= pStepper->u32CurrStepCount)
	{
		Stop_StepperMotor(pStepper);
	}
	else
	{
		HAL_GPIO_TogglePin (pStepper->p_PulsePort, pStepper->u8PulsePin);
		pStepper->u32CurrStepCount++;
	}
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Config_StepperTimer(Stepper *pStepper , TIM_HandleTypeDef *pTimer ,
		void *pCallback , MOTOR_MICRO_STEP_SEL m_uStepSel , TMC_MOTOR_MODE m_MotorMode)
{
	uint32_t u32Prescalar = 0;
	uint32_t u32Timer_clk = 0U;
	uint8_t u8MS1 = (m_uStepSel & 0x01);
	uint8_t u8MS2 = ((m_uStepSel >> 1U) & 0x01);
	(void)u8MS1;/*Ignore to remove warnig*/
	(void)u8MS2;;/*Ignore to remove warnig*/
	pStepper->pTim = pTimer;
	pStepper->Callback_TimerComplete = pCallback;

    // Get APB1 or APB2 timer clock
    u32Timer_clk = HAL_RCC_GetPCLK1Freq();

	g_fMicroStepDividentVal = (float)m_uStepSel;

	u32Prescalar = (uint32_t)((u32Timer_clk / DEFAULT_STEPPER_TIMER_FREQ) - 1U);
	__HAL_TIM_SET_PRESCALER(pStepper->pTim , u32Prescalar);
	__HAL_TIM_SET_COUNTER(pStepper->pTim , 0U);
	Stop_StepperMotor(pStepper);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void SetDirection_Stepper(Stepper *pStepper , ROTATION_DIRECTION m_Dir)
{
	HAL_GPIO_WritePin (pStepper->p_DirPort, pStepper->u8DirPin , (GPIO_PinState)m_Dir);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void EnableStepper(Stepper *pStepper)
{
	HAL_GPIO_WritePin (pStepper->p_EnablePort, pStepper->u8EnablePin , (GPIO_PinState)GPIO_PIN_RESET);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void DisableStepper(Stepper *pStepper)
{
	HAL_GPIO_WritePin (pStepper->p_EnablePort, pStepper->u8EnablePin , (GPIO_PinState)GPIO_PIN_SET);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Set_RpmOfMotor(Stepper *pStepper , uint32_t u32Rpm)
{
	/*RPM=a/360∗f∗60*/
	uint32_t u32Arr = 0.0f;
	double u32Freq = 0.0f;
	double fStepperAngle = 0.0f;
	fStepperAngle = ((double)STEPPER_DEFAULT_ANGLE / (double)g_fMicroStepDividentVal);
	fStepperAngle /= 360.0f;/*Step per revelotion*/
	fStepperAngle *= 60.0f;/*Conversion to minute*/

	if(fStepperAngle)
	{
		u32Freq = ((double)u32Rpm / fStepperAngle);
	}


	if(u32Freq)
	{
		u32Arr = (uint32_t)(DEFAULT_STEPPER_TIMER_FREQ / u32Freq);
		u32Arr -= 1U;
	}

	DisableStepper(pStepper);
	HAL_TIM_Base_Stop(pStepper->pTim);
	__HAL_TIM_SET_AUTORELOAD(pStepper->pTim , (uint32_t)u32Arr);
	__HAL_TIM_SET_COUNTER(pStepper->pTim , 0);
	EnableStepper(pStepper);
	HAL_TIM_Base_Start_IT(pStepper->pTim);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Rotate_StepperSteps(Stepper *pStepper , uint32_t u32Steps ,  uint32_t u32Rpm)
{
	Set_RpmOfMotor(pStepper , u32Rpm);
	pStepper->u32CurrStepCount = 0;
	pStepper->u32TotalStepCount = u32Steps;
	pStepper->bContinousRotationEnable = 0U;
	Start_StepperMotor(pStepper);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Stop_StepperMotor(Stepper *pStepper)
{
	pStepper->u32CurrStepCount = 0;
	pStepper->u32TotalStepCount = 0;
	pStepper->bContinousRotationEnable = 0U;
	HAL_TIM_Base_Stop(pStepper->pTim);
	pStepper->m_State = STEPPER_MOTOR_IDLE;
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void StartContinous_StepperMotor(Stepper *pStepper , uint32_t u32Rpm)
{
	pStepper->u32CurrStepCount = 0;
	pStepper->u32TotalStepCount = 0;
	Set_RpmOfMotor(pStepper , u32Rpm);
	pStepper->bContinousRotationEnable = 1U;
	Start_StepperMotor(pStepper);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Start_StepperMotor(Stepper *pStepper)
{
	EnableStepper(pStepper);
	pStepper->m_State = STEPPER_MOTOR_RUNNING;
	HAL_TIM_Base_Start_IT(pStepper->pTim);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
MOTOR_STATE HI_GetMotorState(Stepper *pStepper)
{
	return (pStepper->m_State);
}
