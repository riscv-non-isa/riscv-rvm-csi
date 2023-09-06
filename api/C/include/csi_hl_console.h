/*
 * High-Level "Console" API
 *
 * This module provides csi_uprintf which is a replacement for the printf C library
 * function, aimed at the requirements of embedded systems.  printf assumes the
 * presence of a "console", which is not always present in embedded systems.
 * csi_uprintf can output formatted text to 3 different places: semihosting (which
 * requires a debugger connection), UART (via the RVM-CSI UART subsystem), or a
 * circular buffer in memory. If none of these are available, for example in a
 * production system, csi_uprintf calls can easily be  nullified, effectively
 * removing them from the code.  In addition, csi_uprintf supports a limited set of
 * formatting options, giving it a much smaller code footprint than printf.
 * Finally, csi_uprintf does not require the presence of a heap; some fixed-size
 * storage for string formatting is instead allocated on the stack.
 *
 * The choice of output destination is determined by a CSI_UPRINTF_OUTPUT macro
 * which should be set by application writers.
 *
 * In addition, macros are provided (CSI_LOG_INFO, CSI_LOG_WARN and CSI_LOG_ERR)
 * which may be used for logging text, via csi_uprintf, at different logging levels
 * (info, warn and error respectively).  The logging level can be set by
 * application writers using a CSI_LOG_LEVEL macro, which then enables only
 * messages at the desired logging levels.
 *
 * There is assumed to be a single "console" for the entire system; i.e. in a
 * multi-threaded or multi-core system, all threads and cores output text via the
 * same mechanism.  This means that BSP implementations of the functions in this
 * module may use static data, and the functions are not re-entrant.
 * Implementation of any locks necessary to prevent overlapping csi_uprintf calls
 * from different threads is left as the responsibility of the application writer.
 *
 * See section "csi_uprintf Format Conversions" section in the RVM-CSI API
 * documentation for more detailed information on the format tags supported by
 * csi_uprintf.
 *
 * Copyright (c) RISC-V International 2023. Creative Commons License. Auto-
 * generated file: DO NOT EDIT
 */

#ifndef CSI_HL_CONSOLE_H
#define CSI_HL_CONSOLE_H

#include "csi_dl_bsp_uart.h"
#include "csi_types.h"

/*
 * Configure a circular buffer for use by csi_uprintf when CSI_UPRINTF_OUTPUT is
 * defined as CSI_UPRINTF_CIRCBUFF.  This function must be called once, before ever
 * calling csi_uprintf in that case, and before ever calling csi_uprintf_circbuff.
 * It does not need to be called if not using circular buffer output mode.
 *
 * @param buff: Pointer to circular buffer to use for console output.
 * @param size_bytes: Circular buffer size in bytes.
 * @return : Status of operation
 */
csi_status_t csi_set_uprintf_circbuff(void *buff, unsigned size_bytes);

/*
 * Configure csi_uprintf operation for use when CSI_UPRINTF_OUTPUT is defined as
 * CSI_UPRINTF_UART.  This function must be called once, before ever calling
 * csi_uprintf in that case, and before ever calling csi_uprintf_uart.  It does not
 * need to be called if not using UART output mode.
 *
 * @param uart: Pointer to a UART object which must be initialized (through a call
 * to csi_uart_init) prior to use.
 * @return : Status of operation
 */
csi_status_t csi_set_uprintf_uart(csi_uart_t *uart);

/*
 * Semi-hosting version of csi_uprintf.  This is not normally called directly by
 * application writers. Rather, call csi_uprintf and define CSI_UPRINTF_OUTPUT to
 * be CSI_UPRINTF_SEMIHOST.
 *
 * @param fmt: Formatted string matching that used by printf
 * @return : Number of characters printed.  A negative number indicates an error.
 */
int csi_uprintf_semihost(char const *fmt, ...) __attribute__((format(printf, 1, 2)));

/*
 * Circular buffering version of csi_uprintf.  This is not normally called directly
 * by application writers. Rather, call csi_uprintf and define CSI_UPRINTF_OUTPUT
 * to be CSI_UPRINTF_CIRCBUFF.
 *
 * @param fmt: Formatted string matching that used by printf
 * @return : Number of characters printed.  A negative number indicates an error.
 */
int csi_uprintf_circbuff(char const *fmt, ...) __attribute__((format(printf, 1, 2)));

/*
 * UART version of csi_uprintf.  This is not normally called directly by
 * application writers. Rather, call csi_uprintf and define CSI_UPRINTF_OUTPUT to
 * be CSI_UPRINTF_UART.
 *
 * @param fmt: Formatted string matching that used by printf
 * @return : Number of characters printed.  A negative number indicates an error.
 */
int csi_uprintf_uart(char const *fmt, ...) __attribute__((format(printf, 1, 2)));


/*
 * Max number of characters that can be printed by a single csi_uprintf call.  This
 * determines the amount of space that the function call will occupy on the stack
 */
#define CSI_UPRINTF_MAX_CHARS (80)

/*
 * Enumerate values for CSI_UPRINTF_OUTPUT
 */
#define CSI_UPRINTF_NONE 0
#define CSI_UPRINTF_SEMIHOST 1
#define CSI_UPRINTF_UART 2
#define CSI_UPRINTF_CIRCBUFF 3

/*
 * Enumerate values for CSI_LOG_LEVEL
 */
#define CSI_LOG_LEVEL_NONE 0
#define CSI_LOG_LEVEL_ERR 1
#define CSI_LOG_LEVEL_WARN 2
#define CSI_LOG_LEVEL_INFO 3

/*
 * Default value for CSI_UPRINTF_OUTPUT
 */
#ifndef CSI_UPRINTF_OUTPUT
#define CSI_UPRINTF_OUTPUT CSI_UPRINTF_SEMIHOST
#endif

/*
 * Default value for CSI_LOG_LEVEL
 */
#ifndef CSI_LOG_LEVEL
#define CSI_LOG_LEVEL CSI_LOG_LEVEL_INFO
#endif

/*
 * CSI_UPRINTF_OUTPUT determines the output destination of formatted printing using
 * cs_uprintf (Semi-hosting, UART, circular buffer or nowhere).  If
 * CSI_UPRINTF_OUTPUT is defined as CSI_UPRINTF_NONE, all csi_uprintf calls are
 * removed from the code.
 */
#if (CSI_UPRINTF_OUTPUT == CSI_UPRINTF_NONE)
#define csi_uprintf(...)
#elif (CSI_UPRINTF_OUTPUT == CSI_UPRINTF_SEMIHOST)
#define csi_uprintf csi_uprintf_semihost
#elif (CSI_UPRINTF_OUTPUT == CSI_UPRINTF_UART)
#define csi_uprintf csi_uprintf_uart
#elif (CSI_UPRINTF_OUTPUT == CSI_UPRINTF_CIRCBUFF)
#define csi_uprintf csi_uprintf_circbuff
#endif

/*
 * The CSI_LOG_ERR, CSI_LOG_WARN and CSI_LOG_INFO macros are provided to support
 * logging at different "log levels" controlled by CSI_LOG_LEVEL.  When the log
 * level is defined as CSI_LOG_LEVEL_INFO, all the CSI_LOG_ERR, CSI_LOG_WARN and
 * CSI_LOG_INFO macros are operational. When the log level is defined as
 * CSI_LOG_LEVEL_WARN, only the error and warning macros are operational.  When the
 * log level is CSI_LOG_LEVEL_ERR, only the CSI_LOG_ERR macro is operational.  And
 * when set to CSI_LOG_LEVEL_NONE, all 3 macros are removed from the code.
 */
#if (CSI_LOG_LEVEL == CSI_LOG_LEVEL_ERR)
#define CSI_ENABLE_ERR_LOGGING  (1)
#elif (CSI_LOG_LEVEL == CSI_LOG_LEVEL_WARN)
#define CSI_ENABLE_ERR_LOGGING  (1)
#define CSI_ENABLE_WARN_LOGGING  (1)
#elif (CSI_LOG_LEVEL == CSI_LOG_LEVEL_INFO)
#define CSI_ENABLE_ERR_LOGGING  (1)
#define CSI_ENABLE_WARN_LOGGING  (1)
#define CSI_ENABLE_INFO_LOGGING  (1)
#endif

#ifdef CSI_ENABLE_ERR_LOGGING
#define CSI_LOG_ERR csi_uprintf
#else
#define CSI_LOG_ERR(...)
#endif

#ifdef CSI_ENABLE_WARN_LOGGING
#define CSI_LOG_WARN csi_uprintf
#else
#define CSI_LOG_WARN(...)
#endif

#ifdef CSI_ENABLE_INFO_LOGGING
#define CSI_LOG_INFO csi_uprintf
#else
#define CSI_LOG_INFO(...)
#endif


#endif /* CSI_HL_CONSOLE_H */ 
