// This program interrupts the robot with a STOP command if one of the proximity sensors on the bottom
// detects a significant difference in floor elevation

#include <Servo.h>

Servo myservo;

const int PINS_LEN = 4;
const int SENSOR_THRESHOLD = 900;

int pins [] = {A0,A1,A2,A3};
void setup() {
  myservo.attach(6);
  for (int i = 0; i < PINS_LEN; i++) {
    pinMode(pins[i], INPUT);
  }
  attachInterrupt(2, sensorRead, RISING);
  Serial.begin(9600);
  
}

void sensorRead(){
  for(int i = 0; i < PINS_LEN; i++){
    if(analogRead(pins[i]) > SENSOR_THRESHOLD){
      Serial.println("STOP");
      
      break;
    }
  }
}

void loop() {
  Serial.println(analogRead(A0));

 /* for (int pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);

  }*/
}
