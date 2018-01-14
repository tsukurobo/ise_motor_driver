/*
 * AVR_motordriver_test.cpp
 *
 * Created: 2018/01/13 20:34:33
 * Author : yoshihara
 */ 

#include <avr/io.h>


int main(void)
{
    DDRB = 0b11000000;
    DDRD = 0b00100011;
	
    while (1) 
    {
    }
}

