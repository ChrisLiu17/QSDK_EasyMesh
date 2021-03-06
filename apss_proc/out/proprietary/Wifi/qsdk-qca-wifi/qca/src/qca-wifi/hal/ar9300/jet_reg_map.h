/*
 * Copyright (c) 2017 Qualcomm Innovation Center, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Innovation Center, Inc.
 */
/*                                                                           */
/* File:       /trees/qingfeng/qingfeng_jet_tdc/blueprint/top/jet_reg_map.h  */
/* Creator:    qingfeng                                                      */
/* Time:       Friday May 26, 2017 [1:33:02 pm]                              */
/*                                                                           */
/* Path:       /trees/qingfeng/qingfeng_jet_tdc/blueprint/top                */
/* Arguments:  /cad/denali/blueprint/3.7.3//Linux-64bit/blueprint -dump      */
/*             -codegen                                                      */
/*             /trees/qingfeng/qingfeng_jet_tdc/flow/blueprint/ath_ansic.codegen*/
/*             -ath_ansic -Wdesc -I                                          */
/*             /trees/qingfeng/qingfeng_jet_tdc/blueprint/top -I             */
/*             /trees/qingfeng/qingfeng_jet_tdc/blueprint -I                 */
/*             /trees/qingfeng/qingfeng_jet_tdc/flow/blueprint -I            */
/*             /trees/qingfeng/qingfeng_jet_tdc/blueprint/sysconfig -odir    */
/*             /trees/qingfeng/qingfeng_jet_tdc/blueprint/top -eval          */
/*             {\\$INCLUDE_SYSCONFIG_FILES=1} -eval                          */
/*             \\$WAR_EV58615_for_ansic_codegen=1 jet_reg.rdl                */
/*                                                                           */
/* Sources:    /trees/qingfeng/qingfeng_jet_tdc/rtl/mac/rtl/mac_dma/blueprint/mac_dma_reg.rdl*/
/*             /trees/qingfeng/qingfeng_jet_tdc/rtl/mac/rtl/mac_dma/blueprint/mac_dcu_reg.rdl*/
/*             /trees/qingfeng/qingfeng_jet_tdc/rtl/rtc/rtc_reg.rdl          */
/*             /trees/qingfeng/qingfeng_jet_tdc/rtl/wmac_glue_intf/rtl/wmac_glue_intf_reg.rdl*/
/*             /trees/qingfeng/qingfeng_jet_tdc/blueprint/sysconfig/bb_reg_map_sysconfig.rdl*/
/*             /trees/qingfeng/qingfeng_jet_tdc/rtl/mac/rtl/mac_pcu/blueprint/mac_pcu_reg.rdl*/
/*             /trees/qingfeng/qingfeng_jet_tdc/blueprint/sysconfig/mac_qcu_reg_sysconfig.rdl*/
/*             /trees/qingfeng/qingfeng_jet_tdc/blueprint/top/dragonfly_radio_reg.rdl*/
/*             /trees/qingfeng/qingfeng_jet_tdc/rtl/wmac_wrap/rtc_sync_reg.rdl*/
/*             /trees/qingfeng/qingfeng_jet_tdc/blueprint/top/merlin2_0_radio_reg_map.rdl*/
/*             /trees/qingfeng/qingfeng_jet_tdc/rtl/mac/rtl/mac_dma/blueprint/mac_qcu_reg.rdl*/
/*             /trees/qingfeng/qingfeng_jet_tdc/blueprint/sysconfig/rtc_reg_sysconfig.rdl*/
/*             /trees/qingfeng/qingfeng_jet_tdc/blueprint/sysconfig/mac_dcu_reg_sysconfig.rdl*/
/*             /trees/qingfeng/qingfeng_jet_tdc/blueprint/top/jet_reg.rdl    */
/*             /trees/qingfeng/qingfeng_jet_tdc/blueprint/sysconfig/merlin2_0_radio_reg_sysconfig.rdl*/
/*             /trees/qingfeng/qingfeng_jet_tdc/rtl/bb/blueprint/bb_reg_map.rdl*/
/*             /trees/qingfeng/qingfeng_jet_tdc/blueprint/sysconfig/mac_pcu_reg_sysconfig.rdl*/
/*             /trees/qingfeng/qingfeng_jet_tdc/blueprint/sysconfig/radio_65_reg_sysconfig.rdl*/
/*             /trees/qingfeng/qingfeng_jet_tdc/blueprint/sysconfig/rtc_sync_reg_sysconfig.rdl*/
/*             /trees/qingfeng/qingfeng_jet_tdc/blueprint/sysconfig/mac_dma_reg_sysconfig.rdl*/
/*             /cad/local/lib/perl/Pinfo.pm                                  */
/*             /trees/qingfeng/qingfeng_jet_tdc/flow/blueprint/ath_ansic.pm  */
/*                                                                           */
/* Blueprint:   3.7.3 (Fri Aug 29 12:39:16 PDT 2008)                         */
/* Machine:    compute-pre-hsi-03                                            */
/* OS:         Linux 2.6.18-348.el5                                          */
/* Description:                                                              */
/*                                                                           */
/*This Register Map contains the complete register set for jet.              */

#ifndef __REG_JET_REG_MAP_H__
#define __REG_JET_REG_MAP_H__

#include "jet_reg_map_macro.h"

struct mac_dma_reg {
  volatile char pad__0[0x8];                      /*        0x0 - 0x8        */
  volatile u_int32_t MAC_DMA_CR;                  /*        0x8 - 0xc        */
  volatile char pad__1[0x8];                      /*        0xc - 0x14       */
  volatile u_int32_t MAC_DMA_CFG;                 /*       0x14 - 0x18       */
  volatile u_int32_t MAC_DMA_RXBUFPTR_THRESH;     /*       0x18 - 0x1c       */
  volatile u_int32_t MAC_DMA_TXDPPTR_THRESH;      /*       0x1c - 0x20       */
  volatile u_int32_t MAC_DMA_MIRT;                /*       0x20 - 0x24       */
  volatile u_int32_t MAC_DMA_GLOBAL_IER;          /*       0x24 - 0x28       */
  volatile u_int32_t MAC_DMA_TIMT;                /*       0x28 - 0x2c       */
  volatile u_int32_t MAC_DMA_RIMT;                /*       0x2c - 0x30       */
  volatile u_int32_t MAC_DMA_TXCFG;               /*       0x30 - 0x34       */
  volatile u_int32_t MAC_DMA_RXCFG;               /*       0x34 - 0x38       */
  volatile u_int32_t MAC_DMA_RXJLA;               /*       0x38 - 0x3c       */
  volatile char pad__2[0x4];                      /*       0x3c - 0x40       */
  volatile u_int32_t MAC_DMA_MIBC;                /*       0x40 - 0x44       */
  volatile u_int32_t MAC_DMA_TOPS;                /*       0x44 - 0x48       */
  volatile u_int32_t MAC_DMA_RXNPTO;              /*       0x48 - 0x4c       */
  volatile u_int32_t MAC_DMA_TXNPTO;              /*       0x4c - 0x50       */
  volatile u_int32_t MAC_DMA_RPGTO;               /*       0x50 - 0x54       */
  volatile char pad__3[0x4];                      /*       0x54 - 0x58       */
  volatile u_int32_t MAC_DMA_MACMISC;             /*       0x58 - 0x5c       */
  volatile u_int32_t MAC_DMA_INTER;               /*       0x5c - 0x60       */
  volatile u_int32_t MAC_DMA_DATABUF;             /*       0x60 - 0x64       */
  volatile u_int32_t MAC_DMA_GTT;                 /*       0x64 - 0x68       */
  volatile u_int32_t MAC_DMA_GTTM;                /*       0x68 - 0x6c       */
  volatile u_int32_t MAC_DMA_CST;                 /*       0x6c - 0x70       */
  volatile u_int32_t MAC_DMA_RXDP_SIZE;           /*       0x70 - 0x74       */
  volatile u_int32_t MAC_DMA_RX_QUEUE_HP_RXDP;    /*       0x74 - 0x78       */
  volatile u_int32_t MAC_DMA_RX_QUEUE_LP_RXDP;    /*       0x78 - 0x7c       */
  volatile char pad__4[0x4];                      /*       0x7c - 0x80       */
  volatile u_int32_t MAC_DMA_ISR_P;               /*       0x80 - 0x84       */
  volatile u_int32_t MAC_DMA_ISR_S0;              /*       0x84 - 0x88       */
  volatile u_int32_t MAC_DMA_ISR_S1;              /*       0x88 - 0x8c       */
  volatile u_int32_t MAC_DMA_ISR_S2;              /*       0x8c - 0x90       */
  volatile u_int32_t MAC_DMA_ISR_S3;              /*       0x90 - 0x94       */
  volatile u_int32_t MAC_DMA_ISR_S4;              /*       0x94 - 0x98       */
  volatile u_int32_t MAC_DMA_ISR_S5;              /*       0x98 - 0x9c       */
  volatile char pad__5[0x4];                      /*       0x9c - 0xa0       */
  volatile u_int32_t MAC_DMA_IMR_P;               /*       0xa0 - 0xa4       */
  volatile u_int32_t MAC_DMA_IMR_S0;              /*       0xa4 - 0xa8       */
  volatile u_int32_t MAC_DMA_IMR_S1;              /*       0xa8 - 0xac       */
  volatile u_int32_t MAC_DMA_IMR_S2;              /*       0xac - 0xb0       */
  volatile u_int32_t MAC_DMA_IMR_S3;              /*       0xb0 - 0xb4       */
  volatile u_int32_t MAC_DMA_IMR_S4;              /*       0xb4 - 0xb8       */
  volatile u_int32_t MAC_DMA_IMR_S5;              /*       0xb8 - 0xbc       */
  volatile char pad__6[0x4];                      /*       0xbc - 0xc0       */
  volatile u_int32_t MAC_DMA_ISR_P_RAC;           /*       0xc0 - 0xc4       */
  volatile u_int32_t MAC_DMA_ISR_S0_S;            /*       0xc4 - 0xc8       */
  volatile u_int32_t MAC_DMA_ISR_S1_S;            /*       0xc8 - 0xcc       */
  volatile char pad__7[0x4];                      /*       0xcc - 0xd0       */
  volatile u_int32_t MAC_DMA_ISR_S2_S;            /*       0xd0 - 0xd4       */
  volatile u_int32_t MAC_DMA_ISR_S3_S;            /*       0xd4 - 0xd8       */
  volatile u_int32_t MAC_DMA_ISR_S4_S;            /*       0xd8 - 0xdc       */
  volatile u_int32_t MAC_DMA_ISR_S5_S;            /*       0xdc - 0xe0       */
  volatile u_int32_t MAC_DMA_DMADBG_0;            /*       0xe0 - 0xe4       */
  volatile u_int32_t MAC_DMA_DMADBG_1;            /*       0xe4 - 0xe8       */
  volatile u_int32_t MAC_DMA_DMADBG_2;            /*       0xe8 - 0xec       */
  volatile u_int32_t MAC_DMA_DMADBG_3;            /*       0xec - 0xf0       */
  volatile u_int32_t MAC_DMA_DMADBG_4;            /*       0xf0 - 0xf4       */
  volatile u_int32_t MAC_DMA_DMADBG_5;            /*       0xf4 - 0xf8       */
  volatile u_int32_t MAC_DMA_DMADBG_6;            /*       0xf8 - 0xfc       */
  volatile u_int32_t MAC_DMA_DMADBG_7;            /*       0xfc - 0x100      */
  volatile u_int32_t MAC_DMA_QCU_TXDP_REMAINING_QCU_7_0;
                                                  /*      0x100 - 0x104      */
  volatile u_int32_t MAC_DMA_QCU_TXDP_REMAINING_QCU_9_8;
                                                  /*      0x104 - 0x108      */
  volatile u_int32_t MAC_DMA_TIMT_0;              /*      0x108 - 0x10c      */
  volatile u_int32_t MAC_DMA_TIMT_1;              /*      0x10c - 0x110      */
  volatile u_int32_t MAC_DMA_TIMT_2;              /*      0x110 - 0x114      */
  volatile u_int32_t MAC_DMA_TIMT_3;              /*      0x114 - 0x118      */
  volatile u_int32_t MAC_DMA_TIMT_4;              /*      0x118 - 0x11c      */
  volatile u_int32_t MAC_DMA_TIMT_5;              /*      0x11c - 0x120      */
  volatile u_int32_t MAC_DMA_TIMT_6;              /*      0x120 - 0x124      */
  volatile u_int32_t MAC_DMA_TIMT_7;              /*      0x124 - 0x128      */
  volatile u_int32_t MAC_DMA_TIMT_8;              /*      0x128 - 0x12c      */
  volatile u_int32_t MAC_DMA_TIMT_9;              /*      0x12c - 0x130      */
};

struct mac_qcu_reg {
  volatile char pad__0[0x800];                    /*        0x0 - 0x800      */
  volatile u_int32_t MAC_QCU_TXDP[10];            /*      0x800 - 0x828      */
  volatile char pad__1[0x8];                      /*      0x828 - 0x830      */
  volatile u_int32_t MAC_QCU_STATUS_RING_START;   /*      0x830 - 0x834      */
  volatile u_int32_t MAC_QCU_STATUS_RING_END;     /*      0x834 - 0x838      */
  volatile u_int32_t MAC_QCU_STATUS_RING_CURRENT; /*      0x838 - 0x83c      */
  volatile char pad__2[0x4];                      /*      0x83c - 0x840      */
  volatile u_int32_t MAC_QCU_TXE;                 /*      0x840 - 0x844      */
  volatile char pad__3[0x3c];                     /*      0x844 - 0x880      */
  volatile u_int32_t MAC_QCU_TXD;                 /*      0x880 - 0x884      */
  volatile char pad__4[0x3c];                     /*      0x884 - 0x8c0      */
  volatile u_int32_t MAC_QCU_CBR[10];             /*      0x8c0 - 0x8e8      */
  volatile char pad__5[0x18];                     /*      0x8e8 - 0x900      */
  volatile u_int32_t MAC_QCU_RDYTIME[10];         /*      0x900 - 0x928      */
  volatile char pad__6[0x18];                     /*      0x928 - 0x940      */
  volatile u_int32_t MAC_QCU_ONESHOT_ARM_SC;      /*      0x940 - 0x944      */
  volatile char pad__7[0x3c];                     /*      0x944 - 0x980      */
  volatile u_int32_t MAC_QCU_ONESHOT_ARM_CC;      /*      0x980 - 0x984      */
  volatile char pad__8[0x3c];                     /*      0x984 - 0x9c0      */
  volatile u_int32_t MAC_QCU_MISC[10];            /*      0x9c0 - 0x9e8      */
  volatile char pad__9[0x18];                     /*      0x9e8 - 0xa00      */
  volatile u_int32_t MAC_QCU_CNT[10];             /*      0xa00 - 0xa28      */
  volatile char pad__10[0x18];                    /*      0xa28 - 0xa40      */
  volatile u_int32_t MAC_QCU_RDYTIME_SHDN;        /*      0xa40 - 0xa44      */
  volatile u_int32_t MAC_QCU_DESC_CRC_CHK;        /*      0xa44 - 0xa48      */
};

struct mac_dcu_reg {
  volatile char pad__0[0x1000];                   /*        0x0 - 0x1000     */
  volatile u_int32_t MAC_DCU_QCUMASK[10];         /*     0x1000 - 0x1028     */
  volatile char pad__1[0x8];                      /*     0x1028 - 0x1030     */
  volatile u_int32_t MAC_DCU_GBL_IFS_SIFS;        /*     0x1030 - 0x1034     */
  volatile char pad__2[0x4];                      /*     0x1034 - 0x1038     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU0_31_0;  /*     0x1038 - 0x103c     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU8_31_0;  /*     0x103c - 0x1040     */
  volatile u_int32_t MAC_DCU_LCL_IFS[10];         /*     0x1040 - 0x1068     */
  volatile char pad__3[0x8];                      /*     0x1068 - 0x1070     */
  volatile u_int32_t MAC_DCU_GBL_IFS_SLOT;        /*     0x1070 - 0x1074     */
  volatile char pad__4[0x4];                      /*     0x1074 - 0x1078     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU0_63_32; /*     0x1078 - 0x107c     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU8_63_32; /*     0x107c - 0x1080     */
  volatile u_int32_t MAC_DCU_RETRY_LIMIT[10];     /*     0x1080 - 0x10a8     */
  volatile char pad__5[0x8];                      /*     0x10a8 - 0x10b0     */
  volatile u_int32_t MAC_DCU_GBL_IFS_EIFS;        /*     0x10b0 - 0x10b4     */
  volatile char pad__6[0x4];                      /*     0x10b4 - 0x10b8     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU0_95_64; /*     0x10b8 - 0x10bc     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU8_95_64; /*     0x10bc - 0x10c0     */
  volatile u_int32_t MAC_DCU_CHANNEL_TIME[10];    /*     0x10c0 - 0x10e8     */
  volatile char pad__7[0x8];                      /*     0x10e8 - 0x10f0     */
  volatile u_int32_t MAC_DCU_GBL_IFS_MISC;        /*     0x10f0 - 0x10f4     */
  volatile char pad__8[0x4];                      /*     0x10f4 - 0x10f8     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU0_127_96;
                                                  /*     0x10f8 - 0x10fc     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU8_127_96;
                                                  /*     0x10fc - 0x1100     */
  volatile u_int32_t MAC_DCU_MISC[10];            /*     0x1100 - 0x1128     */
  volatile char pad__9[0x10];                     /*     0x1128 - 0x1138     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU1_31_0;  /*     0x1138 - 0x113c     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU9_31_0;  /*     0x113c - 0x1140     */
  volatile u_int32_t MAC_DCU_SEQ;                 /*     0x1140 - 0x1144     */
  volatile char pad__10[0x34];                    /*     0x1144 - 0x1178     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU1_63_32; /*     0x1178 - 0x117c     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU9_63_32; /*     0x117c - 0x1180     */
  volatile char pad__11[0x38];                    /*     0x1180 - 0x11b8     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU1_95_64; /*     0x11b8 - 0x11bc     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU9_95_64; /*     0x11bc - 0x11c0     */
  volatile char pad__12[0x38];                    /*     0x11c0 - 0x11f8     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU1_127_96;
                                                  /*     0x11f8 - 0x11fc     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU9_127_96;
                                                  /*     0x11fc - 0x1200     */
  volatile char pad__13[0x38];                    /*     0x1200 - 0x1238     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU2_31_0;  /*     0x1238 - 0x123c     */
  volatile char pad__14[0x34];                    /*     0x123c - 0x1270     */
  volatile u_int32_t MAC_DCU_PAUSE;               /*     0x1270 - 0x1274     */
  volatile char pad__15[0x4];                     /*     0x1274 - 0x1278     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU2_63_32; /*     0x1278 - 0x127c     */
  volatile char pad__16[0x34];                    /*     0x127c - 0x12b0     */
  volatile u_int32_t MAC_DCU_WOW_KACFG;           /*     0x12b0 - 0x12b4     */
  volatile char pad__17[0x4];                     /*     0x12b4 - 0x12b8     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU2_95_64; /*     0x12b8 - 0x12bc     */
  volatile char pad__18[0x34];                    /*     0x12bc - 0x12f0     */
  volatile u_int32_t MAC_DCU_TXSLOT;              /*     0x12f0 - 0x12f4     */
  volatile char pad__19[0x4];                     /*     0x12f4 - 0x12f8     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU2_127_96;
                                                  /*     0x12f8 - 0x12fc     */
  volatile char pad__20[0x3c];                    /*     0x12fc - 0x1338     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU3_31_0;  /*     0x1338 - 0x133c     */
  volatile char pad__21[0x3c];                    /*     0x133c - 0x1378     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU3_63_32; /*     0x1378 - 0x137c     */
  volatile char pad__22[0x3c];                    /*     0x137c - 0x13b8     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU3_95_64; /*     0x13b8 - 0x13bc     */
  volatile char pad__23[0x3c];                    /*     0x13bc - 0x13f8     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU3_127_96;
                                                  /*     0x13f8 - 0x13fc     */
  volatile char pad__24[0x3c];                    /*     0x13fc - 0x1438     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU4_31_0;  /*     0x1438 - 0x143c     */
  volatile u_int32_t MAC_DCU_TXFILTER_CLEAR;      /*     0x143c - 0x1440     */
  volatile char pad__25[0x38];                    /*     0x1440 - 0x1478     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU4_63_32; /*     0x1478 - 0x147c     */
  volatile u_int32_t MAC_DCU_TXFILTER_SET;        /*     0x147c - 0x1480     */
  volatile char pad__26[0x38];                    /*     0x1480 - 0x14b8     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU4_95_64; /*     0x14b8 - 0x14bc     */
  volatile char pad__27[0x3c];                    /*     0x14bc - 0x14f8     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU4_127_96;
                                                  /*     0x14f8 - 0x14fc     */
  volatile char pad__28[0x3c];                    /*     0x14fc - 0x1538     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU5_31_0;  /*     0x1538 - 0x153c     */
  volatile char pad__29[0x3c];                    /*     0x153c - 0x1578     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU5_63_32; /*     0x1578 - 0x157c     */
  volatile char pad__30[0x3c];                    /*     0x157c - 0x15b8     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU5_95_64; /*     0x15b8 - 0x15bc     */
  volatile char pad__31[0x3c];                    /*     0x15bc - 0x15f8     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU5_127_96;
                                                  /*     0x15f8 - 0x15fc     */
  volatile char pad__32[0x3c];                    /*     0x15fc - 0x1638     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU6_31_0;  /*     0x1638 - 0x163c     */
  volatile char pad__33[0x3c];                    /*     0x163c - 0x1678     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU6_63_32; /*     0x1678 - 0x167c     */
  volatile char pad__34[0x3c];                    /*     0x167c - 0x16b8     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU6_95_64; /*     0x16b8 - 0x16bc     */
  volatile char pad__35[0x3c];                    /*     0x16bc - 0x16f8     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU6_127_96;
                                                  /*     0x16f8 - 0x16fc     */
  volatile char pad__36[0x3c];                    /*     0x16fc - 0x1738     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU7_31_0;  /*     0x1738 - 0x173c     */
  volatile char pad__37[0x3c];                    /*     0x173c - 0x1778     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU7_63_32; /*     0x1778 - 0x177c     */
  volatile char pad__38[0x3c];                    /*     0x177c - 0x17b8     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU7_95_64; /*     0x17b8 - 0x17bc     */
  volatile char pad__39[0x3c];                    /*     0x17bc - 0x17f8     */
  volatile u_int32_t MAC_DCU_TXFILTER_DCU7_127_96;
                                                  /*     0x17f8 - 0x17fc     */
  volatile char pad__40[0x704];                   /*     0x17fc - 0x1f00     */
  volatile u_int32_t MAC_SLEEP_STATUS;            /*     0x1f00 - 0x1f04     */
  volatile u_int32_t MAC_LED_CONFIG;              /*     0x1f04 - 0x1f08     */
};

struct host_intf_reg {
  volatile char pad__0[0x4000];                   /*        0x0 - 0x4000     */
  volatile u_int32_t HOST_INTF_RESET_CONTROL;
                                                  /*     0x4000 - 0x4004     */
  volatile char pad__1[0x10];                     /*     0x4004 - 0x4014     */
  volatile u_int32_t HOST_INTF_PM_CTRL;      /*     0x4014 - 0x4018     */
  volatile u_int32_t HOST_INTF_TIMEOUT;      /*     0x4018 - 0x401c     */
  volatile char pad__2[0xc];                      /*     0x401c - 0x4028     */
  volatile u_int32_t HOST_INTF_INTR_SYNC_CAUSE;
                                                  /*     0x4028 - 0x402c     */
  volatile u_int32_t HOST_INTF_INTR_SYNC_ENABLE;
                                                  /*     0x402c - 0x4030     */
  volatile u_int32_t HOST_INTF_INTR_ASYNC_MASK;
                                                  /*     0x4030 - 0x4034     */
  volatile u_int32_t HOST_INTF_INTR_SYNC_MASK;
                                                  /*     0x4034 - 0x4038     */
  volatile u_int32_t HOST_INTF_INTR_ASYNC_CAUSE;
                                                  /*     0x4038 - 0x403c     */
  volatile u_int32_t HOST_INTF_INTR_ASYNC_ENABLE;
                                                  /*     0x403c - 0x4040     */
  volatile char pad__3[0x8];                      /*     0x4040 - 0x4048     */
  volatile u_int32_t HOST_INTF_GPIO_OUT;     /*     0x4048 - 0x404c     */
  volatile u_int32_t HOST_INTF_GPIO_IN;      /*     0x404c - 0x4050     */
  volatile u_int32_t HOST_INTF_SWCOM_GPIO_FUNC_ENABLE;
                                                  /*     0x4050 - 0x4054     */
  volatile char pad__4[0x8];                      /*     0x4054 - 0x405c     */
  volatile u_int32_t HOST_INTF_GPIO_INPUT_VALUE;
                                                  /*     0x405c - 0x4060     */
  volatile char pad__5[0x14];                     /*     0x4060 - 0x4074     */
  volatile u_int32_t HOST_INTF_GPIO_INPUT_STATE;
                                                  /*     0x4074 - 0x4078     */
  volatile char pad__6[0x10];                     /*     0x4078 - 0x4088     */
  volatile u_int32_t HOST_INTF_OBS_CTRL;     /*     0x4088 - 0x408c     */
  volatile u_int32_t HOST_INTF_RFSILENT;     /*     0x408c - 0x4090     */
  volatile char pad__7[0x10];                     /*     0x4090 - 0x40a0     */
  volatile u_int32_t HOST_INTF_MISC;         /*     0x40a0 - 0x40a4     */
  volatile char pad__8[0x10];                     /*     0x40a4 - 0x40b4     */
  volatile u_int32_t HOST_INTF_MAC_TDMA_CCA_CNTL;
                                                  /*     0x40b4 - 0x40b8     */
  volatile u_int32_t HOST_INTF_MAC_TXAPSYNC; /*     0x40b8 - 0x40bc     */
  volatile u_int32_t HOST_INTF_MAC_TXSYNC_INITIAL_SYNC_TMR;
                                                  /*     0x40bc - 0x40c0     */
  volatile u_int32_t HOST_INTF_INTR_PRIORITY_SYNC_CAUSE;
                                                  /*     0x40c0 - 0x40c4     */
  volatile u_int32_t HOST_INTF_INTR_PRIORITY_SYNC_ENABLE;
                                                  /*     0x40c4 - 0x40c8     */
  volatile u_int32_t HOST_INTF_INTR_PRIORITY_ASYNC_MASK;
                                                  /*     0x40c8 - 0x40cc     */
  volatile u_int32_t HOST_INTF_INTR_PRIORITY_SYNC_MASK;
                                                  /*     0x40cc - 0x40d0     */
  volatile u_int32_t HOST_INTF_INTR_PRIORITY_ASYNC_CAUSE;
                                                  /*     0x40d0 - 0x40d4     */
  volatile u_int32_t HOST_INTF_INTR_PRIORITY_ASYNC_ENABLE;
                                                  /*     0x40d4 - 0x40d8     */
  volatile char pad__9[0x18];                     /*     0x40d8 - 0x40f0     */
  volatile u_int32_t HOST_INTF_AXI_BYTE_SWAP;
                                                  /*     0x40f0 - 0x40f4     */
};

struct rtc_reg {
  volatile char pad__0[0x7000];                   /*        0x0 - 0x7000     */
  volatile u_int32_t RESET_CONTROL;               /*     0x7000 - 0x7004     */
  volatile u_int32_t XTAL_CONTROL;                /*     0x7004 - 0x7008     */
  volatile u_int32_t REG_CONTROL0;                /*     0x7008 - 0x700c     */
  volatile u_int32_t REG_CONTROL1;                /*     0x700c - 0x7010     */
  volatile u_int32_t QUADRATURE;                  /*     0x7010 - 0x7014     */
  volatile u_int32_t PLL_CONTROL;                 /*     0x7014 - 0x7018     */
  volatile u_int32_t PLL_SETTLE;                  /*     0x7018 - 0x701c     */
  volatile u_int32_t XTAL_SETTLE;                 /*     0x701c - 0x7020     */
  volatile u_int32_t CLOCK_OUT;                   /*     0x7020 - 0x7024     */
  volatile u_int32_t BIAS_OVERRIDE;               /*     0x7024 - 0x7028     */
  volatile u_int32_t RESET_CAUSE;                 /*     0x7028 - 0x702c     */
  volatile u_int32_t SYSTEM_SLEEP;                /*     0x702c - 0x7030     */
  volatile u_int32_t MAC_SLEEP_CONTROL;           /*     0x7030 - 0x7034     */
  volatile u_int32_t KEEP_AWAKE;                  /*     0x7034 - 0x7038     */
  volatile u_int32_t DERIVED_RTC_CLK;             /*     0x7038 - 0x703c     */
  volatile u_int32_t PLL_CONTROL2;                /*     0x703c - 0x7040     */
};

struct rtc_sync_reg {
  volatile char pad__0[0x7040];                   /*        0x0 - 0x7040     */
  volatile u_int32_t RTC_SYNC_RESET;              /*     0x7040 - 0x7044     */
  volatile u_int32_t RTC_SYNC_STATUS;             /*     0x7044 - 0x7048     */
  volatile u_int32_t RTC_SYNC_DERIVED;            /*     0x7048 - 0x704c     */
  volatile u_int32_t RTC_SYNC_FORCE_WAKE;         /*     0x704c - 0x7050     */
  volatile u_int32_t RTC_SYNC_INTR_CAUSE;         /*     0x7050 - 0x7054     */
  volatile u_int32_t RTC_SYNC_INTR_ENABLE;        /*     0x7054 - 0x7058     */
  volatile u_int32_t RTC_SYNC_INTR_MASK;          /*     0x7058 - 0x705c     */
};

struct merlin2_0_radio_reg_map {
  volatile char pad__0[0x7800];                   /*        0x0 - 0x7800     */
  volatile u_int32_t RXTXBB1_CH1;                 /*     0x7800 - 0x7804     */
  volatile u_int32_t RXTXBB2_CH1;                 /*     0x7804 - 0x7808     */
  volatile u_int32_t RXTXBB3_CH1;                 /*     0x7808 - 0x780c     */
  volatile u_int32_t RXTXBB4_CH1;                 /*     0x780c - 0x7810     */
  volatile u_int32_t RF2G1_CH1;                   /*     0x7810 - 0x7814     */
  volatile u_int32_t RF2G2_CH1;                   /*     0x7814 - 0x7818     */
  volatile u_int32_t RF5G1_CH1;                   /*     0x7818 - 0x781c     */
  volatile u_int32_t RF5G2_CH1;                   /*     0x781c - 0x7820     */
  volatile u_int32_t RF5G3_CH1;                   /*     0x7820 - 0x7824     */
  volatile u_int32_t RXTXBB1_CH0;                 /*     0x7824 - 0x7828     */
  volatile u_int32_t RXTXBB2_CH0;                 /*     0x7828 - 0x782c     */
  volatile u_int32_t RXTXBB3_CH0;                 /*     0x782c - 0x7830     */
  volatile u_int32_t RXTXBB4_CH0;                 /*     0x7830 - 0x7834     */
  volatile u_int32_t RF5G1_CH0;                   /*     0x7834 - 0x7838     */
  volatile u_int32_t RF5G2_CH0;                   /*     0x7838 - 0x783c     */
  volatile u_int32_t RF5G3_CH0;                   /*     0x783c - 0x7840     */
  volatile u_int32_t RF2G1_CH0;                   /*     0x7840 - 0x7844     */
  volatile u_int32_t RF2G2_CH0;                   /*     0x7844 - 0x7848     */
  volatile u_int32_t SYNTH1;                      /*     0x7848 - 0x784c     */
  volatile u_int32_t SYNTH2;                      /*     0x784c - 0x7850     */
  volatile u_int32_t SYNTH3;                      /*     0x7850 - 0x7854     */
  volatile u_int32_t SYNTH4;                      /*     0x7854 - 0x7858     */
  volatile u_int32_t SYNTH5;                      /*     0x7858 - 0x785c     */
  volatile u_int32_t SYNTH6;                      /*     0x785c - 0x7860     */
  volatile u_int32_t SYNTH7;                      /*     0x7860 - 0x7864     */
  volatile u_int32_t SYNTH8;                      /*     0x7864 - 0x7868     */
  volatile u_int32_t SYNTH9;                      /*     0x7868 - 0x786c     */
  volatile u_int32_t SYNTH10;                     /*     0x786c - 0x7870     */
  volatile u_int32_t SYNTH11;                     /*     0x7870 - 0x7874     */
  volatile u_int32_t BIAS1;                       /*     0x7874 - 0x7878     */
  volatile u_int32_t BIAS2;                       /*     0x7878 - 0x787c     */
  volatile u_int32_t BIAS3;                       /*     0x787c - 0x7880     */
  volatile u_int32_t BIAS4;                       /*     0x7880 - 0x7884     */
  volatile u_int32_t GAIN0;                       /*     0x7884 - 0x7888     */
  volatile u_int32_t GAIN1;                       /*     0x7888 - 0x788c     */
  volatile u_int32_t TOP0;                        /*     0x788c - 0x7890     */
  volatile u_int32_t TOP1;                        /*     0x7890 - 0x7894     */
  volatile u_int32_t TOP2;                        /*     0x7894 - 0x7898     */
  volatile u_int32_t TOP3;                        /*     0x7898 - 0x789c     */
};

struct mac_pcu_reg {
  volatile char pad__0[0x8000];                   /*        0x0 - 0x8000     */
  volatile u_int32_t MAC_PCU_STA_ADDR_L32;        /*     0x8000 - 0x8004     */
  volatile u_int32_t MAC_PCU_STA_ADDR_U16;        /*     0x8004 - 0x8008     */
  volatile u_int32_t MAC_PCU_BSSID_L32;           /*     0x8008 - 0x800c     */
  volatile u_int32_t MAC_PCU_BSSID_U16;           /*     0x800c - 0x8010     */
  volatile u_int32_t MAC_PCU_BCN_RSSI_AVE;        /*     0x8010 - 0x8014     */
  volatile u_int32_t MAC_PCU_ACK_CTS_TIMEOUT;     /*     0x8014 - 0x8018     */
  volatile u_int32_t MAC_PCU_BCN_RSSI_CTL;        /*     0x8018 - 0x801c     */
  volatile u_int32_t MAC_PCU_USEC_LATENCY;        /*     0x801c - 0x8020     */
  volatile u_int32_t MAC_PCU_RESET_TSF;           /*     0x8020 - 0x8024     */
  volatile char pad__1[0x14];                     /*     0x8024 - 0x8038     */
  volatile u_int32_t MAC_PCU_MAX_CFP_DUR;         /*     0x8038 - 0x803c     */
  volatile u_int32_t MAC_PCU_RX_FILTER;           /*     0x803c - 0x8040     */
  volatile u_int32_t MAC_PCU_MCAST_FILTER_L32;    /*     0x8040 - 0x8044     */
  volatile u_int32_t MAC_PCU_MCAST_FILTER_U32;    /*     0x8044 - 0x8048     */
  volatile u_int32_t MAC_PCU_DIAG_SW;             /*     0x8048 - 0x804c     */
  volatile u_int32_t MAC_PCU_TSF_L32;             /*     0x804c - 0x8050     */
  volatile u_int32_t MAC_PCU_TSF_U32;             /*     0x8050 - 0x8054     */
  volatile u_int32_t MAC_PCU_TST_ADDAC;           /*     0x8054 - 0x8058     */
  volatile u_int32_t MAC_PCU_DEF_ANTENNA;         /*     0x8058 - 0x805c     */
  volatile u_int32_t MAC_PCU_AES_MUTE_MASK_0;     /*     0x805c - 0x8060     */
  volatile u_int32_t MAC_PCU_AES_MUTE_MASK_1;     /*     0x8060 - 0x8064     */
  volatile u_int32_t MAC_PCU_GATED_CLKS;          /*     0x8064 - 0x8068     */
  volatile u_int32_t MAC_PCU_OBS_BUS_2;           /*     0x8068 - 0x806c     */
  volatile u_int32_t MAC_PCU_OBS_BUS_1;           /*     0x806c - 0x8070     */
  volatile u_int32_t MAC_PCU_DYM_MIMO_PWR_SAVE;   /*     0x8070 - 0x8074     */
  volatile u_int32_t MAC_PCU_TDMA_TXFRAME_START_TIME_TRIGGER_LSB;
                                                  /*     0x8074 - 0x8078     */
  volatile u_int32_t MAC_PCU_TDMA_TXFRAME_START_TIME_TRIGGER_MSB;
                                                  /*     0x8078 - 0x807c     */
  volatile char pad__2[0x4];                      /*     0x807c - 0x8080     */
  volatile u_int32_t MAC_PCU_LAST_BEACON_TSF;     /*     0x8080 - 0x8084     */
  volatile u_int32_t MAC_PCU_NAV;                 /*     0x8084 - 0x8088     */
  volatile u_int32_t MAC_PCU_RTS_SUCCESS_CNT;     /*     0x8088 - 0x808c     */
  volatile u_int32_t MAC_PCU_RTS_FAIL_CNT;        /*     0x808c - 0x8090     */
  volatile u_int32_t MAC_PCU_ACK_FAIL_CNT;        /*     0x8090 - 0x8094     */
  volatile u_int32_t MAC_PCU_FCS_FAIL_CNT;        /*     0x8094 - 0x8098     */
  volatile u_int32_t MAC_PCU_BEACON_CNT;          /*     0x8098 - 0x809c     */
  volatile u_int32_t MAC_PCU_TDMA_SLOT_ALERT_CNTL;
                                                  /*     0x809c - 0x80a0     */
  volatile u_int32_t MAC_PCU_BASIC_SET;           /*     0x80a0 - 0x80a4     */
  volatile u_int32_t MAC_PCU_MGMT_SEQ;            /*     0x80a4 - 0x80a8     */
  volatile u_int32_t MAC_PCU_BF_RPT1;             /*     0x80a8 - 0x80ac     */
  volatile u_int32_t MAC_PCU_BF_RPT2;             /*     0x80ac - 0x80b0     */
  volatile u_int32_t MAC_PCU_TX_ANT_1;            /*     0x80b0 - 0x80b4     */
  volatile u_int32_t MAC_PCU_TX_ANT_2;            /*     0x80b4 - 0x80b8     */
  volatile u_int32_t MAC_PCU_TX_ANT_3;            /*     0x80b8 - 0x80bc     */
  volatile u_int32_t MAC_PCU_TX_ANT_4;            /*     0x80bc - 0x80c0     */
  volatile u_int32_t MAC_PCU_XRMODE;              /*     0x80c0 - 0x80c4     */
  volatile u_int32_t MAC_PCU_XRDEL;               /*     0x80c4 - 0x80c8     */
  volatile u_int32_t MAC_PCU_XRTO;                /*     0x80c8 - 0x80cc     */
  volatile u_int32_t MAC_PCU_XRCRP;               /*     0x80cc - 0x80d0     */
  volatile u_int32_t MAC_PCU_XRSTMP;              /*     0x80d0 - 0x80d4     */
  volatile u_int32_t MAC_PCU_SLP1;                /*     0x80d4 - 0x80d8     */
  volatile u_int32_t MAC_PCU_SLP2;                /*     0x80d8 - 0x80dc     */
  volatile u_int32_t MAC_PCU_SELF_GEN_DEFAULT;    /*     0x80dc - 0x80e0     */
  volatile u_int32_t MAC_PCU_ADDR1_MASK_L32;      /*     0x80e0 - 0x80e4     */
  volatile u_int32_t MAC_PCU_ADDR1_MASK_U16;      /*     0x80e4 - 0x80e8     */
  volatile u_int32_t MAC_PCU_TPC;                 /*     0x80e8 - 0x80ec     */
  volatile u_int32_t MAC_PCU_TX_FRAME_CNT;        /*     0x80ec - 0x80f0     */
  volatile u_int32_t MAC_PCU_RX_FRAME_CNT;        /*     0x80f0 - 0x80f4     */
  volatile u_int32_t MAC_PCU_RX_CLEAR_CNT;        /*     0x80f4 - 0x80f8     */
  volatile u_int32_t MAC_PCU_CYCLE_CNT;           /*     0x80f8 - 0x80fc     */
  volatile u_int32_t MAC_PCU_QUIET_TIME_1;        /*     0x80fc - 0x8100     */
  volatile u_int32_t MAC_PCU_QUIET_TIME_2;        /*     0x8100 - 0x8104     */
  volatile char pad__3[0x4];                      /*     0x8104 - 0x8108     */
  volatile u_int32_t MAC_PCU_QOS_NO_ACK;          /*     0x8108 - 0x810c     */
  volatile u_int32_t MAC_PCU_PHY_ERROR_MASK;      /*     0x810c - 0x8110     */
  volatile u_int32_t MAC_PCU_XRLAT;               /*     0x8110 - 0x8114     */
  volatile u_int32_t MAC_PCU_RXBUF;               /*     0x8114 - 0x8118     */
  volatile u_int32_t MAC_PCU_MIC_QOS_CONTROL;     /*     0x8118 - 0x811c     */
  volatile u_int32_t MAC_PCU_MIC_QOS_SELECT;      /*     0x811c - 0x8120     */
  volatile u_int32_t MAC_PCU_MISC_MODE;           /*     0x8120 - 0x8124     */
  volatile u_int32_t MAC_PCU_FILTER_OFDM_CNT;     /*     0x8124 - 0x8128     */
  volatile u_int32_t MAC_PCU_FILTER_CCK_CNT;      /*     0x8128 - 0x812c     */
  volatile u_int32_t MAC_PCU_PHY_ERR_CNT_1;       /*     0x812c - 0x8130     */
  volatile u_int32_t MAC_PCU_PHY_ERR_CNT_1_MASK;  /*     0x8130 - 0x8134     */
  volatile u_int32_t MAC_PCU_PHY_ERR_CNT_2;       /*     0x8134 - 0x8138     */
  volatile u_int32_t MAC_PCU_PHY_ERR_CNT_2_MASK;  /*     0x8138 - 0x813c     */
  volatile u_int32_t MAC_PCU_TSF_THRESHOLD;       /*     0x813c - 0x8140     */
  volatile u_int32_t MAC_PCU_MISC_MODE4;          /*     0x8140 - 0x8144     */
  volatile u_int32_t MAC_PCU_PHY_ERROR_EIFS_MASK; /*     0x8144 - 0x8148     */
  volatile char pad__4[0x20];                     /*     0x8148 - 0x8168     */
  volatile u_int32_t MAC_PCU_PHY_ERR_CNT_3;       /*     0x8168 - 0x816c     */
  volatile u_int32_t MAC_PCU_PHY_ERR_CNT_3_MASK;  /*     0x816c - 0x8170     */
  volatile u_int32_t MAC_PCU_BLUETOOTH_MODE;      /*     0x8170 - 0x8174     */
  volatile u_int32_t MAC_PCU_BLUETOOTH_WL_WEIGHTS0;
                                                  /*     0x8174 - 0x8178     */
  volatile u_int32_t MAC_PCU_HCF_TIMEOUT;         /*     0x8178 - 0x817c     */
  volatile u_int32_t MAC_PCU_BLUETOOTH_MODE2;     /*     0x817c - 0x8180     */
  volatile u_int32_t MAC_PCU_GENERIC_TIMERS2[16]; /*     0x8180 - 0x81c0     */
  volatile u_int32_t MAC_PCU_GENERIC_TIMERS2_MODE;
                                                  /*     0x81c0 - 0x81c4     */
  volatile u_int32_t MAC_PCU_BLUETOOTH_WL_WEIGHTS1;
                                                  /*     0x81c4 - 0x81c8     */
  volatile u_int32_t MAC_PCU_BLUETOOTH_TSF_BT_ACTIVE;
                                                  /*     0x81c8 - 0x81cc     */
  volatile u_int32_t MAC_PCU_BLUETOOTH_TSF_BT_PRIORITY;
                                                  /*     0x81cc - 0x81d0     */
  volatile u_int32_t MAC_PCU_TXSIFS;              /*     0x81d0 - 0x81d4     */
  volatile u_int32_t MAC_PCU_BLUETOOTH_MODE3;     /*     0x81d4 - 0x81d8     */
  volatile char pad__5[0x14];                     /*     0x81d8 - 0x81ec     */
  volatile u_int32_t MAC_PCU_TXOP_X;              /*     0x81ec - 0x81f0     */
  volatile u_int32_t MAC_PCU_TXOP_0_3;            /*     0x81f0 - 0x81f4     */
  volatile u_int32_t MAC_PCU_TXOP_4_7;            /*     0x81f4 - 0x81f8     */
  volatile u_int32_t MAC_PCU_TXOP_8_11;           /*     0x81f8 - 0x81fc     */
  volatile u_int32_t MAC_PCU_TXOP_12_15;          /*     0x81fc - 0x8200     */
  volatile u_int32_t MAC_PCU_GENERIC_TIMERS[16];  /*     0x8200 - 0x8240     */
  volatile u_int32_t MAC_PCU_GENERIC_TIMERS_MODE; /*     0x8240 - 0x8244     */
  volatile u_int32_t MAC_PCU_SLP32_MODE;          /*     0x8244 - 0x8248     */
  volatile u_int32_t MAC_PCU_SLP32_WAKE;          /*     0x8248 - 0x824c     */
  volatile u_int32_t MAC_PCU_SLP32_INC;           /*     0x824c - 0x8250     */
  volatile u_int32_t MAC_PCU_SLP_MIB1;            /*     0x8250 - 0x8254     */
  volatile u_int32_t MAC_PCU_SLP_MIB2;            /*     0x8254 - 0x8258     */
  volatile u_int32_t MAC_PCU_SLP_MIB3;            /*     0x8258 - 0x825c     */
  volatile u_int32_t MAC_PCU_WOW1;                /*     0x825c - 0x8260     */
  volatile u_int32_t MAC_PCU_WOW2;                /*     0x8260 - 0x8264     */
  volatile u_int32_t MAC_PCU_LOGIC_ANALYZER;      /*     0x8264 - 0x8268     */
  volatile u_int32_t MAC_PCU_LOGIC_ANALYZER_32L;  /*     0x8268 - 0x826c     */
  volatile u_int32_t MAC_PCU_LOGIC_ANALYZER_16U;  /*     0x826c - 0x8270     */
  volatile u_int32_t MAC_PCU_WOW3_BEACON_FAIL;    /*     0x8270 - 0x8274     */
  volatile u_int32_t MAC_PCU_WOW3_BEACON;         /*     0x8274 - 0x8278     */
  volatile u_int32_t MAC_PCU_WOW3_KEEP_ALIVE;     /*     0x8278 - 0x827c     */
  volatile u_int32_t MAC_PCU_WOW_KA;              /*     0x827c - 0x8280     */
  volatile char pad__6[0x4];                      /*     0x8280 - 0x8284     */
  volatile u_int32_t PCU_1US;                     /*     0x8284 - 0x8288     */
  volatile u_int32_t PCU_KA;                      /*     0x8288 - 0x828c     */
  volatile u_int32_t WOW_EXACT;                   /*     0x828c - 0x8290     */
  volatile char pad__7[0x4];                      /*     0x8290 - 0x8294     */
  volatile u_int32_t PCU_WOW4;                    /*     0x8294 - 0x8298     */
  volatile u_int32_t PCU_WOW5;                    /*     0x8298 - 0x829c     */
  volatile u_int32_t MAC_PCU_PHY_ERR_CNT_MASK_CONT;
                                                  /*     0x829c - 0x82a0     */
  volatile char pad__8[0x60];                     /*     0x82a0 - 0x8300     */
  volatile u_int32_t MAC_PCU_AZIMUTH_MODE;        /*     0x8300 - 0x8304     */
  volatile char pad__9[0x10];                     /*     0x8304 - 0x8314     */
  volatile u_int32_t MAC_PCU_AZIMUTH_TIME_STAMP;  /*     0x8314 - 0x8318     */
  volatile u_int32_t MAC_PCU_20_40_MODE;          /*     0x8318 - 0x831c     */
  volatile u_int32_t MAC_PCU_H_XFER_TIMEOUT;      /*     0x831c - 0x8320     */
  volatile char pad__10[0x8];                     /*     0x8320 - 0x8328     */
  volatile u_int32_t MAC_PCU_RX_CLEAR_DIFF_CNT;   /*     0x8328 - 0x832c     */
  volatile u_int32_t MAC_PCU_SELF_GEN_ANTENNA_MASK;
                                                  /*     0x832c - 0x8330     */
  volatile u_int32_t MAC_PCU_BA_BAR_CONTROL;      /*     0x8330 - 0x8334     */
  volatile u_int32_t MAC_PCU_LEGACY_PLCP_SPOOF;   /*     0x8334 - 0x8338     */
  volatile u_int32_t MAC_PCU_PHY_ERROR_MASK_CONT; /*     0x8338 - 0x833c     */
  volatile u_int32_t MAC_PCU_TX_TIMER;            /*     0x833c - 0x8340     */
  volatile u_int32_t MAC_PCU_TXBUF_CTRL;          /*     0x8340 - 0x8344     */
  volatile u_int32_t MAC_PCU_MISC_MODE2;          /*     0x8344 - 0x8348     */
  volatile u_int32_t MAC_PCU_ALT_AES_MUTE_MASK;   /*     0x8348 - 0x834c     */
  volatile u_int32_t MAC_PCU_WOW6;                /*     0x834c - 0x8350     */
  volatile u_int32_t ASYNC_FIFO_REG1;             /*     0x8350 - 0x8354     */
  volatile u_int32_t ASYNC_FIFO_REG2;             /*     0x8354 - 0x8358     */
  volatile u_int32_t ASYNC_FIFO_REG3;             /*     0x8358 - 0x835c     */
  volatile u_int32_t MAC_PCU_WOW5;                /*     0x835c - 0x8360     */
  volatile u_int32_t MAC_PCU_WOW_LENGTH1;         /*     0x8360 - 0x8364     */
  volatile u_int32_t MAC_PCU_WOW_LENGTH2;         /*     0x8364 - 0x8368     */
  volatile u_int32_t WOW_PATTERN_MATCH_LESS_THAN_256_BYTES;
                                                  /*     0x8368 - 0x836c     */
  volatile char pad__11[0x4];                     /*     0x836c - 0x8370     */
  volatile u_int32_t MAC_PCU_WOW4;                /*     0x8370 - 0x8374     */
  volatile u_int32_t WOW2_EXACT;                  /*     0x8374 - 0x8378     */
  volatile u_int32_t PCU_WOW6;                    /*     0x8378 - 0x837c     */
  volatile u_int32_t PCU_WOW7;                    /*     0x837c - 0x8380     */
  volatile u_int32_t MAC_PCU_WOW_LENGTH3;         /*     0x8380 - 0x8384     */
  volatile u_int32_t MAC_PCU_WOW_LENGTH4;         /*     0x8384 - 0x8388     */
  volatile u_int32_t MAC_PCU_LOCATION_MODE_CONTROL;
                                                  /*     0x8388 - 0x838c     */
  volatile u_int32_t MAC_PCU_LOCATION_MODE_TIMER; /*     0x838c - 0x8390     */
  volatile u_int32_t MAC_PCU_TSF2_L32;            /*     0x8390 - 0x8394     */
  volatile u_int32_t MAC_PCU_TSF2_U32;            /*     0x8394 - 0x8398     */
  volatile u_int32_t MAC_PCU_BSSID2_L32;          /*     0x8398 - 0x839c     */
  volatile u_int32_t MAC_PCU_BSSID2_U16;          /*     0x839c - 0x83a0     */
  volatile u_int32_t MAC_PCU_DIRECT_CONNECT;      /*     0x83a0 - 0x83a4     */
  volatile u_int32_t MAC_PCU_TID_TO_AC;           /*     0x83a4 - 0x83a8     */
  volatile u_int32_t MAC_PCU_HP_QUEUE;            /*     0x83a8 - 0x83ac     */
  volatile u_int32_t MAC_PCU_BLUETOOTH_BT_WEIGHTS0;
                                                  /*     0x83ac - 0x83b0     */
  volatile u_int32_t MAC_PCU_BLUETOOTH_BT_WEIGHTS1;
                                                  /*     0x83b0 - 0x83b4     */
  volatile u_int32_t MAC_PCU_BLUETOOTH_BT_WEIGHTS2;
                                                  /*     0x83b4 - 0x83b8     */
  volatile u_int32_t MAC_PCU_BLUETOOTH_BT_WEIGHTS3;
                                                  /*     0x83b8 - 0x83bc     */
  volatile u_int32_t MAC_PCU_AGC_SATURATION_CNT0; /*     0x83bc - 0x83c0     */
  volatile u_int32_t MAC_PCU_AGC_SATURATION_CNT1; /*     0x83c0 - 0x83c4     */
  volatile u_int32_t MAC_PCU_AGC_SATURATION_CNT2; /*     0x83c4 - 0x83c8     */
  volatile u_int32_t MAC_PCU_HW_BCN_PROC1;        /*     0x83c8 - 0x83cc     */
  volatile u_int32_t MAC_PCU_HW_BCN_PROC2;        /*     0x83cc - 0x83d0     */
  volatile u_int32_t MAC_PCU_MISC_MODE3;          /*     0x83d0 - 0x83d4     */
  volatile u_int32_t MAC_PCU_FILTER_RSSI_AVE;     /*     0x83d4 - 0x83d8     */
  volatile u_int32_t MAC_PCU_PHY_ERROR_AIFS_MASK; /*     0x83d8 - 0x83dc     */
  volatile u_int32_t MAC_PCU_PS_FILTER;           /*     0x83dc - 0x83e0     */
  volatile u_int32_t MAC_PCU_AGC_SATURATION_CNT3; /*     0x83e0 - 0x83e4     */
  volatile char pad__12[0x1c];                    /*     0x83e4 - 0x8400     */
  volatile u_int32_t MAC_PCU_TXBUF_BA[64];        /*     0x8400 - 0x8500     */
  volatile char pad__13[0x300];                   /*     0x8500 - 0x8800     */
  volatile u_int32_t MAC_PCU_KEY_CACHE[1024];     /*     0x8800 - 0x9800     */
};

struct mac_pcu_buf_reg {
  volatile char pad__0[0xe000];                   /*        0x0 - 0xe000     */
  volatile u_int32_t MAC_PCU_BUF[2048];           /*     0xe000 - 0x10000    */
};

struct radio65_reg {
  volatile char pad__0[0x16000];                  /*        0x0 - 0x16000    */
  volatile u_int32_t ch0_RXRF_BIAS1;              /*    0x16000 - 0x16004    */
  volatile u_int32_t ch0_RXRF_BIAS2;              /*    0x16004 - 0x16008    */
  volatile u_int32_t ch0_RXRF_GAINSTAGES;         /*    0x16008 - 0x1600c    */
  volatile u_int32_t ch0_RXRF_AGC;                /*    0x1600c - 0x16010    */
  volatile char pad__1[0x30];                     /*    0x16010 - 0x16040    */
  volatile u_int32_t ch0_TXRF1;                   /*    0x16040 - 0x16044    */
  volatile u_int32_t ch0_TXRF2;                   /*    0x16044 - 0x16048    */
  volatile u_int32_t ch0_TXRF3;                   /*    0x16048 - 0x1604c    */
  volatile u_int32_t ch0_TXRF4;                   /*    0x1604c - 0x16050    */
  volatile u_int32_t ch0_TXRF5;                   /*    0x16050 - 0x16054    */
  volatile u_int32_t ch0_TXRF6;                   /*    0x16054 - 0x16058    */
  volatile char pad__2[0x28];                     /*    0x16058 - 0x16080    */
  volatile u_int32_t ch0_SYNTH1;                  /*    0x16080 - 0x16084    */
  volatile u_int32_t ch0_SYNTH2;                  /*    0x16084 - 0x16088    */
  volatile u_int32_t ch0_SYNTH3;                  /*    0x16088 - 0x1608c    */
  volatile u_int32_t ch0_SYNTH4;                  /*    0x1608c - 0x16090    */
  volatile u_int32_t ch0_SYNTH5;                  /*    0x16090 - 0x16094    */
  volatile u_int32_t ch0_SYNTH6;                  /*    0x16094 - 0x16098    */
  volatile u_int32_t ch0_SYNTH7;                  /*    0x16098 - 0x1609c    */
  volatile u_int32_t ch0_SYNTH8;                  /*    0x1609c - 0x160a0    */
  volatile u_int32_t ch0_SYNTH9;                  /*    0x160a0 - 0x160a4    */
  volatile u_int32_t ch0_SYNTH10;                 /*    0x160a4 - 0x160a8    */
  volatile u_int32_t ch0_SYNTH11;                 /*    0x160a8 - 0x160ac    */
  volatile u_int32_t ch0_SYNTH12;                 /*    0x160ac - 0x160b0    */
  volatile u_int32_t ch0_SYNTH13;                 /*    0x160b0 - 0x160b4    */
  volatile u_int32_t ch0_SYNTH14;                 /*    0x160b4 - 0x160b8    */
  volatile char pad__3[0x8];                      /*    0x160b8 - 0x160c0    */
  volatile u_int32_t ch0_BIAS1;                   /*    0x160c0 - 0x160c4    */
  volatile u_int32_t ch0_BIAS2;                   /*    0x160c4 - 0x160c8    */
  volatile u_int32_t ch0_BIAS3;                   /*    0x160c8 - 0x160cc    */
  volatile u_int32_t ch0_BIAS4;                   /*    0x160cc - 0x160d0    */
  volatile u_int32_t ch0_BIAS5;                   /*    0x160d0 - 0x160d4    */
  volatile char pad__4[0x2c];                     /*    0x160d4 - 0x16100    */
  volatile u_int32_t ch0_RXTX1;                   /*    0x16100 - 0x16104    */
  volatile u_int32_t ch0_RXTX2;                   /*    0x16104 - 0x16108    */
  volatile u_int32_t ch0_RXTX3;                   /*    0x16108 - 0x1610c    */
  volatile u_int32_t ch0_RXTX4;                   /*    0x1610c - 0x16110    */
  volatile char pad__5[0x30];                     /*    0x16110 - 0x16140    */
  volatile u_int32_t ch0_BB1;                     /*    0x16140 - 0x16144    */
  volatile u_int32_t ch0_BB2;                     /*    0x16144 - 0x16148    */
  volatile u_int32_t ch0_BB3;                     /*    0x16148 - 0x1614c    */
  volatile char pad__6[0x34];                     /*    0x1614c - 0x16180    */
  volatile u_int32_t ch0_BB_PLL;                  /*    0x16180 - 0x16184    */
  volatile u_int32_t ch0_BB_PLL2;                 /*    0x16184 - 0x16188    */
  volatile u_int32_t ch0_BB_PLL3;                 /*    0x16188 - 0x1618c    */
  volatile char pad__7[0x34];                     /*    0x1618c - 0x161c0    */
  volatile u_int32_t ch0_CPU_PLL;                 /*    0x161c0 - 0x161c4    */
  volatile u_int32_t ch0_CPU_PLL2;                /*    0x161c4 - 0x161c8    */
  volatile u_int32_t ch0_CPU_PLL3;                /*    0x161c8 - 0x161cc    */
  volatile char pad__8[0x74];                     /*    0x161cc - 0x16240    */
  volatile u_int32_t ch0_DDR_PLL;                 /*    0x16240 - 0x16244    */
  volatile u_int32_t ch0_DDR_PLL2;                /*    0x16244 - 0x16248    */
  volatile u_int32_t ch0_DDR_PLL3;                /*    0x16248 - 0x1624c    */
  volatile char pad__9[0x34];                     /*    0x1624c - 0x16280    */
  volatile u_int32_t ch0_TOP;                     /*    0x16280 - 0x16284    */
  volatile u_int32_t ch0_TOP2;                    /*    0x16284 - 0x16288    */
  volatile u_int32_t ch0_TOP3;                    /*    0x16288 - 0x1628c    */
  volatile u_int32_t ch0_THERM;                   /*    0x1628c - 0x16290    */
  volatile char pad__10[0x30];                    /*    0x16290 - 0x162c0    */
  volatile u_int32_t ch0_XTAL;                    /*    0x162c0 - 0x162c4    */
  volatile u_int32_t ch0_XTAL2;                   /*    0x162c4 - 0x162c8    */
  volatile u_int32_t ch0_XTAL3;                   /*    0x162c8 - 0x162cc    */
  volatile char pad__11[0xb4];                    /*    0x162cc - 0x16380    */
  volatile u_int32_t ch0_rbist_cntrl;             /*    0x16380 - 0x16384    */
  volatile u_int32_t ch0_tx_dc_offset;            /*    0x16384 - 0x16388    */
  volatile u_int32_t ch0_tx_tonegen0;             /*    0x16388 - 0x1638c    */
  volatile u_int32_t ch0_tx_tonegen1;             /*    0x1638c - 0x16390    */
  volatile u_int32_t ch0_tx_lftonegen0;           /*    0x16390 - 0x16394    */
  volatile u_int32_t ch0_tx_linear_ramp_i;        /*    0x16394 - 0x16398    */
  volatile u_int32_t ch0_tx_linear_ramp_q;        /*    0x16398 - 0x1639c    */
  volatile u_int32_t ch0_tx_prbs_mag;             /*    0x1639c - 0x163a0    */
  volatile u_int32_t ch0_tx_prbs_seed_i;          /*    0x163a0 - 0x163a4    */
  volatile u_int32_t ch0_tx_prbs_seed_q;          /*    0x163a4 - 0x163a8    */
  volatile u_int32_t ch0_cmac_dc_cancel;          /*    0x163a8 - 0x163ac    */
  volatile u_int32_t ch0_cmac_dc_offset;          /*    0x163ac - 0x163b0    */
  volatile u_int32_t ch0_cmac_corr;               /*    0x163b0 - 0x163b4    */
  volatile u_int32_t ch0_cmac_power;              /*    0x163b4 - 0x163b8    */
  volatile u_int32_t ch0_cmac_cross_corr;         /*    0x163b8 - 0x163bc    */
  volatile u_int32_t ch0_cmac_i2q2;               /*    0x163bc - 0x163c0    */
  volatile u_int32_t ch0_cmac_power_hpf;          /*    0x163c0 - 0x163c4    */
  volatile u_int32_t ch0_rxdac_set1;              /*    0x163c4 - 0x163c8    */
  volatile u_int32_t ch0_rxdac_set2;              /*    0x163c8 - 0x163cc    */
  volatile u_int32_t ch0_rxdac_long_shift;        /*    0x163cc - 0x163d0    */
  volatile u_int32_t ch0_cmac_results_i;          /*    0x163d0 - 0x163d4    */
  volatile u_int32_t ch0_cmac_results_q;          /*    0x163d4 - 0x163d8    */
  volatile char pad__12[0x28];                    /*    0x163d8 - 0x16400    */
  volatile u_int32_t ch1_RXRF_BIAS1;              /*    0x16400 - 0x16404    */
  volatile u_int32_t ch1_RXRF_BIAS2;              /*    0x16404 - 0x16408    */
  volatile u_int32_t ch1_RXRF_GAINSTAGES;         /*    0x16408 - 0x1640c    */
  volatile u_int32_t ch1_RXRF_AGC;                /*    0x1640c - 0x16410    */
  volatile char pad__13[0x30];                    /*    0x16410 - 0x16440    */
  volatile u_int32_t ch1_TXRF1;                   /*    0x16440 - 0x16444    */
  volatile u_int32_t ch1_TXRF2;                   /*    0x16444 - 0x16448    */
  volatile u_int32_t ch1_TXRF3;                   /*    0x16448 - 0x1644c    */
  volatile u_int32_t ch1_TXRF4;                   /*    0x1644c - 0x16450    */
  volatile u_int32_t ch1_TXRF5;                   /*    0x16450 - 0x16454    */
  volatile u_int32_t ch1_TXRF6;                   /*    0x16454 - 0x16458    */
  volatile char pad__14[0xa8];                    /*    0x16458 - 0x16500    */
  volatile u_int32_t ch1_RXTX1;                   /*    0x16500 - 0x16504    */
  volatile u_int32_t ch1_RXTX2;                   /*    0x16504 - 0x16508    */
  volatile u_int32_t ch1_RXTX3;                   /*    0x16508 - 0x1650c    */
  volatile u_int32_t ch1_RXTX4;                   /*    0x1650c - 0x16510    */
  volatile char pad__15[0x30];                    /*    0x16510 - 0x16540    */
  volatile u_int32_t ch1_BB1;                     /*    0x16540 - 0x16544    */
  volatile u_int32_t ch1_BB2;                     /*    0x16544 - 0x16548    */
  volatile u_int32_t ch1_BB3;                     /*    0x16548 - 0x1654c    */
  volatile char pad__16[0x234];                   /*    0x1654c - 0x16780    */
  volatile u_int32_t ch1_rbist_cntrl;             /*    0x16780 - 0x16784    */
  volatile u_int32_t ch1_tx_dc_offset;            /*    0x16784 - 0x16788    */
  volatile u_int32_t ch1_tx_tonegen0;             /*    0x16788 - 0x1678c    */
  volatile u_int32_t ch1_tx_tonegen1;             /*    0x1678c - 0x16790    */
  volatile u_int32_t ch1_tx_lftonegen0;           /*    0x16790 - 0x16794    */
  volatile u_int32_t ch1_tx_linear_ramp_i;        /*    0x16794 - 0x16798    */
  volatile u_int32_t ch1_tx_linear_ramp_q;        /*    0x16798 - 0x1679c    */
  volatile u_int32_t ch1_tx_prbs_mag;             /*    0x1679c - 0x167a0    */
  volatile u_int32_t ch1_tx_prbs_seed_i;          /*    0x167a0 - 0x167a4    */
  volatile u_int32_t ch1_tx_prbs_seed_q;          /*    0x167a4 - 0x167a8    */
  volatile u_int32_t ch1_cmac_dc_cancel;          /*    0x167a8 - 0x167ac    */
  volatile u_int32_t ch1_cmac_dc_offset;          /*    0x167ac - 0x167b0    */
  volatile u_int32_t ch1_cmac_corr;               /*    0x167b0 - 0x167b4    */
  volatile u_int32_t ch1_cmac_power;              /*    0x167b4 - 0x167b8    */
  volatile u_int32_t ch1_cmac_cross_corr;         /*    0x167b8 - 0x167bc    */
  volatile u_int32_t ch1_cmac_i2q2;               /*    0x167bc - 0x167c0    */
  volatile u_int32_t ch1_cmac_power_hpf;          /*    0x167c0 - 0x167c4    */
  volatile u_int32_t ch1_rxdac_set1;              /*    0x167c4 - 0x167c8    */
  volatile u_int32_t ch1_rxdac_set2;              /*    0x167c8 - 0x167cc    */
  volatile u_int32_t ch1_rxdac_long_shift;        /*    0x167cc - 0x167d0    */
  volatile u_int32_t ch1_cmac_results_i;          /*    0x167d0 - 0x167d4    */
  volatile u_int32_t ch1_cmac_results_q;          /*    0x167d4 - 0x167d8    */
  volatile char pad__17[0x28];                    /*    0x167d8 - 0x16800    */
  volatile u_int32_t ch2_RXRF_BIAS1;              /*    0x16800 - 0x16804    */
  volatile u_int32_t ch2_RXRF_BIAS2;              /*    0x16804 - 0x16808    */
  volatile u_int32_t ch2_RXRF_GAINSTAGES;         /*    0x16808 - 0x1680c    */
  volatile u_int32_t ch2_RXRF_AGC;                /*    0x1680c - 0x16810    */
  volatile char pad__18[0x30];                    /*    0x16810 - 0x16840    */
  volatile u_int32_t ch2_TXRF1;                   /*    0x16840 - 0x16844    */
  volatile u_int32_t ch2_TXRF2;                   /*    0x16844 - 0x16848    */
  volatile u_int32_t ch2_TXRF3;                   /*    0x16848 - 0x1684c    */
  volatile u_int32_t ch2_TXRF4;                   /*    0x1684c - 0x16850    */
  volatile u_int32_t ch2_TXRF5;                   /*    0x16850 - 0x16854    */
  volatile u_int32_t ch2_TXRF6;                   /*    0x16854 - 0x16858    */
  volatile char pad__19[0xa8];                    /*    0x16858 - 0x16900    */
  volatile u_int32_t ch2_RXTX1;                   /*    0x16900 - 0x16904    */
  volatile u_int32_t ch2_RXTX2;                   /*    0x16904 - 0x16908    */
  volatile u_int32_t ch2_RXTX3;                   /*    0x16908 - 0x1690c    */
  volatile u_int32_t ch2_RXTX4;                   /*    0x1690c - 0x16910    */
  volatile char pad__20[0x30];                    /*    0x16910 - 0x16940    */
  volatile u_int32_t ch2_BB1;                     /*    0x16940 - 0x16944    */
  volatile u_int32_t ch2_BB2;                     /*    0x16944 - 0x16948    */
  volatile u_int32_t ch2_BB3;                     /*    0x16948 - 0x1694c    */
  volatile char pad__21[0x234];                   /*    0x1694c - 0x16b80    */
  volatile u_int32_t ch2_rbist_cntrl;             /*    0x16b80 - 0x16b84    */
  volatile u_int32_t ch2_tx_dc_offset;            /*    0x16b84 - 0x16b88    */
  volatile u_int32_t ch2_tx_tonegen0;             /*    0x16b88 - 0x16b8c    */
  volatile u_int32_t ch2_tx_tonegen1;             /*    0x16b8c - 0x16b90    */
  volatile u_int32_t ch2_tx_lftonegen0;           /*    0x16b90 - 0x16b94    */
  volatile u_int32_t ch2_tx_linear_ramp_i;        /*    0x16b94 - 0x16b98    */
  volatile u_int32_t ch2_tx_linear_ramp_q;        /*    0x16b98 - 0x16b9c    */
  volatile u_int32_t ch2_tx_prbs_mag;             /*    0x16b9c - 0x16ba0    */
  volatile u_int32_t ch2_tx_prbs_seed_i;          /*    0x16ba0 - 0x16ba4    */
  volatile u_int32_t ch2_tx_prbs_seed_q;          /*    0x16ba4 - 0x16ba8    */
  volatile u_int32_t ch2_cmac_dc_cancel;          /*    0x16ba8 - 0x16bac    */
  volatile u_int32_t ch2_cmac_dc_offset;          /*    0x16bac - 0x16bb0    */
  volatile u_int32_t ch2_cmac_corr;               /*    0x16bb0 - 0x16bb4    */
  volatile u_int32_t ch2_cmac_power;              /*    0x16bb4 - 0x16bb8    */
  volatile u_int32_t ch2_cmac_cross_corr;         /*    0x16bb8 - 0x16bbc    */
  volatile u_int32_t ch2_cmac_i2q2;               /*    0x16bbc - 0x16bc0    */
  volatile u_int32_t ch2_cmac_power_hpf;          /*    0x16bc0 - 0x16bc4    */
  volatile u_int32_t ch2_rxdac_set1;              /*    0x16bc4 - 0x16bc8    */
  volatile u_int32_t ch2_rxdac_set2;              /*    0x16bc8 - 0x16bcc    */
  volatile u_int32_t ch2_rxdac_long_shift;        /*    0x16bcc - 0x16bd0    */
  volatile u_int32_t ch2_cmac_results_i;          /*    0x16bd0 - 0x16bd4    */
  volatile u_int32_t ch2_cmac_results_q;          /*    0x16bd4 - 0x16bd8    */
};

struct chn_reg_map {
  volatile u_int32_t BB_timing_controls_1;        /*        0x0 - 0x4        */
  volatile u_int32_t BB_timing_controls_2;        /*        0x4 - 0x8        */
  volatile u_int32_t BB_timing_controls_3;        /*        0x8 - 0xc        */
  volatile u_int32_t BB_timing_control_4;         /*        0xc - 0x10       */
  volatile u_int32_t BB_timing_control_5;         /*       0x10 - 0x14       */
  volatile u_int32_t BB_timing_control_6;         /*       0x14 - 0x18       */
  volatile u_int32_t BB_timing_control_11;        /*       0x18 - 0x1c       */
  volatile u_int32_t BB_spur_mask_controls;       /*       0x1c - 0x20       */
  volatile u_int32_t BB_find_signal_low;          /*       0x20 - 0x24       */
  volatile u_int32_t BB_sfcorr;                   /*       0x24 - 0x28       */
  volatile u_int32_t BB_self_corr_low;            /*       0x28 - 0x2c       */
  volatile u_int32_t BB_ext_chan_scorr_thr;       /*       0x2c - 0x30       */
  volatile u_int32_t BB_ext_chan_pwr_thr_2_b0;    /*       0x30 - 0x34       */
  volatile u_int32_t BB_radar_detection;          /*       0x34 - 0x38       */
  volatile u_int32_t BB_radar_detection_2;        /*       0x38 - 0x3c       */
  volatile u_int32_t BB_extension_radar;          /*       0x3c - 0x40       */
  volatile u_int32_t BB_tx_preemp_coef_01_b0;     /*       0x40 - 0x44       */
  volatile u_int32_t BB_tx_preemp_coef_23_b0;     /*       0x44 - 0x48       */
  volatile u_int32_t BB_tx_preemp_coef_45_b0;     /*       0x48 - 0x4c       */
  volatile u_int32_t BB_tx_preemp_coef_67_b0;     /*       0x4c - 0x50       */
  volatile u_int32_t BB_tx_preemp_coef_8_b0;      /*       0x50 - 0x54       */
  volatile u_int32_t BB_tx_notch_coef_b0;         /*       0x54 - 0x58       */
  volatile u_int32_t BB_spur_freq_phase_b0;       /*       0x58 - 0x5c       */
  volatile char pad__0[0x24];                     /*       0x5c - 0x80       */
  volatile u_int32_t BB_multichain_control;       /*       0x80 - 0x84       */
  volatile u_int32_t BB_per_chain_csd;            /*       0x84 - 0x88       */
  volatile u_int32_t BB_multichain_control_2;     /*       0x88 - 0x8c       */
  volatile u_int32_t BB_paprd_ctrl3;              /*       0x8c - 0x90       */
  volatile u_int32_t BB_paprd_4ss_mask_ctrl;      /*       0x90 - 0x94       */
  volatile char pad__1[0xc];                      /*       0x94 - 0xa0       */
  volatile u_int32_t BB_tx_crc;                   /*       0xa0 - 0xa4       */
  volatile u_int32_t BB_tstdac_constant;          /*       0xa4 - 0xa8       */
  volatile u_int32_t BB_spur_report_b0;           /*       0xa8 - 0xac       */
  volatile char pad__2[0x4];                      /*       0xac - 0xb0       */
  volatile u_int32_t BB_txiqcal_control_3;        /*       0xb0 - 0xb4       */
  volatile char pad__3[0x8];                      /*       0xb4 - 0xbc       */
  volatile u_int32_t BB_green_tx_control_1;       /*       0xbc - 0xc0       */
  volatile u_int32_t BB_iq_adc_meas_0_b0;         /*       0xc0 - 0xc4       */
  volatile u_int32_t BB_iq_adc_meas_1_b0;         /*       0xc4 - 0xc8       */
  volatile u_int32_t BB_iq_adc_meas_2_b0;         /*       0xc8 - 0xcc       */
  volatile u_int32_t BB_iq_adc_meas_3_b0;         /*       0xcc - 0xd0       */
  volatile u_int32_t BB_tx_phase_ramp_b0;         /*       0xd0 - 0xd4       */
  volatile u_int32_t BB_adc_gain_dc_corr_b0;      /*       0xd4 - 0xd8       */
  volatile char pad__4[0x4];                      /*       0xd8 - 0xdc       */
  volatile u_int32_t BB_rx_iq_corr_b0;            /*       0xdc - 0xe0       */
  volatile char pad__5[0x4];                      /*       0xe0 - 0xe4       */
  volatile u_int32_t BB_paprd_am2am_mask;         /*       0xe4 - 0xe8       */
  volatile u_int32_t BB_paprd_am2pm_mask;         /*       0xe8 - 0xec       */
  volatile u_int32_t BB_paprd_ht40_mask;          /*       0xec - 0xf0       */
  volatile u_int32_t BB_paprd_ctrl0_b0;           /*       0xf0 - 0xf4       */
  volatile u_int32_t BB_paprd_ctrl1_b0;           /*       0xf4 - 0xf8       */
  volatile u_int32_t BB_pa_gain123_b0;            /*       0xf8 - 0xfc       */
  volatile u_int32_t BB_pa_gain45_b0;             /*       0xfc - 0x100      */
  volatile u_int32_t BB_paprd_pre_post_scale_0_b0;
                                                  /*      0x100 - 0x104      */
  volatile u_int32_t BB_paprd_pre_post_scale_1_b0;
                                                  /*      0x104 - 0x108      */
  volatile u_int32_t BB_paprd_pre_post_scale_2_b0;
                                                  /*      0x108 - 0x10c      */
  volatile u_int32_t BB_paprd_pre_post_scale_3_b0;
                                                  /*      0x10c - 0x110      */
  volatile u_int32_t BB_paprd_pre_post_scale_4_b0;
                                                  /*      0x110 - 0x114      */
  volatile u_int32_t BB_paprd_pre_post_scale_5_b0;
                                                  /*      0x114 - 0x118      */
  volatile u_int32_t BB_paprd_pre_post_scale_6_b0;
                                                  /*      0x118 - 0x11c      */
  volatile u_int32_t BB_paprd_pre_post_scale_7_b0;
                                                  /*      0x11c - 0x120      */
  volatile u_int32_t BB_paprd_mem_tab_b0[120];    /*      0x120 - 0x300      */
  volatile u_int32_t BB_chan_info_chan_tab_b0[60];
                                                  /*      0x300 - 0x3f0      */
  volatile u_int32_t BB_chn_tables_intf_addr;     /*      0x3f0 - 0x3f4      */
  volatile u_int32_t BB_chn_tables_intf_data;     /*      0x3f4 - 0x3f8      */
};

struct mrc_reg_map {
  volatile u_int32_t BB_timing_control_3a;        /*        0x0 - 0x4        */
  volatile u_int32_t BB_ldpc_cntl1;               /*        0x4 - 0x8        */
  volatile u_int32_t BB_ldpc_cntl2;               /*        0x8 - 0xc        */
  volatile u_int32_t BB_pilot_spur_mask;          /*        0xc - 0x10       */
  volatile u_int32_t BB_chan_spur_mask;           /*       0x10 - 0x14       */
  volatile u_int32_t BB_short_gi_delta_slope;     /*       0x14 - 0x18       */
  volatile u_int32_t BB_ml_cntl1;                 /*       0x18 - 0x1c       */
  volatile u_int32_t BB_ml_cntl2;                 /*       0x1c - 0x20       */
  volatile u_int32_t BB_tstadc;                   /*       0x20 - 0x24       */
  volatile u_int32_t BB_pilot_spur_mask_2;        /*       0x24 - 0x28       */
  volatile u_int32_t BB_chan_spur_mask_2;         /*       0x28 - 0x2c       */
  volatile u_int32_t BB_ml_cntl3;                 /*       0x2c - 0x30       */
};

struct bbb_reg_map {
  volatile u_int32_t BB_bbb_rx_ctrl_1;            /*        0x0 - 0x4        */
  volatile u_int32_t BB_bbb_rx_ctrl_2;            /*        0x4 - 0x8        */
  volatile u_int32_t BB_bbb_rx_ctrl_3;            /*        0x8 - 0xc        */
  volatile u_int32_t BB_bbb_rx_ctrl_4;            /*        0xc - 0x10       */
  volatile u_int32_t BB_bbb_rx_ctrl_5;            /*       0x10 - 0x14       */
  volatile u_int32_t BB_bbb_rx_ctrl_6;            /*       0x14 - 0x18       */
  volatile u_int32_t BB_force_clken_cck;          /*       0x18 - 0x1c       */
};

struct agc_reg_map {
  volatile u_int32_t BB_settling_time;            /*        0x0 - 0x4        */
  volatile u_int32_t BB_gain_force_max_gains_b0;  /*        0x4 - 0x8        */
  volatile u_int32_t BB_gains_min_offsets;        /*        0x8 - 0xc        */
  volatile u_int32_t BB_desired_sigsize;          /*        0xc - 0x10       */
  volatile u_int32_t BB_find_signal;              /*       0x10 - 0x14       */
  volatile u_int32_t BB_agc;                      /*       0x14 - 0x18       */
  volatile u_int32_t BB_ext_atten_switch_ctl_b0;  /*       0x18 - 0x1c       */
  volatile u_int32_t BB_cca_b0;                   /*       0x1c - 0x20       */
  volatile u_int32_t BB_cca_ctrl_2_b0;            /*       0x20 - 0x24       */
  volatile u_int32_t BB_restart;                  /*       0x24 - 0x28       */
  volatile u_int32_t BB_multichain_gain_ctrl;     /*       0x28 - 0x2c       */
  volatile u_int32_t BB_ext_chan_pwr_thr_1;       /*       0x2c - 0x30       */
  volatile u_int32_t BB_ext_chan_detect_win;      /*       0x30 - 0x34       */
  volatile u_int32_t BB_pwr_thr_20_40_det;        /*       0x34 - 0x38       */
  volatile u_int32_t BB_rifs_srch;                /*       0x38 - 0x3c       */
  volatile u_int32_t BB_peak_det_ctrl_1;          /*       0x3c - 0x40       */
  volatile u_int32_t BB_peak_det_ctrl_2;          /*       0x40 - 0x44       */
  volatile u_int32_t BB_rx_gain_bounds_1;         /*       0x44 - 0x48       */
  volatile u_int32_t BB_rx_gain_bounds_2;         /*       0x48 - 0x4c       */
  volatile u_int32_t BB_peak_det_cal_ctrl;        /*       0x4c - 0x50       */
  volatile u_int32_t BB_agc_dig_dc_ctrl;          /*       0x50 - 0x54       */
  volatile u_int32_t BB_bt_coex_1;                /*       0x54 - 0x58       */
  volatile u_int32_t BB_bt_coex_2;                /*       0x58 - 0x5c       */
  volatile u_int32_t BB_bt_coex_3;                /*       0x5c - 0x60       */
  volatile u_int32_t BB_bt_coex_4;                /*       0x60 - 0x64       */
  volatile u_int32_t BB_bt_coex_5;                /*       0x64 - 0x68       */
  volatile u_int32_t BB_redpwr_ctrl_1;            /*       0x68 - 0x6c       */
  volatile u_int32_t BB_redpwr_ctrl_2;            /*       0x6c - 0x70       */
  volatile char pad__0[0x110];                    /*       0x70 - 0x180      */
  volatile u_int32_t BB_rssi_b0;                  /*      0x180 - 0x184      */
  volatile u_int32_t BB_spur_est_cck_report_b0;   /*      0x184 - 0x188      */
  volatile u_int32_t BB_agc_dig_dc_status_i_b0;   /*      0x188 - 0x18c      */
  volatile u_int32_t BB_agc_dig_dc_status_q_b0;   /*      0x18c - 0x190      */
  volatile u_int32_t BB_dc_cal_status_b0;         /*      0x190 - 0x194      */
  volatile char pad__1[0x2c];                     /*      0x194 - 0x1c0      */
  volatile u_int32_t BB_bbb_sig_detect;           /*      0x1c0 - 0x1c4      */
  volatile u_int32_t BB_bbb_dagc_ctrl;            /*      0x1c4 - 0x1c8      */
  volatile u_int32_t BB_iqcorr_ctrl_cck;          /*      0x1c8 - 0x1cc      */
  volatile u_int32_t BB_cck_spur_mit;             /*      0x1cc - 0x1d0      */
  volatile u_int32_t BB_mrc_cck_ctrl;             /*      0x1d0 - 0x1d4      */
  volatile u_int32_t BB_cck_blocker_det;          /*      0x1d4 - 0x1d8      */
  volatile char pad__2[0x28];                     /*      0x1d8 - 0x200      */
  volatile u_int32_t BB_rx_ocgain[128];           /*      0x200 - 0x400      */
};

struct sm_reg_map {
  volatile u_int32_t BB_D2_chip_id;               /*        0x0 - 0x4        */
  volatile u_int32_t BB_gen_controls;             /*        0x4 - 0x8        */
  volatile u_int32_t BB_modes_select;             /*        0x8 - 0xc        */
  volatile u_int32_t BB_active;                   /*        0xc - 0x10       */
  volatile char pad__0[0x8];                      /*       0x10 - 0x18       */
  volatile u_int32_t BB_vit_spur_mask_A;          /*       0x18 - 0x1c       */
  volatile u_int32_t BB_vit_spur_mask_B;          /*       0x1c - 0x20       */
  volatile u_int32_t BB_vit_spur_mask_C;          /*       0x20 - 0x24       */
  volatile u_int32_t BB_vit_spur_mask_D;          /*       0x24 - 0x28       */
  volatile u_int32_t BB_spectral_scan;            /*       0x28 - 0x2c       */
  volatile u_int32_t BB_radar_bw_filter;          /*       0x2c - 0x30       */
  volatile u_int32_t BB_search_start_delay;       /*       0x30 - 0x34       */
  volatile u_int32_t BB_max_rx_length;            /*       0x34 - 0x38       */
  volatile u_int32_t BB_frame_control;            /*       0x38 - 0x3c       */
  volatile u_int32_t BB_rfbus_request;            /*       0x3c - 0x40       */
  volatile u_int32_t BB_rfbus_grant;              /*       0x40 - 0x44       */
  volatile u_int32_t BB_rifs;                     /*       0x44 - 0x48       */
  volatile u_int32_t BB_spectral_scan_2;          /*       0x48 - 0x4c       */
  volatile char pad__1[0x4];                      /*       0x4c - 0x50       */
  volatile u_int32_t BB_rx_clear_delay;           /*       0x50 - 0x54       */
  volatile u_int32_t BB_analog_power_on_time;     /*       0x54 - 0x58       */
  volatile u_int32_t BB_tx_timing_1;              /*       0x58 - 0x5c       */
  volatile u_int32_t BB_tx_timing_2;              /*       0x5c - 0x60       */
  volatile u_int32_t BB_tx_timing_3;              /*       0x60 - 0x64       */
  volatile u_int32_t BB_xpa_timing_control;       /*       0x64 - 0x68       */
  volatile u_int32_t BB_pef_4ss_mask_ctrl;        /*       0x68 - 0x6c       */
  volatile u_int32_t BB_notch_4ss_mask_ctrl;      /*       0x6c - 0x70       */
  volatile char pad__2[0x10];                     /*       0x70 - 0x80       */
  volatile u_int32_t BB_misc_pa_control;          /*       0x80 - 0x84       */
  volatile u_int32_t BB_switch_table_chn_b0;      /*       0x84 - 0x88       */
  volatile u_int32_t BB_switch_table_com1;        /*       0x88 - 0x8c       */
  volatile u_int32_t BB_switch_table_com2;        /*       0x8c - 0x90       */
  volatile char pad__3[0x10];                     /*       0x90 - 0xa0       */
  volatile u_int32_t BB_multichain_enable;        /*       0xa0 - 0xa4       */
  volatile char pad__4[0x1c];                     /*       0xa4 - 0xc0       */
  volatile u_int32_t BB_cal_chain_mask;           /*       0xc0 - 0xc4       */
  volatile u_int32_t BB_agc_control;              /*       0xc4 - 0xc8       */
  volatile u_int32_t BB_iq_adc_cal_mode;          /*       0xc8 - 0xcc       */
  volatile u_int32_t BB_fcal_1;                   /*       0xcc - 0xd0       */
  volatile u_int32_t BB_fcal_2_b0;                /*       0xd0 - 0xd4       */
  volatile u_int32_t BB_dft_tone_ctrl_b0;         /*       0xd4 - 0xd8       */
  volatile u_int32_t BB_cl_cal_ctrl;              /*       0xd8 - 0xdc       */
  volatile u_int32_t BB_cl_map_0_b0;              /*       0xdc - 0xe0       */
  volatile u_int32_t BB_cl_map_1_b0;              /*       0xe0 - 0xe4       */
  volatile u_int32_t BB_cl_map_2_b0;              /*       0xe4 - 0xe8       */
  volatile u_int32_t BB_cl_map_3_b0;              /*       0xe8 - 0xec       */
  volatile u_int32_t BB_cl_map_pal_0_b0;          /*       0xec - 0xf0       */
  volatile u_int32_t BB_cl_map_pal_1_b0;          /*       0xf0 - 0xf4       */
  volatile u_int32_t BB_cl_map_pal_2_b0;          /*       0xf4 - 0xf8       */
  volatile u_int32_t BB_cl_map_pal_3_b0;          /*       0xf8 - 0xfc       */
  volatile char pad__5[0x4];                      /*       0xfc - 0x100      */
  volatile u_int32_t BB_cl_tab_b0[16];            /*      0x100 - 0x140      */
  volatile u_int32_t BB_synth_control;            /*      0x140 - 0x144      */
  volatile u_int32_t BB_addac_clk_select;         /*      0x144 - 0x148      */
  volatile u_int32_t BB_pll_cntl;                 /*      0x148 - 0x14c      */
  volatile u_int32_t BB_analog_swap;              /*      0x14c - 0x150      */
  volatile u_int32_t BB_addac_parallel_control;   /*      0x150 - 0x154      */
  volatile char pad__6[0x4];                      /*      0x154 - 0x158      */
  volatile u_int32_t BB_force_analog;             /*      0x158 - 0x15c      */
  volatile u_int32_t BB_test_controls;            /*      0x15c - 0x160      */
  volatile u_int32_t BB_test_controls_status;     /*      0x160 - 0x164      */
  volatile u_int32_t BB_tstdac;                   /*      0x164 - 0x168      */
  volatile u_int32_t BB_channel_status;           /*      0x168 - 0x16c      */
  volatile u_int32_t BB_chaninfo_ctrl;            /*      0x16c - 0x170      */
  volatile u_int32_t BB_chan_info_noise_pwr;      /*      0x170 - 0x174      */
  volatile u_int32_t BB_chan_info_gain_diff;      /*      0x174 - 0x178      */
  volatile u_int32_t BB_chan_info_fine_timing;    /*      0x178 - 0x17c      */
  volatile u_int32_t BB_chan_info_gain_b0;        /*      0x17c - 0x180      */
  volatile u_int32_t BB_illegal_tx_rate;          /*      0x180 - 0x184      */
  volatile u_int32_t BB_scrambler_seed;           /*      0x184 - 0x188      */
  volatile u_int32_t BB_bbb_tx_ctrl;              /*      0x188 - 0x18c      */
  volatile u_int32_t BB_bbb_txfir_0;              /*      0x18c - 0x190      */
  volatile u_int32_t BB_bbb_txfir_1;              /*      0x190 - 0x194      */
  volatile u_int32_t BB_bbb_txfir_2;              /*      0x194 - 0x198      */
  volatile u_int32_t BB_heavy_clip_ctrl;          /*      0x198 - 0x19c      */
  volatile u_int32_t BB_heavy_clip_1;             /*      0x19c - 0x1a0      */
  volatile u_int32_t BB_heavy_clip_2;             /*      0x1a0 - 0x1a4      */
  volatile u_int32_t BB_heavy_clip_3;             /*      0x1a4 - 0x1a8      */
  volatile u_int32_t BB_heavy_clip_4;             /*      0x1a8 - 0x1ac      */
  volatile u_int32_t BB_heavy_clip_5;             /*      0x1ac - 0x1b0      */
  volatile u_int32_t BB_hc_fir_ht20_0;            /*      0x1b0 - 0x1b4      */
  volatile u_int32_t BB_hc_fir_ht20_1;            /*      0x1b4 - 0x1b8      */
  volatile u_int32_t BB_hc_fir_ht40_0;            /*      0x1b8 - 0x1bc      */
  volatile u_int32_t BB_hc_fir_ht40_1;            /*      0x1bc - 0x1c0      */
  volatile u_int32_t BB_powertx_rate1;            /*      0x1c0 - 0x1c4      */
  volatile u_int32_t BB_powertx_rate2;            /*      0x1c4 - 0x1c8      */
  volatile u_int32_t BB_powertx_rate3;            /*      0x1c8 - 0x1cc      */
  volatile u_int32_t BB_powertx_rate4;            /*      0x1cc - 0x1d0      */
  volatile u_int32_t BB_powertx_rate5;            /*      0x1d0 - 0x1d4      */
  volatile u_int32_t BB_powertx_rate6;            /*      0x1d4 - 0x1d8      */
  volatile u_int32_t BB_powertx_rate7;            /*      0x1d8 - 0x1dc      */
  volatile u_int32_t BB_powertx_rate8;            /*      0x1dc - 0x1e0      */
  volatile u_int32_t BB_powertx_rate9;            /*      0x1e0 - 0x1e4      */
  volatile u_int32_t BB_powertx_rate10;           /*      0x1e4 - 0x1e8      */
  volatile u_int32_t BB_powertx_rate11;           /*      0x1e8 - 0x1ec      */
  volatile u_int32_t BB_powertx_rate12;           /*      0x1ec - 0x1f0      */
  volatile u_int32_t BB_powertx_max;              /*      0x1f0 - 0x1f4      */
  volatile u_int32_t BB_powertx_sub;              /*      0x1f4 - 0x1f8      */
  volatile u_int32_t BB_tpc_1;                    /*      0x1f8 - 0x1fc      */
  volatile u_int32_t BB_tpc_2;                    /*      0x1fc - 0x200      */
  volatile u_int32_t BB_tpc_3;                    /*      0x200 - 0x204      */
  volatile u_int32_t BB_tpc_4_b0;                 /*      0x204 - 0x208      */
  volatile u_int32_t BB_tpc_5_b0;                 /*      0x208 - 0x20c      */
  volatile u_int32_t BB_tpc_6_b0;                 /*      0x20c - 0x210      */
  volatile u_int32_t BB_tpc_7;                    /*      0x210 - 0x214      */
  volatile u_int32_t BB_tpc_8;                    /*      0x214 - 0x218      */
  volatile u_int32_t BB_tpc_9;                    /*      0x218 - 0x21c      */
  volatile u_int32_t BB_tpc_10;                   /*      0x21c - 0x220      */
  volatile u_int32_t BB_tpc_11_b0;                /*      0x220 - 0x224      */
  volatile u_int32_t BB_tpc_12;                   /*      0x224 - 0x228      */
  volatile u_int32_t BB_tpc_13;                   /*      0x228 - 0x22c      */
  volatile u_int32_t BB_tpc_14;                   /*      0x22c - 0x230      */
  volatile u_int32_t BB_tpc_15;                   /*      0x230 - 0x234      */
  volatile u_int32_t BB_tpc_16;                   /*      0x234 - 0x238      */
  volatile u_int32_t BB_tpc_17;                   /*      0x238 - 0x23c      */
  volatile u_int32_t BB_tpc_18;                   /*      0x23c - 0x240      */
  volatile u_int32_t BB_tpc_19_b0;                /*      0x240 - 0x244      */
  volatile u_int32_t BB_tpc_20;                   /*      0x244 - 0x248      */
  volatile u_int32_t BB_therm_adc_1;              /*      0x248 - 0x24c      */
  volatile u_int32_t BB_therm_adc_2;              /*      0x24c - 0x250      */
  volatile u_int32_t BB_therm_adc_3;              /*      0x250 - 0x254      */
  volatile u_int32_t BB_therm_adc_4;              /*      0x254 - 0x258      */
  volatile u_int32_t BB_tx_forced_gain;           /*      0x258 - 0x25c      */
  volatile u_int32_t BB_powertx_rate13;           /*      0x25c - 0x260      */
  volatile u_int32_t BB_powertx_rate14;           /*      0x260 - 0x264      */
  volatile u_int32_t BB_tpc_21;                   /*      0x264 - 0x268      */
  volatile u_int32_t BB_tpc_22;                   /*      0x268 - 0x26c      */
  volatile char pad__7[0x14];                     /*      0x26c - 0x280      */
  volatile u_int32_t BB_pdadc_tab_b0[32];         /*      0x280 - 0x300      */
  volatile u_int32_t BB_tx_gain_tab_1;            /*      0x300 - 0x304      */
  volatile u_int32_t BB_tx_gain_tab_2;            /*      0x304 - 0x308      */
  volatile u_int32_t BB_tx_gain_tab_3;            /*      0x308 - 0x30c      */
  volatile u_int32_t BB_tx_gain_tab_4;            /*      0x30c - 0x310      */
  volatile u_int32_t BB_tx_gain_tab_5;            /*      0x310 - 0x314      */
  volatile u_int32_t BB_tx_gain_tab_6;            /*      0x314 - 0x318      */
  volatile u_int32_t BB_tx_gain_tab_7;            /*      0x318 - 0x31c      */
  volatile u_int32_t BB_tx_gain_tab_8;            /*      0x31c - 0x320      */
  volatile u_int32_t BB_tx_gain_tab_9;            /*      0x320 - 0x324      */
  volatile u_int32_t BB_tx_gain_tab_10;           /*      0x324 - 0x328      */
  volatile u_int32_t BB_tx_gain_tab_11;           /*      0x328 - 0x32c      */
  volatile u_int32_t BB_tx_gain_tab_12;           /*      0x32c - 0x330      */
  volatile u_int32_t BB_tx_gain_tab_13;           /*      0x330 - 0x334      */
  volatile u_int32_t BB_tx_gain_tab_14;           /*      0x334 - 0x338      */
  volatile u_int32_t BB_tx_gain_tab_15;           /*      0x338 - 0x33c      */
  volatile u_int32_t BB_tx_gain_tab_16;           /*      0x33c - 0x340      */
  volatile u_int32_t BB_tx_gain_tab_17;           /*      0x340 - 0x344      */
  volatile u_int32_t BB_tx_gain_tab_18;           /*      0x344 - 0x348      */
  volatile u_int32_t BB_tx_gain_tab_19;           /*      0x348 - 0x34c      */
  volatile u_int32_t BB_tx_gain_tab_20;           /*      0x34c - 0x350      */
  volatile u_int32_t BB_tx_gain_tab_21;           /*      0x350 - 0x354      */
  volatile u_int32_t BB_tx_gain_tab_22;           /*      0x354 - 0x358      */
  volatile u_int32_t BB_tx_gain_tab_23;           /*      0x358 - 0x35c      */
  volatile u_int32_t BB_tx_gain_tab_24;           /*      0x35c - 0x360      */
  volatile u_int32_t BB_tx_gain_tab_25;           /*      0x360 - 0x364      */
  volatile u_int32_t BB_tx_gain_tab_26;           /*      0x364 - 0x368      */
  volatile u_int32_t BB_tx_gain_tab_27;           /*      0x368 - 0x36c      */
  volatile u_int32_t BB_tx_gain_tab_28;           /*      0x36c - 0x370      */
  volatile u_int32_t BB_tx_gain_tab_29;           /*      0x370 - 0x374      */
  volatile u_int32_t BB_tx_gain_tab_30;           /*      0x374 - 0x378      */
  volatile u_int32_t BB_tx_gain_tab_31;           /*      0x378 - 0x37c      */
  volatile u_int32_t BB_tx_gain_tab_32;           /*      0x37c - 0x380      */
  volatile u_int32_t BB_rtt_ctrl;                 /*      0x380 - 0x384      */
  volatile u_int32_t BB_rtt_table_sw_intf_b0;     /*      0x384 - 0x388      */
  volatile u_int32_t BB_rtt_table_sw_intf_1_b0;   /*      0x388 - 0x38c      */
  volatile char pad__8[0x74];                     /*      0x38c - 0x400      */
  volatile u_int32_t BB_caltx_gain_set_0;         /*      0x400 - 0x404      */
  volatile u_int32_t BB_caltx_gain_set_2;         /*      0x404 - 0x408      */
  volatile u_int32_t BB_caltx_gain_set_4;         /*      0x408 - 0x40c      */
  volatile u_int32_t BB_caltx_gain_set_6;         /*      0x40c - 0x410      */
  volatile u_int32_t BB_caltx_gain_set_8;         /*      0x410 - 0x414      */
  volatile u_int32_t BB_caltx_gain_set_10;        /*      0x414 - 0x418      */
  volatile u_int32_t BB_caltx_gain_set_12;        /*      0x418 - 0x41c      */
  volatile u_int32_t BB_caltx_gain_set_14;        /*      0x41c - 0x420      */
  volatile u_int32_t BB_caltx_gain_set_16;        /*      0x420 - 0x424      */
  volatile u_int32_t BB_caltx_gain_set_18;        /*      0x424 - 0x428      */
  volatile u_int32_t BB_caltx_gain_set_20;        /*      0x428 - 0x42c      */
  volatile u_int32_t BB_caltx_gain_set_22;        /*      0x42c - 0x430      */
  volatile u_int32_t BB_caltx_gain_set_24;        /*      0x430 - 0x434      */
  volatile u_int32_t BB_caltx_gain_set_26;        /*      0x434 - 0x438      */
  volatile u_int32_t BB_caltx_gain_set_28;        /*      0x438 - 0x43c      */
  volatile u_int32_t BB_caltx_gain_set_30;        /*      0x43c - 0x440      */
  volatile char pad__9[0x4];                      /*      0x440 - 0x444      */
  volatile u_int32_t BB_txiqcal_control_0;        /*      0x444 - 0x448      */
  volatile u_int32_t BB_txiqcal_control_1;        /*      0x448 - 0x44c      */
  volatile u_int32_t BB_txiqcal_control_2;        /*      0x44c - 0x450      */
  volatile u_int32_t BB_txiq_corr_coeff_01_b0;    /*      0x450 - 0x454      */
  volatile u_int32_t BB_txiq_corr_coeff_23_b0;    /*      0x454 - 0x458      */
  volatile u_int32_t BB_txiq_corr_coeff_45_b0;    /*      0x458 - 0x45c      */
  volatile u_int32_t BB_txiq_corr_coeff_67_b0;    /*      0x45c - 0x460      */
  volatile u_int32_t BB_txiq_corr_coeff_89_b0;    /*      0x460 - 0x464      */
  volatile u_int32_t BB_txiq_corr_coeff_ab_b0;    /*      0x464 - 0x468      */
  volatile u_int32_t BB_txiq_corr_coeff_cd_b0;    /*      0x468 - 0x46c      */
  volatile u_int32_t BB_txiq_corr_coeff_ef_b0;    /*      0x46c - 0x470      */
  volatile u_int32_t BB_cal_rxbb_gain_tbl_0;      /*      0x470 - 0x474      */
  volatile u_int32_t BB_cal_rxbb_gain_tbl_4;      /*      0x474 - 0x478      */
  volatile u_int32_t BB_cal_rxbb_gain_tbl_8;      /*      0x478 - 0x47c      */
  volatile u_int32_t BB_cal_rxbb_gain_tbl_12;     /*      0x47c - 0x480      */
  volatile u_int32_t BB_cal_rxbb_gain_tbl_16;     /*      0x480 - 0x484      */
  volatile u_int32_t BB_cal_rxbb_gain_tbl_20;     /*      0x484 - 0x488      */
  volatile u_int32_t BB_cal_rxbb_gain_tbl_24;     /*      0x488 - 0x48c      */
  volatile u_int32_t BB_txiqcal_status_b0;        /*      0x48c - 0x490      */
  volatile u_int32_t BB_paprd_trainer_cntl1;      /*      0x490 - 0x494      */
  volatile u_int32_t BB_paprd_trainer_cntl2;      /*      0x494 - 0x498      */
  volatile u_int32_t BB_paprd_trainer_cntl3;      /*      0x498 - 0x49c      */
  volatile u_int32_t BB_paprd_trainer_cntl4;      /*      0x49c - 0x4a0      */
  volatile u_int32_t BB_paprd_trainer_stat1;      /*      0x4a0 - 0x4a4      */
  volatile u_int32_t BB_paprd_trainer_stat2;      /*      0x4a4 - 0x4a8      */
  volatile u_int32_t BB_paprd_trainer_stat3;      /*      0x4a8 - 0x4ac      */
  volatile char pad__10[0x24];                    /*      0x4ac - 0x4d0      */
  volatile u_int32_t BB_tx_preemp_cntl1;          /*      0x4d0 - 0x4d4      */
  volatile u_int32_t BB_tx_preemp_cntl2;          /*      0x4d4 - 0x4d8      */
  volatile u_int32_t BB_tx_notch_cntl1;           /*      0x4d8 - 0x4dc      */
  volatile u_int32_t BB_tx_notch_cntl2;           /*      0x4dc - 0x4e0      */
  volatile char pad__11[0xe0];                    /*      0x4e0 - 0x5c0      */
  volatile u_int32_t BB_watchdog_status;          /*      0x5c0 - 0x5c4      */
  volatile u_int32_t BB_watchdog_ctrl_1;          /*      0x5c4 - 0x5c8      */
  volatile u_int32_t BB_watchdog_ctrl_2;          /*      0x5c8 - 0x5cc      */
  volatile u_int32_t BB_bluetooth_cntl;           /*      0x5cc - 0x5d0      */
  volatile u_int32_t BB_phyonly_warm_reset;       /*      0x5d0 - 0x5d4      */
  volatile u_int32_t BB_phyonly_control;          /*      0x5d4 - 0x5d8      */
  volatile char pad__12[0x4];                     /*      0x5d8 - 0x5dc      */
  volatile u_int32_t BB_eco_ctrl;                 /*      0x5dc - 0x5e0      */
  volatile char pad__13[0x10];                    /*      0x5e0 - 0x5f0      */
  volatile u_int32_t BB_tables_intf_addr_b0;      /*      0x5f0 - 0x5f4      */
  volatile u_int32_t BB_tables_intf_data_b0;      /*      0x5f4 - 0x5f8      */
};

struct chn1_reg_map {
  volatile char pad__0[0x30];                     /*        0x0 - 0x30       */
  volatile u_int32_t BB_ext_chan_pwr_thr_2_b1;    /*       0x30 - 0x34       */
  volatile char pad__1[0xc];                      /*       0x34 - 0x40       */
  volatile u_int32_t BB_tx_preemp_coef_01_b1;     /*       0x40 - 0x44       */
  volatile u_int32_t BB_tx_preemp_coef_23_b1;     /*       0x44 - 0x48       */
  volatile u_int32_t BB_tx_preemp_coef_45_b1;     /*       0x48 - 0x4c       */
  volatile u_int32_t BB_tx_preemp_coef_67_b1;     /*       0x4c - 0x50       */
  volatile u_int32_t BB_tx_preemp_coef_8_b1;      /*       0x50 - 0x54       */
  volatile u_int32_t BB_tx_notch_coef_b1;         /*       0x54 - 0x58       */
  volatile u_int32_t BB_spur_freq_phase_b1;       /*       0x58 - 0x5c       */
  volatile char pad__2[0x4c];                     /*       0x5c - 0xa8       */
  volatile u_int32_t BB_spur_report_b1;           /*       0xa8 - 0xac       */
  volatile char pad__3[0x14];                     /*       0xac - 0xc0       */
  volatile u_int32_t BB_iq_adc_meas_0_b1;         /*       0xc0 - 0xc4       */
  volatile u_int32_t BB_iq_adc_meas_1_b1;         /*       0xc4 - 0xc8       */
  volatile u_int32_t BB_iq_adc_meas_2_b1;         /*       0xc8 - 0xcc       */
  volatile u_int32_t BB_iq_adc_meas_3_b1;         /*       0xcc - 0xd0       */
  volatile u_int32_t BB_tx_phase_ramp_b1;         /*       0xd0 - 0xd4       */
  volatile u_int32_t BB_adc_gain_dc_corr_b1;      /*       0xd4 - 0xd8       */
  volatile char pad__4[0x4];                      /*       0xd8 - 0xdc       */
  volatile u_int32_t BB_rx_iq_corr_b1;            /*       0xdc - 0xe0       */
  volatile char pad__5[0x10];                     /*       0xe0 - 0xf0       */
  volatile u_int32_t BB_paprd_ctrl0_b1;           /*       0xf0 - 0xf4       */
  volatile u_int32_t BB_paprd_ctrl1_b1;           /*       0xf4 - 0xf8       */
  volatile u_int32_t BB_pa_gain123_b1;            /*       0xf8 - 0xfc       */
  volatile u_int32_t BB_pa_gain45_b1;             /*       0xfc - 0x100      */
  volatile u_int32_t BB_paprd_pre_post_scale_0_b1;
                                                  /*      0x100 - 0x104      */
  volatile u_int32_t BB_paprd_pre_post_scale_1_b1;
                                                  /*      0x104 - 0x108      */
  volatile u_int32_t BB_paprd_pre_post_scale_2_b1;
                                                  /*      0x108 - 0x10c      */
  volatile u_int32_t BB_paprd_pre_post_scale_3_b1;
                                                  /*      0x10c - 0x110      */
  volatile u_int32_t BB_paprd_pre_post_scale_4_b1;
                                                  /*      0x110 - 0x114      */
  volatile u_int32_t BB_paprd_pre_post_scale_5_b1;
                                                  /*      0x114 - 0x118      */
  volatile u_int32_t BB_paprd_pre_post_scale_6_b1;
                                                  /*      0x118 - 0x11c      */
  volatile u_int32_t BB_paprd_pre_post_scale_7_b1;
                                                  /*      0x11c - 0x120      */
  volatile u_int32_t BB_paprd_mem_tab_b1[120];    /*      0x120 - 0x300      */
  volatile u_int32_t BB_chan_info_chan_tab_b1[60];
                                                  /*      0x300 - 0x3f0      */
  volatile u_int32_t BB_chn1_tables_intf_addr;    /*      0x3f0 - 0x3f4      */
  volatile u_int32_t BB_chn1_tables_intf_data;    /*      0x3f4 - 0x3f8      */
};

struct agc1_reg_map {
  volatile char pad__0[0x4];                      /*        0x0 - 0x4        */
  volatile u_int32_t BB_gain_force_max_gains_b1;  /*        0x4 - 0x8        */
  volatile char pad__1[0x10];                     /*        0x8 - 0x18       */
  volatile u_int32_t BB_ext_atten_switch_ctl_b1;  /*       0x18 - 0x1c       */
  volatile u_int32_t BB_cca_b1;                   /*       0x1c - 0x20       */
  volatile u_int32_t BB_cca_ctrl_2_b1;            /*       0x20 - 0x24       */
  volatile char pad__2[0x15c];                    /*       0x24 - 0x180      */
  volatile u_int32_t BB_rssi_b1;                  /*      0x180 - 0x184      */
  volatile u_int32_t BB_spur_est_cck_report_b1;   /*      0x184 - 0x188      */
  volatile u_int32_t BB_agc_dig_dc_status_i_b1;   /*      0x188 - 0x18c      */
  volatile u_int32_t BB_agc_dig_dc_status_q_b1;   /*      0x18c - 0x190      */
  volatile u_int32_t BB_dc_cal_status_b1;         /*      0x190 - 0x194      */
  volatile char pad__3[0x6c];                     /*      0x194 - 0x200      */
  volatile u_int32_t BB_rx_ocgain2[128];          /*      0x200 - 0x400      */
};

struct sm1_reg_map {
  volatile char pad__0[0x84];                     /*        0x0 - 0x84       */
  volatile u_int32_t BB_switch_table_chn_b1;      /*       0x84 - 0x88       */
  volatile char pad__1[0x48];                     /*       0x88 - 0xd0       */
  volatile u_int32_t BB_fcal_2_b1;                /*       0xd0 - 0xd4       */
  volatile u_int32_t BB_dft_tone_ctrl_b1;         /*       0xd4 - 0xd8       */
  volatile char pad__2[0x4];                      /*       0xd8 - 0xdc       */
  volatile u_int32_t BB_cl_map_0_b1;              /*       0xdc - 0xe0       */
  volatile u_int32_t BB_cl_map_1_b1;              /*       0xe0 - 0xe4       */
  volatile u_int32_t BB_cl_map_2_b1;              /*       0xe4 - 0xe8       */
  volatile u_int32_t BB_cl_map_3_b1;              /*       0xe8 - 0xec       */
  volatile u_int32_t BB_cl_map_pal_0_b1;          /*       0xec - 0xf0       */
  volatile u_int32_t BB_cl_map_pal_1_b1;          /*       0xf0 - 0xf4       */
  volatile u_int32_t BB_cl_map_pal_2_b1;          /*       0xf4 - 0xf8       */
  volatile u_int32_t BB_cl_map_pal_3_b1;          /*       0xf8 - 0xfc       */
  volatile char pad__3[0x4];                      /*       0xfc - 0x100      */
  volatile u_int32_t BB_cl_tab_b1[16];            /*      0x100 - 0x140      */
  volatile char pad__4[0x40];                     /*      0x140 - 0x180      */
  volatile u_int32_t BB_chan_info_gain_b1;        /*      0x180 - 0x184      */
  volatile char pad__5[0x80];                     /*      0x184 - 0x204      */
  volatile u_int32_t BB_tpc_4_b1;                 /*      0x204 - 0x208      */
  volatile u_int32_t BB_tpc_5_b1;                 /*      0x208 - 0x20c      */
  volatile u_int32_t BB_tpc_6_b1;                 /*      0x20c - 0x210      */
  volatile char pad__6[0x10];                     /*      0x210 - 0x220      */
  volatile u_int32_t BB_tpc_11_b1;                /*      0x220 - 0x224      */
  volatile char pad__7[0x18];                     /*      0x224 - 0x23c      */
  volatile u_int32_t BB_tpc_18_b1;                /*      0x23c - 0x240      */
  volatile u_int32_t BB_tpc_19_b1;                /*      0x240 - 0x244      */
  volatile char pad__8[0x3c];                     /*      0x244 - 0x280      */
  volatile u_int32_t BB_pdadc_tab_b1[32];         /*      0x280 - 0x300      */
  volatile char pad__9[0x84];                     /*      0x300 - 0x384      */
  volatile u_int32_t BB_rtt_table_sw_intf_b1;     /*      0x384 - 0x388      */
  volatile u_int32_t BB_rtt_table_sw_intf_1_b1;   /*      0x388 - 0x38c      */
  volatile char pad__10[0xc4];                    /*      0x38c - 0x450      */
  volatile u_int32_t BB_txiq_corr_coeff_01_b1;    /*      0x450 - 0x454      */
  volatile u_int32_t BB_txiq_corr_coeff_23_b1;    /*      0x454 - 0x458      */
  volatile u_int32_t BB_txiq_corr_coeff_45_b1;    /*      0x458 - 0x45c      */
  volatile u_int32_t BB_txiq_corr_coeff_67_b1;    /*      0x45c - 0x460      */
  volatile u_int32_t BB_txiq_corr_coeff_89_b1;    /*      0x460 - 0x464      */
  volatile u_int32_t BB_txiq_corr_coeff_ab_b1;    /*      0x464 - 0x468      */
  volatile u_int32_t BB_txiq_corr_coeff_cd_b1;    /*      0x468 - 0x46c      */
  volatile u_int32_t BB_txiq_corr_coeff_ef_b1;    /*      0x46c - 0x470      */
  volatile char pad__11[0x1c];                    /*      0x470 - 0x48c      */
  volatile u_int32_t BB_txiqcal_status_b1;        /*      0x48c - 0x490      */
  volatile char pad__12[0x160];                   /*      0x490 - 0x5f0      */
  volatile u_int32_t BB_tables_intf_addr_b1;      /*      0x5f0 - 0x5f4      */
  volatile u_int32_t BB_tables_intf_data_b1;      /*      0x5f4 - 0x5f8      */
};

struct chn2_reg_map {
  volatile char pad__0[0x30];                     /*        0x0 - 0x30       */
  volatile u_int32_t BB_ext_chan_pwr_thr_2_b2;    /*       0x30 - 0x34       */
  volatile char pad__1[0xc];                      /*       0x34 - 0x40       */
  volatile u_int32_t BB_tx_preemp_coef_01_b2;     /*       0x40 - 0x44       */
  volatile u_int32_t BB_tx_preemp_coef_23_b2;     /*       0x44 - 0x48       */
  volatile u_int32_t BB_tx_preemp_coef_45_b2;     /*       0x48 - 0x4c       */
  volatile u_int32_t BB_tx_preemp_coef_67_b2;     /*       0x4c - 0x50       */
  volatile u_int32_t BB_tx_preemp_coef_8_b2;      /*       0x50 - 0x54       */
  volatile u_int32_t BB_tx_notch_coef_b2;         /*       0x54 - 0x58       */
  volatile u_int32_t BB_spur_freq_phase_b2;       /*       0x58 - 0x5c       */
  volatile char pad__2[0x4c];                     /*       0x5c - 0xa8       */
  volatile u_int32_t BB_spur_report_b2;           /*       0xa8 - 0xac       */
  volatile char pad__3[0x14];                     /*       0xac - 0xc0       */
  volatile u_int32_t BB_iq_adc_meas_0_b2;         /*       0xc0 - 0xc4       */
  volatile u_int32_t BB_iq_adc_meas_1_b2;         /*       0xc4 - 0xc8       */
  volatile u_int32_t BB_iq_adc_meas_2_b2;         /*       0xc8 - 0xcc       */
  volatile u_int32_t BB_iq_adc_meas_3_b2;         /*       0xcc - 0xd0       */
  volatile u_int32_t BB_tx_phase_ramp_b2;         /*       0xd0 - 0xd4       */
  volatile u_int32_t BB_adc_gain_dc_corr_b2;      /*       0xd4 - 0xd8       */
  volatile char pad__4[0x4];                      /*       0xd8 - 0xdc       */
  volatile u_int32_t BB_rx_iq_corr_b2;            /*       0xdc - 0xe0       */
  volatile char pad__5[0x10];                     /*       0xe0 - 0xf0       */
  volatile u_int32_t BB_paprd_ctrl0_b2;           /*       0xf0 - 0xf4       */
  volatile u_int32_t BB_paprd_ctrl1_b2;           /*       0xf4 - 0xf8       */
  volatile u_int32_t BB_pa_gain123_b2;            /*       0xf8 - 0xfc       */
  volatile u_int32_t BB_pa_gain45_b2;             /*       0xfc - 0x100      */
  volatile u_int32_t BB_paprd_pre_post_scale_0_b2;
                                                  /*      0x100 - 0x104      */
  volatile u_int32_t BB_paprd_pre_post_scale_1_b2;
                                                  /*      0x104 - 0x108      */
  volatile u_int32_t BB_paprd_pre_post_scale_2_b2;
                                                  /*      0x108 - 0x10c      */
  volatile u_int32_t BB_paprd_pre_post_scale_3_b2;
                                                  /*      0x10c - 0x110      */
  volatile u_int32_t BB_paprd_pre_post_scale_4_b2;
                                                  /*      0x110 - 0x114      */
  volatile u_int32_t BB_paprd_pre_post_scale_5_b2;
                                                  /*      0x114 - 0x118      */
  volatile u_int32_t BB_paprd_pre_post_scale_6_b2;
                                                  /*      0x118 - 0x11c      */
  volatile u_int32_t BB_paprd_pre_post_scale_7_b2;
                                                  /*      0x11c - 0x120      */
  volatile u_int32_t BB_paprd_mem_tab_b2[120];    /*      0x120 - 0x300      */
  volatile u_int32_t BB_chan_info_chan_tab_b2[60];
                                                  /*      0x300 - 0x3f0      */
  volatile u_int32_t BB_chn2_tables_intf_addr;    /*      0x3f0 - 0x3f4      */
  volatile u_int32_t BB_chn2_tables_intf_data;    /*      0x3f4 - 0x3f8      */
};

struct agc2_reg_map {
  volatile char pad__0[0x4];                      /*        0x0 - 0x4        */
  volatile u_int32_t BB_gain_force_max_gains_b2;  /*        0x4 - 0x8        */
  volatile char pad__1[0x10];                     /*        0x8 - 0x18       */
  volatile u_int32_t BB_ext_atten_switch_ctl_b2;  /*       0x18 - 0x1c       */
  volatile u_int32_t BB_cca_b2;                   /*       0x1c - 0x20       */
  volatile u_int32_t BB_cca_ctrl_2_b2;            /*       0x20 - 0x24       */
  volatile char pad__2[0x15c];                    /*       0x24 - 0x180      */
  volatile u_int32_t BB_rssi_b2;                  /*      0x180 - 0x184      */
  volatile char pad__3[0x4];                      /*      0x184 - 0x188      */
  volatile u_int32_t BB_agc_dig_dc_status_i_b2;   /*      0x188 - 0x18c      */
  volatile u_int32_t BB_agc_dig_dc_status_q_b2;   /*      0x18c - 0x190      */
  volatile u_int32_t BB_dc_cal_status_b2;         /*      0x190 - 0x194      */
};

struct sm2_reg_map {
  volatile char pad__0[0x84];                     /*        0x0 - 0x84       */
  volatile u_int32_t BB_switch_table_chn_b2;      /*       0x84 - 0x88       */
  volatile char pad__1[0x48];                     /*       0x88 - 0xd0       */
  volatile u_int32_t BB_fcal_2_b2;                /*       0xd0 - 0xd4       */
  volatile u_int32_t BB_dft_tone_ctrl_b2;         /*       0xd4 - 0xd8       */
  volatile char pad__2[0x4];                      /*       0xd8 - 0xdc       */
  volatile u_int32_t BB_cl_map_0_b2;              /*       0xdc - 0xe0       */
  volatile u_int32_t BB_cl_map_1_b2;              /*       0xe0 - 0xe4       */
  volatile u_int32_t BB_cl_map_2_b2;              /*       0xe4 - 0xe8       */
  volatile u_int32_t BB_cl_map_3_b2;              /*       0xe8 - 0xec       */
  volatile u_int32_t BB_cl_map_pal_0_b2;          /*       0xec - 0xf0       */
  volatile u_int32_t BB_cl_map_pal_1_b2;          /*       0xf0 - 0xf4       */
  volatile u_int32_t BB_cl_map_pal_2_b2;          /*       0xf4 - 0xf8       */
  volatile u_int32_t BB_cl_map_pal_3_b2;          /*       0xf8 - 0xfc       */
  volatile char pad__3[0x4];                      /*       0xfc - 0x100      */
  volatile u_int32_t BB_cl_tab_b2[16];            /*      0x100 - 0x140      */
  volatile char pad__4[0x40];                     /*      0x140 - 0x180      */
  volatile u_int32_t BB_chan_info_gain_b2;        /*      0x180 - 0x184      */
  volatile char pad__5[0x80];                     /*      0x184 - 0x204      */
  volatile u_int32_t BB_tpc_4_b2;                 /*      0x204 - 0x208      */
  volatile u_int32_t BB_tpc_5_b2;                 /*      0x208 - 0x20c      */
  volatile u_int32_t BB_tpc_6_b2;                 /*      0x20c - 0x210      */
  volatile char pad__6[0x10];                     /*      0x210 - 0x220      */
  volatile u_int32_t BB_tpc_11_b2;                /*      0x220 - 0x224      */
  volatile char pad__7[0x18];                     /*      0x224 - 0x23c      */
  volatile u_int32_t BB_tpc_18_b2;                /*      0x23c - 0x240      */
  volatile u_int32_t BB_tpc_19_b2;                /*      0x240 - 0x244      */
  volatile char pad__8[0x3c];                     /*      0x244 - 0x280      */
  volatile u_int32_t BB_pdadc_tab_b2[32];         /*      0x280 - 0x300      */
  volatile char pad__9[0x84];                     /*      0x300 - 0x384      */
  volatile u_int32_t BB_rtt_table_sw_intf_b2;     /*      0x384 - 0x388      */
  volatile u_int32_t BB_rtt_table_sw_intf_1_b2;   /*      0x388 - 0x38c      */
  volatile char pad__10[0xc4];                    /*      0x38c - 0x450      */
  volatile u_int32_t BB_txiq_corr_coeff_01_b2;    /*      0x450 - 0x454      */
  volatile u_int32_t BB_txiq_corr_coeff_23_b2;    /*      0x454 - 0x458      */
  volatile u_int32_t BB_txiq_corr_coeff_45_b2;    /*      0x458 - 0x45c      */
  volatile u_int32_t BB_txiq_corr_coeff_67_b2;    /*      0x45c - 0x460      */
  volatile u_int32_t BB_txiq_corr_coeff_89_b2;    /*      0x460 - 0x464      */
  volatile u_int32_t BB_txiq_corr_coeff_ab_b2;    /*      0x464 - 0x468      */
  volatile u_int32_t BB_txiq_corr_coeff_cd_b2;    /*      0x468 - 0x46c      */
  volatile u_int32_t BB_txiq_corr_coeff_ef_b2;    /*      0x46c - 0x470      */
  volatile char pad__11[0x1c];                    /*      0x470 - 0x48c      */
  volatile u_int32_t BB_txiqcal_status_b2;        /*      0x48c - 0x490      */
  volatile char pad__12[0x160];                   /*      0x490 - 0x5f0      */
  volatile u_int32_t BB_tables_intf_addr_b2;      /*      0x5f0 - 0x5f4      */
  volatile u_int32_t BB_tables_intf_data_b2;      /*      0x5f4 - 0x5f8      */
};

struct chn_broadcast_reg_map {
  volatile u_int32_t BB_dummy1[256];              /*        0x0 - 0x400      */
};

struct agc_broadcast_reg_map {
  volatile u_int32_t BB_dummy[128];               /*        0x0 - 0x200      */
};

struct sm_broadcast_reg_map {
  volatile u_int32_t BB_dummy2[384];              /*        0x0 - 0x600      */
};

struct chn3_reg_map {
  volatile char pad__0[0x30];                     /*        0x0 - 0x30       */
  volatile u_int32_t BB_ext_chan_pwr_thr_2_b3;    /*       0x30 - 0x34       */
  volatile char pad__1[0xc];                      /*       0x34 - 0x40       */
  volatile u_int32_t BB_tx_preemp_coef_01_b3;     /*       0x40 - 0x44       */
  volatile u_int32_t BB_tx_preemp_coef_23_b3;     /*       0x44 - 0x48       */
  volatile u_int32_t BB_tx_preemp_coef_45_b3;     /*       0x48 - 0x4c       */
  volatile u_int32_t BB_tx_preemp_coef_67_b3;     /*       0x4c - 0x50       */
  volatile u_int32_t BB_tx_preemp_coef_8_b3;      /*       0x50 - 0x54       */
  volatile u_int32_t BB_tx_notch_coef_b3;         /*       0x54 - 0x58       */
  volatile u_int32_t BB_spur_freq_phase_b3;       /*       0x58 - 0x5c       */
  volatile char pad__2[0x4c];                     /*       0x5c - 0xa8       */
  volatile u_int32_t BB_spur_report_b3;           /*       0xa8 - 0xac       */
  volatile char pad__3[0x14];                     /*       0xac - 0xc0       */
  volatile u_int32_t BB_iq_adc_meas_0_b3;         /*       0xc0 - 0xc4       */
  volatile u_int32_t BB_iq_adc_meas_1_b3;         /*       0xc4 - 0xc8       */
  volatile u_int32_t BB_iq_adc_meas_2_b3;         /*       0xc8 - 0xcc       */
  volatile u_int32_t BB_iq_adc_meas_3_b3;         /*       0xcc - 0xd0       */
  volatile u_int32_t BB_tx_phase_ramp_b3;         /*       0xd0 - 0xd4       */
  volatile u_int32_t BB_adc_gain_dc_corr_b3;      /*       0xd4 - 0xd8       */
  volatile char pad__4[0x4];                      /*       0xd8 - 0xdc       */
  volatile u_int32_t BB_rx_iq_corr_b3;            /*       0xdc - 0xe0       */
  volatile char pad__5[0x10];                     /*       0xe0 - 0xf0       */
  volatile u_int32_t BB_paprd_ctrl0_b3;           /*       0xf0 - 0xf4       */
  volatile u_int32_t BB_paprd_ctrl1_b3;           /*       0xf4 - 0xf8       */
  volatile u_int32_t BB_pa_gain123_b3;            /*       0xf8 - 0xfc       */
  volatile u_int32_t BB_pa_gain45_b3;             /*       0xfc - 0x100      */
  volatile u_int32_t BB_paprd_pre_post_scale_0_b3;
                                                  /*      0x100 - 0x104      */
  volatile u_int32_t BB_paprd_pre_post_scale_1_b3;
                                                  /*      0x104 - 0x108      */
  volatile u_int32_t BB_paprd_pre_post_scale_2_b3;
                                                  /*      0x108 - 0x10c      */
  volatile u_int32_t BB_paprd_pre_post_scale_3_b3;
                                                  /*      0x10c - 0x110      */
  volatile u_int32_t BB_paprd_pre_post_scale_4_b3;
                                                  /*      0x110 - 0x114      */
  volatile u_int32_t BB_paprd_pre_post_scale_5_b3;
                                                  /*      0x114 - 0x118      */
  volatile u_int32_t BB_paprd_pre_post_scale_6_b3;
                                                  /*      0x118 - 0x11c      */
  volatile u_int32_t BB_paprd_pre_post_scale_7_b3;
                                                  /*      0x11c - 0x120      */
  volatile u_int32_t BB_paprd_mem_tab_b3[120];    /*      0x120 - 0x300      */
  volatile u_int32_t BB_chan_info_chan_tab_b3[60];
                                                  /*      0x300 - 0x3f0      */
  volatile u_int32_t BB_chn3_tables_intf_addr;    /*      0x3f0 - 0x3f4      */
  volatile u_int32_t BB_chn3_tables_intf_data;    /*      0x3f4 - 0x3f8      */
};

struct agc3_reg_map {
  volatile char pad__0[0x4];                      /*        0x0 - 0x4        */
  volatile u_int32_t BB_gain_force_max_gains_b3;  /*        0x4 - 0x8        */
  volatile char pad__1[0x10];                     /*        0x8 - 0x18       */
  volatile u_int32_t BB_ext_atten_switch_ctl_b3;  /*       0x18 - 0x1c       */
  volatile u_int32_t BB_cca_b3;                   /*       0x1c - 0x20       */
  volatile u_int32_t BB_cca_ctrl_2_b3;            /*       0x20 - 0x24       */
  volatile char pad__2[0x15c];                    /*       0x24 - 0x180      */
  volatile u_int32_t BB_rssi_b3;                  /*      0x180 - 0x184      */
  volatile char pad__3[0x4];                      /*      0x184 - 0x188      */
  volatile u_int32_t BB_agc_dig_dc_status_i_b3;   /*      0x188 - 0x18c      */
  volatile u_int32_t BB_agc_dig_dc_status_q_b3;   /*      0x18c - 0x190      */
  volatile u_int32_t BB_dc_cal_status_b3;         /*      0x190 - 0x194      */
};

struct sm3_reg_map {
  volatile char pad__0[0x84];                     /*        0x0 - 0x84       */
  volatile u_int32_t BB_switch_table_chn_b3;      /*       0x84 - 0x88       */
  volatile char pad__1[0x48];                     /*       0x88 - 0xd0       */
  volatile u_int32_t BB_fcal_2_b3;                /*       0xd0 - 0xd4       */
  volatile u_int32_t BB_dft_tone_ctrl_b3;         /*       0xd4 - 0xd8       */
  volatile char pad__2[0x4];                      /*       0xd8 - 0xdc       */
  volatile u_int32_t BB_cl_map_0_b3;              /*       0xdc - 0xe0       */
  volatile u_int32_t BB_cl_map_1_b3;              /*       0xe0 - 0xe4       */
  volatile u_int32_t BB_cl_map_2_b3;              /*       0xe4 - 0xe8       */
  volatile u_int32_t BB_cl_map_3_b3;              /*       0xe8 - 0xec       */
  volatile u_int32_t BB_cl_map_pal_0_b3;          /*       0xec - 0xf0       */
  volatile u_int32_t BB_cl_map_pal_1_b3;          /*       0xf0 - 0xf4       */
  volatile u_int32_t BB_cl_map_pal_2_b3;          /*       0xf4 - 0xf8       */
  volatile u_int32_t BB_cl_map_pal_3_b3;          /*       0xf8 - 0xfc       */
  volatile char pad__3[0x4];                      /*       0xfc - 0x100      */
  volatile u_int32_t BB_cl_tab_b3[16];            /*      0x100 - 0x140      */
  volatile char pad__4[0x40];                     /*      0x140 - 0x180      */
  volatile u_int32_t BB_chan_info_gain_b3;        /*      0x180 - 0x184      */
  volatile char pad__5[0x80];                     /*      0x184 - 0x204      */
  volatile u_int32_t BB_tpc_4_b3;                 /*      0x204 - 0x208      */
  volatile u_int32_t BB_tpc_5_b3;                 /*      0x208 - 0x20c      */
  volatile u_int32_t BB_tpc_6_b3;                 /*      0x20c - 0x210      */
  volatile char pad__6[0x10];                     /*      0x210 - 0x220      */
  volatile u_int32_t BB_tpc_11_b3;                /*      0x220 - 0x224      */
  volatile char pad__7[0x18];                     /*      0x224 - 0x23c      */
  volatile u_int32_t BB_tpc_18_b3;                /*      0x23c - 0x240      */
  volatile u_int32_t BB_tpc_19_b3;                /*      0x240 - 0x244      */
  volatile char pad__8[0x3c];                     /*      0x244 - 0x280      */
  volatile u_int32_t BB_pdadc_tab_b3[32];         /*      0x280 - 0x300      */
  volatile char pad__9[0x84];                     /*      0x300 - 0x384      */
  volatile u_int32_t BB_rtt_table_sw_intf_b3;     /*      0x384 - 0x388      */
  volatile u_int32_t BB_rtt_table_sw_intf_1_b3;   /*      0x388 - 0x38c      */
  volatile char pad__10[0xc4];                    /*      0x38c - 0x450      */
  volatile u_int32_t BB_txiq_corr_coeff_01_b3;    /*      0x450 - 0x454      */
  volatile u_int32_t BB_txiq_corr_coeff_23_b3;    /*      0x454 - 0x458      */
  volatile u_int32_t BB_txiq_corr_coeff_45_b3;    /*      0x458 - 0x45c      */
  volatile u_int32_t BB_txiq_corr_coeff_67_b3;    /*      0x45c - 0x460      */
  volatile u_int32_t BB_txiq_corr_coeff_89_b3;    /*      0x460 - 0x464      */
  volatile u_int32_t BB_txiq_corr_coeff_ab_b3;    /*      0x464 - 0x468      */
  volatile u_int32_t BB_txiq_corr_coeff_cd_b3;    /*      0x468 - 0x46c      */
  volatile u_int32_t BB_txiq_corr_coeff_ef_b3;    /*      0x46c - 0x470      */
  volatile char pad__11[0x1c];                    /*      0x470 - 0x48c      */
  volatile u_int32_t BB_txiqcal_status_b3;        /*      0x48c - 0x490      */
  volatile char pad__12[0x160];                   /*      0x490 - 0x5f0      */
  volatile u_int32_t BB_tables_intf_addr_b3;      /*      0x5f0 - 0x5f4      */
  volatile u_int32_t BB_tables_intf_data_b3;      /*      0x5f4 - 0x5f8      */
};

struct bb_reg_map {
  volatile char pad__0[0x29800];                  /*        0x0 - 0x29800    */
  struct chn_reg_map bb_chn_reg_map;              /*    0x29800 - 0x29bf8    */
  volatile char pad__1[0x8];                      /*    0x29bf8 - 0x29c00    */
  struct mrc_reg_map bb_mrc_reg_map;              /*    0x29c00 - 0x29c30    */
  volatile char pad__2[0xd0];                     /*    0x29c30 - 0x29d00    */
  struct bbb_reg_map bb_bbb_reg_map;              /*    0x29d00 - 0x29d1c    */
  volatile char pad__3[0xe4];                     /*    0x29d1c - 0x29e00    */
  struct agc_reg_map bb_agc_reg_map;              /*    0x29e00 - 0x2a200    */
  struct sm_reg_map bb_sm_reg_map;                /*    0x2a200 - 0x2a7f8    */
  volatile char pad__4[0x8];                      /*    0x2a7f8 - 0x2a800    */
  struct chn1_reg_map bb_chn1_reg_map;            /*    0x2a800 - 0x2abf8    */
  volatile char pad__5[0x208];                    /*    0x2abf8 - 0x2ae00    */
  struct agc1_reg_map bb_agc1_reg_map;            /*    0x2ae00 - 0x2b200    */
  struct sm1_reg_map bb_sm1_reg_map;              /*    0x2b200 - 0x2b7f8    */
  volatile char pad__6[0x8];                      /*    0x2b7f8 - 0x2b800    */
  struct chn2_reg_map bb_chn2_reg_map;            /*    0x2b800 - 0x2bbf8    */
  volatile char pad__7[0x208];                    /*    0x2bbf8 - 0x2be00    */
  struct agc2_reg_map bb_agc2_reg_map;            /*    0x2be00 - 0x2bf94    */
  volatile char pad__8[0x26c];                    /*    0x2bf94 - 0x2c200    */
  struct sm2_reg_map bb_sm2_reg_map;              /*    0x2c200 - 0x2c7f8    */
  volatile char pad__9[0x8];                      /*    0x2c7f8 - 0x2c800    */
  struct chn_broadcast_reg_map bb_chn_broadcast_reg_map;
                                                  /*    0x2c800 - 0x2cc00    */
  volatile char pad__10[0x200];                   /*    0x2cc00 - 0x2ce00    */
  struct agc_broadcast_reg_map bb_agc_broadcast_reg_map;
                                                  /*    0x2ce00 - 0x2d000    */
  volatile char pad__11[0x200];                   /*    0x2d000 - 0x2d200    */
  struct sm_broadcast_reg_map bb_sm_broadcast_reg_map;
                                                  /*    0x2d200 - 0x2d800    */
  struct chn3_reg_map bb_chn3_reg_map;            /*    0x2d800 - 0x2dbf8    */
  volatile char pad__12[0x208];                   /*    0x2dbf8 - 0x2de00    */
  struct agc3_reg_map bb_agc3_reg_map;            /*    0x2de00 - 0x2df94    */
  volatile char pad__13[0x26c];                   /*    0x2df94 - 0x2e200    */
  struct sm3_reg_map bb_sm3_reg_map;              /*    0x2e200 - 0x2e7f8    */
};

struct jet_reg_map {
  struct mac_dma_reg mac_dma_reg_block;           /*        0x0 - 0x130      */
  volatile char pad__0[0x6d0];                    /*      0x130 - 0x800      */
  struct mac_qcu_reg mac_qcu_reg_block;           /*      0x800 - 0xa48      */
  volatile char pad__1[0x5b8];                    /*      0xa48 - 0x1000     */
  struct mac_dcu_reg mac_dcu_reg_block;           /*     0x1000 - 0x1f08     */
  volatile char pad__2[0x20f8];                   /*     0x1f08 - 0x4000     */
  struct wmac_glue_intf_reg wmac_glue_intf_reg_block;
                                                  /*     0x4000 - 0x40f4     */
  volatile char pad__3[0x2f0c];                   /*     0x40f4 - 0x7000     */
  struct rtc_reg rtc_reg_block;                   /*     0x7000 - 0x7040     */
  struct rtc_sync_reg rtc_sync_reg_block;         /*     0x7040 - 0x705c     */
  volatile char pad__4[0x7a4];                    /*     0x705c - 0x7800     */
  struct merlin2_0_radio_reg_map merlin2_0_radio_reg_map;
                                                  /*     0x7800 - 0x789c     */
  volatile char pad__5[0x764];                    /*     0x789c - 0x8000     */
  struct mac_pcu_reg mac_pcu_reg_block;           /*     0x8000 - 0x9800     */
  volatile char pad__6[0x4800];                   /*     0x9800 - 0xe000     */
  struct mac_pcu_buf_reg mac_pcu_buf_reg_block;   /*     0xe000 - 0x10000    */
  volatile char pad__7[0x6000];                   /*    0x10000 - 0x16000    */
  struct radio65_reg radio65_reg_block;           /*    0x16000 - 0x16bd8    */
  volatile char pad__8[0x12c28];                  /*    0x16bd8 - 0x29800    */
  struct bb_reg_map bb_reg_block;                 /*    0x29800 - 0x2ea00    */
};

struct pmu_reg_dragonfly {
  volatile char pad__0[0x16cc0];
  volatile u_int32_t ch0_PMU1;
  volatile u_int32_t ch0_PMU2;
};
#endif /* __REG_JET_REG_MAP_H__ */
