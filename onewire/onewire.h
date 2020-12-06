#ifndef __WIRE_H

#include "STC15F2K60S2.H"

#define wire P14

bit wire_init(void);
void wire_write(unsigned char dat);
unsigned char wire_read(void);

#endif
