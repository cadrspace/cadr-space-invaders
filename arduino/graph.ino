#include <stdint.h>

#include "graph.h"
#include "font5x7.h"

volatile uint8_t buferRed[10][16] = {
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};

uint8_t mask[] = {  0b01111111,  0b10111111,  0b11011111,  0b11101111,  0b11110111,  0b11111011,  0b11111101,  0b11111110};
uint8_t mask2[] = {  0b10000000,  0b01000000,  0b00100000,  0b00010000,  0b00001000,  0b00000100,  0b00000010,  0b00000001};

//// Matrix core.

void graph_init() {
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(OE, OUTPUT);
  pinMode(LAT, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(CLK, OUTPUT);//*/
  pinMode(SPK, OUTPUT);
  digitalWrite(LAT, HIGH);
}

void setAdr(uint8_t adr) { //0--15{
  digitalWrite(A, adr & (1 << 0));
  digitalWrite(B, adr & (1 << 1));
  digitalWrite(C, adr & (1 << 2));
  digitalWrite(D, adr & (1 << 3));
}

void send_buffer() {
  for (int j = 0; j < sizeY; j++) {
    digitalWrite(OE, LOW);
    setAdr(j);
    for (int i = 0; i < (sizeX / 8); i++) {
      SPI.transfer(buferRed[i][j]);
    }
    digitalWrite(LAT, LOW);
    digitalWrite(LAT, HIGH);
    digitalWrite(OE, HIGH);

    delay(1);
  }
}

//// Graphic core.

int getPixel(int x, int y) {
  int realX = (x / 8); // =
  uint8_t _exp = (x % 8); //
  if (! ((buferRed[realX][y] & (0x80 >> _exp)) == 0)) {
    return DARK;
  }
  else
    return RED;
}

void setPixel(int x, int y, uint8_t color) { //20,8
  int realX = (x / 8); // =
  int _exp = (x % 8); //

  if (y >= sizeY) {
    return;
  }
  switch (color)
  {
    case DARK:
      buferRed[realX][y] = buferRed[realX][y] | mask2[_exp];
      //buferGreen[realX][y] = buferGreen[realX][y] | mask2[_exp];
      break;
    case RED:
      buferRed[realX][y] = buferRed[realX][y] & mask[_exp];
      break;
      /*case GREEN:
        buferGreen[realX][y] = buferGreen[realX][y] & mask[_exp];
        break;
        case YELLO:
        buferRed[realX][y] = buferRed[realX][y] & mask[_exp];
        buferGreen[realX][y] = buferGreen[realX][y] & mask[_exp];
        break;*/
  }
}

void sp(int x, int y) {
  setPixel(x, y, RED);
}

void clearScreen() {
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 16; ++j) {
      buferRed[i][j] = 0xFF;
    }
  }
}

bool is_pixel_red(uint8_t x, uint8_t y) {
  return getPixel(x, y) == RED;
}

//// Geometric shapes

void drawCircle(int16_t x0, int16_t y0, int16_t r) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  sp(x0  , y0 + r);
  sp(x0  , y0 - r);
  sp(x0 + r, y0  );
  sp(x0 - r, y0  );

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    sp(x0 + x, y0 + y);
    sp(x0 - x, y0 + y);
    sp(x0 + x, y0 - y);
    sp(x0 - x, y0 - y);
    sp(x0 + y, y0 + x);
    sp(x0 - y, y0 + x);
    sp(x0 + y, y0 - x);
    sp(x0 - y, y0 - x);
  }

}
void fillRectangle(int x1, int y1, int x2, int y2, uint8_t color) {
  for (int x = x1; x <= x2; x++) {
    for (int y = y1; y <= y2; y++) {
      setPixel(x, y, color);
    }
  }
}
void drawRectangle(int x1, int y1, int x2, int y2, uint8_t color) {
  drawHLine(x1, x2, y1, color);
  drawHLine(x1, x2, y2, color);
  drawVLine(x1, y1, y2, color);
  drawVLine(x2, y1, y2, color);
}
void drawVLine(int x, int y1, int y2, uint8_t color) {
  for (int y = y1; y <= y2; y++) {
    setPixel(x, y, color);
  }
}
void drawHLine(int x1, int x2, int y, uint8_t color) {
  for (int x = x1; x <= x2; x++) {
    setPixel(x, y, color);
  }
}

//// Text output.

void printChar(uint8_t x, uint8_t y, uint8_t For_color, uint8_t Bk_color, char ch) {
  uint8_t xx = 0;
  uint8_t yy = 0;;
  ch -= 32;
  for (yy = 0; yy < 7; yy++)
  {
    for (xx = 0; xx < 5; xx++)
    {
      if (bitRead(pgm_read_byte(&font5x7[ch][yy]), 4 - xx)) // 4 == Font witdh -1
      {
        setPixel(x + xx, y + yy, For_color);
      }
      else
      {
        setPixel(x + xx, y + yy, Bk_color);
      }
    }
  }
}

void printString(uint8_t x, uint8_t y, uint8_t For_color, uint8_t Bk_color, char *p) {
  while (*p != '\0')
  {
    printChar(x, y, For_color, Bk_color, *p);
    x += 6; // 6 = font width + 1 pixel space
    p++;
  }
}

byte getPixelHString(int x, int y, char *p) {
  p = p + x / 6;
  return getPixelChar(x % 6, y, *p);
}
byte getPixelChar(uint8_t x, uint8_t y, char ch) {
  ch = ch - 32;
  if (x > 4) return 0; // 4 = font Width -1
  return bitRead(pgm_read_byte(&font5x7[ch][y]), 4 - x); // 4 = Font witdh -1
}

//// Misc.

void draw_cadr_logo(int x, int y, float s) {
  drawRectangle(x, y, x + 10 * s , y + 10 * s , RED);
  drawCircle(x + 5 * s, y + 5 * s, 2 * s);
  drawHLine(x + 5 * s + 2 * s, x + 5 * s + 2 * s + 2 * s + 1, y + 5 * s, RED);

}

//// graph.cpp ends here.
