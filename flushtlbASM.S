#define ASM     1
#include "flushtlbASM.h"
.text

.globl flushtlb, user_mode, halt_return
.align 4

/*
Flushes the TLB, to be called when a new page directory entry is created
*/
flushtlb:
    movl	%cr3,%eax
    movl	%eax,%cr3
    ret
    
/* user_mode
 * Inputs: DS, ESP, CS, EIP
 * Return Value: none
 * Function: pushes the iret context, switches to user mode
 */
user_mode:
    pushl %ebp
    movl %esp, %ebp

    // pushl %eax
    // pushl %ebx
    // pushl %ecx
    // pushl %edx
    //ds, esp, cs, eip
    movl 8(%ebp), %eax
    pushl %eax
    movl 12(%ebp), %eax
    pushl %eax
    pushfl
    movl 16(%ebp), %eax
    pushl %eax
    movl 20(%ebp), %eax
    pushl %eax
    //movw %ax,%ds
    //pushl %eax
    //pushl %ebx
    //pushfl
    //popl %ebx
    //orl $0x200, %ebx
    //pushl %ebx
    //pushl %ecx
    //pushl %edx

    // popl %edx
    // popl %ecx
    // popl %ebx
    // popl %eax
    // pushl %edx
    iret 

halt_return:
    pushl %ebp
    movl %esp, %ebp
    
    movl 8(%ebp),%esp
    movl 16(%ebp),%ebx
    movl 12(%ebp),%ebp

    //popl %eax
    //popl %eax
    //popfl
    //popl %eax
    //popl %eax
    
    movl %ebx, %eax

    leave
    ret

.end
