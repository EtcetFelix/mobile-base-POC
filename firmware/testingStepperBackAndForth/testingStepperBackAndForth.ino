// MultiStepper.pde
// -*- mode: C++ -*-
//
// Shows how to multiple simultaneous steppers
// Runs one stepper forwards and backwards, accelerating and decelerating
// at the limits. Runs other steppers at the same time
//
// Copyright (C) 2009 Mike McCauley
// $Id: MultiStepper.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $
 
#include <AccelStepper.h>

// Motor interface type must be set to 1 when using a driver:
#define motorInterfaceType 1

// Left stepper motor connections
#define dirPinLeftStepper 2
#define stepPinLeftStepper 3

// Right stepper motor connections
#define dirPinRightStepper 4
#define stepPinRightStepper 5
 
AccelStepper leftStepper = AccelStepper(motorInterfaceType, stepPinLeftStepper, dirPinLeftStepper);
AccelStepper rightStepper = AccelStepper(motorInterfaceType, stepPinRightStepper, dirPinRightStepper);
 
int goalPos = 50;
int goalDir = 1;  // 1 for pos, -1 for neg
int leftMoving = 0;
int rightMoving = 0;

int incomingByte = 0;


void setup()
{  
    Serial.begin(9600);
    leftStepper.setMaxSpeed(200.0);
    leftStepper.setAcceleration(100.0);
    leftStepper.moveTo(goalPos);
    
    rightStepper.setMaxSpeed(300.0);
    rightStepper.setAcceleration(100.0);
    rightStepper.moveTo(goalPos);
}

void moveLeftStepperBackAndForth() {
  if (leftStepper.currentPosition() > 0) {
      leftStepper.move(-goalPos*2);
    }
  else if (leftStepper.currentPosition() < 0) {
    leftStepper.move(goalPos*2);
  }
  leftMoving = 1;
}

void moveLeftStepperToDirection() {
  int leftGoalDir = goalDir;
  leftStepper.move(goalPos*2*leftGoalDir);
  leftMoving = 1;
}

void oscillateStepper() {
  if (leftStepper.currentPosition() > 0) {
      goalDir = -1;
    }
  else if (leftStepper.currentPosition() < 0) {
    goalDir = 1;
  }
  moveLeftStepperToDirection();
}
 
void loop()
{
    // Change direction at the limits
    // if (leftStepper.distanceToGo() == 0)
    //     leftStepper.moveTo(-leftStepper.currentPosition());
    // if (rightStepper.distanceToGo() == 0)
    //     rightStepper.moveTo(-rightStepper.currentPosition());
    if (Serial.available() > 0) {
      incomingByte = Serial.read();
      if (leftStepper.distanceToGo() == 0) {
        Serial.println("Received command, moving left");
        Serial.print("left pos: ");
        Serial.println(leftStepper.currentPosition());
        // moveLeftStepperBackAndForth();
        if (incomingByte == 119) {
          Serial.println("Moving Forward");
          goalDir = 1;
        }
        if (incomingByte == 115) {
          Serial.println("Moving Backward");
          goalDir = -1;
        }
        moveLeftStepperToDirection();
      } 
    }
    if (leftStepper.distanceToGo() == 0 && leftMoving) {
      leftMoving = 0;
      Serial.print("leftStepper position: ");
      Serial.println(leftStepper.currentPosition());
    }
    if (rightStepper.distanceToGo() == 0)
        rightStepper.moveTo(-rightStepper.currentPosition());
        rightMoving = 1;
    leftStepper.run();
    rightStepper.run();
}
