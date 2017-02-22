#include "stateMachine.h"
#include "orderSystem.h"
#include "lightSystem.h"
#include "elev.h"
#include <stdio.h>
#include <stdbool.h>

static struct state currentState;
static struct order orderQueue;

//Returns the farthest floor with valid order if elevator is reversing, else -1
static int farthestFloorReversing(){
    if (currentState.elevatorDirection == 1){
        for (int i = 3; i >= 0; i--){
            if(getFloorOrder(orderQueue,  currentState.elevatorDirection, i, true)){
                return i;
            }
        }
    }
    else if (currentState.elevatorDirection == -1){
        for (int i = 0; i < 4; i++){
            if(getFloorOrder(orderQueue,  currentState.elevatorDirection, i, true)){
                return i;
            }            
        }
    }
    else {
        return -1;
    }
}

void stopButtonEvent(bool stopButtonPressed, int floor) {
	currentState.stopButtonPressed = true;
    orderQueue = (struct order){0};
    elev_set_motor_direction(0);
    if (floor != -1){
    	currentState.stopDoorLight = true;
    }
    if (currentState.elevatorDirection != 0){
    	currentState.previousDirection = currentState.elevatorDirection;
    }
    currentState.elevatorDirection = 0; 
    updateLights();
}

void orderButtonEvent(bool buttonSignal[3][4], bool alarm){
    for (int type = 0; type < 3; type++){
        for (int floor = 0; floor < 4; floor ++){
            if (buttonSignal[type][floor] == true){
                setOrder(&orderQueue, type, floor);
            }
        }
    }
    updateLights();
    if (alarm == false && currentState.elevatorDirection == 0){
        moveElevator();
    }
}

void floorSensorEvent(int floor){
    currentState.floor = floor;
    if (getFloorOrder(orderQueue,currentState.elevatorDirection, floor, currentState.reversing) == true){
        if ((currentState.reversing && farthestFloorReversing() == floor) || 
                getFloorOrder(orderQueue,currentState.elevatorDirection, floor, false)){
            elev_set_motor_direction(0);
            clearOrder(&orderQueue, floor);
            setAlarm();
            currentState.previousDirection = currentState.elevatorDirection; 
            currentState.elevatorDirection = 0;
        }

    }
    updateLights();
}

void alarmEvent(bool alarm){
    if (alarm == true){
        currentState.alarm = true;
        updateLights();
    }
    else{
        currentState.alarm = false;
        updateLights();
        moveElevator();
    }
}


static void moveElevator(){
    if ((currentState.elevatorDirection == 0 && currentState.previousDirection == 0) || currentState.floor == 0){
        currentState.elevatorDirection = 1;
        printf("First true \n");
    }
    else if (currentState.floor == 3){
        currentState.elevatorDirection = -1;
        printf("Secon true \n");
    }
    else {
    	currentState.elevatorDirection = currentState.previousDirection;
    }
    printf("Direction: %d\n", currentState.elevatorDirection);
    switch (getElevatorMoveCase()){
    	case 0 :
    		printf("%d", 0);
    		currentState.elevatorDirection = 0;
    		break;
    	case 1 :
    		currentState.reversing = false;
    		floorSensorEvent(currentState.floor);
    		break;
    	case 2 :
    		currentState.reversing = true;
    		floorSensorEvent(currentState.floor);
    	case 3 :
    	    currentState.reversing = false;
    		elev_set_motor_direction(currentState.elevatorDirection);
    		break;
    	case 4 :
    	    currentState.reversing = true;
    	    currentState.elevatorDirection *=-1;
    		elev_set_motor_direction(currentState.elevatorDirection);
    		break;
    	case 5 :
    		currentState.reversing = false;
    		currentState.elevatorDirection *= -1;
    		elev_set_motor_direction(currentState.elevatorDirection);
    		break;
    	case 6 :
    		currentState.reversing = true;
    		elev_set_motor_direction(currentState.elevatorDirection);
    		break;
    }
    currentState.previousDirection = currentState.elevatorDirection;

    printf("\n");
    currentState.stopButtonPressed = false; 
    currentState.stopDoorLight = false;
    updateLights();
}

static void updateLights(){
    for (int type = 0; type < 3; type++){
        for (int floor = 0; floor < 4; floor++){
            if (type == 0){
                currentState.lights.orderLights[type][floor] = orderQueue.callUp[floor];
            }
            else if (type == 1){
                currentState.lights.orderLights[type][floor] = orderQueue.callDown[floor];
            }
            else{
                currentState.lights.orderLights[type][floor] = orderQueue.order[floor];
            }
            
        }
    }
    currentState.lights.door = (currentState.alarm || currentState.stopDoorLight);
    currentState.lights.stop = currentState.stopButtonPressed;
    currentState.lights.floorIndicator = currentState.floor;
    setLights(currentState.lights);
}

static enum ElevatorMoveCase getElevatorMoveCase(){
    printf("Movecase: ");
	enum ElevatorMoveCase moveCase = 0;
	if (getFloorOrder(orderQueue, currentState.elevatorDirection, currentState.floor, false)){
		// Stopped between floors
		if (currentState.stopButtonPressed && currentState.previousDirection != 0){
			moveCase = 4;
            printf("%d", 4);
		}
		else {
			moveCase = 1;
            printf("%d", 1);
		}
	}
	else if (orderExists(orderQueue, currentState.elevatorDirection, currentState.floor)){
		moveCase = 3;
        printf("%d", 3);
	}
	else if (orderExistsReverse(orderQueue, currentState.elevatorDirection, currentState.floor)){
		moveCase = 4;
        printf("%d", 4);
	}
	else if (getFloorOrder(orderQueue, currentState.elevatorDirection*-1, currentState.floor, false)){
		if (currentState.stopButtonPressed && currentState.previousDirection != 0){
			moveCase = 5;
                        printf("%d", 5);
		}
		else {
			moveCase = 2;
                        printf("%d", 2);
		}
	}
	else if (orderExists(orderQueue, currentState.elevatorDirection*-1, currentState.floor)){
		moveCase = 5;
                    printf("%d", 5);
	}
	else if (orderExistsReverse(orderQueue, currentState.elevatorDirection*-1, currentState.floor)){
		moveCase = 6;
                    printf("%d", 6);
	}

	else {
		moveCase = 0;
                    printf("%d", 0);
	}
    printf("\n");
	return moveCase;
}