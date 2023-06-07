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


#endif /* CSI_LL_H */ 
