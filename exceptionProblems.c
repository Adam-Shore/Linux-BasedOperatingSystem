#include "exceptionProblems.h"
#include "lib.h"
// #include "syscall_functions.h"

int32_t halt_function(uint8_t status);

void divide_print() {

    printf("Divide Error");
    halt_function(0);
}

/* void debug_exception_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void debug_exception_print() {

    printf("Debug Exception");
    halt_function(0);
}

/* void nmi_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void nmi_print() {
    
    printf("NMI Interrupt");
    halt_function(0);
}

/* void breakpoint_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void breakpoint_print() {
    
    printf("Breakpoint");
    halt_function(0);
}

/* void overflow_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void overflow_print() {
    
    printf("Overflow");
    halt_function(0);
}

/* void bound_range_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void bound_range_print() {
    
    printf("BOUND Range Exceeded");
    halt_function(0);
}

/* void invalid_op_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void invalid_op_print() {
    
    printf("Invalid Opcode");
    halt_function(0);
}

/* void device_not_avail_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void device_not_avail_print() {
    
    printf("Device Not Available");
    halt_function(0);
}

/* void double_fault_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void double_fault_print() {
    
    printf("Double Fault");
    halt_function(0);
}

/* void invalid_tss_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void invalid_tss_print() {
    
    printf("Invalid TSS");
    halt_function(0);
}

/* void segement_not_present_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void segment_not_present_print() {
    
    printf("Segment Not Present");
    halt_function(0);
}

/* void stack_segment_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void stack_segment_print() {
    
    printf("Stack Segment Fault");
    halt_function(0);
}

/* void general_protection_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void general_protection_print() {
    
    printf("General Protection");
    halt_function(0);
}

/* void page_fault_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void page_fault_print() {
    
    printf("Page Fault");
    halt_function(0);
}

/* void fpu_floating_point_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void fpu_floating_point_print() {
    
    printf("FPU Floating-Point Error");
    halt_function(0);
}

/* void alignment_check_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void alignment_check_print() {
    
    printf("Alignment Check");
    halt_function(0);
}

/* void machine_check_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void machine_check_print() {
    
    printf("Machine Check");
    halt_function(0);
}

/* void simd_floating_point_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void simd_floating_point_print() {
    
    printf("SIMD Floating-Point Exception");
    halt_function(0);
}

/* void coprocessor_segment_print
 * Inputs: none
 * Return Value: none
 * Function: Called when this exception is encountered and prints to screen */
void coprocessor_segment_print() {
    
    printf("Coprocessor Segment Exception");
    halt_function(0);
}

void system_call_print() {
    
    printf("This is a system call");
    halt_function(0);
}



