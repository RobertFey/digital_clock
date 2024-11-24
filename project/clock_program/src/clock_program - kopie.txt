#include <Arduino.h>
#include <Wire.h>
// #include <SparkFun_APDS9960.h>
#include "Adafruit_APDS9960.h"
#include <FastLED.h>

/***********************************************
 IMPORTANT: The APDS-9960 can only accept 3.3V!
 
 Arduino Pin  APDS-9960 Board  Function
 
 3.3V         VCC              Power
 GND          GND              Ground
 A4/P21           SDA              I2C Data
 A5/P22           SCL              I2C Clock

 https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
***********************************************/


// Global variables
// SparkFun_APDS9960 apds = SparkFun_APDS9960();
Adafruit_APDS9960 apds;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;
uint16_t ambient_light = 0;

uint8_t convert_color(uint16_t color_light, uint16_t ambient_light) {
 uint8_t R = (255. * color_light / ambient_light);
 return R;
}


struct Data_RGBA{
  uint16_t red_light;
  uint16_t green_light;
  uint16_t blue_light;
  uint16_t ambient_light;
};




bool get_light_values(struct Data_RGBA *RGBA) {

    //wait for color data to be ready
    while(!apds.colorDataReady()){

      delay(5);
    }
    apds.getColorData(&red_light, &green_light, &blue_light, &ambient_light);
  if (  !ambient_light ||
        !red_light ||
        !green_light ||
        !blue_light ) {
      return false;
    } else {
      RGBA->red_light = red_light;
      RGBA->green_light = green_light;
      RGBA->blue_light = blue_light;
      RGBA->ambient_light = ambient_light;
      // RGBA = {red_light,green_light,blue_light,ambient_light};
      return true;
  }
}



#define LED_PIN     4
#define NUM_LEDS    14
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
Data_RGBA  lightsensor_data = {0,0,0,1};

// extern CRGBPalette16 myRedWhiteBluePalette;
// extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup() {
  delay( 3000 ); // power-up safety delay
  // Initialize Serial port
  Serial.begin(115200);

  if(!apds.begin()){
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("Device initialized!");

  //enable color sensign mode
  apds.enableColor(true);
 
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  

  // Wait for initialization and calibration to finish
  delay(500);
}

void loop() {
    // Read the light levels (ambient, red, green, blue) and print
  get_light_values(&lightsensor_data);
  Serial.print("Ambient: ");
  Serial.print(lightsensor_data.ambient_light);
  Serial.print(" R: ");
  Serial.print(lightsensor_data.red_light);
  Serial.print(" G: ");
  Serial.print(lightsensor_data.green_light);
  Serial.print(" B: ");
  Serial.println(lightsensor_data.blue_light);
  
  // Wait 1 second before next reading
  delay(1000);

  // een-voor-een
  for(int j = 0; j < 3; j++) {
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      memset(leds, 0, NUM_LEDS * 3);
      switch(j) {
        case 0: leds[i].r = convert_color(lightsensor_data.red_light ,lightsensor_data.ambient_light); break;
        case 1: leds[i].g = convert_color(lightsensor_data.green_light ,lightsensor_data.ambient_light); break;
        case 2: leds[i].b = convert_color(lightsensor_data.blue_light ,lightsensor_data.ambient_light); break;
        // case 0: leds[i].r = 255; break;
        // case 1: leds[i].g = 255; break;
        // case 2: leds[i].b = 255; break;
      }
      FastLED.show();
      delay(10);
    }
  }
  Serial.print(" RGB: ");
  Serial.print(convert_color(lightsensor_data.red_light ,lightsensor_data.ambient_light));
  Serial.print(" , ");
  Serial.print(convert_color(lightsensor_data.green_light ,lightsensor_data.ambient_light));
  Serial.print(" , ");
  Serial.println(convert_color(lightsensor_data.blue_light ,lightsensor_data.ambient_light));
}


