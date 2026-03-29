/*
 * DM556.h
 *
 *  Created on: Jun 16, 2024
 *      Author: Alvin
 */

#ifndef DRV_DM556_H_
#define DRV_DM556_H_

#define TIMER_FREQ_INPUT (16 * 1000000)/*xMhz*/
#define DEFAULT_STEPPER_TIMER_FREQ ((1) * 1000000)/*1000 Khz*/

#define STEPPER_DEFAULT_ANGLE (1.8)
#define STEPPER_MICROSTEP (64)

typedef enum
{
	Rotate_Clockwise = 0,
	Rotate_AntiClockwise,
}ROTATION_DIRECTION;

typedef enum
{
	TMC_STEALTH_CHOP = 0U,
	TMC_SPREAD_CYCLE,
}TMC_MOTOR_MODE;

typedef enum
{
	MOTOR_MS_FULL_STEP,
	MOTOR_MS_HALF_STEP,
	MOTOR_MS_STEP_1_4,
	MOTOR_MS_STEP_1_8,
	MOTOR_MS_STEP_1_16,
	MOTOR_MS_STEP_1_32 ,
	MOTOR_MS_STEP_1_64,
	MOTOR_MS_STEP_1_128,
	MOTOR_MS_STEP_1_256,
}MOTOR_MICRO_STEP_SEL;

typedef struct
{
	TIM_HandleTypeDef *pTim;
	uint32_t u32MicroStep;
	uint32_t u32StepAngle;
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
	uint8_t bContinousRotationEnable;
	void (*Callback_TimerComplete)(void *m_Inst);
}Stepper;

void Execute_PulseCallback(Stepper *pStepper);
void Config_StepperTimer(Stepper *pStepper , TIM_HandleTypeDef *pTimer , void *pCallback , MOTOR_MICRO_STEP_SEL m_uStepSel , TMC_MOTOR_MODE m_MotorMode);
void Set_RpmOfMotor(Stepper *pStepper , uint32_t u32Rpm);
void Rotate_StepperSteps(Stepper *pStepper , uint32_t u32Steps ,  uint32_t u32Rpm);
void Stop_StepperMotor(Stepper *pStepper);
void Start_StepperMotor(Stepper *pStepper);
void StartContinous_StepperMotor(Stepper *pStepper , uint32_t u32Rpm);
void SetDirection_Stepper(Stepper *pStepper , ROTATION_DIRECTION m_Dir);
void EnableStepper(Stepper *pStepper);
void DisableStepper(Stepper *pStepper);
#endif /* DRV_DM556_H_ */
