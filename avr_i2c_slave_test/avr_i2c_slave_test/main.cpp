/*
 * avr_i2c_slave_test.cpp
 *
 * Created: 2018/01/21 3:12:14
 * Author : yoshihara
 */ 

//master → slave

#include <avr/io.h>
#include "I2CSlave.h"

#define I2C_ADDR 0b00010000 //自分のアドレスを知る

volatile uint8_t data;

void I2C_received(uint8_t received_data) //命令を聞き流す
{
	data = received_data;
}

void I2C_requested() //命令を受けとる
{
	I2C_transmitByte(data);
}

void setup()
{
	// set received/requested callbacks
	I2C_setCallbacks(I2C_received, I2C_requested); //masterから、他人宛ての命令が来たときはI2C_receivedが、自分宛ての命令を受信したときはI2C_requestedが、割り込む

    // init I2C
	I2C_init(I2C_ADDR); //自分のアドレスを忘れないようにする
}

int main()
{
	setup();

	// Main program loop
	while(1);
}