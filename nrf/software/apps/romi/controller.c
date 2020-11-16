#include "controller.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "buckler.h"
#include "display.h"
#include "lsm9ds1.h"
#include "kobukiSensorPoll.h"
#include "kobukiActuator.h"
#include "kobukiUtilities.h"
float angle = 0;
int16_t rightSpeed = 0;
int16_t leftSpeed = 0;
const int16_t delta = 100;
char cmd;
KobukiSensors_t sensors = {0};
robot_state_t controller (robot_state_t state)
{
  // read sensors from robot
  kobukiSensorPoll(&sensors);

  // delay before continuing
  // Note: removing this delay will make responses quicker, but will result
  //  in printf's in this loop breaking JTAG
  switch(state) {
    case OFF: {
      if (is_button_pressed(&sensors)) {
        leftSpeed = 0;
        rightSpeed = 0;
        state = HALT;
      } else {
        display_write("OFF", DISPLAY_LINE_0);
        kobukiDriveDirect(0, 0);
        state = OFF;
      }
      break; 
    }
    case HALT: {
      if (is_button_pressed(&sensors)) {
        state = OFF;
        kobukiDriveDirect(0, 0);
      }
      else if (cmd == 'F') {
        leftSpeed = delta;
        rightSpeed = delta;
        state = FORWARD;
      }
      else if (cmd == 'B') {
        leftSpeed = -delta;
        rightSpeed = -delta;
        state = BACKWARD;
      }
      else if (cmd == 'L') {
        leftSpeed = -delta;
        rightSpeed = delta;
        angle = 0;
        lsm9ds1_start_gyro_integration();
        state = TURN_LEFT;
      }
      else if (cmd == 'R') {
        leftSpeed = delta;
        rightSpeed = -delta;
        angle = 0;
        lsm9ds1_start_gyro_integration();
        state = TURN_RIGHT;
      }
      else {
        display_write("HALT", DISPLAY_LINE_0);
        kobukiDriveDirect(0, 0);
        state = HALT;
      }
      break;
    }
    case FORWARD: {
      if (is_button_pressed(&sensors)) {
        state = OFF;
        kobukiDriveDirect(0, 0);
      }
      else if (cmd == 'H') {
        leftSpeed = 0;
        rightSpeed = 0;
        state = HALT;
      }
      else if (cmd == 'B') {
        leftSpeed = -delta;
        rightSpeed = -delta;
        state = BACKWARD;
      }
      else if (cmd == 'L') {
        leftSpeed = -delta;
        rightSpeed = delta;
        angle = 0;
        lsm9ds1_start_gyro_integration();
        state = TURN_LEFT;
      }
      else if (cmd == 'R') {
        leftSpeed = delta;
        rightSpeed = -delta;
        angle = 0;
        lsm9ds1_start_gyro_integration();
        state = TURN_RIGHT;
      }
      else {
        display_write("FORWARD", DISPLAY_LINE_0);
        kobukiDriveDirect(leftSpeed, rightSpeed);
        state = FORWARD;
      }
      break; 
    }

    case BACKWARD:
    {
      if (is_button_pressed(&sensors)) {
        state = OFF;
        kobukiDriveDirect(0, 0);
      }
      else if (cmd == 'H') {
        leftSpeed = 0;
        rightSpeed = 0;
        state = HALT;
      }
      else if (cmd == 'F') {
        leftSpeed = delta;
        rightSpeed = delta;
        state = FORWARD;
      }
      else if (cmd == 'L') {
        leftSpeed = -delta;
        rightSpeed = delta;
        angle = 0;
        lsm9ds1_start_gyro_integration();
        state = TURN_LEFT;
      }
      else if (cmd == 'R') {
        leftSpeed = delta;
        rightSpeed = -delta;
        angle = 0;
        lsm9ds1_start_gyro_integration();
        state = TURN_RIGHT;
      }
      else {
        display_write("BACKWARD", DISPLAY_LINE_0);
        kobukiDriveDirect(leftSpeed, rightSpeed);
        state = BACKWARD;
      }
      break; 
    }
    case TURN_RIGHT:
    {
      if (is_button_pressed(&sensors)) {
        state = OFF;
        lsm9ds1_stop_gyro_integration();
        angle = 0;
        kobukiDriveDirect(0, 0);
      }
      else if (angle <= -90 || cmd == 'H') {
        state = HALT;
        lsm9ds1_stop_gyro_integration();
        angle = 0;
        leftSpeed = 0;
        rightSpeed = 0;
      }
      else if (cmd == 'F') {
        state = FORWARD;
        lsm9ds1_stop_gyro_integration();
        angle = 0;
        leftSpeed = delta;
        rightSpeed = delta;

      }
      else if (cmd == 'B') {
        state = BACKWARD;
        lsm9ds1_stop_gyro_integration();
        angle = 0;
        leftSpeed = -delta;
        rightSpeed = -delta;
      }
      else 
      {
        display_write("TURN_RIGHT", DISPLAY_LINE_0);
        kobukiDriveDirect(leftSpeed, rightSpeed);
        angle = lsm9ds1_read_gyro_integration().z_axis;
        state = TURN_RIGHT;

        char buf[16];
        snprintf(buf, 16, "%f", angle);
        display_write(buf, DISPLAY_LINE_1);
      }
      break;
    }
    case TURN_LEFT:
    {
      if (is_button_pressed(&sensors)) {
        state = OFF;
        lsm9ds1_stop_gyro_integration();
        angle = 0;
        kobukiDriveDirect(0, 0);
      }
      else if (angle >= 90 || cmd == 'H') {
        state = HALT;
        lsm9ds1_stop_gyro_integration();
        angle = 0;
        leftSpeed = 0;
        rightSpeed = 0;
      }
      else if (cmd == 'F') {
        state = FORWARD;
        lsm9ds1_stop_gyro_integration();
        angle = 0;
        leftSpeed = delta;
        rightSpeed = delta;
      }
      else if (cmd == 'B') {
        state = BACKWARD;
        lsm9ds1_stop_gyro_integration();
        angle = 0;
        leftSpeed = -delta;
        rightSpeed = -delta;
      }
      else 
      {
        display_write("TURN_LEFT", DISPLAY_LINE_0);
        kobukiDriveDirect(leftSpeed, rightSpeed);
        angle = lsm9ds1_read_gyro_integration().z_axis;
        state = TURN_LEFT;

        char buf[16];
        snprintf(buf, 16, "%f", angle);
        display_write(buf, DISPLAY_LINE_1);
      }
      break;
    }

  }
  return state;
}
