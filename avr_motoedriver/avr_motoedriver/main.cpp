/*
 * avr_motoedriver.cpp
 *
 * Created: 2018/01/14 22:01:58
 * Author :yoshihara
 */ 

#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>



int power = 0;
bool cw = false;


enum{
	TMR0Full = 255,
	TMR0INI = TMR0Full - 255 //122,
	};


int pwm_map (int duty);

int main(void)
{
	//setup
	DDRD = 0b11100000;    //port7,6,5=output
	
	DDRC = 0b00001000;
	
	TCCR0A = 0b10100011;  //highspeedPWM
	
	TCCR0B = 0b00000010;  //1/8,Top=OVF
	
	
    
    while (1) {
		if (power < 0){
			PORTD = 0b01100000;	  //PHASE=high
		}
		else if(power > 0){
			PORTD = 0b11100000;   //PHASE=low
		}
		/*else {
			_delay_ms (1);
			PORTD = 0b01100000;	  //PHASE=high
			_delay_ms (1);
			PORTD = 0b11100000;   //PHASE=low
			
			
		}*/
	OCR0A = pwm_map(abs(power));
	OCR0B = pwm_map(abs(power));
		
	
	/*_delay_ms (500);
	PORTC = 0b00001000;
	_delay_ms (500);
	PORTC = 0b00001000;
	power += 0;*/
    }
	
}

int pwm_map (int duty){
	return TMR0INI + (int)(1.2*duty); //duty=0~100
}


