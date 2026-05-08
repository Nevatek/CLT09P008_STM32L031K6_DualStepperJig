/*
 * DM556.c
 *
 *  Created on: Jun 16, 2024
 *      Author: Alvin
 */

#include "main.h"
#include "Datatype.h"
#include "Drv_DM556.h"

static float g_MotorStepAngleTable[MOTOR_STEP_ANG_MAX] ={	/*MOTOR_STEP_ANG_0_9*/0.9,
															/*MOTOR_STEP_ANG_1_8*/1.8,
															/*MOTOR_STEP_ANG_2_5*/2.5,
															/*MOTOR_STEP_ANG_2_7*/2.7,
															/*MOTOR_STEP_ANG_3_6*/3.6,
															/*MOTOR_STEP_ANG_7_2*/7.2,
															/*MOTOR_STEP_ANG_7_5*/7.5
														};
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
inline void Execute_PulseCallback_MotorX(Stepper *pStepper)
{
    HAL_GPIO_WritePin(pStepper->p_PulsePort, pStepper->u8PulsePin , pStepper->bPulsePinState);
    pStepper->bPulsePinState ^= 1U;/*Toggle BIT*/

    /* Only increment step count on rising edge (when pin goes HIGH) */
    if(pStepper->bPulsePinState)
    {
        pStepper->u32CurrStepCount++;

        /* Check if target steps reached */
        if(pStepper->u32CurrStepCount >= pStepper->u32TotalStepCount &&
        		!pStepper->bContinousRotationEnable)
        {
            Stop_StepperMotor(pStepper);
        }
    }
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
inline void Execute_PulseCallback_MotorY(Stepper *pStepper)
{
    HAL_GPIO_WritePin(pStepper->p_PulsePort, pStepper->u8PulsePin , pStepper->bPulsePinState);
    pStepper->bPulsePinState ^= 1U;/*Toggle BIT*/

    /* Only increment step count on rising edge */
    if(pStepper->bPulsePinState)
    {
        pStepper->u32CurrStepCount++;

        if(pStepper->u32CurrStepCount >= pStepper->u32TotalStepCount &&
        		!pStepper->bContinousRotationEnable)
        {
            Stop_StepperMotor(pStepper);
        }
    }
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Config_StepperTimer(Stepper *pStepper , TIM_HandleTypeDef *pTimer ,
		void *pCallback , MOTOR_STEP_ANGLE_SEL m_MtrAngleSel , MOTOR_MICRO_STEP_SEL m_uStepSel ,
		TMC_MOTOR_MODE m_MotorMode)
{
	uint32_t u32Prescalar = 0;
	uint32_t u32Timer_clk = 0U;
	uint8_t u8MS1 = (m_uStepSel & 0x01);
	uint8_t u8MS2 = ((m_uStepSel >> 1U) & 0x01);
	(void)u8MS1;/*Ignore to remove warnig*/
	(void)u8MS2;;/*Ignore to remove warnig*/

	if(MOTOR_STEP_ANG_0_9 <= m_MtrAngleSel && MOTOR_STEP_ANG_MAX > m_MtrAngleSel)
	{
		pStepper->fStepAngle = g_MotorStepAngleTable[m_MtrAngleSel];
	}
	else
	{
		pStepper->fStepAngle = g_MotorStepAngleTable[MOTOR_STEP_ANG_0_9];
	}

	pStepper->pTim = pTimer;
	pStepper->Callback_TimerComplete = pCallback;

    // Get APB1 or APB2 timer clock
    u32Timer_clk = HAL_RCC_GetPCLK1Freq();

    pStepper->fMicroStepDiv = (uint32_t)m_uStepSel;

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
void Set_FrequencyOfMotor(Stepper *pStepper , uint32_t u32FrequncyHz)
{
	/*RPM=a/360∗f∗60*/
	uint32_t u32Arr = 0.0f;
	if(u32FrequncyHz)
	{
		u32Arr = (uint32_t)(DEFAULT_STEPPER_TIMER_FREQ / u32FrequncyHz);
		u32Arr -= 1U;
	}

	DisableStepper(pStepper);
	HAL_TIM_Base_Stop_IT(pStepper->pTim);
	__HAL_TIM_SET_AUTORELOAD(pStepper->pTim , (uint32_t)u32Arr);
	__HAL_TIM_SET_COUNTER(pStepper->pTim , 0);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Set_RpmOfMotor(Stepper *pStepper, uint32_t u32Rpm)
{
    uint32_t u32Arr = 0U;
    float u32Freq = 0.0;
    float fStepsPerRev = 0.0;

    /* Calculate steps per revolution */
    if(0U < pStepper->fStepAngle && 0U < pStepper->fMicroStepDiv)
    {
    	fStepsPerRev = 360.0 / ((float)pStepper->fStepAngle / (float)pStepper->fMicroStepDiv);
        /* Calculate required step frequency (steps/second) */
        u32Freq = ((float)u32Rpm / 60.0) * fStepsPerRev;

        /* GPIO toggle needs 2× timer frequency (rising + falling edge per step) */
        u32Freq *= 2.0;

        if(0U < u32Freq)
        {
            u32Arr = (uint32_t)((DEFAULT_STEPPER_TIMER_FREQ / u32Freq) - 1U);
            DisableStepper(pStepper);
            HAL_TIM_Base_Stop_IT(pStepper->pTim);
            __HAL_TIM_SET_AUTORELOAD(pStepper->pTim, u32Arr);
            __HAL_TIM_SET_COUNTER(pStepper->pTim, 0);
        }
        else
        {
        	/*NOP*/
        }
    }
    else
    {
    	/*NOP*/
    }
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
void Rotate_StepperSteps_Freq(Stepper *pStepper , uint32_t u32Steps ,  uint32_t u32Freq)
{
	Set_FrequencyOfMotor(pStepper , u32Freq);
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
	HAL_TIM_Base_Stop_IT(pStepper->pTim);
	pStepper->m_State = STEPPER_MOTOR_STOP;
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
void StartContinous_StepperMotor_Freq(Stepper *pStepper , uint32_t u32Freq)
{
	pStepper->u32CurrStepCount = 0;
	pStepper->u32TotalStepCount = 0;
	Set_FrequencyOfMotor(pStepper , u32Freq);
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
