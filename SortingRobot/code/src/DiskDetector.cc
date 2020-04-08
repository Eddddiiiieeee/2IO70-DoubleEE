#include "DiskDetector.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include <wiringPi.h>

DiskDetector::DiskDetector(const dzn::locator &loc) : skel::DiskDetector(loc) {
    pin = 0;
    activated = false;
}

void DiskDetector::setPin(int bcmValue) {
    pin = bcmValue;
}

void DiskDetector::sensor_activate() {
    activated = true;
}

void DiskDetector::sensor_deactivate() {
    activated = false;
}

void DiskDetector::checkSensor() {
    if (!activated) {return;}
    if (!digitalRead(pin)) {
        sensor.out.triggered(); 
    }
}
