
#include "timer.h"
volatile sig_atomic_t isTimeout = true;

void timerStart(int sec){
	isTimeout = false;
	signal(SIGALRM, timeoutHandler);
	alarm(sec);

}

void timeoutHandler(int timeoutSignal){
	isTimeout = true;
}