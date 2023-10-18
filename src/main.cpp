#include <Arduino.h>
#include <Encoder.h>
// *** SPFD5408 change -- Begin
#include <SPFD5408_Adafruit_GFX.h>     // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h>  // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
// *** SPFD5408 change -- End
#include <RectwithText.hpp>
// #include <string.h>
// Définir les broches de l'encodeur
Encoder monEncodeur1(20, 21);  // Broches A et B de l'encodeur
Encoder monEncodeur2(19, 18);  // Broches A et B de l'encodeur
// Start buttons
#define boutonMarche1Pin 32
#define boutonMarche2Pin 34
// Cylinder inputs / outputs
#define CYL_CNT_WIDTH 33
#define SEN_CYL_CNT_WIDTH 30
#define CYL_CNT_HOLE 37
#define SEN_CYL_CNT_HOLE 28
#define CYL_CNT_OPENING 31
#define SEN_CYL_CNT_OPENING 36
#define CYL_CNT_MARK 35
#define SEN_CYL_CNT_MARK 26
#define CYL_OPEN_EXIT 27

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3  // Chip Select goes to Analog 3
#define LCD_CD A2  // Command/Data goes to Analog 2
#define LCD_WR A1  // LCD Write goes to Analog 1
#define LCD_RD A0  // LCD Read goes to Analog 0

#define LCD_RESET A4  // Can alternately just connect to Arduino's reset pin

// tft declaration
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
RectwithText ts2i("TS2I", 2, {0, 0, 1, 5});
RectwithText stib("STIB", 2, {0, 240, 1, 5});
RectwithText t1("T1", 2, {20, 10, 1, 5});
RectwithText t2("T2", 2, {20, 235, 1, 5});
RectwithText t1val("000", 4, {60, 0, 1, 5}, {0, 0, 0, 0});
RectwithText t2val("000", 4, {60, 230, 1, 5}, {0, 0, 0, 0});
RectwithText ok("000", 4, {120, 0, 1, 5}, {0, 0, 0, 0});
RectwithText nok("000", 4, {120, 230, 1, 5}, {0, 0, 0, 0});
int totalCounter1 = 0;
int totalCounter2 = 0;
int nbBonnes = 0;
int nbMauvaises = 0;

// Valeurs minimales et maximales pour la course (à personnaliser)
long valeurMin1 = 1397;
long valeurMax1 = 1420;
long valeurMin2 = 640;
long valeurMax2 = 2000;

int stepCycle = 0;
bool is2ButtonsPressed() {
    return digitalRead(boutonMarche1Pin) + digitalRead(boutonMarche1Pin) == 0;
}

void setup() {
    Serial.begin(9600);
    Serial.println(F("TFT LCD test"));
    pinMode(boutonMarche1Pin, INPUT_PULLUP);
    pinMode(boutonMarche2Pin, INPUT_PULLUP);
    pinMode(SEN_CYL_CNT_WIDTH, INPUT_PULLUP);
    pinMode(SEN_CYL_CNT_HOLE, INPUT_PULLUP);
    pinMode(SEN_CYL_CNT_OPENING, INPUT_PULLUP);
    pinMode(SEN_CYL_CNT_MARK, INPUT_PULLUP);
    pinMode(CYL_CNT_WIDTH, OUTPUT);
    pinMode(CYL_CNT_HOLE, OUTPUT);
    pinMode(CYL_CNT_OPENING, OUTPUT);
    pinMode(CYL_CNT_MARK, OUTPUT);
    pinMode(CYL_OPEN_EXIT, OUTPUT);

    tft.reset();
    tft.begin(0x9341);  // SDFP5408

    tft.setRotation(1);
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);

    ts2i.draw();
    stib.draw();
    t1.draw();
    t2.draw();
    t1val.draw();
    t2val.draw();
    ok.setBackgroudColor(GREEN);
    ok.draw();
    nok.draw();
}

void loop() {
    if (!is2ButtonsPressed())
        return;
    int position1;
    int position2;
    char mystr[7];
    if (stepCycle == 0) {
        stepCycle++;
        Serial.println("Départ cycle, sortie vérin épaisseur");
        digitalWrite(CYL_CNT_WIDTH, HIGH);  // Active le vérin
        while (digitalRead(SEN_CYL_CNT_WIDTH) == HIGH)
            ;
        delay(3000);
        Serial.println("---->fin sortie vérin épaisseur");
        position1 = monEncodeur1.read();  // Lire la position de l'encodeur
        // Serial.print("Position de l'encodeur1 : ");
        // Serial.println(position1);

        sprintf(mystr, "%03d", position1);
        // Serial.println(mystr);
        // Serial.println(String(position1, 0));
        t1val.setText(mystr);
        t1val.draw();
        delay(1000);
    }
    position1 = 1400;
    if (position1 >= valeurMin1 && position1 <= valeurMax1) {
        Serial.println("TEST 1 OK, sortie vérin ouverture");
        if (!is2ButtonsPressed())
            return;
        if (stepCycle == 1) {
            stepCycle++;
            digitalWrite(CYL_CNT_HOLE, HIGH);  // Active le vérin
            while (digitalRead(SEN_CYL_CNT_HOLE) == HIGH)
                ;
            delay(3000);  // Attend 2 secondes
            Serial.println("----> Fin sortie vérin ouverture");
            position2 = monEncodeur2.read();  // Lire la position de l'encodeur
            // Serial.print("Position de l'encodeur2 : ");
            // Serial.println(position2);
            sprintf(mystr, "%d", position2);
            // Serial.println(String(position1, 0));
            t2val.setText(mystr);
            t2val.draw();
            delay(200);
            position2 = 1000;
        }
        if (position2 >= valeurMin2 && position2 <= valeurMax2) {
            Serial.println("TEST 2 OK, sortie vérin marquage");
            // Ajoutez ici les actions pour le cas "Pièce bonne"

            if (!is2ButtonsPressed())
                return;
            if (stepCycle == 2) {
                stepCycle++;
                digitalWrite(CYL_CNT_MARK, HIGH);  // Active le vérin
                while (digitalRead(SEN_CYL_CNT_MARK) == HIGH)
                    ;
                delay(500);
            }
            Serial.println("Fin sortie vérin marquage");
            if (!is2ButtonsPressed())
                return;
            if (stepCycle == 3) {
                stepCycle++;
                digitalWrite(CYL_CNT_MARK, LOW);  // Active le vérin
                while (digitalRead(SEN_CYL_CNT_MARK) == HIGH) {
                    // Attendez
                }
                // Attend 2 secondes
                Serial.println(F("Basculer l'exit sur pieces bonnes"));
                digitalWrite(CYL_OPEN_EXIT, HIGH);  // Active le vérin
            }
            Serial.println(F("---->Retour vérin ouverture"));
            if (!is2ButtonsPressed())
                return;
            if (stepCycle == 4) {
                stepCycle++;
                digitalWrite(CYL_CNT_HOLE, LOW);  // Active le vérin
                while (digitalRead(SEN_CYL_CNT_HOLE) == HIGH) {
                    // Attendez
                }
            }
            Serial.println(F("---->Retour vérin épaisseur"));
            if (!is2ButtonsPressed())
                return;
            if (stepCycle == 5) {
                stepCycle++;
                digitalWrite(CYL_CNT_WIDTH, LOW);  // Active le vérin
                while (digitalRead(SEN_CYL_CNT_WIDTH) == HIGH) {
                    // Attendez
                }
            }
            // Attend 2 secondes
            Serial.println(F("Ouverture porte"));
            if (!is2ButtonsPressed())
                return;
            if (stepCycle == 6) {
                stepCycle++;
                digitalWrite(CYL_CNT_OPENING, HIGH);  // Active le vérin
                while (digitalRead(SEN_CYL_CNT_OPENING) == HIGH) {
                    // Attendez
                }
                delay(100);
            }
            // Attend 2 secondes
            Serial.println(F("----> Fermeture porte"));
            if (!is2ButtonsPressed())
                return;
            if (stepCycle == 7) {
                stepCycle++;
                digitalWrite(CYL_CNT_OPENING, LOW);  // Active le vérin
                while (digitalRead(SEN_CYL_CNT_OPENING) == HIGH) {
                    // Attendez
                }
                Serial.println(F("----> Basculer l'exit sur pieces mauvaises"));
                digitalWrite(CYL_OPEN_EXIT, LOW);  // Active le vérin
                delay(800);
                nbBonnes++;
                char mystr[7];
                sprintf(mystr, "%03d", nbBonnes);
                ok.setText(mystr);
                ok.draw();
            }
        } else {
            Serial.println("TEST 2 NOK");
            // Ajoutez ici les actions pour le cas "Pièce mauvaise"
            if (!is2ButtonsPressed())
                return;
            if (stepCycle == 2) {
                stepCycle++;
                digitalWrite(CYL_CNT_HOLE, LOW);  // Active le vérin
                while (digitalRead(SEN_CYL_CNT_HOLE) == HIGH) {
                    // Attendez
                }
            }
            if (!is2ButtonsPressed())
                return;
            if (stepCycle == 3) {
                stepCycle++;
                digitalWrite(CYL_CNT_WIDTH, LOW);  // Active le vérin
                while (digitalRead(SEN_CYL_CNT_WIDTH) == HIGH) {
                    // Attendez
                }
            }
            if (!is2ButtonsPressed())
                return;
            if (stepCycle == 4) {
                stepCycle++;
                digitalWrite(CYL_CNT_OPENING, HIGH);  // Active le vérin
                while (digitalRead(SEN_CYL_CNT_OPENING) == HIGH) {
                    // Attendez
                }
            }
            if (!is2ButtonsPressed())
                return;
            if (stepCycle == 5) {
                stepCycle++;
                digitalWrite(CYL_CNT_OPENING, LOW);  // Active le vérin
                while (digitalRead(SEN_CYL_CNT_OPENING) == LOW) {
                    // Attendez
                }  // Attend 2 secondes
                nbMauvaises++;
                char mystr[7];
                sprintf(mystr, "%03d", nbMauvaises);
                nok.setText(mystr);
                nok.draw();
            }
        }
    } else {
        Serial.println("TEST 1 NOK");
        // Ajoutez ici les actions pour le cas "Pièce mauvaise"
        if (!is2ButtonsPressed())
            return;
        if (stepCycle == 1) {
            stepCycle++;
            digitalWrite(CYL_CNT_WIDTH, LOW);  // Active le vérin
            while (digitalRead(SEN_CYL_CNT_WIDTH) == HIGH) {
                // Attendez
            }
        }
        if (!is2ButtonsPressed())
            return;
        if (stepCycle == 2) {
            stepCycle++;
            digitalWrite(CYL_CNT_OPENING, HIGH);  // Active le vérin
            while (digitalRead(SEN_CYL_CNT_OPENING) == HIGH) {
                // Attendez
            }
        }
        if (!is2ButtonsPressed())
            return;
        if (stepCycle == 3) {
            stepCycle++;
            digitalWrite(CYL_CNT_OPENING, LOW);  // Active le vérin
            while (digitalRead(SEN_CYL_CNT_OPENING) == LOW) {
                // Attendez
            }
            nbMauvaises++;
            char mystr[7];
            sprintf(mystr, "%03d", nbMauvaises);
            nok.setText(mystr);
            nok.draw();
        }
    }
    delay(100);  // Rafraîchir toutes les 100 ms (ajustez selon vos besoins)
}