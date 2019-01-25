#ifndef Custom_Timer
#define Custom_Timer

#include "TypeDefs.h"
extern CustomTimer ceTimer;
void setStartTime(CustomTimer *timer);
void setEndTime(CustomTimer *timer);
void calculateTimerDifferences(CustomTimer *timer);

#endif