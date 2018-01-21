/*
 * avr_i2c_slave_test.cpp
 *
 * Created: 2018/01/21 3:12:14
 * Author : yoshihara
 */ 

//master �� slave

#include <avr/io.h>
#include "I2CSlave.h"

#define I2C_ADDR 0b00010000 //�����̃A�h���X��m��

volatile uint8_t data;

void I2C_received(uint8_t received_data) //���߂𕷂�����
{
	data = received_data;
}

void I2C_requested() //���߂��󂯂Ƃ�
{
	I2C_transmitByte(data);
}

void setup()
{
	// set received/requested callbacks
	I2C_setCallbacks(I2C_received, I2C_requested); //master����A���l���Ă̖��߂������Ƃ���I2C_received���A�������Ă̖��߂���M�����Ƃ���I2C_requested���A���荞��

    // init I2C
	I2C_init(I2C_ADDR); //�����̃A�h���X��Y��Ȃ��悤�ɂ���
}

int main()
{
	setup();

	// Main program loop
	while(1);
}