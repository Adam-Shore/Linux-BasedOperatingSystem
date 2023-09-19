.text

.globl load_page_dir, enable_paging
.align 4

load_page_dir:
    pushl %ebp
    movl %esp, %ebp
    movl 8(%esp), %eax
    movl %eax, %cr3
    movl %cr4, %eax
    orl $0x00000010, %eax
    movl %eax, %cr4 
    leave
    ret

enable_paging:
    pushl %ebp
    movl %esp, %ebp
    movl %cr0, %eax
    orl $0x80000001, %eax
    movl %eax, %cr0
    leave
    ret

.end
