#include "eventHandler.h"
#include "stateMachine.h"
#include "orderSystem.h"
#include "timer.h"
#include <stdio.h>
#include <unistd.h>
static struct input newInput;
static struct input oldInput;
static bool timerAlarm;
extern sig_atomic_t isTimeout;

//Resets elevator to first floor
static void resetElevator(){
    elev_set_motor_direction(-1);
    while (elev_get_floor_sensor_signal() != 0){
    }
    elev_set_motor_direction(0);
}

//Updates button signal array in new input
static void updateButtonSignal(){
    for (int type = 0; type < 3; type++){
            for (int floor = 0; floor < 4; floor++){
                if ((type == 0 && floor == 3) || (type == 1 && floor == 0)){
                    continue;
                }
                newInput.buttonSignal[type][floor] = elev_get_button_signal(type, floor);
            }
    }
}

void setAlarm(){
        timerStart(3);
}

//Compares button signal arrays in old and new input
static bool buttonSignalChanged(){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 4; j++){
            if (newInput.buttonSignal[i][j] != oldInput.buttonSignal[i][j]){
                return true;
            }
        }
    }
    return false;
}


void eventHandler() {
    elev_init();
    resetElevator();   
    while(1){
        usleep(100*1000);
        newInput.stopSignal = elev_get_stop_signal();
        newInput.floorSensorSignal = elev_get_floor_sensor_signal();
        updateButtonSignal(); 
        newInput.alarm = !isTimeout;
        if (newInput.floorSensorSignal != oldInput.floorSensorSignal && newInput.floorSensorSignal != -1){
            floorSensorEvent(newInput.floorSensorSignal);
        }
        if (buttonSignalChanged(newInput, oldInput)){
            orderButtonEvent(newInput.buttonSignal, newInput.alarm);
        }
        if (newInput.stopSignal){
            stopButtonEvent(newInput.stopSignal, newInput.floorSensorSignal); 
        }
        if (newInput.alarm != oldInput.alarm){
            alarmEvent(newInput.alarm);
        }
        memcpy(&oldInput, &newInput, sizeof newInput);
        memset(&newInput, 0, sizeof newInput);
    }
}

