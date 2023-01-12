#include<reg52.h>

void Delay(unsigned int t)
{
	while(t--);
}
 
main (void) 
{
	unsigned char temp;
	while(1){

		P1=temp=0x80;
		Delay(50000);
	while(temp>0)
	{
	temp=temp>>1;
		Delay(50000);
		temp=temp>>1;
	P1=temp;
	Delay(50000);
	}
}
	}
