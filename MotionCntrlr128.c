#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include"glob_func.h"

/*------for limit switch operation------

---use tcnt 2 store d current position (in terms of steps from limit switch)--
---the store value of max steps(steps to reach home 4m limit sw) in OCRnA---
---use a mode where interrupt is executed on value of OCRnA n not on roll-over-- */


int main()
{

  
	DDRA=0xFF;
	DDRB=0x00;		 //PB4,5,6 -->keypad columns
	DDRC=0b11100011; //PC5,6,7-->LCD cmd ; PC0,1-->keypad rows
	DDRD=0x00;   	 //PD0,1,2,3 --> extra interrupt
	DDRE=0x30;		 //PE4,5 --> limit switches
	DDRF=0xFF;		 //PORTF --> LCD data	
	DDRG=0x07;		 //PG0,1 -->keypad rows ; PG2 --> solenoid

	// ENABLE PULL UPS
	PORTB=0x0F;
	PORTC|=((1<<2)|(1<<3)|(1<<4));
	PORTD=0xFF;
	PORTE=0xCF;
	PORTG=0x18;

	TCCR1B |= (1 << WGM12) ; //setup timer for o/p comp mode 
	TCCR3B |= (1 << WGM32) ; //setup timer for o/p comp mode 
do
{
	
	lcd_init();
	lcd_menu_display();
	MainOption=find_key_pressed();
//	MainOption=4;

	switch(MainOption)
	{
		case 1:
			Pt2Pt();
			break;
		case 2:
			Jogn();
			break;
		case 3:
			Interpoln();
			break;
		case 4:
			EGearn();
			break;
		case 5:
			break;
		default:
			DispErr(); 
				
	}
}while(MainOption!=5);
lcd_clear; //#define lcd_clear as cmd(<woteva>)
	//sleepmode;  after this, only restart shud wrk..
while(1)
{
}

}

void xSequence()
{
static unsigned char jA=0;

if(valEntered==4)
		{
			jA++;
			if(jA>3)
			{
				jA=0;
			//	PORTB&=~(1<<0);
			}
			PORTA=PORTA & 0xF0;
			PORTA|=(1<<jA);
		}
		else
		{
			if(jA==0)
			{
				jA=4;
			//	PORTB&=~(1<<3);
			}	
						jA--;
			PORTA=PORTA & 0xF0;
			PORTA|=(1<<jA);
		}


}

void ySequence()
{

	static unsigned char jB=4;
		if(valEntered==4)
			{
				jB++;
				if(jB>7)
				{
					jB=4;
					//RTD&=~(1<<0);
				}
				PORTA=PORTA & 0x0F;
				PORTA|=(1<<jB);
			}
			else
			{
				if(jB<=4)
				{
					jB=8;
				//	PORTD&=~(1<<0);
				}
				jB--;
				PORTA=PORTA & 0x0F;
				PORTA|=(1<<jB);
			}

}


ISR(TIMER1_COMPA_vect)
{
	switch(MainOption)
	{
	case 1:
		ptTopt_x_ISR();
		break;
	case 2:
		Jogn_x_ISR();
		break;
	case 3:
		Interpol_ISR();
		break;
	
	}
}

ISR(TIMER3_COMPA_vect)
{
	switch(MainOption)
	{
	case 1:
		ptTopt_y_ISR();
		break;
	case 2:
		Jogn_y_ISR();
		break;
	case 4:
		EGearn_ISR();
		break;
	
	}
}
