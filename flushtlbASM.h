#ifndef FLUSHTLBASM_H
#ifndef ASM
extern void flushtlb(void);
extern void user_mode(uint32_t ds, uint32_t esp, uint32_t cs, uint32_t eip);
extern void halt_return(uint32_t esp, uint32_t ebp, uint8_t status);
#endif
#endif
