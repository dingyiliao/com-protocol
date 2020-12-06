#include "iic.h"
#include <intrins.h>

/// @brief delay about 5us for some neccessary operation
///
/// @param none
/// @return none
void iic_delay(void)
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 12;
	while (--i);
}

/// @brief generate a start condition
///
/// @param none
/// @return none
void iic_start(void)
{
	// if scl = 1 in the last time, which will generate a stop condition and does not effect current communication
	// if scl = 0 in the last time, which also has not effect to current communication
	sda = 1;
	scl = 1;
	iic_delay();
	sda = 0;
	iic_delay();
	scl = 0;
}

/// @brief generate a stop condition
///
/// @param none
/// @return none
void iic_stop(void)
{
	// if scl = 1 in the last time, which will generate a start condition and does not effect current communication
	// if scl = 0 in the last time, which has not effect to current communication
	sda = 0;
	scl = 1;
	iic_delay();
	sda = 1;
}

/// @brief write a byte to the slave
///
/// @param[in] byt - a byte that wait to be sent
/// @return none
void iic_write(unsigned char byt)
{
	unsigned char idx;
	
	EA = 0;
	for (idx = 0; idx < 8; idx++)
	{
		sda = byt & 0x80 ? 1 : 0;
		iic_delay();
		scl = 1;
		byt <<= 1;
		iic_delay();
		scl = 0;
	}
	EA = 1;
}

/// @brief wait for a acknowledge signal from the slave
///
/// @param none
/// @return ack - acknowledge from the slave
bit iic_wait_ack(void)
{
	bit ack;

	sda = 1;
	iic_delay();
	scl = 1;
	iic_delay();
	ack = sda;
	scl = 0;
	iic_delay();
	
	return ack;
}

/// @brief read a byte from the slave
///
/// @param none
/// @return byt - a byte from the slave
unsigned char iic_read(void)
{
	unsigned char byt = 0x00;
	unsigned char idx;
	
	EA = 0;
	for (idx = 0; idx < 8; idx++)
	{
		scl = 1;
		iic_delay();
		byt <<= 1;
		if (sda)
			byt |= 0x01;
		scl = 0;
		iic_delay();
	}
	EA = 1;
	
	return byt;
}

/// @brief send a acknowledge to the slave
///
/// @param[in] ack - the acknowledge signal that need to be sent to the slave
/// @return none
void iic_send_ack(bit ack)
{
	scl = 0;
	iic_delay();
	sda = ack;
	iic_delay();
	scl = 1;
	iic_delay();
	scl = 0;
	sda = 1;  	// release bus
	iic_delay();
}
