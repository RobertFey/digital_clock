#include <Wire.h>
#include <SparkFun_APDS9960.h>

/***********************************************
 IMPORTANT: The APDS-9960 can only accept 3.3V!
 
 Arduino Pin  APDS-9960 Board  Function
 
 3.3V         VCC              Power
 GND          GND              Ground
 A4           SDA              I2C Data
 A5           SCL              I2C Clock
***********************************************/


// Global variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;
uint16_t ambient_light = 0;


struct Data_RGBA {
  uint16_t red_light;
  uint16_t green_light;
  uint16_t blue_light;
  uint16_t ambient_light;
};

void get_light_values(Data_RGBA &RGBA) {
  //uint16_t red_light = 0;
  //uint16_t green_light = 0;
  //uint16_t blue_light = 0;
  //uint16_t ambient_light = 0;
  if (  !apds.readAmbientLight(ambient_light) ||
        !apds.readRedLight(red_light) ||
        !apds.readGreenLight(green_light) ||
        !apds.readBlueLight(blue_light) ) {
      break;
    } else {
      RGBA.red_light = red_light;
      RGBA.green_light = green_light;
      RGBA.blue_light = blue_light;
      RGBA.ambient_light = ambient_light;
  }
}

Data_RGBA = lightsensor_data;

void setup() {

  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - ColorSensor"));
  Serial.println(F("--------------------------------"));

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }

  // Start running the APDS-9960 light sensor (no interrupts)
  if ( apds.enableLightSensor(false) ) {
    Serial.println(F("Light sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during light sensor init!"));
  }

  // Wait for initialization and calibration to finish
  delay(500);
}

void loop() {
    // Read the light levels (ambient, red, green, blue) and print
  get_light_values(lightsensor_data);
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
}

