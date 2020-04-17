#ifndef COLORSENSOR_HH
#define COLORSENSOR_HH

#include "interfaces.hh"

class ColorSensor : public skel::ColorSensor {
    void colorSensor_activate();
    void colorSensor_deactivate();

    int pin = 0;
    bool activated = false;

public:
    ColorSensor(const dzn::locator& loc);
    void setPin(int bcmValue);
    void checkSensor();
};

#endif