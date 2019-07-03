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

#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {start, ADD, ADDwait, SUB, SUBwait, wait, reset} state;
unsigned char temp = 0x00;
void Tick();

int main(void) {

DDRA = 0x00; PORTA = 0xFF; // Configure port B's 8 pins as inputs
DDRB = 0xFF; PORTB = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
    state = start;
    while (1) {
	Tick();
    }
    return 1;
}


void Tick() {
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
		state = ADDwait;
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
                state = SUBwait;
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
            break;
		case ADDwait:
			break;
        case SUB:
			temp = (temp == 0x00) ? 0x00 : (temp - 0x01);
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
