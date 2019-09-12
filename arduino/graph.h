#ifndef __GRAPH_H__
#define __GRAPH_H__

#define A 2
#define B 3
#define C 4
#define D 5
#define OE 9
#define LAT 10
#define R1 11
#define G1 12
#define CLK 13

#define SPK 8

int sizeX = 80; // !!!!> x%8==0
int sizeY = 16;

enum {
  DARK,
  RED,
  GREEN,
  YELLO
};

extern volatile uint8_t buferRed[10][16];

extern void graph_init();
extern void setAdr(uint8_t adr);
extern void send_buffer();

extern int getPixel(int x, int y);
extern void sp(int x, int y);
extern void setPixel(int x, int y, uint8_t color);
extern void clearScreen();
extern bool is_pixel_red(uint8_t x, uint8_t y);

extern void drawCircle(int16_t x0, int16_t y0, int16_t r);
extern void fillRectangle(int x1, int y1, int x2, int y2, uint8_t color);
extern void drawRectangle(int x1, int y1, int x2, int y2, uint8_t color);
extern void drawVLine(int x, int y1, int y2, uint8_t color);
extern void drawHLine(int x1, int x2, int y, uint8_t color);

extern void printChar(uint8_t x, uint8_t y, uint8_t For_color, uint8_t Bk_color, char ch);
extern void printString(uint8_t x, uint8_t y, uint8_t For_color, uint8_t Bk_color, char *p);
extern byte getPixelHString(int x, int y, char *p);
extern byte getPixelChar(uint8_t x, uint8_t y, char ch);

extern void draw_cadr_logo(int x, int y, float s);

#endif /* ifndef __GRAPH_H_ */_
