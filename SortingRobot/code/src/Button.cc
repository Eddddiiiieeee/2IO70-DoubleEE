#include "interfaces.hh"
#include "Button.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include <wiringPi.h>

Button::Button(const dzn::locator& loc) : skel::Button(loc) {
    
}

void Button::sensor_activate() {
    activated = true;
}

void Button::sensor_deactivate() {
    activated = false;
}

void Button::setPin(int bcmValue) {
    pin = bcmValue;
}

void Button::checkSensor() {
    if (!activated) {
        return;
    }
    if (digitalRead(pin)) {
        sensor.out.triggered();
    }
}