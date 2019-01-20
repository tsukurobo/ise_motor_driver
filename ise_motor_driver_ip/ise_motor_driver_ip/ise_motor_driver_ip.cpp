/*
 * ise_motor_driver.cpp
 *
 * Created: 2018/03/09 19:51:04
 * Author : Yoshihara(kari)
 */ 
#define F_CPU 20000000UL

#include <math.h>
#include <avr/io.h>
#include "I2CSlave.h"
#include "TI2C.h"
#include "motordriver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define I2C_ADDR 0x14


volatile long count = 0;
volatile int pw = 0;
volatile int target_enc = 400;
volatile long timer_count = 0;

void i2c_received_cb(char* str) {
	motor_set_speed(atoi(str));
}

void i2c_request_cb(char* buf) {
	// set TI2C_buf_send.str_buf
	//strcpy(buf, "Hello World\n");
	sprintf(buf,"%ld",count);
}

void pid() {
  double target_vel;
  double now_vel;
  double KP = 0.1;
  double KI = 0;
  double KD = 0;
  double p , i , d ;
  double integral ;
  double power;
  
  double dt = 0.1; // 0.1sec
  
  static double now_diff;   
  static double pre_enc;	
  static double pre_diff;
  double now_enc = count;
  
  
  pre_diff = now_diff;
  now_diff = target_enc - now_enc;
  if(now_diff>320)power=12;
  else if(now_diff<-320)power=-12;
  else{
  integral += now_diff * dt;

  p = KP * now_diff;
  i = KI * integral ;
  d = KD * (now_diff - pre_diff) /dt;
  
  power = power+ p + i + d;
  if(fabs(target_enc)<0.05 && fabs(now_vel)<0.01 /*&&fabs(power) < 10*/)
  {
    integral = 0;
    power = 0;

  }
  else if(12 < power )
    power = 12;
  else if(power < -12)
    power = -12;
  }
 //return power;  
	motor_set_speed(power);
}

ISR(PCINT1_vect, ISR_NOBLOCK){//encorder
	if((PINC & (1 << PINC0)) ^ ((PINC & (1 << PINC1))>> 1))--count;
	else ++count;	
}


ISR(TIMER1_COMPA_vect){//PID
	if (timer_count > (0.1 / 0.00325)) {
		pid();	
		timer_count = 0;
	} 
	timer_count ++;		
}


void setup (){
	
	//ピン変化割り込み許可（PCINT8~15）
	PCICR |= (1<<PCIE1);
	//ピン変化割り込み許可（PCINT8）
	PCMSK1 |= (1<<PCINT8);
	motor_init();
	motor_set_speed(pw);
	
	DDRC = 0x00;
	PORTC = 0x00;
	
	// LED of addresses 0x10~ 0x1F 0~F -> 0~15
	PORTD |= (0b00001111 & I2C_ADDR);
	
	TCCR1A = 0b00000010;
	TCCR1B = 0b00011001;
	TIMSK1 = 0b00100000;
	ICR1 = 64999; // 3.25ms // 20MHz

	sei();
	motor_set_speed(12);
	TI2C_init_sync(I2C_ADDR,i2c_received_cb, i2c_request_cb);
}


int main(void)
{
	setup();
	
    /* Replace with your application code */
    while (1) 
    {
		I2C_main();
    }
}

