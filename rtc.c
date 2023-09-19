#include "rtc.h"


//#include <pc.h>

int flag = 0;
uint32_t frequency_buffer;
int virtual_counter = 0;
int virtual_frequency = 16;

/* void rtc_init
 * Inputs: none
 * Return Value: none
 * Function: initialize the rtc
 */
void rtc_init(){
     
    cli();			// disable interrupts
    
    disable_irq(RTC_IRQ_NUM);
    
    outb(REGISTER_B, RTC_PORT);		// select register B, and disable NMI
    char prev = inb(RTC_DATA);	// read the current value of register B
    outb(REGISTER_B, RTC_PORT);		// set the index again (a read will reset the index to register D)
    outb(prev | 0x40, RTC_DATA);	// write the previous value ORed with 0x40. This turns on bit 6 of register B

    outb(REGISTER_C, RTC_PORT);	// select register C
    inb(RTC_DATA);		// just throw away contents

    enable_irq(RTC_IRQ_NUM);
    
    sti(); //enable interrupts
    
    
}

/* void rtc_handler
 * Inputs: none
 * Return Value: none
 * Function: the handler for whenever the rtc interrupt is triggered
 */
void rtc_handler(){
    
    //test_interrupts();
    //printf("RTC test");
    outb(REGISTER_C, RTC_PORT);	// select register C
    inb(RTC_DATA);		// just throw away contents
    send_eoi(RTC_IRQ_NUM); //send eoi signal
    flag = 1; //reset flag
    virtual_counter++;
    
}


/* void rtc_change_rate
 * Inputs: unsigned char rate
 * Return Value: none
 * Function: changes the rate of the rtc
 */
void rtc_change_rate(unsigned char rate){
    rate &= 0x0F; //select lower bits of rate with mask
    cli(); //disable interrupts
    //set port and data using register A
    outb(REGISTER_A, RTC_PORT);
    char prev=inb(RTC_DATA);
    outb(REGISTER_A, RTC_PORT);
    outb((prev & 0xF0) | rate, RTC_DATA);
    sti(); //enable interrupts
}

/* void rtc_open
 * Inputs: const uint8_t* filename
 * Return Value: int 0 on success
 * Function: opens the rtc and initializes the rate
 */
int rtc_open(const uint8_t* filename){
    //sets rate as default of 15
    rtc_change_rate(6);
    virtual_frequency = 1;
    return 0;
}

 /* void rtc_close
 * Inputs: int32_t fd
 * Return Value: int 0 on success
 * Function: to close the rtc
 */
int rtc_close(int32_t fd){
    //just returns 0
    return 0;
}

/* void rtc_read
 * Inputs: int32_t fd, void* buf, int32_t nbytes
 * Return Value: int 0 on success
 * Function: when handler called enough times, read is done
 */
int rtc_read(int32_t fd, void* buf, int32_t nbytes){
    //do not advance until flag is reset
    while((virtual_counter < virtual_frequency));
    flag = 0;
    virtual_counter = 0;
    return 0;
}

/* void rtc_write
 * Inputs: int32_t fd, void* buf, int32_t nbytes
 * Return Value: int 0 on success
 * Function: changes the rate depending on the inputted frequency
 */
int rtc_write(int32_t fd, const void* buf, int32_t nbytes){ //test if frequency is within range
    frequency_buffer = *(uint32_t*)buf; //define a variable for frequency based on input buffer
    /*test if frequency has specific values of powers of 2*/
    
    if(frequency_buffer <= FREQ1 && frequency_buffer >= FREQ10){
        if(frequency_buffer == FREQ1){
            //rtc_change_rate(6); //set new rate based on corresponding frequency
            virtual_frequency = 1;
            return 0;
        }
        else if(frequency_buffer == FREQ2){
            //rtc_change_rate(7); //set new rate based on corresponding frequency
            virtual_frequency = 2;
            return 0;
        }
        else if(frequency_buffer == FREQ3){
            //rtc_change_rate(8); //set new rate based on corresponding frequency
            virtual_frequency = 4;
            return 0;
        }
        else if(frequency_buffer == FREQ4){
            //rtc_change_rate(9); //set new rate based on corresponding frequency
            virtual_frequency = 8;
            return 0;
        }
        else if(frequency_buffer == FREQ5){
            //rtc_change_rate(10); //set new rate based on corresponding frequency
            virtual_frequency = 16;
            return 0;
        }
        else if(frequency_buffer == FREQ6){
            //rtc_change_rate(11); //set new rate based on corresponding frequency
            virtual_frequency = 32;
            return 0;
        }
        else if(frequency_buffer == FREQ7){
            //rtc_change_rate(12); //set new rate based on corresponding frequency
            virtual_frequency = 64;
            return 0;
        }
        else if(frequency_buffer == FREQ8){
            //rtc_change_rate(13); //set new rate based on corresponding frequency
            virtual_frequency = 128;
            return 0;
        }
        else if(frequency_buffer == FREQ9){
            //rtc_change_rate(14); //set new rate based on corresponding frequency
            virtual_frequency = 256;
            return 0;
        }
        else if(frequency_buffer == FREQ10){
            //rtc_change_rate(15); //set new rate based on corresponding frequency
            virtual_frequency = 512;
            return 0;
        }
        else{
            virtual_frequency = 1;
            return -1;
        }
    }
    return -1;

}
