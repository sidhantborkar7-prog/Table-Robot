#ifndef WEATHERCLOCK_H
#define WEATHERCLOCK_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <time.h>

class WeatherClock
{
public:
    // Constructor
    WeatherClock(Adafruit_SSD1306 *oled);

    // Initialize WiFi, Time and first Weather update
    void begin();

    // Call continuously in loop()
    void update();

    // Show Time + Temperature on OLED
    void show();

    // Check WiFi connection
    bool isConnected();

    // Display "No Internet" message
    void showNoInternet();

private:
    // Download weather from OpenWeather
    void getWeather();

    // OLED object
    Adafruit_SSD1306 *displayObj;

    // Weather data
    float temperature;

    // Weather update timer
    unsigned long lastWeatherUpdate;

    // WiFi credentials
    const char *ssid = "Mochi";
    const char *password = "Esp32@321";

    // OpenWeather settings
    String apiKey = "1053cacc28febb66d06ecac70a0464bf";
    String city = "Nagpur";
    String country = "IN";
};

#endif