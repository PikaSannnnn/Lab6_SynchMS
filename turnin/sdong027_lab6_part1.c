/*	Author: sdong027
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: 
 *	Demo Link: https://www.youtube.com/watch?v=pZ-tNajHszk	
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

enum LIGHT {INIT, ONE, TWO, THREE} L_STATE = INIT;
unsigned char LightSys() {
	switch(L_STATE) {
		case INIT:
			L_STATE = ONE;
			break;
		case ONE:
			L_STATE = TWO;
			break;
		case TWO:
			L_STATE = THREE;
			break;
		case THREE:
			L_STATE = ONE;
			break;
	}

	switch(L_STATE) {
		case ONE:
			return 0x01;
		case TWO:
			return 0x02;
		case THREE:
			return 0x04;
	}

	return 0x00;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF;
	PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
	//unsigned char tmpB = 0x00;
    /* Insert your solution below */
	while (1) {
		PORTB = LightSys();
		while (!TimerFlag);
		TimerFlag = 0;
	}
}
