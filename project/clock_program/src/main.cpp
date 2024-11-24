#include <Arduino.h>
#include "esp_mac.h"
#include "credentials.h"
#include <WiFi.h>
#include <SPI.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
//#include <ctime>
#include <time.h>


#ifdef CREDENTIALS
const char* ssid      = SECRET_SSID;               // Set you WiFi SSID
const char* password  = SECRET_PASS;               // Set you WiFi password
#else
const char* ssid     = "FAMFEY01";
const char* password = "1222CL5B0357720191";
#endif


#define NTP_OFFSET  3600 // In seconds 
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "nl.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

// put function declarations here:
String epoch_to_utc(long);

int result = 0;

void setup() {
  // put your setup code here, to run once:
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  timeClient.begin();
  delay(3000);

  Serial.begin(115200);
  Serial.println("Test Test Test");
}

void loop() {
  // put your main code here, to run repeatedly:
  timeClient.update();

  Serial.println(timeClient.getFormattedTime());
  Serial.println(timeClient.getEpochTime());
  Serial.println(epoch_to_utc(timeClient.getEpochTime()));
  Serial.println(timeClient.getDay());


  delay(2000);

}

String epoch_to_utc(long epoch) {
  const time_t old = (time_t)epoch;
  struct tm *oldt = gmtime(&old);
  return asctime(oldt);
}
