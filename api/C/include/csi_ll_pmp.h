/*
 * Low-Level PMP API
 *
 * This module provides functionality for PMP configuration.
 *
 * Copyright (c) RISC-V International 2023. Creative Commons License. Auto-
 * generated file: DO NOT EDIT
 */

#ifndef CSI_LL_PMP_H
#define CSI_LL_PMP_H

#include "csi_types.h"

/*
 * Attribute definitions of PMP entry: PMP_ATTR_FLAG_R: indicate that the PMP entry
 * permits to read PMP_ATTR_FLAG_W: indicate that the PMP entry permits to write
 * PMP_ATTR_FLAG_X: indicate that the PMP entry permits to execute PMP_ATTR_FLAG_L:
 * indicate that the PMP entry permits to lock
 */
#define PMP_ATTR_FLAG_R				    0x01
#define PMP_ATTR_FLAG_W				    0x02
#define PMP_ATTR_FLAG_X				    0x04
#define PMP_ATTR_FLAG_L				    0x80

/*
 * Address-matching definitions of PMP entry: PMP_ATTR_ADDR_MATCHING_DISABLED:
 * indicate that the PMP entry is disabled PMP_ATTR_ADDR_MATCHING_TOR: indicate
 * that the PMP entry's address-matching is TOR PMP_ATTR_ADDR_MATCHING_NA4:
 * indicate that the PMP entry's address-matching is NA4
 * PMP_ATTR_ADDR_MATCHING_NAPOT: indicate that the PMP entry's address-matching is
 * NAPOT
 */
#define PMP_ATTR_ADDR_MATCHING_DISABLED 0x0
#define PMP_ATTR_ADDR_MATCHING_TOR		  0x08
#define PMP_ATTR_ADDR_MATCHING_NA4		  0x10
#define PMP_ATTR_ADDR_MATCHING_NAPOT	  0x18


/*
 * Get the number of PMP entries for the hard id. If the hart id is invalid, such
 * as out of support range, it should return an error
 *
 * @param hart_id: The hart id used to get the number of pmp entry
 * @return : The number of PMP entry; -1 will be returned as and error code if the
 * input is invalid.
 */
int csi_pmp_get_num_entries(unsigned int hart_id);

/*
 * Set PMP entry. This function only can be called in M-mode. If it is called in
 * other mode, it will cause an access fault exception. When this function is
 * called with success, it will not be effective untill the core exits M-mode.
 * Also, an error should be returned if the input parameters are invalid, such as
 * idx out of range, address or size is not supported etc.
 *
 * @param idx: The PMP entry index
 * @param base_addr: The base address of this entry need to be configured. If the
 * address-matching mode is TOR, it would be ignored. And the base address of this
 * entry would be address of previous entry
 * @param size: The size of this range need to be configured.
 * @param pmp_attr: The configuration information of PMP entry.The PMP
 * configuration format is: bit0[R]: indicate that the PMP entry permits to read.
 * bit1[W]: indicate that the PMP entry permits to write. bit2[X]: indicate that
 * the PMP entry permits to execute. bit4_3[A]: indicate the address-matching mode:
 * TOR/NA4/NAPOT. bit7[L]: indicate that the PMP entry permits to lock.
 * @return : Status code
 */
csi_status_t csi_pmp_set_entry(unsigned int idx, unsigned long base_addr, unsigned long size, unsigned char pmp_attr);

/*
 * Get PMP entry configure information. This function only can be called in M-mode.
 * If it is called in other mode, it will cause an access fault exception. Also, an
 * error should be returned if the input parameters are invalid, such as idx out of
 * range.
 *
 * @param idx: The index of PMP config entries
 * @param base_addr: A pointer to the buffer which is used to output the base
 * address of this entry
 * @param size: A pointer to the buffer which is used to output the size of this
 * entry
 * @param pmp_attr: A pointer to the buffer which is used to output the the
 * configuration information of PMP entry.The PMP configuration format is: bit0[R]:
 * indicate that the PMP entry permits to read. bit1[W]: indicate that the PMP
 * entry permits to write. bit2[X]: indicate that the PMP entry permits to execute.
 * bit4_3[A]: indicate the address-matching mode: TOR/NA4/NAPOT. bit7[L]: indicate
 * that the PMP entry permits to lock.
 * @return : Status code
 */
csi_status_t csi_pmp_get_entry(unsigned int idx, unsigned long *base_addr, unsigned long *size, unsigned char *pmp_attr);


#endif /* CSI_LL_PMP_H */ 
