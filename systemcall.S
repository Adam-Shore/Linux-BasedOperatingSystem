#define ASM     1
#include "systemcall.h"
.text
 
.globl system_call, halt, execute, read, write, open, close, getargs, vidmap, handler, sigreturn

.align 4


/* system_call
 * Inputs: system call number in eax
 * Return Value: none
 * Function: calls the appropriate system call based on eax val, checks for invalid sys call num
 */
system_call:
    
    /*make sure eax is from 0-9*/
    cli
    subl $1, %eax
    cmpl $0,%eax
    jl INVALID_CALL_NUM
    cmpl $9,%eax
    jg INVALID_CALL_NUM

    /*push flags and arguments to stack*/
    //pushal
    pushfl
    pushl %esp
    pushl %ebp

    pushl %edi
    pushl %esi

    pushl %edx
    pushl %ecx
    pushl %ebx
    sti
    call *jump_table(,%eax,4) //eax holds system call number
    popl %ebx
    popl %ecx
    popl %edx


    popl %esi
    popl %edi

    popl %ebp
    popl %esp

    popfl
    //popal
    iret

INVALID_CALL_NUM:
    /*return -1 if invalid system call number*/
    movl $-1, %eax
    iret

jump_table:
    .long halt_function, execute_function, read_function, write_function, open_function, close_function, getargs_function, vidmap_function, set_handler_function, sigreturn_function

.end
