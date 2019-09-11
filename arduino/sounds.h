#ifndef __SOUNDS_H__
#define __SOUNDS_H__

const float a3 = 220.00;
const float c4 = 261.63;
const float d4 = 293.66;
const float e4 = 329.63;
const float f4 = 349.23;
const float g4 = 392.00;
const float a4 = 440.00;
const float b4 = 493.88;
const float c5 = 523.25;
const float d5 = 587.33;
const float e5 = 659.26;
const float f5 = 698.46;
const float g5 = 783.99;
const float a5 = 880.00;
const float b5 = 987.77;

extern void set_tone(int pin, float freq, long len);
extern void play_tone();

#endif /* ifndef __SOUNDS_H__ */
