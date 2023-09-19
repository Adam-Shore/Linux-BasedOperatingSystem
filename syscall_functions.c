#include "syscall_functions.h"

pcb_t processes[6];
int pid = -1;

/* void invalid_function
 * Inputs: none
 * Return Value: int32_t
 * Function: used when table pointer should point to invalid function
 */
int32_t invalid_function() {
    return -1;
}


/* void halt_function
 * Inputs: uint8_t status
 * Return Value: none
 * Function: halts the previous execute call and returns back to the end of execute
 */
int32_t halt_function(uint8_t status) {

    
    /*Re-Execute "shell" if parent pid is -1*/
    if(processes[pid].pid == 0){
        uint8_t temp[30] = "shell ";
        execute_function((uint8_t *) temp);
    }

    /*close all fd's in halting process*/
    int s;
    for(s=0; s<8; s++){
        close_function(s);
    }

    /*Get Parent Process*/
    processes[pid].active = 0;
    memset(processes[pid].args, 0, 4096);
    int old_pid = pid;


    pid = processes[pid].parent_pid;
    terminals[cur_terminal].latest_pid = pid;
    

    /*Set TSS For Parent*/
    // processes[pid].esp0 = (0x800000 - (0x2000*(pid))) - 4;
    tss.esp0 = (0x800000 - (0x2000*(pid))) - 4; //Check if this sets over
    tss.ss0 = KERNEL_DS;


    /*Unmap Paging For Current Process*/
    /*Unmap Paging For Current Process*/
    /*--------------------------------*/
    
    //not sure if correct
    page_dir[32].mb.val = 0x00000000;

    /*--------------------------------*/
    /*Unmap Paging For Current Process*/
    /*Unmap Paging For Current Process*/


    /*Map Parent's Paging*/
    page_dir[32].mb.p = 1;
    page_dir[32].mb.rw = 1;
    page_dir[32].mb.us = 1;
    page_dir[32].mb.pwt = 0;
    page_dir[32].mb.pcd = 0;
    page_dir[32].mb.a = 0;
    page_dir[32].mb.d = 0;
    page_dir[32].mb.ps = 1;
    page_dir[32].mb.g = 0;
    page_dir[32].mb.avail = 0;
    page_dir[32].mb.pat = 0;
    page_dir[32].mb.res = 0;
    page_dir[32].mb.p_base = (0x800000 + (0x400000 * pid))>> 22; // = 8 MB >> 22 OR 12 MB >> 22

    uint32_t addr = (uint32_t) first_page_table;
    page_dir[0].kb.p = 1;
    page_dir[0].kb.rw = 1;
    page_dir[0].kb.us = 0;
    page_dir[0].kb.pwt = 0;
    page_dir[0].kb.pcd = 0;
    page_dir[0].kb.a = 0;
    page_dir[0].kb.zero = 0;
    page_dir[0].kb.ps = 0;
    page_dir[0].kb.g = 0;
    page_dir[0].kb.avail = 0;
    page_dir[0].kb.p_base = addr >> 12;

    video_mapping_page[cur_terminal].p = 0;
    video_mapping_page[cur_terminal].rw = 1;
    video_mapping_page[cur_terminal].us = 1; 
    video_mapping_page[cur_terminal].pwt = 0;
    video_mapping_page[cur_terminal].pcd = 0;
    video_mapping_page[cur_terminal].a = 0;
    video_mapping_page[cur_terminal].d = 0;
    video_mapping_page[cur_terminal].pat = 0;
    video_mapping_page[cur_terminal].g = 0;
    video_mapping_page[cur_terminal].avail = 0;
    video_mapping_page[cur_terminal].p_base = 0xB8 + (cur_terminal * 0x1000);

    flushtlb();

    /*Close Any Relevant FDs*/
    // int i = 0;
    // for(i = 2; i < 8; i++){
    //     processes[old_pid].fd_arr[i].avail = 1;
    //     processes[old_pid].fd_arr[i].file_pos = 0;
    //     processes[old_pid].fd_arr[i].inode_num = 0;
    //     processes[old_pid].fd_arr[i].file_op_table.open_ptr = NULL;
    //     processes[old_pid].fd_arr[i].file_op_table.read_ptr = NULL;
    //     processes[old_pid].fd_arr[i].file_op_table.write_ptr = NULL;
    //     processes[old_pid].fd_arr[i].file_op_table.close_ptr = NULL;
    // }

    /*set Parent's Process As Active*/
    processes[pid].active = 1;



    /*Call Halt Return*/
    uint32_t esp_val = processes[old_pid].saved_esp;
    uint32_t ebp_val = processes[old_pid].saved_ebp;
    uint32_t ret_val = status;
    halt_return(esp_val, ebp_val, ret_val);
    
    return status;
    /*
    if there is an exception raised when the program is running, we need to squash program and return to shell - otherwise, we can assume programs run to completion
    */
}


/* int32_t execute_function
 * Inputs: const uint8_t* command
 * Return Value: int32_t
 * Function: takes in command as filename and command, executes file
 */
int32_t execute_function(const uint8_t* command){ 
    uint32_t old_pid = pid;


    if(command == NULL){
        return -1;
    }

    //clear();
 
    //initialize pcb here
    uint8_t space = 32;
    uint8_t* start = (uint8_t* )command;
    uint8_t* final = (uint8_t* )command;
    uint8_t size = 0;
    int i;
    int temp;
    //first two elements of fd array are stdin and stdout
    file_op_table_t stdin_table;
    stdin_table.open_ptr = (void*)terminal_open;
    stdin_table.read_ptr = (void*)terminal_read;
    stdin_table.write_ptr = (void*)invalid_function;
    stdin_table.close_ptr = (void*)terminal_close;

    file_op_table_t stdout_table;
    stdout_table.open_ptr = (void*)terminal_open;
    stdout_table.read_ptr = (void*)invalid_function;
    stdout_table.write_ptr = (void*)terminal_write;
    stdout_table.close_ptr = (void*)terminal_close;



    // 0 is null terminating say you just give one command called shell
    while(*final != space && *final != '\0'){
        final++;
        size++;
    }

    uint8_t filename_buffer[size+1];
    memcpy(filename_buffer,start,size);
    //printf("%x \n",size);
    filename_buffer[size] = '\0';

    //printf("%s \n",filename_buffer);

    directory_entry dentry;
    int32_t return_value = read_dentry_by_name(filename_buffer,&dentry);
    //printf("%x",return_value);
    //printf("\n");
    

    if(return_value == -1){ //check if the filename is valid
        return -1;
    }

    // printf("%d \n",*final);

    uint32_t space_counter = 0;
    while(*final == space){ //skip all leading spaces
        final++;
        space_counter++;

    }

    command_buffer = final; //to hold args that are after the filename

    temp = -1;
    for (i = 0; i < 6; i++) {
        if (processes[i].active == 0) {
            pid = i;
            temp = pid;
            break;
        }
    }

    if (temp == -1) {
        printf("Max programs already running\n");
        return 0;
    }


    strncpy((int8_t*)processes[pid].args, (int8_t*)command_buffer, strlen((int8_t*)command)- size - space_counter);

    uint8_t check_buffer[4];
    read_data(dentry.inode_num,0,check_buffer,4);
    // uint8_t i;
    // printf(type(check_buffer[0]));
    if(check_buffer[0] != 0x7f || (char)check_buffer[1] != 'E' || (char)check_buffer[2] != 'L' || (char)check_buffer[3] != 'F'){
        //printf("not an executable!");
        return -1;
    }
    //printf("an executable!");


    //processes array holds all processes, max 6
    //setting value of top of the kernel stack through esp0
    //set esp0 to TSS esp0, already defined
    

    processes[pid].terminal = cur_terminal;
    terminals[cur_terminal].latest_pid = pid;

    processes[pid].fd_arr[0].file_op_table = stdin_table;
    processes[pid].fd_arr[0].inode_num = 0;
    processes[pid].fd_arr[0].file_pos = 0;
    processes[pid].fd_arr[0].flags = 0;
    processes[pid].fd_arr[0].avail = 0;

    processes[pid].fd_arr[1].file_op_table = stdout_table;
    processes[pid].fd_arr[1].inode_num = 0;
    processes[pid].fd_arr[1].file_pos = 0;
    processes[pid].fd_arr[1].flags = 0;
    processes[pid].fd_arr[1].avail = 0;

    //first two entries in FD array have to be stdin and stdout, assign their correspodning file_op_table
    for (i = 2; i < 8; i++) {
        processes[pid].fd_arr[i].avail = 1;
    }
    

    
    /*Map Parent's Paging*/
    page_dir[32].mb.p = 1;
    page_dir[32].mb.rw = 1;
    page_dir[32].mb.us = 1;
    page_dir[32].mb.pwt = 0;
    page_dir[32].mb.pcd = 0;
    page_dir[32].mb.a = 0;
    page_dir[32].mb.d = 0;
    page_dir[32].mb.ps = 1;
    page_dir[32].mb.g = 0;
    page_dir[32].mb.avail = 0;
    page_dir[32].mb.pat = 0;
    page_dir[32].mb.res = 0;
    page_dir[32].mb.p_base = (0x800000 + (0x400000 * pid))>> 22; // = 8 MB >> 22 OR 12 MB >> 22

    flushtlb();

    //Flush TLB after you change the Paging

    //Read EXE
    uint8_t * address = (uint8_t *)0x08048000;
    inode_entry *cur_inode = &(inode_entry_ptr[dentry.inode_num]);
    // uint8_t temp_buf[cur_inode->length];

    read_data(dentry.inode_num,0,address, cur_inode->length);
    //tmp = tmp;
    // for(i=0; i<cur_inode->length;i++){
    //     putc(temp_buf[i]);
    // }

    //printf("Read data ret: %d\n", tmp);
    //printf("Read %d bytes into address.\n", cur_inode->length);
    //for (i = 0; i < 255; ++i) {
    //    printf("%x", *(address + i));
    //}
    //printf("End file.\n");

    //update pcb block data and save values
    processes[pid].pid = pid;
    processes[pid].parent_pid = old_pid;
    register uint32_t tempEsp asm("esp");
    processes[pid].saved_esp = tempEsp;
    register uint32_t tempEbp asm("ebp");
    processes[pid].saved_ebp = tempEbp;
    processes[pid].active = 1;
    // processes[pid].esp0 = (0x7CF000 - (0x2000*pid));
    processes[pid].esp0 = (0x800000 - (0x2000*(pid))) - 4; // 8 MB minus 8kb times pid (the minus 4 is to prevent overlap)
    tss.esp0 = (0x800000 - (0x2000*(pid))) - 4;
    tss.ss0 = KERNEL_DS;
    
    //Check through info mem

    //put 24-27 of .exe in EIP
    // uint32_t instr[4];
    // read_data(dentry.inode_num,24,instr, 4);
    // uint32_t eip = 0x00000000;
    // eip = eip | instr[0];
    // eip = eip | (instr[1] << 8);
    // eip = eip | (instr[2] << 16);
    // eip = eip | (instr[3] << 24);

    //get valid eip and read data
    uint32_t eip = 0x00000000;
    if(read_data(dentry.inode_num,(uint32_t)24,(uint8_t*)&eip,4) == -1){
        return -1;
    }


    // printf("four bytes\n");
    // printf("%x", instr[0]);
    // printf("%x", instr[1]);
    // printf("%x", instr[2]);
    // printf("%x", instr[3]);
    uint32_t usr_esp = 0x8400000 - 4; // 132MB
    //printf("\n");
    //printf("EIP  IS: %x", eip);
    
    //switch to user mode
    user_mode(USER_DS, usr_esp, USER_CS, eip);
    
    return 0;
}



/* int32_t read_function
 * Inputs: int32_t fd, void* buf, int32_t nbytes
 * Return Value: int32_t
 * Function: accesses appropriate read pointer and calls it if file descriptor is valid
 */
int32_t read_function(int32_t fd, void* buf, int32_t nbytes){
    //make sure a valid fd
    if (fd < 0 || fd > fd_ind) {
        return -1;
    }

    //if not a valid fd, return -1
    if (processes[pid].fd_arr[fd].avail == 1) {
        return -1;
    }

    //setupPCB();
    // I'm not sure if i should be making this call here, the PCB for some reason was all 0 so I added this
    
    //make call to read function
    int32_t return_val = processes[pid].fd_arr[fd].file_op_table.read_ptr(fd, buf, nbytes);


    return return_val;
}

/* int32_t write_function
 * Inputs: int32_t fd, const void* buf, int32_t nbytes
 * Return Value: int32_t
 * Function: checks that file descriptor is valid and calls appropriate write value
 */
int32_t write_function(int32_t fd, const void* buf, int32_t nbytes){
    //make sure a valid fd
    if (fd < 0 || fd >= fd_ind) {
        return -1;
    }
    //if not a valid fd, return -1
    if (processes[pid].fd_arr[fd].avail == 1) {
        return -1;
    }

    //setupPCB();
    // I'm not sure if i should be making this call here, the PCB for some reason was all 0 so I added this
    
    //make call to write function
    int return_val = processes[pid].fd_arr[fd].file_op_table.write_ptr(fd, (void* )buf, nbytes);

    return return_val;
}

/* int32_t open_function
 * Inputs: const uint8_t* filename
 * Return Value: int32_t
 * Function: reads data entry, allocate unused file descriptor and initialize it
 */

int32_t open_function(const uint8_t* filename){
    //make sure file is valid
    //find directory entry correspoding to file

    //printf(" hello",filename);

    //printf(" \n %x \n",filename);
    
    if (filename == NULL) {
        return -1;
    }
    directory_entry dir_entr;
    int i;
    int cur_fd = -1;
    int32_t rf = read_dentry_by_name(filename, &dir_entr);
    if (rf == -1) {
        return -1;
    }
    

    //allocate unused fd, return -1 if none available

    for (i = 2; i < 8; i++) {
        if (processes[pid].fd_arr[i].avail) {
            cur_fd = i;
            break;
        }
    }

    if (cur_fd == -1) {
        return -1;
    }

    file_descriptor_t fd_elem;
    file_op_table_t ft;


    fd_elem.inode_num = 0;
    fd_elem.file_pos = 0;
    fd_elem.flags = 0;
    fd_elem.avail = 0;
    //figure out what file type it is and set function ptrs appropriately
    if (dir_entr.filetype == 0) { //RTC file
        ft.open_ptr = (void*)rtc_open;
        ft.read_ptr = (void*)rtc_read;
        ft.write_ptr = (void*)rtc_write;
        ft.close_ptr = (void*)rtc_close;
    } else if (dir_entr.filetype == 1) { //directory file
        ft.open_ptr = (void*)dir_open;
        ft.read_ptr = (void*)dir_read;
        ft.write_ptr = (void*)dir_write;
        ft.close_ptr = (void*)dir_close;
    } else if (dir_entr.filetype == 2) { //regular file
        ft.open_ptr = (void*)filesystem_open;
        ft.read_ptr = (void*)filesystem_read;
        ft.write_ptr = (void*)filesystem_write;
        ft.close_ptr = (void*)filesystem_close;

        fd_elem.inode_num = dir_entr.inode_num;
    }
    
    fd_elem.file_op_table = ft;


    processes[pid].fd_arr[cur_fd] = fd_elem;

    fd_ind = cur_fd;

    return cur_fd;
}

/* int32_t close_function
 * Inputs: int32_t fd
 * Return Value: int32_t
 * Function: set passed in file descriptor to not available
 */
int32_t close_function(int32_t fd){
    if (fd <= 1 || fd > 7) {
        return -1;
    }
    //needs to be a valid fd that isn't stdin or stdout


    if (processes[pid].fd_arr[fd].avail == 1) {
        return -1;
    }

    processes[pid].fd_arr[fd].avail = 1;
    return 0;
}

/* int32_t getargs_function
 * Inputs: uint8_t* buf, int32_t nbytes
 * Return Value: int32_t
 * Function: retreive arguments from the executable file and place them into the inputted buffer
 */
int32_t getargs_function(uint8_t* buf, int32_t nbytes){
    //copy to user buffer
    strncpy((int8_t*)buf, (int8_t*)processes[pid].args, nbytes);

    return 0;
}

/* int32_t vidmap_function
 * Inputs: uint8_t** screen start
 * Return Value: int32_t
 * Function: map the video screen utilizing the screen_start input value
 */
int32_t vidmap_function(uint8_t** screen_start){
    if (screen_start == NULL) {
        return -1;
    }

    if (screen_start < (uint8_t**)0x8000000 || screen_start > (uint8_t**)(0x8000000 + 0x400000)) { 
        //below 128MB or above 128MB + 4 MB
        return -1;
    }

    video_mapping_page[0].p = 1;
    video_mapping_page[0].rw = 1;
    video_mapping_page[0].us = 1; 
    video_mapping_page[0].pwt = 0;
    video_mapping_page[0].pcd = 0;
    video_mapping_page[0].a = 0;
    video_mapping_page[0].d = 0;
    video_mapping_page[0].pat = 0;
    video_mapping_page[0].g = 0;
    video_mapping_page[0].avail = 0;
    video_mapping_page[0].p_base = 0xB8;

    // int i = 0;
    // for (i = 0; i < 1024; i++) { 
    //     // first_page_table[i].rw = 1;
    //     // first_page_table[i].us = 0; 
    //     // first_page_table[i].pwt = 0;
    //     // first_page_table[i].pcd = 0;
    //     // first_page_table[i].a = 0;
    //     // first_page_table[i].d = 0;
    //     // first_page_table[i].pat = 0;
    //     // first_page_table[i].g = 0;
    //     // first_page_table[i].avail = 0;
    //     // first_page_table[i].p_base = i;

    //     video_mapping_page[i].p = 0;
    //     video_mapping_page[i].rw = 1;
    //     video_mapping_page[i].us = 0; 
    //     video_mapping_page[i].pwt = 0;
    //     video_mapping_page[i].pcd = 0;
    //     video_mapping_page[i].a = 0;
    //     video_mapping_page[i].d = 0;
    //     video_mapping_page[i].pat = 0;
    //     video_mapping_page[i].g = 0;
    //     video_mapping_page[i].avail = 0;
    //     video_mapping_page[i].p_base = i;
    // }

    // video_mapping_page[cur_terminal].p = 1;
    // video_mapping_page[cur_terminal].rw = 1;
    // video_mapping_page[cur_terminal].us = 1; 
    // video_mapping_page[cur_terminal].pwt = 0;
    // video_mapping_page[cur_terminal].pcd = 0;
    // video_mapping_page[cur_terminal].a = 0;
    // video_mapping_page[cur_terminal].d = 0;
    // video_mapping_page[cur_terminal].pat = 0;
    // video_mapping_page[cur_terminal].g = 0;
    // video_mapping_page[cur_terminal].avail = 0;
    // video_mapping_page[cur_terminal].p_base = 0xB8 + (cur_terminal * 0x1000);

    flushtlb();

    *screen_start = (uint8_t*)0x08400000;
    
    return 0;
}

/* int32_t set_handler_function
 * Inputs: int32_t signum, void* handler address
 * Return Value: int32_t
 * Function: TBD: Checkpoint 4
 */
int32_t set_handler_function(int32_t signum, void* handler_address){
    return -1;
}

/* int32_t sigreturn_function
 * Inputs: void
 * Return Value: int32_t
 * Function: TBD: Checkpoint 4
 */
int32_t sigreturn_function(void){
    return -1;
}

/*helper function that returns the current fd*/
extern file_descriptor_t* get_cur_fd(int32_t fd) {
    return &processes[pid].fd_arr[fd];
}
