/*
 * DM556.h
 *
 *  Created on: Jun 16, 2024
 *      Author: Alvin
 */

#ifndef DRV_DM556_H_
#define DRV_DM556_H_

#define DEFAULT_STEPPER_TIMER_FREQ (1000000ULL)/*1000 Khz*/

typedef enum
{
	STEPPER_MOTOR_STOP = 0U,
	STEPPER_MOTOR_RUNNING,
	STEPPER_MOTOR_START_ENTRY,
	STEPPER_MOTOR_STOP_ENTRY,
}MOTOR_STATE;

typedef enum
{
	Rotate_Clockwise = 0U,
	Rotate_AntiClockwise,
}ROTATION_DIRECTION;

typedef enum
{
	TMC_STEALTH_CHOP = 0U,
	TMC_SPREAD_CYCLE,
}TMC_MOTOR_MODE;

typedef enum
{
	MOTOR_MS_FULL_STEP = 1U,
	MOTOR_MS_HALF_STEP = 2U,
	MOTOR_MS_STEP_1_4 = 4U,
	MOTOR_MS_STEP_1_8 = 8U,
	MOTOR_MS_STEP_1_16 = 16U,
	MOTOR_MS_STEP_1_32  = 32U,
	MOTOR_MS_STEP_1_64 = 64U,
	MOTOR_MS_STEP_1_128 = 128U,
	MOTOR_MS_STEP_1_256 = 256U,
}MOTOR_MICRO_STEP_SEL;

typedef enum
{
	MOTOR_STEP_ANG_0_9 = 0U,
	MOTOR_STEP_ANG_1_8,
	MOTOR_STEP_ANG_2_5,
	MOTOR_STEP_ANG_2_7,
	MOTOR_STEP_ANG_3_6,
	MOTOR_STEP_ANG_7_2,
	MOTOR_STEP_ANG_7_5,
	MOTOR_STEP_ANG_MAX
}MOTOR_STEP_ANGLE_SEL;

typedef struct
{
	TIM_HandleTypeDef *pTim;
	float fStepAngle;
	uint32_t fMicroStepDiv;
	uint32_t u32CurrStepCount;
	uint32_t u32TotalStepCount;
	uint32_t u32TimerInputFreq;
	uint32_t u32TimerBaseFreq;
	GPIO_TypeDef *p_PulsePort;
	GPIO_TypeDef *p_DirPort;
	GPIO_TypeDef *p_EnablePort;
	uint16_t u8PulsePin;
	uint16_t u8DirPin;
	uint16_t u8EnablePin;
	uint8_t bContinousRotationEnable : 1U;
	uint8_t bHomeSensEnable : 1U;
	uint8_t bPulsePinState : 1U;
	uint8_t bMoveToHomeFlag : 1U;
	uint8_t : 4U;
	MOTOR_STATE m_State;
	void (*Callback_TimerComplete)(void *m_Inst);
}Stepper;

void Execute_PulseCallback_MotorX(Stepper *pStepper);
void Execute_PulseCallback_MotorY(Stepper *pStepper);
void Config_StepperTimer(Stepper *pStepper , TIM_HandleTypeDef *pTimer ,
		void *pCallback , MOTOR_STEP_ANGLE_SEL m_MtrAngleSel , MOTOR_MICRO_STEP_SEL m_uStepSel ,
		TMC_MOTOR_MODE m_MotorMode);
void Set_RpmOfMotor(Stepper *pStepper , uint32_t u32Rpm);
void Set_FrequencyOfMotor(Stepper *pStepper , uint32_t u32FrequncyHz);
void Rotate_StepperSteps(Stepper *pStepper , uint32_t u32Steps ,  uint32_t u32Rpm);
void Rotate_StepperSteps_Freq(Stepper *pStepper , uint32_t u32Steps ,  uint32_t u32Freq);
void Stop_StepperMotor(Stepper *pStepper);
void Start_StepperMotor(Stepper *pStepper);
void StartContinous_StepperMotor(Stepper *pStepper , uint32_t u32Rpm);
void StartContinous_StepperMotor_Freq(Stepper *pStepper , uint32_t u32Freq);
void SetDirection_Stepper(Stepper *pStepper , ROTATION_DIRECTION m_Dir);
void EnableStepper(Stepper *pStepper);
void DisableStepper(Stepper *pStepper);
MOTOR_STATE HI_GetMotorState(Stepper *pStepper);
#endif /* DRV_DM556_H_ */
