// #include <stdlib.h>
#include <stdio.h>
// #include <string.h>
#include <time.h>
#include "Timer.h"
#include "TypeDefs.h"
#define true 1
#define false 0

CustomTimer ceTimer;

//Public Members ===============================================================
void setStartTime(CustomTimer *timer){
	timer->start=clock();
}

void setEndTime(CustomTimer *timer){
	timer->end=clock();
}

void calculateTimerDifferences(CustomTimer *timer){
	timer->difference=timer->end-timer->start;
	timer->hours=(int)((float)timer->difference/(CLOCKS_PER_SEC*3600));
	timer->minutes=(int)((timer->difference-(timer->hours*3600*CLOCKS_PER_SEC))/(CLOCKS_PER_SEC*60));
	timer->seconds=(int)((timer->difference-(timer->hours*3600*CLOCKS_PER_SEC)-(timer->minutes*60*CLOCKS_PER_SEC))/CLOCKS_PER_SEC);
	timer->millisecs=(int)((timer->difference-(timer->hours*3600*CLOCKS_PER_SEC)-(timer->minutes*60*CLOCKS_PER_SEC)-(timer->seconds*CLOCKS_PER_SEC)));
	// printf("It took %d clicks\n",timer->difference);
	// printf("  . Hours: %d\n",timer->hours);
	// printf("  . Minutes: %d\n",timer->minutes);
	// printf("  . Seconds: %d\n",timer->seconds);
	// printf("  . Millis: %f\n",timer->millisecs);
}