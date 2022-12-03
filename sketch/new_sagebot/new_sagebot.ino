#include "TwoWayMotor.h"
#include "BLEUart.h"

// Function prototypes
void handleIncoming(std::string &command);
void lightworks(int LEDconfig);
void OnAndOff();
void brighten();
void dim();
void sensorRead();
void off();
void on();
// Motors
// Pins are IN1, IN2, ENA
TwoWayMotor left(22, 1, 19);
TwoWayMotor right(25, 26, 12);

// BLE
BLEUart BLE("Sagebot", handleIncoming);

/*LED Global Variable
  0 = Flash on and off slowly
  1 = Gradually Brighten
  2 = Gradually Dim
  3 = Instant Flash
  4 = Turn lights off
*/ 
int LEDconfig = 0;
// LED connections
const byte led_gpio = 2; // the PWM pin the LED is attached to
const byte led_gpio2 = 15;
// LED Light controls
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
// PWM parameters for LED
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;

// IR Sensor Variables
const int PINS_LEN = 4;
const int SENSOR_THRESHOLD = 900;
int pins [] = {0,1,2,3};

void handleIncoming(std::string &command) {
  // Echo command to serial for debugging purposes.
  Serial.println("*************");
  Serial.printf("Received value: %s\n", command.c_str());
  Serial.println("*************");
  
  // First character is always the command name.
  // Min length is 1 (S)
  if (command.length() < 1) {
    return;
  }
  // int cmd = (command.at(2) - '0') * (command.at(3) - '0');
  char cmd = command.at(0);
  switch (cmd) {
    case 'T':
      for (int i = 0; i < 3; i++) {
        BLE.write("Moving forwards.\n");
        left.write(255);
        right.write(255);

        delay(2000);
        
        BLE.write("Moving back.\n");
        left.write(-255);
        right.write(-255);

        delay(2000);
              
        BLE.write("Turning left.\n");
        left.write(255);
        right.write(-255);

        delay(2000);
              
        BLE.write("Turning right.\n");
        left.write(-255);
        right.write(255);

        delay(2000);
      }
      
      break;
    case 'F':
      BLE.write("Moving forwards.");
      Serial.println("Moving Forward");
      left.write(255);
      right.write(255);

      break;
    case 'B':
      BLE.write("Moving back.");
      Serial.println("Moving Backwards");
      left.write(-255);
      right.write(-255);
    
      break;
    case 'L':
      BLE.write("Turning left.");
      Serial.println("Turning Left");
      left.write(255);
      right.write(-255);
      
      break;
    case 'R':
      BLE.write("Turning right.");
      Serial.println("Turning Right");
      left.write(-255);
      right.write(255);
      
      break;
    case 'S':
      BLE.write("Stopping.");
      Serial.println("Motor stopped");
      left.write(0);
      right.write(0);
      
      break;
    default:
      BLE.write("Unknown command " + command);
      break;
  }
  
}


void setup() {

  // Initialize channels
  // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
  ledcSetup(0, 4000, 8); // 12 kHz PWM, 8-bit resolution
  //ledcSetup(1, 4000, 8);
  ledcSetup(pwmChannel, freq, resolution);

  //Set up the LEDs.
  //ledcAttachPin(led_gpio, 1); // assign a led pins to a channel
  ledcAttachPin(led_gpio2, 2);
  
  // Set up the IR sensor.
  /*
  for (int i = 0; i < PINS_LEN; i++) {
    pinMode(pins[i], INPUT);
  }
  */
  attachInterrupt(2, sensorRead, RISING);
  Serial.begin(9600);
  
  //Setup motors
  left.init();
  right.init();

  // Setup Bluetooth
  BLE.init();
}

void loop() {
  //TODO: Actually test with this...
  //Reads the IR sensor to detect if Sagebot will fall.
  //sensorRead();
  //lightworks(LEDconfig);
}

// IR Sensor reading.
void sensorRead(){
  for(int i = 0; i < PINS_LEN; i++){
    if(analogRead(pins[i]) > SENSOR_THRESHOLD){
      std::string s("S");
      Serial.println("SENSOR STOP!");
      handleIncoming(s);
      break;
    }
  }
}

/*****************************************************/
/*              LED-MANAGEMENT FUNCTIONS             */
/*****************************************************/
// TODO: turn this into a class.  One class per LED?

// Light managing function.
void lightworks(int LEDconfig){
  switch (LEDconfig)
  {
  case '0':
    OnAndOff();
    break;
  case '1':
    brighten();
    break;
  case '2':
    dim();
    break;
  case '3':
    on();
    break;
  case '4':
    off();
    break;
  default:
    OnAndOff();
    break;
  }
}

// LEDconfig 0. Flashes the LEDs on and off slowly.
void OnAndOff() {
  ledcWrite(0, brightness); // set the brightness of the LED
  ledcWrite(1, brightness);
  
  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  // wait for 30 milliseconds to see the dimming effect
  delay(30);
}

// LEDconfig 1. Slowly Brighten the LEDs.
void brighten() {
  ledcWrite(0, brightness); // set the brightness of the LED
  ledcWrite(1, brightness);
  
  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // wait for 30 milliseconds to see the dimming effect
  delay(30);
}

// LEDconfig 2. Slowly dim the LEDs.
void dim() {
  ledcWrite(0, brightness); // set the brightness of the LED
  ledcWrite(1, brightness);
  
  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  // wait for 30 milliseconds to see the dimming effect
  delay(30);
}

// LEDconfig 3. Instantly flash the LEDs.
void on() {
  ledcWrite(0, 255); // set the brightness of the LED
  ledcWrite(1, 255);
}

// LEDconfig 4. Turns off the LEDs.
void off() {
  ledcWrite(0, 0); // set the brightness of the LED
  ledcWrite(1, 0);
}
