/* Copyright 2023 Dual Tachyon
 * https://github.com/DualTachyon
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *     Unless required by applicable law or agreed to in writing, software
 *     distributed under the License is distributed on an "AS IS" BASIS,
 *     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *     See the License for the specific language governing permissions and
 *     limitations under the License.
 */

#include <stddef.h>
#include <string.h>

#include "driver/eeprom.h"
#include "driver/i2c.h"
#include "driver/system.h"
#include "eeprom_map.h"

// 24M02: 2Mbit (256KB) organized as 4 x 64KB pages
// Device address: 1010 A17 A16 R/W
// A17:A16 select the 64KB page from bits 17:16 of the address

static uint8_t EEPROM_DeviceAddr(uint32_t Address)
{
    return 0xA0 | (((Address >> 16) & 0x03) << 1);
}

void EEPROM_ReadBuffer(uint32_t Address, void *pBuffer, uint16_t Size)
{
    uint8_t devAddr = EEPROM_DeviceAddr(Address);

    I2C_Start();
    I2C_Write(devAddr);
    I2C_Write((Address >> 8) & 0xFF);
    I2C_Write((Address >> 0) & 0xFF);
    I2C_Start();
    I2C_Write(devAddr | 1);
    I2C_ReadBuffer(pBuffer, Size);
    I2C_Stop();
}

void EEPROM_WriteBuffer(uint32_t Address, const void *pBuffer)
{
    if (pBuffer == NULL || Address >= EEPROM_MAX_ADDR)
        return;

    uint8_t devAddr = EEPROM_DeviceAddr(Address);

    uint8_t buffer[8];
    EEPROM_ReadBuffer(Address, buffer, 8);
    if (memcmp(pBuffer, buffer, 8) == 0) {
        return;
    }

    I2C_Start();
    I2C_Write(devAddr);
    I2C_Write((Address >> 8) & 0xFF);
    I2C_Write((Address >> 0) & 0xFF);
    I2C_WriteBuffer(pBuffer, 8);
    I2C_Stop();

    // 24M02 write cycle: max 10ms
    SYSTEM_DelayMs(10);
}
