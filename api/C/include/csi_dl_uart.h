/*
 * UART Support
 *
 * Control of UART (Universal Asynchronous Receiver/Transmitter) or similar
 * communications devices.
 *
 * In hardware terminology, Synchronous / Asynchronous refers to whether a clock is
 * transmitted with the data. In software terminology, the same terms are used to
 * refer to whether a function waits for completion of a transmission or reception
 * of data.  In the following text, the software terminolgy is used.
 *
 * Copyright (c) RISC-V International 2022. Creative Commons License. Auto-
 * generated file: DO NOT EDIT
 */

#ifndef CSI_DL_UART_H
#define CSI_DL_UART_H

#include "csi_dl_bsp_uart.h"
#include "csi_types.h"
#include <stdint.h>

/*
 * Function prototype for a generic function used to register an ISR (Interrupt
 * Service Routine).  In systems that use the RVM-CSI high-level interrupt
 * subsystem, this will be implemented by csi_register_m_isr.
 *
 * @param irq_system_ctx: Context pointer for the interrupt subsystem.
 * @param isr: Pointer to handler function implementing the ISR; or NULL pointer to
 * un-register a handler. The handler function should be a normal function without
 * the interrupt compiler attribute applied: i.e. it is intended to be called from
 * a base trap handler, not vectored-to directly.
 * @param isr_ctx: Pointer to the user's context.  This will be passed into the
 * handler function when called.
 * @param irq_source: Enumeration of the interrupt source.
 */
typedef csi_status_t (csi_register_isr_wrapper_t)(void *irq_system_ctx, csi_isr_t *isr, void *isr_ctx, int irq_source);

/*
 * Number of data bits in UART Tx / Rx packet
 */
typedef enum {
    UART_DATA_BITS_5, /* 5 data bits */
    UART_DATA_BITS_6, /* 6 data bits */
    UART_DATA_BITS_7, /* 7 data bits */
    UART_DATA_BITS_8, /* 8 data bits */
    UART_DATA_BITS_9, /* 9 data bits */
} csi_uart_data_bits_t;

/*
 * UART parity configuration
 */
typedef enum {
    UART_PARITY_NONE, /* No parity bit */
    UART_PARITY_EVEN, /* Even parity */
    UART_PARITY_ODD, /* Odd parity */
} csi_uart_parity_t;

/*
 * Number of stop bits in UART Tx / Rx packet
 */
typedef enum {
    UART_STOP_BITS_1, /* 1 stop bit */
    UART_STOP_BITS_2, /* 2 stop bits */
    UART_STOP_BITS_1_5, /* 1.5 stop bits */
} csi_uart_stop_bits_t;

/*
 * UART flow control mode.
 */
typedef enum {
    UART_FLOWCTRL_NONE, /* No throttling */
    UART_FLOWCTRL_RTS, /* Ready To Send mode */
    UART_FLOWCTRL_CTS, /* Clear To Send mode */
    UART_FLOWCTRL_RTS_CTS, /* Both Ready To Send and Clear To Send operative */
} csi_uart_flowctrl_t;

/*
 * UART event.
 */
typedef enum {
    UART_EVENT_SEND_COMPLETE, /* Data sending complete */
    UART_EVENT_RECEIVE_COMPLETE, /* Data reception complete */
    UART_EVENT_RECEIVE_FIFO_READABLE, /* Data available in UART Rx FIFO */
    UART_EVENT_BREAK_INTR, /* Data reception interrupted (break signal from sender) */
    UART_EVENT_ERROR_OVERFLOW, /* Data receive FIFO overflowed */
    UART_EVENT_ERROR_PARITY, /* Data reception parity error */
    UART_EVENT_ERROR_FRAMING, /* Framing error (no effective stop bit) */
    UART_EVENT_TX_ABORT, /* Transmission aborted by calling csi_uart_stop_tx */
    UART_EVENT_RX_ABORT, /* Reception aborted by calling csi_uart_stop_rx */
} csi_uart_event_t;

/*
 * Function prototype for user's callback function to be run by the UART driver on
 * UART events.
 *
 * @param callback_ctx: Context pointer that was passed into
 * csi_uart_register_callback.
 * @param event: Event giving rise to the callback.
 * @param bytes_transferred: number of bytes of transfer completed up to this
 * point.
 */
typedef void (csi_uart_callback_t)(void *callback_ctx, csi_uart_event_t event, unsigned bytes_transferred);

/*
 * Structure of information concerning UART state
 */
typedef struct {
    bool readable;
    bool writable;
    bool overrun_error;
    bool parity_error;
    bool framing_error;
    bool tx_fifo_empty;
} csi_uart_state_t;


/*
 * Initialize a UART driver instance and associate it with a physical channel.
 * This function must be called before calling any other functions in this module.
 *
 * @param uart: Context structure for this driver instance: initialized by this
 * function call.  The structure declaration csi_uart_t is published by the BSP in
 * csi_hl_bsp_interrupts.h but should be considered private to the BSP.
 * Application code instantiates this structure and passes in a pointer to it.
 * @param idx: Index of the UART, starting at 0 for the first-numbered device in
 * the system.  The BSP may publish macros giving meaningful names to UART
 * instances.
 * @param irq_reg_fn: Pointer to a function that the BSP code can use to register
 * an interrupt handler to support asynchronous callbacks.  May be NULL in which
 * case asynchronous UART functions are not supported.
 * @param irq_system_ctx: Context pointer for the interrupt subsystem, to pass into
 * irq_reg_fn. May be NULL in which case asynchronous UART functions are not
 * supported.
 * @param source: Enumeration of the interrupt source, to be passed into
 * irq_reg_fn.  The user must select the appropriate value to select the interrupt
 * associated with the UART being initialized. Unused if irq_reg_fn and / or
 * irq_system_ctx are NULL.
 * @param wait_fn: Pointer to a function that the BSP should call to implement a
 * wait while polling.
 * @return : Status of initialisation operation
 */
csi_status_t csi_uart_init(csi_uart_t *uart, unsigned idx, csi_register_isr_wrapper_t *irq_reg_fn, void *irq_system_ctx, int source, csi_poll_wait_wrapper_t *wait_fn);

/*
 * Uninitialize a UART driver instance and free associated context space.
 *
 * @param uart: Context structure for this driver instance, as passed into
 * csi_uart_init.
 * @return : Status of uninitialisation operation
 */
csi_status_t csi_uart_uninit(csi_uart_t *uart);

/*
 * Set UART baud rate (symbols per second)
 *
 * If this function is never called, the rate defaults to 115200 baud.
 *
 * @param uart: Context structure for this driver instance, as passed into
 * csi_uart_init.
 * @param baud: Baud rate
 * @return : Status code
 */
csi_status_t csi_uart_baud(csi_uart_t *uart, unsigned baud);

/*
 * Set UART data Tx/Rx format.
 *
 * If this function is never called, the format defaults to 8 data bits, no parity,
 * 1 stop bit.
 *
 * @param uart: Context structure for this driver instance, as passed into
 * csi_uart_init.
 * @param data_bits: Number of data bits in a packet
 * @param parity: Parity configuration
 * @param stop_bits: Number of stop bits in a packet
 * @return : Status code
 */
csi_status_t csi_uart_format(csi_uart_t *uart, csi_uart_data_bits_t data_bits, csi_uart_parity_t parity, csi_uart_stop_bits_t stop_bits);

/*
 * Set UART flow control (throttling) mode.
 *
 * If this function is never called, the default is to use no flow control.
 *
 * @param uart: Context structure for this driver instance, as passed into
 * csi_uart_init.
 * @param flowctrl: Flow control mode
 * @return : Status code
 */
csi_status_t csi_uart_flowctrl(csi_uart_t *uart, csi_uart_flowctrl_t flowctrl);

/*
 * Register or unregister a callback function to respond to UART events.  This
 * function must be called prior to sending or receiving data in asynchronous mode,
 * but need not be called if only synchronous operation is required.  This function
 * will fail and return CSI_ERROR if a NULL value for the irq_reg_fn parameter was
 * passed to csi_uart_init.
 *
 * After registering a callback, it will be called to flag
 * UART_EVENT_RECEIVE_FIFO_READABLE when any data is received.
 *
 * @param uart: Context structure for this driver instance, as passed into
 * csi_uart_init.
 * @param callback: User's callback function.  Passing NULL for this parameter will
 * unregister any previously registered handler.
 * @param callback_ctx: User's context pointer which will be passed into the
 * callback function when it is run.
 * @return : Status code
 */
csi_status_t csi_uart_register_callback(csi_uart_t *uart, csi_uart_callback_t *callback, void *callback_ctx);

/*
 * Send data via UART in synchronous mode.  This function will block until the data
 * has been sent, or until the specified timeout is exceeded, during which time it
 * will poll, repeatedly calling the wait function passed as the wait_fn argument
 * into csi_uart_init.  Synchronous mode can be used in the case where the
 * irq_reg_fn argument to csi_uart_init was set to NULL, and where no callback
 * function has been registered (with csi_uart_register_callback).
 *
 * @param uart: Context structure for this driver instance, as passed into
 * csi_uart_init.
 * @param data: Address of buffer of data to send.  Data will be transmitted in
 * byte order.
 * @param size_bytes: Data length in bytes to send.
 * @param timeout: Timeout for sending a single byte of data, in ms.  If this
 * timeout is exceeded then data send will be aborted and the function will return,
 * but this does not constitute an error.
 * @return : Returns the number of bytes sent, or an error code from csi_status_t.
 */
int csi_uart_send(csi_uart_t *uart, void *data, unsigned size_bytes, unsigned timeout);

/*
 * Send data via UART in asynchronous mode.  Prior to using this function, a
 * callback handler must have been registered using csi_uart_register_callback.
 * This function will return immediately without blocking; completion (or failure)
 * of the transmission will be signalled via the user's callback function.
 *
 * @param uart: Context structure for this driver instance, as passed into
 * csi_uart_init.
 * @param data: Address of buffer of data to send.  Data will be transmitted in
 * byte order.
 * @param size_bytes: Data length in bytes to send.
 * @return : Status code
 */
csi_status_t csi_uart_send_async(csi_uart_t *uart, void *data, unsigned size_bytes);

/*
 * Receive data from UART in synchronous mode.  This function will block until the
 * requested length of data has been received, or until the specified timeout is
 * exceeded, during which time it may poll, repeatedly calling the wait function
 * passed as the wait_fn argument into csi_uart_init. Synchronous mode can be used
 * in the case where the irq_reg_fn argument to csi_uart_init was set to NULL, and
 * where no callback function has been registered (with
 * csi_uart_register_callback).
 *
 * @param uart: Context structure for this driver instance, as passed into
 * csi_uart_init.
 * @param data: Address of buffer for data receiption.  Received data will be
 * filled in byte order.
 * @param size_bytes: Data length in bytes to receive.
 * @param timeout: Timeout for receiving a single byte of data, in ms.   If this
 * timeout is exceeded then data reception will be aborted and the function will
 * return, but this does not constitute an error.
 * @return : Returns the number of bytes received, or an error code from
 * csi_status_t.
 */
int csi_uart_receive(csi_uart_t *uart, void *data, unsigned size_bytes, unsigned timeout);

/*
 * Receive data from UART in asynchronous mode.  Prior to using this function, a
 * callback handler must have been registered using csi_uart_register_callback.
 * This function will return immediately without blocking; completion (or failure)
 * of data reception will be signalled via the user's callback function.
 *
 * @param uart: Context structure for this driver instance, as passed into
 * csi_uart_init.
 * @param data: Address of buffer for data receiption.  Received data will be
 * filled in byte order.
 * @param size_bytes: Data length in bytes to receive.
 * @return : Status code
 */
csi_status_t csi_uart_receive_async(csi_uart_t *uart, void *data, unsigned size_bytes);

/*
 * Read a single byte from UART.  If the UART receive FIFO is empty, this function
 * will block and poll (repeatedly calling the wait function passed as the wait_fn
 * argument into csi_uart_init) until a character is received.
 *
 * @param uart: Context structure for this driver instance, as passed into
 * csi_uart_init.
 * @return : Received data byte
 */
uint8_t csi_uart_getc(csi_uart_t *uart);

/*
 * Transmit a single byte via UART. If the UART transmit FIFO is full, this
 * function will block and poll (repeatedly calling the wait function passed as the
 * wait_fn argument into csi_uart_init) until the byte can be transmitted.
 *
 * @param uart: Context structure for this driver instance, as passed into
 * csi_uart_init.
 * @param data: data byte to send
 */
void csi_uart_putc(csi_uart_t *uart, uint8_t data);

/*
 * Cancel any in-progress transmission in asynchronous mode.
 *
 * @param uart: Context structure for this driver instance, as passed into
 * csi_uart_init.
 * @return : Status code
 */
csi_status_t csi_uart_stop_tx(csi_uart_t *uart);

/*
 * Cancel any in-progress reception in asynchronous mode.
 *
 * @param uart: Context structure for this driver instance, as passed into
 * csi_uart_init.
 * @return : Status code
 */
csi_status_t csi_uart_stop_rx(csi_uart_t *uart);

/*
 * Get UART state information
 *
 * @param uart: Context structure for this driver instance, as passed into
 * csi_uart_init.
 * @param state: Pointer to structure to be filled with state information
 * @return : Status code
 */
csi_status_t csi_uart_get_state(csi_uart_t *uart, csi_uart_state_t *state);

/*
 * Throttle any in-progress reception (normally by setting CTS inactive). This
 * should be called only if hardware flow control has been enabled.
 *
 * @param uart: Context structure for this driver instance, as passed into
 * csi_uart_init.
 * @return : Status code
 */
csi_status_t csi_uart_throttle_rx(csi_uart_t *uart);

/*
 * Un-throttle any in-progress reception (normally by setting CTS active). This
 * should be called only if hardware flow control has been enabled.
 *
 * @param uart: Context structure for this driver instance, as passed into
 * csi_uart_init.
 * @return : Status code
 */
csi_status_t csi_uart_unthrottle_rx(csi_uart_t *uart);


#endif /* CSI_DL_UART_H */ 
