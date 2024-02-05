#include "utils.h"

#include <stdio.h>
#include <stdint.h>
#include "helpers.h"

void read_top_sec(unsigned int *addr, unsigned int length)
{
	if((uint32_t)addr >= 0x10000000 && ((int)addr + length - 1) <= 0x10004000)
	{
		dump_bytes(addr, length, (unsigned long)addr);
	}
	else
	{
		printf("Address out of the TOP-SECRET range.");
	}
}

void read_csr(unsigned int *addr, unsigned int length) {

	if((uint32_t)addr >= 0xf0000000 && ((uint32_t)addr + length - 1) <= 0xf0010000)
	{
		dump_bytes(addr, length, (unsigned long)addr);
	}
	else
	{
		printf("Address out of the CSR range.");
	}
}

void mem_write(void *addr, unsigned int value, unsigned int count, unsigned int size)
{
	for (int i = 0; i < count; i++) {
		switch (size) {
		case 1:
			*(uint8_t *)addr = value;
			addr += 1;
			break;
		case 2:
			*(uint16_t *)addr = value;
			addr += 2;
			break;
		case 4:
			*(uint32_t *)addr = value;
			addr += 4;
			break;
		case 8:
			*(uint64_t *)addr = value;
			addr += 8;
			break;
		default:
			printf("Incorrect size");
			return;
		}
	}
}
