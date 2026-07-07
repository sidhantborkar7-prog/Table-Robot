#include "Sleep.h"

// OLED object from main .ino
extern Adafruit_SSD1306 display;

//-----------------------------
// Animation Variables
//-----------------------------
int breatheOffset = 0;
bool inhale = true;

int twitchOffset = 0;
bool twitching = false;

int zY = 0;

unsigned long lastBreath = 0;
unsigned long lastZ = 0;
unsigned long lastTwitch = 0;
unsigned long twitchDuration = 0;

//-----------------------------
// Draw Sleeping Eye
//-----------------------------
void drawEye(int x, int y)
{
    display.drawLine(x, y + 10, x + 5, y + 13, WHITE);
    display.drawLine(x + 5, y + 13, x + 10, y + 14, WHITE);
    display.drawLine(x + 10, y + 14, x + 15, y + 13, WHITE);
    display.drawLine(x + 15, y + 13, x + 20, y + 10, WHITE);

    display.drawLine(x, y + 11, x + 5, y + 14, WHITE);
    display.drawLine(x + 5, y + 14, x + 10, y + 15, WHITE);
    display.drawLine(x + 10, y + 15, x + 15, y + 14, WHITE);
    display.drawLine(x + 15, y + 14, x + 20, y + 11, WHITE);
}

//-----------------------------
// Sleep Animation
//-----------------------------
void sleepFace()
{
    unsigned long now = millis();

    //-----------------------------
    // Breathing
    //-----------------------------
    if (now - lastBreath > 450)
    {
        if (inhale)
            breatheOffset++;
        else
            breatheOffset--;

        if (breatheOffset >= 2)
            inhale = false;

        if (breatheOffset <= 0)
            inhale = true;

        lastBreath = now;
    }

    //-----------------------------
    // Floating Z
    //-----------------------------
    if (now - lastZ > 220)
    {
        zY++;

        if (zY > 18)
            zY = 0;

        lastZ = now;
    }

    //-----------------------------
    // Random Twitch
    //-----------------------------
    if (!twitching && now - lastTwitch > random(7000,12000))
    {
        twitching = true;
        twitchOffset = 1;
        twitchDuration = now;
    }

    if (twitching)
    {
        if (now - twitchDuration > 180)
        {
            twitchOffset = 0;
            twitching = false;
            lastTwitch = now;
        }
    }

    display.clearDisplay();

    int faceY = breatheOffset + twitchOffset;

    drawEye(34,16+faceY);
    drawEye(74,16+faceY);

    if(breatheOffset==2)
        display.drawCircle(64,46+faceY,3,WHITE);
    else
        display.drawCircle(64,46+faceY,2,WHITE);

    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(100,14-zY);
    display.print("Z");

    display.setCursor(108,8-zY);
    display.print("z");

    display.setCursor(116,2-zY);
    display.print("z");

    display.display();
}