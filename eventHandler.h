#include "elev.h"
#include <stdbool.h>
#pragma once
struct input {
    bool stopSignal;
    int floorSensorSignal;
    // First index = tag_elev_lamp_type from elev.h
    bool buttonSignal[3][4];
    bool inputChanged;
    bool alarm;
};
static bool buttonSignalChanged();
void eventHandler();
static void resetElevator();
static void updateButtonSignal();
void setAlarm();