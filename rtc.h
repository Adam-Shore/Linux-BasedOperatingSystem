#ifndef RTC_H
#define RTC_H

#include "x86_desc.h"
#include "i8259.h"
#include "lib.h"
#include "exceptionASM.h"

#define RTC_PORT 0x70
#define RTC_DATA 0x71
#define RTC_IRQ_NUM 8
#define REGISTER_A 0x8A
#define REGISTER_B 0x8B
#define REGISTER_C 0x0C

#define RATE_BUFFER 0X0F
#define RATE_BUFFER2 0XF0
 
#define FREQ1 1024
#define FREQ2 512
#define FREQ3 256
#define FREQ4 128
#define FREQ5 64
#define FREQ6 32
#define FREQ7 16
#define FREQ8 8
#define FREQ9 4
#define FREQ10 2


extern int flag;
extern uint32_t frequency_buffer;
extern int virtual_counter;
extern int virtual_frequency;



void rtc_init();
int rtc_open(const uint8_t* filename);
int rtc_close(int32_t fd);
int rtc_read(int32_t fd, void* buf, int32_t nbytes);
int rtc_write(int32_t fd, const void* buf, int32_t nbytes);
void rtc_change_rate(unsigned char rate);
void rtc_handler();



#endif
