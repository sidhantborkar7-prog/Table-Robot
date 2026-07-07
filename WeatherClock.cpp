#include "WeatherClock.h"

WeatherClock::WeatherClock(Adafruit_SSD1306 *oled)
{
    displayObj = oled;
    temperature = 0.0;
    lastWeatherUpdate = 0;
}

void WeatherClock::begin()
{
    WiFi.begin(ssid, password);

    // Wait up to 10 seconds for WiFi
    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - start < 10000)
    {
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        configTime(19800, 0, "pool.ntp.org");
        getWeather();
        lastWeatherUpdate = millis();
    }
}

void WeatherClock::update()
{
    // Try to reconnect if WiFi is lost
    if (WiFi.status() != WL_CONNECTED)
    {
        static unsigned long lastReconnect = 0;

        if (millis() - lastReconnect > 10000)
        {
            lastReconnect = millis();
            WiFi.reconnect();
        }

        return;
    }

    // Update weather every 10 minutes
    if (millis() - lastWeatherUpdate >= 600000)
    {
        lastWeatherUpdate = millis();
        getWeather();
    }
}

bool WeatherClock::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void WeatherClock::getWeather()
{
    HTTPClient http;

    String url =
        "http://api.openweathermap.org/data/2.5/weather?q=" +
        city + "," + country +
        "&appid=" + apiKey +
        "&units=metric";

    http.begin(url);

    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK)
    {
        String payload = http.getString();

        DynamicJsonDocument doc(2048);

        if (deserializeJson(doc, payload) == DeserializationError::Ok)
        {
            temperature = doc["main"]["temp"].as<float>();
        }
    }

    http.end();
}

void WeatherClock::show()
{
    struct tm timeinfo;

    char timeString[12] = "--:--";

    if (getLocalTime(&timeinfo))
    {
        strftime(timeString, sizeof(timeString), "%I:%M %p", &timeinfo);
    }

    displayObj->clearDisplay();

    displayObj->setTextColor(SSD1306_WHITE);

    // Time
    displayObj->setTextSize(2);
    displayObj->setCursor(15, 5);
    displayObj->print(timeString);

    displayObj->drawLine(0, 24, 128, 24, SSD1306_WHITE);

    // Temperature
    displayObj->setTextSize(3);
    displayObj->setCursor(10, 36);
    displayObj->print(temperature, 1);
    displayObj->print((char)247);
    displayObj->print("C");

    displayObj->display();
}

void WeatherClock::showNoInternet()
{
    displayObj->clearDisplay();

    displayObj->setTextColor(SSD1306_WHITE);

    displayObj->setTextSize(2);
    displayObj->setCursor(8, 18);
    displayObj->print("No Internet");

    displayObj->setTextSize(1);
    displayObj->setCursor(20, 45);
    displayObj->print("Connect to WiFi");

    displayObj->display();
}