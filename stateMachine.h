#pragma once
#include "eventHandler.h"
#include <stdbool.h>
#include "elev.h"
#include "lightSystem.h"
#include "eventHandler.h"


enum ElevatorDirection {
    DOWN = -1,
    INACTIVE = 0,
    UP = 1
};

enum ElevatorMoveCase{
	DO_NOTHING = 0,
	SAME_FLOOR_ORDER = 1,
	SAME_FLOOR_ORDER_REVERSE = 2,
	ORDER_ELEV_DIR = 3,
	ORDER_ELEV_DIR_REVERSE = 4,
	ORDER_OTHER_DIR = 5,
	ORDER_OTHER_DIR_REVERSE = 6
};

struct state {
    unsigned int floor;
    enum tag_elev_motor_direction elevatorDirection;
    enum tag_elev_motor_direction previousDirection;
    struct light lights;
    bool alarm;
    bool reversing;
    bool stopButtonPressed;
    bool stopDoorLight;
};

void stopButtonEvent(bool stopButtonPressed, int floor);
void orderButtonEvent(bool buttonSignal[3][4], bool alarm);
void floorSensorEvent(int floor);
void alarmEvent(bool alarm);
static void moveElevator();
static void updateLights();
static enum ElevatorMoveCase getElevatorMoveCase();
static int farthestFloorReversing();