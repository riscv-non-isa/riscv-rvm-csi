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

#include "csi_types.h"

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

/*
 * Set the frequency of the system timer.  Note that there is typically a single
 * timer for all harts in the system, so this function can affect the operation of
 * other harts.  When using the RVM-CSI high-level interrupt and timer support
 * module, the timer will be compared against a compare register for each hart in
 * order to produce a regular timer interrupt at a tick rate configured using
 * csi_set_timer_tick, which is used for timing purposes.  This function must run
 * in machine mode.
 *
 * @param timer_freq_mhz: System timer frequency in MHz
 * @return : Status of operation.  CSI_ERROR will be returned if the request is
 * invalid.
 */
csi_status_t csi_timer_config(unsigned timer_freq_mhz);

/*
 * Read the current timer value.  This function can be called from M-mode or
 * U-mode.  However, on systems where the timer is not directly readable from
 * U-mode, the function will have to ECALL to M-mode to make the read, which will
 * likely make the value innaccurate due to the delay incurred by this.
 *
 * @return : Current timer value
 */
long long csi_read_mtime(void);

/*
 * Get the system timer frequency as configured by csi_timer_config.
 *
 * @return : System timer frequency in Hz
 */
long long csi_get_timer_freq(void);


#endif /* CSI_LL_H */ 
