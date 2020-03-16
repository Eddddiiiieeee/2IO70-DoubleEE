// Main function of the sorting robot
// BCM PIN SETUP (USE BCM VALUES, NOT WIRINGPI)

#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>

#include <dzn/runtime.hh>
#include <dzn/locator.hh>

// PINS ARE SET TO BCM
/********** OUTPUT PINS **********/
const int PIN_PISTON_TAKE = -1;
const int PIN_PISTON_SORT = -1;
const int PIN_MOTOR_BELT = -1;

// PINS ARE SET TO BCM
/********** INPUT PINS **********/
const int PIN_SENSOR_COLOR = -1;
const int PIN_SENSOR_DETECT = -1;

/********** FUNCTION DECLARATIONS **********/
void setup();

int main() {
    dzn::locator loc;
    dzn::runtime rt;
    loc.set(rt);

    setup();

    while (true) {
        // Interaction
    }
}

void setup() {
    wiringPiSetupGpio(); // BROADCOM PIN SETUP

    pinMode(PIN_PISTON_TAKE, OUTPUT);
    pinMode(PIN_PISTON_SORT, OUTPUT);
    pinMode(PIN_MOTOR_BELT, OUTPUT);

    pinMode(PIN_SENSOR_COLOR, INPUT);
    pinMode(PIN_SENSOR_DETECT, INPUT);
}
