#ifndef DISKDETECTOR_HH
#define DISKDETECTOR_HH

#include "interfaces.hh"
//#include "sortingSystem.hh"

class DiskDetector : public skel::DiskDetector {
    void sensor_activate();
    void sensor_deactivate();

    int pin = 0;
    bool activated = false;

public:
    DiskDetector(const dzn::locator& loc);
    void setPin(int bcmValue);
    void checkSensor();
};

#endif