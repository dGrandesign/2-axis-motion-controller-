#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include"glob_func.h"

volatile unsigned char stepsToMatch;
volatile unsigned int GRatio;
#define fracSteps OCR1B   
#define fracStepsStored OCR3B     	//        <------
#define matchErr SBIT(OCR2,0)	//        <-------

void EGearn()
{

do
{
	lcd_init();
	lcd_display("Master SPEED(Hz):");
	valEntered=find_key_pressed();		
	if(valEntered>1000)			//MAX SPEED for EGEARING is 1000Hz
		DispErr();
}while(valEntered>1000);
speed=valEntered;

speed=100;


do
{
	lcd_init();
	lcd_display("Gearing Ratio? :");	
	cmd(0xC0);
//	delay_us(6);
	lcd_display("(M/S * 1000)");
	valEntered=find_key_pressed();
	if(valEntered>5000 || valEntered<200)
		DispErr();

}while(valEntered>5000 || valEntered<200);


GRatio=valEntered;

//GRatio=2500;

if(GRatio>=1000)
{
	stepsToMatch=GRatio/1000;
	fracSteps=GRatio-stepsToMatch*1000;			//<-------------
}
else
{
	stepsToMatch=1000/GRatio;
	speed=speed*stepsToMatch;
	fracSteps=(1000000/GRatio)-(stepsToMatch*1000);//<-----------
}

if(speed>100)
	Count[0]=196;
else if(speed>50)
	Count[0]=253;
else
	Count[0]=1000000/(long)(256*speed);

speed=1000000/(long)(256*speed);
OCR3A=speed;
ValCount();
	
lcd_init();
lcd_display("Press 'start'");
		row1=0;
		row0=1;
		row2=0;
		row3=1;
		_delay_ms(1000);
		while(col1!=0);
		row2=1;

	if(col1==0)
	{
	dispRunMsg();
	ETIMSK |= (1 << OCIE3A);
	sei();
	TCCR3B|=(1<<CS32);
	row1=1;
	row3=0;	
	
	while(col1!=0);
	chkT=1;
	while(chkT==1);

	}

	PORTB&=0xF0;
	PORTD&=0x0F;

	TCCR3B &= ~(1<<CS32) ; 
	ETIMSK&=~(1<<OCIE3A);
    	_delay_ms(3000);
	lcd_init();
		lcd_display("Done");
		_delay_ms(3000);
	chkT=0;

}



//ISR(TIMER3_COMPC_vect)
void EGearn_ISR()
{

		static unsigned char stepCount=0;

	
	//-------for master motor--------
	
	
	if(GRatio>=1000 || (GRatio<1000 && stepCount==(stepsToMatch+matchErr))) //matchErr
	{
		xSequence();
		
		if(GRatio>=1000)
		{
			stepCount++;
			if(chkT==1)		
			{
				if(i!=0)		//To decelerate
				{
					i--;
					OCR3A=Count[i];
		
				}
				else				//To stop
					chkT=0;
			
			}
			
			
			else if(Count[i]!=0)	//To accelerate
			{
				OCR3A=Count[i];
				i++;
			}
			else					//To slew
				OCR3A=speed;
		}
		else
			
			fracSteps+=fracStepsStored;	// <----from here
			if(fracSteps>999)
			{
				matchErr=1;
				fracSteps=fracSteps-1000;
			}
			else
			{
				//	fracSteps+=fracStepsStored;
				matchErr=0;
			}			// <------to here
		stepCount=0;
	}
	//--------master end--------------

	//-------for slave----------------
	if(GRatio<1000 || (GRatio>1000 && stepCount==(stepsToMatch+matchErr)))  //matchErr
	{
		ySequence();

		if(GRatio<1000)
		{
				stepCount++;
				if(chkT==1)
			{
				if(i!=0)		//To decelerate
				{
					i--;
					OCR3A=Count[i];
		
				}
				else			//To stop
					chkT=0;
			
			}
			
			
			else if(Count[i]!=0)	//To accelerate
			{
				OCR3A=Count[i];
				i++;
			}
			else					//To slew
				OCR3A=speed;
		}
		
		else
			
			fracSteps+=fracStepsStored;		//  <-----from here
			if(fracSteps>999)
			{
					matchErr=1;
					fracSteps=fracSteps-1000;
			}
			else
			{
				matchErr=0;				//  <----to here
					//fracSteps+=fracStepsStored;
			}
			stepCount=0;
	}	

	//---------slave end------------
}
