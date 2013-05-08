#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include"glob_func.h"
#define chk3b SBIT(TCCR3B,2)

volatile unsigned int steps;

void Pt2Pt()
{
  cli();
	i=0;

do
{
	lcd_init();
	lcd_display("Enter axis: ");
	valEntered=find_key_pressed();

	if(valEntered==1)
	{
		TIMSK |= (1 << OCIE3A); //OCR3A for X-axis
		TCCR1B |= (1 << CS12);
	}
	else if(valEntered==2)
	{
		ETIMSK |= (1 << OCIE3A); //Enable CTC interrupt OCR3B for Y-axis
		TCCR3B |= (1 << CS32);
	}
	else
		DispErr();
}while(valEntered!=1 && valEntered!=2);

do
{
	lcd_init();
	lcd_display("enter distance: ");
	valEntered=find_key_pressed();
	if(valEntered>300)	//1500###############
		DispErr();
}while(valEntered>300);	//1500##############

	steps=(long)valEntered*10000/3925; //valEntered*100##############
	
do
{	lcd_display("enter speed: ");
	valEntered=find_key_pressed();
	if(valEntered>1000 || valEntered<5)	//assume 1 rad/sec is d max speed with which d motor can run
					//n 0.005 rad/sec is d min speed
		DispErr();
}while(valEntered>1000 || valEntered<5);
//speed_cnt=238.671/speed in rad/sec*1000;

speed=238671/valEntered;

	if(valEntered<183) 		//if slow
		{
			Count[0]=speed;
			//speed=1302;
				//i mean speed count
		}
	else//	 if(valEntered<1000)   
		{
			Count[0]=1302;
			//speed=238;
		}
OCR1A=Count[0];
OCR3A=Count[0];
	
ValCount();	

do
{	
	lcd_init();
	lcd_display("enter direction: ");
	valEntered=find_key_pressed();
	if(valEntered!=4 && valEntered!=6)
		DispErr();
}while(valEntered!=4 && valEntered!=6);



lcd_init();
lcd_display("Press 'start'");
		row1=0;
		row0=1;
		row2=1;
		row3=1;
		_delay_ms(1000);
		while(col1!=0);
		

	dispRunMsg();
	TCNT1=0x00;
	TCNT3=0x00;

	sei();	
	
	while(chkT==0);
	TCCR3B &= ~(1<<CS32) ; 
	TCCR1B &= ~(1<<CS12);
	chkT=0;

//	PORTB&=0xF0;
//	PORTD&=0x0F;
	
	_delay_ms(3000);
	TIMSK &= ~(1 << OCIE1A);  
	ETIMSK &= ~(1 << OCIE3A);  
	lcd_init();
		lcd_display("Done");
		_delay_ms(3000);
	cli();
}



void ptTopt_y_ISR()
{
static unsigned char step3=0;

	ySequence();

if(Count[i]!=0 && steps!=0)
{
	OCR3A=Count[i];	//####
	i++;
	steps--;
	step3++;  //#####

}	
else if(step3==0)
{
	TCCR3B&=~(1<<CS32);
	chkT=1;
}
else
{
	if(steps>step3)
	{
		OCR3A=speed;
		steps--;

	}
	else
	{
		steps=0;
		i--;
		OCR3A=Count[i];
		step3--;
	}

}

}


void ptTopt_x_ISR()
{
static unsigned char step2=0;

	xSequence();

if(Count[i]!=0 && steps!=0)
{
	OCR1A=Count[i];	//####
	i++;
	steps--;
	step2++;  //#####

}	
else if(step2==0)
{
	TCCR1B&=~(1<<CS12);
	chkT=1;
}
else
{
	if(steps>step2)
	{
		OCR1A=speed;
		steps--;

	}
	else
	{
		steps=0;
		i--;
		OCR1A=Count[i];
		step2--;
	}

}

}
