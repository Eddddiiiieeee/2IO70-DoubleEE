// Main function of the sorting robot
// BCM PIN SETUP (USE BCM VALUES, NOT WIRINGPI)

#include <iostream>
#include <string>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <wiringPi.h>
#include <softPwm.h>
#include <mosquitto.h>

#include <dzn/runtime.hh>
#include <dzn/locator.hh>
#include "mainRobot.hh"
#include "Timer.hh"

// PINS ARE SET TO BCM
/********** OUTPUT PINS **********/
const int PIN_PISTON_TAKE = 9;
const int PIN_PISTON_SORT = 26;
const int PIN_MOTOR_SORT = 17;
const int PIN_MOTOR1_RETURN = 18;
const int PIN_MOTOR2_RETURN = 18;
/*********************************/

// PINS ARE SET TO BCM
/********** INPUT PINS **********/
const int PIN_SENSOR_COLOR = 25;
const int PIN_SENSOR_TAKE_DETECT = 2;
const int PIN_SENSOR_SORT_DETECT = 23;
const int PIN_SENSOR_RETURN_DETECT = 24;

const int PIN_ERROR_TAKE_PISTON = 12;
const int PIN_ERROR_SORT_BELT = 9;
const int PIN_ERROR_SORT_PISTON = 11;
const int PIN_ERROR_RETURN_BELT1 = 16;
const int PIN_ERROR_RETURN_BELT2 = 22;
/********************************/

/********** MQTT **********/
mosquitto* mosq;
const char* HOST = "86.91.204.180";
const int PORT = 1883;
const int KEEPALIVE = 60;

int ROBOT_NUMBER;
std::string INPUT_CHANNEL = "factory/robot/in";
std::string OUTPUT_CHANNEL = "factory/robot/out";

const char* MSG_START = "start";
const char* MSG_STOP = "stop";
const char* MSG_EMERGENCY_STOP = "emergencyStop";

const char* MSG_OUT_EMERGENCY = "emergency";
std::string MSG_OUT_HEARTBEAT = "heartbeat";
std::string MSG_OUT_TOOK_DISK = "tookDisk:";
std::string MSG_OUT_REQUEST_DISKS_TAKEN = "requestDisksTaken:";
/**************************/


/********** MISC GLOBAL VARIABLES **********/
int DISK_COUNTER[4];
Robot* sys;
const int MAX_DIFFERENCE = 3; // Max difference of disks
bool SHUTDOWN = false;
bool communicate = true;
/*******************************************/


/********** FUNCTION DECLARATIONS **********/
void setup_pins();
bool setup_mqtt();
//void shutdown();
void destroy_mqtt();
bool preCheck();
void respondDisksTaken(int);
bool canTakeDisk();
void recovery();
void recalculateData(char*);
/*******************************************/


void shutdown(int signum) {
    std::cout << "Shutting Down..." << std::endl;

    if (communicate) {
        destroy_mqtt();
    }

    sys->robot.in.stop();

    exit(signum);
}


int main() {
    dzn::locator loc;
    dzn::runtime rt;
    loc.set(rt);

    Robot system(loc);
    sys = &system;
    
    setup_pins();

    char input;
    std::cout << "Run Without MQTT? (y/n): ";
    std::cin >> input;
    if (input == 'y') {
        if(setup_mqtt()) return 1;
        std::cout << "MQTT Enabled...\n";
    } else if (input == 'n') {
        communicate = false;
        std::cout << "MQTT Disabled...\n";
    } else {
        std::cout << "Invalid input... Defaulting to running with MQTT...\n";
    }
    
    DISK_COUNTER[0] = 0;
    DISK_COUNTER[1] = 0;
    DISK_COUNTER[2] = 0;
    DISK_COUNTER[3] = 0;

    // Set pins on corresponding sensors
    system.taker.blocker.button.setPin(PIN_ERROR_TAKE_PISTON);
    system.taker.detector.setPin(PIN_SENSOR_TAKE_DETECT);

    system.sorter.blocker.button.setPin(PIN_ERROR_SORT_PISTON);
    system.sorter.belt.button.setPin(PIN_ERROR_SORT_BELT);
    system.sorter.diskDetector.setPin(PIN_SENSOR_SORT_DETECT);
    system.sorter.colorSensor.setPin(PIN_SENSOR_COLOR);

    system.returner.firstBelt.button.setPin(PIN_ERROR_RETURN_BELT1);
    system.returner.elevatedBelt.button.setPin(PIN_ERROR_RETURN_BELT2);
    system.returner.sensor.setPin(PIN_SENSOR_RETURN_DETECT);


    // Bind Motors and Pistons
    system.taker.blocker.piston.piston.in.extend = []{ digitalWrite(PIN_PISTON_TAKE, 1); };
    system.taker.blocker.piston.piston.in.retract = []{ digitalWrite(PIN_PISTON_TAKE, 0); };

    system.sorter.blocker.piston.piston.in.extend = []{ digitalWrite(PIN_PISTON_SORT, 1); };
    system.sorter.blocker.piston.piston.in.retract = []{ digitalWrite(PIN_PISTON_SORT, 0); };
    system.sorter.belt.motor.motor.in.start = []{ digitalWrite(PIN_MOTOR_SORT, 1); };
    system.sorter.belt.motor.motor.in.stop = []{ digitalWrite(PIN_MOTOR_SORT, 0); };

    system.returner.firstBelt.motor.motor.in.start = []{ digitalWrite(PIN_MOTOR1_RETURN, 1); };
    system.returner.firstBelt.motor.motor.in.stop = []{ digitalWrite(PIN_MOTOR1_RETURN, 0); };
    system.returner.elevatedBelt.motor.motor.in.start = []{ digitalWrite(PIN_MOTOR2_RETURN, 1); };
    system.returner.elevatedBelt.motor.motor.in.start = []{ digitalWrite(PIN_MOTOR2_RETURN, 0); };

    // Bind RaspberryPi ports
    system.robot.out.emergency = []{
        if (communicate) {
            std::cout << "Communication :: Emergency! Reporting to broker...\n";
            mosquitto_publish(mosq, nullptr, OUTPUT_CHANNEL.c_str(), strlen(MSG_OUT_EMERGENCY), MSG_OUT_EMERGENCY, 0, false);
        } else {
            std::cout << "Communication (TEST MODE) :: Emergency! Reporting to broker... (NO ACTUAL COMMUNICATION!)\n";
        }
    };
    system.controller.controller.out.requestDiskCounters = []{
        // TODO: Consider if we want to use factory data
    };
    system.controller.controller.out.heartbeat = []{
        if (communicate) {
            std::cout << "Communication :: Sending our Heartbeat...\n";
            mosquitto_publish(mosq, nullptr, OUTPUT_CHANNEL.c_str(), MSG_OUT_HEARTBEAT.length(), MSG_OUT_HEARTBEAT.c_str(), 0, false); 
        } else {
            std::cout << "Communication (TEST MODE) :: Sending our Heartbeat... (NO ACTUAL COMMUNICATION!)\n";
        }
    };
    system.controller.controller.out.tookDisk_out = []{
        DISK_COUNTER[ROBOT_NUMBER] += 1;
        if (communicate) {
            std::cout << "Communication :: Disk taken; signaling to robots...\n";
            mosquitto_publish(mosq, nullptr, OUTPUT_CHANNEL.c_str(), MSG_OUT_TOOK_DISK.length(), MSG_OUT_TOOK_DISK.c_str(), 0, false); 
        } else {
            std::cout << "Communication (TEST MODE) :: Disk taken; signaling to robots... (NO ACTUAL COMMUNICATION!)\n";
        }
    };
    system.controller.controller.out.error_out = []{
        recovery();
    };
    system.controller.controller.out.requestDisksTaken = []{
        if (communicate) {
            std::cout << "Communication :: Requesting disk data from other robots.\n";
            mosquitto_publish(mosq, nullptr, OUTPUT_CHANNEL.c_str(), MSG_OUT_REQUEST_DISKS_TAKEN.length(), MSG_OUT_REQUEST_DISKS_TAKEN.c_str(), 0, false);
        } else {
            std::cout << "Communication (TEST MODE) :: Requesting disk data from other robots. (NO ACTUAL COMMUNICATION!)\n";
        }
    };

    // Binding error codes
    system.controller.controller.out.T010 = []{
        std::cout << "Main Robot CRITICAL ERROR :: T010 - Taking System: piston blocking Factory Belt. Timer timed out. Alerting fellow bots...\n";
    };
    system.controller.controller.out.T000 = []{
        std::cout << "Main Robot Error :: T000 - Taking System: piston not actuating. Timer timed out... \n";
    };
    system.controller.controller.out.S000 = []{
        std::cout << "Main Robot Error :: S000 - Sorting System: piston not actuating. Timer timed out...\n";
    };
    system.controller.controller.out.S010 = []{
        std::cout << "Main Robot Error :: S010 - Sorting System: piston stuck extended too long. Timer timed out...\n";
    };
    system.controller.controller.out.S100 = []{
        std::cout << "Main Robot Error :: S100 - Sorting System: belt not actuating button before timer timeout...\n";
    };
    system.controller.controller.out.R100 = []{
        std::cout << "Main Robot Error :: R100 - Returning System: belt no.1 failed to actuate button before timer timeout...\n";
    };
    system.controller.controller.out.R101 = []{
        std::cout << "Main Robot Error :: R101 - Returning System: belt no.2 (elevator) failed to actuate button before timer timeout...\n";
    };
    system.controller.controller.out.R290 = []{
        std::cout << "Main Robot Error :: R290 - Returning System: disk to be returned has not been returned before the timer timeout. Disk potentially lost...\n";
    };
    system.controller.controller.out.R200 = []{
        std::cout << "Main Robot Warning :: R200 - Returning System: disk returned that was not accounted from the Sorting System...\n";
    };

    // Heartbeat Warning Messages
    system.controller.heartbeat1.out.timeout = []{
        std::cout << "Robot Communication Warning :: Heartbeat failure from robot 1. Failure to recieve heartbeat from robot 1 within 5 seconds...\n";
    };
    system.controller.heartbeat2.out.timeout = []{
        std::cout << "Robot Communication Warning :: Heartbeat failure from robot 2. Failure to recieve heartbeat from robot 1 within 5 seconds...\n";
    };
    system.controller.heartbeat3.out.timeout = []{
        std::cout << "Robot Communication Warning :: Heartbeat failure from robot 3. Failure to recieve heartbeat from robot 1 within 5 seconds...\n";
    };
    system.controller.heartbeat4.out.timeout = []{
        std::cout << "Robot Communication Warning :: Heartbeat failure from robot 4. Failure to recieve heartbeat from robot 1 within 5 seconds...\n";
    };

    system.controller.controller.in.reboot = []{
        DISK_COUNTER[0] = 0;
        DISK_COUNTER[1] = 0;
        DISK_COUNTER[2] = 0;
        DISK_COUNTER[3] = 0;
    };

    std::cout << "Checking Dezyne Bindings...\n";
    system.check_bindings();

    // Shutdown signal CTRL + C
    signal(SIGINT, shutdown);

    std::cout << "Commencing precheck sequence...\n";
    if (!preCheck()) {
        std::cout << "Pre Check Error :: One of the sensors give incorrect readings before startup: \n";
        std::cout << "\t Color Sensor: " << digitalRead(PIN_SENSOR_COLOR) << ". Should be: 0\n";
        std::cout << "\t Taker Disk Detector: " << digitalRead(PIN_SENSOR_TAKE_DETECT) << ". Should be: 1\n";
        std::cout << "\t Sorter Disk Detector: " << digitalRead(PIN_SENSOR_SORT_DETECT) << ". Should be: 1\n";
        std::cout << "\t Return Disk Detector: " << digitalRead(PIN_SENSOR_RETURN_DETECT) << ". Should be: 1\n";
        std::cout << "\t Taker Piston Detector: " << digitalRead(PIN_ERROR_TAKE_PISTON) << ". Should be: 0\n";
        std::cout << "\t Sorting Piston Detector: " << digitalRead(PIN_ERROR_SORT_PISTON) << ". Should be: 0\n";

        return 1;
    }
    
    if (!communicate) { // Start if no communication (start tests)
        system.robot.in.start();
    }

    // Event Loop
    while (true) {
        // Check Heartbeat Timers
        system.heartbeatClock.checkTimer();
        system.heartbeat1.checkTimer();
        system.heartbeat2.checkTimer();
        system.heartbeat3.checkTimer();
        system.heartbeat4.checkTimer();

        // Check Taker Timers
        system.taker.blocker.pistonTimer.checkTimer();
        system.taker.blocker.timeoutTimer.checkTimer();
        //Check Taker Sensors
        system.taker.blocker.button.checkSensor();
        system.taker.detector.checkSensor();


        // Check Sorter Timers
        system.sorter.blocker.pistonTimer.checkTimer();
        system.sorter.blocker.timeoutTimer.checkTimer();
        system.sorter.belt.timer.checkTimer();
        system.sorter.colorTimer.checkTimer();
        system.sorter.detectorTimer.checkTimer();
        // Check Sorter Sensors
        system.sorter.blocker.button.checkSensor();
        system.sorter.belt.button.checkSensor();
        system.sorter.diskDetector.checkSensor();
        system.sorter.colorSensor.checkSensor();


        // Check Returner Timers
        system.returner.firstBelt.timer.checkTimer();
        system.returner.elevatedBelt.timer.checkTimer();
        system.returner.timer.checkTimer();
        // Check Returner Sensors
        system.returner.firstBelt.button.checkSensor();
        system.returner.elevatedBelt.button.checkSensor();
        system.returner.sensor.checkSensor();

        if (canTakeDisk()) {
            system.controller.controller.in.pauseTaking();
        } else {
            system.controller.controller.in.resumeTaking();
        }

        delay(100);
    }
}



/********** START FUNCTION DEFINITIONS **********/
void setup_pins() {
    wiringPiSetupGpio(); // BROADCOM PIN SETUP

    pinMode(PIN_PISTON_TAKE, OUTPUT);
    pinMode(PIN_PISTON_SORT, OUTPUT);
    pinMode(PIN_MOTOR_SORT, OUTPUT);
    pinMode(PIN_MOTOR1_RETURN, OUTPUT);
    pinMode(PIN_MOTOR2_RETURN, OUTPUT);

    pinMode(PIN_SENSOR_COLOR, INPUT);
    pinMode(PIN_SENSOR_TAKE_DETECT, INPUT);
    pinMode(PIN_SENSOR_SORT_DETECT, INPUT);
    pinMode(PIN_SENSOR_RETURN_DETECT, INPUT);

    pinMode(PIN_ERROR_TAKE_PISTON, INPUT);
    pinMode(PIN_ERROR_SORT_BELT, INPUT);
    pinMode(PIN_ERROR_SORT_PISTON, INPUT);
    pinMode(PIN_ERROR_RETURN_BELT1, INPUT);
    pinMode(PIN_ERROR_RETURN_BELT2, INPUT);
}

bool setup_mqtt() {
    mosquitto_lib_init();
    mosq = mosquitto_new(nullptr, true, nullptr);

    std::cout << "Robot Number: ";
    std::cin >> ROBOT_NUMBER;

    INPUT_CHANNEL.insert(13,std::to_string(ROBOT_NUMBER));
    OUTPUT_CHANNEL.insert(13,std::to_string(ROBOT_NUMBER));

    MSG_OUT_HEARTBEAT.append(std::to_string(ROBOT_NUMBER));
    MSG_OUT_TOOK_DISK.append(std::to_string(ROBOT_NUMBER));
    MSG_OUT_REQUEST_DISKS_TAKEN.append(std::to_string(ROBOT_NUMBER));

    if (!mosq) {
        std::cout << "MQTT ERROR: OUT OF MEMORY" << std::endl;
        return false;
    }

    // Callback message after recieving message from broker
    mosquitto_message_callback_set(mosq, [](mosquitto* _mosq, void* obj, const mosquitto_message* msg) {
        if (!msg->payloadlen) {
            std::cout << "MQTT Error :: malformed message length received.\n";
            return;
        }

        char* payload = (char*) msg -> payload;

        if (strstr(msg->topic, INPUT_CHANNEL.c_str()) != NULL) {
            if (strcmp(payload, MSG_START) == 0) {
                sys->robot.in.start();
            } else if (strcmp(payload, MSG_STOP) == 0) {
                sys->robot.in.stop();
            } else if (strcmp(payload, MSG_EMERGENCY_STOP) == 0) {
                sys->robot.in.emergencyStop();
            } else if (strstr(payload, "heartbeat") != NULL) { // Heartbeat msg handling
                if (strcmp(payload, "heartbeat1") == 0) {
                    sys->robot.in.heartbeat1();
                } else if (strcmp(payload, "heartbeat2") == 0) {
                    sys->robot.in.heartbeat2();
                } else if (strcmp(payload, "heartbeat3") == 0) {
                    sys->robot.in.heartbeat3();
                } else if (strcmp(payload, "heartbeat4") == 0) {
                    sys->robot.in.heartbeat4();
                } else {
                    std::cout << "MQTT Warning :: Robot number for heartbeat out of bounds: " << payload << std::endl;
                }
            } else if (strcmp(payload, "respondDisksCounters") == 0) {
                // function to respond disk data from broker 
            } else if (strstr(payload, "tookDisk") != NULL) {
                // function to add to disk data
                if (strpbrk(payload, "1") != NULL && ROBOT_NUMBER != 1) {
                    DISK_COUNTER[0] += 1;
                } else if (strpbrk(payload, "2") != NULL && ROBOT_NUMBER != 2) {
                    DISK_COUNTER[1] += 1;
                } else if (strpbrk(payload, "3") != NULL && ROBOT_NUMBER != 3) {
                    DISK_COUNTER[2] += 1;
                } else if (strpbrk(payload, "4") != NULL && ROBOT_NUMBER != 4) {
                    DISK_COUNTER[3] += 1;
                } else {
                    std::cout << "MQTT Warning :: Robot number for tookDisk out of bounds: " << payload << std::endl;
                }
            } else if (strstr(payload, "requestDisksTaken") != NULL) {
                // respond to another robot request for our data (NEED TO ADD ROBOT NUMBER)
                if (strpbrk(payload, "1") != NULL && ROBOT_NUMBER != 1) {
                    respondDisksTaken(1);
                } else if (strpbrk(payload, "2") != NULL && ROBOT_NUMBER != 2) {
                    respondDisksTaken(2);
                } else if (strpbrk(payload, "3") != NULL && ROBOT_NUMBER != 3) {
                    respondDisksTaken(3);
                } else if (strpbrk(payload, "4") != NULL && ROBOT_NUMBER != 4) {
                    respondDisksTaken(4);
                } else {
                    std::cout << "MQTT Warning :: Robot number for requestDisksTaken out of bounds: " << payload << std::endl;
                }
            } else if (strstr(payload, "respondDisksTaken") != NULL) {
                if (strpbrk(payload, (std::to_string(ROBOT_NUMBER)).c_str())) {
                    recalculateData(payload);
                }
            } else if (strstr(payload, "error") != NULL) {
                std::cout << "Warning! A robot has encountered an error: " << payload << std::endl;
            } else {
                std::cout << "MQTT Warning :: unrecognized message from broker: " << payload << std::endl;
            }
        }
    });

    if (mosquitto_connect_async(mosq, HOST, PORT, KEEPALIVE) != MOSQ_ERR_SUCCESS) {
        std::cout << "MQTT ERROR: Could not connect..." << std::endl;
        return false;
    }

    mosquitto_subscribe(mosq, nullptr, INPUT_CHANNEL.c_str(), 0);
    mosquitto_loop_start(mosq);
    
    return true;
}



void destroy_mqtt() {
    mosquitto_disconnect(mosq);
    mosquitto_loop_stop(mosq, true);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

bool preCheck() {
    return (
        !digitalRead(PIN_SENSOR_COLOR)          &&
        digitalRead(PIN_SENSOR_TAKE_DETECT)     &&
        digitalRead(PIN_SENSOR_SORT_DETECT)     &&
        digitalRead(PIN_SENSOR_RETURN_DETECT)   &&
        !digitalRead(PIN_ERROR_TAKE_PISTON)     &&
        !digitalRead(PIN_ERROR_SORT_PISTON)
        );
}

void respondDisksTaken(int robotNum) {
    std::string payloadMessage = "respondDisksTaken:";
    payloadMessage.append(std::to_string(robotNum));
    payloadMessage.append(",");
    payloadMessage.append(std::to_string(DISK_COUNTER[0]));
    payloadMessage.append(",");
    payloadMessage.append(std::to_string(DISK_COUNTER[1]));
    payloadMessage.append(",");
    payloadMessage.append(std::to_string(DISK_COUNTER[2]));
    payloadMessage.append(",");
    payloadMessage.append(std::to_string(DISK_COUNTER[3]));
    mosquitto_publish(mosq, nullptr, OUTPUT_CHANNEL.c_str(), payloadMessage.length(), payloadMessage.c_str(), 0, false);
}

bool canTakeDisk() {
    int min = DISK_COUNTER[0];
    for (int i = 1; i < 4; i++) {
        if (DISK_COUNTER[i] < min) {
            min = DISK_COUNTER[i];
        }
    }
    
    if ((DISK_COUNTER[ROBOT_NUMBER] - min) > MAX_DIFFERENCE) {
        return false;
    } else {
        return true;
    }
}

void recovery() {
    char input;
    std::cout << "Error Encountered. Reboot? (y/n): ";
    std::cin >> input;
    if (input == 'y') {
        std::cout << "Rebooting...\n";
        sys->robot.in.reboot();
        sys->controller.controller.out.requestDisksTaken();
    } else {
        std::cout << "ok.";
        destroy_mqtt();
        sys->robot.in.stop();
    }
}

void recalculateData(char* response) {
    char msg[strlen(response)];
    strcpy(msg, response);
    char* values = strtok(msg, "respondDisksCounter:, ");
    int tempValue = -1;
    for (int i = -1; (i < 4 && values != NULL); i++) {
        if (i == -1) { 
            continue; 
        }

        tempValue = atoi(values);

        if (tempValue > DISK_COUNTER[i]) {
            DISK_COUNTER[i] = tempValue;
        }

        values = strtok(NULL, "respondDisksCounter:, ");
    }
}
/************************************************/