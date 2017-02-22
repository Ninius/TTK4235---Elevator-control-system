#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>



void timerStart(int sec);
void timeoutHandler(int timeoutSignal);
