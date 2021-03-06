//
// -----------------------------------------------------------------------------
// Copyright (c) 2011-2014 Qualcomm Atheros, Inc.  All rights reserved.
// -----------------------------------------------------------------------------
// FILE         : tiered_sm1_table_map
// DESCRIPTION  : Software Header File for WiFi 2.0
// THIS FILE IS AUTOMATICALLY GENERATED BY DENALI BLUEPRINT, DO NOT EDIT
// -----------------------------------------------------------------------------
//

#ifndef __REG_TIERED_SM1_TABLE_MAP_H__
#define __REG_TIERED_SM1_TABLE_MAP_H__

#include "tiered_sm1_table_map_macro.h"

struct sm1_table_map {
  volatile u_int32_t BB_sm1_tables_dummy1;        /*        0x0 - 0x4        */
  volatile unsigned int pad__0[0xbf];             /*        0x4 - 0x300      */
  volatile u_int32_t BB_dc_dac_mem_b1[52];        /*      0x300 - 0x3d0      */
  volatile unsigned int pad__1[0x3cc];            /*      0x3d0 - 0x1300     */
  volatile u_int32_t BB_sm1_hc_preemp_lut[412];   /*     0x1300 - 0x1970     */
  volatile u_int32_t BB_sm1_tables_dummy2;        /*     0x1970 - 0x1974     */
};

struct tiered_sm1_table_map {
  struct sm1_table_map sm1_table_map;             /*        0x0 - 0x1974     */
};

#endif /* __REG_TIERED_SM1_TABLE_MAP_H__ */
