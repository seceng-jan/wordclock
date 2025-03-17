#include "main.h"


void setup() {
  
  // Initialize Serial
  Serial.begin(115200);
  
  // HomeKit Setup
  homeSpan.begin(Category::Bridges, HUB_NAME);
  homeSpan.autoPoll();

  // Led Init
  led_init();

  s = OFF;
  srand(time(0));

  // Wait for WiFi connection
  while ( WiFi.status() != WL_CONNECTED) {
    delay (500);
    toggle_led(DOT_1, 0, 0, 128);
    toggle_led(DOT_2, 0, 0, 128);
    toggle_led(DOT_3, 0, 0, 128);
    toggle_led(DOT_4, 0, 0, 128);
    led_update();
  }
  wifi_connected = true;

  configTime(3600, 3600, "pool.ntp.org");
  // Set timezone to Germany
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
  tzset();

  
  time(&now);

} // end of setup()

void loop(){  
  switch(s){
    // The clock is on, check if something has changed
    case ON:
      fetchTime();
      // Check for new year
      if(timeinfo.tm_mon == 11 && timeinfo.tm_mday == 31 && timeinfo.tm_hour == 23 && timeinfo.tm_min == 59 && timeinfo.tm_sec == 40){
        s = NEWYEAR;
        break;
      }
      // Delay for glow effect
      if(is_birthday){
        if(ctr == 10000){
          ctr = 0;
          s = UPDATE_TIME;
        }else{
          ctr ++;
        }
        break;
      }
      // Check if a minute has passed
      if(timeinfo.tm_min != minute){
        minute = timeinfo.tm_min;
        s = UPDATE_TIME;
        break;
      }
      // Check if the colors have changed
      if(is_led_changed()){
        s = UPDATE_TIME;
        break;
      }
      // If it is midnight, check for events
      if(timeinfo.tm_hour == 0 && timeinfo.tm_min == 0 && timeinfo.tm_sec == 0){
        s = CHECK_EVENTS;
      }
      break;
    // New year countdown
    case NEWYEAR:
      start_new_year_countdown();
      s = ON;
      break;
    // Check for events
    case CHECK_EVENTS:
      is_birthday = check_birthdays(timeinfo.tm_mday, timeinfo.tm_mon);
      if(timeinfo.tm_mon == 11 && timeinfo.tm_mday >=24 && timeinfo.tm_mday <=26){
        is_christmas = true;
      }else{
        is_christmas = false;
      }
      s = ON;
      break;
    case UPDATE_TIME:
        show_time(timeinfo.tm_hour, timeinfo.tm_min, get_led_red(), get_led_green(), get_led_blue());
        if(is_led_on()){
          s = ON;
        }else{
          s = OFF;
        }
        led_update();
      break;
    case OFF:
      if(is_led_on()){
        s = CHECK_EVENTS;
      }
      sleep(2);
      break;
  }
    
} // end of loop()

void fetchTime(){
  time(&now); // calls the NTP server once a hour
  localtime_r(&now, &timeinfo);
  //Serial.println(timeinfo.tm_mon);
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void show_time(int hours, int minutes, uint8_t r, uint8_t g, uint8_t b){
  disable_all_leds();
  enable_leds(ES_IST, sizeof(ES_IST), r, g, b);
   
  // Set minutes
  bool h_plus_one = false;
  if(minutes < 5){
    if(is_birthday && minutes < 1){
      disable_all_leds();
      enable_leds_glow(HEART_2, sizeof(HEART_2), 200, 0,0);
      return;
    }
    enable_leds(UHR, sizeof(UHR), r, g, b);
  }else if(minutes < 10){
    enable_leds(FUENF_1, sizeof(FUENF_1), r, g, b);
    enable_leds(NACH, sizeof(NACH), r, g, b);
  }else if(minutes < 15){
    enable_leds(ZEHN_1, sizeof(ZEHN_1), r, g, b);
    enable_leds(NACH, sizeof(NACH), r, g, b);
  }else if(minutes < 20){
    enable_leds(VIERTEL, sizeof(VIERTEL), r, g, b);
    enable_leds(NACH, sizeof(NACH), r, g, b);
  }else if(minutes < 25){
    enable_leds(ZWANZIG_1, sizeof(ZWANZIG_1), r, g, b);
    enable_leds(NACH, sizeof(NACH), r, g, b);
  }else if(minutes < 30){
    enable_leds(FUENF_1, sizeof(FUENF_1), r, g, b);
    enable_leds(VOR, sizeof(VOR), r, g, b);
    enable_leds(HALB, sizeof(HALB), r, g, b);
    h_plus_one = true;
  }else if(minutes < 35){
    enable_leds(HALB, sizeof(HALB), r, g, b);
    h_plus_one = true;
  }else if(minutes < 40){
    enable_leds(FUENF_1, sizeof(FUENF_1), r, g, b);
    enable_leds(NACH, sizeof(NACH), r, g, b);
    enable_leds(HALB, sizeof(HALB), r, g, b);
    h_plus_one = true;
  }else if(minutes < 45){
    enable_leds(ZWANZIG_1, sizeof(ZWANZIG_1), r, g, b);
    enable_leds(VOR, sizeof(VOR), r, g, b);
    h_plus_one = true;
  }else if(minutes < 50){
    enable_leds(VIERTEL, sizeof(VIERTEL), r, g, b);
    enable_leds(VOR, sizeof(VOR), r, g, b);
    h_plus_one = true;
  }else if(minutes < 55){
    enable_leds(ZEHN_1, sizeof(ZEHN_1), r, g, b);
    enable_leds(VOR, sizeof(VOR), r, g, b);
    h_plus_one = true;
  }else{
    enable_leds(FUENF_1, sizeof(FUENF_1), r, g, b);
    enable_leds(VOR, sizeof(VOR), r, g, b);
    h_plus_one = true;
  }

  // Set minute dots
  int minor = minutes % 5;
  if(minor >= 1){
    enable_leds(DOT_1, sizeof(DOT_1), r, g, b);
  }
  if(minor >= 2){
    enable_leds(DOT_2, sizeof(DOT_2), r, g, b);
  }
  if(minor >= 3){
    enable_leds(DOT_3, sizeof(DOT_3), r, g, b);
  }
  if(minor >= 4){
    enable_leds(DOT_4, sizeof(DOT_4), r, g, b);
  }

  // Set hours
  if (h_plus_one){
    hours++;
  }
  hours = hours % 12;

  switch(hours){
    case(0): enable_leds(ZWOELF, sizeof(ZWOELF), r, g, b); break;
    case(1): if(minutes < 5){
        enable_leds(EIN, sizeof(EIN), r, g, b); break;
      }else{
        enable_leds(EINS, sizeof(EINS), r, g, b); break;
      }
    case(2): enable_leds(ZWEI, sizeof(ZWEI), r, g, b); break;
    case(3): enable_leds(DREI, sizeof(DREI), r, g, b); break;
    case(4): enable_leds(VIER, sizeof(VIER), r, g, b); break;
    case(5): enable_leds(FUENF, sizeof(FUENF), r, g, b); break;
    case(6): enable_leds(SECHS, sizeof(SECHS), r, g, b); break;
    case(7): enable_leds(SIEBEN, sizeof(SIEBEN), r, g, b); break;
    case(8): enable_leds(ACHT, sizeof(ACHT), r, g, b); break;
    case(9): enable_leds(NEUN, sizeof(NEUN), r, g, b); break;
    case(10): enable_leds(ZEHN, sizeof(ZEHN), r, g, b); break;
    case(11): enable_leds(ELF, sizeof(ELF), r, g, b); break;
  }

  if(is_christmas && minutes < 2){
    christmas_lights();
  }
}
