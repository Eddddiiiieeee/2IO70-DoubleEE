/**
 * Implementation of the Timer
 * 
 * Description:
 *      The timeout check must be done on the main loop to avoid
 *      using multithreading. May implement later... may not...
 * 
 * @author Edward Liem
 *      w/ help from Sten Wessel's code
 */

#include "Timer.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include <wiringPi.h>

Timer::Timer(const dzn::locator& loc) : skel::Timer(loc) {
    targetTime = 0;
}

void Timer::iTimer_start(unsinged int milliseconds) {
    targetTime = millis() + milliseconds;
}

void Timer::iTimer_cancel() {
    targetTime = 0;
}

void Timer::checkTimer() {
    if (targetTime != 0) {
        return;
    }

    if (millis() >= targetTime) {
        targetTime = 0;
        iTimer.out.timeout();
    }
}