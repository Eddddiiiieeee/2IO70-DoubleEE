// Main function of the sorting robot
// BCM PIN SETUP (USE BCM VALUES, NOT WIRINGPI)

#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>

#include <dzn/runtime.hh>
#include <dzn/locator.hh>

#include "sortingSystem.hh"
#include "beltSystem.hh"
#include "blockingSystem.hh"
#include "interfaces.hh"
#include "ITimer.hh"

// PINS ARE SET TO BCM
/********** OUTPUT PINS **********/
//const int PIN_PISTON_TAKE = -1;
const int PIN_PISTON_SORT = 26;
const int PIN_MOTOR_BELT = 17;

// PINS ARE SET TO BCM
/********** INPUT PINS **********/
const int PIN_SENSOR_COLOR = 25;
const int PIN_SENSOR_DETECT = 23;
const int PIN_SENSOR_SORTING_BELT = 19;

/********** FUNCTION DECLARATIONS **********/
void setup();


int main() {
    dzn::locator loc;
    dzn::runtime rt;
    loc.set(rt);

    SortingSystem system(loc);
    system.check_bindings();

    setup();

    // TEMP SETUP
    system.colorSensor.setPin(PIN_SENSOR_COLOR);
    system.diskDetector.setPin(PIN_SENSOR_DETECT);
    
    system.belt.controller.motor.in.start = []{ digitalWrite(PIN_MOTOR_BELT, 1); };
    system.belt.controller.motor.in.stop = []{ digitalWrite(PIN_MOTOR_BELT, 0)};

    system.blocker.controller.piston.in.extend = []{ digitalWrite(PIN_PISTON_SORT, 1); };
    system.blocker.controller.piston.in.retract = []{ digitalWrite(PIN_PISTON_SORT, 0); };

    // EVENT LOOP
    while (true) {
        // Timer Check
        system.timer.checkTimer();
        system.belt.timer.checkTimer();
        system.blocker.pistonTimer.checkTimer();
        system.blocker.timeoutTimer.checkTimer();

        // Sensor Check
        system.diskDetector.checkSensor();
        system.colorSensor.checkSensor();
        system.belt.button.checkSensor();
        system.blocker.button.checkSensor();
        
        delay(100);
    }
}

void setup() {
    wiringPiSetupGpio(); // BROADCOM PIN SETUP

    //pinMode(PIN_PISTON_TAKE, OUTPUT);
    pinMode(PIN_PISTON_SORT, OUTPUT);
    pinMode(PIN_MOTOR_BELT, OUTPUT);

    pinMode(PIN_SENSOR_COLOR, INPUT);
    pinMode(PIN_SENSOR_DETECT, INPUT);
    pinMode(PIN_SENSOR_SORTING_BELT, INPUT);
}
