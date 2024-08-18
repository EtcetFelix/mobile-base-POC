#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <AccelStepper.h>
#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/float32.h>

rcl_publisher_t publisher;
std_msgs__msg__Int32 msg;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;

//Subscribers for left and right motor.
rcl_subscription_t r_subscriber;
rcl_subscription_t l_subscriber;

// Motor interface type must be set to 1 when using a driver:
#define motorInterfaceType 1

// Left stepper motor connections
#define stepPinLeftStepper 26
#define dirPinLeftStepper 25
// Right stepper motor connections
#define dirPinRightStepper 17
#define stepPinRightStepper 16 
 
AccelStepper leftStepper = AccelStepper(motorInterfaceType, stepPinLeftStepper, dirPinLeftStepper);
AccelStepper rightStepper = AccelStepper(motorInterfaceType, stepPinRightStepper, dirPinRightStepper);

//Message types for subscription. The unit is Radian per Second.
std_msgs__msg__Float32 l_msg;
std_msgs__msg__Float32 r_msg;

//This changes according to total step size for 1 turn of the stepper.
//Current configuration: 3200 step for 1 complete turn. 1 turn is 2*PI radians.
//radiantToStep: 3200/(2*PI)
float pi = 3.141592653589793;
int steps_in_stepper  = 200;
#define radianToStep (steps_in_stepper/(2*pi))

#define LED_PIN 13

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}


void error_loop(){
  while(1){
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
}

void timer_callback(rcl_timer_t * timer, int64_t last_call_time)
{  
  RCLC_UNUSED(last_call_time);
  if (timer != NULL) {
    RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
    msg.data++;
  }
}

void step_steppers() {
  leftStepper.run();
  rightStepper.run();
}

void apply_steps_to_steppers(AccelStepper stepper, float steps_to_move) {
  //Apply the change
  if(steps_to_move == 0){
    stepper.stop(); //Stop
  }
  else if(steps_to_move < 0){
    stepper.move(steps_to_move); //For negative values run backwards
  }
  else{
    stepper.move(steps_to_move); //For positive values run forwards
  }
}

//Callback function for left motor
void subscription_callback_left_motor(const void * msgin)
{  
  //Unpack incoming message
  const std_msgs__msg__Float32 * msg = (const std_msgs__msg__Float32 *)msgin;
  
  //Calculate the speed 
  uint32_t speed = (uint32_t)(fabs(msg->data)*radianToStep);
  
  //Update the desired speed
  leftStepper.setSpeed(speed);
  apply_steps_to_steppers(leftStepper, msg->data);
  
}

//Callback function for right motor.
void subscription_callback_right_motor(const void * msgin)
{  
  //Unpack incoming message
  const std_msgs__msg__Float32 * msg = (const std_msgs__msg__Float32 *)msgin;

  //Calculate the speed 
  uint32_t speed = (uint32_t)(fabs(msg->data)*radianToStep);

  //Update the desired speed
  rightStepper.setSpeed(speed);

  apply_steps_to_steppers(rightStepper, msg->data);
}

void setup() {
  set_microros_transports();
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  
  
  delay(2000);

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_arduino_node", "", &support));

  // create publisher
  RCCHECK(rclc_publisher_init_default(
    &publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "micro_ros_arduino_node_publisher"));

  // create timer,
  const unsigned int timer_timeout = 1000;
  RCCHECK(rclc_timer_init_default(
    &timer,
    &support,
    RCL_MS_TO_NS(timer_timeout),
    timer_callback));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_timer(&executor, &timer));
  //Init subscriptions  
  RCCHECK(rclc_executor_add_subscription(&executor, &l_subscriber, &l_msg, &subscription_callback_left_motor, ON_NEW_DATA));
  RCCHECK(rclc_executor_add_subscription(&executor, &r_subscriber, &r_msg, &subscription_callback_right_motor, ON_NEW_DATA));

  msg.data = 0;
}

void loop() {
  delay(100);
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
  step_steppers();
}
