#include "stc15f2k60s2.h"
#include <intrins.h>

#define sclk P17
#define io P23
#define rst P13

typedef unsigned char uint8_t;

void delay_for_4us()
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 9;
	while (--i);
}

void write_byte(uint8_t byte)
{
	uint8_t i;
	
	EA = 0;
	for (i = 0; i < 8; i++)
	{
		sclk = 0;
		io = byte & 0x01;
		byte >>= 1;
		sclk = 1;
	}
	io = 1;
	EA = 1;
}

uint8_t read_byte()
{
	uint8_t byte = 0x00;
	uint8_t i;
	
	EA = 0;
	for (i = 0; i < 8; i++)
	{
		sclk = 0;
		byte >>= 1;
		if (io)
			byte |= 0x80;
		sclk = 1;
	}
	EA = 1;
	
	return byte;
}


/// @brief write a byte to ds1302 register
///
/// @param[in] addr - the address of being writen register
/// @param[in] byte - a byte to ds1302 register
/// @return none
void ds1302_write(uint8_t addr, uint8_t byte)
{
	rst = 0;
	delay_for_4us();
	sclk = 0;
	delay_for_4us();
	rst = 1;
	delay_for_4us();
	write_byte(addr);
	write_byte(byte);
	rst = 0;
}

/// @brief read a byte from ds1302 register
///
/// @param[in] addr - the address of being read register
/// @return byte - a byte from ds1302
uint8_t ds1302_read(uint8_t addr)
{
	uint8_t byte;
	
	rst = 0;
	delay_for_4us();
	sclk = 0;
	delay_for_4us();
	rst = 1;
	delay_for_4us();
	write_byte(addr);
	byte = read_byte();
	rst = 0;
	
	return byte;
}

/// @brief initialize ds1302
///
/// @param none
/// @return none
void initialize_ds1302()
{
	uint8_t temp;
	uint8_t i;
	uint8_t addr_inf[] = {
		0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c
	};
	uint8_t dat_inf[] = {
		0x00, 0x30, 0x15, 0x16, 0x12, 0x03, 0x20
	};
	
	temp = ds1302_read(0x81);
	if (temp & 0x80)
	{
		ds1302_write(0x8e, 0x00);  // disable write protect
		for (i = 0; i < 7; i++)
			ds1302_write(addr_inf[i], dat_inf[i]);
	}
}
