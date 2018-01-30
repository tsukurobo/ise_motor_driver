#include <Wire.h>

#define TARGET_ADDR 0x11

uint8_t val = -100;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  digitalWrite(12,HIGH); 
}

int pw_to_i2c(int power) {
  /**
   * @param power -100~100
   * power is 0~100, but i2c can 0~255
   */
   return power + 100;
  }

void loop() {
  Wire.beginTransmission(TARGET_ADDR);
  if(val > 100)val = -100;
  Wire.write(pw_to_i2c(val));
  Wire.endTransmission();
  //Serial.print(val_.dev[0]);Serial.print("_");Serial.println(val_.dev[1]);
  //intToByte(1200000);
  Serial.println(val);
  val++;
  delay(300);
}

