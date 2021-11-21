/*	Author: Diego Rivera
 *	Lab Section: 21
 *	Assignment: Final Project Demo 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: 
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h" 
#endif

typedef struct task { 
  int state; 
  unsigned long period; 
  unsigned long elapsedTime; 
  int (*TickFct)(int);     
} task; 


task tasks[1]; 
const unsigned long tasksPeriodGCD = 200; 
const unsigned long LED = 200;
const unsigned long roundPeriod = 200;

void TimerISR() {
	unsigned char i; 
	for(i = 0; i < 2; ++i){ 
        if(tasks[i].elapsedTime >= tasks[i].period){         
	tasks[i].state = tasks[i].TickFct(tasks[i].state);            
	tasks[i].elapsedTime = 0; 
      } 
        tasks[i].elapsedTime += tasksPeriodGCD; 
   } 
}

unsigned char row[20] = {8, 7,6,5,4,3,2,1,0,0,0,0,8,7,6,5,4,3,2,1};
unsigned char column[16] = {0,2,0,7,0,4,2,4,2,7,3,5,3,4,6,7};
unsigned char r = 0;
unsigned char c = 0;
enum LED_States{LED_start, LED_display} LED_state;
int LED_Tick(int state){
	unsigned char tmpD = 0x00;
	unsigned char tmpC = 0x00;
	switch(LED_state){
	case LED_start:  //set the board completely off initially
	tmpD = tmpD | 0x24;
	PORTD = tmpD;
	for(int i = 0; i < 8; i++){
	tmpD = (tmpD & 0xF6);
	tmpC = (tmpC & 0xFE);
	PORTC = tmpC;
	PORTD = tmpD;
	tmpD = tmpD | 0x09;
	tmpC = tmpC | 0x01;
	PORTC = tmpC;
	PORTD = tmpD;
	}
	tmpD = tmpD | 0x12;
	tmpC = tmpC | 0x02;
	PORTC = tmpC;
	PORTD = tmpD;
	LED_state = LED_display;
	break;
	
	case LED_display:
	tmpC = 0x00;
	tmpD = 0x00;   //D5 should be 0, (SER)
	PORTD = tmpD;

	for(int m = 0; m < 9; m++){
	if(m == row[r] || m == row[r + 10]){
	tmpC = tmpC | 0x04; //make C2 1, (SER shift 1 in) 
	}
	PORTC = tmpC;
	
	tmpC = (tmpC & 0xFE);  //make C0 0(SRCLK)
	
	for(int n = 0; n < 8; n++){
	tmpD = (tmpD & 0xD7);  //make D3 0 (SRCLK)
	if(n == column[c] || n == column[c+1]){ 
	tmpD = tmpD | 0x20; //D5 to 1 SER
	}
	PORTD = tmpD;
	tmpD = tmpD | 0x08;    //make D3 1 (SRCLK) red row
	PORTD = tmpD;
	}
	
	PORTC = tmpC;
	tmpC = tmpC | 0x01;    //make C0 1 (SRCLK) column
	PORTC = tmpC;
	tmpC = tmpC & 0xFB;    //make C2 0 (SER)
	PORTC = tmpC; //
	}

	tmpC = tmpC | 0x02;    //make C1 1 to output it (RCLK) 
	tmpD = tmpD | 0x10;    //make D4 1 to output it (RCLK)
	PORTC = tmpC;
	PORTD = tmpD;
	LED_state = LED_display;
	break;
}
	return state;
}

enum Round_States{round_start, round_inc} r_state;
int Round_Tick(int state){
	switch(r_state){
	case round_start:
	r_state = round_inc;
	break;	
	
	case round_inc:
	r++;
	if(r >= 10){
	r = 0;
	c+= 2;
	}
	if(c >= 16){
	c = 0;
	}
	r_state = round_inc;
	break;
	
	return state;
}
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

    /* Insert your solution below */
	unsigned char i = 0; 
	tasks[i].state = LED_start; 
    	tasks[i].period = LED; 
    	tasks[i].elapsedTime = tasks[i].period; 
   	tasks[i].TickFct = &LED_Tick; 
	i++;
	tasks[i].state = round_start; 
    	tasks[i].period = roundPeriod; 
    	tasks[i].elapsedTime = tasks[i].period; 
   	tasks[i].TickFct = &Round_Tick; 
	i++;
    	TimerSet(tasksPeriodGCD); 
    	TimerOn();

    while (1) {

    }
    return 1;
}
