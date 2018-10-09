#include <Servo.h>
#include <MsTimer2.h>
Servo gateServo_A;
Servo gateServo_B;

const int led_a = 9;
const int led_b = 10;
bool sta_a = false;
bool sta_b = false;
const int button_a=5;
const int button_b=6;
//These are the button to be pressed to control the gate
unsigned int time_open_a=0;
unsigned int time_open_b=0;
//These two store the time of opening the corresponding gate

const unsigned int interval=2000;
//the time of holding the gate to be open




//declaration of ISR

void open_gate_a();
void open_gate_b();
void close_gate_a();
void close_gate_();
void setup() {
    Serial.begin(9600);
    pinMode(led_a,OUTPUT);
    digitalWrite(led_a,LOW);
    pinMode(led_b,OUTPUT);
    digitalWrite(led_b,LOW);
    pinMode(button_a,INPUT_PULLUP);
    pinMode(button_b,INPUT_PULLUP);
    gateServo_A.attach(9);
    gateServo_B.attach(10);
    
    attachInterrupt(digitalPinToInterrupt(button_a),open_gate_a,FALLING);
    attachInterrupt(digitalPinToInterrupt(button_b),open_gate_b,FALLING);
    
    MsTimer2::set(1,check);
    MsTimer2::start();
}
//计时器每0.01秒a,b就加一
void check()
{
  if(sta_a == true){
    time_open_a++;
    if(time_open_a >= interval){
      close_gate_a();
    }
  }
  if(sta_b == true){
    time_open_b++;
    if(time_open_b >= interval){
      close_gate_b();
    }
  }
} 

void loop() {
}

void open_gate_a(){
    if(sta_a == false){
      gateServo_A.write(90);
      digitalWrite(led_a,HIGH);
      sta_a = true;
      time_open_a=0;
    }
}

void open_gate_b(){
    if(sta_b == false){
      gateServo_B.write(90);
      digitalWrite(led_b,HIGH);
      sta_b = true;
      time_open_b=0;
    }
}

void close_gate_a(){
      gateServo_A.write(0);
      digitalWrite(led_a,LOW);
      sta_a = false;
      time_open_a=0;
}

void close_gate_b(){
      gateServo_B.write(0);
      digitalWrite(led_b,LOW);
      sta_b = false;
      time_open_b=0;
}
