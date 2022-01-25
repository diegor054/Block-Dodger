/*	Author: Diego Rivera
 *	Lab Section: 21
 *	Assignment: Final Project Demo 
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://www.youtube.com/watch?v=eGWgO8izN5U 
 *
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h" 
#include "nokia.h"
#endif

typedef struct task { 
  int state; 
  unsigned long period; 
  unsigned long elapsedTime; 
  int (*TickFct)(int);     
} task; 


task tasks[10];
const unsigned long tasksPeriodGCD = 1; 
const unsigned long LED = 1;
const unsigned long blank = 1;
const unsigned long A_D_C = 100;
const unsigned long R1 = 500;
const unsigned long RSelect = 200;
const unsigned long C = 100;
const unsigned long R2 = 350;
const unsigned long R3 = 450;
const unsigned long R4 = 300;
const unsigned long L_C_D = 550;

void TimerISR() {
	unsigned char i; 
	for(i = 0; i < 10; ++i){ 
        if(tasks[i].elapsedTime >= tasks[i].period){         
	tasks[i].state = tasks[i].TickFct(tasks[i].state);            
	tasks[i].elapsedTime = 0; 
      } 
        tasks[i].elapsedTime += tasksPeriodGCD; 
   } 
}

int round;
int collision;
int counter;
unsigned char lastRow;
int passed;
unsigned char playerPosition;
unsigned char arr[8] = {0x00, 0x00, 0x00,0x00,0x00,0x00,0x00,0x00};

enum LED_States{LED_start, LED_display} LED_state;
int LED_Tick(int state){
	unsigned char tmpD = 0x00;
	unsigned char tmpC = 0x00;
	if(round != 0){
	switch(state){
	case LED_start:  
	state = LED_display;
	break;
	
	case LED_display:
	tmpC = 0x00;
	tmpD = 0x00; 
	
	for(unsigned char i = 0; i < 8; i++){
	tmpC = (tmpC & 0xFE);  //make C0 0(SRCLK)
	tmpC = (tmpC & 0xFD);    //make C1 0 (RCLK)
	tmpD = (tmpD & 0xEF);    //make D4 0 (RCLK)
	PORTD = tmpD;
	PORTC = tmpC;
	
		for(signed char j = 7; j >= 0; j--){
		tmpD = (tmpD & 0xD7);  //make D3 0 (SRCLK)
		PORTD = tmpD;
		if(!(arr[i] & 1 << j)){
		tmpD = tmpD | 0x20; //D5 to 1 SER to shift in a 1 in my rows 
		}else{
		tmpD = tmpD & 0xDF;
		}
		PORTD = tmpD;
		tmpD = tmpD | 0x08;    //make D3 1 (SRCLK) red row
		PORTD = tmpD;
		}

		for(int col = 0; col < 8; col++){
		tmpC = (tmpC & 0xFE);  //make C0 0(SRCLK)
		tmpC = tmpC & 0xFB;    //make C2 0 (SER) 
		PORTC = tmpC;
		tmpC = tmpC | 0x01;    //make C0 1 (SRCLK) column
		PORTC = tmpC;
		}
		tmpC = (tmpC & 0xFD);    //make C1 0 (RCLK)
		PORTC = tmpC;
		tmpC = tmpC | 0x02;    //make C1 1 (RCLK) 
		PORTC = tmpC;
		tmpC = (tmpC & 0xFD);    //make C1 0 (RCLK)
		PORTC = tmpC;

		
		tmpC = (tmpC & 0xFE);  //make C0 0(SRCLK)
		tmpC = tmpC | 0x04;       //make C2 (ser) a 1 to choose right column
		PORTC = tmpC;
		tmpC = tmpC | 0x01;    //make C0 1 (SRCLK) column
		PORTC = tmpC;	

		for(unsigned char k = 0; k < i; k++){
		tmpC = (tmpC & 0xFE);  //make C0 0(SRCLK)
		tmpC = tmpC & 0xFB;    //make C2 0 (SER) to get to the right column
		PORTC = tmpC;
		tmpC = tmpC | 0x01;    //make C0 1 (SRCLK) column
		PORTC = tmpC;
		}
	
	tmpD = tmpD | 0x10;    //make D4 1 (RCLK)
	PORTD = tmpD; 
	tmpC = tmpC | 0x02;    //make C1 1 (RCLK) 
	PORTC = tmpC; 
	}
	//after nested loops
	for(int r = 0; r < 8; r++){
		tmpD = (tmpD & 0xD7);  //make D3 0 (SRCLK)
		tmpD = tmpD | 0x20; //D5 to 1 SER to shift in a 1 in my rows 
		PORTD = tmpD;
		tmpD = tmpD | 0x08;    //make D3 1 (SRCLK) red row
		PORTD = tmpD;
	}
	tmpD = (tmpD & 0xEF);    //make D4 0 (RCLK)
	PORTD = tmpD;
	tmpD = tmpD | 0x10;    //make D4 1 (RCLK)
	PORTD = tmpD;

	//player
	tmpC = (tmpC & 0xFD);    //make C1 0 (RCLK)
	PORTC = tmpC;
		
	for(unsigned char k = 0; k < 7; k++){
		tmpC = (tmpC & 0xFE);  //make C0 0(SRCLK)
		tmpC = tmpC & 0xFB;    //make C2 0 (SER) to get to the right column
		PORTC = tmpC;
		tmpC = tmpC | 0x01;    //make C0 1 (SRCLK) column
		PORTC = tmpC;
		}

		tmpC = (tmpC & 0xFE);  //make C0 0(SRCLK)
		tmpC = tmpC | 0x04;       //make C2 (ser) a 1 to choose right column
		PORTC = tmpC;
		tmpC = tmpC | 0x01;    //make C0 1 (SRCLK) column
		PORTC = tmpC;
	tmpC = tmpC | 0x02;    //make C1 1 (RCLK) 
	PORTC = tmpC; 

	tmpD = tmpD & 0xFD; //make D1 0 (RCLK)
	for(signed char j = 7; j >= 0; j--){
		tmpD = tmpD & 0xFE;    //make D0 0 (SRCLK) blue
		PORTD = tmpD;
		if(!(playerPosition & 1 << j)){
		tmpD = tmpD | 0x04; //D2 to 1 SER to shift in a 1 in my rows (blue)
		}else{
		tmpD = tmpD & 0xFB;
		}
		PORTD = tmpD;
		tmpD = tmpD | 0x01;    //make D0 1 (SRCLK) blue
		PORTD = tmpD;
	}
	tmpD = tmpD | 0x02;    //make D1 1 to output it (RCLK)
	PORTD = tmpD;
	
	for(int b = 0; b < 8; b++){
		tmpD = tmpD & 0xFE;    //make D0 0 (SRCLK) blue
		tmpD = tmpD | 0x04; //D2 to 1 SER to shift in a 1 in my rows (blue)
		PORTD = tmpD;
		tmpD = tmpD | 0x01;    //make D0 1 (SRCLK) blue
		PORTD = tmpD;
	}
	tmpD = tmpD & 0xFD; //make D1 0 (RCLK)
	PORTD = tmpD;
	tmpD = tmpD | 0x02;    //make D1 1 to output it (RCLK)
	PORTD = tmpD;


	state = LED_display;
	break;

}
	default:
	state = LED_start;
	break;
}	
	return state;
}


enum Blank_States{Blank_Board} Blank_state;
int Blank_Tick(int state){
	unsigned char m = 0;
	if(round == 0){
	unsigned char tmpD = 0x00;
	unsigned char tmpC = 0x00;
	switch(state){
	case Blank_Board:  //set the board completely off initially
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
	for(int i = 0; i < 7; i++){
		arr[i] = 0;
	}
	
	state = Blank_Board;
	break;
	}
	default:
	state = Blank_Board;
	break;
	}
	return state;
}


void ADC_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

unsigned char max;
enum RoundSelect_States{RoundSelect_start, RoundSelect_wait} RoundSelect_state;
int RoundSelect_Tick(int state){
	unsigned char tmpA;
	switch(state){
	case RoundSelect_start:
	tmpA = ~PINA & 0x10;
	round = 0;
	counter = 1;
	max = 0;
	if(tmpA != 0){
	collision = 0;
	state = RoundSelect_wait;
	}
	break;
	
	case RoundSelect_wait:
	tmpA = 0;
	round = counter;
	if(collision == 1){
	state = RoundSelect_start;
	}
	break;
	default:
	state = RoundSelect_start;
	break;
	}
	return state;
}


enum Collision_States{Collision_start} Collision_state;
int Collision_Tick(int state){
	switch(state){

	case Collision_start:
	if((lastRow & playerPosition) != 0){
	collision = 1;
	lastRow = 0;
	}
	break;
	
	default:
	state = Collision_start;
	break;
	}
	return state;
}

unsigned char temp2;
unsigned char s;
unsigned char s2;
unsigned char happened;
unsigned char round_1;
unsigned char round_2;
unsigned char round_3;
unsigned char round_4;
enum LCD_States{LCD_init, LCD_start} LCD_state;
int LCD_Tick(int state){
	
	switch(state){
	case LCD_init:
	temp2 = -1;	
	happened = 0;
	s = 0;
	round_1 = 0;
	round_2 = 0;
	round_3 = 0;
	round_4 = 0;
	state = LCD_start;
	break;

	case LCD_start:
	if(temp2 != round && happened == 0){
	clear_LCD();
	happened = 1;
	round_1 = 0;
	round_2 = 0;
	round_3 = 0;
	round_4 = 0;
	}
	if(round == 0){
	print_Button();
	temp2 = 0;
	happened = 0;
	}
	if(collision == 1 && s < 5){
	print_Skull();
	temp2 = round;
	happened = 0;
	s++;
	}
	if(round == 1 && round_1 == 0){
	s = 0;
	temp2 = 1;
	happened = 0;
	round_1 = 1;
	print_Round1();
	}
	if(round == 2 && round_2 == 0){
	s = 0;
	temp2 = 2;
	happened = 0;
	round_2 = 1;
	print_Round2();
	}
	if(round == 3 && round_3 == 0){
	s = 0;
	temp2 = 3;
	happened = 0;
	round_3 = 1;
	print_Round3();
	}
	if(round == 4 && round_4 == 0){
	s = 0;
	temp2 = 4;
	happened = 0;
	round_4 = 1;
	print_Round4();
	}
	if(passed == 1){
	print_CheckMark();
	}
	break;
	
	default:
	state = LCD_init;
	break;
	
	}
	return state;
}

enum ADC_States{ADC_start, ADC_sample} ADC_state;
int ADC_Tick(int state){
	switch(ADC_state){
	case ADC_start:
	playerPosition = 0x10;
	ADC_state = ADC_sample;
	break;
	
	case ADC_sample:
	if(ADC < 75 && playerPosition > 0x01 && ADC > 0){
	playerPosition = playerPosition >> 1;
	}
	if(ADC > 950 && playerPosition < 0x80 && ADC <  1024){
	playerPosition = playerPosition << 1;
	}
	break;
	default:
	state = ADC_start;
	break;

	}
	return state;
}


enum Round1_States{Round1_sample} Round1_state;
int Round1_Tick(int state){
	if(round == 1){
	switch(state){
	case Round1_sample:
	for(int i = 0; i < 7; i++){
		arr[i] = arr[i + 1];
	}

	int number = (rand() % 10);
	if(number == 0 && max < 30){
	arr[7] = 0xF0;
	}else if(number == 1 || number == 2 || number == 3|| number == 4|| number == 5){ //higher chance at empty rows for easier game
	arr[7] = 0x00;
	}else if(number == 6){
	arr[7] = 0x52;
	}else if(number == 7){
	arr[7] = 0xC9;
	}else if(number == 8){
	arr[7] = 0x55;
	}else{
	arr[7] = 0xAC;
	}
	max++;
	if(max >= 30 && max <= 40){
	arr[7] = 0x00;
	}
	if(max == 36){
	passed = 1;
	}
	if(max == 41){
	passed = 0;
	counter = 2;
	max = 0;
	}
	lastRow = arr[0];
	break;
	}
	
	default:
	state = Round1_sample;
	break;
	
	}
	return state;
}

enum Round2_States{Round2_sample} Round2_state;
int Round2_Tick(int state){
	if(round == 2){
	switch(state){
	case Round2_sample:
	for(int i = 0; i < 7; i++){
		arr[i] = arr[i + 1];
	}

	int number = (rand() % 7);
	if(max < 30){
	if(number == 0){
	arr[7] = 0xA5;
	}else if(number == 1 || number == 2 || number == 3){ 
	arr[7] = 0x00;
	}else if(number == 4){
	arr[7] = 0x53;
	}else if(number == 5){
	arr[7] = 0xC9;
	}else if(number == 6){
	arr[7] = 0x55;
	}else{
	arr[7] = 0xAC;
	}
	}
	max++;
	if(max >= 30 && max <= 40){
	arr[7] = 0x00;
	}
	if(max == 36){
	passed = 1;
	}
	if(max == 41){
	max = 0;
	passed = 0;
	counter = 3;
	}
	lastRow = arr[0];
	break;
	}
	default:
	state = Round2_sample;
	break;
	}
	return state;
}

enum Round3_States{Round3_sample} Round3_state;
int Round3_Tick(int state){
	if(round == 3){
	switch(state){
	case Round3_sample:
	for(int i = 0; i < 7; i++){
		arr[i] = arr[i + 1];
	}

	int number = (rand() % 6);
	if(max < 30){
	if(number == 0){
	arr[7] = 0x37;
	}else if(number == 1 || number == 2 || number == 3){ 
	arr[7] = 0x00;
	}else if(number == 4){
	arr[7] = 0x44;
	}else if(number == 5){
	arr[7] = 0x93;
	}else{
	arr[7] = 0x97;
	}
	} 
	max++;
	if(max >= 30 && max <= 40){
	arr[7] = 0x00;
	}
	if(max == 36){
	passed = 1;
	}
	if(max == 41){
	max = 0;
	passed = 0;
	counter = 4;
	}
	lastRow = arr[0];
	break;
	}
	default:
	state = Round3_sample;
	break;
	}
	return state;
}

enum Round4_States{Round4_sample} Round4_state;
int Round4_Tick(int state){
	if(round == 4){
	switch(state){
	case Round4_sample:
	for(int i = 0; i < 7; i++){
		arr[i] = arr[i + 1];
	}

	int number = (rand() % 5);
	if(max < 30){
	if(number == 0){
	arr[7] = 0x37;
	}else if(number == 1){ 
	arr[7] = 0x00;
	}else if(number == 2){
	arr[7] = 0x3B;
	}else if(number == 3){
	arr[7] = 0x44;
	}else if(number == 4){
	arr[7] = 0x9B;
	}else{
	arr[7] = 0x97;
	}
	}
	max++;
	if(max >= 30 && max <= 40){
	arr[7] = 0x00;
	}
	
	if(max == 41){
	max = 0;
	print_GameOver();
	round = 0;
	}
	lastRow = arr[0];
	break;
	}
	default:
	state = Round4_sample;
	break;
	}
	return state;
}



unsigned char init[6] = {0x21, 0xB9, 0x04, 0x14, 0x20, 0x0C};
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;

    /* Insert your solution below */
	unsigned char i = 0; 
	tasks[i].state = LED_start; 
    	tasks[i].period = LED; 
    	tasks[i].elapsedTime = tasks[i].period; 
   	tasks[i].TickFct = &LED_Tick; 
	i++;
	tasks[i].state = ADC_start; 
    	tasks[i].period = A_D_C; 
    	tasks[i].elapsedTime = tasks[i].period; 
   	tasks[i].TickFct = &ADC_Tick; 
	i++;
	tasks[i].state = Blank_Board; 
    	tasks[i].period = blank; 
    	tasks[i].elapsedTime = tasks[i].period; 
   	tasks[i].TickFct = &Blank_Tick; 
	i++;
	tasks[i].state = Round1_sample; 
    	tasks[i].period = R1; 
    	tasks[i].elapsedTime = tasks[i].period; 
   	tasks[i].TickFct = &Round1_Tick; 
	i++;
	tasks[i].state = RoundSelect_start; 
    	tasks[i].period = RSelect; 
    	tasks[i].elapsedTime = tasks[i].period; 
   	tasks[i].TickFct = &RoundSelect_Tick; 
	i++;
	tasks[i].state = Collision_start; 
    	tasks[i].period = C; 
    	tasks[i].elapsedTime = tasks[i].period; 
   	tasks[i].TickFct = &Collision_Tick; 
	i++;
	tasks[i].state = Round2_sample; 
    	tasks[i].period = R2; 
    	tasks[i].elapsedTime = tasks[i].period; 
   	tasks[i].TickFct = &Round2_Tick; 
	i++;
	tasks[i].state = Round3_sample; 
    	tasks[i].period = R3; 
    	tasks[i].elapsedTime = tasks[i].period; 
   	tasks[i].TickFct = &Round3_Tick; 
	i++;
	tasks[i].state = Round4_sample; 
    	tasks[i].period = R4; 
    	tasks[i].elapsedTime = tasks[i].period; 
   	tasks[i].TickFct = &Round4_Tick; 
	i++;
	tasks[i].state = LCD_init; 
    	tasks[i].period = L_C_D; 
    	tasks[i].elapsedTime = tasks[i].period; 
   	tasks[i].TickFct = &LCD_Tick; 
	i++;
	
	ADC_init();
    	TimerSet(tasksPeriodGCD); 
    	TimerOn();

	//initialize
	for(int i = 0; i < 6; i++){
	LCD_NOKIA_OUT(0, init[i]);
	}

	clear_LCD();
    while (1) {

    }
    return 1;
}
