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


#define I2C_ADDR 0x63

  int max_pow = 100;
  double KP = 1.0;
  double KI = 0.01;
  double KD = 0.01;


volatile long count = 0;
volatile int pw = 0;
volatile int target_vel = 0;
volatile long timer_count = 0;

void i2c_received_cb(char* str) {
	//motor_set_speed(atoi(str));
	if (str[0] == 'a') {
		// a:p=1.0,i=0.1,d=1.0,
		int i = 0;
		int j = 0;
		char pgain[100];
		char igain[100];
		char dgain[100];
		for (i = 4; i < 1000; i++){
			if (str[i] == ',') {
				KP = atof(pgain);
				break;
			}			
			pgain[j] = str[i];
			j++;						
		}		
		j = 0;
		for (i = i+3; i < 1000; i++){
			if (str[i] == ',') {
				KI = atof(igain);
				break;
			}
			igain[j] = str[i];
			j++;
		}
		j = 0;
		for (i = i+3; i < 1000; i++){
			if (str[i] == ',') {
				KD = atof(dgain);
			}
			dgain[j] = str[i];
			j++;
		}				

	} else {
		// -50, 30...
		target_vel = atoi(str);
	}	
}

void i2c_request_cb(char* buf) {
	// set TI2C_buf_send.str_buf
	//strcpy(buf, "Hello World\n");
	sprintf(buf,"%ld",count);
}

void pid() {

  double p , i , d ;

  static double power;
  
  double dt = 0.0001; // sec
  
  static double integral ;
  static double now_diff;   
  static double pre_vel;
  static double now_vel;
  static double pre_enc;	
  static double pre_diff;
  double now_enc = count;

  pre_enc = now_enc;
  now_enc = count;
  pre_vel = now_vel;
  now_vel = (pre_enc - now_enc)/dt;
  
  pre_diff = now_diff;
  now_diff = target_vel - now_vel;
  if(now_diff>320)power=max_pow;
  else if(now_diff<-320)power=-max_pow;
  else{
  integral += now_diff * dt;

  p = KP * now_diff;
  i = KI * integral ;
  d = KD * (now_diff - pre_diff) /dt;
  
  //power = power+ p + i + d;
  power = p + i + d;
  if(fabs(target_vel)<0.05 && fabs(now_vel)<0.01 /*&&fabs(power) < 10*/)
  {
    integral = 0;
    power = 0;

  }
  else if(max_pow < power )
    power = max_pow;
  else if(power < -max_pow)
    power = -max_pow;
  }
 //return power;  
	motor_set_speed(power);
}

ISR(PCINT1_vect, ISR_NOBLOCK){//encorder
	if((PINC & (1 << PINC0)) ^ ((PINC & (1 << PINC1))>> 1))--count;
	else ++count;	
}


ISR(TIMER1_COMPA_vect){//PID
	return;
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
	
	//TCCR1A = 0b00000010;
	//TCCR1B = 0b00011001;
	//TIMSK1 = 0b00100000;
	//ICR1 = 64999; // 3.25ms // 20MHz

	//sei();
	//motor_set_speed(30);
	motor_set_speed(-30);
	TI2C_init_sync(I2C_ADDR,i2c_received_cb, i2c_request_cb);
}


int main(void)
{
	setup();
	double i = 0;
	
    /* Replace with your application code */
    while (1) 
    {
		while( !(TWCR & (1 << TWINT))){
			if (i > 10){
				pid();
				i = 0;
			}
			i++;
			_delay_us(10);
		}
		I2C_body();
    }
}

