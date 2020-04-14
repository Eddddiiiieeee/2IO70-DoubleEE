#ifndef BUTTON_HH
#define BUTTON_HH

#include "interfaces.hh"

class Button : public skel::Button {
    void sensor_activate();
    void sensor_deactivate();

    int pin;
    bool activated;

public:
    Button(const dzn::locator& loc);
    void setPin(int bcmValue);
    void checkSensor();
};

#endif