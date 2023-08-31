
#include <Arduino.h>
#include <M5Unified.h>
//#include "M5CoreS3.h"
//#include <M5Core2.h> 
M5Canvas canvas(&M5.Lcd);

void setup() {
  M5.begin();
  auto cfg = M5.config();
  M5.Lcd.fillScreen(BLACK);

  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(5);
  M5.Lcd.setCursor(0, 50);
  M5.Lcd.println("water");
  M5.Lcd.setCursor(0, 150);
  M5.Lcd.println("on");
  M5.Lcd.fillTriangle(250, 50, 208, 75, 250, 100, WHITE);

  canvas.setTextSize(2);
  canvas.createSprite(320, 30);
}

bool isWaterOn = false;

// 水まきの処理
void waterButton_cb(int touchX, int touchY)
{
  if (touchX < M5.Lcd.width() / 2 || touchY > M5.Lcd.height() / 2)
    return;  

  if (isWaterOn)
  {
    M5.update();
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(5);
    M5.Lcd.setCursor(0, 50);
    M5.Lcd.println("water");
    M5.Lcd.setCursor(0, 150);
    M5.Lcd.println("off");
    M5.Lcd.fillTriangle(250, 50, 208, 75, 250, 100, WHITE);
    canvas.setTextSize(2);
    canvas.createSprite(320, 30);
    isWaterOn = false;
    M5.Display.endWrite();
    delay(500);
  }
  else if(!isWaterOn)
  {
    M5.update();
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(5);
    M5.Lcd.setCursor(0, 50);
    M5.Lcd.println("water");
    M5.Lcd.setCursor(0, 150);
    M5.Lcd.println("on");
    M5.Lcd.fillTriangle(250, 50, 208, 75, 250, 100, WHITE);
    canvas.setTextSize(2);
    canvas.createSprite(320, 30);
    isWaterOn = true;
    M5.Display.endWrite();
    delay(500);
  }
}

int touchX = 0;
int touchY = 0;
bool touchP = false;

void loop() {
  M5.update();
  canvas.fillSprite(BLACK);
  M5.Display.startWrite();

  if (M5.Touch.isEnabled()) {
    auto t = M5.Touch.getDetail();
    touchX = t.x;
    touchY = t.y;
    touchP = t.isPressed();
  }

  if (touchP) {
    // コールバック関数を呼び出す
    waterButton_cb(touchX, touchY);
    touchP = false;
  }

  delay(100);
}


