#ifndef IDT_H
#define IDT_H

#include "x86_desc.h"
#include "exceptionASM.h"
// #include "exceptionProblems.h"
#define NUM_VEC     256
#define RTC_NUM     40
#define KEYBOARD_NUM     33
#define SYSTEM_CALL_NUM 0x80
#define PIT_NUM         32


void init_idt();


#endif
