#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<math.h>
#include"glob_func.h"

volatile char fxy;

#define signBit SBIT(fxy,7)
volatile unsigned int BiggerSteps;
volatile unsigned char x3,y3;

void Interpoln()
{
  
	cli();
	
	do
	{
	lcd_init();
	lcd_display("Enter end pt:"); //value as(x mm, y mm)
	_delay_ms(2000);
	lcd_init();
	lcd_display("X:");
	x3=find_key_pressed();
	lcd_init();
	lcd_display("Y:");
	y3=find_key_pressed();
	if(x3>300 || y3>300)
	{
	DispErr();
	}
	}while(x3>300 || y3>300);


fxy=x3-y3;	
if(fxy>=0)
	BiggerSteps=(long)x3*10000/3925;
else
	BiggerSteps=(long)y3*10000/3925;

/*	do
	{	
	lcd_init();
	lcd_display("enter direction: ");
	valEntered=find_key_pressed();
	if(valEntered!=4 && valEntered!=6)
		DispErr();
	}while(valEntered!=4 && valEntered!=6);

*/
	
	do
	{
		lcd_init();
		lcd_display("Enter speed:"); //slow/med/rapid
		valEntered=find_key_pressed();

		//speed=238671/valEntered;
		if(valEntered==7) 		//if slow
		{
			Count[0]=SLOW_ACCEL;
			speed=SLOW_SPEED;
				//i mean speed count
		}
		else if(valEntered==8)  //if med
		{
			Count[0]=MED_ACCEL;
			speed=MED_SPEED;		//i mean speed count
		}
		else if(valEntered==9)   //if rapid
		{
			Count[0]=FAST_ACCEL;
			speed=FAST_SPEED;
		}
		else
		{
			DispErr();
	
		}
	
		
	}while(valEntered!=7 && valEntered!=8 && valEntered!=9);
		
valEntered=4;
OCR1A=speed;

	
ValCount();
	
lcd_init();
lcd_display("Press 'start'");
		row1=0;
		row0=1;
		row2=0;
		row3=1;
		_delay_ms(100);
		while(col1!=0);
		row2=1;
		
	if(col1==0)
	{
	dispRunMsg();
	TIMSK |= (1 << OCIE1A);
	sei();
	chkT=0;
	TCCR1B|= (1<<CS12);
	row0=0;
	row3=0;	
	row2=0;
	
	while(chkT==0);

	}
	
	chkT=0;
	//PORTB&=0xF0;
	PORTD&=0x0F;
	TCCR1B &= ~(1<<CS12) ; 
	TIMSK &= ~(1 << OCIE1A);  
  		_delay_ms(3000);
	lcd_init();
		lcd_display("Done");
		_delay_ms(3000);

cli();
}	


//ISR(TIMER1_COMPC_vect)
void Interpol_ISR()
{
static unsigned char step2=0;
OCR1A=Count[i];

if(BiggerSteps>0)
{


		if(signBit==0)
			{

				xSequence();
						
		//		xSteps--;
				fxy=fxy-y3; 
			}
		if(signBit==1)
			{
				
			ySequence();

			//	ySteps--;
				fxy=fxy+x3;
			}

if( Count[i]!=0 && BiggerSteps>step2  )
{

	OCR1A=Count[i];	//####
//	if(i!=0)
	step2++; 
	i++;
	BiggerSteps--;
	 //#####
	//steps--;
	

}	
else if(step2==0)
{
//	TCCR1B&=~(1<<CS12);
	chkT=1;
}
else
{
	if( BiggerSteps>=step2 )
	{
		OCR1A=speed;
		BiggerSteps--;

	}
	else
	{
		BiggerSteps--;;
		i--;
		OCR1A=Count[i];
		step2--;
	}

}
}

else
	//TCCR1B&=~(1<<CS12);	
	chkT=1;

}
