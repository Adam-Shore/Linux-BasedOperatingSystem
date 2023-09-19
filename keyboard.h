#ifndef KEYBOARD_H
#define KEYBOARD_H


#include "x86_desc.h"
#include "lib.h"

#include "rtc.h"
#include "i8259.h"

#define KEYBOARD_IRQ_NUM 1
#define keyboard_length 36
#define keyboard_port 0x60
#define CAPS_LOCK 1
#define SHIFT 2
#define L_SHIFT_P 0x2A
#define R_SHIFT_P 0x36
#define L_SHIFT_R 0xAA
#define R_SHIFT_R 0xB6
#define L_CTRL_P 0x1D
#define R_CTRL_P 0x1D //E0
#define L_CTRL_R 0x9D
#define R_CTRL_R 0x9D //E0
#define ALT_P 0x38
#define ALT_R 0xB8
#define NUM_COLS    80
#define NUM_ROWS    25
#define F1_P 0x3B
#define F2_P 0x3C
#define F3_P 0x3D


void keyboard_init();
void terminal_open(const uint8_t* filename);
void terminal_close(int32_t fd);
int terminal_read(int32_t fd, void* buf, int32_t nbytes);
void terminal_write(int32_t fd, const void* buf, int32_t nbytes);
void keyboard_handler();





#endif
