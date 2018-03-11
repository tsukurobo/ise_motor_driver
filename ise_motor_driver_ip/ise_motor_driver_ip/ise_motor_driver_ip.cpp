/*
 * ise_motor_driver.cpp
 *
 * Created: 2018/03/09 19:51:04
 * Author : Yoshihara(kari)
 */ 
#define F_CPU 20000000UL

#include <avr/io.h>
#include "TI2C.h"
#include "motordriver.h"
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

#define I2C_ADDR 0x10


volatile long count = 0;
volatile int pw = 50;

void i2c_received_cb(char* str) {
}

void i2c_request_cb(char* buf) {
	// set TI2C_buf_send.str_buf
	//strcpy(buf, "Hello World\n");
	sprintf(buf,"hi %ld %d aaaaaaaaaaa",count, pw);
	//count -= 1;
	//pw -=1;
}

ISR(PCINT1_vect, ISR_NOBLOCK){//jumon
	//sei();
    //count += 100;
	if((PINC & (1 << PINC0)) ^ ((PINC & (1 << PINC1))>> 1))--count;
	else ++count;	
}


void setup (){
	
		//ピン変化割り込み許可（PCINT0~7）
		PCICR |= (1<<PCIE1);
		//ピン変化割り込み許可（PCINT0）
		PCMSK1 |= (1<<PCINT8);
		//PCMSK1 = 0b00000000;
	motor_init();
	motor_set_speed(pw);
	
	DDRC = 0x00;
	PORTC = 0x00;
	

	//sei();
	
	TI2C_init(I2C_ADDR,i2c_received_cb, i2c_request_cb);
}


int main(void)
{
	setup();
    /* Replace with your application code */
    while (1) 
    {
		motor_set_speed(pw);
		pw--;
		_delay_ms(100);
    }
}

