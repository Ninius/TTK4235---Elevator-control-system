#include "orderSystem.h"

int getFloorOrder(struct order queue, int direction, int floor, bool reversing){
    if (reversing == true){
        direction = 2;
    }
    switch (direction){
        case 2 :
            return (queue.callUp[floor] || queue.order[floor] || queue.callDown[floor]);
            break;
        case 1 :
            return (queue.callUp[floor] || queue.order[floor]);
            break;
        case 0 :
            return (queue.callUp[floor] || queue.order[floor] || queue.callDown[floor]);
            break;
        case -1:
            return (queue.callDown[floor] || queue.order[floor]);
            break;
        default:
            return 0;
    }
}
//Assumes everyone enters elevator if stopped on floor
void clearOrder(struct order *queue, int floor){
    queue->order[floor] = 0;
    queue->callUp[floor] = 0;
    queue->callDown[floor] = 0;
}

//Returns true if order exist for reversing elevator
int orderExistsReverse(struct order queue, int direction, int floor){
    int orderExists = 0;
    if (direction == -1){
        for (int i = floor + 1; i < 4; i++){
            if (queue.callDown[i] || queue.order[i]){
               orderExists = 1;
            }
        }
    }
    if (direction == 1){
        for (int i = 0; i < floor; i++){
            if (queue.callUp[i] || queue.order[i]){
                orderExists = 1;
            }
        }
    }
    return orderExists;   
}
//Returns true if order exists in current direction
int orderExists(struct order queue, int direction, int floor){
    int orderExists = 0;
    if (direction == 1){
        for (int i = floor; i < 4; i++){
            if (queue.callUp[i] || queue.order[i]){
               orderExists = 1;
            }
        }
    }
    if (direction == -1){
        for (int i = 0; i < floor; i++){
            if (queue.callDown[i] || queue.order[i]){
                orderExists = 1;
            }
        }
    }
    return orderExists;
}

void setOrder(struct order *queue, int type, int floor){
    if (type == 0){
        queue->callUp[floor] = 1;
    }
    if (type == 1){
        queue->callDown[floor] = 1;
    }
    if (type == 2){
        queue->order[floor] = 1;
    }
}