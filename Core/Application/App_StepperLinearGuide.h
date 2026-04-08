/*
 * App_StepperLinearGuide.h
 *
 *  Created on: 29-Mar-2026
 *      Author: aldrin.rebellow
 */

#ifndef APPLICATION_APP_STEPPERLINEARGUIDE_H_
#define APPLICATION_APP_STEPPERLINEARGUIDE_H_

void App_StepperLinearGuide_Init(void);
void App_StepperLinearGuide_Exe(void);
void Callback_StepperTimer_MotX(void);
void Callback_StepperTimer_MotY(void);
void CallBack_HomeSensDetectMotorX(void);
void CallBack_HomeSensDetectMotorY(void);
void App_Disable_AllMotors(void);
#endif /* APPLICATION_APP_STEPPERLINEARGUIDE_H_ */
