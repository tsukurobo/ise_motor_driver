const int PWM_H = 10;
const int PWM_L = 9;
const int PHASE = 3;
const int FF1 =7;
const int FF2 =8;
const int SR =4;
const int RESET = 5;
static int PWM; 

int F1;
int F2;
void setup() {
TCCR1B &= B11111000;
TCCR1B |= B00000011;
pinMode(PWM_H, OUTPUT);
pinMode(PWM_L, OUTPUT);
pinMode(PHASE, OUTPUT);
pinMode(SR, OUTPUT);
pinMode(F1, INPUT);
pinMode(F2, INPUT);
pinMode(RESET,OUTPUT);
digitalWrite(SR,LOW);
digitalWrite(RESET,HIGH);
PWM =255;
Serial.begin(9600);
}
void loop() {
//  digitalWrite(PWM_H,LOW);
analogWrite(PWM_H, PWM);
analogWrite(PWM_L, PWM);
//digitalWrite(PWM_L,HIGH);
//digitalWrite(PHASE, LOW);
F1 = digitalRead(FF1);
F2 = digitalRead(FF2);
//Serial.print(F1);
//Serial.print(F2);
delay(1000);
if(PWM>7){
  PWM=PWM-10;
  analogWrite(PWM_H, PWM);
  analogWrite(PWM_L, PWM);
  //digitalWrite(PWM_H,HIGH);
//  digitalWrite(PWM_L,HIGH);
  Serial.println(PWM);
}
else{
  
}
//delay(1000);
//analogWrite(PWM_H, PWM);
//analogWrite(PWM_L, PWM);
//digitalWrite(PHASE, HIGH);
//delay(1000);
}

