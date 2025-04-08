/*
 * flash.c
 *
 *  Created on: Mar 30, 2025
 *      Author: P.HIEU
 */

#include "flash.h"

void Flash_Unlock(void){
	if (FLASH_CR & FLASH_CR_LOCKa){
		FLASH_KEYR = FLASH_KEY1a;
		FLASH_KEYR = FLASH_KEY2a;
		while(FLASH_CR & FLASH_CR_LOCKa);
	}
}

void Flash_Lock(void){
	FLASH_CR |= FLASH_CR_LOCKa;
}

void Flash_ErasePage(uint32_t page){
	while(FLASH_CR & FLASH_CR_LOCKa);
	FLASH_CR |= FLASH_CR_PERa;
	FLASH_AR = page;
	FLASH_CR |= FLASH_CR_STRTa;
	while(FLASH_SR & FLASH_SR_BSYa);
	FLASH_CR &= ~FLASH_CR_PERa;
}

void Flash_Write(uint32_t address, uint16_t data){
	while(FLASH_CR & FLASH_CR_LOCKa);
	FLASH_CR |= FLASH_CR_PGa;
	*(volatile uint16_t *)address = data;
	while(FLASH_SR & FLASH_SR_BSYa);
	FLASH_CR &= ~FLASH_CR_PGa;
}

void Flash_WriteString(uint32_t address, const char* str) {
	FLASH_ACR &= ~FLASH_ACR_LATENCYa;
    uint16_t data;
    uint32_t i;
    uint32_t len = strlen(str) + 1;

    Flash_Unlock();
    Flash_ErasePage(address);

    for (i = 0; i < len; i += 2) {
        data = str[i] | (str[i + 1] << 8);
        Flash_Write(address + i, data);
    }

    Flash_Lock();
}

void Flash_ReadString(uint32_t location, char *buffer, size_t max_length) {
    uint32_t address = location;
    size_t i = 0;

    while (i < max_length - 1) {
        uint16_t data = *(uint16_t *)address;
        buffer[i] = data & 0xFF;
        buffer[i + 1] = (data >> 8) & 0xFF;

        if (buffer[i] == '\0' || buffer[i + 1] == '\0') break;
        address += 2;
        i += 2;
    }

    buffer[max_length - 1] = '\0';
}
