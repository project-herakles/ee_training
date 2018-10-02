#include <Servo.h>
#define LEFT_DEFAULT {9, 5, 7, false, gate_left}
#define RIGHT_DEFAULT {10, 6, 8, false, gate_right}

//The reference position for the servo
struct Gate_Ref{
  int OPEN;
  int CLOSE;
};
const Gate_Ref gate_left = {130, 50}, gate_right = {90, 170};

enum State {VACANT, OCCUPIED};
int time_counterR = 0;
int time_counterL = 0;
struct Device {
  int pin_servo; //the motor controlling the gate.
  int pin_switch;
  int pin_indicator; //one LED, turned on when the gate is opened, turned off otherwise
  bool occupied;
  Gate_Ref ref;
  Servo servo;
}
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
void setup() {
  // put your setup code here, to run once:
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
  
//Setup the control and state of the Right gate
  pinMode(right.pin_switch, INPUT_PULLUP);
  pinMode(right.pin_servo, OUTPUT);
  pinMode(right.pin_indicator, OUTPUT);
  digitalWrite(right.pin_indicator, LOW);
  right.servo.attach(right.pin_servo);
  right.servo.write(right.ref.CLOSE);
  attachInterrupt(digitalPinToInterrupt(left.pin_switch), startup_right, LOW);
  delay(15);
  

  //Setup the control and state of the Left gate
  pinMode(left.pin_switch, INPUT_PULLUP);
  pinMode(left.pin_servo, OUTPUT);
  pinMode(left.pin_indicator, OUTPUT);
  digitalWrite(left.pin_indicator, LOW);
  left.servo.attach(left.pin_servo);
  left.servo.write(left.ref.CLOSE);
  attachInterrupt(digitalPinToInterrupt(right.pin_switch), startup_left, LOW);
  delay(15);
  
  //Serial.begin(115200);
}
  //ISR
void startup_right(){
  digitalWrite(right.pin_indicator, HIGH);
  right.servo.write(right.ref.OPEN);
  time_counterR = 1;
  }  

void startup_left(){
  digitalWrite(left.pin_indicator, HIGH);
  leftt.servo.write(left.ref.OPEN);
  time_counterL = 1;
  }  
  
SIGNAL(TIMER0_COMPA_vect){
  if (time_counterR > 0){
    time_counterR ++;
    }
  if (time_counterL > 0){
    time_counterL ++;
    }
  else 
    right.occupied = check_occupied(right.pin_switch);
    left.occupied = check_occupied(left.pin_switch); // necessary?
    } 
  }

void loop() {
  // put your main code here, to run repeatedly:
  if (time_counterR > 2000){
    right.servo.write(right.ref.CLOSE);
    digitalWrite(right.pin_indicator, LOW);
    time_counterR = 0;
    }
  if (time_counterL > 2000){
    left.servo.write(left.ref.CLOSE);
    digitalWrite(left.pin_indicator, LOW);
    time_counterL = 0;
    }
}
