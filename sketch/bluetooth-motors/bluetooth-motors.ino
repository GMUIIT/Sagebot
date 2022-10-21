/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
   Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE" 
   Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   In this example rxValue is the data received (only accessible inside that function).
   And txValue is the data to be sent, in this example just a byte incremented every second. 
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <ESP32Servo.h>

// BLE globals
BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

// Motor A
int motor1Pin1 = 23; 
int motor1Pin2 = 21; 
int enable1Pin = 22;

// Motor B
int motor2Pin1 = 19;
int motor2Pin2 = 18; 
int enable2Pin = 5;

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

// IR Sensor Variables
Servo myservo;
const int PINS_LEN = 4;
const int SENSOR_THRESHOLD = 900;
int pins [] = {0,1,2,3};

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

/**
 * Write the given string to the virtual serial port.
 */
void writeString(std::string str) {
    pTxCharacteristic->setValue((uint8_t*)str.c_str(), str.length());
    pTxCharacteristic->notify();
    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
}

void handleIncoming(std::string &command) {
  // First character is always the command name.
  // Min length is 1 (S)
  if (command.length() < 1) {
    return;
  }
  // int cmd = (command.at(2) - '0') * (command.at(3) - '0');
  char cmd = command.at(0);
  switch (cmd) {
    case 'F':
      writeString("Moving forwards.");
      Serial.println("Moving Forward");
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW);
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, HIGH);
      break;
    case 'B':
      writeString("Moving back.");
      Serial.println("Moving Backwards");
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, HIGH); 
      digitalWrite(motor2Pin1, HIGH);
      digitalWrite(motor2Pin2, LOW);
      break;
    case 'L':
      writeString("Turning left.");
      Serial.println("Turning Left");
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, HIGH); 
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, HIGH); 
      break;
    case 'R':
      writeString("Turning right.");
      Serial.println("Turning Right");
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW); 
      digitalWrite(motor2Pin1, HIGH);
      digitalWrite(motor2Pin2, LOW);
      break;
    case 0:
      writeString("Stopping.");
      Serial.println("Motor stopped");
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, LOW);
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, LOW);
      break;
    default:
      writeString("Unknown command " + command);
      break;
  }
  
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++)
          Serial.print(rxValue[i]);

        Serial.println();
        Serial.println("*********");
        handleIncoming(rxValue);
      }
    }
};

void setup() {

  //Set up the LEDs.
  ledcAttachPin(led_gpio, 0); // assign a led pins to a channel
  ledcAttachPin(led_gpio2, 1);

  // Initialize channels
  // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
  // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
  ledcSetup(0, 4000, 8); // 12 kHz PWM, 8-bit resolution
  ledcSetup(1, 4000, 8);

  // Set up the IR sensor.
  myservo.attach(6);
  for (int i = 0; i < PINS_LEN; i++) {
    pinMode(pins[i], INPUT);
  }
  attachInterrupt(2, sensorRead, RISING);
  Serial.begin(9600);

  // Setup Bluetooth
  //Serial.begin(115200);

  //////// BLE init
  // Create the BLE Device
  BLEDevice::init("Sagebot");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
										CHARACTERISTIC_UUID_TX,
										BLECharacteristic::PROPERTY_NOTIFY
									);
                      
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
											 CHARACTERISTIC_UUID_RX,
											BLECharacteristic::PROPERTY_WRITE
										);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {

  //TODO: Actually test with this...
  //Reads the IR sensor to detect if Sagebot will fall.
  //sensorRead();
  //lightworks(LEDconfig);

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
		    // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}

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

// IR Sensor reading.
void sensorRead(){
  for(int i = 0; i < PINS_LEN; i++){
    if(analogRead(pins[i]) > SENSOR_THRESHOLD){
      std::string s("S");
      Serial.println("STOP");
      handleIncoming(s);
      break;
    }
  }
}
