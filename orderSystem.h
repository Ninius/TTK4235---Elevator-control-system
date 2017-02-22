#include <stdbool.h>
#pragma once
struct order {
    bool callUp[4];
    bool callDown [4];
    bool order [4];
};

int getFloorOrder(struct order queue, int direction, int floor, bool reversing);
void clearOrder(struct order *queue, int floor);
void setOrder(struct order *queue, int type, int floor);
int orderExists(struct order queue, int direction, int floor);
int orderExistsReverse(struct order wueue, int direction, int floor);