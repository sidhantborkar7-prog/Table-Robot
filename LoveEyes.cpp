#include "LoveEyes.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define OLED_ADDR 0x3C

#define SDA_PIN 8
#define SCL_PIN 9

LoveEyes::LoveEyes()
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET)
{
    floatOffset = 0;
    floatDir = 1;

    blink = false;

    lastBlink = 0;
    blinkStart = 0;

    eyeOffsetX = 0;

    hearts[0] = {20,60,1};
    hearts[1] = {45,70,2};
    hearts[2] = {70,65,1};
    hearts[3] = {95,75,2};
    hearts[4] = {115,60,1};
}

void LoveEyes::begin()
{
    Wire.begin(SDA_PIN, SCL_PIN);

    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

    display.clearDisplay();
    display.display();

    randomSeed(millis());
}

void LoveEyes::update()
{
    animateFloat();

    if(blink)
    {
        drawBlink();
    }
    else
    {
        drawLoveEyes();
        drawFloatingHearts();
    }

    display.display();

    if(!blink && millis()-lastBlink > random(2500,5000))
    {
        blink = true;
        blinkStart = millis();
    }

    if(blink && millis()-blinkStart > 180)
    {
        blink = false;
        lastBlink = millis();
    }

    delay(20);
}

void LoveEyes::animateFloat()
{
    static unsigned long lastMove=0;

    if(millis()-lastMove>120)
    {
        lastMove=millis();

        floatOffset+=floatDir;

        if(floatOffset>2)
            floatDir=-1;

        if(floatOffset<-2)
            floatDir=1;
    }
}

void LoveEyes::drawHeart(int x,int y,int s)
{
    display.fillCircle(x-s/2,y-s/3,s/2,SSD1306_WHITE);

    display.fillCircle(x+s/2,y-s/3,s/2,SSD1306_WHITE);

    display.fillTriangle(
        x-s,
        y,
        x+s,
        y,
        x,
        y+s,
        SSD1306_WHITE);
}

void LoveEyes::drawLoveEyes()
{
    display.clearDisplay();

    drawHeart(leftEyeX,
              eyeY+floatOffset,
              14);

    drawHeart(rightEyeX,
              eyeY+floatOffset,
              14);

    display.fillCircle(leftEyeX-16,
                       eyeY-18+floatOffset,
                       1,
                       SSD1306_WHITE);

    display.fillCircle(rightEyeX+16,
                       eyeY-18+floatOffset,
                       1,
                       SSD1306_WHITE);

    display.fillCircle(leftEyeX-12,
                       eyeY-22+floatOffset,
                       2,
                       SSD1306_WHITE);

    display.fillCircle(rightEyeX+12,
                       eyeY-22+floatOffset,
                       2,
                       SSD1306_WHITE);
}

void LoveEyes::drawBlink()
{
    display.clearDisplay();

    int h = map(millis()-blinkStart,0,180,14,2);

    if(h<2)
        h=2;

    display.fillRoundRect(
        leftEyeX-14,
        eyeY-h/2+floatOffset,
        28,
        h,
        2,
        SSD1306_WHITE);

    display.fillRoundRect(
        rightEyeX-14,
        eyeY-h/2+floatOffset,
        28,
        h,
        2,
        SSD1306_WHITE);
}

void LoveEyes::pulseHearts()
{
    static unsigned long lastPulse=0;
    static bool big=false;

    if(millis()-lastPulse>350)
    {
        lastPulse=millis();
        big=!big;
    }

    display.clearDisplay();

    int size=big?15:13;

    drawHeart(leftEyeX,eyeY+floatOffset,size);

    drawHeart(rightEyeX,eyeY+floatOffset,size);
}

void LoveEyes::randomMovement()
{
    static unsigned long lastMove=0;

    if(millis()-lastMove>1800)
    {
        lastMove=millis();

        eyeOffsetX=random(-1,2);
    }
}

void LoveEyes::drawSmallHeart(int x,int y)
{
    display.drawPixel(x,y,SSD1306_WHITE);

    display.drawPixel(x-1,y-1,SSD1306_WHITE);

    display.drawPixel(x+1,y-1,SSD1306_WHITE);

    display.drawPixel(x,y+1,SSD1306_WHITE);
}

void LoveEyes::drawFloatingHearts()
{
    for(int i=0;i<5;i++)
    {
        drawSmallHeart(hearts[i].x,hearts[i].y);

        hearts[i].y-=hearts[i].speed;

        if(hearts[i].y<-5)
        {
            hearts[i].y=70;

            hearts[i].x=random(10,118);
        }
    }
}