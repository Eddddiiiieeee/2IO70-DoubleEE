#include "ColorSensor.hh"
#include "interfaces.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include <wiringPi.h>

ColorSensor::ColorSensor(const dzn::locator& loc) : skel::Button(loc) {
    pin = -1;
    activated = false;
}

ColorSensor::colorSensor_activate() {
    activated = true;
}

ColorSensor::colorSensor_deactivate() {
    activated = false;
}

ColorSensor::setPin(int bcmValue) {
    pin = bcmValue;
}

ColorSensor::checkSensor() {
    if (!activated) {
        return;
    }
    if (digitalRead(pin)) {
        colorSensor.out.lightDisk();
    }
}