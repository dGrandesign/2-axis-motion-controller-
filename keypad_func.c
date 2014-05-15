//Made by Trupti Pomaje

#include <avr/io.h> 
#include <util/delay.h>
//#include "lcd_functions.c"
#include"glob_func.h"



//void DispErr();
void check_keys_pressed();
char find_column();
char find_row();
void display_key_pressed(int column,int row);



/*
#define rs SBIT(PORTC,1)
#define rw SBIT(PORTC,6)
#define e SBIT(PORTC,7)
#define busy SBIT(PORTA,7)
#define sbusy SBIT(PINA,7) 


#define row0 SBIT(PORTD,1)
#define row1 SBIT(PORTD,0)
#define row2 SBIT(PORTD,2)
#define row3 SBIT(PORTD,3)

#define col0 SBIT(PIND,4)
#define col1 SBIT(PIND,5)
#define col2 SBIT(PIND,6)
*/



#define lcddata PORTF
#define rs SBIT(PORTC,7)
#define rw SBIT(PORTC,6)
#define e SBIT(PORTC,5)
//#define busy SBIT(PIND,7) 



 void dispRunMsg()
 {
   lcd_init();
	_delay_us(50);
	lcd_display("Running");
	_delay_us(50);
	lcd_display(".");
	_delay_us(50);
	lcd_display(".");
	_delay_us(50);
	lcd_display(".");
 
 }


void DispErr()
{
	lcd_display("Enter valid option!");
	_delay_ms(2000);

}


void check_keys_pressed()
{
     while(1)
  		{
		  if((col_and)==0)
		  break;
		}
     _delay_ms(200);
	
	if(col_and==1)
		 check_keys_pressed();
}
char find_column()
{
	unsigned char column;
   while(1)
   {
	if(col0==0)
	{
	 column=0;
	 break;
	}
	if(col1==0)
	{
	 column=1;
	 break;
	}
	else
	 column=2;
	 break;
   }
  
  return column;
}

char find_row()
{
  	unsigned char row;
   while(1)
   {	
	row0=1;
	row2=row3=0;
	if(col_and==1)
	{
    row=0;
	break;
	}	
	
	row1=1;
	row0=0;
	if(col_and==1)
	{
	  row=1;
	  break;
	}
	
	row2=1;
	row1=0;
	if(col_and==1)
	{
	  row=2;
	  break;
	}
    else
	  
	  row=3;
	  break;
   }	
  row0=row1=row2=row3=0;//------------------------------------

return row;
}

long find_key_pressed()
{

	unsigned char column,row;
	unsigned char key[4][3]={{1,2,3},
						     {4,5,6},
						   	 {7,8,9},
						   	 {10,0,0xa0}};
	unsigned long a; 
	static long value;
	value=0;
	while(1)
	{
	   //	PORTD&=~(1<<0)|(1<<1)|(1<<2)|(1<<3);
	   check_keys_pressed();
	 	
		column=find_column();
	    row=find_row();
	 
               
		
          if(column==0&&row==3)
		 			break;
             
	     
	     
	     if(column==2&&row==3)
	     {  
           value=(value/10);
		   cmd(0x10);
		   
		   display_key_pressed(column,row);
           cmd(0x10);
		  
	     }
		else
		 
		 {
		     display_key_pressed(column,row);
		 	 
			 a=key[row][column];
		  	
			value=value*10+a;
		 //PORTB=value;
		 }

		

	}

	return value;
}


//-----------------------------lcd----------------------------------

void lcd_init()
{
	
	
	cmd(0x38);//check whether to use one line
	cmd(0x0E);//disply on,cursor on,blink of cursor position chara
	cmd(0x01);//clear display & sets ram addrs 0 in add counter
	cmd(0x06);//for incrementing cursor position
	cmd(0x80);
}

void lcd_menu_display()
{
	cmd(0x80);
	data("1:Pnt2Pnt");
	cmd(0x8A);
	data("4.EGearn");
	cmd(0xC0);
	data("2:jogging");
	cmd(0x90);
	data("3:interpolation");
	cmd(0xD0);
	data("Enter: ");
	//cmd(0x90);
	return;//-----------------------------
}

void lcd_display(unsigned char a[])
{
	//cmd(0x80);
    data(a);
	cmd(0xD0);
	return;
}


void display_key_pressed(int column,int row)
{
	unsigned char key[4][3]={{'1','2','3'},
						 {'4','5','6'},
						 {'7','8','9'},
						 {' ','0',' '}};
	//keydata(key[row][column]);
 //   lcd_ready();
	lcddata=key[row][column];
	_delay_us(3);
	rs=1;
	_delay_us(3);
	rw=0;
	_delay_us(3);
	e=1;
	_delay_ms(1);
	e=0;
    _delay_us(3);
	return;
}

void cmd(unsigned char command)
{
 //	lcd_ready();
	lcddata=command;
	//_delay_us(3);
	rs=0;
	//_delay_us(3);
	rw=0;
	//_delay_us(3);

	e=1;
	_delay_ms(1);
	e=0;
	return;
}

void data(unsigned char a[])
{
	unsigned char i=0;
//	lcd_ready();
	while(a[i]!=0)
	{
		lcddata=a[i];
		//_delay_us(3);
		rs=1;
		//_delay_us(3);
		rw=0;
		//_delay_us(3);
		e=1;
		_delay_ms(1);
		e=0;
		i++;
	}
	return;
}




