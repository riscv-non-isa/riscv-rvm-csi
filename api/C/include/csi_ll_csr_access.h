/*
 * Low-Level CSR ACCESS API
 *
 * APIs giving access to CSRs.
 *
 * Copyright (c) RISC-V International 2023. Creative Commons License. Auto-
 * generated file: DO NOT EDIT
 */

#ifndef CSI_LL_CSR_ACCESS_H
#define CSI_LL_CSR_ACCESS_H

#include "csi_types.h"
#include <stdint.h>

#ifndef __ASSEMBLER__

/*
 * macros
 */
#define __STR(s)                #s
#define STRINGIFY(s)            __STR(s)
#define __ASM                   __asm


/*
 * Read the content of csr register to __v, then write content of val into csr
 * register, then return __v.
 *
 * @param csr: CSR macro definition
 * @param val: value to store into the CSR register
 * @return : the CSR register value before written
 */
#define csi_csr_swap(csr, val)                                  \
    ({                                                          \
        rv_csr_t __v = (unsigned long)(val);                    \
        __ASM volatile("csrrw %0, " STRINGIFY(csr) ", %1"       \
                    : "=r"(__v)                                \
                    : "rK"(__v)                                \
                    : "memory");                               \
        __v;                                                    \
    })

/*
 * Read the content of csr register to __v and return it.
 *
 * @param csr: CSR macro definition
 * @return : the CSR register value
 */
#define csi_csr_read(csr)                                       \
    ({                                                          \
        rv_csr_t __v;                                           \
        __ASM volatile("csrr %0, " STRINGIFY(csr)               \
                      : "=r"(__v)                                \
                      :                                          \
                      : "memory");                               \
        __v;                                                    \
    })

/*
 * Write the content of val to csr register.
 *
 * @param csr: CSR macro definition
 * @param val: value to store into the CSR register
 */
#define csi_csr_write(csr, val)                                 \
    ({                                                          \
        rv_csr_t __v = (rv_csr_t)(val);                         \
        __ASM volatile("csrw " STRINGIFY(csr) ", %0"            \
                      :                                          \
                      : "rK"(__v)                                \
                      : "memory");                               \
    })

/*
 * Read the content of csr register to __v, then set csr register to be __v | val,
 * then return __v.
 *
 * @param csr: CSR macro definition
 * @param val: Mask value to be used wih csrrs instruction
 * @return : the CSR register value before written
 */
#define csi_csr_read_set(csr, val)                              \
    ({                                                          \
        rv_csr_t __v = (rv_csr_t)(val);                         \
        __ASM volatile("csrrs %0, " STRINGIFY(csr) ", %1"       \
                    : "=r"(__v)                                \
                    : "rK"(__v)                                \
                    : "memory");                               \
        __v;                                                    \
    })

/*
 * Set csr register to be csr_content | val.
 *
 * @param csr: CSR macro definition
 * @param val: Mask value to be used wih csrs instruction
 */
#define csi_csr_set(csr, val)                                   \
    ({                                                          \
        rv_csr_t __v = (rv_csr_t)(val);                         \
        __ASM volatile("csrs " STRINGIFY(csr) ", %0"            \
                    :                                          \
                    : "rK"(__v)                                \
                    : "memory");                               \
    })

/*
 * Read the content of csr register to __v, then set csr register to be __v & ~val,
 * then return __v.
 *
 * @param csr: CSR macro definition
 * @param val: Mask value to be used wih csrrc instruction
 * @return : the CSR register value before written
 */
#define csi_csr_read_clear(csr, val)                            \
    ({                                                          \
        rv_csr_t __v = (rv_csr_t)(val);                         \
        __ASM volatile("csrrc %0, " STRINGIFY(csr) ", %1"       \
                    : "=r"(__v)                                \
                    : "rK"(__v)                                \
                    : "memory");                               \
        __v;                                                    \
    })

/*
 * Set csr register to be csr_content & ~val
 *
 * @param csr: CSR macro definition
 * @param val: Mask value to be used wih csrc instruction
 */
#define csi_csr_clear(csr, val)                                 \
    ({                                                          \
        rv_csr_t __v = (rv_csr_t)(val);                         \
        __ASM volatile("csrc " STRINGIFY(csr) ", %0"            \
                    :                                          \
                    : "rK"(__v)                                \
                    : "memory");                               \
    })

#endif // __ASSEMBLER__

#endif /* CSI_LL_CSR_ACCESS_H */ 
