#include "HomeSpan.h" 
#include <time.h>
//#include "DCF77.h"

#include "DEV_LED.h"     
#include <driver/adc.h>

#include "words.h"
#include "dates.h"

bool wifi_connected = false;

enum STATE {
  OFF,
  ON,
  UPDATE_TIME,
  NEWYEAR,
  CHECK_EVENTS,
}; 

enum STATE s; 

struct tm timeinfo;
time_t now;

#define DCF_PIN 2	         // Connection pin to DCF 77 device
#define DCF_INTERRUPT 0		 // Interrupt number associated with pin

#define HUB_NAME "ESP32-Wordclock"
#define CLOCK_NAME "Wordclock"
//DCF77 DCF = DCF77(DCF_PIN,DCF_INTERRUPT);

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(114, 4, NEO_GRBW + NEO_KHZ800);
const int presistorPin = 35;
int R = 0, G = 0, B = 0, W;
int R_OLD = 0, G_OLD = 0, B_OLD = 0;
int brightness=0;

bool is_birthday = false;
bool is_christmas = false;

int minute = -1;

void setup() {
  Serial.begin(115200);
  s = OFF;
  srand(time(0));

  // DCF Setup
  //DCF.Start();

  // HomeKit Setup
  homeSpan.begin(Category::Bridges, HUB_NAME);
  
  new SpanAccessory();  
    new Service::AccessoryInformation();
      new Characteristic::Identify();

  new SpanAccessory();                                                          
  new Service::AccessoryInformation();    
    new Characteristic::Identify();               
    new Characteristic::Name(CLOCK_NAME); 
  new DEV_RgbLED(&pixels, &R, &G, &B);

  homeSpan.autoPoll();

  // Photoresistor
  //pinMode(presistorPin, INPUT); 
  //pinMode(21, OUTPUT); 
  

  // LED Setup

  pixels.begin();
  pixels.clear(); // Set all pixel colors to 'off'
  Serial.print("\n+++Pixels Started\n");

  // Wait for WiFi connection (or Signal from the DCF / To be implemented)
  while ( WiFi.status() != WL_CONNECTED) {
  //while ( WiFi.status() != WL_CONNECTED || DCF.getTime() != 0) {
    delay (500);
    toggle_led(DOT_1, 0, 0, 128);
    toggle_led(DOT_2, 0, 0, 128);
    toggle_led(DOT_3, 0, 0, 128);
    toggle_led(DOT_4, 0, 0, 128);
    pixels.show();
    //homeSpan.poll();
  }
  wifi_connected = true;

  configTime(3600, 3600, "pool.ntp.org");
  // Set timezone to Germany
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
  tzset();

  
  time(&now);

} // end of setup()

//////////////////////////////////////
struct RGBW {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
};

int min3(int a, int b, int c){
  if(a < b){
    if(a < c) return a;
    else return c;
  }else{
    if(b < c) return b;
    else return c;
  }
}

inline struct RGBW rgb_2_rgbw(int r, int g, int b) {
    uint8_t min_component = min3(r, g, b);
    uint8_t w = min_component;
    bool is_min = true;
    // if (min_component <= 84) {
    //     w = 3 * min_component;
    // } else {
    //     w = 255;
    //     is_min = false;
    // }
    uint8_t r_prime;
    uint8_t g_prime;
    uint8_t b_prime;
    // if (is_min) {
        r_prime = r - min_component;
        g_prime = g - min_component;
        b_prime = b - min_component;
    // } else {
    //     uint8_t w3 = w / 3;
    //     r_prime = r - w3;
    //     g_prime = g - w3;
    //     b_prime = b - w3;
    // }
    struct RGBW out = { r_prime, g_prime, b_prime, w };
    return out;
}

void start_new_year_countdown(){
  disable_all_leds();
  enable_leds(ZWANZIG_1, sizeof(ZWANZIG_1), R, G, B);
  pixels.show();
  sleep(10);
  disable_all_leds();
  enable_leds(ZEHN_1, sizeof(ZEHN_1), R, G, B);
  pixels.show();
  sleep(1);
  disable_all_leds();
  enable_leds(NEUN, sizeof(NEUN), R, G, B);
  pixels.show();
  sleep(1);
  disable_all_leds();
  enable_leds(ACHT, sizeof(ACHT), R, G, B);
  pixels.show();
  sleep(1);
  disable_all_leds();
  enable_leds(SIEBEN, sizeof(SIEBEN), R, G, B);
  pixels.show();
  sleep(1);
  disable_all_leds();
  enable_leds(SECHS, sizeof(SECHS), R, G, B);
  pixels.show();
  sleep(1);
  disable_all_leds();
  enable_leds(FUENF, sizeof(FUENF), R, G, B);
  pixels.show();
  sleep(1);
  disable_all_leds();
  enable_leds(VIER, sizeof(VIER), R, G, B);
  pixels.show();
  sleep(1);
  disable_all_leds();
  enable_leds(DREI, sizeof(DREI), R, G, B);
  pixels.show();
  sleep(1);
  disable_all_leds();
  enable_leds(ZWEI, sizeof(ZWEI), R, G, B);
  pixels.show();
  sleep(1);
  disable_all_leds();
  enable_leds(EINS, sizeof(EINS), R, G, B);
  pixels.show();
  sleep(1);
  disable_all_leds();
  for(int i = 0; i < 240; i++){
    enable_leds_party(ES_IST, sizeof(ES_IST));
    enable_leds_party(UHR, sizeof(UHR));
    enable_leds_party(ZWOELF, sizeof(ZWOELF));
    enable_leds_party(DOTS, sizeof(DOTS));
    pixels.show();
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
    pixels.show();
    delay(500);
    enable_leds(DOT_4, 1, 150, 0, 0);
    enable_leds(DOT_1, 1, 0, 150, 0);
    enable_leds(DOT_2, 1, 150, 0, 0);    
    enable_leds(DOT_3, 1, 0, 150, 0);
    pixels.show();
    delay(500);
  }
}

void show_time(int hours, int minutes, uint8_t R, uint8_t G, uint8_t B){
  R_OLD = R;
  G_OLD = G; 
  B_OLD = B;
  disable_all_leds();
  enable_leds(ES_IST, sizeof(ES_IST), R, G, B);

  // Set minutes
  bool h_plus_one = false;
  if(minutes < 5){
    if(is_birthday && minutes < 1){
      disable_all_leds();
      enable_leds_glow(HEART_2, sizeof(HEART_2), 200, 0,0);
      return;
    }
    enable_leds(UHR, sizeof(UHR), R, G, B);
  }else if(minutes < 10){
    enable_leds(FUENF_1, sizeof(FUENF_1), R, G, B);
    enable_leds(NACH, sizeof(NACH), R, G, B);
  }else if(minutes < 15){
    enable_leds(ZEHN_1, sizeof(ZEHN_1), R, G, B);
    enable_leds(NACH, sizeof(NACH), R, G, B);
  }else if(minutes < 20){
    enable_leds(VIERTEL, sizeof(VIERTEL), R, G, B);
    enable_leds(NACH, sizeof(NACH), R, G, B);
  }else if(minutes < 25){
    enable_leds(ZWANZIG_1, sizeof(ZWANZIG_1), R, G, B);
    enable_leds(NACH, sizeof(NACH), R, G, B);
  }else if(minutes < 30){
    enable_leds(FUENF_1, sizeof(FUENF_1), R, G, B);
    enable_leds(VOR, sizeof(VOR), R, G, B);
    enable_leds(HALB, sizeof(HALB), R, G, B);
    h_plus_one = true;
  }else if(minutes < 35){
    enable_leds(HALB, sizeof(HALB), R, G, B);
    h_plus_one = true;
  }else if(minutes < 40){
    enable_leds(FUENF_1, sizeof(FUENF_1), R, G, B);
    enable_leds(NACH, sizeof(NACH), R, G, B);
    enable_leds(HALB, sizeof(HALB), R, G, B);
    h_plus_one = true;
  }else if(minutes < 45){
    enable_leds(ZWANZIG_1, sizeof(ZWANZIG_1), R, G, B);
    enable_leds(VOR, sizeof(VOR), R, G, B);
    h_plus_one = true;
  }else if(minutes < 50){
    enable_leds(VIERTEL, sizeof(VIERTEL), R, G, B);
    enable_leds(VOR, sizeof(VOR), R, G, B);
    h_plus_one = true;
  }else if(minutes < 55){
    enable_leds(ZEHN_1, sizeof(ZEHN_1), R, G, B);
    enable_leds(VOR, sizeof(VOR), R, G, B);
    h_plus_one = true;
  }else{
    enable_leds(FUENF_1, sizeof(FUENF_1), R, G, B);
    enable_leds(VOR, sizeof(VOR), R, G, B);
    h_plus_one = true;
  }

  // Set minute dots
  int minor = minutes % 5;
  if(minor >= 1){
    enable_leds(DOT_1, sizeof(DOT_1), R, G, B);
  }
  if(minor >= 2){
    enable_leds(DOT_2, sizeof(DOT_2), R, G, B);
  }
  if(minor >= 3){
    enable_leds(DOT_3, sizeof(DOT_3), R, G, B);
  }
  if(minor >= 4){
    enable_leds(DOT_4, sizeof(DOT_4), R, G, B);
  }

  // Set hours
  if (h_plus_one){
    hours++;
  }
  hours = hours % 12;

  switch(hours){
    case(0): enable_leds(ZWOELF, sizeof(ZWOELF), R, G, B); break;
    case(1): if(minutes < 5){
        enable_leds(EIN, sizeof(EIN), R, G, B); break;
      }else{
        enable_leds(EINS, sizeof(EINS), R, G, B); break;
      }
    case(2): enable_leds(ZWEI, sizeof(ZWEI), R, G, B); break;
    case(3): enable_leds(DREI, sizeof(DREI), R, G, B); break;
    case(4): enable_leds(VIER, sizeof(VIER), R, G, B); break;
    case(5): enable_leds(FUENF, sizeof(FUENF), R, G, B); break;
    case(6): enable_leds(SECHS, sizeof(SECHS), R, G, B); break;
    case(7): enable_leds(SIEBEN, sizeof(SIEBEN), R, G, B); break;
    case(8): enable_leds(ACHT, sizeof(ACHT), R, G, B); break;
    case(9): enable_leds(NEUN, sizeof(NEUN), R, G, B); break;
    case(10): enable_leds(ZEHN, sizeof(ZEHN), R, G, B); break;
    case(11): enable_leds(ELF, sizeof(ELF), R, G, B); break;
  }

  if(is_christmas && minutes < 2){
    christmas_lights();
  }
}

void disable_all_leds(){
  pixels.clear();
}

void enable_leds_party(const uint8_t* ptr, int lenght){
  for(int i =  0; i < lenght/sizeof(uint8_t); i++){
    uint8_t colors[3] = {0, 0, 0};
    colors[rand()%3] = 150;
    pixels.setPixelColor(ptr[i], pixels.Color(rand()%150, rand()%150, rand()%150));
  }
}

void enable_leds_glow(const uint8_t* ptr, int lenght, uint8_t r, uint8_t g, uint8_t b){
  for(int i =  0; i < lenght/sizeof(uint8_t); i++){
    int r_current = (pixels.getPixelColor(ptr[i]) >> 16) & 0xFF;
    int g_current = (pixels.getPixelColor(ptr[i]) >> 8) & 0xFF;
    int b_current = pixels.getPixelColor(ptr[i]) & 0xFF;
    r_current = r_current+((r - r_current) >> 3)+(rand()%3)-1;
    g_current = g_current+((g - g_current) >> 3)+(rand()%3)-1;
    b_current = b_current+((b - b_current) >> 3)+(rand()%3)-1;
    if(r_current < 0) r_current = 0;
    if(g_current < 0) g_current = 0;
    if(b_current < 0) b_current = 0;
    pixels.setPixelColor(ptr[i], pixels.Color(r_current, g_current, b_current));
  }
}

void enable_leds(const uint8_t* ptr, int lenght, uint8_t r, uint8_t g, uint8_t b){
  struct RGBW col = rgb_2_rgbw(r, g, b);

  for(int i =  0; i < lenght/sizeof(uint8_t); i++){
    pixels.setPixelColor(ptr[i], pixels.Color(col.r, col.g, col.b, col.w));
  }
}

void toggle_led(const uint8_t* led, uint8_t r, uint8_t g, uint8_t b){
  uint32_t color = pixels.getPixelColor(*led);
  if((color & 0xFF) == 0 && ((color >> 8) & 0xFF) == 0 && ((color >> 16) & 0xFF) == 0){
    pixels.setPixelColor(*led, pixels.Color(r, g, b));
  } else{
    pixels.setPixelColor(*led, pixels.Color(0, 0, 0));
  }
}

void fetchTime(){
  time(&now); // calls the NTP server once a hour
  localtime_r(&now, &timeinfo);
  //Serial.println(timeinfo.tm_mon);
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void loop(){  
  switch(s){
    case ON:
      fetchTime();
      // Check for new year since that is obviously the most important
      if(timeinfo.tm_mon == 11 && timeinfo.tm_mday == 31 && timeinfo.tm_hour == 23 && timeinfo.tm_min == 59 && timeinfo.tm_sec == 40){
        s = NEWYEAR;
        break;
      }
      if(timeinfo.tm_min != minute){
        minute = timeinfo.tm_min;
        s = UPDATE_TIME;
        break;
      }
      if(R != R_OLD || G != G_OLD || B != B_OLD){
        s = UPDATE_TIME;
        break;
      }
      if(timeinfo.tm_hour == 0 && timeinfo.tm_min == 0 && timeinfo.tm_sec == 0){
        s = CHECK_EVENTS;
      }
      break;
    case NEWYEAR:
      start_new_year_countdown();
      s = ON;
      break;
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
        show_time(timeinfo.tm_hour, timeinfo.tm_min, R, G, B);
        if(R == 0 && G == 0 && B == 0){
          s = OFF;
        }else{
          s = ON;
        }
        pixels.show();
      break;
    case OFF:
      if(R != 0 || G != 0 || B != 0){
        s = ON;
      }
      sleep(2);
      break;
  }

  

  
  
  // DCF Time
  //time_t DCFtime = DCF.getTime();
  //if (DCFtime!=0)
  //{
  //  Serial.println("DCF Time is updated");
    //Serial.println(&DCFtime, "%A, %B %d %Y %H:%M:%S");
  //}	
  
  // Light Sensor
  // digitalWrite(21, HIGH);
  //brightness = (analogRead(presistorPin)/3300.0)*255;
  //double b = analogRead(presistorPin);
  //Serial.println(b);
  
  
  // Test Code
  // for(int i = 0; i<114; i++){
  //   pixels.clear();
  //   pixels.setPixelColor(i, pixels.Color(100, 100,100));
  //   pixels.show();
  //   delay(500);
  // }
  

  //delay(1000);
  
    
} // end of loop()
