/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

#define stepPinLeftStepper 26  // yellow led
#define dirPinLeftStepper 25  // White led

#define dirPinRightStepper 17   // yellow led
#define stepPinRightStepper 16  // White led

int arrayLength = 4;
int pins[] = {
  dirPinLeftStepper, 
  stepPinLeftStepper, 
  dirPinRightStepper, 
  stepPinRightStepper,
};

void turnOnLights() {
  for (int i = 0; i < arrayLength; i++) {
    digitalWrite(pins[i], HIGH); 
  }
}

void turnOffLights() {
  for (int i = 0; i < arrayLength; i++) {
    digitalWrite(pins[i], LOW); 
  }
}


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  for (int i = 0; i < arrayLength; i++) {
    pinMode(pins[i], OUTPUT);
  }
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  Serial.println("Output");
  turnOnLights();
  delay(200);   
  turnOffLights();
  delay(200);                      // wait for a second
}
