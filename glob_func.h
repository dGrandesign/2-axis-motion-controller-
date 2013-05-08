void lcd_display(unsigned char a[]);
void cmd(unsigned char command);
void data(unsigned char a[]);
long find_key_pressed();
void DispErr();
void dispRunMsg();
void Jogn();
void ValCount();
void Pt2Pt();
void Interpoln();
void EGearn();
void ySequence();
void xSequence();
void ptTopt_y_ISR();
void ptTopt_x_ISR();
void Jogn_y_ISR();
void Jogn_x_ISR();
void Interpol_ISR();
void EGearn_ISR();

struct bits { 
  uint8_t b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1; 
} __attribute__((__packed__));

#define SBIT_(port,pin) ((*(volatile struct bits*)&port).b##pin) 
#define   SBIT(x,y)       SBIT_(x,y) 
#define col_and (col0&col1&col2)

#define row1 SBIT(PORTG,0)
#define row0 SBIT(PORTG,1)
#define row2 SBIT(PORTC,0)
#define row3 SBIT(PORTC,1)

#define col0 SBIT(PINB,6)
#define col1 SBIT(PINB,5)
#define col2 SBIT(PINB,4)

#define lcd_clear cmd(0x01)  	


#define chkT SBIT(TCNT2,6)


//-------global variables------------
#define speed OCR3C
#define valEntered OCR1C
volatile unsigned int Count[50]; //,speed;
//volatile unsigned int valEntered;
volatile unsigned int i;
volatile unsigned char MainOption;

//---------speed n accel count--------

#define SLOW_SPEED 390	 
#define MED_SPEED 78
#define FAST_SPEED 39

#define SLOW_ACCEL 390
#define MED_ACCEL 253
#define FAST_ACCEL 253
