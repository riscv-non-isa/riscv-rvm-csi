/*
 * Standard Type Definitions
 *
 * General-purpose definitions such as return codes.
 *
 * Copyright (c) RISC-V International 2023. Creative Commons License. Auto-
 * generated file: DO NOT EDIT
 */

#ifndef CSI_TYPES_H
#define CSI_TYPES_H

/*
 * Return type for functions indicating status
 */
typedef enum {
    CSI_SUCCESS = 0, /* Operation completed successfully */
    CSI_ERROR = -1, /* Generic error code */
    CSI_BUSY = -2, /* Device busy, operation could not be completed. */
    CSI_TIMEOUT = -3, /* Operation timed out. */
    CSI_NOT_IMPLEMENTED = -4, /* Functionality not implemented */
    CSI_NOT_INITIALIZED = -5, /* Sub-system not correctly initialised, operation failed. */
    CSI_OUT_OF_MEM = -6, /* Out-of-memory error */
} csi_status_t;

/*
 * Function prototype for the user's timeout callback function (M-mode or U-mode)
 *
 * @param callback_context: Context pointer that was passed into csi_set_timeout
 */
typedef void (csi_timeout_callback_t)(void *callback_context);

/*
 * Function prototype for the user's trap handler (M-mode or U-mode)
 *
 * @param source: Enumerated interrupt source for which the handler was registered.
 * @param isr_ctx: User's context pointer as supplied when the handler was
 * registered.
 * @param mtval: Contents of the mtval register associated with this trap.
 */
typedef void (csi_isr_t)(int source, void *isr_ctx, unsigned long mtval);

/*
 * Function prototype for a delay function to be used when polling.  In non-
 * threaded systems this function could just contain noops, so the system will
 * busy-wait.  In threaded systems this may implement a timed delay which allows
 * other processes to execute while waiting.
 *
 * @param wait_time_us: Wait time in microseconds.  This may be ignored if the wait
 * is implemented as noops.
 */
typedef void (csi_poll_wait_wrapper_t)(unsigned wait_time_us);


#endif /* CSI_TYPES_H */ 
