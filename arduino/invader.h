#ifndef __INVADER_H__
#define __INVADER_H__

const byte INVADER_W = 14;
const byte INVADER_H = 9;

enum {
  MOVING,
  ATTACKING,
  NPC_HIDDEN,
  DEFEATED
};

extern unsigned int invader[];
extern unsigned int invader_original[];

extern void refresh_invader();

#endif /* ifndef __INVADER_H__ */
