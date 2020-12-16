#include "stc15f2k60s2.h"

extern initialize_ds1302();
extern ds1302_read(unsigned char);

unsigned char flag = 0;
unsigned char buff[7] = { 0 };
unsigned char seg[] = {
	0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e
};

void cls_led()
{
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0x80;
	P2 &= 0x1f;
}

void sel_disp(unsigned char disp, unsigned char seg)
{
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0xf0;
	P2 &= 0x1f;
	P0 = 0x01 << disp;
	P2 = (P2 & 0x1f) | 0xc0;
	P2 &= 0x1f;
	P0 = seg;
	P2 = (P2 & 0x1f) | 0xf0;
	P2 &= 0x1f;
}

void conf_t0()
{
	AUXR |= 0x80;  // set 1T mode
	TMOD &= 0xf0;
	TH0 = 0xd1;
	TL0 = 0x20;
	TF0 = 0;
	ET0 = 1;
	TR0 = 1;
	EA = 1;
}

void main()
{
	unsigned char i;
	unsigned char addr_inf[] = {
		0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c
	};
	
	cls_led();
	initialize_ds1302();
	conf_t0();
	while (1)
	{
		if (flag)
		{
			flag = 0;
			for (i = 0; i < 7; i++)
				buff[i] = ds1302_read(addr_inf[i]);
		}
		for (i = 0; i < 6; i++)
			switch (i)
			{
				case 0: sel_disp(i, seg[buff[0] & 0x0f]); break;
				case 1: sel_disp(i, seg[buff[0] >> 4]); break;
				case 2: sel_disp(i, seg[buff[1] & 0x0f]); break;
				case 3: sel_disp(i, seg[buff[1] >> 4]); break;
				case 4: sel_disp(i, seg[buff[2] & 0x0f]); break;
				case 5: sel_disp(i, seg[buff[2] >> 4]); break;
			}
	}
}

void t0_routine() interrupt 1
{
	static unsigned char idx = 0;
	
	if (++idx == 200)
	{
		idx = 0;
		flag = 1;
	}
}
