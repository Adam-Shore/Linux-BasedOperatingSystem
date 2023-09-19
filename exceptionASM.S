#define ASM     1
#include "exceptionASM.h"
.text

.globl divide, debug_exception, nmi, breakpoint, overflow, bound_range, invalid_op
.globl device_not_available, double_fault, invalid_tss, segment_not_present
.globl stack_segment, general_protection, page, fpu_floating_point_error
.globl alignment_check, machine_check, simd_floating_point_exception
.globl coprocessor_segment, rtc_exception, keyboard_exception, system_call_exception, pit_exception

.align 4

divide:
    pushal
    pushfl
    call divide_print
    popfl
    popal
    iret

debug_exception:
    pushal
    pushfl
    call debug_exception_print
    popfl
    popal
    iret

nmi:
    pushal
    pushfl
    call nmi_print
    popfl
    popal
    iret

breakpoint:
    pushal
    pushfl
    call breakpoint_print
    popfl
    popal
    iret

overflow:
    pushal
    pushfl
    call overflow_print
    popfl
    popal
    iret

bound_range:
    pushal
    pushfl
    call bound_range_print
    popfl
    popal
    iret

invalid_op:
    pushal
    pushfl
    call invalid_op_print
    popfl
    popal
    iret

device_not_available:
    pushal
    pushfl
    call device_not_avail_print
    popfl
    popal
    iret

double_fault:
    pushal
    pushfl
    call double_fault_print
    popfl
    popal
    iret

invalid_tss:
    pushal
    pushfl
    call invalid_tss_print
    popfl
    popal
    iret

segment_not_present:
    pushal
    pushfl
    call segment_not_present_print
    popfl
    popal
    iret

stack_segment:
    pushal
    pushfl
    call stack_segment_print
    popfl
    popal
    iret

general_protection:
    pushal
    pushfl
    call general_protection_print
    popfl
    popal
    iret

page:
    pushal
    pushfl
    call page_fault_print
    popfl
    popal
    iret

fpu_floating_point_error:
    pushal
    pushfl
    call fpu_floating_point_print
    popfl
    popal
    iret

alignment_check:
    pushal
    pushfl
    call alignment_check_print
    popfl
    popal
    iret

machine_check:
    pushal
    pushfl
    call machine_check_print
    popfl
    popal
    iret

simd_floating_point_exception:
    pushal
    pushfl
    call simd_floating_point_print
    popfl
    popal
    iret

coprocessor_segment:
    pushal
    pushfl
    call coprocessor_segment_print
    popfl
    popal
    iret

rtc_exception:
    pushal
    pushfl
    call rtc_handler
    popfl
    popal
    iret

pit_exception:
    pushal
    pushfl
    call pit_handler
    popfl
    popal
    iret

keyboard_exception:
    pushal
    pushfl
    call keyboard_handler
    popfl
    popal
    iret


system_call_exception:
    pushal
    pushfl
    call system_call
    popfl
    popal
    iret

.end
