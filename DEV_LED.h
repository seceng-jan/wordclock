
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

#include "extras/PwmPin.h"                          // library of various PWM functions
#include <Adafruit_NeoPixel.h>


struct DEV_RgbLED : Service::LightBulb {       // RGB LED (Command Cathode)

  Adafruit_NeoPixel *mypixels;
  int *R, *G, *B;
  
  SpanCharacteristic *power;                   // reference to the On Characteristic
  SpanCharacteristic *H;                       // reference to the Hue Characteristic
  SpanCharacteristic *S;                       // reference to the Saturation Characteristic
  SpanCharacteristic *V;                       // reference to the Brightness Characteristic
  
  
  DEV_RgbLED(Adafruit_NeoPixel *pixels, int *pR, int *pG, int *pB) : Service::LightBulb(){       // constructor() method
    
    R = pR;
    G = pG;
    B = pB;
    mypixels = pixels;
    power=new Characteristic::On(1);                    
    H=new Characteristic::Hue(0);              // instantiate the Hue Characteristic with an initial value of 0 out of 360
    S=new Characteristic::Saturation(0);       // instantiate the Saturation Characteristic with an initial value of 0%
    V=new Characteristic::Brightness(70);     // instantiate the Brightness Characteristic with an initial value of 100%
    V->setRange(5,100,1);                      // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%
    update();
    
  } // end constructor

  boolean update(){                         // update() method

    boolean p;
    float v, h, s, r, g, b;

    h=H->getVal<float>();                      // get and store all current values.  Note the use of the <float> template to properly read the values
    s=S->getVal<float>();
    v=V->getVal<float>();                      // though H and S are defined as FLOAT in HAP, V (which is brightness) is defined as INT, but will be re-cast appropriately
    p=power->getVal();


    if(power->updated()){
      p=power->getNewVal();
    }
      
    if(H->updated()){
      h=H->getNewVal<float>();
    }

    if(S->updated()){
      s=S->getNewVal<float>();
    }

    if(V->updated()){
      v=V->getNewVal<float>();
    }

    // Here we call a static function of LedPin that converts HSV to RGB.
    // Parameters must all be floats in range of H[0,360], S[0,1], and V[0,1]
    // R, G, B, returned [0,1] range as well

    LedPin::HSVtoRGB(h,s/100.0,v/100.0,&r,&g,&b);   // since HomeKit provides S and V in percent, scale down by 100


    *R=p*255*r;                                      // since LedPin uses percent, scale back up by 100, and multiple by status fo power (either 0 or 1)
    *G=p*255*g;
    *B=p*255*b;



    return(true);                               // return true
  
  } // update
};
      
//////////////////////////////////
