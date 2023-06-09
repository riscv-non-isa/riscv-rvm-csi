/*
 * Standard Type Definitions
 *
 * General-purpose definitions such as return codes.
 *
 * Copyright (c) RISC-V International 2022. Creative Commons License. Auto-
 * generated file: DO NOT EDIT
 */

#ifndef CSI_TYPES_H
#define CSI_TYPES_H

/*
 * Return type for functions indicating status
 */
typedef enum {
    CSI_SUCCESS = 0, /* Operation completed successfully */
    CSI_NOT_IMPLEMENTED = -1, /* Functionality not implemented */
    CSI_ERROR = -2, /* Generic error code */
    CSI_NOT_INITIALIZED = -3, /* Sub-system not correctly initialised, operation failed. */
    CSI_OUT_OF_MEM = -4, /* Out-of-memory error */
} csi_status_t;

/*
 * Function prototype for the user's timeout callback function (M-mode or U-mode)
 *
 * @param callback_context: Context pointer that was passed into csi_set_timeout
 */
typedef void (csi_timeout_callback_t)(void *callback_context);


#endif /* CSI_TYPES_H */ 
