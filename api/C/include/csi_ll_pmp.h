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
    bool flag_w; /* The flag of writable. 0: not writable; 1: writable */
    bool flag_x; /* The flag of execable. 0: not execable; 1: execable */
    csi_addr_matching_t mode; /* Address matching mode */
    bool lock; /* The flag of lock,  0: not be locked; 1 : be locked */
} csi_pmp_region_attr_t;

/*
 * Get PMP range number for the hard id.
 *
 * If the hard id is invalidate, such as out of support range, it should return
 * an error
 *
 * @param hart_id: The hart id used to get the pmp range number
 * @param range_number: A pointer to store the ouptut value of PMP range number
 * @return : Status code
 */
csi_status_t csi_pmp_get_range_num(unsigned int hart_id, unsigned int *range_number);

/*
 * Configure PMP entry.
 *
 * This function only can be called in M-mode. If it is called
 * in other mode, it may return an error or cause carsh. When this function is called
 * with success, it will not be effective untill the core has been exit M-mode.
 *
 * Also, an error should be returned if the output parameters is invalid, such as idx
 * out of range, address or size is not supported or over overlapped etc.
 *
 * @param idx: The index of PMP config entries
 * @param base_addr: The base address of this entry need to be configured
 * @param size: The size of this entry need to be configured
 * @param attr: A point to the attribution of this entry to be configured
 * @return : Status code
 */
csi_status_t csi_pmp_config_region(unsigned int idx, unsigned long base_addr,
                                   unsigned long size, csi_pmp_region_attr_t *attr);

/*
 * Get PMP entry configure information.
 *
 * This function only can be called in M-mode. If it is called
 * in other mode, it may return an error or cause carsh.
 *
 * Also, an error should be returned if the output parameters is invalid, such as idx
 * out of range.
 *
 * @param idx: The index of PMP config entries
 * @param base_addr: A pointer to the buffer which is used to output the base address
 *                    of this entry
 * @param size: A pointer to the buffer which is used to output the size of this entry
 * @param attr: A point to the buffer which is used to output attribute of this entry
 * @return : Status code
 */
csi_status_t csi_pmp_get_region(unsigned int idx, unsigned long *base_addr,
                                   unsigned long *size, csi_pmp_region_attr_t *attr);

/*
 * Lock PMP entry by index.
 *
 * This function only can be called in M-mode. If it is called
 * in other mode, it may return an error or cause carsh. When this function is called
 * with success, it will not be effective untill the core has been exit M-mode.
 *
 * @param idx: The index of PMP config entries
 * @return : Status code
 */
csi_status_t csi_pmp_lock_region(unsigned int idx);

/*
 * Set PMP entry address and size need to be controlled by index.
 *
 * This function only can be called in M-mode. If it is called
 * in other mode, it may return an error or cause carsh. When this function is called
 * with success, it will not be effective untill the core has been exit M-mode.
 *
 * @param idx: The index of PMP config entries
 * @param base_addr: The base address of this entry need to be configured
 * @param size: The size of this entry need to be configured
 * @return : Status code
 */
csi_status_t csi_pmp_set_address(unsigned int idx, unsigned long base_addr,
                                unsigned long size);

/*
 * Get PMP entry address and size been controlled by index.
 *
 * This function only can be called in M-mode. If it is called
 * in other mode, it may return an error or cause carsh. When this function is called
 * with success, it will not be effective untill the core has been exit M-mode.
 *
 * @param idx: The index of PMP config entries
 * @param base_addr: A pointer to the buffer which is used to output the base address
 *                    of this range
 * @param size: A pointer to the buffer which is used to output the size of this entry
 * @return : Status code
 */
csi_status_t csi_pmp_get_address(unsigned int idx, unsigned long *base_addr,
                                unsigned long *size);

/*
 * Set PMP entry address mode by index.
 *
 * This function only can be called in M-mode. If it is called
 * in other mode, it may return an error or cause carsh. When this function is called
 * with success, it will not be effective untill the core has been exit M-mode.
 *
 * @param idx: The index of PMP config entries
 * @param mode: The address mode of PMP config entries
 * @return : Status code
 */
csi_status_t csi_pmp_set_address_mode(unsigned int idx, csi_addr_matching_t mode);

/*
 * Get PMP entry address mode by index.
 *
 * This function only can be called in M-mode. If it is called
 * in other mode, it may return an error or cause carsh. When this function is called
 * with success, it will not be effective untill the core has been exit M-mode.
 *
 * @param idx: The index of PMP config entries
 * @param mode: A pointer to the buffer which is used to output the address mode of
 *              this entry
 * @return : Status code
 */
csi_status_t csi_pmp_get_address_mode(unsigned int idx, csi_addr_matching_t *mode);

/*
 * Set PMP enctry excecutable flag by index.
 *
 * This function only can be called in M-mode. If it is called
 * in other mode, it may return an error or cause carsh. When this function is called
 * with success, it will not be effective untill the core has been exit M-mode.
 *
 * @param idx: The index of PMP config entries
 * @param flag_x: The excecutable flag of PMP config entries.
 * @return : Status code
 */
csi_status_t csi_pmp_set_executable(unsigned int idx, bool flag_x);

/*
 * Get PMP enctry excecutable flag by index.
 *
 * This function only can be called in M-mode. If it is called
 * in other mode, it may return an error or cause carsh. When this function is called
 * with success, it will not be effective untill the core has been exit M-mode.
 *
 * @param idx: The index of PMP config entries
 * @param flag_x: A pointer to the buffer which is used to output the excecutable flag
 *                  of PMP config entries.
 * @return : Status code
 */
csi_status_t csi_pmp_get_executable(unsigned int idx, bool *flag_x);

/*
 * Set PMP enctry readable flag by index.
 *
 * This function only can be called in M-mode. If it is called
 * in other mode, it may return an error or cause carsh. When this function is called
 * with success, it will not be effective untill the core has been exit M-mode.
 *
 * @param idx: The index of PMP config entries
 * @param flag_r: The readable flag of PMP config entries.
 * @return : Status code
 */
csi_status_t csi_pmp_set_readable(unsigned int idx, bool flag_r);

/*
 * Get PMP enctry readable flag by index.
 *
 * This function only can be called in M-mode. If it is called
 * in other mode, it may return an error or cause carsh. When this function is called
 * with success, it will not be effective untill the core has been exit M-mode.
 *
 * @param idx: The index of PMP config entries
 * @param flag_r: A pointer to the buffer which is used to output the readable flag
 *                  of PMP config entries.
 * @return : Status code
 */
csi_status_t csi_pmp_get_readable(unsigned int idx, bool *flag_r);

/*
 * Set PMP enctry writable flag by index.
 *
 * This function only can be called in M-mode. If it is called
 * in other mode, it may return an error or cause carsh. When this function is called
 * with success, it will not be effective untill the core has been exit M-mode.
 *
 * @param idx: The index of PMP config entries
 * @param flag_w: The writable flag of PMP config entries.
 * @return : Status code
 */
csi_status_t csi_pmp_set_writable(unsigned int idx, bool flag_w);

/*
 * Get PMP enctry writable flag by index.
 *
 * This function only can be called in M-mode. If it is called
 * in other mode, it may return an error or cause carsh. When this function is called
 * with success, it will not be effective untill the core has been exit M-mode.
 *
 * @param idx: The index of PMP config entries
 * @param flag_w: A pointer to the buffer which is used to output the writable flag
 *                  of PMP config entries.
 * @return : Status code
 */
csi_status_t csi_pmp_get_writable(unsigned int idx, bool *flag_w);

#endif /* CSI_LL_PMP_H */
