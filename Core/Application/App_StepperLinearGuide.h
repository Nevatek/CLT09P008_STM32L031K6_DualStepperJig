/*
 * App_StepperLinearGuide.h
 *
 *  Created on: 29-Mar-2026
 *      Author: aldrin.rebellow
 */

#ifndef APPLICATION_APP_STEPPERLINEARGUIDE_H_
#define APPLICATION_APP_STEPPERLINEARGUIDE_H_


#define DEFAULT_HOME_POS_OFFSET_STEPS		(10U)
#define MIN_DEFAULT_MOTOR_STROKE_DELAY_MS	(100U)
#define DEFAULT_MOTOR_STROKE_DELAY_MS		(1000U)

typedef enum
{
	MTR_APP_IDLE = 0U,
	MTR_APP_MOVE_TO_HOME,
	MTR_APP_MOVE_TO_HOME_WAIT,
	MTR_APP_MOVE_TO_HOME_COMPLETED,
	MTR_APP_MOVE_TO_STEP,
	MTR_APP_MOVE_TO_STEP_WAIT,
	MTR_APP_MOVE_TO_STEP_COMPLETED
}MTR_APP_PROCESS;

void App_StepperLinearGuide_Init(void);
void App_StepperLinearGuide_Exe(void);
void Callback_StepperTimer_MotX(void);
void Callback_StepperTimer_MotY(void);
void CallBack_HomeSensDetectMotorX(void);
void CallBack_HomeSensDetectMotorY(void);
void App_Disable_AllMotors(void);
#endif /* APPLICATION_APP_STEPPERLINEARGUIDE_H_ */
