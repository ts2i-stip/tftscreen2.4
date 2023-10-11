#pragma once

#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
//#include <string.h>

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

extern Adafruit_TFTLCD tft;

typedef struct Borders{
  int top = 0;
  int left = 0;
  int bottom = 0;
  int right = 0;
public:
  Borders(int t = 0, int l = 0, int b = 0, int r = 0)
    :top(t), left(l), bottom(b), right(r)
    {}
}Borders_t;

class RectwithText
{
private:
  int fontSize = 1;
  String text = "1";
  uint16_t textColor = WHITE;
  Borders_t borders;
  Borders_t margins;
  uint16_t backgroudColor = RED;
  uint16_t borderColor = GREEN;
  bool hasBorder = false;
public:
  RectwithText(String ptext = "1",
    int pfontSize = 1,
    Borders_t pborders = {30, 30, 1, 1},
    Borders_t pmargins = {0, 0, -1, -1}
  );
  int getWidth();
  int getHeight();
  void setTextColor(uint16_t color);
  void setBackgroudColor(uint16_t color);
  void setBorderColor(uint16_t color);
  void setText(String t);
  void draw();
  void clear();
};
