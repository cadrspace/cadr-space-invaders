static uint8_t  _pin = 0;
static int32_t _count = 0;
static float    _freq = 0;

void set_tone(int pin, float freq, long len) {
  long p = 1000000 / freq;
  _freq = freq;
  _pin = pin;
  _count = len / p;
}

void play_tone() {
  if (_count < 0) {
    return;
  }
  tone(_pin, _freq, 10);
  _count -= 1;
}
