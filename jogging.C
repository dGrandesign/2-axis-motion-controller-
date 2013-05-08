#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include"glob_func.h"
#define chk1b SBIT(TCCR1B,2)


void Jogn()
{
  //for jogging, do not enable int0

	//REMEMBER TO KEEP GLOBAL INTERRUPTS DISABLED UNTIL START OF MOTION
		cli();
		i=0;		
		do
		{
		lcd_init();
		lcd_display("Select Axis:");
		valEntered=find_key_pressed();
		if(valEntered==1)
		{
			TIMSK|=(1<<OCIE1A);		//OCR1A for X-axis
			TCCR1B|=(1<<CS12);
		}
		else if(valEntered==2)
		{
			ETIMSK|=(1<<OCIE3A);		//OCR1B for Y-axis
			TCCR3B|=(1<<CS32);
		}
		else
		{
			DispErr();
		//	goto Opt1;
		}
		}while(valEntered!=1 && valEntered!=2);
	
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
		//	goto Opt2;
		}
	
		OCR1A=speed;
		OCR3A=speed;
	}while(valEntered!=7 && valEntered!=8 && valEntered!=9);
		
		ValCount();
		lcd_init();
//Opt3:	

	do
	{
		lcd_display("Enter Direction:");
		valEntered=find_key_pressed();
		if(valEntered!=4 && valEntered!=6)
		{
			DispErr();
		//	goto Opt3;
		}
	}while(valEntered!=4 && valEntered!=6 );

		lcd_init();
		lcd_display("Press 'start'.other to restart");

		row1=0;
		row0=1;
		row2=1;
		row3=1;
		_delay_ms(1000);
		while(col1!=0);  //(col_and==1);	//a li'l suspicious
		

		dispRunMsg();
		TCNT3=0x00;
		TCNT1=0x00;
		sei();
		
		while(col1==0);

		row1=0;
		row0=0;
		row2=0;
		row3=0;
		chkT=1;  //set t bit..start decel
		
		while(chkT==1);
		
		chkT=0;
	//	PORTB&=0xF0;
	//	PORTD&=0x0F;
		TCCR1B &= ~(1<<CS12) ; 
		TIMSK &= ~(1 << OCIE1A); 

		TCCR3B &= ~(1<<CS32) ; 
		ETIMSK &= ~(1 << OCIE3A); 
			_delay_ms(3000);
		lcd_init();
		lcd_display("Stopped");
		_delay_ms(3000);
	
cli();
}


void ValCount()
{
	//unsigned int i=1;
	i=1;
	Count[1]=Count[0]-(2*Count[0])/5;
	Count[1]=(long)Count[1]*676/1000;
	while(Count[i]>=speed)
	{
		i++;
		Count[i]=Count[i-1]-(long)(2*Count[i-1])/(4*i+1);
		if(Count[i]==Count[i-2])
		{
			Count[i]-=2;
		}
	}
	Count[i]=speed;
	Count[++i]=0x0000;
	i=0;	
}


void Jogn_x_ISR()
{
	xSequence();

if(chkT==0)
{
	if(Count[i]!=0)
	{
	OCR1A=Count[i];
	i++;
	}
	else
		OCR1A=speed;
}
else
	{
	i--;
	if(i==0)
	{
		TCCR1B&=~(1<<CS12);
		TIMSK&=~(1<<OCIE1A);
	}
	else
	{
		
		OCR1A=Count[i];
	
	}

}

}

void Jogn_y_ISR()
{

	ySequence();

if(chkT==0)
{
	if(Count[i]!=0)
	{
	OCR3A=Count[i];
	i++;
	}
	else
		OCR3A=speed;
}
else
	{
	
	i--;
	if(i==0)
	{
		TCCR3B&=~(1<<CS32);
		ETIMSK&=~(1<<OCIE3A);
	}
	else
	{
		
		OCR3A=Count[i];
	
	}


}
}
