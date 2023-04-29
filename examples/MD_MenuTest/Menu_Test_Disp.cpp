#include "Menu_Test.h"

#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define OLED_WIDTH    128   // OLED display width, in pixels
#define OLED_HEIGHT   32    // OLED display height, in pixels
#define OLED_I2C_ADDR 0x3C  // OLED I2C Address
#define OLED_RESET    -1    // OLED Reset pin # (-1 if not used or sharing Arduino reset pin)

#define FONT_SCALE 1    // OLED font scaling
#define LINE_HEIGHT (8 * FONT_SCALE)  // Height of text line
#define LINE1_X  0      // Line 1 coordinates for start of text
#define LINE1_Y  0
#define LINE2_X  0      // Line 2 coordinates for start of text
#define LINE2_Y  (LINE_HEIGHT + 2)

// Declaration a SSD1306 display connected using I2C
static Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

bool display(MD_Menu::userDisplayAction_t action, char *msg)
{
  bool success = true;

  switch (action)
  {
  case MD_Menu::DISP_INIT:
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    success = oled.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR);

    oled.clearDisplay();
    oled.setTextSize(FONT_SCALE);
    oled.setTextColor(SSD1306_WHITE);
    oled.setRotation(2);
    oled.display();
    break;

  case MD_Menu::DISP_CLEAR:
    oled.clearDisplay();
    oled.display();
    break;

  case MD_Menu::DISP_L0:
    // clear the line, put cursor in position and print the message
    oled.fillRect(LINE1_X, LINE1_Y, OLED_WIDTH-LINE1_X, LINE_HEIGHT, SSD1306_BLACK);
    oled.setCursor(LINE1_X, LINE1_Y);
    oled.print(msg);
    oled.display();
    break;

  case MD_Menu::DISP_L1:
    // clear the line, put cursor in position and print the message
    oled.fillRect(LINE2_X, LINE2_Y, OLED_WIDTH - LINE2_X, LINE_HEIGHT, SSD1306_BLACK);
    oled.setCursor(LINE2_X, LINE2_Y);
    oled.print(msg);
    oled.display();
    break;
  }

  return success;
}