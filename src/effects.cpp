#include "effects.h"

void start_new_year_countdown(){
    disable_all_leds();
    enable_leds(ZWANZIG_1, sizeof(ZWANZIG_1), get_led_red(), get_led_green(), get_led_blue());
    led_update();
    sleep(10);
    disable_all_leds();
    enable_leds(ZEHN_1, sizeof(ZEHN_1), get_led_red(), get_led_green(), get_led_blue());
    led_update();
    sleep(1);
    disable_all_leds();
    enable_leds(NEUN, sizeof(NEUN), get_led_red(), get_led_green(), get_led_blue());
    led_update();
    sleep(1);
    disable_all_leds();
    enable_leds(ACHT, sizeof(ACHT), get_led_red(), get_led_green(), get_led_blue());
    led_update();
    sleep(1);
    disable_all_leds();
    enable_leds(SIEBEN, sizeof(SIEBEN), get_led_red(), get_led_green(), get_led_blue());
    led_update();
    sleep(1);
    disable_all_leds();
    enable_leds(SECHS, sizeof(SECHS), get_led_red(), get_led_green(), get_led_blue());
    led_update();
    sleep(1);
    disable_all_leds();
    enable_leds(FUENF, sizeof(FUENF), get_led_red(), get_led_green(), get_led_blue());
    led_update();
    sleep(1);
    disable_all_leds();
    enable_leds(VIER, sizeof(VIER), get_led_red(), get_led_green(), get_led_blue());
    led_update();
    sleep(1);
    disable_all_leds();
    enable_leds(DREI, sizeof(DREI), get_led_red(), get_led_green(), get_led_blue());
    led_update();
    sleep(1);
    disable_all_leds();
    enable_leds(ZWEI, sizeof(ZWEI), get_led_red(), get_led_green(), get_led_blue());
    led_update();
    sleep(1);
    disable_all_leds();
    enable_leds(EINS, sizeof(EINS), get_led_red(), get_led_green(), get_led_blue());
    led_update();
    sleep(1);
    disable_all_leds();
    for(int i = 0; i < 240; i++){
      enable_leds_party(ES_IST, sizeof(ES_IST));
      enable_leds_party(UHR, sizeof(UHR));
      enable_leds_party(ZWOELF, sizeof(ZWOELF));
      enable_leds_party(DOTS, sizeof(DOTS));
      led_update();
      delay(500);
    }
    sleep(10);
  }

  void christmas_lights(){
    for(int i=0; i<60; i++){
      enable_leds(DOT_1, 1, 150, 0, 0);
      enable_leds(DOT_2, 1, 0, 150, 0);
      enable_leds(DOT_3, 1, 150, 0, 0);    
      enable_leds(DOT_4, 1, 0, 150, 0);
      led_update();
      delay(500);
      enable_leds(DOT_4, 1, 150, 0, 0);
      enable_leds(DOT_1, 1, 0, 150, 0);
      enable_leds(DOT_2, 1, 150, 0, 0);    
      enable_leds(DOT_3, 1, 0, 150, 0);
      led_update();
      delay(500);
    }
  }