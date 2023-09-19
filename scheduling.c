#include "scheduling.h"

// int cur_terminal = 0;
terminal_t terminals[3];
int pid;
int32_t execute_function(const uint8_t* command);
int32_t halt_function(uint8_t status);
pcb_t processes[6];

/* void switch_terminal
 * Inputs: int terminal_from, int terminal_to
 * Return Value: none
 * Function: switch to terminal_to, swap all corresponding vars and vidmem */
void switch_terminal(int terminal_from, int terminal_to){
    if (terminal_from == terminal_to) {
        return;
    }

    terminals[terminal_from].cursor_y = get_screen_y(); //save cursor position before switching
    terminals[terminal_from].cursor_x = get_screen_x();

    if (terminal_from == 0) {
        memcpy((char*)TERM_1_MEM, (char*)REAL_VID_MEM, 4096);
    } else if (terminal_from == 1) {
        memcpy((char*)TERM_2_MEM, (char*)REAL_VID_MEM, 4096);
    } else if (terminal_from == 2) {
        memcpy((char*)TERM_3_MEM, (char*)REAL_VID_MEM, 4096);
    }

    //save_screen(buf);

    //memcpy(terminals[terminal_from].screen_text, buf, SCREEN_SIZE);
    
    //add vidmapping stuff
    //restore_screen(terminals[terminal_to].screen_text);
    
    //pid = terminals[terminal_to].latest_pid;
    //tss.esp0 = (0x800000 - (0x2000*(pid))) - 4; //???

    cur_terminal = terminal_to; //set new cur_terminal
    // processes[pid].active = 0;
    pid = terminals[terminal_to].latest_pid;

    if (terminal_to == 0) {
        memcpy((char*)REAL_VID_MEM, (char*)TERM_1_MEM, 4096);
    } else if (terminal_to == 1) {
        memcpy((char*)REAL_VID_MEM, (char*)TERM_2_MEM, 4096);
    } else if (terminal_to == 2) {
        memcpy((char*)REAL_VID_MEM, (char*)TERM_3_MEM, 4096);
    }

    set_position(terminals[terminal_to].cursor_x, terminals[terminal_to].cursor_y);
    update_cursor();

    if (terminals[terminal_to].running == 0 && terminal_to != 0){
        clear();
        terminals[terminal_to].running = 1;
        execute_function((uint8_t*)"shell");
        // talk to sched
    }

}

/* void schedule
 * Inputs: none
 * Return Value: none
 * Function: round robin rotate through processes to execute, including not displayed processes */
void schedule(){
    return;
}
