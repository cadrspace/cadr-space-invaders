#include <SPI.h>
#include "font5x7.h"
#include <MsTimer2.h>
#include <limits.h>

#include "config.h"
#include "invader.h"
#include "graph.h"

int sizeX = 80; // !!!!> x%8==0
int sizeY = 16;

enum {
  RUNNING,
  NPC_DEFEATED,
  GAME_OVER,
  YOU_WON
};

int npc_state = MOVING;
int game_state = RUNNING;
byte npc_hp = 42;

volatile unsigned int invader_pos[] = { 2, 0 }; // x, y
volatile char invader_dx = 1;

volatile byte bomb_pos[] = { 0, 0 };

void init_pins() {
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_FIRE, INPUT_PULLUP);
  pinMode(SPEAKER_PIN, OUTPUT);
}

void setup() {
  Serial.begin(9600);
  graph_init();
  SPI.begin();
  randomSeed(analogRead(A5));
  init_pins();
  invader_pos[0] = random(1, 50);

  MsTimer2::set(2, Interrupt);
  MsTimer2::start();
  reload();
}

float sc[] = {0.6, 0.8, 1, 1.2};
int pula[] = { -1, -1};

long tm = millis();
volatile int count = 0;
int posC = 10;

void draw_invader(unsigned int x, unsigned int y) {
  for (int iy = 0; iy < INVADER_H; ++iy) {
    for (int ix = 0; ix < INVADER_W; ++ix) {
      setPixel(x + ix, y + iy,
               (invader[iy] & (1 << ix)) > 0);
    }
  }
}

void reload() {
  clearScreen();
  draw_invader(invader_pos[0], invader_pos[1]);
}

void handle_buttons() {
  if (digitalRead(BUTTON_LEFT) == LOW)
  {
    drawHLine(posC, posC + 4, 14, DARK);
    posC--;
    posC = constrain(posC, 0, 70);
    drawHLine(posC, posC + 4, 14, RED);
    delay(2);
  }
  if (digitalRead(BUTTON_RIGHT) == LOW)
  {
    drawHLine(posC, posC + 4, 14, DARK);
    posC++;
    posC = constrain(posC, 0, 74);
    drawHLine(posC, posC + 4, 14, RED);
    delay(2);
  }
  if ((digitalRead(BUTTON_FIRE) == LOW)
      && ((pula[1] < 0) && (pula[0] < 0))
      && ( millis() - tm > 10))  {
    pula[0] = posC + 2;
    pula[1] = 13;
    tm = millis();
    set_tone(SPEAKER_PIN, 880, 1000);
  }
}

boolean is_invader_hit(byte px, byte py) {
  return (px >= invader_pos[0])
         && (px < (invader_pos[0] + INVADER_W))
         && (py >= invader_pos[1])
         && (py < (invader_pos[1] + INVADER_H));
}

void loop() {
  count++;
  switch (game_state) {
    case RUNNING:
      handle_buttons();

      if ((pula[1] >= 0) && (pula[0] >= 0))
      {
        if (is_pixel_red(pula[0], pula[1] - 1)
            && is_invader_hit(pula[0], pula[1])) {
          byte in_pos_x = pula[0] - invader_pos[0] - 1;
          byte in_pos_y = (pula[1] - 1) - invader_pos[1] - 1;
          invader[in_pos_y] &= ~(0x1000 >> in_pos_x);
          setPixel(pula[0], pula[1] - 1, DARK);
          setPixel(pula[0], pula[1], DARK);
          pula[0] = pula[1] = -1;
          set_tone(SPEAKER_PIN, 220, 10000);
          npc_hp--;

          if (npc_hp < 30) {
            npc_state  = DEFEATED;
            game_state = NPC_DEFEATED;
          }
        }
        else {
          setPixel(pula[0], pula[1], DARK);
          pula[1]--;
          if (pula[1] < 0)
          {
            pula[0] = pula[1] = -1;
            return;
          }
          setPixel(pula[0], pula[1], RED);
          delay(2);
        }
      }
      break;
      
    case NPC_DEFEATED:
      clearScreen();
      for (int i = 0; i < 500; ++i) {
        delay(1);
        handle_buttons();
        set_tone(SPEAKER_PIN, 220, 1000 );
        count++;
      }
      npc_state = NPC_HIDDEN;
      clearScreen();
      delay(1);
      printString(15, 4, RED , DARK, "YOU WON!");
      for (float f = 440; f < 1000; f *= 2) {
        set_tone(SPEAKER_PIN, f, 10000 / f);
        delay(10000 / f);
      }
      delay(100);
      clearScreen();
      invader_pos[0] = random(1, 50);
      invader_pos[1] = 1;
      refresh_invader();
      npc_hp = 44;
      npc_state  = MOVING;
      game_state = RUNNING;
      break;
      
    case GAME_OVER:
      npc_state = NPC_HIDDEN;
      clearScreen();
      for (float f = 1000; f > 50; f /= 1.5) {
        set_tone(SPEAKER_PIN, f, 10000 / f);
        printString(13, 4, RED , DARK, "YOU LOSE!");
        delay(10000 / f);
      }
      clearScreen();
      invader_pos[0] = random(1, 50);
      invader_pos[1] = 0;
      refresh_invader();
      npc_hp = 44;
      npc_state  = MOVING;
      game_state = RUNNING;
      break;
  }
}

unsigned int lenString(char *p) {
  unsigned int retVal = 0;
  while (*p != '\0')
  {
    retVal++;
    p++;
  }
  return retVal;
}

//--------------------------------------------------

void update_bomb_position() {
  drawHLine(bomb_pos[0], bomb_pos[0] + 2, bomb_pos[1] - 1, DARK);
  drawHLine(bomb_pos[0], bomb_pos[0] + 2, bomb_pos[1], RED);
  Serial.println(bomb_pos[0]);
}

void clear_ground() {
  drawHLine(0, 80, 15, DARK);
}

void update_invader_position() {
  switch (npc_state) {
    case MOVING:
      if (count % 1 == 0) {
        invader_pos[0] += invader_dx;
        if (random(20) == 0)
          invader_dx = random(3) - 1;
        if (invader_pos[0] > 60)
          invader_dx = -1;
        if (invader_pos[0] < 1)
          invader_dx = 1;
        draw_invader(invader_pos[0], invader_pos[1]);
      }
      if (bomb_pos[0] > 0 && ((count % 10) == 0)) {
        if (random(4) == 0) {
          clear_ground();
        }
        bomb_pos[1]++;

        if (bomb_pos[1] > 15) {
          bomb_pos[0] = bomb_pos[1] = 0;

        } else {
          update_bomb_position();
        }
        if (((bomb_pos[0] >= posC) && (bomb_pos[0] <= posC + 4))
            && (bomb_pos[1] == 14)) {
          game_state = GAME_OVER;
          npc_state = DEFEATED;
        }
      }
      if ((bomb_pos[0] == 0) && (random(10) == 0))
        npc_state = ATTACKING;
      break;
    case ATTACKING:
      bomb_pos[0] = invader_pos[0] + 6;
      bomb_pos[1] = invader_pos[1] + 8;
      npc_state = MOVING;
      break;
    case NPC_HIDDEN:
      //draw_invader(invader_pos[0], invader_pos[1]);
      break;
    case DEFEATED:
      if (count % 10 == 0) {
        invader[random(2, INVADER_H)] &= ~(random(INT_MAX));
        invader_pos[1]++;
        if (invader_pos[1] > 9) {
          npc_state = NPC_HIDDEN;
        }
      }
      draw_invader(invader_pos[0], invader_pos[1]);
  }
}

void Interrupt() {
  update_invader_position();
  send_buffer();
  play_tone();
}
