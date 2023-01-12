/*MY name is Kei Hong Chan and my reg number is 140170067*/
#include <stdio.h>
#include <math.h>
#define value 10

int check_value(unsigned int number,int chk)
{
	float number2;
do{
char buf[80];
printf("\nhow many resistor you have? \nPlease in the form num/num to confirm second times.\n");
gets(buf);
chk = sscanf(buf,"%u/%f",&number,&number2);
printf("\nchk is %d",chk);
printf("\nthe number of resistor you type is %u",number);
if(chk!=2 || number < 2  || number > 10 || number != number2)
{
 printf("\nYou type the value is out of range!! \nYour value have decimal or not the same value between two value.");
 system("PAUSE");
 exit(0);
}
}while(chk!=2);
return(number);
}

float take_value (unsigned int number,float y[value],int ary)
{
/*ary is the value of array*/
  for(ary=0;ary<number;++ary)
  {
    printf("\nFor value %d ",ary+1);
    scanf("%f", &y[ary]);
    if(y[ary]>1e6||y[ary]<1e-3)
    {
    printf("\nYour resistance is out of range! Program stop.");
    system("PAUSE");
    exit(0);
    }
    printf("\nthe value of Resistor%d is %fOhms",ary+1,y[ary]);
}
return(y[value]);
   
}

float mean_value(unsigned int number, float y[value],int ary)
{
	float sum=0 ,x=0;
    for(ary=0;ary<number;++ary)
      sum += y[ary];
   /*find the value of average*/
    printf("\nThe total resistance in series is %f",sum);
    x = sum/number;
    /*x is the value of average*/
	return(x); 
}

float standard_devation(unsigned int number, float y[value],int ary,float s,float x)
{
	x = mean_value(number,y,ary);
	printf("\nThe mean value is %f",x);
	float b[value],sum=0;
    for(ary=0;ary<number;++ary)
    b[ary]=((y[ary]-x)*(y[ary]-x));
     for(ary=0;ary<number;++ary)
     sum += b[ary];
    s= sqrt((sum/(number-1)));
    return(s);
}

float maximum (unsigned int number, float y[value],int ary)
{
	int x;
	float temp;
   for(x=0;x < number ; ++x)
   for(ary=0;ary < number ; ++ary)
    if(y[ary] < y[ary+1])
    {
	 temp = y[ary];
     y[ary]=y[ary+1];
     y[ary+1]=temp;
}
if(y[0]>1e2 && y[0]<1e5)
{
	temp = y[0]/1000;
	printf("\nThe maximum value is %fkOhms",temp);
}else
if(y[0]<0)
{
	temp=y[0]*1000;
	printf("\nThe maximum value is %fmOhms",temp);
}else
if(y[0]>=0&&y[0]<1e2)
{
printf("\nThe maximum value is %f",y[0]);
}else
{
    temp=y[0]/1000000;
	printf("\nThe maximum value is %fMOhms",temp);
}
}

float proccessing (unsigned int number,int ary,float y[value],float x,float s)
{
	char c_in;
    char proccess;
do{
  printf("\nPlease choose one opion that you want to do.\n(1 is find the mean value,2 is find the stand devation,\n 3 is find the maximum value,4 is get all value in once)");
  scanf(" %c",&proccess);
  switch(proccess)
{ case 1 :
  x = mean_value(number,y,ary);
  printf("\nThe mean value of resistance is %f",x);
  break;
  case 2 :
  s = standard_devation(number,y,ary,s,x);
  printf("\nThe standard devation is equal to the value %f",s);
  break;
  case 3 :
  maximum(number,y,ary);
  break;
  case 4:
  x = mean_value(number,y,ary);
  s = standard_devation(number,y,ary,s,x);
  maximum(number,y,ary);
  printf("\nThe mean value of resistance is %f",x);
  printf("\nThe standard devation is equal to the value %f",s);
  break;
  default:
  printf("\nyou type undefined value system stop");
  system("PAUSE");
  return(0);
}
printf("\nIf you want start again the process press Y. If not press any cheractor");
scanf(" %c",&c_in);
}while ((c_in == 'y')||(c_in == 'Y'));
}

int main (void)
{
unsigned char number;
int  chk, ary=0;
float x=0,y[value], s=0;
number = check_value(number,chk);
printf("\nyou typed the number of resistor is %u",number);
take_value(number,y,ary);
proccessing (number,ary,y,x,s);
return(0);
}
