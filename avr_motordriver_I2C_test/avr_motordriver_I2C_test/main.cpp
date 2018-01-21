/*
 * avr_motordriver_I2C_test.cpp
 *
 * Created: 2018/01/21 18:56:19
 * Author : yoshihara
 */ 

//motordriver_I2C

//I2C
#define F_CPU 1000000UL

#include <avr/io.h>
#include "I2CSlave.h"
#include "motordriver.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>


#define I2C_ADDR 0x10

volatile uint8_t data; //êÆêî0Å`255ÇÅudataÅvÇ∆åƒÇ—Ç‹Ç∑

int pw = 0;

void I2C_received(uint8_t received_data) //Å®slave
{
	data = received_data;
	pw = data - 100;
	
	motor_set_speed(pw);
	pwm_map(data);
}

void I2C_requested() //Å®master
{
	I2C_transmitByte(data);
}

void setup()
{
	// set received/requested callbacks
	I2C_setCallbacks(I2C_received, I2C_requested);

	// init I2C
	I2C_init(I2C_ADDR);
	
	 motor_init();
	
	DDRD = 0b11100000;  //port7,6,5=output
}

int main()
{
	setup();

	// Main program loop
	while(1){
		if (pw == 0){
		_delay_ms (1);
		PORTD = 0b01100000;	  //PHASE=high
		_delay_ms (1);
		PORTD = 0b11100000;   //PHASE=low
		}
	}
}
