#include <RectwithText.hpp>

RectwithText::RectwithText(String ptext,
                           int pfontSize,
                           Borders_t pborders,
                           Borders_t pmargins) :
                           fontSize(pfontSize), 
                           text(ptext), 
                           borders(pborders),
                           margins(pmargins){
  //draw();
}

int RectwithText::getWidth()
{
  int widthByPixel = 5;
  return borders.right * fontSize * widthByPixel + borders.right - 1 + 4 + 2 * margins.left;
}

int RectwithText::getHeight()
{
  int heightByPixel = 7;
  return borders.bottom * fontSize * heightByPixel + borders.bottom - 1 + 4 + 2 * margins.top;
}

void RectwithText::setTextColor(uint16_t color){
  textColor = color;
}

void RectwithText::setBackgroudColor(uint16_t color){
  backgroudColor = color;
}

void RectwithText::setBorderColor(uint16_t color){
  borderColor = color;
}

void RectwithText::setText(String t){
  text = t;
}

void RectwithText::draw(){
  int w = getWidth();
  int h = getHeight();
  int charWidth = fontSize * 5;
  if(hasBorder){
    tft.drawRect(borders.left, borders.top, w, h, borderColor);
  }
  tft.fillRect(borders.left+1, borders.top+1, w - 2, h - 2, backgroudColor);
  tft.setTextSize(fontSize);
  tft.setCursor(borders.left + ((w - (text.length() + 1) * charWidth) / 2),
  borders.top + 2 + margins.top);
  tft.print(text);
}

void RectwithText::clear(){
  int w = getWidth();
  int h = getHeight();
  if(hasBorder){
    tft.drawRect(borders.left, borders.top, w, h, BLACK);
  }
  tft.fillRect(borders.left+1, borders.top+1, w - 2, h - 2, BLACK);
}
