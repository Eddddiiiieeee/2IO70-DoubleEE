#include <iostream>
#include <wiringPi.h>

int main(){
    wiringPiSetup();

    pinMode(4, INPUT);
    while (true) {
        std::cout << digitalRead(4) << std::endl;
        delay(500);
    }
    return 0;
}