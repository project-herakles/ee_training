// I've found two ways to implement this. You can either code the
// program to do the counting itself or use the function millis()
// to do the timing. I personally choose the millis() method because
// it's easier to manage if we were to expand upon this code. 
// But, I have also wrote a version of the program that manually do
// the counting and I have attached segments of it in block 
// comments at the end of this document.
// Also, whenever you use these timer functions, I would recommend you to not change the prescaler.

//As for what I have done, I revised the original sketch to use interrupts for controlling servos instead of for-loop.
//Although "State" and "right.occupied/left.occupied" is not needed for the code to work, I kept it anyway
//for book-keeping/in case it's needed in the future. However, currently the code does not set the "Device" to
//occupied = true when the button is pressed. This means "occupied" will remain "false" throughout the code.
//Feel free to implement that feature if needed BUT it is not required for the servo part of the
//bullet refilling system to work.

//- Aslan

#include <Servo.h>
// servo signal pin, switch pin, led indicator pin
// NOTE: Changed the pushbutton pins to 3 and 2 because those two allow for interrupts
#define LEFT_DEFAULT {9, 3, 7, false, gate_left}
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
  int current_position;
  unsigned long prevMilli;  //  store the time when the door is opened
};
Device right = RIGHT_DEFAULT;
Device left = LEFT_DEFAULT;

int secondsToDelay = 2; // in seconds
unsigned long milliMax = 50*24*60*60;

void setup()
{
  //Setup the control and state of the Right gate
  pinMode(right.pin_switch, INPUT_PULLUP);
  pinMode(right.pin_servo, OUTPUT);
  pinMode(right.pin_indicator, OUTPUT);
  digitalWrite(right.pin_indicator, LOW);
  right.servo.attach(right.pin_servo);
  right.servo.write(right.ref.CLOSE);
  right.current_position = right.ref.CLOSE;
  right.prevMilli = milliMax;
  delay(15);

  //Setup the control and state of the Left gate
  pinMode(left.pin_switch, INPUT_PULLUP);
  pinMode(left.pin_servo, OUTPUT);
  pinMode(left.pin_indicator, OUTPUT);
  digitalWrite(left.pin_indicator, LOW);
  left.servo.attach(left.pin_servo);
  left.servo.write(left.ref.CLOSE);
  left.current_position = left.ref.CLOSE;
  left.prevMilli = milliMax;
  delay(15);
  
  // external and timer interrupt for both servos
  //Arduino uses a prescaler of 64 by default. So, it will
  //count once per (1/16M)*64 seconds. In OCR0A, we set the
  //Arduino to count up to 255 times before calling the
  //timed interrupt. Because this is implemented with timer0,
  //OCR0A cannot exceed 0-255. Timer1, however, can
  //but the Servo library uses it so it's best not
  //to mess with that else the servos malfunctions.

  //- Aslan
 
   TCCR0A = 0;
   TCNT0 = 0;
   OCR0A = 255;
   TCCR0A |= (1<<WGM01); // enable the CTC mode
   TIMSK0 |= (1<<OCIE0A); //enable the interrupt

   // to make sure the first interrupt works
   digitalWrite(right.pin_switch, HIGH);
   digitalWrite(left.pin_switch, HIGH);
   attachInterrupt(digitalPinToInterrupt(right.pin_switch), right_open, FALLING);
   attachInterrupt(digitalPinToInterrupt(left.pin_switch), left_open, FALLING);
}

void loop(){
  // do stuff here idk
}

 //As I do not know how the whole system will work exactly, there are implementation details here
 //that can be changed. Currently, both "open()" functions are coded so that when the servos are at the "OPEN" angle,
 //multiple presses of the button would NOT extend the duration of the opened "door". For example, when the car
 //pushes the button (regardless of whether it holds the button or releases it after), the door will close after
 //approximately # seconds (changeable in the "secondsToDelay" variable above). However, when the door is opened,
 //repressing the button will not extend the seconds of the opened door. But, if this functionality is needed, then simply place
 //the line "[right/left].prevMilli = millis();" outside of the "if" statements in the "open" functions.
 //This way, repressing the button WILL extend the duration of the door opened by # seconds.
 
 //NOTE: When the door is closing, repressing the button will open the door at "OPEN" angle again. The things stated in the
 //paragraph above only applies when the door is opened at the maximum angle "OPEN" degrees.
 
 //- Aslan

void right_open()
{
  if (right.current_position != right.ref.OPEN)
  {
    digitalWrite(right.pin_indicator, HIGH);
    right.servo.write(right.ref.OPEN);
    right.current_position = right.ref.OPEN;
    right.prevMilli = millis();
  }
}

void left_open()
{
  if (left.current_position != left.ref.OPEN)
  {
    digitalWrite(left.pin_indicator, HIGH);
    left.servo.write(left.ref.OPEN);
    left.current_position = left.ref.OPEN;
    left.prevMilli = millis();
  }
}

ISR(TIMER0_COMPA_vect)
{
  unsigned long currentMilli = millis();
  if (currentMilli >= right.prevMilli + secondsToDelay*1000)
  {
    right.servo.write(right.ref.CLOSE);
    digitalWrite(right.pin_indicator, LOW);
    right.current_position = right.ref.CLOSE;
    right.prevMilli = milliMax;
  }
  if (currentMilli >= left.prevMilli + secondsToDelay*1000)
  {
    left.servo.write(left.ref.CLOSE);
    digitalWrite(left.pin_indicator, LOW);
    left.current_position = left.ref.CLOSE;
    left.prevMilli = milliMax;
  }
}

// for the curious or those who are just bored, I've included segments of counting method
/*
* const long compareCounter = 255;
*int secondsToDelay = 2;
*unsigned long countToValue = 2/((64/(16*pow(10,6)))*compareCounter);
*
* In setup():
* TCCR0A = 0;
* TCNT0 = 0;
* OCR0A = compareCounter;
* TCCR0A |= (1<<WGM01); // enable the CTC mode
* TIMSK0 |= (1<<OCIE0A); //enable the interrupt
* 
* In ISR
* ++counter;
* if (called == true && counter >= countToValue)
  {
     servo.write(0);
    counter = 0;
    called = false;
  }
*/
