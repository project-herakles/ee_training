#include <Servo.h>
#define LEFT_DEFAULT {11, 3, 7, false, gate_left}
#define RIGHT_DEFAULT {10, 2, 8, false, gate_right}

//The reference position for the servo
struct Gate_Ref{
  int OPEN;
  int CLOSE;
};
const Gate_Ref gate_left = {130, 50}, gate_right = {90, 170};

enum State {VACANT, OCCUPIED};
struct Device {
  int pin_servo; //the motor controlling the gate.
  int pin_switch;
  int pin_indicator; //one LED, turned on when the gate is opened, turned off otherwise
  bool occupied;
  Gate_Ref ref;
  Servo servo;
};
Device right = RIGHT_DEFAULT;
Device left = LEFT_DEFAULT;

void setup()
{
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

  attachInterrupt(digitalPinToInterrupt(right.pin_switch), rightGate, FALLING);
  attachInterrupt(digitalPinToInterrupt(left.pin_switch), leftGate, FALLING);
  
  Serial.begin(9600);
}

unsigned long lmil, rmil;
void rightGate()
{
  if(right.occupied == false)
  {
    Serial.println("right open");
    digitalWrite(right.pin_indicator, HIGH);
    right.servo.write(right.ref.OPEN);
    right.occupied = true;
    rmil = millis();
  }
}

void leftGate()
{
  if(left.occupied == false)
  {
    Serial.println("left open");
    digitalWrite(left.pin_indicator, HIGH);
    left.servo.write(left.ref.OPEN);
    left.occupied = true;
    lmil = millis();
  }
}

//Open the gate if the button state is swithced from OCCUPIED to VACANT, and close the gate automatically after 2 seconds.
void loop()
{
  if(right.occupied && millis() >= rmil + 2000)
  {
    Serial.println("right close");
    digitalWrite(right.pin_indicator, LOW);
    right.servo.write(right.ref.CLOSE);
    right.occupied = false;
  }
  if(left.occupied && millis() >= lmil + 2000)
  {
    Serial.println("left close");
    digitalWrite(left.pin_indicator, LOW);
    left.servo.write(left.ref.CLOSE);
    left.occupied = false;
  }
}
