#include "pit.h"

void schedule();

/* void pit_init
 * Inputs: int hz
 * Return Value: none
 * Function: initialize pit */
void pit_init(int hz) {
    //cli();
    //disable_irq(PIT_IRQ_NUM);
    ticks = 0;
    int divisor = 1193180/hz;
    enable_irq(PIT_IRQ_NUM);
    outb(SQUARE_WAVE, COMMAND_REG);
    outb(divisor & 0xFF, CHANNEL_0);
    outb(divisor >> 8, CHANNEL_0);
    //sti();
}

/* void pit_handler
 * Inputs: none
 * Return Value: none
 * Function: handle pit interrupts */
void pit_handler() {
    ticks++;
    if (ticks % 36 == 0) {
        //printf("One second");
        schedule();
    }
    send_eoi(PIT_IRQ_NUM);
}
