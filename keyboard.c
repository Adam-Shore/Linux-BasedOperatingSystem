
#include "keyboard.h"

unsigned char* kb_buf = 0;
unsigned char kb_buf1[128];
unsigned char kb_buf2[128];
unsigned char kb_buf3[128];
int kb_ind = 0;

int kb_ind1 = 0;
int kb_ind2 = 0;
int kb_ind3 = 0;

int read_ind = 0;

int kb_flag = 0;
int caps = 0;
int ctrl = 0;
int tab = 0;
int alt = 0;
void switch_terminal(int terminal_from, int terminal_to);
int cur_terminal = 0;

char keys[keyboard_length] = {'1','2','3','4','5','6','7','8','9','0','a','b','c','d','e','f',
                        'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
                        };
unsigned char kbdus[128] =
{
    '0',  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    CAPS_LOCK,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

/* void keyboard_init
 * Inputs: none
 * Return Value: none
 * Function: initialize kb */
void keyboard_init(){
    // cli();
    enable_irq(KEYBOARD_IRQ_NUM);
    
    // sti();
}

/* void keyboard_handler
 * Inputs: none
 * Return Value: none
 * Function: handle keyboard interrupt */
void keyboard_handler(){
  uint8_t c = inb(keyboard_port);

  
  if (cur_terminal == 0){
    kb_buf = kb_buf1;
    kb_ind = kb_ind1;
  } else if (cur_terminal == 1) {
    kb_buf = kb_buf2;
    kb_ind = kb_ind2;
  } else if (cur_terminal == 2) {
    kb_buf = kb_buf3;
    kb_ind = kb_ind3;
  }
  // int i;
  int k;


    if (c == ALT_P) {
      alt = 1;
    }

    if (c == ALT_R) {
      alt = 0;
    }


    if (c == L_SHIFT_P || c == R_SHIFT_P) { //handle the case for R_SHIFT_P and L_SHIFT_P keys
        if (caps == 0) { //swap value for caps
          caps = 1;
        } else if (caps == 1) {
          caps = 0;
        }
      }

    if (c == L_SHIFT_R || c == R_SHIFT_R) {//handle the case for R_SHIFT_R and L_SHIFT_R keys
      if (caps == 0) { //swap value for caps
        caps = 1;
      } else if (caps == 1) {
        caps = 0;
      }
    }

    if (c == L_CTRL_P || c == R_CTRL_P) {//handle the case for R_CTRL_P and L_CTRL_P keys
        if (ctrl == 0) { //swap value for ctrl
          ctrl = 1;
        } else if (ctrl == 1) {
          ctrl = 0;
        }
      }

    if (c == L_CTRL_R || c == R_CTRL_R) {//handle the case for R_SHIFT_R and L_SHIFT_R keys
      if (ctrl == 0) { //swap value for ctrl
        ctrl = 1;
      } else if (ctrl == 1) {
        ctrl = 0;
      }
    }

    if(c<128){

      if (alt && (c == F1_P)) {
        //save what's on the screen
        //clear the screen
        //load from memory what was last on screen for terminal
        //execute shell if not already executed
        
        //execute_function((uint8_t*)"shell");
        send_eoi(KEYBOARD_IRQ_NUM);
        switch_terminal(cur_terminal, 0);
        
        return;
      }

      if (alt && (c == F2_P)) {
        send_eoi(KEYBOARD_IRQ_NUM);
        switch_terminal(cur_terminal, 1);
        //send_eoi(KEYBOARD_IRQ_NUM);
        return;
      }

      if (alt && (c == F3_P)) {
        send_eoi(KEYBOARD_IRQ_NUM);
        switch_terminal(cur_terminal, 2);
        //send_eoi(KEYBOARD_IRQ_NUM);
        return;
      }

      if (kbdus[c] == '\b') { //handle the case for \b
        // if (kb_ind != 0) {
        //   kb_ind--;
        // }
        //kb_buf[kb_ind] = NULL;
        if (tab) {
          for (k = 0; k < 4; k++) {
            backspace();
          }
          kb_ind--;
        } else if (kb_ind != 0){
          kb_ind--;
          backspace();
        }
        
      }

      if (kbdus[c] == '\t') {//handle the case for \t
        kb_buf[kb_ind] = '\t';
        kb_ind += 1;
        for (k = 0; k < 4; k++) {
          putc(' ');
        }
        tab = 1;
      } else {
        tab = 0;
      }
      
      if (ctrl && kbdus[c] == 'l') {//handle the case for l
            //reset values
            kb_ind = 0;
            kb_flag = 0;
            clearscreen(); //call to reset the screen
      }
      
      if (kbdus[c] == '\n') {//handle the case for \n
        if(scrolldown()) { //move down if needed
          kb_flag = 1;
          read_ind = kb_ind;
          kb_ind = 0;
          send_eoi(KEYBOARD_IRQ_NUM); //send eoi signal
          return;
        }
      }

      if (kb_ind >= 127) { //alternative case for \n
        if (kbdus[c] == '\n') {
          //putc('\n');
        } else {
          send_eoi(KEYBOARD_IRQ_NUM); //send eoi signal
          return;
        }
         
      }
      //make check for all other valid cases
      if (kbdus[c] != 0 && kbdus[c] != CAPS_LOCK && kbdus[c] != '\b' && kbdus[c] != '\t') {
          //check if in range
          if (caps && kbdus[c] >= 97 && kbdus[c] <= 122) { //values represent the range of valid entires for this segment
            //set buffer value and increment index
            kb_buf[kb_ind] = kbdus[c] - 32;//32 is offset
            kb_ind++;
            
            if (kb_ind == NUM_COLS) { //if reached end of line case
              newline(kbdus[c] - 32);//32 is offset
              //kb_ind = 0;
            } else {
              putc(kbdus[c] - 32);//32 is offset
            }
          } else {
            kb_buf[kb_ind] = kbdus[c];
            kb_ind++;
            if (kb_ind == NUM_COLS) { //if reached end of line
              newline(kbdus[c]);
              //kb_ind = 0;
            } else {
              putc(kbdus[c]);
            }
          }
          //clear screen if called correspondignly
          if (ctrl && kbdus[c] == 'l') {
            kb_ind = 0;
            kb_flag = 0;
            clearscreen();
          }
          
      }
      //handle case for \n
      if (kbdus[c] == '\n') {
        kb_flag = 1;
        read_ind = kb_ind;
        kb_ind = 0;
        // if(scrolldown()) {
        //   send_eoi(KEYBOARD_IRQ_NUM);
        //   return;
        // }
      }

      //handle case for caps lock
      if (kbdus[c] == CAPS_LOCK) {
        if (caps == 0) {
          caps = 1;
        } else if (caps == 1) {
          caps = 0;
        }
      }

      
      
         
    }
  if (cur_terminal == 0){
    kb_ind1 = kb_ind;
  } else if (cur_terminal == 1) {
    kb_ind2 = kb_ind;
  } else if (cur_terminal == 2) {
    kb_ind3 = kb_ind;
  }
  send_eoi(KEYBOARD_IRQ_NUM); //send eoi
}


/* void terminal_open
 * Inputs: filename
 * Return Value: none
 * Function: open terminal */
void terminal_open(const uint8_t* filename){
    return;
}

/* void terminal_open
 * Inputs: file descriptor
 * Return Value: none
 * Function: close terminal */
void terminal_close(int32_t fd){
    return;
}

/* void terminal_read
 * Inputs: int32_t fd, void* buf, int32_t nbytes
 * Return Value: none
 * Function: read nbytes into passed in buf from kb buf */ 
int terminal_read(int32_t fd, void* buf, int32_t nbytes){
    int i;


    if (cur_terminal == 0){
      kb_buf = kb_buf1;
      kb_ind = kb_ind1;
    } else if (cur_terminal == 1) {
      kb_buf = kb_buf2;
      kb_ind = kb_ind2;
    } else if (cur_terminal == 2) {
      kb_buf = kb_buf3;
      kb_ind = kb_ind3;
    }

    //wait for flag from interrupt
    while(!kb_flag);
    kb_flag = 0;
    //loop through indexes
    for (i = 0; i < read_ind; i++) {
      if (i > 127 || i >= nbytes) { //127 if valid indexes
        break;
      }
      //change buffer using keyboard buffer
      *((unsigned char*)buf + i) = kb_buf[i];
    }
    // while (kbdus[kb_buf[i]] != '\n' && i < 127 && i < nbytes) {
    //   *((unsigned char*)buf + i) = kb_buf[i];
    //   i++;
    // }
    //clearbuffer(kb_buf, nbytes);
    return i;
}

/* void terminal_write
 * Inputs: int32_t fd, void* buf, int32_t nbytes
 * Return Value: none
 * Function: write nbytes from buf to screen */
void terminal_write(int32_t fd, const void* buf, int32_t nbytes){
    int i;
    //scrolldown();
    //loop through nbytes
    for (i = 0; i < nbytes; i++) {
      //print characters desired from buffer
      
      putc(*((unsigned char*)buf + i));
      
    }
    if (cur_terminal == 0){
      kb_ind1 = 0;
      kb_ind = kb_ind1;
    } else if (cur_terminal == 1) {
      kb_ind2 = 0;
      kb_ind = kb_ind2;
    } else if (cur_terminal == 2) {
      kb_ind3 = 0;
      kb_ind = kb_ind3;
    }
    return;
}
