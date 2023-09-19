#ifndef SYSCALL_FN_H_
#define SYSCALL_FN_H_

#include "lib.h"
#include "filesystem.h"
#include "rtc.h"
#include "systemcall.h"
#include "keyboard.h"
#include "paging.h"
#include "flushtlbASM.h"
#include "scheduling.h"

#define FD_MAX_IND 8

extern int32_t halt_function(uint8_t status);
extern int32_t execute_function(const uint8_t* command);
int32_t read_function(int32_t fd, void* buf, int32_t nbytes);
int32_t write_function(int32_t fd, const void* buf, int32_t nbytes);
int32_t open_function(const uint8_t* filename);
int32_t close_function(int32_t fd);
int32_t getargs_function(uint8_t* buf, int32_t nbytes);
int32_t vidmap_function(uint8_t** screen_start);
int32_t set_handler_function(int32_t signum, void* handler_address);
int32_t sigreturn_function(void);

extern file_descriptor_t* get_cur_fd(int32_t fd);

pcb_t cur_pcb;

int fd_ind = 2;

uint8_t * command_buffer; //Holds all the arguments

extern int pid;
extern pcb_t processes[6];



//index starts at 2 because first two entries in fd_arr are always occupied by stdin and stdout respectively

//these declarations maybe should be in systemcall.h but i was getting errors so i moved them

#endif
