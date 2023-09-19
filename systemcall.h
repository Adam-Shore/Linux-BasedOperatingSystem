#ifndef SYSTEMCALL_H
#define SYSTEMCALL_H
#include "x86_desc.h"
#include "types.h"
//#include "lib.h"
//#include "rtc.h"
//#include "keyboard.h"

#ifndef ASM

typedef struct file_op_table_t {
    int32_t (*open_ptr)(uint8_t*);
    int32_t (*read_ptr)(int32_t, void*, int32_t);
    int32_t (*write_ptr)(int32_t, void*, int32_t);
    int32_t (*close_ptr)(int32_t);
}__attribute__ ((packed)) file_op_table_t;

typedef struct file_descriptor_t{
    file_op_table_t file_op_table;
    uint32_t inode_num;
    uint32_t file_pos;
    uint32_t flags;
    uint32_t avail; // active is 0 and active is 1
} __attribute__ ((packed)) file_descriptor_t;
//file_op_table is a file_op_table_t struct


/*will need to have file descriptor array full of file_descriptor_t elements
pcb tontains an fd_arr
initialize pcb in execute i believe
open will set the open, close, read, write ptrs
ex. if open file and recognize it's rtc, set ptrs to rtc_open, rtc_close, rtc_read, rtc_write functions
*/

typedef struct pcb_t{
    file_descriptor_t fd_arr[8]; //IS this right?
    uint32_t pid;
    uint32_t parent_pid;
    uint32_t saved_esp;
    uint32_t saved_ebp;
    uint32_t active;
    uint32_t esp0;
    uint8_t args[4096];
    int terminal;
} __attribute__ ((packed)) pcb_t;
//i know for sure the fd_arr is correct but im not sure about the rest of them, they may be the wrong sizes






extern void system_call(void);




#endif
#endif
