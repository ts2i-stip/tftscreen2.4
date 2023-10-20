#include <RectwithText.hpp>

RectwithText::RectwithText(String ptext, int pfontSize, Measure_t ppos, Measure_t pmargins, Measure_t pdim) : text(ptext), fontSize(pfontSize), pos(ppos), dim(pdim), margins(pmargins) {
    // if (dim.x < 0) {
    //     dim.x = strlen(text.c_str());
    // }
    // if (dim.y < 0) {
    //     dim.y = 1;
    // }
    // Serial.print("Constructor dim.x = "); Serial.print(dim.x);
    // Serial.print(", dim.y = "); Serial.println(dim.y);
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
    // Serial.println("inside setText dim.x = "); Serial.print(dim.x);
    if (dim.x < 0) {
        dim.x = text.length();// strlen(text.c_str());
        // Serial.print("setText dim.x = "); Serial.print(dim.x);
    }
    if (dim.y < 0) {
        dim.y = 1;
        // Serial.print(", dim.y = "); Serial.println(dim.y);
    }
    
    
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
    tft.setTextColor(textColor);
    tft.print(text);
    tft.setTextColor(WHITE);
    // Serial.print("w = ");
    // Serial.print(w);
    // Serial.print(" h = ");
    // Serial.print(h);
    // Serial.print(" txtx = ");
    // Serial.print(pos.x + margins.x + 1);
    // Serial.print(" txty ");
    // Serial.println(pos.y + 2 + margins.y);
}

void RectwithText::clear() {
    int w = GET_WIDTH(dim.x, fontSize, margins.x);
    int h = GET_HIGH(dim.y, fontSize, margins.y);
    if (hasBorder) {
        tft.drawRect(pos.x, pos.y, w, h, BLACK);
    }
    tft.fillRect(pos.x + 1, pos.y + 1, w - 2, h - 2, BLACK);
}

int RectwithText::getFontSize() {
    return fontSize;
}
