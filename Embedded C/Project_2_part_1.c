/*
	Project 2 
	John Gleeson - 12162213
	
	Program for ATMega328P (Arduino) to control a motor via keyboard commands over a serial interface.
*/

#include <ctype.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#define F_CPU 16000000L

unsigned char qcntr = 0,sndcntr = 0;   /*indexes into the queue*/
unsigned char queue[50];       /*character queue*/

unsigned int adc_value;
volatile int pollingFlag;
unsigned long int adc_mv;
volatile int trigger;
volatile long totalArray = 0;
volatile int adc_value_flag;
const int numberOFReadings = 10;
const int ADCThreshold = 409;

int readIndex = 0;
volatile long average = 0;
long resolution = 5000/1023;

void initialiseTimer0 (void);
void initialiseTimer2 (void);
void initialiseUSART (void);
void initialiseADC(void);
void continuouslyPoll(void);
void arrayFunction(void);


/*message arrays*/
char message[80];

/*this function loads the queue and */
/*starts the sending process*/
void sendmsg (char *s)
{
	qcntr = 0;    /*preset indices*/
	sndcntr = 1;  /*set to one because first character already sent*/
	queue[qcntr++] = 0x0d;   /*put CRLF into the queue first*/
	queue[qcntr++] = 0x0a;
	while (*s) queue[qcntr++] = *s++;   /*put characters into queue*/
	UDR0 = queue[0];  /*send first character to start process*/
}

int main(void)
{
	DDRD = 0b10001000;
	DDRB = 0b00010010;
	PORTD = 0b10000000; //enable pull-up resistor for PD7
	
	unsigned int adcArray[10] = {0};
	
	totalArray = 0;
	initialiseTimer0();
	initialiseTimer2();
	initialiseUSART();
	initialiseADC();
	
	char ch;  /* character variable for received character*/
	
	sei(); /*global interrupt enable */

	while (1)
	{
		if(adc_value_flag == 1){
			
			if(readIndex >= numberOFReadings){
				readIndex =0; // go back to the start
			}

			adcArray[readIndex] = adc_value;				// read the new result into an array
			totalArray = totalArray +  adcArray[readIndex]; // add the reading to totalArray
			readIndex = readIndex + 1;					    // go to the next position in the array
			
			// if we're at the end of the array
			if(readIndex >= numberOFReadings){
				average = totalArray / numberOFReadings;
				totalArray = 0;
				
				// if the average ADC value is over the threshold value
				if (average >= ADCThreshold){
					
					PORTD = PORTD | (1<<PD7);	// toggle the led
					PORTB = (1<<PORTB1);		// turn on the brake
					OCR2B = 0;
				}
			}
			
			// while the continuous trigger is enabled, send the ADC mV value to the queue
			while (trigger == 1 && pollingFlag  == 1){
				
				adc_mv = (resolution *  adc_value) ;
				sprintf(message, "Voltage = %ldmV",adc_mv);
				if(qcntr == sndcntr){
					sendmsg(message);
				}
				pollingFlag = 0;
			}
			adc_value_flag = 0; 	// set the ADC flag to zero
		}
		
		if (UCSR0A & 0x80) /*check for character received*/
		{
			unsigned int convertChar;
			ch = UDR0;    /*get character sent from PC*/
			ch = toupper(ch);
			switch (ch)
			{
				case 'F': // Forward
				PORTB = (1<<PORTB4);
				break;
				
				case 'R': //Reverse
				PORTB = (0<<PORTB4);
				break;
				
				case 'B': //Brake
				PORTB = (1<<PORTB1);
				sprintf(message, "Motor Stopped.");
				sendmsg(message);
				break;
				
				case 'S': //Start motor
				PORTB = (0<<PORTB1);
				OCR2B = 25;
				sprintf(message, "Motor Started at 10%%.");
				sendmsg(message);
				break;
				
				case 'A': //Report ADC in hex
				sprintf(message, "ADC value in hex is: 0x%x",adc_value);
				sendmsg(message);
				break;
				
				case 'V': //Report ADC in millivolts
				adc_mv =  resolution * adc_value ;
				sprintf(message, "ADC voltage is: %ldmV",adc_mv);
				sendmsg(message);
				break;
				
				case 'C': //Continuously report ADC in millivolts
				trigger = 1;
				break;
				
				case 'E': //Stop continuously reporting ADC in millivolts
				trigger = 0;
				break;
				
				case 'M': //Report motor direction
				if ((PORTB  & 0b00010000)==0){
					sprintf(message, "Motor is rotating in reverse.");
					} else {
					sprintf(message, "Motor is rotating forward.");
				}
				sendmsg(message);
				break;
				
				case 'O': //Report OCR2B
				sprintf(message, "OCR2B value is: %d",OCR2B);
				sendmsg(message);
				break;
				
				case 'G': //Report average of last 10 ADC readings
				sprintf(message, "Average of last 10 ADC values = %ld",average);
				sendmsg(message);
				break;
				
				case '0' : case '1' : case '2' : case '3' : case '4' : case '5' : case '6' : case '7' : case '8' : case '9' :
				convertChar = ch - '0';
				OCR2B = 25 * convertChar;
				if (convertChar != 0){
					sprintf(message, "Speed is set to %d0%% ",convertChar);
					
					} else{
					sprintf(message, "Speed is set to %d%% ",convertChar);
				}
				sendmsg(message);
				break;
				
				default:
				break;
			}
		}
	}
}

void initialiseTimer0(void){
	TCCR0A = 0;    // Disable timer outputs
	TCCR0B = (5<<CS00);  // Set T0 Source = Clock (16MHz)/1024 and put Timer in Normal mode
	OCR0A = 0;
	TCNT0 = 99;    // Recall: 256-99 = 157 & 157*64us = 10ms, approx 10ms
	TIMSK0 = (0<<TOIE0); // Enable Timer 0 overflow interrupt disabled
}

void initialiseTimer2(void){
	TCCR2A = ((1<<COM2B1)|(0<<COM2B0)|(1<<WGM20));
	TCCR2B = (7<<CS20); // clock source 1024
	TIMSK2 = 0; //Disable all timer/counter2 interrupts
	TIFR2 = 0; // Disable counter2 int
}

void initialiseUSART(void){
	
	UCSR0A = 0x00;
	UCSR0B = ((1<<RXEN0) | (1<<TXEN0) | (1<<TXCIE0) |(0<<UCSZ02));  //enable receiver, transmitter and transmit interrupt
	UCSR0C = (3<<UCSZ00);
	UBRR0 = 103;  //baud rate = 9600
	
}

void initialiseADC(void){
	ADMUX = ((0<<MUX0) | (1<<REFS0) | (0<<ADLAR) );  //  ADC0 as ADC input, AVCC selected for VREF, ADLAR disabled
	ADCSRA = ((1<<ADEN)|(0<<ADSC) | (1<<ADATE) | (1<<ADIE) | (7<<ADPS0)); /*  ADEN - Enable ADC
																			  ADATE - Enable ADC auto-trigger
																			  ADIE - Enable ADC interrupt
																			  ADPS0 - SET prescaler to 128
																			*/
	ADCSRB = (1<<ADTS2);

	
}
/*this interrupt occurs whenever the */
/*USART has completed sending a character*/
ISR(USART_TX_vect)
{
	/*send next character and increment index*/
	if (qcntr != sndcntr)  UDR0 = queue[sndcntr++];
}

ISR(ADC_vect){
	
	TCNT0 = 99;
	adc_value = ADC;
	adc_value_flag = 1;
	pollingFlag = 1;
	TIFR0 = 0x01;
}

