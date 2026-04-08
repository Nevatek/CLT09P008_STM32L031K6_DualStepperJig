/**
 ******************************************************************************
 * @file    Drv_InternalEEPROM.h
 * @brief   STM32L031K6 Internal Data EEPROM Driver Header
 * @author  Aldrin Rebello
 * @date    April 08, 2026
 ******************************************************************************
 * @attention
 *
 * STM32L031K6 has 1KB of internal Data EEPROM
 * Address Range: 0x08080000 to 0x080803FF
 * Programming: Byte, Half-Word, Word aligned access
 *
 ******************************************************************************
 */

#ifndef DRV_INTERNAL_EEPROM_H
#define DRV_INTERNAL_EEPROM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/* Defines -------------------------------------------------------------------*/
#define EEPROM_START_ADDR       0x08080000U     /**< EEPROM start address */
#define EEPROM_END_ADDR         0x080803FFU     /**< EEPROM end address */
#define EEPROM_SIZE             1024U           /**< Total EEPROM size in bytes */

/* Return Status Codes -------------------------------------------------------*/
typedef enum
{
    EEPROM_OK               = 0x00U,    /**< Operation successful */
    EEPROM_ERROR            = 0x01U,    /**< General error */
    EEPROM_INVALID_ADDR     = 0x02U,    /**< Invalid address */
    EEPROM_INVALID_LENGTH   = 0x03U,    /**< Invalid data length */
    EEPROM_NULL_PTR         = 0x04U,    /**< Null pointer error */
    EEPROM_UNLOCK_FAILED    = 0x05U,    /**< Failed to unlock EEPROM */
    EEPROM_WRITE_FAILED     = 0x06U,    /**< Write operation failed */
    EEPROM_READ_FAILED      = 0x07U,    /**< Read operation failed */
    EEPROM_TIMEOUT          = 0x08U,    /**< Operation timeout */
} EEPROM_StatusTypeDef;

/* Public Function Prototypes ------------------------------------------------*/

/**
 * @brief  Initializes the internal EEPROM driver
 * @note   This function should be called once before using EEPROM operations
 * @retval EEPROM_StatusTypeDef: Status of initialization
 */
EEPROM_StatusTypeDef Drv_InternalEEPROM_Init(void);

/**
 * @brief  Writes data buffer to internal EEPROM
 * @param  addr: EEPROM address offset (0 to EEPROM_SIZE-1)
 * @param  pBuffer: Pointer to data buffer to write
 * @param  length: Number of bytes to write
 * @retval EEPROM_StatusTypeDef: Status of write operation
 * @note   Address range: 0 to 1023 (relative offset from EEPROM_START_ADDR)
 */
EEPROM_StatusTypeDef Drv_InternalEEPROM_Write(uint16_t addr,
                                               const uint8_t* pBuffer, 
                                               uint16_t length);

/**
 * @brief  Reads data from internal EEPROM into buffer
 * @param  addr: EEPROM address offset (0 to EEPROM_SIZE-1)
 * @param  pBuffer: Pointer to buffer where data will be read
 * @param  length: Number of bytes to read
 * @retval EEPROM_StatusTypeDef: Status of read operation
 * @note   Address range: 0 to 1023 (relative offset from EEPROM_START_ADDR)
 */
EEPROM_StatusTypeDef Drv_InternalEEPROM_Read(uint16_t addr,
                                              uint8_t* pBuffer, 
                                              uint16_t length);

/**
 * @brief  Erases specified number of bytes in EEPROM (writes 0x00)
 * @param  addr: EEPROM address offset (0 to EEPROM_SIZE-1)
 * @param  length: Number of bytes to erase
 * @retval EEPROM_StatusTypeDef: Status of erase operation
 */
EEPROM_StatusTypeDef Drv_InternalEEPROM_Erase(uint16_t addr, uint16_t length);

/**
 * @brief  Erases entire EEPROM (writes 0x00 to all locations)
 * @retval EEPROM_StatusTypeDef: Status of erase operation
 */
EEPROM_StatusTypeDef Drv_InternalEEPROM_EraseAll(void);

/**
 * @brief  Writes a single byte to EEPROM
 * @param  addr: EEPROM address offset (0 to EEPROM_SIZE-1)
 * @param  data: Byte to write
 * @retval EEPROM_StatusTypeDef: Status of write operation
 */
EEPROM_StatusTypeDef Drv_InternalEEPROM_WriteByte(uint16_t addr, uint8_t data);

/**
 * @brief  Reads a single byte from EEPROM
 * @param  addr: EEPROM address offset (0 to EEPROM_SIZE-1)
 * @param  pData: Pointer to store read byte
 * @retval EEPROM_StatusTypeDef: Status of read operation
 */
EEPROM_StatusTypeDef Drv_InternalEEPROM_ReadByte(uint16_t addr, uint8_t* pData);

/**
 * @brief  Gets the last error status
 * @retval EEPROM_StatusTypeDef: Last error status
 */
EEPROM_StatusTypeDef Drv_InternalEEPROM_GetLastError(void);

#ifdef __cplusplus
}
#endif

#endif /* DRV_INTERNAL_EEPROM_H */

/****************************** END OF FILE ***********************************/
