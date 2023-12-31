#include <Arduino.h>
#include <RotaryEncoder.h>
// *** SPFD5408 change -- Begin
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
// *** SPFD5408 change -- End
#include <RectwithText.hpp>
#include <string.h>

// Encoder inputs
#define ENC1_A  19
#define ENC1_B  18
#define ENC2_A  20
#define ENC2_B  21
// Start buttons
#define START_BTNL   32
#define START_BTNR   34
// Cylinder inputs / outputs
#define CYL_CNT_WIDTH           33
#define SEN_CYL_CNT_WIDTH       30
#define TIME_CYL_CNT_WIDTH      5000
#define CYL_CNT_HOLE            37
#define SEN_CYL_CNT_HOLE        36  // CHECK
#define CYL_CNT_OPENING         31
#define SEN_CYL_CNT_OPENING     28
#define CYL_CNT_MARK            35
#define SEN_CYL_CNT_MARK        26
#define CYL_OPEN_EXIT           27
// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin


// encoder1 to check the width of the piece
// encoder2 to check the depth
RotaryEncoder *encoder1 = nullptr;
RotaryEncoder *encoder2 = nullptr;
// tft declaration
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

bool isOnCycle = false;
bool isPieceOk = true;
// This interrupt routine will be called on any change of one of the input signals
// of the encoder1
void checkPosition1()
{
  encoder1->tick(); // just call tick() to check the state.
}
// This interrupt routine will be called on any change of one of the input signals
// of the encoder2
void checkPosition2()
{
  encoder2->tick(); // just call tick() to check the state.
}

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println(F("TFT LCD test"));

  // Setup start buttons
  pinMode(START_BTNL, INPUT_PULLUP);
  pinMode(START_BTNR, INPUT_PULLUP);

  // Setup cylindes
  pinMode(CYL_CNT_HOLE, OUTPUT);
  pinMode(CYL_CNT_MARK, OUTPUT);
  pinMode(CYL_CNT_OPENING, OUTPUT);
  pinMode(CYL_CNT_WIDTH, OUTPUT);
  pinMode(CYL_OPEN_EXIT,OUTPUT);
  pinMode(SEN_CYL_CNT_HOLE, INPUT);
  pinMode(SEN_CYL_CNT_MARK, INPUT);
  pinMode(SEN_CYL_CNT_OPENING, INPUT);
  pinMode(SEN_CYL_CNT_WIDTH, INPUT);
  tft.reset();
  tft.begin(0x9341); // SDFP5408

  tft.setRotation(3);
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);

  // Setup encoders
  encoder1 = new RotaryEncoder(ENC1_A, ENC1_B, RotaryEncoder::LatchMode::TWO03);
  encoder2 = encoder1;
  //encoder2 = new RotaryEncoder(ENC2_A, ENC2_B, RotaryEncoder::LatchMode::TWO03);
  // register interrupt routine for encoder1
  attachInterrupt(digitalPinToInterrupt(ENC1_A), checkPosition1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC1_B), checkPosition1, CHANGE);
  
  // register interrupt routine for encoder1
  attachInterrupt(digitalPinToInterrupt(ENC2_A), checkPosition2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC2_B), checkPosition2, CHANGE);

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

  Serial.println("Waiting for the start ...");
} // setup()

int checkCycleStatus(){
  return digitalRead(START_BTNL) + digitalRead(START_BTNR);
}

// Read the current position of the encoder and print out when changed.
void loop()
{
  if(!isOnCycle && checkCycleStatus()==0){
    Serial.println(F("Cycle start"));
    // Width control /1
    encoder1->tick(); // just call tick() to check the state.
    digitalWrite(CYL_CNT_WIDTH, HIGH);
    Serial.println(F("Cyl width out"));
    while(encoder1->getPosition() < 3500){
      Serial.print("pos:");
      Serial.print(encoder1->getPosition());
      Serial.print(" dir:");
      Serial.println((int)(encoder1->getDirection()));
    }
    static int pos1 = 0;
    int newPos1;
    while (true){
      delay(5);
      newPos1 = encoder1->getPosition();
      Serial.print("pos:");
      Serial.print(newPos1);
      Serial.print(" dir:");
      Serial.println((int)(encoder1->getDirection()));
      if (pos1 != newPos1){
        pos1 = newPos1;
      }else{
        break;
      }
    }
    Serial.print("Last pos :");
    Serial.println((int)(pos1));
    if(pos1 < 4500){    // Width OK
      // Hole control /1
      Serial.println(F("Cylinder Hole out"));
      encoder2->tick(); // just call tick() to check the state.
      encoder2->setPosition(0);
      digitalWrite(CYL_CNT_HOLE, HIGH);
      Serial.println(F("Cyl HOLE out"));
      while(encoder2->getPosition() < 3500){
        Serial.print("pos:");
        Serial.print(encoder2->getPosition());
        Serial.print(" dir:");
        Serial.println((int)(encoder2->getDirection()));
      }
      static int pos2 = 0;
      int newPos2;
      while (true){
        delay(10);
        newPos2 = encoder2->getPosition();
        Serial.print("pos:");
        Serial.print(newPos2);
        Serial.print(" dir:");
        Serial.println((int)(encoder2->getDirection()));
        if (pos2 != newPos2){
          pos2 = newPos2;
        }else{
          break;
        }
      }
      Serial.print("Last pos cylinder hole :");
      Serial.println((int)(pos2));
      if(pos2 > 3500 && pos2 < 4500){ // Hole OK
        // Mark /1
        Serial.println(F("Cylinder mark out"));
        digitalWrite(CYL_CNT_MARK, HIGH);
        delay(1500);
        // Mark /0
        Serial.println(F("Cylinder mark in"));
        digitalWrite(CYL_CNT_MARK, LOW);
        while (digitalRead(SEN_CYL_CNT_MARK) != HIGH);
        // Hole control /0
        Serial.println(F("Cylinder hole in"));
        digitalWrite(CYL_CNT_HOLE, LOW);
        while (digitalRead(SEN_CYL_CNT_HOLE) != HIGH);
        // Width control /0
        Serial.println(F("Cylinder width in"));
        digitalWrite(CYL_CNT_WIDTH, LOW);
        while (digitalRead(SEN_CYL_CNT_WIDTH) != HIGH);
        // Switch to good pieces
        Serial.println(F("Cylinder switch out"));
        digitalWrite(CYL_OPEN_EXIT, HIGH);
        delay(1500);
        // Opening
        Serial.println(F("Cylinder opening out"));
        digitalWrite(CYL_CNT_OPENING, HIGH);
        while (digitalRead(SEN_CYL_CNT_OPENING) != HIGH);
        Serial.println(F("Cylinder opening in"));
        digitalWrite(CYL_CNT_OPENING, LOW);
        Serial.println(F("Cylinder switch in"));
        digitalWrite(CYL_OPEN_EXIT, LOW);
      }else{
        isPieceOk = false;
      }
    }else{
      isPieceOk = false;
    }
    if(!isPieceOk){
      Serial.println(F("Bad piece"));
      Serial.println(F("Cylinder width in"));
      digitalWrite(CYL_CNT_WIDTH, LOW);
      while (digitalRead(SEN_CYL_CNT_WIDTH) != HIGH);
      Serial.println(F("Cylinder opening out"));
      digitalWrite(CYL_CNT_OPENING, HIGH);
      delay(1500);
      Serial.println(F("Cylinder opening in"));
      digitalWrite(CYL_CNT_OPENING, LOW);
    }
    isOnCycle = false;
    while(checkCycleStatus() != 2);
  }
} // loop ()