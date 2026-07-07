#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FluxGarage_RoboEyes.h>

#undef N
#undef E
#undef S
#undef W
#undef NE
#undef NW
#undef SE
#undef SW

#include "WeatherClock.h"
#include "Idle.h"
#include "Sleep.h"
#include "LoveEyes.h"
#include "WeatherClock.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define TOUCH_PIN 4

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    OLED_RESET);

LoveEyes love;
WeatherClock weather(&display);

RoboEyes<Adafruit_SSD1306> roboEyes(display);

// Face States

enum FaceState
{
    FACE_SLEEP,
    FACE_IDLE,
    FACE_TIRED,
    FACE_CURIOUS,
    FACE_ANGRY,
    FACE_LOVE,
    FACE_WEATHER
};

FaceState currentFace = FACE_SLEEP;

 
// Touch Variables
 

bool lastTouchState = LOW;

unsigned long touchStartTime = 0;
unsigned long lastTapTime = 0;
unsigned long lastInteractionTime = 0;

int tapCount = 0;

const unsigned long DOUBLE_TAP_TIME = 500;
const unsigned long LONG_PRESS_TIME    = 4000;
const unsigned long WEATHER_PRESS_TIME = 7000;
const unsigned long SLEEP_TIMEOUT = 10000;

 
// RoboEyes
 

void setRoboMood(uint8_t mood)
{
    roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100);

    roboEyes.setPosition(DEFAULT);

    switch (mood)
    {
        case DEFAULT:

            roboEyes.setMood(DEFAULT);
            roboEyes.setCuriosity(ON);
            roboEyes.setIdleMode(ON,2,2);
            roboEyes.setAutoblinker(ON,3,2);
            break;

        case TIRED:

            roboEyes.setMood(TIRED);
            roboEyes.setCuriosity(OFF);
            roboEyes.setIdleMode(ON,4,2);
            roboEyes.setAutoblinker(ON,5,1);
            break;

        case ANGRY:

            roboEyes.setMood(ANGRY);
            roboEyes.setCuriosity(OFF);
            roboEyes.setIdleMode(ON,2,1);
            roboEyes.setAutoblinker(ON,5,1);
            break;
    }
}

 

void setup()
{
    // Serial.begin(115200);
    if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C))
    {
        while(1);
    }
    love.begin();
    weather.begin();

    pinMode(TOUCH_PIN, INPUT);

    randomSeed(analogRead(0));

    lastInteractionTime = millis();

    currentFace = FACE_SLEEP;

    // Serial.println("Mochi Started");
}

 

void loop()
{
    weather.update();
    unsigned long now = millis();

    bool touch = digitalRead(TOUCH_PIN);

    // Exit Love or Weather mode immediately when touch is released
    if ((currentFace == FACE_LOVE || currentFace == FACE_WEATHER) && !touch)
    {
        currentFace = FACE_IDLE;

        setRoboMood(DEFAULT);

        tapCount = 0;
        lastTapTime = 0;

        lastInteractionTime = millis();
    }

  
    // Touch Press
  

    if(touch && !lastTouchState)
    {
        touchStartTime = now;
    }

   
     // Long Press
   

    // 4–6 seconds -> Love
    if (touch &&
        (now - touchStartTime >= LONG_PRESS_TIME) &&
        (now - touchStartTime < WEATHER_PRESS_TIME))
    {
        currentFace = FACE_LOVE;
    }

    // 6 seconds or more -> Weather
    if (touch &&
        (now - touchStartTime >= WEATHER_PRESS_TIME))
    {
        currentFace = FACE_WEATHER;
        lastInteractionTime = now;
    }
      
        // Touch Release
      

    if (!touch &&
        lastTouchState &&
        currentFace != FACE_LOVE &&
        currentFace != FACE_WEATHER)
    {
        tapCount++;
        lastTapTime = now;
    }

  
    // Detect Taps
  

    if(tapCount > 0 && (now - lastTapTime > DOUBLE_TAP_TIME))
    {
        if(tapCount == 1)
        {
            currentFace = FACE_IDLE;
        }

        else if(tapCount == 2)
        {
            currentFace = FACE_TIRED;
            setRoboMood(DEFAULT);
        }

        else if(tapCount == 3)
        {
            currentFace = FACE_CURIOUS;  
            setRoboMood(TIRED);
        }

        else if(tapCount >= 4)
        {
            currentFace = FACE_ANGRY;
            setRoboMood(ANGRY);

        }

        lastInteractionTime = now;
        tapCount = 0;
    }

  
    // Auto Sleep
  

    if(now-lastInteractionTime>SLEEP_TIMEOUT)
    {
        currentFace=FACE_SLEEP;
    }

  

    lastTouchState=touch;

  
    // Draw Face
  

    switch(currentFace)
    {
        case FACE_SLEEP:
            sleepFace();
            break;

        case FACE_IDLE:
            idleFace();
            break;

        case FACE_TIRED:
        case FACE_CURIOUS:
        case FACE_ANGRY:
            roboEyes.update();
            break;
        case FACE_LOVE:
            love.update();
            break;
        case FACE_WEATHER:

        if(weather.isConnected())
        {
            weather.show();
        }
        else
        {
            weather.showNoInternet();
        }

        break;
    }

    delay(20);
}