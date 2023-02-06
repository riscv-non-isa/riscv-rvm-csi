/*
* Top-level include file for RVM-CSI Hardware Abstraction Layer.  This is the only
* header to be included in the user's application.
*/

#ifndef RVM_CSI_H
#define RVM_CSI_H

#include "csi_defs.h" /* Standard definitions for use in other headers */
#include "csi_csrs.h" /* Standard RISC-V CSR addresses and bitfields */
#include "csi_bsp_csrs.h" /* Custom CSR addresses and bitfields supplied by BSP */
#include "csi_bsp_perip.h" /* Peripheral register addresses and bitfields supplied by BSP */
#include "csi_bsp_interrupts.h" /* Enumeration of external interrupt sources supplied by BSP */
#include "csi_bsp_defs.h" /* Other macro definitions supplied by BSP */
#include "csi.h" /* The platform-agnostic RVM-CSI API */

#endif /* RVM_CSI_H */
