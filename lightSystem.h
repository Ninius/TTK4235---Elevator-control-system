#include <stdbool.h>
#pragma once
struct light{
    bool orderLights[3][4];
    int floorIndicator;
    bool door;
    bool stop;
};

void setLights(struct light state);