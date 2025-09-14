#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Bounce2.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define I2C_ADDRESS 0x3C
#define OLED_RESET -1
#define PIXEL_COUNT 7
#define PIXEL_SIZE 10

struct Pixel
{
    float x;
    float y;
    float dx;
    float dy;
    float rotation;
};

struct Point
{
    float x;
    float y;
};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Bounce keyD2;
Bounce keyD3;
int diodeState = LOW;
float pixelSpeed = 1.0;
Pixel pixels[PIXEL_COUNT];

void animationInitialize();
void animationUpdate();
void pixelDraw(Pixel *pixel);
void pixelNormalize(Pixel *pixel);

void setup()
{
    Serial.begin(9600);

    Wire.begin();

    Serial.println("Scanning I2C devices...");

    if (!display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS))
    {
        Serial.println("SSD1306 allocation failed");
        for (;;)
            ;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(F("Polduino Ano V1 "));
    display.display();
    delay(1000);

    keyD2.attach(DD2, INPUT_PULLUP);
    keyD3.attach(DD3, INPUT_PULLUP);
    keyD2.interval(5);
    keyD3.interval(5);

    pinMode(LED_BUILTIN, OUTPUT);
    animationInitialize();
}

void loop()
{
    display.clearDisplay();
    display.setCursor(0, 0);
    keyD2.update();
    keyD3.update();
    animationUpdate();

    if (keyD2.fell())
    {
        pixelSpeed -= 1;
    }

    if (keyD3.fell())
    {
        pixelSpeed += 1;
    }

    pixelSpeed = constrain(pixelSpeed, 1, 5);

    if (keyD2.fell() && keyD3.fell())
    {
        diodeState = !diodeState;
    }

    digitalWrite(LED_BUILTIN, diodeState);
    display.display();
}

void animationInitialize()
{
    for (int i = 0; i < PIXEL_COUNT; i++)
    {
        pixels[i].x = random(0, SCREEN_WIDTH);
        pixels[i].y = random(0, SCREEN_HEIGHT);
        pixels[i].dx = random(-100, 101) / 100.0;
        pixels[i].dy = random(-100, 101) / 100.0;
        pixelNormalize(pixels + i);
        pixels[i].rotation = random(0, 360) / 180.0 * PI;
    }
}

void animationUpdate()
{
    for (int i = 0; i < PIXEL_COUNT; i++)
    {
        pixels[i].x += pixels[i].dx * pixelSpeed;
        pixels[i].y += pixels[i].dy * pixelSpeed;
        pixels[i].rotation += pixels[i].dx * pixels[i].dy;

        if (pixels[i].x < (PIXEL_SIZE / 2) || pixels[i].x >= (SCREEN_WIDTH - PIXEL_SIZE))
        {
            pixels[i].dx *= -1;
        }
        if (pixels[i].y < (PIXEL_SIZE / 2) || pixels[i].y >= (SCREEN_HEIGHT - PIXEL_SIZE))
        {
            pixels[i].dy *= -1;
        }

        pixelDraw(pixels + i);
    }
}

void pixelDraw(Pixel *pixel)
{
    Point p1, p2, p3, p4;
    p1.x = pixel->x + (PIXEL_SIZE / 2) * cos(pixel->rotation);
    p1.y = pixel->y + (PIXEL_SIZE / 2) * sin(pixel->rotation);
    p2.x = pixel->x + (PIXEL_SIZE / 2) * cos(pixel->rotation + PI / 2);
    p2.y = pixel->y + (PIXEL_SIZE / 2) * sin(pixel->rotation + PI / 2);
    p3.x = pixel->x + (PIXEL_SIZE / 2) * cos(pixel->rotation + PI);
    p3.y = pixel->y + (PIXEL_SIZE / 2) * sin(pixel->rotation + PI);
    p4.x = pixel->x + (PIXEL_SIZE / 2) * cos(pixel->rotation + 3 * PI / 2);
    p4.y = pixel->y + (PIXEL_SIZE / 2) * sin(pixel->rotation + 3 * PI / 2);

    display.drawLine(p1.x, p1.y, p2.x, p2.y, SSD1306_WHITE);
    display.drawLine(p2.x, p2.y, p3.x, p3.y, SSD1306_WHITE);
    display.drawLine(p3.x, p3.y, p4.x, p4.y, SSD1306_WHITE);
    display.drawLine(p4.x, p4.y, p1.x, p1.y, SSD1306_WHITE);
}

void pixelNormalize(Pixel *pixel)
{
    double length = sqrt(pixel->dx * pixel->dx + pixel->dy * pixel->dy);
    if (length > 0)
    {
        pixel->dx /= length;
        pixel->dy /= length;
    }
}
