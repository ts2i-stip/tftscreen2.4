#include <Arduino.h>
#include <RotaryEncoder.h>

// Encoder inputs
#define ENC1_A  19
#define ENC1_B  18
#define ENC2_A  20
#define ENC2_B  21
// Start buttons
#define START_BTN1   22
#define START_BTN2   23
// Cylinder inputs / outputs
#define CYL_CNT_WIDTH           26
#define SEN_CYL_CNT_WIDTH       27
#define TIME_CYL_CNT_WIDTH      5000
#define CYL_CNT_HOLE            28
#define SEN_CYL_CNT_HOLE        29
#define CYL_CNT_OPENING         30
#define SEN_CYL_CNT_OPENING     31
#define CYL_CNT_MARK            32
#define SEN_CYL_CNT_MARK        33
#define CYL_OPEN_EXIT           34


// encoder1 to check the width of the piece
// encoder2 to check the depth
RotaryEncoder *encoder1 = nullptr;
RotaryEncoder *encoder2 = nullptr;

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
  Serial.println("Start checking ...");

  // Setup start buttons
  pinMode(START_BTN1, INPUT_PULLUP);
  pinMode(START_BTN2, INPUT_PULLUP);

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

  Serial.println("Waiting for the start ...");
} // setup()

int checkCycleStatus(){
  return digitalRead(START_BTN1) + digitalRead(START_BTN2);
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
        delay(5);
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
      if(pos2 > 4000 && pos2 < 4500){ // Hole OK
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
  /*Serial.print(digitalRead(START_BTN1));
  Serial.print(" and B2 = ");
  Serial.println(digitalRead(START_BTN2));
  delay(100);*/
  /*
  static int pos1 = 0;
  static int pos2 = 0;

  encoder1->tick(); // just call tick() to check the state.
  encoder2->tick(); // just call tick() to check the state.

  // Monitor encoder1
  int newPos1 = encoder1->getPosition();
  if (pos1 != newPos1) {
    Serial.print("pos:");
    Serial.print(newPos1);
    Serial.print(" dir:");
    Serial.println((int)(encoder1->getDirection()));
    pos1 = newPos1;
  } // if
  // Monitor encoder2
  int newPos2 = encoder2->getPosition();
  if (pos2 != newPos2) {
    Serial.print("pos:");
    Serial.print(newPos2);
    Serial.print(" dir:");
    Serial.println((int)(encoder2->getDirection()));
    pos2 = newPos2;
  } // if
  */
} // loop ()