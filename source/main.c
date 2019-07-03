/*	Author: zlian030
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #6  Exercise #3
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
volatile unsigned long _avr_timer_M = 1;
volatile unsigned long _avr_timer_cntcurr = 0;

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

volatile enum States {start, ADD, ADDwait, SUB, SUBwait, wait, reset} state;

volatile unsigned char temp = 0x07;

volatile unsigned char count = 0x00;

volatile void Tick();

volatile int main(void) {
    DDRA = 0x00; PORTA = 0xFF; // Configure port B's 8 pins as inputs
    DDRB = 0xFF; PORTB = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
    state = start;
    TimerSet(300);
    TimerOn();
    while (1) {
	while (!TimerFlag) {};
        TimerFlag = 0;
        Tick();
    }
    return 1;
}


volatile void Tick() {
    switch(state) {
	case start:
	    state = wait;
	    break;
	case ADD:
	    state = ADDwait;
	    break;
	case ADDwait:
	    if ((~PINA & 0x01) && (~PINA & 0x02)) {
		state = reset;
	    }
	    else if (!(~PINA & 0x01) && (~PINA & 0x02)) {
		state = SUB;
	    }
	    else if (!(~PINA & 0x01) && !(~PINA & 0x02)) {
		state = wait;
	    }
	    else {
		if (count < 10) { count++;}
		else {state = ADD;}
	    }
	    break;
	case SUB:
	    state = SUBwait;
	    break;
	case SUBwait:
            if ((~PINA & 0x01) && (~PINA & 0x02)) {
                state = reset;
            }
            else if (!(~PINA & 0x02) && (~PINA & 0x01)) {
                state = ADD;
            }
	    else if (!(~PINA & 0x01) && !(~PINA & 0x02)){
		state = wait;
	    }
            else {
		if (count < 10) { count++;}
                else {state = SUB;}
            }
            break;
        case wait:
            if ((~PINA & 0x01) && (~PINA & 0x02)) {
                state = reset;
            }
            else if (!(~PINA & 0x02) && (~PINA & 0x01)) {
                state = ADD;
            }
	    else if (!(~PINA & 0x01) && (~PINA & 0x02)) {
                state = SUB;
            }
	    else {
		state = wait;
	    } 
            break;
	case reset:
	    state = wait;
	    break;
	default:
	    state = wait;
	    break;
    }

    switch(state) {
        case start:
            break;
        case ADD:
            temp = (temp == 0x09) ? 0x09 : (temp + 0x01);
	    count = 0x00;
            break;
	case ADDwait:
	    break;
        case SUB:
	    temp = (temp == 0x00) ? 0x00 : (temp - 0x01);
            count = 0x00;
	    break;
	case SUBwait:
	    break;
        case wait:
            break;
	case reset:
	    temp = 0x00;
	    break;
        default:
            break;
    }
    
    PORTB = temp;    
}
