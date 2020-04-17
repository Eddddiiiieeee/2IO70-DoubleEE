#include "ColorSensor.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include <wiringPi.h>

ColorSensor::ColorSensor(const dzn::locator& loc) : skel::ColorSensor(loc) {

}

void ColorSensor::colorSensor_activate() {
    activated = true;
}

void ColorSensor::colorSensor_deactivate() {
    activated = false;
}

void ColorSensor::setPin(int bcmValue) {
    pin = bcmValue;
}

void ColorSensor::checkSensor() {
    if (!activated) {
        return;
    }
    if (digitalRead(pin)) {
        colorSensor.out.triggered();
    }
}