#ifndef SETTINGS_H
#define SETTINGS_H

// Display Settings
#define DARKMODE false

//Weather Settings
#define CITY_ID "2659811" // Get your city ID from http://bulk.openweathermap.org/sample/city.list.json.gz
#define OPENWEATHERMAP_URL "http://api.openweathermap.org/data/2.5/weather?id=" //open weather api
#define TEMP_UNIT "metric" //metric = Celsius , imperial = Fahrenheit
#define TEMP_LANG "de"
#define WEATHER_UPDATE_INTERVAL 6 //must be greater than 5, measured in minutes
//NTP Settings
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 3600 * +2 //New York is UTC -5 EST, -4 EDT, will be overwritten by weather data

//Steps Settings
#define ACTIVITY_GOAL 200 //steps
#define ACTIVITY_REMINDER 50 //steps

extern watchySettings settings;

#endif
