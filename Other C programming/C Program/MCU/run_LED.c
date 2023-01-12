#include<reg52.h>

void Delay(unsigned int t)
{
	while(t--);
}
 
main (void) 
{
	unsigned char temp;
	while(1){
	P1=temp=0xfe;
			Delay(500000); 
	while(temp<0xff)
	{
	temp=temp<<1;
	temp=temp|0x01;
	P1=temp;
	Delay(50000);
	}
		P1=temp=0x7f;
		Delay(500000);
	while(temp<0xff)
	{
	temp=temp>>1;
	temp=temp|0x80;
	P1=temp;
	Delay(50000);
	}
}
	}
