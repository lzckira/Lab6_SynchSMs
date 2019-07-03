/*	Author: zlian030
*       Partner(s) Name: 
*	Lab Section:
*	Assignment: Lab #6  Exercise #1
*	Exercise Description: [optional - include for your own benefit]
*
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum States {start, LED1, LED2, LED3} state;
/*unsigned char temp = 0x00;
unsigned char temp2 = 0x00;*/
void Tick();

int main(void) {

//  DDRA = 0x00; PORTA = 0xFF; // Configure port B's 8 pins as inputs
    DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
    state = start;
    TimerSet(125);
    TimerOn();
    while (1) {
	Tick();
	while (!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}


void Tick() {
    switch(state) {
	case start:
	    state = LED1;
	    PORTB = 0x01;
	    break;
	case LED1:
	    state = LED2;
	    PORTB = 0x01;
	    break;
	case LED2:
	    state = LED3;
	    PORTB = 0x02;
	    break;
	case LED3:
	    state = LED1;
	    PORTB = 0x04;
	    break;
	default:
		break;
		    
	}
}
