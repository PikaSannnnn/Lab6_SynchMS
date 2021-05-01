/*	Author: Stephen Dong
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: Demo: https://www.youtube.com/watch?v=H1mWpErM-CY
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

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
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

enum LIGHT {INIT, UP, DOWN, LIT, LIT_WAIT} L_STATE = INIT;
unsigned char LightSys(unsigned char button, unsigned char tmpB) {
	static unsigned char rst_hold = 0x00;
	switch(L_STATE) {
		case INIT:
			break;
		case UP:
			if (!button && rst_hold) {
				rst_hold = 0x00;	 		
			}
			if (button && !rst_hold) {
				L_STATE = LIT;
			}
			else if (tmpB & 0x04) {	// 3rd light is on
				L_STATE = DOWN;			
			}
			else {
				L_STATE = UP;
			}
			break;
		case DOWN:
			if (!button && rst_hold) {
				rst_hold = 0x00;			
			}
			if (button && !rst_hold) {
				L_STATE = LIT;
			}
			else if (tmpB & 0x01) {	// 1st light is on
				L_STATE = UP;			
			}
			else {
				L_STATE = DOWN;
			}
			break;
		case LIT:
			if (!button) {
				L_STATE = LIT_WAIT;			
			}
			break;
		case LIT_WAIT:
			if (button) {
				L_STATE = INIT;
				rst_hold = 0xFF;
			}
			break;
	}

	switch(L_STATE) {
		case INIT:
			L_STATE = UP;
			return 0x01;
		case UP:
			return (tmpB << 1);
		case DOWN:
			return (tmpB >> 1);
		case LIT:
			return tmpB;
		case LIT_WAIT:
			return tmpB;
	}

	return tmpB;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(300);
	TimerOn();
	unsigned char button = 0x00;
	unsigned char tmpB = 0x00;
    /* Insert your solution below */
	while (1) {
		button = ~PINA & 0x01;
		tmpB = LightSys(button, tmpB);
		PORTB = tmpB;
		while (!TimerFlag);
		TimerFlag = 0;
	}
}
