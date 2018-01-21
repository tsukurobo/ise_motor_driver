/*
 * avr_I2C_slave_test2.cpp
 *
 * Created: 2018/01/21 4:16:23
 * Author : yoshihara
 */ 

#include <avr/io.h>

#include "I2CSlave.h"

#define I2C_ADDR 0x10

volatile uint8_t data;

void I2C_received(uint8_t received_data)
{
	data = received_data;
}

void I2C_requested()
{
	I2C_transmitByte(data);
}

void setup()
{
	// set received/requested callbacks
	I2C_setCallbacks(I2C_received, I2C_requested);

	// init I2C
	I2C_init(I2C_ADDR);
}

int main()
{
	setup();

	// Main program loop
	while(1);
}