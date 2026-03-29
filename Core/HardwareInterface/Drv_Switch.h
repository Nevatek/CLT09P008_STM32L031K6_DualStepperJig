/*
 * Drv_Switch.h
 *
 *  Created on: Jul 1, 2024
 *      Author: Alvin
 */

#ifndef DRV_SWITCH_H_
#define DRV_SWITCH_H_

typedef enum
{
	enSW_Released = 0,
	enSW_Pushed,
	enSw_RisingEdge,
	enSw_FallingEdge,
}SW_STATE;

typedef struct
{
	GPIO_TypeDef *p_Port;
	uint16_t u8Pin;
	uint8_t u8PinVal;
	uint8_t u8PrevPinVal;
	SW_STATE m_CurrState;
}SWITCH;

void ConfigSwitch(SWITCH *pSw , GPIO_TypeDef *pPort , uint16_t u8Pin);
SW_STATE GetState_Switch(SWITCH *pSw);
#endif /* DRV_SWITCH_H_ */
