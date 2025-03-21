#include "led_control.h"

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(114, 4, NEO_GRBW + NEO_KHZ800);

int R = 0, G = 0, B = 0;
int R_OLD = 0, G_OLD = 0, B_OLD = 0;

void led_init(){
  Serial.println("LED INit");
  
  Serial.println("+++RGB LED Service Started");
  // LED Setup
  pixels.begin();
  pixels.clear(); // Set all pixel colors to 'off'
  Serial.print("\n+++Pixels Started\n");
}

void led_update(){
  R_OLD = R;
  G_OLD = G; 
  B_OLD = B;
  pixels.show();
}

bool is_led_changed(){
  if(R != R_OLD || G != G_OLD || B != B_OLD){
    return true;
  }
  return false;
}

bool is_led_on(){
  if(R != 0 || G != 0 || B != 0){
    return true;
  }
  return false;
}

uint8_t get_led_red(){
  return R;
}

uint8_t get_led_green(){
  return G;
}

uint8_t get_led_blue(){
  return B;
}

void disable_all_leds(){
  pixels.clear();
}
  
void enable_leds_party(const uint8_t* ptr, int lenght){
  for(int i =  0; i < lenght/sizeof(uint8_t); i++){
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
  #if (LED_IS_RGBW == 1)
    struct RGBW col = rgb_2_rgbw(r, g, b);

    for(int i =  0; i < lenght/sizeof(uint8_t); i++){
      pixels.setPixelColor(ptr[i], pixels.Color(col.r, col.g, col.b, col.w));
    }
  #else
    for(int i =  0; i < lenght/sizeof(uint8_t); i++){
      pixels.setPixelColor(ptr[i], pixels.Color(r, g, b));
    } 
  #endif
}

void toggle_led(const uint8_t* led, uint8_t r, uint8_t g, uint8_t b){
  uint32_t color = pixels.getPixelColor(*led);
  if((color & 0xFF) == 0 && ((color >> 8) & 0xFF) == 0 && ((color >> 16) & 0xFF) == 0){
    pixels.setPixelColor(*led, pixels.Color(r, g, b));
  } else{
    pixels.setPixelColor(*led, pixels.Color(0, 0, 0));
  }
}

inline struct RGBW rgb_2_rgbw(uint8_t r, uint8_t g, uint8_t b) {
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

int min3(int a, int b, int c){
  if(a < b){
    if(a < c) return a;
    else return c;
  }else{
    if(b < c) return b;
    else return c;
  }
}
