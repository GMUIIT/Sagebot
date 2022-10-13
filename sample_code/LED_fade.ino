const byte led_gpio = 2; // the PWM pin the LED is attached to
const byte led_gpio2 = 15;
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

// the setup routine runs once when you press reset:
void setup() {
  ledcAttachPin(led_gpio, 0); // assign a led pins to a channel
  ledcAttachPin(led_gpio2, 1);
  // Initialize channels
  // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
  // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
  ledcSetup(0, 4000, 8); // 12 kHz PWM, 8-bit resolution

  ledcSetup(1, 4000, 8);
}

// the loop routine runs over and over again forever:
void loop() {
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

//TODO Functionalities:
//Gradually Brighten
//Gradually dim
//Instantly flash
//Instant off