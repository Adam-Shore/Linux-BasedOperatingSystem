#ifndef SCHEDULING_H
#define SCHEDULING_H
#include "lib.h"
#include "x86_desc.h"
#include "systemcall.h"

#define SCREEN_SIZE  2000
#define REAL_VID_MEM 0xB8000
#define TERM_1_MEM   0xB9000
#define TERM_2_MEM   0xBA000
#define TERM_3_MEM   0xBB000

typedef struct terminal_t{
    uint16_t cursor_y;
    uint16_t cursor_x;
    uint8_t screen_text[SCREEN_SIZE];
    uint32_t running;
    uint8_t terminal_buf[128];
    int latest_pid;

} __attribute__ ((packed)) terminal_t;


extern int cur_terminal;
extern terminal_t terminals[3];


extern void switch_terminal(int terminal_from, int terminal_to);

extern void schedule();

#endif
