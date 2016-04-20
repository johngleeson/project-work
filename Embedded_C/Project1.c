/*
* Project1.c
*
* Created: 09/10/2013 23:37:12
*  Author: Ciaran
*  Modified by John Gleeson 12162213
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#define MEDIUM_THRESHOLD_VOLTAGE 714  // 3.5V threshold for ADC	(1024/5*3.5)
#define LOW_THRESHOLD_VOLTAGE 408     // 2V threshold for ADC (1024/5*2)

unsigned int adc_reading;	/* Defined as a global here. Would any other scope be ok?  */
unsigned int timecount0;
volatile unsigned int multiplier;		// Multiplier to control loop timing in timer0 ISR
volatile unsigned int currentValue;		// Stores current value of PORTD
volatile unsigned int previousValue;	// Stores previous value of PORTD
volatile unsigned int trigger = 1;		// Used to initailise PORTD, currentValue and previousValue
const int ceilingValue = 7;				// The maximum value allowed on PORTD
const int floorValue = 2;				// The minimum value allowed on PORTD

void controlTimer(void);			    // Prototype for controlTimer function
void controlADC(void);					// Prototype for controlADC function

int main(void)
{
	//Set PortD to all outputs because LEDs are connected to this PORT
	DDRD = 0xff;		// 0xff = 0b11111111; all ones
	DDRB = 0x00;		// Set PORTB as inputs
	PORTB = 0x01;		// Pull-up resistor on PB0

	controlTimer();		// Call the controlTimer function
	controlADC();		// Call the controlADC function
	sei();				// Global interrupt enable (I=1)
	
	while(1){
		
		// if the PB0 is 0, PORTD bit 0 is high
		if ((PINB & 0b00000001) == 0) PORTD = PORTD | (1<<0);
		
	}
}

void controlTimer(){
	TCCR0B = (5<<CS00);		// Set T0 Source = Clock (16MHz)/1024 and put Timer in Normal mode
	TCCR0A = 0;				// Disable timer outputs
	OCR0A = 0;
	TCNT0 = 61;				// Recall: 256-61 = 195 & 195*64us = 12.48ms, approx 12.5ms
	TIMSK0 = (1<<TOIE0);	// Enable Timer 0 overflow interrupt enable
	
}

void controlADC(){
	
	ADMUX = ((0<<MUX0) | (1<<REFS0) | (0<<ADLAR) );  //  ADC0 as ADC input, AVCC selected for VREF, ADLAR disabled
	ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIE) | (7<<ADPS0)); /*  ADEN - Enable ADC
																				ADSC - Start ADC conversion 
																				ADATE - Enable ADC auto-trigger
																				ADIE - Enable ADC interrupt
																				ADPS0 - SET prescaler to 128
																			*/
	ADCSRB = (0<<ADTS0); /* Select AutoTrigger Source to Free Running Mode */

}

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 61;						// TCNT0 needs to be set to the start point each time
	++timecount0;					// Count the number of times the interrupt has been reached
	if (timecount0 >= multiplier)	// multiplier is varied depending on ADC. Values are 40, 200 or 400
	{
		// if statement used once to make sure the sequence starts on bit 2
		if(trigger == 1){
			
			previousValue = 1;		  // Making sure that previousValue < currentValue
			currentValue = floorValue;  // Sets currentValue to the 1st LED I want lit
			trigger = 0;			  // Clears the trigger so this if statement isn't used again
		}
		
		//if statement to make the LEDs light from bit 2 -> bit 7
		if (((currentValue < ceilingValue) && (currentValue > previousValue)) || (currentValue == floorValue))	{
			
			PORTD = (1<<currentValue);		//Lights the LED @ currentValue
			previousValue = currentValue;	//Copies currentValue to previousValue
			currentValue++;					//Increments currentValue
			timecount0 = 0;					//Resets the time counter
		}
		//else make the LEDs light from bit 7 -> bit 2
		else {

			PORTD = (1<<currentValue);		//Lights the LED @ currrentValue
			previousValue = currentValue;	//Copies currentValue to previousValue
			currentValue--;					//Decrements currentValue
			timecount0 = 0;					//Resets the time counter
		}
	}
}

ISR (ADC_vect)//handles ADC interrupts
{
	adc_reading = ADC;   // Set ADC to free-running mode

	// if statement to set my multiplier value based on the ADC voltage
	if (adc_reading <= LOW_THRESHOLD_VOLTAGE){

		multiplier = 40; //500ms   (12.5ms * 40)
	}
	else if (((adc_reading <= MEDIUM_THRESHOLD_VOLTAGE) && (adc_reading > (LOW_THRESHOLD_VOLTAGE )))){
		multiplier = 200; //  2500ms	(12.5ms * 200)
	}
	else{
		multiplier = 400; //5000ms		(12.5ms * 400)
	}
}

