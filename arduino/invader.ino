#include "invader.h"

unsigned int invader[INVADER_H] = {
  0b0000000000000,
  0b0001000000100,
  0b0000100001000,
  0b0001111111000,
  0b0011011101100,
  0b0111111111110,
  0b0101111111010,
  0b0101000001010,
  0b0000110110000
};
unsigned int invader_original[INVADER_H] = {
  0b0000000000000,
  0b0001000000100,
  0b0000100001000,
  0b0001111111000,
  0b0011011101100,
  0b0111111111110,
  0b0101111111010,
  0b0101000001010,
  0b0000110110000
};

void refresh_invader() {
  for (int idx = 0; idx < INVADER_H; ++idx) {
    invader[idx] = invader_original[idx];
  }
}
