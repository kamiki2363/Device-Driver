#include<reg52.h>
sbit LATCH  = P2^1;
sbit LATCH_2=P2^0;
sbit Keyport=P0;
#define number_0 0x3f
#define number_1 0x06
#define number_2 0x5b
#define number_3 0x4f
#define number_4 0x66
#define number_5 0x6d
#define number_6 0x7d
#define number_7 0x07
#define number_8 0x7f
#define number_9 0x6f

unsigned char code table[10] = {number_0, number_1, number_2, number_3, number_4, number_5, number_6, number_7, number_8, number_9};
unsigned char code try[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};


void Delay(unsigned int t)
{
	while(t--);
}
void DelayMs(unsigned int t)
{
	while(t--);
	{
	Delay(245);
	Delay(245);
	}
} 
unsigned char check_the_key(unsigned char cord_h,cord_l,total)
	{
		Keyport=0x0f;
		cord_h=Keyport&0x0f;
		if(cord_h!=0x0f)
		{DelayMs(10);
			if(Keyport&0x0f!=0x0f)
			{
				cord_h=Keyport&0x0f;
				Keyport=cord_h|0xf0;
				cord_l=Keyport&0xf0;
				while((Keyport&0xf0)!=0xf0);
				total=cord_h+cord_l;
				return(total);
			}
		}return(0xff);
	}

main (void) 
{
	unsigned char i=0,temp=0,total=0,cord_h,cord_l;
	while(1){
	unsigned char (check_the_key);
	if(total==0x77)
	{	
		P1=try[0];
		LATCH=1;
		LATCH=0;
		P1=number_1;
		LATCH_2=1;
		LATCH_2=0;
		DelayMs(50000);
	}else
	if(total==0xb7)
	{	
		P1=try[0];
		LATCH=1;
		LATCH=0;
		P1=number_2;
		LATCH_2=1;
		LATCH_2=0;
		DelayMs(50000);
	}else
		if(total==0xd7)
	{	
		P1=try[1];
		LATCH=1;
		LATCH=0;
		P1=number_1;
		LATCH_2=1;
		LATCH_2=0;
		DelayMs(50000);
	}
}

 	/*while(1)
	{
	P1=try[temp];
	LATCH=1;
	LATCH=0;
		if(temp>=7)
		{
			temp=0;
		}
		else(temp++);
	if(i>=9)
		i=0;
	else(i++);
	P1=table[i];
	LATCH_2=1;	
	LATCH_2=0;
	DelayMs(50000);	
}*/
	}
