#include "lightSystem.h"
#include "elev.h"
#include <stdio.h>
void setLights(struct light state){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 4; j++){
            if ((i == 0 && j == 3) || (i == 1 && j == 0)){
                continue;
            }
            elev_set_button_lamp(i, j, state.orderLights[i][j]);
        }
    }
    elev_set_stop_lamp(state.stop);
    elev_set_door_open_lamp(state.door);
    elev_set_floor_indicator(state.floorIndicator); 
}