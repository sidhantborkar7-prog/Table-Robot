#ifndef LOVEEYES_H
#define LOVEEYES_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class LoveEyes
{
public:
    LoveEyes();

    void begin();
    void update();

private:
    Adafruit_SSD1306 display;

    // Eye positions
    const int eyeY = 32;
    const int leftEyeX = 38;
    const int rightEyeX = 90;

    int floatOffset;
    int floatDir;

    bool blink;

    unsigned long lastBlink;
    unsigned long blinkStart;

    int eyeOffsetX;

    struct HeartParticle
    {
        int x;
        int y;
        int speed;
    };

    HeartParticle hearts[5];

    void animateFloat();

    void drawHeart(int x, int y, int s);

    void drawLoveEyes();

    void drawBlink();

    void drawSmallHeart(int x, int y);

    void drawFloatingHearts();

    void pulseHearts();

    void randomMovement();
};

#endif