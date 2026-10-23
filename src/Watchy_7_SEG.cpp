#include "Watchy_7_SEG.h"
#include "settings.h"

const uint8_t BATTERY_SEGMENT_WIDTH = 7;
const uint8_t BATTERY_SEGMENT_HEIGHT = 11;
const uint8_t BATTERY_SEGMENT_SPACING = 9;
const uint8_t WEATHER_ICON_WIDTH = 48;
const uint8_t WEATHER_ICON_HEIGHT = 32;

void Watchy7SEG::drawWatchFace(){
    display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    drawTime();
    drawDate();
    drawSteps();
    drawWeather();
    drawBattery();
    display.drawBitmap(120, 77, WIFI_CONFIGURED ? wifi : wifioff, 26, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    if(BLE_CONFIGURED){
        display.drawBitmap(100, 75, bluetooth, 13, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
}

void Watchy7SEG::drawTime(){
    display.setFont(&DSEG7_Classic_Bold_53);
    display.setCursor(5, 53+5);
    int displayHour;
    if(HOUR_12_24==12){
      displayHour = ((currentTime.Hour+11)%12)+1;
    } else {
      displayHour = currentTime.Hour;
    }
    if(displayHour < 10){
        display.print("0");
    }
    display.print(displayHour);
    display.print(":");
    if(currentTime.Minute < 10){
        display.print("0");
    }
    display.println(currentTime.Minute);
}

// I like the swiss way of writing the date
String dayStrCh(uint8_t day) {
  switch (day) {
    case 1: return "Sunntig";
    case 2: return "Mäntig";
    case 3: return "Zischtig";
    case 4: return "Mittwuch";
    case 5: return "Donnschtig";
    case 6: return "Fritig";
    case 7: return "Samschtig";
    default: return "";
  }
}

// As there are only 3 letters for the month, we need to use the german abbreviations
String monthShortStrCh(uint8_t month) {
  switch (month) {
    case 1: return "Jan";
    case 2: return "Feb";
    case 3: return "Mär";
    case 4: return "Apr";
    case 5: return "Mai";
    case 6: return "Jun";
    case 7: return "Jul";
    case 8: return "Aug";
    case 9: return "Sep";
    case 10: return "Okt";
    case 11: return "Nov";
    case 12: return "Dez";
    default: return "";
  }
}

void Watchy7SEG::drawDate(){
    display.setFont(&Seven_Segment10pt7b);

    int16_t  x1, y1;
    uint16_t w, h;

    String dayOfWeek = dayStrCh(currentTime.Wday);
    display.getTextBounds(dayOfWeek, 5, 85, &x1, &y1, &w, &h);
    if(currentTime.Wday == 4){
        w = w - 5;
    }
    display.setCursor(90 - w, 90); // Mofified position for longer CH weekday names, was 85 each
    display.println(dayOfWeek);

    String month = monthShortStrCh(currentTime.Month);
    display.getTextBounds(month, 60, 110, &x1, &y1, &w, &h);
    display.setCursor(85 - w, 110);
    display.println(month);

    display.setFont(&DSEG7_Classic_Bold_25);
    display.setCursor(5, 120);
    if(currentTime.Day < 10){
    display.print("0");
    }
    display.println(currentTime.Day);
    display.setCursor(5, 150);
    display.println(tmYearToCalendar(currentTime.Year));// offset from 1970, since year is stored in uint8_t
}
void Watchy7SEG::drawSteps(){
    // reset step counter at midnight
    if (currentTime.Hour == 0 && currentTime.Minute == 0){
        sensor.resetStepCounter();
    }
    uint32_t stepCount = sensor.getCounter();
    // // not yet implemented, logic is a bit more complicated concidering that we wake up only once per minute
    // if (stepCount >= this->lastVibrationStepCount + ACTIVITY_REMINDER && stepCount > 0) {
    //     this->lastVibrationStepCount = stepCount;
    //     Watchy::vibMotor(35, 1);
    // } else if (stepCount >= this->lastVibrationStepCount + ACTIVITY_GOAL && stepCount > 0) {
    //     this->lastVibrationStepCount = stepCount;
    //     Watchy::vibMotor(35, 3);
    // }
    display.drawBitmap(10, 165, steps, 19, 23, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(35, 190);
    display.println(stepCount);
}
void Watchy7SEG::drawBattery(){
    display.drawBitmap(154, 73, battery, 37, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.fillRect(159, 78, 27, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);//clear battery segments
    int8_t batteryLevel = 0;
    float VBAT = getBatteryVoltage();
    if(VBAT > 4.1){
        batteryLevel = 3;
    }
    else if(VBAT > 3.95 && VBAT <= 4.1){
        batteryLevel = 2;
    }
    else if(VBAT > 3.80 && VBAT <= 3.95){
        batteryLevel = 1;
    }
    else if(VBAT <= 3.80){
        batteryLevel = 0;
    }

    for(int8_t batterySegments = 0; batterySegments < batteryLevel; batterySegments++){
        display.fillRect(159 + (batterySegments * BATTERY_SEGMENT_SPACING), 78, BATTERY_SEGMENT_WIDTH, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
}

void Watchy7SEG::drawWeather() {
    
    weatherData currentWeather = getWeatherData();
    if (connectWiFi()) {
        _weatherData = currentWeather;  // Update the stored weather data only when connected to wifi, we don't want the temperature from the build in sensor
    }
    
    int8_t temperature = _weatherData.temperature;
    int16_t weatherConditionCode = _weatherData.weatherConditionCode;

    display.setFont(&DSEG7_Classic_Regular_39);
    int16_t  x1, y1;
    uint16_t w, h;
    display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
    if (159 - w - x1 > 87) {
        display.setCursor(159 - w - x1, 150);
    } else {
        display.setFont(&DSEG7_Classic_Bold_25);
        display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
        display.setCursor(159 - w - x1, 136);
    }
    display.println(temperature);
    display.drawBitmap(165, 110, _weatherData.isMetric ? celsius : fahrenheit, 26, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    const unsigned char* weatherIcon;

    //https://openweathermap.org/weather-conditions
    if (weatherConditionCode > 801) { //Cloudy
        weatherIcon = cloudy;
    } else if (weatherConditionCode == 801) { //Few Clouds
        weatherIcon = cloudsun;
    } else if (weatherConditionCode == 800) { //Clear
        weatherIcon = sunny;
    } else if (weatherConditionCode >= 700) { //Atmosphere
        weatherIcon = atmosphere;
    } else if (weatherConditionCode >= 600) { //Snow
        weatherIcon = snow;
    } else if (weatherConditionCode >= 500) { //Rain
        weatherIcon = rain;
    } else if (weatherConditionCode >= 300) { //Drizzle
        weatherIcon = drizzle;
    } else if (weatherConditionCode >= 200) { //Thunderstorm
        weatherIcon = thunderstorm;
    } else {
        weatherIcon = thereisnocloud;
    }

    display.drawBitmap(145, 158, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

}
