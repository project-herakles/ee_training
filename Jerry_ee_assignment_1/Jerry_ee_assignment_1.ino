#include <Servo.h>
Servo left_servo, right_servo;
const int left_switch = 3, right_switch = 2;
unsigned long starter_left = 0, starter_right = 0;

void open_left(){
  delay(15);
  if (digitalRead(left_switch) == LOW){
    left_servo.write(90);
    starter_left = count;
  }
}

void open_right(){
  delay(15);
  if (digitalRead(right_switch) == LOW){
    right_servo.write(90);
    starter_right = count;
  }
}

void setup(){
  pinMode(left_switch,INPUT_PULLUP);
  pinMode(right_switch,INPUT_PULLUP);
  left_servo.attach(9);
  right_servo.attach(10);
  
  attachInterrupt(digitalPinToInterrupt(left_switch),open_left,FALLING);
  attachInterrupt(digitalPinToInterrupt(right_switch),open_right,FALLING);
  
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}

SIGNAL(TIMER0_COMPA_vect)
{
  unsigned long currentMillis = millis();
  if ((currentMillis > starter_right + 2000) && (starter_right != 0)){
    right_servo.write(0);
    starter_right = 0;
  }
  if ((currentMillis > starter_left + 2000) && (starter_left != 0)){
    left_servo.write(0);
    starter_left = 0;
  }
}

void loop(){
}
