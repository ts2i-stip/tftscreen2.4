#include <RectwithText.hpp>

#define CARARCTER_WIDTH 5
#define CARACTER_HIGH 7

#define GET_WIDTH(x, sz, mgl) x* sz* CARARCTER_WIDTH + x - 1 + 4 + 2 * mgl
#define GET_HIGH(y, sz, mgl) y* sz* CARARCTER_WIDTH + y - 1 + 4 + 2 * mgl

RectwithText::RectwithText(String ptext, int pfontSize, Measure_t ppos, Measure_t pdim, Measure_t pmargins) : text(ptext), fontSize(pfontSize), pos(ppos), dim(pdim), margins(pmargins) {
    if (dim.x < 0) {
        dim.x = text.length();
    }
    if (dim.y < 0) {
        dim.y = 1;
    }
}

void RectwithText::setPos(Measure_t pPos) {
    pos = pPos;
}

void RectwithText::setDim(Measure_t pDim) {
    dim = pDim;
}

void RectwithText::setMargins(Measure_t pMargins) {
    margins = pMargins;
}

Measure_t RectwithText::getPos() {
    return pos;
}

Measure_t RectwithText::getDim() {
    return dim;
}

Measure_t RectwithText::getMargins() {
    return margins;
}

void RectwithText::setTextColor(uint16_t color) {
    textColor = color;
}

void RectwithText::setBackgroudColor(uint16_t color) {
    backgroudColor = color;
}

void RectwithText::setBorderColor(uint16_t color) {
    borderColor = color;
}

void RectwithText::setText(String t) {
    text = t;
}

void RectwithText::setFontSize(int size) {
    fontSize = size;
}

void RectwithText::draw() {
    int w = GET_WIDTH(dim.x, fontSize, margins.x);
    int h = GET_HIGH(dim.y, fontSize, margins.y);
    if (hasBorder) {
        tft.drawRect(pos.x, pos.y, w, h, borderColor);
    }
    tft.fillRect(pos.x + 1, pos.y + 1, w - 2, h - 2, backgroudColor);
    tft.setTextSize(fontSize);
    tft.setCursor(pos.x + margins.x + 1,
                  pos.y + 1 + margins.y);
    tft.print(text);
    Serial.print("w = ");
    Serial.print(w);
    Serial.print(" h = ");
    Serial.print(h);
    Serial.print(" txtx = ");
    Serial.print(pos.x + margins.x + 1);
    Serial.print(" txty ");
    Serial.println(pos.y + 2 + margins.y);
}

void RectwithText::clear() {
    int w = GET_WIDTH(dim.x, fontSize, margins.x);
    int h = GET_HIGH(dim.y, fontSize, margins.y);
    if (hasBorder) {
        tft.drawRect(pos.x, pos.y, w, h, BLACK);
    }
    tft.fillRect(pos.x + 1, pos.y + 1, w - 2, h - 2, BLACK);
}
