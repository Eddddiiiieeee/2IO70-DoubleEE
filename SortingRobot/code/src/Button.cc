#include "interfaces.hh"
#include "Button.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include <wiringPi.h>

Button::Button(const dzn::locator& loc) : skel::Button(loc) {
    pin = -1;
    activated = false;
}

Button::sensor_activate() {
    activated = true;
}

Button::sensor_deactivate() {
    activated = false;
}

Button::setPin(int bcmValue) {
    pin = bcmValue;
}

Button::checkSensor() {
    if (!activated) {
        return;
    }
    if (digitalRead(pin)) {
        sensor.out.triggered();
    }
}