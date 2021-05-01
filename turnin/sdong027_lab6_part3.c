/*	Author: sdong027
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #6  Exercise #3
 *	Exercise Description: 
 *	Demo Link: https://www.youtube.com/watch?v=bAcDZ7UDNY0 
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

enum OUT {RST, WAIT, WAIT_RELEASE_INC, WAIT_RELEASE_DEC, INC, DEC, HOLD_INC, HOLD_DEC} OUT_STATE = -1;	// set to default
unsigned char LightSys(unsigned char tmpA, unsigned char tmpB) {
	static unsigned char timer_cnt;
        switch (OUT_STATE) {
            case RST:
                if (!tmpA) {
                    OUT_STATE = WAIT;
                }
                else if ((tmpA & 0x01) && !(tmpA & 0x02)) {
                    OUT_STATE = INC;
                }
                else if ((tmpA & 0x02) && !(tmpA & 0x01)) {
                    OUT_STATE = DEC;
                }
                break;
            case WAIT:
                if ((tmpA & 0x01) && !(tmpA & 0x02)) {
                    OUT_STATE = INC;
                }
                else if ((tmpA & 0x02) && !(tmpA & 0x01)) {
                    OUT_STATE = DEC;
                }
                else if ((tmpA & 0x01) && (tmpA & 0x02)) {
                    OUT_STATE = RST;
                }
                break;
            case WAIT_RELEASE_INC:
		if (tmpA) {		
		    timer_cnt++;
		    	if (timer_cnt == 10) {	// 10 * 100ms
				OUT_STATE = HOLD_INC;
		    	}
		}
                else if (!tmpA) {
                    OUT_STATE = WAIT;
                }
                else if ((tmpA & 0x02) && !(tmpA & 0x01)) {
                    OUT_STATE = DEC;
                }
                else if ((tmpA & 0x01) && (tmpA & 0x02)) {
                    OUT_STATE = RST;
                }
                break;
            case WAIT_RELEASE_DEC:
                if (tmpA) {		
		    	timer_cnt++;
		    	if (timer_cnt == 10) {	// 10 * 100ms
		    		OUT_STATE = HOLD_DEC;
		    	}
		}
                else if (!tmpA) {
                    OUT_STATE = WAIT;
                }
                else if ((tmpA & 0x01) && !(tmpA & 0x02)) {
                    OUT_STATE = INC;
                }
                else if ((tmpA & 0x01) && (tmpA & 0x02)) {
                    OUT_STATE = RST;
                }
                break;
            case INC:
                if ((tmpA & 0x01) && !(tmpA & 0x02)) {
                    OUT_STATE = WAIT_RELEASE_INC;
                }
                else if ((tmpA & 0x02) && !(tmpA & 0x01)) {
                    OUT_STATE = DEC;
                }
                else if ((tmpA & 0x01) && (tmpA & 0x02)) {
                    OUT_STATE = RST;
                }
                break;
            case DEC:
                if ((tmpA & 0x01) && !(tmpA & 0x02)) {
                    OUT_STATE = INC;
                }
                else if ((tmpA & 0x02) && !(tmpA & 0x01)) {
                    OUT_STATE = WAIT_RELEASE_DEC;
                }
                else if ((tmpA & 0x01) && (tmpA & 0x02)) {
                    OUT_STATE = RST;
                }
                break;
	    case HOLD_INC:
		timer_cnt++;
		if (!tmpA) {
		    	OUT_STATE = WAIT;
			timer_cnt = 0x00;
		}
		break;
	    case HOLD_DEC:
		timer_cnt++;
		if (!tmpA) {
		    	OUT_STATE = WAIT;
			timer_cnt = 0x00;
		}
		break;
            default:
                tmpB = 0x07;
		timer_cnt = 0x00;
                OUT_STATE = WAIT;
                break;
        }

        switch (OUT_STATE) {
            case RST:
                tmpB = 0x00;
                break;
            case WAIT:
                break;
            case WAIT_RELEASE_INC:
                break;
            case WAIT_RELEASE_DEC:
                break;
            case INC:
                if (tmpB < 9) {
                    tmpB++;
                }
                break;
            case DEC:
                if (tmpB > 0) {
                    	tmpB--;
                }
                break;
	    case HOLD_INC:
                if ((tmpB < 9) && (timer_cnt == 10)) {
                    	tmpB++;
		    	timer_cnt = 0x00;
                }
                break;
            case HOLD_DEC:
                if ((tmpB > 0) && (timer_cnt == 10)) {
                    	tmpB--;
		    	timer_cnt = 0x00;
                }
                break;
        }

	return tmpB;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(100);
	TimerOn();
	unsigned char button = 0x00;
	unsigned char tmpB = 0x00;
    /* Insert your solution below */
	while (1) {
		button = ~PINA & 0x03;
		tmpB = LightSys(button, tmpB);
		PORTB = tmpB;
		while (!TimerFlag);
		TimerFlag = 0;
	}
}
