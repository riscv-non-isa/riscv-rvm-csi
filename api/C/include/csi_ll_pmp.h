/*
 * Low-Level PMP API
 *
 * This module provides functionality for PMP configuration.
 *
 * Copyright (C) 2017-2023 Alibaba Group Holding Limited
 */

#ifndef CSI_LL_PMP_H
#define CSI_LL_PMP_H

#include "csi_types.h"
#include <stdint.h>

/*
 * Types of address Matching
 */
typedef enum {
    ADDRESS_MATCHING_OFF   = 0,	/* Null region (disabled) */
    ADDRESS_MATCHING_TOR   = 1, /* Top of range */
    ADDRESS_MATCHING_NA4   = 2, /* Naturally aligned four-byte region */
    ADDRESS_MATCHING_NAPOT = 3, /* Naturally aligned power-of-two region, ≥8 bytes */
} csi_addr_matching_t;

/*
 * Attribution of PMP coniguration
 */
typedef struct {
    bool flag_r; /* The flag of readable. 0: not readable; 1: readable */
    bool flag_w; /* The flag of writeable. 0: not writeable; 1: writeable */
    bool flag_x; /* The flag of execable. 0: not execable; 1: execable */
    csi_addr_matching_t mode; /* Address matching mode */
    bool lock; /* The flag of lock,  0: not be locked; 1 : be locked */
} csi_mpu_region_attr_t;

/*
 * Configure PMP range.
 *
 * This function only can be called in M-mode. If it is called
 * in other mode, it may return an error or cause carsh. When this function is called
 * with success, it will not be effective untill the core has been exit M-mode.
 *
 * @param idx: The index of PMP config entries
 * @param base_addr: The base address of this range need to be configured
 * @param size: The size of this range need to be configured
 * @param attr: A point to the attribution of this range to be configured
 * @param enable: Eanble flag to indicate whether this ranged can be accessed or not
 * @return : Status code
 */
csi_status_t csi_mpu_config_region(unsigned int idx, unsigned long base_addr,
                                   unsigned long size, mpu_region_attr_t *attr,
                                   unsigned int enable);

/*
 * Disable PMP range by index.
 *
 * This function only can be called in M-mode. If it is called
 * in other mode, it may return an error or cause carsh. When this function is called
 * with success, it will not be effective untill the core has been exit M-mode.
 *
 * @param idx: The index of PMP config entries
 * @return : Status code
 */
csi_status_t csi_mpu_disable_region(unsigned int idx);

#endif /* CSI_LL_PMP_H */
