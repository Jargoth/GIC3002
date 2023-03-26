/*
 * GIC3002.c
 *
 * Created: 2015-02-10 09:15:59
 *  Author: Jargoth
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 8000000UL

// Define baud rate
#define USART_BAUDRATE 38400
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

volatile int data[16];
volatile int col;
volatile int next_col = 0;
volatile int recieved = 0;

void PORTS_init(void)
{
	//initialize ports
	DDRA = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xC0;
}

void USART_Init(void){
   // Set baud rate
   UBRRL = BAUD_PRESCALE;// Load lower 8-bits into the low byte of the UBRR register
   UBRRH = (BAUD_PRESCALE >> 8); 
	 /* Load upper 8-bits into the high byte of the UBRR register
    Default frame format is 8 data bits, no parity, 1 stop bit
  to change use UCSRC, see AVR datasheet*/ 

  // Enable receiver and receive complete interrupt 
  UCSRB = ((1<<RXEN) | (1<<RXCIE));
}

void set_initial_data(void)
{
	data[0] = 0x00;
	data[1] = 0x00;
	data[2] = 0x00;
	data[3] = 0x00;
	data[4] = 0x00;
	data[5] = 0x00;
	data[6] = 0x00;
	data[7] = 0x00;
	data[8] = 0x00;
	data[9] = 0x00;
	data[10] = 0x00;
	data[11] = 0x00;
	data[12] = 0x00;
	data[13] = 0x00;
	data[14] = 0x00;
	data[15] = 0x00;
	data[16] = 0x00;
	//F
	data[17] = 0x7E;
	data[18] = 0x0A;
	data[19] = 0x02;
	data[20] = 0x00;
	//R
	data[21] = 0x7E;
	data[22] = 0x1A;
	data[23] = 0x64;
	data[24] = 0x00;
	//E
	data[25] = 0x7E;
	data[26] = 0x4A;
	data[27] = 0x42;
	data[28] = 0x00;
	//D
	data[29] = 0x7E;
	data[30] = 0x42;
	data[31] = 0x3C;
	data[32] = 0x00;
	//R
	data[33] = 0x7E;
	data[34] = 0x1A;
	data[35] = 0x64;
	data[36] = 0x00;
	//I
	data[37] = 0x7E;
	data[38] = 0x00;
	//K
	data[39] = 0x7E;
	data[40] = 0x18;
	data[41] = 0x66;
	
	data[42] = 0x00;
	data[43] = 0x00;
	data[44] = 0x00;
	data[45] = 0x00;
	data[46] = 0x00;
	data[47] = 0x00;
	data[48] = 0x00;
	data[49] = 0x00;
	data[50] = 0x00;
	data[51] = 0x00;
	data[52] = 0x00;
	data[53] = 0x00;
	data[54] = 0x00;
	data[55] = 0x00;
	data[56] = 0x00;
	data[57] = 0x00;
	col = 0x00;
}


void serialize_data(int out)
{
	int i = 0;
	while (i < 8)
	{
		
		PORTD = 0x00;
		PORTC = out; //skicka ut MSB (PORTC7)
		out = out << 1;//skifta vänster
		PORTD = 0x80;
		i++;
	}
}

void load_screen(void)
{
	data[0] = 0x00;
	data[1] = 0x00;
	data[2] = 0x00;
	data[3] = 0x00;
	data[4] = 0x00;
	data[5] = 0x00;
	data[6] = 0x00;
	data[7] = 0x00;
	data[8] = 0x00;
	data[9] = 0x00;
	data[10] = 0x00;
	data[11] = 0x00;
	data[12] = 0x00;
	data[13] = 0x00;
	data[14] = 0x00;
	data[15] = 0x00;
	data[16] = 0x00;
	//L
	data[17] = 0x7E;
	data[18] = 0x40;
	data[19] = 0x40;
	data[20] = 0x00;
	//O
	data[21] = 0x3C;
	data[22] = 0x42;
	data[23] = 0x3C;
	data[24] = 0x00;
	//A
	data[25] = 0x7C;
	data[26] = 0x12;
	data[27] = 0x7C;
	data[28] = 0x00;
	//D
	data[29] = 0x7E;
	data[30] = 0x42;
	data[31] = 0x3C;
	data[32] = 0x00;
	//I
	data[33] = 0x7E;
	data[34] = 0x00;
	//N
	data[35] = 0x7E;
	data[36] = 0x0C;
	data[37] = 0x30;
	data[38] = 0x7E;
	data[39] = 0x00;
	//G
	data[40] = 0x3C;
	data[41] = 0x52;
	data[42] = 0x34;
	
	data[43] = 0x00;
	data[44] = 0x00;
	data[45] = 0x00;
	data[46] = 0x00;
	data[47] = 0x00;
	data[48] = 0x00;
	data[49] = 0x00;
	data[50] = 0x00;
	data[51] = 0x00;
	data[52] = 0x00;
	data[53] = 0x00;
	data[54] = 0x00;
	data[55] = 0x00;
	data[56] = 0x00;
	data[57] = 0x00;
	data[58] = 0x00;
	col = 0x00;
	
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	int utdata[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int timer = 16000;
	int timer2 = 12000;
	while(k < 16)
	{
		while (timer)
		{
			serialize_data(utdata[i] ^ data[i+j]);
			PORTD = 0x40;
			PORTA = i;
			PORTD = 0x00;
			i++;
			if (i == 16)
			{
				i = 0;
			}
			timer--;
			timer2--;
			if (timer2 == 0)
			{
				utdata[k] = utdata[k]*2 + 1;
				timer2 =12000;
				l++;
				if (l==8)
				{
					l = 0;
					k++;
				}
			}
		}
		timer = 16000;
		j++;
		if (j == 42)
		{
			j = 0;
		}
	}
}


ISR(USART_RX_vect)
{
	int temp;
	temp = UDR;
	_delay_ms(5);
	data[temp] = UDR;
/*	while (next_col < 16)
	{
		_delay_ms(100);
		data[next_col] = UDR;             //read UART register into value
		next_col++;
	}*/
	next_col = 0;
	recieved = 1;
}


int main(void)
{
	PORTS_init();
	USART_Init();
	load_screen();
	set_initial_data();
	sei();
	int i = 0;
	int j = 0;
	int timer = 16000;
    while(1)
    {
		while (timer)
		{
			serialize_data(data[i+j]);
			PORTD = 0x40;
			PORTD = 0x00;
			PORTA = i;
			i++;
			if (i == 16)
			{
				i = 0;
			}
			timer --;
		}
		j++;
		if (j == 41)
		{
			j = 0;
		}
		timer = 16000;
		if (recieved)
		{
			j = 0;
		}
    }
	return 0;
}