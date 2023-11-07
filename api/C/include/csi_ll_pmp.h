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
 * Get PMP range number for the hard id. If the hart id is invalid, such as out of
 * support range, it should return an error
 *
 * @param hart_id: The hart id used to get the pmp entry number
 * @return : The number of PMP entry; -1 will be returned as and error code if the
 * input is invalid.
 */
int csi_pmp_get_entry_num(unsigned int hart_id);

/*
 * Set PMP entry. This function only can be called in M-mode. If it is called in
 * other mode, it may cause carsh. When this function is called with success, it
 * will not be effective untill  the core exits M-mode. Also, an error should be
 * returned if the output parameters is invalid, such as idx out of range, address
 * or size is not supported or over overlapped etc.
 *
 * @param idx: The PMP entry index
 * @param base_addr: The base address of this range need to be configured
 * @param size: The size of this range need to be configured
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
 * If it is called in other mode, it may cause carsh. Also, an error should be
 * returned if the output parameters is invalid, such as idx out of range.
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


/*
 * Attribute definitions of PMP entry: PMP_ATTR_FLAG_R: indicate that the PMP entry
 * permits to read PMP_ATTR_FLAG_W: indicate that the PMP entry permits to write
 * PMP_ATTR_FLAG_X: indicate that the PMP entry permits to execute PMP_ATTR_FLAG_L:
 * indicate that the PMP entry permits to lock
 */
#define PMP_ATTR_FLAG_R				    _UL(0x01)
#define PMP_ATTR_FLAG_W				    _UL(0x02)
#define PMP_ATTR_FLAG_X				    _UL(0x04)
#define PMP_ATTR_FLAG_L				    _UL(0x80)

/*
 * Address-matching definitions of PMP entry: PMP_ATTR_ADDR_MATCHING_TOR: indicate
 * that the PMP entry's address-matching is TOR PMP_ATTR_ADDR_MATCHING_NA4:
 * indicate that the PMP entry's address-matching is NA4
 * PMP_ATTR_ADDR_MATCHING_NAPOT: indicate that the PMP entry's address-matching is
 * NAPOT
 */
#define PMP_ATTR_ADDR_MATCHING_TOR		_UL(0x08)
#define PMP_ATTR_ADDR_MATCHING_NA4		_UL(0x10)
#define PMP_ATTR_ADDR_MATCHING_NAPOT	_UL(0x18)


#endif /* CSI_LL_PMP_H */ 
