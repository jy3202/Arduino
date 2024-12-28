// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
#include "Wire.h"
#include <Servo.h>
Servo myservo1;
Servo myservo2;
int pos;
#include "DHT.h"
#define DHTTYPE DHT20   // DHT 20
/*Notice: The DHT10 and DHT20 is different from other DHT* sensor ,it uses i2c interface rather than one wire*/
/*So it doesn't require a pin.*/
DHT dht(DHT20);         //   DHT10 DHT20 don't need to define Pin
#include "rgb_lcd.h"     // include Seeed Studio LCD library
#include "Adafruit_NeoPixel.h"
#define RANGERPIN      5
#define NUMPIXELS      10
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, RANGERPIN, NEO_GRB + NEO_KHZ800);
rgb_lcd lcd;
const int  buttonPin = 6;    // the pin that the pushbutton is attached to
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int lightState1 = 0;
int lightState2 = 0;
int lightState3 = 0;
int lightsStay = 0;
int startingh;
const int buzzer = 2;
#if defined(ARDUINO_ARCH_AVR)
#define debug  Serial
#elif defined(ARDUINO_ARCH_SAMD) ||  defined(ARDUINO_ARCH_SAM)
#define debug  SerialUSB
#else
#define debug  Serial
#endif
void setup() {
  debug.begin(9600);
  Wire.begin();
  dht.begin();
  lcd.begin(16, 2);
  myservo1.attach(3);
  myservo2.attach(7);
  pixels.setBrightness(10);
  pixels.begin();
  pinMode(buttonPin, INPUT);
  startingh = dht.readHumidity();
  pinMode(buzzer, OUTPUT);
}
void loop() {
  float temp_hum_val[2] = {0};
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  myservo1.write(0);
  myservo2.write(0);
  pixels.clear();
  delay(100);
  char _buffer[7];
  int centimeters;
  int seconds;
  int threshold = 60;
  // int newthreshold = dht.readTempAndHumidity(temp_hum_val);
  int state = 1;
  float h = dht.readHumidity();
  float hincrease = (h-startingh)/startingh;
  buttonState = digitalRead(buttonPin);
  Serial.print("buttonState: ");
  Serial.print(buttonState);
  Serial.print(" %\t");
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      buttonPushCounter++;
    }
    else {
    }
  }  
  if (buttonPushCounter % 2 == 0) {
    if (buttonState == HIGH) {
          // if the current state is HIGH then the button
          // wend from off to on:
          tone(buzzer, 780); // Send 1KHz sound signal...
          delay(500);        // ...for 1 sec
          noTone(buzzer);     // Stop sound...
        }
        else {
        }
    if (!dht.readTempAndHumidity(temp_hum_val)) {
        debug.print("Humidity1: ");
        debug.print(temp_hum_val[0]);
        debug.print(" %\t");
        debug.print("Temperature: ");
        debug.print(temp_hum_val[1]);
        debug.println(" *C");
        Serial.print("Humidity2: ");
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print(hincrease);
        // check threshold is exceeded
        lightState1 = lightState1 + 50;
        lightState2 = lightState2 + 20;
        lightState3 = lightState3 - 50;
        if(hincrease>0.05){
          if (seconds<5) {
            for (pos = 0; pos <= 45; pos += 1) { // goes from 0 degrees to 180 degrees
              // in steps of 1 degree
              myservo1.write(pos);              // tell servo to go to position in variable 'pos'
              delay(10);                       // waits 15ms for the servo to reach the position
            }
            for (pos = 45; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
              myservo1.write(pos);              // tell servo to go to position in variable 'pos'
              delay(10);                       // waits 15ms for the servo to reach the position
            }
            for (pos = 0; pos <= 45; pos += 1) { // goes from 0 degrees to 180 degrees
              // in steps of 1 degree
              myservo2.write(pos);              // tell servo to go to position in variable 'pos'
              delay(10);                       // waits 15ms for the servo to reach the position
            }
            for (pos = 45; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
              myservo2.write(pos);              // tell servo to go to position in variable 'pos'
              delay(10);                       // waits 15ms for the servo to reach the position
            }
            pixels.setPixelColor(0, pixels.Color(lightState1, lightState2, lightState3));
            pixels.show();
            delay (1000);
            seconds = seconds + 1;
              for (pos = 0; pos <= 45; pos += 1) { // goes from 0 degrees to 180 degrees
                // in steps of 1 degree
                myservo1.write(pos);              // tell servo to go to position in variable 'pos'
                delay(10);                       // waits 15ms for the servo to reach the position
              }
              for (pos = 45; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
                myservo1.write(pos);              // tell servo to go to position in variable 'pos'
                delay(10);                       // waits 15ms for the servo to reach the position
              }
              for (pos = 0; pos <= 45; pos += 1) { // goes from 0 degrees to 180 degrees
                // in steps of 1 degree
                myservo2.write(pos);              // tell servo to go to position in variable 'pos'
                delay(10);                       // waits 15ms for the servo to reach the position
              }
              for (pos = 45; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
                myservo2.write(pos);              // tell servo to go to position in variable 'pos'
                delay(10);                       // waits 15ms for the servo to reach the position
              }
              pixels.setPixelColor(2, pixels.Color(lightState1, lightState2, lightState3));
              pixels.show();
              delay (1000);
              seconds = seconds + 1;
                for (pos = 0; pos <= 45; pos += 1) { // goes from 0 degrees to 180 degrees
                  // in steps of 1 degree
                  myservo1.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(10);                       // waits 15ms for the servo to reach the position
                }
                for (pos = 45; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
                  myservo1.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(10);                       // waits 15ms for the servo to reach the position
                }
                for (pos = 0; pos <= 45; pos += 1) { // goes from 0 degrees to 180 degrees
                  // in steps of 1 degree
                  myservo2.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(10);                       // waits 15ms for the servo to reach the position
                }
                for (pos = 45; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
                  myservo2.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(10);                       // waits 15ms for the servo to reach the position
                }
                pixels.setPixelColor(4, pixels.Color(lightState1, lightState2, lightState3));
                pixels.show();
                delay (1000);
                seconds = seconds + 1;
                  for (pos = 0; pos <= 45; pos += 1) { // goes from 0 degrees to 180 degrees
                    // in steps of 1 degree
                    myservo1.write(pos);              // tell servo to go to position in variable 'pos'
                    delay(10);                       // waits 15ms for the servo to reach the position
                  }
                  for (pos = 45; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
                    myservo1.write(pos);              // tell servo to go to position in variable 'pos'
                    delay(10);                       // waits 15ms for the servo to reach the position
                  }
                  for (pos = 0; pos <= 45; pos += 1) { // goes from 0 degrees to 180 degrees
                    // in steps of 1 degree
                    myservo2.write(pos);              // tell servo to go to position in variable 'pos'
                    delay(10);                       // waits 15ms for the servo to reach the position
                  }
                  for (pos = 45; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
                    myservo2.write(pos);              // tell servo to go to position in variable 'pos'
                    delay(10);                       // waits 15ms for the servo to reach the position
                  }
                  pixels.setPixelColor(6, pixels.Color(lightState1, lightState2, lightState3));
                  pixels.show();
                  delay (1000);
                  seconds = seconds+1;
                    for (pos = 0; pos <= 45; pos += 1) { // goes from 0 degrees to 180 degrees
                      // in steps of 1 degree
                      myservo1.write(pos);              // tell servo to go to position in variable 'pos'
                      delay(10);                       // waits 15ms for the servo to reach the position
                    }
                    for (pos = 45; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
                      myservo1.write(pos);              // tell servo to go to position in variable 'pos'
                      delay(10);                       // waits 15ms for the servo to reach the position
                    }
                    for (pos = 0; pos <= 45; pos += 1) { // goes from 0 degrees to 180 degrees
                      // in steps of 1 degree
                      myservo2.write(pos);              // tell servo to go to position in variable 'pos'
                      delay(10);                       // waits 15ms for the servo to reach the position
                    }
                    for (pos = 45; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
                      myservo2.write(pos);              // tell servo to go to position in variable 'pos'
                      delay(10);                       // waits 15ms for the servo to reach the position
                    }
                    pixels.setPixelColor(8, pixels.Color(lightState1, lightState2, lightState3));
                    pixels.show();
                    delay (1000);
                    seconds = seconds+1;
                    lightsStay = 1;  
                    delay (5000);
        }
      }
    }
  }
  else {
    for (int ii = 0; ii < 10; ++ii) {
      pixels.setPixelColor(ii, 0, 0, 0);
      lightState1 = 0;
      lightState2 = 0;
      lightState3 = 0;
    }
      pixels.show();
    if (buttonState == HIGH) {
          // if the current state is HIGH then the button
          // wend from off to on:
          tone(buzzer, 200); // Send 1KHz sound signal...
          delay(500);        // ...for 1 sec
          noTone(buzzer);     // Stop sound..
    }
    if(digitalRead(!state)){
      delay(50); // remove switch debounce
      state = 1;
    }
  }
}
