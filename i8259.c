/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"
#include "types.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {
    //unsigned char a1, a2;
   
    /*save the masks*/
    master_mask = MASK;
    slave_mask = MASK;

    /*start the initialization sequence (in cascade mode)*/
    outb(ICW1, MASTER_8259_PORT);
    outb(ICW1, SLAVE_8259_PORT);
    outb(ICW2_MASTER, MASTER_8259_PORT+1);
    outb(ICW2_SLAVE, SLAVE_8259_PORT+1);
    outb(ICW3_MASTER, MASTER_8259_PORT+1);
    outb(ICW3_SLAVE, SLAVE_8259_PORT+1);
    outb(ICW4, MASTER_8259_PORT+1);
    outb(ICW4, SLAVE_8259_PORT+1);

    /*restore the saved masks*/
    outb(master_mask, MASTER_8259_PORT+1);
    outb(slave_mask, SLAVE_8259_PORT+1);

    /*enable slave PIC*/
    enable_irq(2);
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
   
    if(irq_num < 8){
        master_mask = master_mask & (~(0x1<<(irq_num)));
        outb(master_mask, MASTER_8259_PORT+1);
    }
    else{
        slave_mask = slave_mask & (~(0x1<<(irq_num-8)));
        outb(slave_mask, SLAVE_8259_PORT+1);
        master_mask = master_mask & IRQ2_ENABLE;
        outb(master_mask, MASTER_8259_PORT+1);
    }
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    if(irq_num < 8){
        master_mask = master_mask | ((0x1<<(irq_num)));
        outb(master_mask, MASTER_8259_PORT+1);
    }
    else{
        slave_mask = slave_mask | ((0x1<<(irq_num-8)));
        outb(slave_mask, SLAVE_8259_PORT+1);
    }
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    //int ret = irq_num | EOI; /*irq_num gets OR'd with the EOI byte and sent out
    //                           to the PIC to declare the interrupt finished*/
    if(irq_num < 8){ /*check if return value needs to be sent to secondary PIC*/
        outb(irq_num | EOI, MASTER_8259_PORT);
    }
    else{
        outb((irq_num - 8) | EOI, SLAVE_8259_PORT);
        outb(ICW3_SLAVE | EOI, MASTER_8259_PORT);
    }
   
}
