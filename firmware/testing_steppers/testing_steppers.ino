#include "AccelStepper.h"

// Motor interface type must be set to 1 when using a driver:
#define motorInterfaceType 1

// Left stepper motor connections
#define stepPinLeftStepper 26  // top pin
#define dirPinLeftStepper 25  // bottom pin

// Right stepper motor connections
#define dirPinRightStepper 17
#define stepPinRightStepper 16 

//This changes according to total step size for 1 turn of the stepper.
//Current configuration: 3200 step for 1 complete turn. 1 turn is 2*PI radians.
//radiantToStep: 3200/(2*PI)
#define radianToStep 509.2 

int incomingByte = 0;

// Create instances of the AccelStepper class:
AccelStepper leftStepper = AccelStepper(motorInterfaceType, stepPinLeftStepper, dirPinLeftStepper);
AccelStepper rightStepper = AccelStepper(motorInterfaceType, stepPinRightStepper, dirPinRightStepper);
int leftMoving = 0;
int rightMoving = 0;

void setup() {
  // setupDebugStuff();
  // Set the maximum speed and acceleration:
  leftStepper.setMaxSpeed(200);
  leftStepper.setAcceleration(30);
  leftStepper.moveTo(24);

  rightStepper.setMaxSpeed(300);
  rightStepper.setAcceleration(200);
  rightStepper.moveTo(24);
}

void loop() {
  // loopDebugStuff();
  Serial.begin(9600);
  if (leftStepper.distanceToGo() == 0)
    leftStepper.moveTo(-leftStepper.currentPosition());
  if (rightStepper.distanceToGo() == 0)
    rightStepper.moveTo(-rightStepper.currentPosition());
  // Run to target position with set speed and acceleration/deceleration:
  leftStepper.run();
  rightStepper.run();
}