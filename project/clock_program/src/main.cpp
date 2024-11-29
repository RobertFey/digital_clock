#include <Arduino.h>
#include "esp_mac.h"
#include "credentials.h"
#include <WiFi.h>
#include <SPI.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
//#include <ctime>
#include <time.h>
#include <ESP32Time.h>
#include <RTClib.h>

#define CREDENTIALS true

#ifdef CREDENTIALS
const char* ssid      = SECRET_SSID;               // Set you WiFi SSID
const char* password  = SECRET_PASS;               // Set you WiFi password
#else
const char* ssid     = "";
const char* password = "";
#endif


#define NTP_OFFSET  3600 // In seconds 
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "nl.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

hw_timer_t *Timer0_Cfg = NULL;
// put function declarations here:
String epoch_to_utc(long);
bool fetch_ntp_epoch(void);
void IRAM_ATTR Timer0_ISR(void);

ESP32Time rtc_esp32(0);
RTC_DS3231 rtc_module;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  
  timeClient.begin();
  rtc_module.begin();
  delay(2000);

  Serial.println("Test Test Test");

  Timer0_Cfg = timerBegin(0, 80, true);
  timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR, true);
  timerAlarmWrite(Timer0_Cfg, 10000000, true);
  timerAlarmEnable(Timer0_Cfg);
}

bool testinterrupt = false;
u_int epochtime = 0;

void loop() {
  DateTime now = rtc_module.now();

  Serial.println(rtc_esp32.getTimeDate(true));
  Serial.println(now.timestamp(now.TIMESTAMP_FULL));
  
  bool test = fetch_ntp_epoch();
  Serial.println(test);
  delay(10000);

}

String epoch_to_utc(long epoch) {
  const time_t old = (time_t)epoch;
  struct tm *oldt = gmtime(&old);
  return  asctime(oldt);
}


/*!
    @brief  returns a bool if ntp is succesfull and update the RTC clocks.
    @return true is succesfull false if failed.
*/
bool fetch_ntp_epoch(void) {
  int n = 0;
  do {
    timeClient.update();
    Serial.print(".");
  } while (!timeClient.isTimeSet() and (++n <= 10));

  if (timeClient.isTimeSet()) {
    epochtime = timeClient.getEpochTime();
    rtc_esp32.setTime(epochtime);
    rtc_module.adjust(DateTime(epochtime));
    return true;
  } else {
    rtc_esp32.setTime(rtc_module.now().unixtime());
    return false;
  }
}


void IRAM_ATTR Timer0_ISR(void)
{
  testinterrupt = true;
}
