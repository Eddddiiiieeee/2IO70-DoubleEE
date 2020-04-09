/**
 * Manually created file for the native Timer component
 * 
 * @author Edward Liem 
 * with help from Sten Wessel's Code
 */

#ifndef TIMER_HH
#define TIMER_HH

#include "ITimer.hh"

class Timer : public skel::Timer {
    void iTimer_start(unsinged int milliseconds);
    void iTimer_cancel();

    unsigned targetTime;
    
public:
    Timer(const dzn::locator& loc);
    void checkTimer();
}

#endif 