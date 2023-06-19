/*
 * Low-Level HAL API
 *
 * APIs giving access to low-level functionality (may not be portable).
 *
 * Copyright (c) RISC-V International 2022. Creative Commons License. Auto-
 * generated file: DO NOT EDIT
 */

#ifndef CSI_LL_H
#define CSI_LL_H

/*
 * Interrupt enable bits in the mie CSR
 */
typedef enum {
    CSI_SW_INTERRUPTS_ENABLE = 8, /* Software interrupts enable bit index */
    CSI_TIMER_INTERRUPTS_ENABLE = 128, /* Timer interrupts enable bit index */
    CSI_EXT_INTERRUPTS_ENABLE = 2048, /* External interrupts enable bit index */
} csi_interrupt_enables_t;


/*
 * Enable or disable 3 classes of interrupts for this hart.  Must be run in machine
 * mode.
 *
 * @param int_enables: This word will be written directly to the mie CSR in order
 * to control whether software interrupts, timer interrupts and external interrupts
 * are enabled.  To enable all 3 classes of interrupts, pass a value of
 * CSI_SW_INTERRUPTS_ENABLE | CSI_TIMER_INTERRUPTS_ENABLE |
 * CSI_EXT_INTERRUPTS_ENABLE.  To disable all interrupts, pass a value of 0.
 * @return : Integer in the same format as int_enables, reflecting the previous
 * value of int_enables prior to the change.  This may be stored and passed back
 * into another call to csi_ll_set_interrupt_enables in order to restore the
 * previous interrupt enables state.
 */
unsigned csi_ll_set_interrupt_enables(unsigned int_enables);


#endif /* CSI_LL_H */ 
