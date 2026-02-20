#include <Arduino.h>
#include "Watchy_7_SEG.h"
#include "settings.h"
#include "secrets.h"

// Initialize the settings object
watchySettings settings{
    .cityID = CITY_ID,
    .weatherAPIKey = OPENWEATHERMAP_APIKEY,
    .weatherURL = OPENWEATHERMAP_URL,
    .weatherUnit = TEMP_UNIT,
    .weatherLang = TEMP_LANG,
    .weatherUpdateInterval = WEATHER_UPDATE_INTERVAL,
    .ntpServer = NTP_SERVER,
    .gmtOffset = GMT_OFFSET_SEC,
    .vibrateOClock = false,
};

// Now create your watchy object with the populated settings
Watchy7SEG watchy(settings);

void setup(){
  // Init the watchy with the settings
  watchy.init();
}

void loop(){}
