/*
 * AVR_motordriver_test1.cpp
 *
 * Created: 2018/01/13 21:28:51
 * Author : yoshihara
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
enum{   TMR0FuLL = 255,
	TMR0INI = TMR0FuLL - 122,       // 周期 1kHz
	OC0A_INI = TMR0INI + 96,        // Dty 80%
	OC0B_INI = TMR0INI + 24         // Dty 20%
};

ISR(TIMER0_OVF_vect) {          //Tmr0ｵｰﾊﾞｰﾌﾛｰ割り込み関数
	TCNT0 = TMR0INI;                // タイマ0の初期値
}

int main(void)
{
   DDRC = 0b00000000;
   DDRB = 0xFF;            // ポートbを全出力設定
   DDRD = 0b11100000;            // ポートDを1111 0011  #2,3だけ入力設定
   PORTD = 0b11100000;           //          0000 1100  #2,3だけ内部プルアップ設定

   //TCCR0A = 0xA3;          // 1010 0011    高速PWMモード
   //TCCR0B = 0x02;          // 0000 0010    1/8 ,Top=OVF
   //TIMSK = 0x02;           // 0000 0010    TMR0 TMR0_OVF割込み許可

   TCNT0 = TMR0INI;                    // タイマ0の初期値
   
   OCR0A = OC0A_INI;                               /// Dty 80%
   OCR0B = OC0A_INI;                               // Dty 20%

   sei();                  // 割り込みの許可
	 
	 
	 
    while (1) 
    {
    }
}

