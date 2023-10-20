#pragma once

#include <SPFD5408_Adafruit_GFX.h>     // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h>  // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
// #include <string.h>

// Assign human-readable names to some common 16-bit color values:
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

#define CARACTER_WIDTH 5
#define CARACTER_HIGH 7

#define GET_WIDTH(x, sz, mgl) x * sz * (CARACTER_WIDTH + 1) + 2 + 2 * mgl
#define GET_HIGH(y, sz, mgl) y * sz* CARACTER_HIGH + sz + 1 + 2 * mgl

extern Adafruit_TFTLCD tft;

typedef struct Measure {
    int x;
    int y;

   public:
    Measure(int xx = 0, int yy = 0) : x(xx), y(yy) {}
} Measure_t;
typedef struct Borders {
    int top = 0;
    int left = 0;
    int bottom = 0;
    int right = 0;

   public:
    Borders(int t = 0, int l = 0, int b = 0, int r = 0)
        : top(t), left(l), bottom(b), right(r) {}
} Borders_t;

class RectwithText {
   private:
    String text;
    int fontSize;
    Measure_t pos;
    Measure_t dim;
    Measure_t margins;
    uint16_t backgroudColor = RED;
    uint16_t borderColor = GREEN;
    uint16_t textColor = WHITE;
    bool hasBorder = false;

   public:
    RectwithText(String ptext = "1",
                 int pfontSize = 1,
                 Measure_t ppos = {0, 0},
                 Measure_t pmargins = {1, 1},
                 Measure_t pdim = {-1, -1});
    void setPos(Measure_t pPos);
    void setDim(Measure_t pDim);
    void setMargins(Measure_t pMargins);
    Measure_t getPos();
    Measure_t getDim();
    Measure_t getMargins();
    int getFontSize();
    void setTextColor(uint16_t color);
    void setBackgroudColor(uint16_t color);
    void setBorderColor(uint16_t color);
    void setText(String t);
    void setFontSize(int size);
    void draw();
    void clear();
};
