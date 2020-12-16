#include "stc15f2k60s2.h"
#include <intrins.h>

#define sclk P17
#define io P23
#define rst P13

typedef unsigned char uint8_t;

code uint8_t w_addr[] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};
code uint8_t w_wp = 0x8e;
code uint8_t r_sec = 0x81;

void ds1302_write_single_byte(uint8_t addr, uint8_t byte)
{
	uint8_t i;
	
	rst = 0;
	_nop_();
	sclk = 0;
	_nop_();
	rst = 1;
	_nop_();
	for (i = 0; i < 8; i++)
	{
		sclk = 0;
		io = addr & 0x01;  // transmit from lsb end at msb
		addr >>= 1;
		sclk = 1;
	}
	for (i = 0; i < 8; i++)
	{
		sclk = 0;
		io = byte & 0x01;
		byte >>= 1;
		sclk = 1;
	}
	rst = 0;
	_nop_();
}

uint8_t ds1302_read_single_byte(uint8_t addr)
{
	uint8_t 
		byte = 0x00,
		i;
	
	rst = 0;
	_nop_();
	sclk = 0;
	_nop_();
	rst = 1;
	_nop_();
	for (i = 0; i < 8; i++)  // write address to ds1302
	{
		sclk = 0;
		io = addr & 0x01;  // transmit from lsb end at msb
		addr >>= 1;
		sclk = 1;
	}
	for (i = 0; i < 8; i++)  // write command to ds1302
	{
		sclk = 0;
		byte >>= 1;
		if (io)
			byte |= 0x80;
		sclk = 1;
	}
	rst = 0;
	_nop_();
	// used to make ds1302 stable
	sclk = 0;
	_nop_();
	sclk = 1;
	_nop_();
	io = 0;
	_nop_();
	io = 1;
	_nop_();
	
	return byte;
}

void ds1302_init()
{
	uint8_t
		temp,
		dat_inf[] = {	0x00, 0x30, 0x15, 0x16, 0x12, 0x03, 0x20 };

	temp = ds1302_read_single_byte(r_sec);
	if (temp & 0x80 != 0)
	{
		ds1302_write_single_byte(w_wp, 0x00);
		for (temp = 0; temp < 7; temp++)
			ds1302_write_single_byte(w_addr[temp], dat_inf[temp]);
	}
}
