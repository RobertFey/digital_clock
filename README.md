# digital_clock
A digital clock with RGB LED, ESP32.

# functionality to add
* surrounding light and color detection
* NTP time sync
* WIFI connection initial startup as AP if there is no config
* WIFI config via webinterface
* Clock config via webinterface
* MQTT interface for Homeassistant
* Display time HH:MM
* Display sec dot
* Display day of the week
* Display WIFI status
* Display Daylight savings status

# casing
The plan is to laser cut the case parts out of multiplex hardwood. 

The windows for all indications are made out of transparent or translucent rasin

# hardware
*. ESP32 WROVER
*. LED WS2812B
*. RGB sensor APDS 9960
*. I2C memory for storing settings
