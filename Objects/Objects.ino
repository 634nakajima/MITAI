/*
  Analog input, analog output, serial output
 
 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.
 
 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground
 
 created 29 Dec. 2008
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
 
 */

// These constants won't change.  They're used to give names
// to the pins used:
const int fanIn = A0;  // Analog input pin that the potentiometer is attached to
const int drawerIn = A1;
const int lightIn = A2;

const int fanOut = 3; // Analog output pin that the LED is attached to
const int lightOut = 6;

int fanValue = 0;        // value read from the pot
int drawerValue = 0;        // value output to the PWM (analog out)
int lightValue = 0;

int prefV = 0;        // value read from the pot
int predV = 0;        // value output to the PWM (analog out)
int prelV = 0;

int fanOutValue = 0;
int drawerOutValue = 0;
int lightOutValue = 0;

byte preFOV = 0;
byte preLOV = 0;
byte preDOV = 0;

byte fanData[2], drawerData[2], lightData[2];
byte objectID, rcvVal;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  fanData[0] = 0;
  lightData[0] = 1;
  drawerData[0] = 2;
}

void loop() {
  // read the analog in value:
  fanValue = (analogRead(fanIn)+prefV)/2;
  drawerValue = (0.25*analogRead(drawerIn)+0.75*predV);            
  lightValue = (analogRead(lightIn)+prelV)/2;             

  prefV = fanValue;
  predV = drawerValue;
  prelV = lightValue;
  
  if (drawerValue < 150) drawerValue = 150;
  if (drawerValue > 680) drawerValue = 680;
  if (lightValue < 30) lightValue = 0;
  if (fanValue < 30) fanValue = 0;

  preFOV = (int)fanData[1];
  preLOV = (int)lightData[1];
  preDOV = (int)drawerData[1];
  
  // map it to the range of the analog out:
  fanOutValue = map(fanValue, 0, 1023, 0, 15)*17;
  drawerOutValue = map(drawerValue, 150, 690, 0, 15)*17;
  lightOutValue = map(lightValue, 0, 1023, 0, 15)*17;
  
  fanData[1] = (byte)fanOutValue;//(((int)fanOutValue+(int)preFOV)/2);
  drawerData[1] = (byte)drawerOutValue;
  lightData[1] = (byte)lightOutValue;
  
  if(preFOV != fanData[1]) {
    Serial.write((byte *)fanData, sizeof(byte)*2);
    delay(10);
  }
  if(preLOV != lightData[1]) {
    Serial.write((byte *)lightData, sizeof(byte)*2);
      delay(10);
  }
  if(preDOV != drawerData[1]) {
    Serial.write((byte *)drawerData, sizeof(byte)*2);
    delay(10);
  }
  
  if (Serial.available() > 1) {
    while(Serial.available()) {
      objectID = Serial.read();
      rcvVal = Serial.read();
    
      switch(objectID) {
        case 0:
          analogWrite(fanOut, rcvVal);
          delay(10);
          break;
        case 1:
          analogWrite(lightOut, rcvVal);
          delay(10);
          break;
        default:
          break;
      }
    }
  }
  delay(30);                     
}
