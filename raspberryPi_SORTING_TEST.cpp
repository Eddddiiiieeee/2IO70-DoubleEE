#include <iostream>
#include <wiringPi.h>

int main() {
    wiringPiSetupGpio();

    pinMode(17, OUTPUT); //MotorForward
    pinMode(27, OUTPUT); //MotorBack

    pinMode(26, OUTPUT); //SortingPiston
    pinMode(23, INPUT); //DiskDetect
    pinMode(24, INPUT); //ColorDetect

    digitalWrite(17, 1);

    while(true) {
        if (digitalRead(23)){
            for (int i = 0; i < 15; i++) {
                digitalWrite(26,0);
                if (digitalRead(24)) {
                    digitalWrite(26,1);
                    break;
                }
                delay(200);
            }
        }
        delay(100);
    }
}