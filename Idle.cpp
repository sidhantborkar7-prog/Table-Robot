#include "Idle.h"

// OLED object comes from the main .ino
extern Adafruit_SSD1306 display;

// ---------------- Variables ----------------

int leftEyeX = 40;
int rightEyeX = 80;

int eyeY = 18;

int eyeWidth = 25;
int eyeHeight = 30;

int targetLeftEyeX = leftEyeX;
int targetRightEyeX = rightEyeX;

int moveSpeed = 2;

int blinkState = 0;

int blinkDelay = 1000;

unsigned long lastBlinkTime = 0;
unsigned long moveTime = 0;

int expression = 0;

// ---------------- Draw Eye ----------------

void drawExpression(int eyeX, int eyeY, int eyeWidth, int eyeHeight, int exp)
{
    display.fillRoundRect(
        eyeX,
        eyeY,
        eyeWidth,
        eyeHeight,
        5,
        WHITE);

    switch (exp)
    {
        case 0:
            break;

        case 1:

            display.fillRect(
                eyeX + 5,
                eyeY + 18,
                eyeWidth - 10,
                4,
                WHITE);

            break;

        case 2:

            display.fillRect(
                eyeX + 5,
                eyeY + eyeHeight - 12,
                eyeWidth - 10,
                4,
                WHITE);

            break;

        case 3:

            display.fillRect(
                eyeX + 5,
                eyeY + 7,
                eyeWidth - 10,
                4,
                WHITE);

            break;
    }
}

// ---------------- Idle Animation ----------------

void idleFace()
{
    unsigned long currentTime = millis();

    // Blink

    if (currentTime - lastBlinkTime > blinkDelay && blinkState == 0)
    {
        blinkState = 1;
        lastBlinkTime = currentTime;
    }
    else if (currentTime - lastBlinkTime > 400 && blinkState == 1)
    {
        blinkState = 0;
        lastBlinkTime = currentTime;
    }

    // Eye Movement

    if (currentTime - moveTime > random(2000, 5000) && blinkState == 0)
    {
        int eyeMovement = random(0, 3);

        if (eyeMovement == 1)
        {
            targetLeftEyeX = 30;
            targetRightEyeX = 60;
        }
        else if (eyeMovement == 2)
        {
            targetLeftEyeX = 50;
            targetRightEyeX = 80;
        }
        else
        {
            targetLeftEyeX = 40;
            targetRightEyeX = 70;
        }

        moveTime = currentTime;
    }

    // Smooth Eye Motion

    if (leftEyeX != targetLeftEyeX)
        leftEyeX += (targetLeftEyeX - leftEyeX) / moveSpeed;

    if (rightEyeX != targetRightEyeX)
        rightEyeX += (targetRightEyeX - rightEyeX) / moveSpeed;

    display.clearDisplay();

    // Left Eye

    if (blinkState == 0)
    {
        drawExpression(
            leftEyeX,
            eyeY,
            eyeWidth,
            eyeHeight,
            expression);
    }
    else
    {
        display.fillRect(
            leftEyeX,
            eyeY + eyeHeight / 2 - 2,
            eyeWidth,
            4,
            WHITE);
    }

    // Right Eye

    if (blinkState == 0)
    {
        drawExpression(
            rightEyeX,
            eyeY,
            eyeWidth,
            eyeHeight,
            expression);
    }
    else
    {
        display.fillRect(
            rightEyeX,
            eyeY + eyeHeight / 2 - 2,
            eyeWidth,
            4,
            WHITE);
    }

    display.display();

    if (currentTime - moveTime > random(3000, 7000))
    {
        expression = random(0, 4);
        moveTime = currentTime;
    }
}