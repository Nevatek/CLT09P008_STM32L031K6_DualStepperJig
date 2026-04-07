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
#include "Drv_DM556.h"
#include "App_StepperLinearGuide.h"
#include "ApplicationLayer.h"

APPL_CONFIG g_ApplConfig;
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ApplicationLayer_Init(void)
{
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
	/*Execute Stepper linear guide app layer*/
	App_StepperLinearGuide_Exe();
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
/*END OF FILE*/
