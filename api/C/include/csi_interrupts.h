/*
 * Trap Handling / Interrupt and Timer Support
 *
 * This module provides functionality for routing interrupts and registering
 * handlers for traps, controlling the system timer, and managing timed events. The
 * BSP provides a base trap handler.  Running the initialisation function
 * csi_interrupts_init will put the address of this handler in the mtvec register.
 * This base trap handler provides default exception handling, and calls any
 * handlers registered by the user. Systems using RVM-CSI may provide their own
 * trap handling support, in which case csi_interrupts_init should not be called,
 * and all functions in this module will be inoperative.
 *
 * There will be at most one instance of the trap handling and interrupt subsystem
 * per hart.
 * The base trap handler will attempt to inform users of unhandled exceptions via
 * the RVM-CSI console API.  Other unhandled interrupts will be ignored.
 *
 * Copyright (c) RISC-V International 2022. Creative Commons License. Auto-
 * generated file: DO NOT EDIT
 */

#ifndef CSI_INTERRUPTS_H
#define CSI_INTERRUPTS_H

#include "csi_defs.h"
#include "csi_bsp_interrupts.h"

/*
 * Function prototype for the user's trap handler (M-mode or U-mode)
 *
 * @param source: Enumerated interrupt source for which the handler was registered.
 * @param isr_ctx: User's context pointer as supplied when the handler was
 * registered.
 * @param mtval: Contents of the mtval register associated with this trap.
 */
typedef void (csi_isr_t)(int source, void *isr_ctx, unsigned mtval);

/*
 * Function prototype for the user's timeout callback function (M-mode or U-mode)
 *
 * @param timeout_handle: Handle for this timeout instance, as passed into
 * csi_set_timeout
 * @param callback_context: Context pointer that was passed into csi_set_timeout
 */
typedef void (csi_timeout_callback_t)(csi_timeout_t *timeout_handle, void *callback_context);


/*
 * Initialize interrupt and timer sub-system for this hart.  Must be called before
 * calling any other functions in this module.  This function must run in machine
 * mode.  Following initialization, all interrupt sources (except exceptions and
 * NMI) are disabled.  To enable a source, first register a handler, using
 * csi_register_m_isr or csi_register_u_isr; then enable the source with
 * csi_enable_m_trap_source or csi_enable_u_trap_source.
 *
 * @param mctx: Pointer to an area of memory to be used as M-mode context space for
 * the interrupt subsystem.  The BSP will define a macro
 * CSI_INTERRUPT_MCTX_MIN_SIZE_BYTES which defines the minimum required size of
 * this context space in bytes.  Calling this function initializes the context
 * space.
 * @param mctx_size: Size of memory allocated at the mctx pointer in bytes.
 * Allocating more than CSI_INTERRUPT_MCTX_MIN_SIZE_BYTES may permit additional
 * M-mode handlers to be registered.
 * @return : Status of initialisation operation
 */
csi_status_t csi_interrupts_init(void *mctx, unsigned mctx_size);

/*
 * Un-initialize the interrupt and timer sub-system.  After calling this function,
 * the memory allocated for the contexts may be freed, and no other functions in
 * this module may be called.  This function must run in machine mode.
 *
 * @param mctx: M-mode context pointer previously initialised by
 * csi_interrupts_init.
 * @return : Status of uninitialisation operation
 */
csi_status_t csi_interrupts_uninit(void *mctx);

/*
 * This function is run from U-mode to obtain a handle that can be used to
 * reference the interrupt sub-system for the current hart.  (This will usually be
 * the address of mctx as passed into csi_interrupts_init, which can be de-
 * referenced by M-mode code but not U-mode).
 *
 * @return : IRQ system handle, or 0 if inapplicable.
 */
unsigned get_interrupts_u_handle(void);

/*
 * Registers a user-supplied function (isr) that will be called in M-mode by the
 * base trap handler on receipt of a trap arising from from the source signal.
 * isr_ctx is a parameter that will be passed into the user's ISR along with the
 * signal number and mtval contents.  This function transparently deals with
 * routing the desired signal to the hart and enabling the interrupt.  This
 * function must run in machine mode.  Running this function with a NULL pointer
 * for the isr parameter will un-register a previously registered handler.
 *
 * @param mctx: M-mode context pointer previously initialised by
 * csi_interrupts_init.
 * @param isr: Pointer to the user's handler function; or NULL pointer to un-
 * register a handler.
 * @param isr_ctx: Pointer to the user's context.  This will be passed into the
 * handler function when called.
 * @param source: Enumerated interrupt / exception source.  This must be one of the
 * enumerations from csi_trap_source_t, or a value from the BSP (extending the
 * enumerations in csi_trap_source_t) enumerating a platform-specific external
 * interrupt source.
 * @return : Status of ISR registration operation.  CSI_ERROR will be returned if
 * the source parameter is invalid.  CSI_NOT_INITIALIZED will be returned if
 * csi_interrupts_init has not been called prior to calling this function, or if
 * csi_interrupts_uninit was called prior to calling this function.  CSI_OUT_OF_MEM
 * will be returned if the number of handlers registered exceeds that supported by
 * the context size.
 */
csi_status_t csi_register_m_isr(void *mctx, csi_isr_t *isr, void *isr_ctx, int source);

/*
 * Registers a user-supplied function (isr) that will be called in U-mode by the
 * base trap handler on receipt of a trap arising from from the source signal.
 * isr_ctx is a parameter that will be passed into the user's ISR along with the
 * signal number and mtval contents.  This function transparently deals with
 * routing the desired signal to the hart and enabling the interrupt.  This
 * function must run in user mode.  Running this function with a NULL pointer for
 * the isr parameter will un-register a previously registered handler.  Note that
 * before registering a user mode handler for a source,
 * csi_set_umode_trap_permissions must have been run (from machine mode) in order
 * to enable handling of the trap in U-mode.
 *
 * @param irq_system_handle: Handle for the interrupt sub-system on this hart,
 * obtained by running get_interrupts_u_handle
 * @param isr: Pointer to the user's handler function; or NULL pointer to un-
 * register a handler
 * @param isr_ctx: Pointer to the user's context.  This will be passed into the
 * handler function when called.
 * @param source: Enumerated interrupt / exception source.  This must be one of the
 * enumerations from csi_trap_source_t, or a value from the BSP (extending the
 * enumerations in csi_trap_source_t) enumerating a platform-specific external
 * interrupt source.
 * @return : Status of ISR registration operation.  CSI_ERROR will be returned if
 * the source parameter is invalid, or if the requested source has not been enabled
 * for u-mode handling by first running  csi_set_umode_trap_permissions .
 * CSI_NOT_INITIALIZED will be returned if csi_interrupts_init has not been called
 * prior to calling this function, or if csi_interrupts_uninit was called prior to
 * calling this function. CSI_OUT_OF_MEM will be returned if the number of handlers
 * registered exceeds that supported by the context size.
 */
csi_status_t csi_register_u_isr(unsigned irq_system_handle, csi_isr_t *isr, void *isr_ctx, int source);

/*
 * Disable a trap source.  This function must be run in machine mode.  Note that
 * this only gates an interrupting signal within the interrupt control logic.
 * Device drivers may offer separate controls for turning their interrupts on and
 * off at source.
 *
 * @param mctx: M-mode context pointer previously initialised by
 * csi_interrupts_init.
 * @param source: Enumerated interrupt / exception source.  This must be one of the
 * enumerations from csi_trap_source_t, or a value from the BSP (extending the
 * enumerations in csi_trap_source_t) enumerating a platform-specific external
 * interrupt source.
 * @return : Status of disable operation.  CSI_ERROR will be returned if the source
 * parameter is invalid.
 */
csi_status_t csi_disable_m_trap_source(void *mctx, int source);

/*
 * Enable a trap source.  This function must be run in machine mode.  Note that
 * this only gates an interrupting signal within the interrupt control logic.
 * Device drivers may offer separate controls for turning their interrupts on and
 * off at source.
 *
 * @param mctx: M-mode context pointer previously initialised by
 * csi_interrupts_init.
 * @param source: Enumerated interrupt / exception source.  This must be one of the
 * enumerations from csi_trap_source_t, or a value from the BSP (extending the
 * enumerations in csi_trap_source_t) enumerating a platform-specific external
 * interrupt source.
 * @return : Status of enable operation.  CSI_ERROR will be returned if the source
 * parameter is invalid.
 */
csi_status_t csi_enable_m_trap_source(void *mctx, int source);

/*
 * Disable handling of a trap arising from a particular source.  This function must
 * be run in user mode. It will only have an effect if a user-mode handler has
 * previously been registered for the selected source. Note this only disables the
 * running of the handler.  Device drivers may offer separate controls for turning
 * their interrupts on and off at source.
 *
 * @param irq_system_handle: Handle for the interrupt sub-system on this hart,
 * obtained by running get_interrupts_u_handle
 * @param source: Enumerated interrupt / exception source.  This must be one of the
 * enumerations from csi_trap_source_t, or a value from the BSP (extending the
 * enumerations in csi_trap_source_t) enumerating a platform-specific external
 * interrupt source.
 * @return : Status of disable operation.  CSI_ERROR will be returned if the source
 * parameter is invalid.
 */
csi_status_t csi_disable_u_trap_source(unsigned irq_system_handle, int source);

/*
 * Enable handling of a trap arising from a particular source.  This function must
 * be run in user mode. It will only have an effect if a user-mode handler has
 * previously been registered for the selected source. Note this only enables the
 * running of the handler.  Device drivers may offer separate controls for turning
 * their interrupts on and off at source.
 *
 * @param irq_system_handle: Handle for the interrupt sub-system on this hart,
 * obtained by running get_interrupts_u_handle
 * @param source: Enumerated interrupt / exception source.  This must be one of the
 * enumerations from csi_trap_source_t, or a value from the BSP (extending the
 * enumerations in csi_trap_source_t) enumerating a platform-specific external
 * interrupt source.
 * @return : Status of enable operation.  CSI_ERROR will be returned if the source
 * parameter is invalid.
 */
csi_status_t csi_enable_u_trap_source(unsigned irq_system_handle, int source);

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
 * into another call to csi_set_interrupt_enables in order to restore the previous
 * interrupt enables state.
 */
unsigned csi_set_interrupt_enables(unsigned int_enables);

/*
 * Informs the interrupt subsystem whether a given signal is allowed to be handled
 * in user mode.  This function must be run in machine mode.  Any signals for which
 * this function is not run will not be permitted to be handled in U-mode.
 *
 * @param mctx: M-mode context pointer previously initialised by
 * csi_interrupts_init.
 * @param source: Enumerated interrupt / exception source.  This must be one of the
 * enumerations from csi_trap_source_t, or a value from the BSP (extending the
 * enumerations in csi_trap_source_t) enumerating a platform-specific external
 * interrupt source.
 * @param permitted: True if user mode is permitted to register a handler for this
 * trap source.  False if not permitted.
 * @return : Status of permissions setting operation.  CSI_ERROR will be returned
 * if the source parameter is invalid.
 */
csi_status_t csi_set_umode_trap_permissions(void *mctx, int source, bool permitted);

/*
 * Allocates a signal number for software use.  The signal number must be outside
 * the range currently reserved by the BSP, otherwise an error is raised.  When
 * raised, the signal will be routed to the specified hart ID.  Must be run in
 * machine mode.
 *
 * @param mctx: M-mode context pointer previously initialised by
 * csi_interrupts_init.
 * @param signal: Source enumeration for this signal.  A unique number >=
 * CSI_TOTAL_BSP_TRAP_SOURCES must be chosen.
 * @param hartid: ID of the hart to which this signal will be routed.
 * @return : Status of operation.  CSI_ERROR will be returned if the source
 * parameter is invalid.
 */
csi_status_t csi_define_sw_signal(void *mctx, int signal, int hartid);

/*
 * Raises a software signal, previously defined using csi_define_sw_signal.  Must
 * be run in machine mode.
 *
 * @param mctx: M-mode context pointer previously initialised by
 * csi_interrupts_init.
 * @param signal: Source enumeration for this signal.
 * @return : Status of operation.  CSI_ERROR will be returned if the specified
 * signal is not software-raisable.
 */
csi_status_t csi_raise_m_sw_signal(void *mctx, int signal);

/*
 * Raises a software signal, previously defined using csi_define_sw_signal.  Must
 * be run in user mode. User-mode permissions for the selected signal must have
 * been set by previously running csi_set_umode_trap_permissions.
 *
 * @param irq_system_handle: Handle for the interrupt sub-system on this hart,
 * obtained by running get_interrupts_u_handle
 * @param signal: Source enumeration for this signal.
 * @return : Status of operation.  CSI_ERROR will be returned if the specified
 * signal is not software-raisable.
 */
csi_status_t csi_raise_u_sw_signal(unsigned irq_system_handle, int signal);

/*
 * Set priority for an interrupt source.  Priorities range from 0 to
 * CSI_MAX_INTERRUPT_PRIORITY, where 0 indicates "never interrupt", and 1 is the
 * lowest subsequent priority level. This function must be run in machine mode.
 *
 * @param mctx: M-mode context pointer previously initialised by
 * csi_interrupts_init.
 * @param signal: Source enumeration for this signal.
 * @param priority: priority level.
 * @return : Status of operation.  CSI_ERROR will be returned if the request is
 * invalid.
 */
csi_status_t csi_set_irq_priority(void *mctx, int signal, int priority);

/*
 * Get priority of an interrupt source.  Must be run in machine mode.
 *
 * @param mctx: M-mode context pointer previously initialised by
 * csi_interrupts_init.
 * @param signal: Source enumeration for this signal.
 * @return : priority level (resulting from calls to csi_set_irq_priority).  -1
 * will be returned if the request is invalid.
 */
int csi_get_irq_priority(void *mctx, int signal);

/*
 * Set a priority threshold below which interrupts will be masked.  Must be run in
 * machine mode.
 *
 * @param mctx: M-mode context pointer previously initialised by
 * csi_interrupts_init.
 * @param threshold: Priority threshold.
 * @return : Status of operation.  CSI_ERROR will be returned if the request is
 * invalid.
 */
csi_status_t csi_set_irq_priority_thresh(void *mctx, int threshold);

/*
 * Get the current priority threshold for interrupts  Must be run in machine mode.
 *
 * @param mctx: M-mode context pointer previously initialised by
 * csi_interrupts_init.
 * @return : Priority threshold, or -1 if priority thresholds unsupported, or on
 * error.
 */
int csi_get_irq_priority_thresh(void *mctx);

/*
 * Force an external interrupt (e.g. for testing purposes).  Must be run in machine
 * mode.
 *
 * @param mctx: M-mode context pointer previously initialised by
 * csi_interrupts_init.
 * @param signal: Source enumeration for this signal.
 */
void csi_force_ext_irq(void *mctx, int signal);

/*
 * Set the frequency of the system timer.  Note that there is typically a single
 * timer for all harts in the system, so this function can affect the operation of
 * other harts.  The timer will be compared against a compare register for each
 * hart in order to produce a regular timer interrupt at a tick rate configured
 * using csi_set_timer_tick, which is used for timing purposes. This function must
 * run in machine mode.
 *
 * @param timer_freq_mhz: System timer frequency in MHz
 * @return : Status of operation.  CSI_ERROR will be returned if the request is
 * invalid.
 */
csi_status_t csi_timer_config(unsigned timer_freq_mhz);

/*
 * This function causes the BSP to register a base handler for timer interrupts and
 * to control the value of the mtimecmp register in order to produce a regular
 * timer tick interrupt at the requested frequency.   This base timer handler can
 * then service multiple timed events that may be configured using csi_set_timeout.
 * This function must be run in machine mode.
 *
 * @param mctx: M-mode context pointer previously initialised by
 * csi_interrupts_init.
 * @param tick_period_us: Timer tick period in microseconds.  Longer tick periods
 * put less load on the system (due to servicing interrupts), but limit the
 * accuracy of any timed events. Passing a value of 0 for the tick period turns off
 * timer interrupts.
 * @return : Status of operation.  CSI_ERROR will be returned if the request is
 * invalid.
 */
csi_status_t csi_set_timer_tick(void *mctx, unsigned tick_period_us);

/*
 * Registers a callback function (callback) which will be called after a period set
 * by timeout_ticks unless cancelled with cs_cancel_timeout().  callback_context
 * will be passed into the user's callback function.  The structure pointed to by
 * timeout_handle will be initialised by this function and used as a handle for
 * this timeout instance.  This function must be run in machine mode.
 *
 * @param mctx: M-mode context pointer previously initialised by
 * csi_interrupts_init.
 * @param timeout_handle: Handle for this timeout instance.  The structure
 * declaration csi_timeout_t is published by the BSP in csi_bsp_interrupts.h but
 * should be considered private to the BSP.  Application code instantiates this
 * structure and passes in a pointer to it.
 * @param callback: Pointer to the users callback function, to be called when the
 * timeout expires.
 * @param callback_context: Pointer to the user's context space, which will be
 * passed into the callback function.
 * @param timeout_ticks: Timeout period in ticks (configured by csi_set_timer_tick)
 * @return : Status of operation.
 */
csi_status_t csi_set_m_timeout(void *mctx, csi_timeout_t *timeout_handle, csi_timeout_callback_t *callback, void *callback_context, int timeout_ticks);

/*
 * Registers a callback function (callback) which will be called after a period set
 * by timeout_ticks unless cancelled with cs_cancel_timeout().  callback_context
 * will be passed into the user's callback function.  The structure pointed to by
 * timeout_handle will be initialised by this function and used as a handle for
 * this timeout instance.  This function must be run in user mode.
 *
 * @param irq_system_handle: Handle for the interrupt sub-system on this hart,
 * obtained by running get_interrupts_u_handle
 * @param timeout_handle: Handle for this timeout instance.  The structure
 * declaration csi_timeout_t is published by the BSP in csi_bsp_interrupts.h but
 * should be considered private to the BSP.  Application code instantiates this
 * structure and passes in a pointer to it.
 * @param callback: Pointer to the users callback function, to be called when the
 * timeout expires.
 * @param callback_context: Pointer to the user's context space, which will be
 * passed into the callback function.
 * @param timeout_ticks: Timeout period in ticks (configured by csi_set_timer_tick)
 * @return : Status of operation.
 */
csi_status_t csi_set_u_timeout(unsigned irq_system_handle, csi_timeout_t *timeout_handle, csi_timeout_callback_t *callback, void *callback_context, int timeout_ticks);

/*
 * Cancels a timeout previously configured with csi_set_timeout, using the
 * associated handle. Cancelling a timer that has already expired is not an error.
 * This function may be run in either machine mode or user mode.
 *
 * @param timeout_handle: handle for this timeout instance, previously initialised
 * with csi_set_timeout
 * @return : Status of operation.
 */
csi_status_t csi_cancel_timeout(csi_timeout_t *timeout_handle);


#endif /* CSI_INTERRUPTS_H */ 
