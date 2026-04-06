/*
 * Drv_Switch.c
 *
 *  Created on: Jul 1, 2024
 *      Author: Alvin
 */
#include "main.h"
#include "Datatype.h"
#include "Drv_Switch.h"
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ConfigSwitch(SWITCH *pSw , GPIO_TypeDef *pPort , uint16_t u8Pin)
{
	pSw->p_Port = pPort;
	pSw->u8Pin = u8Pin;
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
SW_STATE GetState_Switch(SWITCH *pSw)
{
	SW_STATE m_State;
	pSw->u8PinVal = HAL_GPIO_ReadPin(pSw->p_Port, pSw->u8Pin);
	if(FALSE == pSw->u8PinVal && TRUE == pSw->u8PrevPinVal)
	{
		m_State = enSw_FallingEdge;
	}
	else if(TRUE == pSw->u8PinVal && FALSE == pSw->u8PrevPinVal)
	{
		m_State = enSw_RisingEdge;
	}
	else if(TRUE == pSw->u8PinVal && TRUE == pSw->u8PrevPinVal)
	{
		m_State = enSW_Released;
	}
	else/*When both condition are FALSE*/
	{
		m_State = enSW_Pushed;
	}
	pSw->u8PrevPinVal = pSw->u8PinVal;
	return (m_State);
}
