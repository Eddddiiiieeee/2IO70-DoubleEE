// Raspberry Pi PWM Test
#include <iostream>
#include <string>
#include <wiringPi.h>
#include <softPwm.h>

int range = 1024;

// Function Prototype
static void menu ();

int main () {
    wiringPiSetup();

    menu();

    return 0;
}

static void menu () {
    short int choice = 0;

    while (choice != 9) {
        std::cout << std::string(100, '\n'); // Equivalent to 'clear' command
        std::cout << "PWM RaspberryPi Demo\n";

        std::cout << "1 - Hardware PWM\n";
        std::cout << "2 - Software PWM\n";
        std::cout << "9 - Exit\n\n";

        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << std::string(100, '\n');
                pinMode(1, PWM_OUTPUT);
                
                /*
                std::cout << "Range (Default 1024): ";
                std::cin >> range;
                pwmSetRange(range);
                
                std::cout << "Mode (0 = Balanced [default], 1 = Space): ";
                std::cin >> choice;
                if (choice == 0) {
                    pwmSetMode(PWM_MODE_BAL);
                } else {
                    pwmSetMode(PWM_MODE_MS);
                }

                std::cout << "Clock (32 = default = 585Hz): ";
                std::cin >> choice;
                pwmSetClock(choice);
                */

                choice = 0;
                while (choice != -1) {
                    pwmWrite(1, choice);
                    std::cout << std::string(100, '\n');
                    std::cout << "(-1 to quit)\n";
                    std::cout << choice << std::endl;
                    std::cout << "PWM Duty Cycle (0 - " << range << "): ";
                    std::cin >> choice;
                    if (choice > range) {
                        std::cout << "ERROR: choice > range";
                        std::cout << "(enter any value to continue...)";
                        std::cin >> choice;
                        std::cin.clear();
                        std::cin.ignore();
                        choice = 0;
                    }
                }
                pwmWrite(1, 0);
                choice = 0;
                break;
            case 2: 
                int pin;
                std::cout << std::string(100, '\n');

                std::cout << "Range (100 so value is proportional to %): ";
                std::cin >> range;
                pwmSetRange(range);

                std::cout << "Pin (wiringPi Pin # !!!): ";
                std::cin >> pin;
                if (softPwmCreate(pin, 0, range) != 0) {
                    std::cout << "ERROR: you messed up...\n";
                }

                choice = 0;
                while (choice != -1) {
                    softPwmWrite(pin, choice);
                    std::cout << std::string(100, '\n');
                    std::cout << "(-1 to quit)\n"; 
                    std::cout << choice << std::endl;
                    std::cout << "PWM Duty Cycle (0 - " << range << "): ";
                    std::cin >> choice;
                    if (choice > range) {
                        std::cout << "ERROR: choice > range\n";
                        std::cout << "(enter any value to continue...)";
                        std::cin >> choice;
                        std::cin.clear();
                        std::cin.ignore();
                        choice = 0;
                    }
                }
                choice = 0;
                break;
            default:
                break;
        }
    }
}