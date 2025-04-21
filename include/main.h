// Main header file for the project
//

#ifndef MAIN_H
#define MAIN_H

#include <time.h>
#include <driver/adc.h>
#include "dates.h"
#include "effects.h"
#include "led_control.h"
#include "words.h"
#include "ota_update.h"
#include "config.h"


// Enum for the state machine
enum STATE {
    OFF,
    ON,
    UPDATE_TIME,
    NEWYEAR,
    CHECK_EVENTS,
}; 


// Function prototypes
void setup();
void loop();
void fetchTime();
void show_time(int hours, int minutes, uint8_t r, uint8_t g, uint8_t b);
void toggle_status();

// Global variables
DEV_RgbLED* led = nullptr; // Declare a pointer to store the object
bool wifi_connected = false;
enum STATE s; 

struct tm timeinfo;
time_t now;

//const int presistorPin = 35;
int ctr = 0;

bool is_birthday = false;
bool is_christmas = false;

int minute = -1;
#endif
