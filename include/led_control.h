#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include "DEV_LED.h"   
#include "config.h"  

// Global variables
extern Adafruit_NeoPixel pixels;
extern int R, G, B;
extern int R_OLD, G_OLD, B_OLD;

struct RGBW {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
};

// Function prototypes
struct RGBW rgb_2_rgbw(uint8_t r, uint8_t g, uint8_t b);
int min3(int a, int b, int c);
void led_init();
void led_update();
bool is_led_changed();
bool is_led_on();
uint8_t get_led_red();
uint8_t get_led_green();
uint8_t get_led_blue();
void disable_all_leds();
void toggle_led(const uint8_t* led, uint8_t r, uint8_t g, uint8_t b);
void enable_leds(const uint8_t* ptr, int lenght, uint8_t r, uint8_t g, uint8_t b);
void enable_leds_party(const uint8_t* ptr, int lenght);
void enable_leds_glow(const uint8_t* ptr, int lenght, uint8_t r, uint8_t g, uint8_t b);

#endif