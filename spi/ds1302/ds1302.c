#include "stc15f2k60s2.h"
#include <intrins.h>

#define sclk P17
#define io P23
#define rst P13

typedef unsigned char uint8_t;

code unsigned char w_addr[] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};
code unsigned char r_addr[] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d};

/// @brief write a byte to ds1302 register
///
/// @param[in] addr - the address of being writen register
/// @param[in] byte - a byte to ds1302 register
/// @return none
void write_byte(uint8_t byte)
{
	uint8_t i;
	
	for (i = 0; i < 8; i++)
	{
		io = byte & 0x01;
		byte >>= 1;
		sclk = 1;
		sclk = 0;
	}
	io = 1;
}

/// @brief read a byte from ds1302 register
///
/// @param[in] addr - the address of being read register
/// @return byte - a byte from ds1302
uint8_t read_byte()
{
	uint8_t byte = 0x00;
	uint8_t i;
	
	for (i = 0; i < 8; i++)
	{
		byte >>= 1;
		if (io)
			byte |= 0x80;
		sclk = 1;
		sclk = 0;
	}
	
	return byte;
}


/// @brief write a command to ds1302 register
///
/// @param[in] addr - the address of being writen register
/// @param[in] command - a command to ds1302 register
/// @return none
void ds1302_write(uint8_t addr, uint8_t command)
{
	rst = 1;
	write_byte(addr);
	write_byte(command);
	rst = 0;
}

/// @brief read a byte from ds1302 register
///
/// @param[in] addr - the address of being read register
/// @return byte - a byte from ds1302
uint8_t ds1302_read(uint8_t addr)
{
	uint8_t byte;
	
	rst = 1;
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
	uint8_t dat_inf[] = {
		0x00, 0x30, 0x15, 0x16, 0x12, 0x03, 0x20
	};
	
	sclk = 0;
	rst = 0;
	temp = ds1302_read(0x81);
	if ((temp & 0x80) != 0)
	{
		ds1302_write(0x8e, 0x00);  // disable write protect
		for (i = 0; i < 7; i++)
			ds1302_write(w_addr[i], dat_inf[i]);
	}
}
