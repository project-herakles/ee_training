#include <Servo.h>
#define LEFT_DEFAULT {9, 2, 7, 0, gate_left}
#define RIGHT_DEFAULT {10, 3, 8, 0, gate_right}

//The reference position for the servo
struct Gate_Ref{
  int OPEN;
  int CLOSE;
};
const Gate_Ref gate_left = {130, 50}, gate_right = {90, 170};

struct Device {
  int pin_servo; //the motor controlling the gate.
  int pin_switch;
  int pin_indicator; //one LED, turned on when the gate is opened, turned off otherwise
  unsigned long starter; //record the millis() when hardware interrupt is activated
  Gate_Ref ref;
  Servo servo;
};
Device right = RIGHT_DEFAULT;
Device left = LEFT_DEFAULT;

unsigned long reset = 100000; //a big enough number so that currentmillis+2000 cannot reach

void open_left(){
  delay(15);
  if (digitalRead(left.pin_switch) == LOW){ //double check to prevent glitch
    digitalWrite(left.pin_indicator, HIGH);
    left.servo.write(left.ref.OPEN);
    left.starter = millis(); //record starter
  }
}

void open_right(){
  delay(15);
  if (digitalRead(right.pin_switch) == LOW){
    digitalWrite(right.pin_indicator, HIGH);
    right.servo.write(right.ref.OPEN);
    right.starter = millis();
  }
}

void setup()
{
  //Setup the control and state of the Right gate
  pinMode(right.pin_switch, INPUT_PULLUP);
  pinMode(right.pin_servo, OUTPUT);
  pinMode(right.pin_indicator, OUTPUT);
  digitalWrite(right.pin_indicator, LOW);
  right.servo.attach(right.pin_servo);
  right.servo.write(right.ref.CLOSE);
  right.starter = reset; //set the default starter
  delay(15);
  

  //Setup the control and state of the Left gate
  pinMode(left.pin_switch, INPUT_PULLUP);
  pinMode(left.pin_servo, OUTPUT);
  pinMode(left.pin_indicator, OUTPUT);
  digitalWrite(left.pin_indicator, LOW);
  left.servo.attach(left.pin_servo);
  left.servo.write(left.ref.CLOSE);
  left.starter = reset;
  delay(15);

  //Insert hardware interrupt
  attachInterrupt(digitalPinToInterrupt(left.pin_switch),open_left,FALLING);
  attachInterrupt(digitalPinToInterrupt(right.pin_switch),open_right,FALLING);

  //Insert timer interrupt
  TCCR0A = 0;
  TCNT0 = 0;
  OCR0A = 255;
  TCCR0A |= (1<<WGM01);
  TIMSK0 |= (1<<OCIE0A);
}

ISR(TIMER0_COMPA_vect)
{
  unsigned long currentMillis = millis(); //fetch current millis()
  if (currentMillis >= right.starter + 2000) //if it has already been 2 seconds
  {
    digitalWrite(right.pin_indicator, LOW);
    right.servo.write(right.ref.CLOSE);
    right.starter = reset; //reset starter
  }
  currentMillis = millis(); //refetch currentmillis in case it changes
  if (currentMillis >= left.starter + 2000)
  {
    digitalWrite(left.pin_indicator, LOW);
    left.servo.write(left.ref.CLOSE);
    left.starter = reset;
  }
}

void loop(){
}
