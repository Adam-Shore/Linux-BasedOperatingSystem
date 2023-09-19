#ifndef EXCEPTIONASM_H
#define EXCEPTIONASM_H
#include "x86_desc.h"
#include "types.h"
//#include "lib.h"
//#include "rtc.h"
//#include "keyboard.h"

#ifndef ASM

extern void divide(void);
extern void debug_exception(void);
extern void nmi(void);
extern void breakpoint(void);
extern void overflow(void);
extern void bound_range(void);
extern void invalid_op(void);
extern void device_not_available(void);
extern void double_fault(void);
extern void invalid_tss(void);
extern void segment_not_present(void);
extern void stack_segment(void);
extern void general_protection(void);
extern void page(void);
extern void fpu_floating_point_error(void);
extern void alignment_check(void);
extern void machine_check(void);
extern void simd_floating_point_exception(void);
extern void coprocessor_segment(void);

extern void rtc_exception(void);
extern void pit_exception(void);
extern void keyboard_exception(void);
extern void system_call_exception(void);




#endif
#endif
