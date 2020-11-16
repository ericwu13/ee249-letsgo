#include "kobukiSensorTypes.h"

// Robot states
// Add your own states here
typedef enum {
  OFF,
  HALT,
  TURN_LEFT,
  TURN_RIGHT,
  BACKWARD,
  FORWARD
} robot_state_t;


robot_state_t controller(robot_state_t state);
