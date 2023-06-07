/*
 * Enumeration of Standard Interrupt Sources
 *
 * Enumeration of standard interrupt sources for use by high-level interrupt and
 * timer sub-system APIs.
 *
 * Copyright (c) RISC-V International 2022. Creative Commons License. Auto-
 * generated file: DO NOT EDIT
 */

#ifndef CSI_HL_INTERRUPT_SOURCES_H
#define CSI_HL_INTERRUPT_SOURCES_H

/*
 * Enumerates all sources in the system that can give rise to a trap event.  This
 * enumeration should be treated as an integer which can have additional values
 * beyond those enumerated here.  A standard set of interrupt and exception sources
 * are enumerated here, which exclude external sources.  All external interrupt
 * sources should be enumerated by the BSP within csi_hl_bsp_interrupts.h; and
 * their ennumerated values should follow on from CSI_NUM_STANDARD_TRAP_SOURCES.
 * csi_hl_bsp_interrupts.h must also define CSI_TOTAL_BSP_TRAP_SOURCES to indicate
 * the total number of sources defined.  User applications may then use values
 * above this to enumerate user-defined software signals.  Note that the actual
 * enumerated values have no meaning beyond serving to uniquely identify the source
 * of a trap event.  They are not intended to map onto values in the mcause
 * register.
 */
typedef enum {
    CSI_ENUM_NMI = 0, /* Non-maskable exception (hardware error) */
    CSI_ENUM_MACHINE_SW_INTERRUPT, /* Machine mode software interrupt */
    CSI_ENUM_MACHINE_TIMER_INTERRUPT, /* Machine mode timer interrupt */
    CSI_ENUM_INSTRUCTION_ADDR_MISALIGNED_EXCEPTION, /* Instruction misaligned exception */
    CSI_ENUM_INSTRUCTION_ACCESS_FAULT_EXCEPTION, /* Instruction access fault exception */
    CSI_ENUM_ILLEGAL_INSTRUCTION_EXCEPTION, /* Illegal instruction exception */
    CSI_ENUM_BREAKPOINT_EXCEPTION, /* Breakpoint exception */
    CSI_ENUM_LOAD_ADDR_MISALIGNED_EXCEPTION, /* Load address misaligned exception */
    CSI_ENUM_LOAD_ACCESS_FAULT_EXCEPTION, /* Load access fault exception */
    CSI_ENUM_STORE_ADDR_MISALIGNED_EXCEPTION, /* Store address misaligned exception */
    CSI_ENUM_STORE_ACCESS_FAULT_EXCEPTION, /* Store access fault exception */
    CSI_ENUM_ECALL_FROM_UMODE, /* ECALL from User mode */
    CSI_ENUM_ECALL_FROM_MMODE, /* ECALL from Machine mode */
    CSI_ENUM_INST_PAGE_FAULT, /* Instruction page-fault */
    CSI_ENUM_LOAD_PAGE_FAULT, /* Load page-fault */
    CSI_ENUM_STORE_PAGE_FAULT, /* Store page-fault */
    CSI_NUM_STANDARD_TRAP_SOURCES, /* Must come last in this list */
} csi_trap_source_t;


#endif /* CSI_HL_INTERRUPT_SOURCES_H */ 
