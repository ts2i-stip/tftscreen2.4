// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.

// Modified for SPFD5408 Library by Joao Lopes
// Version 0.9.2 - Rotation for Mega and screen initial

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

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).



Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;


// Copy string from flash to serial port
// Source string MUST be inside a PSTR() declaration!
void progmemPrint(const char *str) {
  char c;
  while(c = pgm_read_byte(str++)) Serial.print(c);
}

// Same as above, with trailing newline
void progmemPrintln(const char *str) {
  progmemPrint(str);
  Serial.println();
}

void setup(void){
  Serial.begin(115200);
  progmemPrintln(PSTR("TFT LCD test"));

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
  RectwithText t1val("00.0", 4, {50, 0, 1, 5}, {0, 0, 0, 0});
  RectwithText t2val("00.0", 4, {50, tft.width() - t1val.getWidth(), 1, 5}, {0, 0, 0, 0});
  ts2i.draw();
  stib.draw();
  t1.draw();
  t2.draw();
  t1val.draw();
  t2val.draw();
  delay(1000);
  t1val.setText(String(11.1, 1));
  t1val.draw();
  //stib.draw();
  
  //tft.println("TS2I");
}

void loop(void){

}