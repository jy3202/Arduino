#include <Adafruit_NeoPixel.h>
#include <Servo.h>

#define soundSensorPin A2
#define rgbLEDPin 4
#define motorPin 7

#define blueColor 0x0000FF
#define greenColor 0x00FF00
#define whiteColor 0xFFFFFF

Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, rgbLEDPin, NEO_GRB + NEO_KHZ800); // Initialize with 10 LEDs
Servo myServo; // Create a servo object

// Declare soundData as a global variable
int soundData[100]; // Increased the number of samples
int recordedSamples;

unsigned long lastBreathTime = 0;
int breathInterval = 20; // Adjust the interval for the breathing effect

void setup() {
pinMode(soundSensorPin, INPUT);
pinMode(motorPin, OUTPUT);

strip.begin();
myServo.attach(motorPin); // Attach the servo to the motor pin

// Initial state
setColor(blueColor);
myServo.write(90); // Set the initial position of the servo to the center

Serial.begin(9600);
}

void loop() {
// State 1: Waiting for sound input
while (digitalRead(soundSensorPin) == LOW) {
// Breathing blue light during waiting
breathingBlue();
}

// State 2: Recording for 5 seconds with increased sensitivity, amplification, and intensity filtering
setColor(greenColor);
int maxIntensity = recordSound(5000, 50, 1.5, 0.5); // Record for 5 seconds with increased sensitivity, amplification, and intensity filtering

// State 3: Playback based on recorded sound intensity with limited servo motion
setColor(0); // Turn off RGB LEDs during playback
playbackRecording(maxIntensity, recordedSamples);

delay(1000); 
myServo.write(90);

// State 4: Turning off for 3 seconds
setColor(0); // Turn off the light
delay(3000); // Delay for 3 seconds

// State 5: Back to waiting, turn the light blue again
setColor(blueColor);
}

void setColor(uint32_t color) {
strip.setPixelColor(4, color); // Set the color of the middle LED
strip.show(); // Update the LED
}

void breathingBlue() {
unsigned long currentTime = millis();
if (currentTime - lastBreathTime >= breathInterval) {
static float brightness = 0;
float brightnessNormalized = (sin(brightness) + 1) / 2; // Apply sine function for non-linear change
int brightnessValue = brightnessNormalized * 255; // Scale to 0-255 range
strip.setBrightness(brightnessValue); // Set the brightness
strip.setPixelColor(4, blueColor); // Set the color to blue
strip.show(); // Update the LED
brightness += 0.1; // Adjust the speed of the breathing effect
lastBreathTime = currentTime;
}
}

int recordSound(int duration, int threshold, float amplification, float intensityFilter) {
// Capture sound data
int soundIntensity;
int maxIntensity = 0;

Serial.println("Recording...");
unsigned long startTime = millis();

int dataIndex = 0;

while (millis() - startTime < duration && dataIndex < sizeof(soundData)/sizeof(soundData[0])) {
soundIntensity = analogRead(soundSensorPin);
maxIntensity = max(maxIntensity, soundIntensity);

// Store sound intensity data in the array if above the threshold and intensity filter
if (soundIntensity > threshold && soundIntensity > intensityFilter * maxIntensity) {
// Amplify the sound intensity before storing in the array
soundData[dataIndex++] = int(amplification * soundIntensity);
}

// Print sound intensity during recording (optional)
Serial.print("Sound Intensity: ");
Serial.println(soundIntensity);

// Delay to control the sampling rate
delay(100);
}

recordedSamples = dataIndex;
Serial.println("Recording complete.");
return maxIntensity;
}
void playbackRecording(int maxIntensity, int recordedSamples) {
unsigned long startTime = millis();
int duration = 5000; // Playback duration in milliseconds

// Calculate the dynamic range of the recorded sound data
int minIntensity = soundData[0];
for (int i = 1; i < recordedSamples; i++) {
minIntensity = min(minIntensity, soundData[i]);
}
int dynamicRange = maxIntensity - minIntensity;

for (int dataIndex = 0; dataIndex < recordedSamples; dataIndex++) {
int soundIntensity = soundData[dataIndex];

// Normalize the intensity value to fit within the range of 0 to 255
int normalizedIntensity = map(soundIntensity, minIntensity, maxIntensity, 0, 255);

// Map the normalized intensity to servo position
int servoPosition = map(normalizedIntensity, 0, 255, 0, 180);
myServo.write(servoPosition);

// Vary the speed based on the frequency of peaks
int delayTime = map(dataIndex, 0, sizeof(soundData)/sizeof(soundData[0]), 200, 20);
delay(delayTime);
}

// Reset the servo motor to its original position (center)
myServo.write(90);

Serial.println("Playback complete.");
}