#include "wire.h"
#include <intrins.h>

void delay(unsigned short us)  // each delay 1us
{
    do
    {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
    while (us--);
}

/// @brief generate a reset pulse
///
/// @param none
/// @return ack - a response from the slave
bit wire_init(void)
{
	bit ack;
	
	EA = 0;
	wire = 0;
	delay(480);  // generate a 480us reset pulse
	wire = 1;
	delay(70);  // release wire bus to wait a response from the slave
	ack = wire;
	while (!wire);  // waiting for wire to be low electric level
	EA = 1;

	return ack;
}

/// @brief write a byte to the slave
///
/// @param[in] dat - data that need to be transmitted to the slave
/// @return none
void wire_write(unsigned char dat)
{
    unsigned char idx;

    EA = 0;
    for (idx = 0; idx < 8; idx++)
    {
        if (dat & 0x01)
        {
            wire = 0;
            delay(6);
            wire= 1;
            delay(64);
        }
        else
        {
            wire = 0;
            delay(60);
            wire = 1;
            delay(10);
        }
				dat >>= 1;
    }
    EA = 1;
}

/// @brief receive a byte from the slave
///
/// @param none
/// @return dat - a byte that was received from the slave
unsigned char wire_read(void)
{
    unsigned char mask;
    unsigned char dat = 0xff;

    EA = 0;
    for (mask = 0x01; mask != 0; mask <<= 1)
    {
        wire = 0;
        delay(6);
        wire = 1;
        delay(9);
        if (!wire)
            dat &= (~mask);
        delay(55);
    }
    EA = 1;

    return dat;
}
