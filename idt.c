// All empty descriptor slots in the IDT should have the present flag for the descriptor set to 0. - does this mean that i do this for the intel reserved interrupts?

#include "idt.h"
#include "exceptionASM.h"
#include "systemcall.h"

/* int32_t init_idt
 * Inputs: none
 * Return Value: none
 * Function: initialize idt, set up handlers for all idt entries */
void init_idt() {
    int i;
    for (i = 0; i < 20; i++) {
        if (i == 15) {
            continue;   
        } 
        //idt[i].offset_15_00 = 0x0000;
        idt[i].seg_selector = KERNEL_CS;
        idt[i].reserved4 = 0;
        idt[i].reserved3 = 0;
        idt[i].reserved2 = 1;
        idt[i].reserved1 = 1;
        idt[i].size = 1;
        idt[i].reserved0 = 0;
        idt[i].dpl = 0;
        idt[i].present = 1;
        //idt[i].offset_31_16 = 0x0000;
        
    }

    for (i = 32; i < 256; i++) {
        //idt[i].offset_15_00 = 0x0000;
        idt[i].seg_selector = KERNEL_CS;
        idt[i].reserved4 = 0;
        idt[i].reserved3 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved1 = 1;
        idt[i].size = 1;
        idt[i].reserved0 = 0;
        idt[i].dpl = 0;
        idt[i].present = 0;
        //idt[i].offset_31_16 = 0x0000;

        if (i == SYSTEM_CALL_NUM) {
            idt[i].dpl = 0x3;
            idt[i].present = 1;
        }
        else if(i==RTC_NUM || i==KEYBOARD_NUM || i==PIT_NUM){
            idt[i].present = 1;
        }
    }


    // idt[0].present=1;
    SET_IDT_ENTRY(idt[0], divide);
    // idt[1].present=1;
    SET_IDT_ENTRY(idt[1], debug_exception);
    // idt[2].present=1;
    SET_IDT_ENTRY(idt[2], nmi);
    // idt[3].present=1;
    SET_IDT_ENTRY(idt[3], breakpoint);
    // idt[4].present=1;
    SET_IDT_ENTRY(idt[4], overflow);
    // idt[5].present=1;
    SET_IDT_ENTRY(idt[5], bound_range);
    // idt[6].present=1
    SET_IDT_ENTRY(idt[6], invalid_op);
    // idt[7].present=1;
    SET_IDT_ENTRY(idt[7], device_not_available);
    // idt[8].present=1;
    SET_IDT_ENTRY(idt[8], double_fault);
    // idt[10].present=1;
    SET_IDT_ENTRY(idt[9], coprocessor_segment);
    SET_IDT_ENTRY(idt[10], invalid_tss);
    // idt[11].present=1;
    SET_IDT_ENTRY(idt[11], segment_not_present);
    // idt[12].present=1;
    SET_IDT_ENTRY(idt[12], stack_segment);
    // idt[13].present=1;
    SET_IDT_ENTRY(idt[13], general_protection);
    // idt[14].present=1;
    SET_IDT_ENTRY(idt[14], page);
    // idt[16].present=1;
    //SET_IDT_ENTRY(idt[15], );
    SET_IDT_ENTRY(idt[16], fpu_floating_point_error);
    // idt[17].present=1;
    SET_IDT_ENTRY(idt[17], alignment_check);
    // idt[18].present=1;
    SET_IDT_ENTRY(idt[18], machine_check);
    // idt[19].present=1;
    SET_IDT_ENTRY(idt[19], simd_floating_point_exception);

    
    // idt[RTC_NUM].seg_selector = KERNEL_CS;
    // idt[RTC_NUM].reserved4 = 0;
    // idt[RTC_NUM].reserved3 = 0;
    // idt[RTC_NUM].reserved2 = 1;
    // idt[RTC_NUM].reserved1 = 1;
    // idt[RTC_NUM].size = 1;
    // idt[RTC_NUM].reserved0 = 0;
    // idt[RTC_NUM].dpl = 0;
    // idt[RTC_NUM].present = 1;

    SET_IDT_ENTRY(idt[RTC_NUM], rtc_exception);
    SET_IDT_ENTRY(idt[KEYBOARD_NUM], keyboard_exception);
    SET_IDT_ENTRY(idt[SYSTEM_CALL_NUM], system_call);
    SET_IDT_ENTRY(idt[PIT_NUM], pit_exception);


}
