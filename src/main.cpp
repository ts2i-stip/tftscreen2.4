// *** SPFD5408 change -- Begin
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
// *** SPFD5408 change -- End
#include <RectwithText.hpp>
#include <string.h>

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// tft declaration
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup(void){
  Serial.begin(115200);
  Serial.println(F("TFT LCD test"));

  tft.reset();
  tft.begin(0x9341); // SDFP5408

  tft.setRotation(3);
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  RectwithText ts2i("TS2I", 2, {0, 0, 1, 5}, {0, 0, 0, 0});
  RectwithText stib("STIB", 2, {0, tft.width() - ts2i.getWidth(), 1, 5},
        {0, 0, 0, 0});
  RectwithText t1("T1", 2, {20, 0, 1, 5}, {0, 0, 0, 0});
  RectwithText t2("T2", 2, {20, tft.width() - ts2i.getWidth(), 1, 5}, {0, 0, 0, 0});
  RectwithText t1val("9999", 4, {50, 0, 1, 5}, {0, 0, 0, 0});
  RectwithText t2val("9999", 4, {50, tft.width() - t1val.getWidth(), 1, 5}, {0, 0, 0, 0});
  RectwithText ok("9999", 4, {90, 0, 1, 5}, {0, 0, 0, 0});
  RectwithText nok("9999", 4, {90, tft.width() - ok.getWidth(), 1, 5}, {0, 0, 0, 0});
  ts2i.draw();
  stib.draw();
  t1.draw();
  t2.draw();
  t1val.draw();
  t2val.draw();
  ok.draw();
  nok.draw();
  //t1val.setText(String(11.1, 1));
}

void loop(void){

}