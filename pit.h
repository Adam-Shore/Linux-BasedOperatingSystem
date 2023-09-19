#ifndef PIT_H
#define PIT_H

#include "exceptionASM.h"
#include "x86_desc.h"
#include "i8259.h"
#include "lib.h"

#define COMMAND_REG 0x43
#define CHANNEL_0   0x40
#define SQUARE_WAVE 0x36
#define PIT_IRQ_NUM 0

void pit_init(int hz);
void pit_handler();

int ticks;

#endif
