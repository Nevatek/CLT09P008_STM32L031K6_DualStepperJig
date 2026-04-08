/**
 ******************************************************************************
 * @file    Drv_InternalEEPROM.c
 * @brief   STM32L031K6 Internal Data EEPROM Driver Implementation
 * @author  Aldrin Rebello
 * @date    April 08, 2026
 ******************************************************************************
 * @attention
 *
 * This driver provides safe access to STM32L031K6 internal EEPROM
 * - Byte-wise programming with automatic unlock/lock
 * - Address validation
 * - Error handling with timeout protection
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "Drv_InternalEEPROM.h"
#include "stm32l0xx_hal.h"

/* Private defines -----------------------------------------------------------*/
#define EEPROM_TIMEOUT          1000U   /**< Timeout in milliseconds */

/* Private variables ---------------------------------------------------------*/
static EEPROM_StatusTypeDef lastError = EEPROM_OK;

/* Private function prototypes -----------------------------------------------*/
static bool IsAddressValid(uint16_t addr, uint16_t length);
static EEPROM_StatusTypeDef UnlockEEPROM(void);
static void LockEEPROM(void);
static EEPROM_StatusTypeDef WaitForLastOperation(uint32_t timeout);

/* Public Functions ----------------------------------------------------------*/

/**
 * @brief  Initializes the internal EEPROM driver
 */
EEPROM_StatusTypeDef Drv_InternalEEPROM_Init(void)
{
    lastError = EEPROM_OK;
    
    /* Ensure EEPROM is locked initially */
    LockEEPROM();
    return EEPROM_OK;
}

/**
 * @brief  Writes data buffer to internal EEPROM
 */
EEPROM_StatusTypeDef Drv_InternalEEPROM_Write(uint16_t addr,
                                               const uint8_t* pBuffer, 
                                               uint16_t length)
{
    EEPROM_StatusTypeDef status = EEPROM_OK;
    uint32_t eepromAddr;
    
    /* Validate parameters */
    if (pBuffer == NULL)
    {
        lastError = EEPROM_NULL_PTR;
        return EEPROM_NULL_PTR;
    }
    
    if (length == 0)
    {
        lastError = EEPROM_INVALID_LENGTH;
        return EEPROM_INVALID_LENGTH;
    }
    
    if (!IsAddressValid(addr, length))
    {
        lastError = EEPROM_INVALID_ADDR;
        return EEPROM_INVALID_ADDR;
    }
    
    /* Unlock EEPROM for write access */
    status = UnlockEEPROM();
    if (status != EEPROM_OK)
    {
        lastError = status;
        return status;
    }
    
    /* Calculate absolute EEPROM address */
    eepromAddr = EEPROM_START_ADDR + addr;
    
    /* Write data byte by byte */
    for (uint16_t i = 0; i < length; i++)
    {
        /* Wait for any previous operation to complete */
        status = WaitForLastOperation(EEPROM_TIMEOUT);
        if (status != EEPROM_OK)
        {
            LockEEPROM();
            lastError = status;
            return status;
        }
        
        /* Program byte using HAL function */
        HAL_StatusTypeDef halStatus = HAL_FLASHEx_DATAEEPROM_Program(
            FLASH_TYPEPROGRAMDATA_BYTE,
            eepromAddr + i,
            (uint32_t)pBuffer[i]
        );
        
        if (halStatus != HAL_OK)
        {
            LockEEPROM();
            lastError = EEPROM_WRITE_FAILED;
            return EEPROM_WRITE_FAILED;
        }
    }
    
    /* Wait for final operation to complete */
    status = WaitForLastOperation(EEPROM_TIMEOUT);
    
    /* Lock EEPROM after write */
    LockEEPROM();
    
    lastError = status;
    return status;
}

/**
 * @brief  Reads data from internal EEPROM into buffer
 */
EEPROM_StatusTypeDef Drv_InternalEEPROM_Read(uint16_t addr,
                                              uint8_t* pBuffer, 
                                              uint16_t length)
{
    uint32_t eepromAddr;
    
    /* Validate parameters */
    if (pBuffer == NULL)
    {
        lastError = EEPROM_NULL_PTR;
        return EEPROM_NULL_PTR;
    }
    
    if (length == 0)
    {
        lastError = EEPROM_INVALID_LENGTH;
        return EEPROM_INVALID_LENGTH;
    }
    
    if (!IsAddressValid(addr, length))
    {
        lastError = EEPROM_INVALID_ADDR;
        return EEPROM_INVALID_ADDR;
    }
    
    /* Calculate absolute EEPROM address */
    eepromAddr = EEPROM_START_ADDR + addr;
    
    /* Read data byte by byte */
    for (uint16_t i = 0; i < length; i++)
    {
        pBuffer[i] = (*(__IO uint8_t*)(eepromAddr + i));
    }
    
    lastError = EEPROM_OK;
    return EEPROM_OK;
}

/**
 * @brief  Erases specified number of bytes in EEPROM (writes 0x00)
 */
EEPROM_StatusTypeDef Drv_InternalEEPROM_Erase(uint16_t addr, uint16_t length)
{
    EEPROM_StatusTypeDef status;
    uint8_t zeroBuffer[256];
    uint16_t bytesToErase;
    uint16_t currentAddr = addr;
    uint16_t remainingBytes = length;
    
    /* Validate parameters */
    if (length == 0)
    {
        lastError = EEPROM_INVALID_LENGTH;
        return EEPROM_INVALID_LENGTH;
    }
    
    if (!IsAddressValid(addr, length))
    {
        lastError = EEPROM_INVALID_ADDR;
        return EEPROM_INVALID_ADDR;
    }
    
    /* Prepare zero buffer */
    for (uint16_t i = 0; i < 256; i++)
    {
        zeroBuffer[i] = 0x00;
    }
    
    /* Erase in chunks of up to 256 bytes */
    while (remainingBytes > 0)
    {
        bytesToErase = (remainingBytes > 256) ? 256 : remainingBytes;
        
        status = Drv_InternalEEPROM_Write(currentAddr, zeroBuffer, bytesToErase);
        if (status != EEPROM_OK)
        {
            lastError = status;
            return status;
        }
        
        currentAddr += bytesToErase;
        remainingBytes -= bytesToErase;
    }
    
    lastError = EEPROM_OK;
    return EEPROM_OK;
}

/**
 * @brief  Erases entire EEPROM (writes 0x00 to all locations)
 */
EEPROM_StatusTypeDef Drv_InternalEEPROM_EraseAll(void)
{
    return Drv_InternalEEPROM_Erase(0, EEPROM_SIZE);
}

/**
 * @brief  Writes a single byte to EEPROM
 */
EEPROM_StatusTypeDef Drv_InternalEEPROM_WriteByte(uint16_t addr, uint8_t data)
{
    return Drv_InternalEEPROM_Write(addr, &data, 1);
}

/**
 * @brief  Reads a single byte from EEPROM
 */
EEPROM_StatusTypeDef Drv_InternalEEPROM_ReadByte(uint16_t addr, uint8_t* pData)
{
    return Drv_InternalEEPROM_Read(addr, pData, 1);
}

/**
 * @brief  Gets the last error status
 */
EEPROM_StatusTypeDef Drv_InternalEEPROM_GetLastError(void)
{
    return lastError;
}

/* Private Functions ---------------------------------------------------------*/

/**
 * @brief  Validates if address and length are within EEPROM bounds
 */
static bool IsAddressValid(uint16_t addr, uint16_t length)
{
    if (addr >= EEPROM_SIZE)
    {
        return false;
    }
    
    if ((uint32_t)addr + (uint32_t)length > EEPROM_SIZE)
    {
        return false;
    }
    
    return true;
}

/**
 * @brief  Unlocks the EEPROM for write access
 */
static EEPROM_StatusTypeDef UnlockEEPROM(void)
{
    HAL_StatusTypeDef halStatus;
    
    /* Unlock the Flash to enable the flash control register access */
    halStatus = HAL_FLASH_Unlock();
    if (halStatus != HAL_OK)
    {
        return EEPROM_UNLOCK_FAILED;
    }
    
    /* Unlock the Data EEPROM */
    halStatus = HAL_FLASHEx_DATAEEPROM_Unlock();
    if (halStatus != HAL_OK)
    {
        HAL_FLASH_Lock();
        return EEPROM_UNLOCK_FAILED;
    }
    
    return EEPROM_OK;
}

/**
 * @brief  Locks the EEPROM after write access
 */
static void LockEEPROM(void)
{
    /* Lock the Data EEPROM */
    HAL_FLASHEx_DATAEEPROM_Lock();
    
    /* Lock the Flash */
    HAL_FLASH_Lock();
}

/**
 * @brief  Waits for the last EEPROM operation to complete
 */
static EEPROM_StatusTypeDef WaitForLastOperation(uint32_t timeout)
{
    uint32_t tickstart = HAL_GetTick();
    
    /* Wait until operation is complete or timeout occurs */
    while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY))
    {
        if ((HAL_GetTick() - tickstart) > timeout)
        {
            return EEPROM_TIMEOUT;
        }
    }
    
    /* Check for any error flags */
    if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_WRPERR) || 
        __HAL_FLASH_GET_FLAG(FLASH_FLAG_PGAERR) ||
        __HAL_FLASH_GET_FLAG(FLASH_FLAG_SIZERR) ||
        __HAL_FLASH_GET_FLAG(FLASH_FLAG_OPTVERR) ||
        __HAL_FLASH_GET_FLAG(FLASH_FLAG_RDERR) ||
        __HAL_FLASH_GET_FLAG(FLASH_FLAG_FWWERR) ||
        __HAL_FLASH_GET_FLAG(FLASH_FLAG_NOTZEROERR))
    {
        /* Clear error flags */
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | 
                               FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR | 
                               FLASH_FLAG_OPTVERR | FLASH_FLAG_RDERR | 
                               FLASH_FLAG_FWWERR | FLASH_FLAG_NOTZEROERR);
        return EEPROM_WRITE_FAILED;
    }
    
    /* Clear end of operation flag */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
    
    return EEPROM_OK;
}

/****************************** END OF FILE ***********************************/
