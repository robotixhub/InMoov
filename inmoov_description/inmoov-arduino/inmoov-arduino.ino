#include <Servo.h>

#include <ros.h>
#include <std_msgs/Int32.h>
#include <sensor_msgs/JointState.h>

ros::NodeHandle node_handle;

Servo robot_servos[6];

int servo_pins[6] = {11,10, 9, 6, 5, 3}; 
int mid_positions[6] = {180,180,180,180,180,180};
int SERVO_CURRENT_POSITIONS[6];

float TARGET_JOINT_POSITIONS[6] = {0,0,0,0,0,0};

void writeServos() {
  for (int j = 0; j < 6; j++) {
    int target_angle;
    if (j == 2) {
     
      target_angle = - TARGET_JOINT_POSITIONS[j]*(180/3.14) + mid_positions[j];
    } else {
      target_angle = TARGET_JOINT_POSITIONS[j]*(180/3.14) + mid_positions[j];
    }
    robot_servos[j].write(target_angle);
    SERVO_CURRENT_POSITIONS[j] = target_angle;
  }
  node_handle.spinOnce();
}


void servoControlSubscriberCallbackJointState(const sensor_msgs::JointState& msg) {
  TARGET_JOINT_POSITIONS[0] = msg.position[0];
  TARGET_JOINT_POSITIONS[1] = msg.position[1];
  TARGET_JOINT_POSITIONS[2] = msg.position[2];
  TARGET_JOINT_POSITIONS[3] = msg.position[3];
  TARGET_JOINT_POSITIONS[4] = msg.position[4];
  TARGET_JOINT_POSITIONS[4] = msg.position[5];
  writeServos();

}


ros::Subscriber<sensor_msgs::JointState> servo_control_subscriber_joint_state("joint_states", &servoControlSubscriberCallbackJointState);


void setup() {
  for (unsigned int i = 0; i < 6; i++) {
    robot_servos[i].attach(servo_pins[i]);
    robot_servos[i].write(mid_positions[i]);
    SERVO_CURRENT_POSITIONS[i] = mid_positions[i];
  }

  node_handle.getHardware()->setBaud(115200);
  node_handle.initNode();
  node_handle.subscribe(servo_control_subscriber_joint_state);
}

void loop() {
   node_handle.spinOnce();
   delay(1);
}
