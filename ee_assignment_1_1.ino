#include <Servo.h>
#define LEFT_DEFAULT {9, 5, 7, false, gate_left,0}
#define RIGHT_DEFAULT {10, 6, 8, false, gate_right, 0}

//The reference position for the servo
struct Gate_Ref{
  int OPEN;
  int CLOSE;
};
const Gate_Ref gate_left = {130, 50}, gate_right = {90, 170};

//enum State {VACANT, OCCUPIED};
struct Device {
  int pin_servo; //the motor controlling the gate.
  int pin_switch;
  int pin_indicator; //one LED, turned on when the gate is opened, turned off otherwise
  bool occupied;
  Gate_Ref ref;
  int openTime;
  Servo servo;
};
Device right = RIGHT_DEFAULT;
Device left = LEFT_DEFAULT;

//Exmamine if the button is pressed, which indicates the parking slot is occcupied
//@return TRUE if the button is pressedif the button is pressed, FALSE otherwise
bool check_occupied(const int& pin_switch){
  int first = digitalRead(pin_switch); // reads LOW if the button is pressed
  delay(15);
  int second = digitalRead(pin_switch); // read second time in case of glitches
  if (first != second)
    return check_occupied(pin_switch);
  else 
    return !first; //TRUE if the button is pressed
}

//declaration of self-written functions

void open_gate_right();
void open_gate_left();
void close_gate_right();
void close_gate_left();

const int interval=2000;

void setup()
{
  Serial.begin(9600);
  //Setup the control and state of the Right gate
  pinMode(right.pin_switch, INPUT_PULLUP);
  pinMode(right.pin_servo, OUTPUT);
  pinMode(right.pin_indicator, OUTPUT);
  digitalWrite(right.pin_indicator, LOW);
  right.servo.attach(right.pin_servo);
  right.servo.write(right.ref.CLOSE);
  delay(15);
  

  //Setup the control and state of the Left gate
  pinMode(left.pin_switch, INPUT_PULLUP);
  pinMode(left.pin_servo, OUTPUT);
  pinMode(left.pin_indicator, OUTPUT);
  digitalWrite(left.pin_indicator, LOW);
  left.servo.attach(left.pin_servo);
  left.servo.write(left.ref.CLOSE);
  delay(15);
  
  attachInterrupt(digitalPinToInterrupt(left.pin_switch),open_gate_left,FALLING);
  attachInterrupt(digitalPinToInterrupt(right.pin_switch),open_gate_right,FALLING);
  
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
  TCCR0A |= _BV(WGM01);
}

//Timer interrupt
SIGNAL(TIMER0_COMPA_vect) 
{
  if(right.occupied == true){
    right.openTime++;
    if(right.openTime >= interval){
      close_gate_right();
    }
  }
  if(left.occupied == true){
    left.openTime++;
    if(left.openTime >= interval){
      close_gate_left();
    }
  }
} 

//Open the gate if the button state is swithced from OCCUPIED to VACANT, and close the gate automatically after 2 seconds.
void loop(){
}

//Below is the self-written function

void open_gate_right(){
    if(right.occupied == false){
      right.servo.write(right.ref.OPEN);
      digitalWrite(right.pin_indicator,HIGH);
      right.occupied = true;
      right.openTime=0;
    }
}

void open_gate_left(){
    if(left.occupied == false){
      left.servo.write(left.ref.OPEN);
      digitalWrite(left.pin_indicator,HIGH);
      left.occupied = true;
      left.openTime=0;
    }
}

void close_gate_right(){
    if(right.occupied == true){
      right.servo.write(right.ref.CLOSE);
      digitalWrite(right.pin_indicator,LOW);
      right.occupied = false;
      right.openTime=0;
    }
}

void close_gate_left(){
    if(left.occupied == true){
      left.servo.write(left.ref.CLOSE);
      digitalWrite(left.pin_indicator,LOW);
      left.occupied = false;
      left.openTime=0;
    }
}