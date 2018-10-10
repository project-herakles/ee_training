#include <Servo.h>

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

  int ltime=-1;


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
  attachInterrupt(digitalPinToInterrupt(2),timer_right,FALLING);
  



  //Setup the control and state of the Left gate

  pinMode(left.pin_switch, INPUT_PULLUP);

  pinMode(left.pin_servo, OUTPUT);

  pinMode(left.pin_indicator, OUTPUT);

  digitalWrite(left.pin_indicator, LOW);

  left.servo.attach(left.pin_servo);
  
  right.servo.write(right.ref.CLOSE);

  delay(15);

  attachInterrupt(digitalPinToInterrupt(3),timer_left,FALLING);
   TCCR0A = 0;

   TCNT0 = 0;

   OCR0A = 255;

   TCCR0A |= (1<<WGM01); // enable the CTC mode

   TIMSK0 |= (1<<OCIE0A); //enable the interrupt
 

  

  //Serial.begin(115200);

}



//Open the gate if the button state is swithced from OCCUPIED to VACANT, and close the gate automatically after 2 seconds.

void loop(){

  if (!right.occupied){

    right.occupied = check_occupied(right.pin_switch);

    if (right.occupied){

      digitalWrite(right.pin_indicator, HIGH);

      right.servo.write(right.ref.OPEN);


      delay(15);

      digitalWrite(right.pin_indicator, LOW);

    }

  } else {

    right.occupied = check_occupied(right.pin_switch);

  }

  if (!left.occupied){

    left.occupied = check_occupied(left.pin_switch);

    if (left.occupied){

      digitalWrite(left.pin_indicator, HIGH);

      left.servo.write(left.ref.OPEN);
       delay(15);

      digitalWrite(left.pin_indicator, LOW);



    }

  } else {

    left.occupied = check_occupied(left.pin_switch);

  }

}
void timer_right()
{

   digitalWrite(right.pin_indicator, HIGH);

    right.servo.write(right.ref.OPEN);
    rtime=0;
}
void timer_left()
{
    digitalWrite(right.pin_indicator, HIGH);

    left.servo.write(left.ref.OPEN);
    ltime=0;    
} 
ISR(TIMER0_COMPA_vect){
  if (rtime<2000){
    rtime++;
  }
  if (rtime==2000){
    right.servo.write(right.ref.CLOSE);
    digitalWrite(right.pin_indicator, LOW);
    rtime=0;
  }
   
   if (ltime<2000){
    ltime++;
   }
   if (ltime==2000){
     left.servo.write(left.ref.CLOSE);
     digitalWrite(left.pin_indicator, LOW);
     ltime=0;
   }
    
       
        
  }

  


  
