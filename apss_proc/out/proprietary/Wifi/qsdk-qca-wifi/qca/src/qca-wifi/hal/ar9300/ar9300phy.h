/*
 * Copyright (c) 2011, 2017 Qualcomm Innovation Center, Inc.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Innovation Center, Inc.
 *
 * Copyright (c) 2002-2005 Atheros Communications, Inc. 
 * All Rights Reserved.
 *
 * 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * 
 */

#ifndef _ATH_AR9300PHY_H_
#define _ATH_AR9300PHY_H_

#include "osprey_reg_map.h"

/*
 * BB PHY register map
 */
#define AR_PHY_BASE     offsetof(struct bb_reg_map, bb_chn_reg_map)      /* base address of phy regs */
#define AR_PHY(_n)      (AR_PHY_BASE + ((_n)<<2))

/*
 * Channel Register Map
 */
#define AR_CHAN_BASE      offsetof(struct bb_reg_map, bb_chn_reg_map)
#define AR_CHAN_OFFSET(_x)   (AR_CHAN_BASE + offsetof(struct chn_reg_map, _x))

#define AR_PHY_TIMING1      AR_CHAN_OFFSET(BB_timing_controls_1)
#define AR_PHY_TIMING2      AR_CHAN_OFFSET(BB_timing_controls_2)
#define AR_PHY_TIMING3      AR_CHAN_OFFSET(BB_timing_controls_3)
#define AR_PHY_TIMING4      AR_CHAN_OFFSET(BB_timing_control_4)
#define AR_PHY_TIMING5      AR_CHAN_OFFSET(BB_timing_control_5)
#define AR_PHY_TIMING6      AR_CHAN_OFFSET(BB_timing_control_6)
#define AR_PHY_TIMING11     AR_CHAN_OFFSET(BB_timing_control_11)
#define AR_PHY_SPUR_REG     AR_CHAN_OFFSET(BB_spur_mask_controls)
#define AR_PHY_RX_IQCAL_CORR_B0    AR_CHAN_OFFSET(BB_rx_iq_corr_b0)
#define AR_PHY_TX_IQCAL_CONTROL_3  AR_CHAN_OFFSET(BB_txiqcal_control_3)
#define AR_PHY_TXIQCAL_CONTROL_2  AR_SM_OFFSET(overlay_0xa580.Jet.BB_txiqcal_control_2)
#define TX_IQCAL_CONTROL_3__IQCAL_MEAS_LEN__SHIFT      27
#define TX_IQCAL_CONTROL_3__IQCAL_MEAS_LEN__MASK       0x18000000U

#define AR_PHY_TXIQCAL_CONTROL_2_MAX_TX_GAIN_GET(x)    (((x) & 0x00003e00) >> 9)
#define AR_PHY_TXIQCAL_CONTROL_2_MAX_TX_GAIN_SET(x)    (((x) << 9) & 0x00003e00)
#define AR_PHY_TXIQCAL_CONTROL_2_MIN_TX_GAIN_GET(x)    (((x) & 0x000001f0) >> 4)
#define AR_PHY_TXIQCAL_CONTROL_2_MIN_TX_GAIN_SET(x)    (((x) << 4) & 0x000001f0)

/* BB_timing_control_11 */
#define AR_PHY_TIMING11_SPUR_FREQ_SD	0x3FF00000
#define AR_PHY_TIMING11_SPUR_FREQ_SD_S  20

#define AR_PHY_TIMING11_SPUR_DELTA_PHASE 0x000FFFFF
#define AR_PHY_TIMING11_SPUR_DELTA_PHASE_S 0

#define AR_PHY_TIMING11_USE_SPUR_FILTER_IN_AGC 0x40000000
#define AR_PHY_TIMING11_USE_SPUR_FILTER_IN_AGC_S 30

#define AR_PHY_TIMING11_USE_SPUR_FILTER_IN_SELFCOR 0x80000000
#define AR_PHY_TIMING11_USE_SPUR_FILTER_IN_SELFCOR_S 31

/* BB_spur_mask_controls */
#define AR_PHY_SPUR_REG_ENABLE_NF_RSSI_SPUR_MIT		0x4000000
#define AR_PHY_SPUR_REG_ENABLE_NF_RSSI_SPUR_MIT_S	26

#define AR_PHY_SPUR_REG_ENABLE_MASK_PPM				0x20000     /* bins move with freq offset */
#define AR_PHY_SPUR_REG_ENABLE_MASK_PPM_S			17
#define AR_PHY_SPUR_REG_SPUR_RSSI_THRESH            0x000000FF
#define AR_PHY_SPUR_REG_SPUR_RSSI_THRESH_S          0
#define AR_PHY_SPUR_REG_EN_VIT_SPUR_RSSI			0x00000100
#define AR_PHY_SPUR_REG_EN_VIT_SPUR_RSSI_S			8
#define AR_PHY_SPUR_REG_MASK_RATE_CNTL				0x03FC0000
#define AR_PHY_SPUR_REG_MASK_RATE_CNTL_S			18

/* BB_rx_iq_corr_b0 */
#define AR_PHY_RX_IQCAL_CORR_B0_LOOPBACK_IQCORR_EN   0x20000000
#define AR_PHY_RX_IQCAL_CORR_B0_LOOPBACK_IQCORR_EN_S         29
/* BB_txiqcal_control_3 */
#define AR_PHY_TX_IQCAL_CONTROL_3_IQCORR_EN   0x80000000
#define AR_PHY_TX_IQCAL_CONTROL_3_IQCORR_EN_S         31
#define AR_PHY_TXIQCAL_CONTROL_0_ENABLE_COMBINED_CARR_IQ_CAL 0x40000000
#define AR_PHY_TXIQCAL_CONTROL_0_ENABLE_COMBINED_CARR_IQ_CAL_S 30
#if 0
/* enable vit puncture per rate, 8 bits, lsb is low rate */
#define AR_PHY_SPUR_REG_MASK_RATE_CNTL       (0xFF << 18)
#define AR_PHY_SPUR_REG_MASK_RATE_CNTL_S     18
#define AR_PHY_SPUR_REG_ENABLE_MASK_PPM      0x20000     /* bins move with freq offset */
#define AR_PHY_SPUR_REG_MASK_RATE_SELECT     (0xFF << 9) /* use mask1 or mask2, one per rate */
#define AR_PHY_SPUR_REG_MASK_RATE_SELECT_S   9
#define AR_PHY_SPUR_REG_ENABLE_VIT_SPUR_RSSI 0x100
#define AR_PHY_SPUR_REG_SPUR_RSSI_THRESH     0x7F
#define AR_PHY_SPUR_REG_SPUR_RSSI_THRESH_S   0
#endif

#define AR_PHY_FIND_SIG_LOW  AR_CHAN_OFFSET(BB_find_signal_low)
#define AR_PHY_SFCORR           AR_CHAN_OFFSET(BB_sfcorr)
#if 0
#define AR_PHY_SFCORR_M2COUNT_THR    0x0000001F
#define AR_PHY_SFCORR_M2COUNT_THR_S  0
#define AR_PHY_SFCORR_M1_THRESH      0x00FE0000
#define AR_PHY_SFCORR_M1_THRESH_S    17
#define AR_PHY_SFCORR_M2_THRESH      0x7F000000
#define AR_PHY_SFCORR_M2_THRESH_S    24
#endif

#define AR_PHY_SFCORR_LOW       AR_CHAN_OFFSET(BB_self_corr_low)
#define AR_PHY_SFCORR_EXT       AR_CHAN_OFFSET(BB_ext_chan_scorr_thr)
#if 0
#define AR_PHY_SFCORR_EXT_M1_THRESH       0x0000007F   // [06:00]
#define AR_PHY_SFCORR_EXT_M1_THRESH_S     0
#define AR_PHY_SFCORR_EXT_M2_THRESH       0x00003F80   // [13:07]
#define AR_PHY_SFCORR_EXT_M2_THRESH_S     7
#define AR_PHY_SFCORR_EXT_M1_THRESH_LOW   0x001FC000   // [20:14]
#define AR_PHY_SFCORR_EXT_M1_THRESH_LOW_S 14
#define AR_PHY_SFCORR_EXT_M2_THRESH_LOW   0x0FE00000   // [27:21]
#define AR_PHY_SFCORR_EXT_M2_THRESH_LOW_S 21
#define AR_PHY_SFCORR_SPUR_SUBCHNL_SD_S   28
#endif

#define AR_PHY_EXT_CCA              AR_CHAN_OFFSET(BB_ext_chan_pwr_thr_2_b0)
#define AR_PHY_RADAR_0              AR_CHAN_OFFSET(BB_radar_detection)      /* radar detection settings */
#define AR_PHY_RADAR_1              AR_CHAN_OFFSET(BB_radar_detection_2)
#define AR_PHY_RADAR_1_CF_BIN_THRESH	0x07000000
#define AR_PHY_RADAR_1_CF_BIN_THRESH_S	24
#define AR_PHY_RADAR_EXT            AR_CHAN_OFFSET(BB_extension_radar) /* extension channel radar settings */
#define AR_PHY_MULTICHAIN_CTRL      AR_CHAN_OFFSET(BB_multichain_control)
#define AR_PHY_PERCHAIN_CSD         AR_CHAN_OFFSET(BB_per_chain_csd)
#define QCN5500_TX_PREEMP_COEF_01_B0    AR_CHAN_OFFSET(BB_tx_preemp_coef_01_b0)
#define QCN5500_TX_PREEMP_COEF_23_B0    AR_CHAN_OFFSET(BB_tx_preemp_coef_23_b0)
#define QCN5500_TX_PREEMP_COEF_45_B0    AR_CHAN_OFFSET(BB_tx_preemp_coef_45_b0)
#define QCN5500_TX_PREEMP_COEF_67_B0    AR_CHAN_OFFSET(BB_tx_preemp_coef_67_b0)
#define QCN5500_TX_PREEMP_COEF_8_B0     AR_CHAN_OFFSET(BB_tx_preemp_coef_8_b0)
#define QCN5500_TX_NOTCH_COEF_B0        AR_CHAN_OFFSET(BB_tx_notch_coef_b0)
#define QCN5500_SPUR_FREQ_PHASE_B0      AR_CHAN_OFFSET(BB_spur_freq_phase_b0)
#define QCN5500_PHY_MULTICHAIN_CNTRL_2  AR_CHAN_OFFSET(BB_multichain_control_2)
#define AR_PHY_CF_MAXCCAPWR_EXT_0    0x000001FF
#define AR_PHY_CF_MAXCCAPWR_EXT_0_S  0
#define AR_PHY_TX_PHASE_RAMP_0      AR_CHAN_OFFSET(BB_tx_phase_ramp_b0)
#define AR_PHY_ADC_GAIN_DC_CORR_0   AR_CHAN_OFFSET(BB_adc_gain_dc_corr_b0)
#define AR_PHY_GREEN_TX_CTRL_1     AR_CHAN_OFFSET(BB_green_tx_control_1)
#define AR_PHY_GREEN_TX_CTRL_1_GREENTX_ENABLE	0x00000001
#define AR_PHY_GREEN_TX_CTRL_1_GREENTX_ENABLE_S	0
#define AR_PHY_GREEN_TX_CTRL_1_GREENTX_CASES   0x00000002
#define AR_PHY_GREEN_TX_CTRL_1_GREENTX_CASES_S 1

#define AR_PHY_IQ_ADC_MEAS_0_B0     AR_CHAN_OFFSET(BB_iq_adc_meas_0_b0)
#define AR_PHY_IQ_ADC_MEAS_1_B0     AR_CHAN_OFFSET(BB_iq_adc_meas_1_b0)
#define AR_PHY_IQ_ADC_MEAS_2_B0     AR_CHAN_OFFSET(BB_iq_adc_meas_2_b0)
#define AR_PHY_IQ_ADC_MEAS_3_B0     AR_CHAN_OFFSET(BB_iq_adc_meas_3_b0)

#define AR_PHY_TX_IQ_CORR_0         AR_CHAN_OFFSET(BB_tx_iq_corr_b0)
#define AR_PHY_TX_CRC               AR_CHAN_OFFSET(BB_tx_crc)
#define AR_PHY_TST_DAC_CONST        AR_CHAN_OFFSET(BB_tstdac_constant)
#define AR_PHY_SPUR_REPORT_0        AR_CHAN_OFFSET(BB_spur_report_b0)
#define AR_PHY_CHAN_INFO_TAB_0      AR_CHAN_OFFSET(BB_chan_info_chan_tab_b0)


/*
 * Channel Field Definitions
 */
/* BB_timing_controls_2 */
#define AR_PHY_TIMING2_USE_FORCE_PPM    0x00001000
#define AR_PHY_TIMING2_FORCE_PPM_VAL    0x00000fff
#define AR_PHY_TIMING2_HT_Fine_Timing_EN    0x80000000
#define AR_PHY_TIMING2_DC_OFFSET	0x08000000
#define AR_PHY_TIMING2_DC_OFFSET_S	27

/* BB_timing_controls_3 */
#define AR_PHY_TIMING3_DSC_MAN      0xFFFE0000
#define AR_PHY_TIMING3_DSC_MAN_S    17
#define AR_PHY_TIMING3_DSC_EXP      0x0001E000
#define AR_PHY_TIMING3_DSC_EXP_S    13
/* BB_timing_control_4 */
#define AR_PHY_TIMING4_IQCAL_LOG_COUNT_MAX 0xF000  /* Mask for max number of samples (logarithmic) */
#define AR_PHY_TIMING4_IQCAL_LOG_COUNT_MAX_S   12  /* Shift for max number of samples */
#define AR_PHY_TIMING4_DO_CAL    0x10000     /* perform calibration */
#define AR_PHY_TIMING4_ENABLE_PILOT_MASK	0x10000000
#define AR_PHY_TIMING4_ENABLE_PILOT_MASK_S	28
#define AR_PHY_TIMING4_ENABLE_CHAN_MASK		0x20000000
#define AR_PHY_TIMING4_ENABLE_CHAN_MASK_S	29

#define AR_PHY_TIMING4_ENABLE_SPUR_FILTER 0x40000000
#define AR_PHY_TIMING4_ENABLE_SPUR_FILTER_S 30
#define AR_PHY_TIMING4_ENABLE_SPUR_RSSI 0x80000000
#define AR_PHY_TIMING4_ENABLE_SPUR_RSSI_S 31

/* BB_adc_gain_dc_corr_b0 */
#define AR_PHY_NEW_ADC_GAIN_CORR_ENABLE 0x40000000
#define AR_PHY_NEW_ADC_DC_OFFSET_CORR_ENABLE 0x80000000
/* BB_self_corr_low */
#define AR_PHY_SFCORR_LOW_USE_SELF_CORR_LOW  0x00000001
#define AR_PHY_SFCORR_LOW_M2COUNT_THR_LOW    0x00003F00
#define AR_PHY_SFCORR_LOW_M2COUNT_THR_LOW_S  8
#define AR_PHY_SFCORR_LOW_M1_THRESH_LOW      0x001FC000
#define AR_PHY_SFCORR_LOW_M1_THRESH_LOW_S    14
#define AR_PHY_SFCORR_LOW_M2_THRESH_LOW      0x0FE00000
#define AR_PHY_SFCORR_LOW_M2_THRESH_LOW_S    21
/* BB_sfcorr */
#define AR_PHY_SFCORR_M2COUNT_THR    0x0000001F
#define AR_PHY_SFCORR_M2COUNT_THR_S  0
#define AR_PHY_SFCORR_M1_THRESH      0x00FE0000
#define AR_PHY_SFCORR_M1_THRESH_S    17
#define AR_PHY_SFCORR_M2_THRESH      0x7F000000
#define AR_PHY_SFCORR_M2_THRESH_S    24
/* BB_ext_chan_scorr_thr */
#define AR_PHY_SFCORR_EXT_M1_THRESH       0x0000007F   // [06:00]
#define AR_PHY_SFCORR_EXT_M1_THRESH_S     0
#define AR_PHY_SFCORR_EXT_M2_THRESH       0x00003F80   // [13:07]
#define AR_PHY_SFCORR_EXT_M2_THRESH_S     7
#define AR_PHY_SFCORR_EXT_M1_THRESH_LOW   0x001FC000   // [20:14]
#define AR_PHY_SFCORR_EXT_M1_THRESH_LOW_S 14
#define AR_PHY_SFCORR_EXT_M2_THRESH_LOW   0x0FE00000   // [27:21]
#define AR_PHY_SFCORR_EXT_M2_THRESH_LOW_S 21
#define AR_PHY_SFCORR_EXT_SPUR_SUBCHANNEL_SD 0x10000000
#define AR_PHY_SFCORR_EXT_SPUR_SUBCHANNEL_SD_S 28
#define AR_PHY_SFCORR_SPUR_SUBCHNL_SD_S   28
/* BB_ext_chan_pwr_thr_2_b0 */
#define AR_PHY_EXT_CCA_THRESH62 0x007F0000
#define AR_PHY_EXT_CCA_THRESH62_S       16
#define AR_PHY_EXT_MINCCA_PWR   0x01FF0000
#define AR_PHY_EXT_MINCCA_PWR_S 16
#define AR_PHY_EXT_CYCPWR_THR1 0x0000FE00L 		// [15:09]
#define AR_PHY_EXT_CYCPWR_THR1_S 9
/* BB_timing_control_5 */
#define AR_PHY_TIMING5_CYCPWR_THR1  0x000000FE
#define AR_PHY_TIMING5_CYCPWR_THR1_S    1
#define AR_PHY_TIMING5_CYCPWR_THR1_ENABLE  0x00000001
#define AR_PHY_TIMING5_CYCPWR_THR1_ENABLE_S    0
#define AR_PHY_TIMING5_CYCPWR_THR1A  0x007F0000
#define AR_PHY_TIMING5_CYCPWR_THR1A_S    16
#define AR_PHY_TIMING5_RSSI_THR1A     (0x7F << 16)
#define AR_PHY_TIMING5_RSSI_THR1A_S   16
#define AR_PHY_TIMING5_RSSI_THR1A_ENA (0x1 << 15)
/* BB_radar_detection) */
#define AR_PHY_RADAR_0_ENA  0x00000001  /* Enable radar detection */
#define AR_PHY_RADAR_0_FFT_ENA  0x80000000  /* Enable FFT data */
#define AR_PHY_RADAR_0_INBAND   0x0000003e  /* Inband pulse threshold */
#define AR_PHY_RADAR_0_INBAND_S 1
#define AR_PHY_RADAR_0_PRSSI    0x00000FC0  /* Pulse rssi threshold */
#define AR_PHY_RADAR_0_PRSSI_S  6
#define AR_PHY_RADAR_0_HEIGHT   0x0003F000  /* Pulse height threshold */
#define AR_PHY_RADAR_0_HEIGHT_S 12
#define AR_PHY_RADAR_0_RRSSI    0x00FC0000  /* Radar rssi threshold */
#define AR_PHY_RADAR_0_RRSSI_S  18
#define AR_PHY_RADAR_0_FIRPWR   0x7F000000  /* Radar firpwr threshold */
#define AR_PHY_RADAR_0_FIRPWR_S 24
/* BB_radar_detection_2 */
#define AR_PHY_RADAR_1_RELPWR_ENA       0x00800000  /* enable to check radar relative power */
#define AR_PHY_RADAR_1_USE_FIR128       0x00400000  /* enable to use the average inband power
                                                     * measured over 128 cycles
                                                     */
#define AR_PHY_RADAR_1_RELPWR_THRESH    0x003F0000  /* relative pwr thresh */
#define AR_PHY_RADAR_1_RELPWR_THRESH_S  16
#define AR_PHY_RADAR_1_BLOCK_CHECK      0x00008000  /* Enable to block radar check if weak OFDM
                                                     * sig or pkt is immediately after tx to rx
                                                     * transition
                                                     */
#define AR_PHY_RADAR_1_MAX_RRSSI        0x00004000  /* Enable to use max rssi */
#define AR_PHY_RADAR_1_RELSTEP_CHECK    0x00002000  /* Enable to use pulse relative step check */
#define AR_PHY_RADAR_1_RELSTEP_THRESH   0x00001F00  /* Pulse relative step threshold */
#define AR_PHY_RADAR_1_RELSTEP_THRESH_S 8
#define AR_PHY_RADAR_1_MAXLEN           0x000000FF  /* Max length of radar pulse */
#define AR_PHY_RADAR_1_MAXLEN_S         0
/* BB_extension_radar */
#define AR_PHY_RADAR_EXT_ENA            0x00004000  /* Enable extension channel radar detection */
#define AR_PHY_RADAR_DC_PWR_THRESH      0x007f8000
#define AR_PHY_RADAR_DC_PWR_THRESH_S    15
#define AR_PHY_RADAR_LB_DC_CAP          0x7f800000
#define AR_PHY_RADAR_LB_DC_CAP_S        23
/* per chain csd*/
#define AR_PHY_PERCHAIN_CSD_chn1_2chains    0x0000001f
#define AR_PHY_PERCHAIN_CSD_chn1_2chains_S  0
#define AR_PHY_PERCHAIN_CSD_chn1_3chains    0x000003e0
#define AR_PHY_PERCHAIN_CSD_chn1_3chains_S  5
#define AR_PHY_PERCHAIN_CSD_chn2_3chains    0x00007c00
#define AR_PHY_PERCHAIN_CSD_chn2_3chains_S  10

/* BB_find_signal_low */
#define AR_PHY_FIND_SIG_LOW_FIRSTEP_LOW (0x3f << 6)
#define AR_PHY_FIND_SIG_LOW_FIRSTEP_LOW_S   6
#define AR_PHY_FIND_SIG_LOW_FIRPWR      (0x7f << 12)
#define AR_PHY_FIND_SIG_LOW_FIRPWR_S    12
#define AR_PHY_FIND_SIG_LOW_FIRPWR_SIGN_BIT 19
#define AR_PHY_FIND_SIG_LOW_RELSTEP     0x1f
#define AR_PHY_FIND_SIG_LOW_RELSTEP_S   0
#define AR_PHY_FIND_SIG_LOW_RELSTEP_SIGN_BIT 5
/* BB_chan_info_chan_tab_b* */
#define AR_PHY_CHAN_INFO_TAB_S2_READ    0x00000008
#define AR_PHY_CHAN_INFO_TAB_S2_READ_S           3
/* BB_rx_iq_corr_b* */
#define AR_PHY_RX_IQCAL_CORR_IQCORR_Q_Q_COFF 0x0000007F   /* Mask for kcos_theta-1 for q correction */
#define AR_PHY_RX_IQCAL_CORR_IQCORR_Q_Q_COFF_S   0   /* shift for Q_COFF */
#define AR_PHY_RX_IQCAL_CORR_IQCORR_Q_I_COFF 0x00003F80   /* Mask for sin_theta for i correction */
#define AR_PHY_RX_IQCAL_CORR_IQCORR_Q_I_COFF_S   7   /* Shift for sin_theta for i correction */
#define AR_PHY_RX_IQCAL_CORR_IQCORR_ENABLE   0x00004000   /* enable IQ correction */
#define AR_PHY_RX_IQCAL_CORR_LOOPBACK_IQCORR_Q_Q_COFF   0x003f8000
#define AR_PHY_RX_IQCAL_CORR_LOOPBACK_IQCORR_Q_Q_COFF_S 15
#define AR_PHY_RX_IQCAL_CORR_LOOPBACK_IQCORR_Q_I_COFF   0x1fc00000
#define AR_PHY_RX_IQCAL_CORR_LOOPBACK_IQCORR_Q_I_COFF_S 22

/*
 * MRC Register Map
 */
#define AR_MRC_BASE      offsetof(struct bb_reg_map, bb_mrc_reg_map)
#define AR_MRC_OFFSET(_x)   (AR_MRC_BASE + offsetof(struct mrc_reg_map, _x))

#define AR_PHY_TIMING_3A       AR_MRC_OFFSET(BB_timing_control_3a)
#define AR_PHY_LDPC_CNTL1      AR_MRC_OFFSET(BB_ldpc_cntl1)
#define AR_PHY_LDPC_CNTL2      AR_MRC_OFFSET(BB_ldpc_cntl2)
#define AR_PHY_PILOT_SPUR_MASK AR_MRC_OFFSET(BB_pilot_spur_mask)
#define AR_PHY_CHAN_SPUR_MASK  AR_MRC_OFFSET(BB_chan_spur_mask)
#define AR_PHY_SGI_DELTA       AR_MRC_OFFSET(BB_short_gi_delta_slope)
#define AR_PHY_ML_CNTL_1       AR_MRC_OFFSET(BB_ml_cntl1)
#define AR_PHY_ML_CNTL_2       AR_MRC_OFFSET(BB_ml_cntl2)
#define AR_PHY_TST_ADC         AR_MRC_OFFSET(BB_tstadc)

/* BB_pilot_spur_mask fields */
#define AR_PHY_PILOT_SPUR_MASK_CF_PILOT_MASK_IDX_A		0x00000FE0
#define AR_PHY_PILOT_SPUR_MASK_CF_PILOT_MASK_IDX_A_S	5
#define AR_PHY_PILOT_SPUR_MASK_CF_PILOT_MASK_A			0x1F
#define AR_PHY_PILOT_SPUR_MASK_CF_PILOT_MASK_A_S		0

/* BB_chan_spur_mask fields */
#define AR_PHY_CHAN_SPUR_MASK_CF_CHAN_MASK_IDX_A	0x00000FE0
#define AR_PHY_CHAN_SPUR_MASK_CF_CHAN_MASK_IDX_A_S	5
#define AR_PHY_CHAN_SPUR_MASK_CF_CHAN_MASK_A		0x1F
#define AR_PHY_CHAN_SPUR_MASK_CF_CHAN_MASK_A_S		0

/*
 * MRC Feild Definitions
 */
#define AR_PHY_SGI_DSC_MAN   0x0007FFF0
#define AR_PHY_SGI_DSC_MAN_S 4
#define AR_PHY_SGI_DSC_EXP   0x0000000F
#define AR_PHY_SGI_DSC_EXP_S 0
/*
 * BBB Register Map
 */
#define AR_BBB_BASE      offsetof(struct bb_reg_map, bb_bbb_reg_map)
#define AR_BBB_OFFSET(_x)   (AR_BBB_BASE + offsetof(struct bbb_reg_map, _x))

#define AR_PHY_BBB_RX_CTRL(_i)  AR_BBB_OFFSET(BB_bbb_rx_ctrl_##_i)

/*
 * AGC Register Map
 */
#define AR_AGC_BASE      offsetof(struct bb_reg_map, bb_agc_reg_map)
#define AR_AGC_OFFSET(_x)   (AR_AGC_BASE + offsetof(struct agc_reg_map, _x))

#define AR_PHY_SETTLING         AR_AGC_OFFSET(BB_settling_time)
#define AR_PHY_FORCEMAX_GAINS_0 AR_AGC_OFFSET(BB_gain_force_max_gains_b0)
#define AR_PHY_GAINS_MINOFF     AR_AGC_OFFSET(BB_gains_min_offsets)
#define AR_PHY_DESIRED_SZ       AR_AGC_OFFSET(BB_desired_sigsize)
#define AR_PHY_FIND_SIG         AR_AGC_OFFSET(BB_find_signal)
#define AR_PHY_AGC              AR_AGC_OFFSET(BB_agc)
#define AR_PHY_EXT_ATTEN_CTL_0  AR_AGC_OFFSET(BB_ext_atten_switch_ctl_b0)
#define AR_PHY_CCA_0            AR_AGC_OFFSET(BB_cca_b0)
#define AR_PHY_EXT_CCA0         AR_AGC_OFFSET(BB_cca_ctrl_2_b0)
#define AR_PHY_CCA_CTRL_0       AR_AGC_OFFSET(BB_cca_ctrl_2_b0)
#define AR_PHY_RESTART          AR_AGC_OFFSET(BB_restart)
#define AR_PHY_MC_GAIN_CTRL     AR_AGC_OFFSET(BB_multichain_gain_ctrl)
#define AR_PHY_EXTCHN_PWRTHR1   AR_AGC_OFFSET(BB_ext_chan_pwr_thr_1)
#define AR_PHY_EXT_CHN_WIN      AR_AGC_OFFSET(BB_ext_chan_detect_win)
#define AR_PHY_20_40_DET_THR    AR_AGC_OFFSET(BB_pwr_thr_20_40_det)
#define AR_PHY_RIFS_SRCH        AR_AGC_OFFSET(BB_rifs_srch)
#define AR_PHY_PEAK_DET_CTRL_1  AR_AGC_OFFSET(BB_peak_det_ctrl_1)

#define AR_PHY_PEAK_DET_ENABLE  0x00000002
#define AR_PHY_PEAK_DET_ENABLE_S  1
#define AR_PHY_use_oc_gain_table 0x1
#define AR_PHY_SETTLING_FORCE_3CHN_SETTLE_CLIQ    0x80000000
#define AR_PHY_SETTLING_FORCE_3CHN_SETTLE_CLIQ_S  31

#define AR_PHY_PEAK_DET_CTRL_2  AR_AGC_OFFSET(BB_peak_det_ctrl_2)

#define AR_PHY_RX_GAIN_BOUNDS_1 AR_AGC_OFFSET(BB_rx_gain_bounds_1)
#define AR_PHY_RX_GAIN_BOUNDS_2 AR_AGC_OFFSET(BB_rx_gain_bounds_2)
#define AR_PHY_RSSI_0           AR_AGC_OFFSET(BB_rssi_b0)
#define AR_PHY_SPUR_CCK_REP0    AR_AGC_OFFSET(BB_spur_est_cck_report_b0)
#define AR_PHY_CCK_DETECT       AR_AGC_OFFSET(BB_bbb_sig_detect)
#define AR_PHY_DAG_CTRLCCK      AR_AGC_OFFSET(BB_bbb_dagc_ctrl)
#define AR_PHY_IQCORR_CTRL_CCK  AR_AGC_OFFSET(BB_iqcorr_ctrl_cck)
#define AR_PHY_DIG_DC_STATUS_I_B0 AR_AGC_OFFSET(BB_agc_dig_dc_status_i_b0)
#define AR_PHY_DIG_DC_STATUS_Q_B0 AR_AGC_OFFSET(BB_agc_dig_dc_status_q_b0)
#define AR_PHY_DIG_DC_C1_RES            0x000001ff
#define AR_PHY_DIG_DC_C1_RES_S          0
#define AR_PHY_DIG_DC_C2_RES            0x0003fe00
#define AR_PHY_DIG_DC_C2_RES_S          9
#define AR_PHY_DIG_DC_C3_RES            0x07fc0000
#define AR_PHY_DIG_DC_C3_RES_S          18

#define AR_PHY_CCK_SPUR_MIT     AR_AGC_OFFSET(BB_cck_spur_mit)
#define AR_PHY_CCK_SPUR_MIT_SPUR_RSSI_THR                           0x000001fe
#define AR_PHY_CCK_SPUR_MIT_SPUR_RSSI_THR_S                                  1
#define AR_PHY_CCK_SPUR_MIT_SPUR_FILTER_TYPE                        0x60000000
#define AR_PHY_CCK_SPUR_MIT_SPUR_FILTER_TYPE_S                              29
#define AR_PHY_CCK_SPUR_MIT_USE_CCK_SPUR_MIT                        0x00000001
#define AR_PHY_CCK_SPUR_MIT_USE_CCK_SPUR_MIT_S                               0
#define AR_PHY_CCK_SPUR_MIT_CCK_SPUR_FREQ                           0x1ffffe00
#define AR_PHY_CCK_SPUR_MIT_CCK_SPUR_FREQ_S                                  9

#define AR_PHY_MRC_CCK_CTRL         AR_AGC_OFFSET(BB_mrc_cck_ctrl)
#define AR_PHY_MRC_CCK_ENABLE       0x00000001
#define AR_PHY_MRC_CCK_ENABLE_S              0
#define AR_PHY_MRC_CCK_MUX_REG      0x00000002
#define AR_PHY_MRC_CCK_MUX_REG_S             1

#define AR_PHY_RX_OCGAIN        AR_AGC_OFFSET(BB_rx_ocgain)

#define AR_PHY_CCA_NOM_VAL_OSPREY_2GHZ          -110
#define AR_PHY_CCA_NOM_VAL_OSPREY_5GHZ          -105
#define AR_PHY_CCA_MIN_GOOD_VAL_OSPREY_2GHZ     -125
#define AR_PHY_CCA_MIN_GOOD_VAL_OSPREY_5GHZ     -125
#define AR_PHY_CCA_MAX_GOOD_VAL_OSPREY_2GHZ     -60
#define AR_PHY_CCA_MAX_GOOD_VAL_OSPREY_FCC_2GHZ -95
#define AR_PHY_CCA_MAX_GOOD_VAL_OSPREY_5GHZ     -60
#define AR_PHY_CCA_MAX_GOOD_VAL_OSPREY_FCC_5GHZ -100
#define AR_PHY_CCA_NOM_VAL_PEACOCK_5GHZ         -105

#define AR_PHY_CCA_NOM_VAL_JUPITER_2GHZ          -127
#define AR_PHY_CCA_MIN_GOOD_VAL_JUPITER_2GHZ     -127
#define AR_PHY_CCA_NOM_VAL_JUPITER_5GHZ          -127
#define AR_PHY_CCA_MIN_GOOD_VAL_JUPITER_5GHZ     -127

#define AR_PHY_BT_COEX_4        AR_AGC_OFFSET(BB_bt_coex_4)
#define AR_PHY_BT_COEX_5        AR_AGC_OFFSET(BB_bt_coex_5)

/*
 * Noise floor readings at least CW_INT_DELTA above the nominal NF
 * indicate that CW interference is present.
 */
#define AR_PHY_CCA_CW_INT_DELTA 30

/*
 * AGC Field Definitions
 */
/* BB_ext_atten_switch_ctl_b0 */
#define AR_PHY_EXT_ATTEN_CTL_RXTX_MARGIN    0x00FC0000
#define AR_PHY_EXT_ATTEN_CTL_RXTX_MARGIN_S  18
#define AR_PHY_EXT_ATTEN_CTL_BSW_MARGIN     0x00003C00
#define AR_PHY_EXT_ATTEN_CTL_BSW_MARGIN_S   10
#define AR_PHY_EXT_ATTEN_CTL_BSW_ATTEN      0x0000001F
#define AR_PHY_EXT_ATTEN_CTL_BSW_ATTEN_S    0
#define AR_PHY_EXT_ATTEN_CTL_XATTEN2_MARGIN     0x003E0000
#define AR_PHY_EXT_ATTEN_CTL_XATTEN2_MARGIN_S   17
#define AR_PHY_EXT_ATTEN_CTL_XATTEN1_MARGIN     0x0001F000
#define AR_PHY_EXT_ATTEN_CTL_XATTEN1_MARGIN_S   12
#define AR_PHY_EXT_ATTEN_CTL_XATTEN2_DB         0x00000FC0
#define AR_PHY_EXT_ATTEN_CTL_XATTEN2_DB_S       6
#define AR_PHY_EXT_ATTEN_CTL_XATTEN1_DB         0x0000003F
#define AR_PHY_EXT_ATTEN_CTL_XATTEN1_DB_S       0
/* BB_gain_force_max_gains_b0 */
#define AR_PHY_RXGAIN_TXRX_ATTEN    0x0003F000
#define AR_PHY_RXGAIN_TXRX_ATTEN_S  12
#define AR_PHY_RXGAIN_TXRX_RF_MAX   0x007C0000
#define AR_PHY_RXGAIN_TXRX_RF_MAX_S 18
#define AR9280_PHY_RXGAIN_TXRX_ATTEN    0x00003F80
#define AR9280_PHY_RXGAIN_TXRX_ATTEN_S  7
#define AR9280_PHY_RXGAIN_TXRX_MARGIN   0x001FC000
#define AR9280_PHY_RXGAIN_TXRX_MARGIN_S 14
/* BB_settling_time */
#define AR_PHY_SETTLING_SWITCH  0x00003F80
#define AR_PHY_SETTLING_SWITCH_S    7
/* BB_desired_sigsize */
#define AR_PHY_DESIRED_SZ_ADC       0x000000FF
#define AR_PHY_DESIRED_SZ_ADC_S     0
#define AR_PHY_DESIRED_SZ_PGA       0x0000FF00
#define AR_PHY_DESIRED_SZ_PGA_S     8
#define AR_PHY_DESIRED_SZ_TOT_DES   0x0FF00000
#define AR_PHY_DESIRED_SZ_TOT_DES_S 20
/* BB_cca_b0 */
#define AR_PHY_MINCCA_PWR       0x1FF00000
#define AR_PHY_MINCCA_PWR_S     20
#define AR_PHY_CCA_THRESH62     0x0007F000
#define AR_PHY_CCA_THRESH62_S   12
#define AR9280_PHY_MINCCA_PWR       0x1FF00000
#define AR9280_PHY_MINCCA_PWR_S     20
#define AR9280_PHY_CCA_THRESH62     0x000FF000
#define AR9280_PHY_CCA_THRESH62_S   12
#define AR_PHY_CCA_COUNT_MAXC       0x00000E00
#define AR_PHY_CCA_COUNT_MAXC_S     9
#define AR_PHY_CF_MAXCCAPWR_0       0x000001FF
#define AR_PHY_CF_MAXCCAPWR_0_S     0
/* BB_cca_ctrl_2_b0 */
#define AR_PHY_EXT_CCA0_THRESH62    0x000000FF
#define AR_PHY_EXT_CCA0_THRESH62_S  0
#define AR_PHY_EXT_CCA0_THRESH62_1    0x000001FF
#define AR_PHY_EXT_CCA0_THRESH62_1_S  0
#define AR_PHY_EXT_CCA0_THRESH62_ENABLE    0x00000200
#define AR_PHY_EXT_CCA0_THRESH62_ENABLE_S  9
#define AR_PHY_EXT_CCA0_THRESH62_MODE    0x00040000
#define AR_PHY_EXT_CCA0_THRESH62_MODE_S  18
/* BB_bbb_sig_detect */
#define AR_PHY_CCK_DETECT_WEAK_SIG_THR_CCK          0x0000003F
#define AR_PHY_CCK_DETECT_WEAK_SIG_THR_CCK_S        0
#define AR_PHY_CCK_DETECT_ANT_SWITCH_TIME           0x00001FC0 // [12:6] settling time for antenna switch
#define AR_PHY_CCK_DETECT_ANT_SWITCH_TIME_S         6
#define AR_PHY_CCK_DETECT_BB_ENABLE_ANT_FAST_DIV    0x2000
#define AR_PHY_CCK_DETECT_BB_ENABLE_ANT_FAST_DIV_S  13

/* BB_bbb_dagc_ctrl */
#define AR_PHY_DAG_CTRLCCK_EN_RSSI_THR  0x00000200
#define AR_PHY_DAG_CTRLCCK_EN_RSSI_THR_S  9
#define AR_PHY_DAG_CTRLCCK_RSSI_THR 0x0001FC00
#define AR_PHY_DAG_CTRLCCK_RSSI_THR_S   10

/* BB_rifs_srch */
#define AR_PHY_RIFS_INIT_DELAY         0x3ff0000

/*B_tpc_7*/
#define AR_PHY_TPC_7_TX_GAIN_TABLE_MAX		0x3f
#define AR_PHY_TPC_7_TX_GAIN_TABLE_MAX_S	(0)

/* BB_agc */
#define AR_PHY_AGC_QUICK_DROP_S (22)
#define AR_PHY_AGC_QUICK_DROP (0xf << AR_PHY_AGC_QUICK_DROP_S)
#define AR_PHY_AGC_COARSE_LOW       0x00007F80
#define AR_PHY_AGC_COARSE_LOW_S     7
#define AR_PHY_AGC_COARSE_HIGH      0x003F8000
#define AR_PHY_AGC_COARSE_HIGH_S    15
#define AR_PHY_AGC_COARSE_PWR_CONST 0x0000007F    
#define AR_PHY_AGC_COARSE_PWR_CONST_S   0
/* BB_find_signal */
#define AR_PHY_FIND_SIG_FIRSTEP  0x0003F000
#define AR_PHY_FIND_SIG_FIRSTEP_S        12
#define AR_PHY_FIND_SIG_FIRPWR   0x03FC0000
#define AR_PHY_FIND_SIG_FIRPWR_S         18
#define AR_PHY_FIND_SIG_FIRPWR_SIGN_BIT  25
#define AR_PHY_FIND_SIG_RELPWR   (0x1f << 6)
#define AR_PHY_FIND_SIG_RELPWR_S          6
#define AR_PHY_FIND_SIG_RELPWR_SIGN_BIT  11
#define AR_PHY_FIND_SIG_RELSTEP        0x1f
#define AR_PHY_FIND_SIG_RELSTEP_S         0
#define AR_PHY_FIND_SIG_RELSTEP_SIGN_BIT  5
/* BB_restart */
#define AR_PHY_RESTART_DIV_GC   0x001C0000 /* bb_ant_fast_div_gc_limit */
#define AR_PHY_RESTART_DIV_GC_S 18
#define AR_PHY_RESTART_ENA      0x01       /* enable restart */
#define AR_PHY_DC_RESTART_DIS   0x40000000 /* disable DC restart */

#define AR_PHY_TPC_OLPC_GAIN_DELTA_PAL_ON       0xFF000000 //Mask BIT[31:24]
#define AR_PHY_TPC_OLPC_GAIN_DELTA_PAL_ON_S     24
#define AR_PHY_TPC_OLPC_GAIN_DELTA              0x00FF0000 //Mask BIT[23:16]
#define AR_PHY_TPC_OLPC_GAIN_DELTA_S            16

#define AR_PHY_TPC_6_ERROR_EST_MODE             0x03000000 //Mask BIT[25:24]
#define AR_PHY_TPC_6_ERROR_EST_MODE_S           24

/*
 * SM Register Map
 */
#define AR_SM_BASE      offsetof(struct bb_reg_map, bb_sm_reg_map)
#define AR_SM_OFFSET(_x)   (AR_SM_BASE + offsetof(struct sm_reg_map, _x))

#define AR_EXT_SM_BASE      offsetof(struct bb_reg_map, overlay_0xa800.Poseidon.bb_sm_ext_reg_map)
#define AR_EXT_SM_OFFSET(_x)   (AR_EXT_SM_BASE + offsetof(struct sm_ext_reg_map, _x))

#define AR_PHY_D2_CHIP_ID        AR_SM_OFFSET(BB_D2_chip_id)
#define AR_PHY_GEN_CTRL          AR_SM_OFFSET(BB_gen_controls)
#define AR_PHY_MODE              AR_SM_OFFSET(BB_modes_select)
#define AR_PHY_ACTIVE            AR_SM_OFFSET(BB_active)
#define AR_PHY_SPUR_MASK_A       AR_SM_OFFSET(overlay_0xa214.Osprey.BB_vit_spur_mask_A)
#define AR_PHY_SPUR_MASK_B       AR_SM_OFFSET(overlay_0xa214.Osprey.BB_vit_spur_mask_B)
#define AR_PHY_SPUR_MASK_A_DRAGONFLY AR_SM_OFFSET(overlay_0xa214.Dragonfly.BB_vit_spur_mask_A)
#define AR_PHY_SPUR_MASK_B_DRAGONFLY AR_SM_OFFSET(overlay_0xa214.Dragonfly.BB_vit_spur_mask_B)
#define AR_PHY_SPUR_MASK_C_DRAGONFLY AR_SM_OFFSET(overlay_0xa214.Dragonfly.BB_vit_spur_mask_C)
#define AR_PHY_SPUR_MASK_D_DRAGONFLY AR_SM_OFFSET(overlay_0xa214.Dragonfly.BB_vit_spur_mask_D)
#define QCN5500_PHY_SPUR_MASK_A      AR_SM_OFFSET(overlay_0xa214.Jet.BB_vit_spur_mask_A)
#define QCN5500_PHY_SPUR_MASK_B      AR_SM_OFFSET(overlay_0xa214.Jet.BB_vit_spur_mask_B)
#define QCN5500_PHY_SPUR_MASK_C      AR_SM_OFFSET(overlay_0xa214.Jet.BB_vit_spur_mask_C)
#define QCN5500_PHY_SPUR_MASK_D      AR_SM_OFFSET(overlay_0xa214.Jet.BB_vit_spur_mask_D)

#define MAC_PCU_TST_ADDAC__SAMPLE_SIZE_2K__SHIFT         15
#define MAC_PCU_TST_ADDAC__SAMPLE_SIZE_2K__MASK          0x00008000U
#define MAC_PCU_TST_ADDAC__SAMPLE_SIZE_2K__WIDTH         1
#define GEN_CTRL__DYN_20_40__SHIFT                   2
#define GEN_CTRL__DYN_20_40__WIDTH                   1
#define GEN_CTRL__DYN_20_40__MASK                    0x00000004U

#define AR_PHY_SPECTRAL_SCAN     AR_SM_OFFSET(BB_spectral_scan)
#define AR_PHY_RADAR_BW_FILTER   AR_SM_OFFSET(BB_radar_bw_filter)
#define AR_PHY_SEARCH_START_DELAY AR_SM_OFFSET(BB_search_start_delay)
#define AR_PHY_MAX_RX_LEN        AR_SM_OFFSET(BB_max_rx_length)
#define AR_PHY_FRAME_CTL         AR_SM_OFFSET(BB_frame_control)
#define AR_PHY_FRAME_CTL_CF_TX_CLIP					0x7
#define AR_PHY_FRAME_CTL_CF_TX_CLIP_S	    		3
#define AR_PHY_RFBUS_REQ         AR_SM_OFFSET(BB_rfbus_request)
#define AR_PHY_RFBUS_GRANT       AR_SM_OFFSET(BB_rfbus_grant)
#define AR_PHY_RIFS              AR_SM_OFFSET(BB_rifs)
#define AR_PHY_RX_CLR_DELAY      AR_SM_OFFSET(BB_rx_clear_delay)
#define AR_PHY_RX_DELAY          AR_SM_OFFSET(BB_analog_power_on_time)
#define AR_PHY_BB_POWERTX_RATE9  AR_SM_OFFSET(BB_powertx_rate9) 
#define AR_PHY_TPC_7			 AR_SM_OFFSET(BB_tpc_7)
#define AR_PHY_CL_MAP_0_B0       AR_SM_OFFSET(BB_cl_map_0_b0)
#define AR_PHY_CL_MAP_1_B0       AR_SM_OFFSET(BB_cl_map_1_b0)
#define AR_PHY_CL_MAP_2_B0       AR_SM_OFFSET(BB_cl_map_2_b0)
#define AR_PHY_CL_MAP_3_B0       AR_SM_OFFSET(BB_cl_map_3_b0)

#define AR_PHY_RF_CTL(_i)        AR_SM_OFFSET(BB_tx_timing_##_i)

#define AR_PHY_XPA_TIMING_CTL    AR_SM_OFFSET(BB_xpa_timing_control)
#define AR_PHY_MISC_PA_CTL       AR_SM_OFFSET(BB_misc_pa_control)
#define AR_PHY_SWITCH_CHAIN_0    AR_SM_OFFSET(BB_switch_table_chn_b0)
#define AR_PHY_SWITCH_COM        AR_SM_OFFSET(BB_switch_table_com1)
#define AR_PHY_SWITCH_COM_2      AR_SM_OFFSET(BB_switch_table_com2)
#define AR_PHY_RX_CHAINMASK      AR_SM_OFFSET(BB_multichain_enable)
#define AR_PHY_CAL_CHAINMASK     AR_SM_OFFSET(BB_cal_chain_mask)
#define AR_PHY_AGC_CONTROL       AR_SM_OFFSET(BB_agc_control)
#define AR_PHY_CALMODE           AR_SM_OFFSET(BB_iq_adc_cal_mode)
#define AR_PHY_FCAL_1            AR_SM_OFFSET(BB_fcal_1)
#define AR_PHY_FCAL_2_0          AR_SM_OFFSET(BB_fcal_2_b0)
#define AR_PHY_DFT_TONE_CTL_0    AR_SM_OFFSET(BB_dft_tone_ctrl_b0)
#define AR_PHY_CL_CAL_CTL        AR_SM_OFFSET(BB_cl_cal_ctrl)
#define CL_CAL_CTL__ENABLE_PARALLEL_CAL__SHIFT      0
#define CL_CAL_CTL__ENABLE_PARALLEL_CAL__MASK       0x00000001U
#define CL_CAL_CTL__ENABLE_CL_CALIBRATE__SHIFT      1
#define CL_CAL_CTL__ENABLE_CL_CALIBRATE__MASK       0x00000002U
#define AR_PHY_CL_TAB_0          AR_SM_OFFSET(BB_cl_tab_b0)
#define AR_PHY_SYNTH_CONTROL     AR_SM_OFFSET(BB_synth_control)
#define AR_PHY_ADDAC_CLK_SEL     AR_SM_OFFSET(BB_addac_clk_select)
#define AR_PHY_PLL_CTL           AR_SM_OFFSET(BB_pll_cntl)
#define AR_PHY_ANALOG_SWAP       AR_SM_OFFSET(BB_analog_swap)
#define AR_PHY_ADDAC_PARA_CTL    AR_SM_OFFSET(BB_addac_parallel_control)
#define AR_PHY_XPA_CFG           AR_SM_OFFSET(BB_force_analog)
#define AR_PHY_AIC_CTRL_0_B0_10  AR_SM_OFFSET(overlay_0xa580.Jupiter_10.BB_aic_ctrl_0_b0)
#define AR_PHY_AIC_CTRL_1_B0_10  AR_SM_OFFSET(overlay_0xa580.Jupiter_10.BB_aic_ctrl_1_b0)
#define AR_PHY_AIC_CTRL_2_B0_10  AR_SM_OFFSET(overlay_0xa580.Jupiter_10.BB_aic_ctrl_2_b0)
#define AR_PHY_AIC_CTRL_3_B0_10  AR_SM_OFFSET(overlay_0xa580.Jupiter_10.BB_aic_ctrl_3_b0)
#define AR_PHY_AIC_STAT_0_B0_10  AR_SM_OFFSET(overlay_0xa580.Jupiter_10.BB_aic_stat_0_b0)
#define AR_PHY_AIC_STAT_1_B0_10  AR_SM_OFFSET(overlay_0xa580.Jupiter_10.BB_aic_stat_1_b0)
#define AR_PHY_AIC_CTRL_0_B0_20  AR_SM_OFFSET(overlay_0xa580.Jupiter_20.BB_aic_ctrl_0_b0)
#define AR_PHY_AIC_CTRL_1_B0_20  AR_SM_OFFSET(overlay_0xa580.Jupiter_20.BB_aic_ctrl_1_b0)
#define AR_PHY_AIC_CTRL_2_B0_20  AR_SM_OFFSET(overlay_0xa580.Jupiter_20.BB_aic_ctrl_2_b0)
#define AR_PHY_AIC_CTRL_3_B0_20  AR_SM_OFFSET(overlay_0xa580.Jupiter_20.BB_aic_ctrl_3_b0)
#define AR_PHY_AIC_CTRL_4_B0_20  AR_SM_OFFSET(overlay_0xa580.Jupiter_20.BB_aic_ctrl_4_b0)
#define AR_PHY_AIC_STAT_0_B0_20  AR_SM_OFFSET(overlay_0xa580.Jupiter_20.BB_aic_stat_0_b0)
#define AR_PHY_AIC_STAT_1_B0_20  AR_SM_OFFSET(overlay_0xa580.Jupiter_20.BB_aic_stat_1_b0)
#define AR_PHY_AIC_STAT_2_B0_20  AR_SM_OFFSET(overlay_0xa580.Jupiter_20.BB_aic_stat_2_b0)
#define AR_PHY_AIC_CTRL_0_B1_10  AR_SM1_OFFSET(overlay_0x4b0.Jupiter_10.BB_aic_ctrl_0_b1)
#define AR_PHY_AIC_CTRL_1_B1_10  AR_SM1_OFFSET(overlay_0x4b0.Jupiter_10.BB_aic_ctrl_1_b1)
#define AR_PHY_AIC_STAT_0_B1_10  AR_SM1_OFFSET(overlay_0x4b0.Jupiter_10.BB_aic_stat_0_b1)
#define AR_PHY_AIC_STAT_1_B1_10  AR_SM1_OFFSET(overlay_0x4b0.Jupiter_10.BB_aic_stat_1_b1)
#define AR_PHY_AIC_CTRL_0_B1_20  AR_SM1_OFFSET(overlay_0x4b0.Jupiter_20.BB_aic_ctrl_0_b1)
#define AR_PHY_AIC_CTRL_1_B1_20  AR_SM1_OFFSET(overlay_0x4b0.Jupiter_20.BB_aic_ctrl_1_b1)
#define AR_PHY_AIC_CTRL_4_B1_20  AR_SM1_OFFSET(overlay_0x4b0.Jupiter_20.BB_aic_ctrl_4_b1)
#define AR_PHY_AIC_STAT_0_B1_20  AR_SM1_OFFSET(overlay_0x4b0.Jupiter_20.BB_aic_stat_0_b1)
#define AR_PHY_AIC_STAT_1_B1_20  AR_SM1_OFFSET(overlay_0x4b0.Jupiter_20.BB_aic_stat_1_b1)
#define AR_PHY_AIC_STAT_2_B1_20  AR_SM1_OFFSET(overlay_0x4b0.Jupiter_20.BB_aic_stat_2_b1)
#define AR_PHY_AIC_SRAM_ADDR_B0  AR_SM_OFFSET(BB_tables_intf_addr_b0)
#define AR_PHY_AIC_SRAM_DATA_B0  AR_SM_OFFSET(BB_tables_intf_data_b0)
#define AR_PHY_AIC_SRAM_ADDR_B1  AR_SM1_OFFSET(overlay_0x4b0.Jupiter_10.BB_tables_intf_addr_b1)
#define AR_PHY_AIC_SRAM_DATA_B1  AR_SM1_OFFSET(overlay_0x4b0.Jupiter_10.BB_tables_intf_data_b1)

/*BB_peak_det_ctrl_1  -  AR_PHY_PEAK_DET_CTRL_1*/
#define AR_PHY_USE_PEAK_DET                     0x00000001
#define AR_PHY_USE_PEAK_DET_S                   1

/* AIC fields */
#define AR_PHY_AIC_MON_ENABLE                   0x80000000
#define AR_PHY_AIC_MON_ENABLE_S                 31
#define AR_PHY_AIC_CAL_MAX_HOP_COUNT            0x7F000000
#define AR_PHY_AIC_CAL_MAX_HOP_COUNT_S          24
#define AR_PHY_AIC_CAL_MIN_VALID_COUNT          0x00FE0000
#define AR_PHY_AIC_CAL_MIN_VALID_COUNT_S        17
#define AR_PHY_AIC_F_WLAN                       0x0001FC00
#define AR_PHY_AIC_F_WLAN_S                     10
#define AR_PHY_AIC_CAL_CH_VALID_RESET           0x00000200
#define AR_PHY_AIC_CAL_CH_VALID_RESET_S         9
#define AR_PHY_AIC_CAL_ENABLE                   0x00000100
#define AR_PHY_AIC_CAL_ENABLE_S                 8
#define AR_PHY_AIC_BTTX_PWR_THR                 0x000000FE
#define AR_PHY_AIC_BTTX_PWR_THR_S               1
#define AR_PHY_AIC_ENABLE                       0x00000001
#define AR_PHY_AIC_ENABLE_S                     0
#define AR_PHY_AIC_CAL_BT_REF_DELAY             0x78000000
#define AR_PHY_AIC_CAL_BT_REF_DELAY_S           27
#define AR_PHY_AIC_CAL_ROT_ATT_DB_EST_ISO       0x07000000
#define AR_PHY_AIC_CAL_ROT_ATT_DB_EST_ISO_S     24
#define AR_PHY_AIC_CAL_COM_ATT_DB_EST_ISO       0x00F00000
#define AR_PHY_AIC_CAL_COM_ATT_DB_EST_ISO_S     20
#define AR_PHY_AIC_BT_IDLE_CFG                  0x00080000
#define AR_PHY_AIC_BT_IDLE_CFG_S                19
#define AR_PHY_AIC_STDBY_COND                   0x00060000
#define AR_PHY_AIC_STDBY_COND_S                 17
#define AR_PHY_AIC_STDBY_ROT_ATT_DB             0x0001F800
#define AR_PHY_AIC_STDBY_ROT_ATT_DB_S           11
#define AR_PHY_AIC_STDBY_COM_ATT_DB             0x00000700
#define AR_PHY_AIC_STDBY_COM_ATT_DB_S           8
#define AR_PHY_AIC_RSSI_MAX                     0x000000F0
#define AR_PHY_AIC_RSSI_MAX_S                   4
#define AR_PHY_AIC_RSSI_MIN                     0x0000000F
#define AR_PHY_AIC_RSSI_MIN_S                   0
#define AR_PHY_AIC_RADIO_DELAY                  0x7F000000
#define AR_PHY_AIC_RADIO_DELAY_S                24
#define AR_PHY_AIC_CAL_STEP_SIZE_CORR           0x00F00000
#define AR_PHY_AIC_CAL_STEP_SIZE_CORR_S         20
#define AR_PHY_AIC_CAL_ROT_IDX_CORR             0x000F8000
#define AR_PHY_AIC_CAL_ROT_IDX_CORR_S           15
#define AR_PHY_AIC_CAL_CONV_CHECK_FACTOR        0x00006000
#define AR_PHY_AIC_CAL_CONV_CHECK_FACTOR_S      13
#define AR_PHY_AIC_ROT_IDX_COUNT_MAX            0x00001C00
#define AR_PHY_AIC_ROT_IDX_COUNT_MAX_S          10
#define AR_PHY_AIC_CAL_SYNTH_TOGGLE             0x00000200
#define AR_PHY_AIC_CAL_SYNTH_TOGGLE_S           9
#define AR_PHY_AIC_CAL_SYNTH_AFTER_BTRX         0x00000100
#define AR_PHY_AIC_CAL_SYNTH_AFTER_BTRX_S       8
#define AR_PHY_AIC_CAL_SYNTH_SETTLING           0x000000FF
#define AR_PHY_AIC_CAL_SYNTH_SETTLING_S         0
#define AR_PHY_AIC_MON_MAX_HOP_COUNT            0x0FE00000
#define AR_PHY_AIC_MON_MAX_HOP_COUNT_S          21
#define AR_PHY_AIC_MON_MIN_STALE_COUNT          0x001FC000
#define AR_PHY_AIC_MON_MIN_STALE_COUNT_S        14
#define AR_PHY_AIC_MON_PWR_EST_LONG             0x00002000
#define AR_PHY_AIC_MON_PWR_EST_LONG_S           13
#define AR_PHY_AIC_MON_PD_TALLY_SCALING         0x00001800
#define AR_PHY_AIC_MON_PD_TALLY_SCALING_S       11
#define AR_PHY_AIC_MON_PERF_THR                 0x000007C0
#define AR_PHY_AIC_MON_PERF_THR_S               6
#define AR_PHY_AIC_CAL_COM_ATT_DB_FIXED         0x00000020
#define AR_PHY_AIC_CAL_COM_ATT_DB_FIXED_S       5
#define AR_PHY_AIC_CAL_TARGET_MAG_SETTING       0x00000018
#define AR_PHY_AIC_CAL_TARGET_MAG_SETTING_S     3
#define AR_PHY_AIC_CAL_PERF_CHECK_FACTOR        0x00000006
#define AR_PHY_AIC_CAL_PERF_CHECK_FACTOR_S      1
#define AR_PHY_AIC_CAL_PWR_EST_LONG             0x00000001
#define AR_PHY_AIC_CAL_PWR_EST_LONG_S           0
#define AR_PHY_AIC_MON_DONE                     0x80000000
#define AR_PHY_AIC_MON_DONE_S                   31
#define AR_PHY_AIC_MON_ACTIVE                   0x40000000
#define AR_PHY_AIC_MON_ACTIVE_S                 30
#define AR_PHY_AIC_MEAS_COUNT                   0x3F000000
#define AR_PHY_AIC_MEAS_COUNT_S                 24
#define AR_PHY_AIC_CAL_ANT_ISO_EST              0x00FC0000
#define AR_PHY_AIC_CAL_ANT_ISO_EST_S            18
#define AR_PHY_AIC_CAL_HOP_COUNT                0x0003F800
#define AR_PHY_AIC_CAL_HOP_COUNT_S              11
#define AR_PHY_AIC_CAL_VALID_COUNT              0x000007F0
#define AR_PHY_AIC_CAL_VALID_COUNT_S            4
#define AR_PHY_AIC_CAL_BT_TOO_WEAK_ERR          0x00000008
#define AR_PHY_AIC_CAL_BT_TOO_WEAK_ERR_S        3
#define AR_PHY_AIC_CAL_BT_TOO_STRONG_ERR        0x00000004
#define AR_PHY_AIC_CAL_BT_TOO_STRONG_ERR_S      2
#define AR_PHY_AIC_CAL_DONE                     0x00000002
#define AR_PHY_AIC_CAL_DONE_S                   1
#define AR_PHY_AIC_CAL_ACTIVE                   0x00000001
#define AR_PHY_AIC_CAL_ACTIVE_S                 0
#define AR_PHY_AIC_MEAS_MAG_MIN                 0xFFC00000
#define AR_PHY_AIC_MEAS_MAG_MIN_S               22
#define AR_PHY_AIC_MON_STALE_COUNT              0x003F8000
#define AR_PHY_AIC_MON_STALE_COUNT_S            15
#define AR_PHY_AIC_MON_HOP_COUNT                0x00007F00
#define AR_PHY_AIC_MON_HOP_COUNT_S              8
#define AR_PHY_AIC_CAL_AIC_SM                   0x000000F8
#define AR_PHY_AIC_CAL_AIC_SM_S                 3
#define AR_PHY_AIC_SM                           0x00000007
#define AR_PHY_AIC_SM_S                         0
#define AR_PHY_AIC_SRAM_VALID                   0x00000001
#define AR_PHY_AIC_SRAM_VALID_S                 0
#define AR_PHY_AIC_SRAM_ROT_QUAD_ATT_DB         0x0000007E
#define AR_PHY_AIC_SRAM_ROT_QUAD_ATT_DB_S       1
#define AR_PHY_AIC_SRAM_VGA_QUAD_SIGN           0x00000080
#define AR_PHY_AIC_SRAM_VGA_QUAD_SIGN_S         7
#define AR_PHY_AIC_SRAM_ROT_DIR_ATT_DB          0x00003F00
#define AR_PHY_AIC_SRAM_ROT_DIR_ATT_DB_S        8
#define AR_PHY_AIC_SRAM_VGA_DIR_SIGN            0x00004000
#define AR_PHY_AIC_SRAM_VGA_DIR_SIGN_S          14
#define AR_PHY_AIC_SRAM_COM_ATT_6DB             0x00038000
#define AR_PHY_AIC_SRAM_COM_ATT_6DB_S           15

#define AR_PHY_FRAME_CTL_CF_OVERLAP_WINDOW	3
#define AR_PHY_FRAME_CTL_CF_OVERLAP_WINDOW_S	0

/* BB_cl_tab_bx */
#define AR_PHY_CL_TAB_CARR_LK_DC_ADD_I              0x07FF0000
#define AR_PHY_CL_TAB_CARR_LK_DC_ADD_I_S            16
#define AR_PHY_CL_TAB_CARR_LK_DC_ADD_Q              0x0000FFE0
#define AR_PHY_CL_TAB_CARR_LK_DC_ADD_Q_S            5
#define AR_PHY_CL_TAB_GAIN_MOD                      0x0000001F
#define AR_PHY_CL_TAB_GAIN_MOD_S                    0

/* BB_vit_spur_mask_A fields */
#define AR_PHY_SPUR_MASK_A_CF_PUNC_MASK_IDX_A		0x0001FC00
#define AR_PHY_SPUR_MASK_A_CF_PUNC_MASK_IDX_A_S		10
#define AR_PHY_SPUR_MASK_A_CF_PUNC_MASK_A			0x3FF
#define AR_PHY_SPUR_MASK_A_CF_PUNC_MASK_A_S			0

/* enable_flt_svd*/
#define AR_PHY_ENABLE_FLT_SVD                       0x00001000
#define AR_PHY_ENABLE_FLT_SVD_S                     12

#define AR_PHY_TEST              AR_SM_OFFSET(overlay_0xa35c.Osprey.BB_test_controls)
#define AR_PHY_TEST_DRAGONFLY    AR_SM_OFFSET(overlay_0xa35c.Dragonfly.BB_test_controls)
#define AR_PHY_TEST_CONTROLS     AR_SM_OFFSET(overlay_0xa35c.Jet.BB_test_controls)

#define AR_PHY_TEST_BBB_OBS_SEL       0x780000
#define AR_PHY_TEST_BBB_OBS_SEL_S     19 /* bits 19 to 22 are cf_bbb_obs_sel*/ 

#define AR_PHY_TEST_RX_OBS_SEL_BIT5_S 23 
#define AR_PHY_TEST_RX_OBS_SEL_BIT5   (1 << AR_PHY_TEST_RX_OBS_SEL_BIT5_S)// This is bit 5 for cf_rx_obs_sel

#define AR_PHY_TEST_CHAIN_SEL      0xC0000000 
#define AR_PHY_TEST_CHAIN_SEL_S    30 /*bits 30 and 31 are tstdac_out_sel which selects which chain to drive out*/         

#define AR_PHY_TEST_CTL_STATUS   AR_SM_OFFSET(overlay_0xa35c.Osprey.BB_test_controls_status)
#define AR_PHY_TEST_CTL_STATUS_DRAGONFLY   AR_SM_OFFSET(overlay_0xa35c.Dragonfly.BB_test_controls_status)
#define AR_PHY_TEST_CTL_STATUS_JET  AR_SM_OFFSET(overlay_0xa35c.Jet.BB_test_controls_status)

#define AR_PHY_TEST_CTL_TSTDAC_EN         0x1
#define AR_PHY_TEST_CTL_TSTDAC_EN_S       0 /*cf_tstdac_en, driver to tstdac bus, 0=disable, 1=enable*/
#define AR_PHY_TEST_CONTROLS_TSTDAC_OUT_SEL   0xc0000000
#define AR_PHY_TEST_CONTROLS_TSTDAC_OUT_SEL_S 30
#define AR_PHY_TEST_CONTROLS_FORCE_AGC_CLEAR   0x10000000
#define AR_PHY_TEST_CONTROLS_FORCE_AGC_CLEAR_S 28
#define AR_PHY_TEST_CONTROLS_AGC_OBS_SEL_4   0x1000000
#define AR_PHY_TEST_CONTROLS_AGC_OBS_SEL_4_S 24
#define AR_PHY_TEST_CONTROLS_RX_OBS_SEL_5TH_BIT   0x800000
#define AR_PHY_TEST_CONTROLS_RX_OBS_SEL_5TH_BIT_S 23
#define AR_PHY_TEST_CONTROLS_CF_BBB_OBS_SEL   0x780000
#define AR_PHY_TEST_CONTROLS_CF_BBB_OBS_SEL_S 19
#define AR_PHY_TEST_CONTROLS_AGC_OBS_SEL_3   0x40000
#define AR_PHY_TEST_CONTROLS_AGC_OBS_SEL_3_S 18
#define AR_PHY_TEST_CONTROLS_SLOW_CLK160   0x20000
#define AR_PHY_TEST_CONTROLS_SLOW_CLK160_S 17
#define AR_PHY_TEST_CONTROLS_ENABLE_MINI_OBS   0x8000
#define AR_PHY_TEST_CONTROLS_ENABLE_MINI_OBS_S 15
#define AR_PHY_TEST_CONTROLS_ENABLE_RFSILENT_BB   0x2000
#define AR_PHY_TEST_CONTROLS_ENABLE_RFSILENT_BB_S 13
#define AR_PHY_TEST_CONTROLS_CLKOUT_IS_CLK32   0x400
#define AR_PHY_TEST_CONTROLS_CLKOUT_IS_CLK32_S 10
#define AR_PHY_TEST_CONTROLS_CARDBUS_MODE   0x300
#define AR_PHY_TEST_CONTROLS_CARDBUS_MODE_S 8
#define AR_PHY_TEST_CONTROLS_CF_RFSHIFT_SEL   0x60
#define AR_PHY_TEST_CONTROLS_CF_RFSHIFT_SEL_S 5
#define AR_PHY_TEST_CONTROLS_CF_TSTTRIG   0x10
#define AR_PHY_TEST_CONTROLS_CF_TSTTRIG_S 4
#define AR_PHY_TEST_CONTROLS_CF_TSTTRIG_SEL   0xf
#define AR_PHY_TEST_CONTROLS_CF_TSTTRIG_SEL_S 0

#define AR_PHY_TEST_CTL_TX_OBS_SEL        0x1C
#define AR_PHY_TEST_CTL_TX_OBS_SEL_S      2 /* cf_tx_obs_sel, bits 2:4*/
#define AR_PHY_TEST_CTL_TX_OBS_MUX_SEL    0x60
#define AR_PHY_TEST_CTL_TX_OBS_MUX_SEL_S  5 /* cf_tx_obs_sel, bits 5:6, setting to 11 selects ADC*/
#define AR_PHY_TEST_CTL_TSTADC_EN         0x100
#define AR_PHY_TEST_CTL_TSTADC_EN_S       8 /*cf_tstadc_en, driver to tstadc bus, 0=disable, 1=enable*/
#define AR_PHY_TEST_CTL_RX_OBS_SEL        0x3C00
#define AR_PHY_TEST_CTL_RX_OBS_SEL_S      10 /* cf_tx_obs_sel, bits 10:13*/
#define AR_PHY_TEST_CTL_FORCE_AGC_CLR     0x10000000
#define AR_PHY_TEST_CTL_FORCE_AGC_CLR_S   28
#define AR_PHY_TEST_CTL_DEBUGPORT_SEL     0xe0000000
#define AR_PHY_TEST_CTL_DEBUGPORT_SEL_S   29

#define AR_PHY_TEST_CTL_STATUS_CF_DEBUGPORT_SEL   0xe0000000
#define AR_PHY_TEST_CTL_STATUS_CF_DEBUGPORT_SEL_S 29
#define AR_PHY_TEST_CTL_STATUS_CF_DEBUGPORT_EN   0x10000000
#define AR_PHY_TEST_CTL_STATUS_CF_DEBUGPORT_EN_S 28
#define AR_PHY_TEST_CTL_STATUS_DISABLE_AGC_TO_A2   0x8000000
#define AR_PHY_TEST_CTL_STATUS_DISABLE_AGC_TO_A2_S 27
#define AR_PHY_TEST_CTL_STATUS_CF_DEBUGPORT_IN   0x800000
#define AR_PHY_TEST_CTL_STATUS_CF_DEBUGPORT_IN_S 23
#define AR_PHY_TEST_CTL_STATUS_CF_ENABLE_FFT_DUMP   0x80000
#define AR_PHY_TEST_CTL_STATUS_CF_ENABLE_FFT_DUMP_S 19
#define AR_PHY_TEST_CTL_STATUS_AGC_OBS_SEL   0x70000
#define AR_PHY_TEST_CTL_STATUS_AGC_OBS_SEL_S 16
#define AR_PHY_TEST_CTL_STATUS_RESET_A2   0x8000
#define AR_PHY_TEST_CTL_STATUS_RESET_A2_S 15
#define AR_PHY_TEST_CTL_STATUS_DISABLE_A2_WARM_RESET   0x4000
#define AR_PHY_TEST_CTL_STATUS_DISABLE_A2_WARM_RESET_S 14
#define TEST_CTL_STATUS_JET__RX_OBS_SEL__MASK   0x3c00
#define TEST_CTL_STATUS_JET__RX_OBS_SEL__SHIFT  10
#define AR_PHY_TEST_CTL_STATUS_CF_RX_SRC_IS_TSTADC   0x200
#define AR_PHY_TEST_CTL_STATUS_CF_RX_SRC_IS_TSTADC_S 9
#define AR_PHY_TEST_CTL_STATUS_CF_TSTADC_EN   0x100
#define AR_PHY_TEST_CTL_STATUS_CF_TSTADC_EN_S 8
#define AR_PHY_TEST_CTL_STATUS_CF_TX_SRC_ALTERNATE   0x80
#define AR_PHY_TEST_CTL_STATUS_CF_TX_SRC_ALTERNATE_S 7
#define TEST_CTL_STATUS_JET__CF_TX_OBS_MUX_SEL__MASK   0x60
#define TEST_CTL_STATUS_JET__CF_TX_OBS_MUX_SEL__MASK_S  5
#define TEST_CTL_STATUS_JET__CF_TX_OBS_MUX_SEL__SHIFT  5
#define TEST_CTL_STATUS_JET__CF_TX_OBS_SEL__MASK   0x1c
#define TEST_CTL_STATUS_JET__CF_TX_OBS_SEL__MASK_S   2
#define TEST_CTL_STATUS_JET__CF_TX_OBS_SEL__SHIFT  2
#define AR_PHY_TEST_CTL_STATUS_CF_TX_SRC_IS_TSTDAC   0x2
#define AR_PHY_TEST_CTL_STATUS_CF_TX_SRC_IS_TSTDAC_S 1
#define AR_PHY_TEST_CTL_STATUS_CF_TSTDAC_EN   0x1
#define AR_PHY_TEST_CTL_STATUS_CF_TSTDAC_EN_S 0

#define AR_PHY_TSTDAC            AR_SM_OFFSET(overlay_0xa35c.Osprey.BB_tstdac)
#define QCN5500_PHY_TSTDAC            AR_SM_OFFSET(overlay_0xa35c.Jet.BB_tstdac)
#define AR_PHY_CHAN_STATUS       AR_SM_OFFSET(overlay_0xa35c.Osprey.BB_channel_status)
#define AR_PHY_CHAN_INFO_MEMORY  AR_SM_OFFSET(overlay_0xa35c.Osprey.BB_chaninfo_ctrl)
#define AR_PHY_CHAN_INFO_MEMORY_DRAGONFLY  AR_SM_OFFSET(overlay_0xa35c.Dragonfly.BB_chaninfo_ctrl)

#define AR_PHY_TSTDAC_TSTDAC_OUT_I   0xffc00
#define AR_PHY_TSTDAC_TSTDAC_OUT_I_S 10
#define AR_PHY_TSTDAC_TSTDAC_OUT_Q   0x3ff
#define AR_PHY_TSTDAC_TSTDAC_OUT_Q_S 0
#define AR_PHY_CHNINFO_NOISEPWR  AR_SM_OFFSET(overlay_0xa35c.Osprey.BB_chan_info_noise_pwr)
#define AR_PHY_CHNINFO_GAINDIFF  AR_SM_OFFSET(overlay_0xa35c.Osprey.BB_chan_info_gain_diff)
#define AR_PHY_CHNINFO_GAINDIFF_DRAGONFLY  AR_SM_OFFSET(overlay_0xa35c.Dragonfly.BB_chan_info_gain_diff)
#define AR_PHY_CHNINFO_FINETIM   AR_SM_OFFSET(overlay_0xa35c.Osprey.BB_chan_info_fine_timing)
#define AR_PHY_CHAN_INFO_GAIN_0  AR_SM_OFFSET(overlay_0xa35c.Osprey.BB_chan_info_gain_b0)
#define AR_PHY_CHAN_INFO_GAIN_0_DRAGONFLY  AR_SM_OFFSET(overlay_0xa35c.Dragonfly.BB_chan_info_gain_b0)
#define QCN5500_PHY_CHAN_INFO_GAIN_0       AR_SM_OFFSET(overlay_0xa35c.Jet.BB_chan_info_gain_b0)
#define QCN5500_PHY_CHNINFO_GAINDIFF       AR_SM_OFFSET(overlay_0xa35c.Jet.BB_chan_info_gain_diff)
#define QCN5500_PHY_CHAN_INFO_MEMORY       AR_SM_OFFSET(overlay_0xa35c.Jet.BB_chaninfo_ctrl)
#define AR_PHY_SCRAMBLER_SEED    AR_SM_OFFSET(overlay_0xa35c.Osprey.BB_scrambler_seed)
#define AR_PHY_CCK_TX_CTRL       AR_SM_OFFSET(overlay_0xa35c.Osprey.BB_bbb_tx_ctrl)
#define AR_PHY_TX_FIR(_i)        AR_SM_OFFSET(overlay_0xa35c.Osprey.BB_bbb_txfir_##_i)
#define AR_PHY_HEAVYCLIP_CTL     AR_SM_OFFSET(overlay_0xa35c.Osprey.BB_heavy_clip_ctrl)
#define AR_PHY_HEAVYCLIP_CTL_DRAGONFLY     AR_SM_OFFSET(overlay_0xa35c.Dragonfly.BB_heavy_clip_ctrl)
#define QCN5500_PHY_HEAVYCLIP_CTL          AR_SM_OFFSET(overlay_0xa35c.Jet.BB_heavy_clip_ctrl)
#define AR_PHY_HEAVYCLIP_CTL_ENABLE_FOUR_TAP_HC_FILTER  	    0x1
#define AR_PHY_HEAVYCLIP_CTL_ENABLE_FOUR_TAP_HC_FILTER_S     	18
#define AR_PHY_HEAVYCLIP_CTL_HEAVY_CLIP_FACTOR_XR  	    		0xFF
#define AR_PHY_HEAVYCLIP_CTL_HEAVY_CLIP_FACTOR_XR_S     		10
#define AR_PHY_HEAVYCLIP_CTL_PRE_EMP_HT40_ENABLE  	    		0x1
#define AR_PHY_HEAVYCLIP_CTL_PRE_EMP_HT40_ENABLE_S     			9
#define AR_PHY_HEAVYCLIP_CTL_CF_HEAVY_CLIP_ENABLE  	    		0x1FF
#define AR_PHY_HEAVYCLIP_CTL_CF_HEAVY_CLIP_ENABLE_S     		0

#define AR_PHY_HEAVYCLIP_CTL_DF_ENABLE_FOUR_TAP_HC_FILTER  	    0x1
#define AR_PHY_HEAVYCLIP_CTL_DF_ENABLE_FOUR_TAP_HC_FILTER_S     28
#define AR_PHY_HEAVYCLIP_CTL_DF_HEAVY_CLIP_FACTOR_XR  	    	0xFF
#define AR_PHY_HEAVYCLIP_CTL_DF_HEAVY_CLIP_FACTOR_XR_S     		20
#define AR_PHY_HEAVYCLIP_CTL_DF_PRE_EMP_HT40_ENABLE  	    	0x1
#define AR_PHY_HEAVYCLIP_CTL_DF_PRE_EMP_HT40_ENABLE_S     		19
#define AR_PHY_HEAVYCLIP_CTL_DF_CF_HEAVY_CLIP_ENABLE    		0x7FFFF
#define AR_PHY_HEAVYCLIP_CTL_DF_CF_HEAVY_CLIP_ENABLE_S     		0
#define AR_PHY_HEAVYCLIP_20     AR_SM_OFFSET(overlay_0xa35c.Osprey.BB_heavy_clip_20)
#define AR_PHY_HEAVYCLIP_40     AR_SM_OFFSET(overlay_0xa35c.Osprey.BB_heavy_clip_40)
#define AR_PHY_HEAVYCLIP_1      AR_SM_OFFSET(overlay_0xa35c.Dragonfly.BB_heavy_clip_1)
#define AR_PHY_HEAVYCLIP_2      AR_SM_OFFSET(overlay_0xa35c.Dragonfly.BB_heavy_clip_2)
#define AR_PHY_HEAVYCLIP_3      AR_SM_OFFSET(overlay_0xa35c.Dragonfly.BB_heavy_clip_3)
#define AR_PHY_HEAVYCLIP_4      AR_SM_OFFSET(overlay_0xa35c.Dragonfly.BB_heavy_clip_4)
#define AR_PHY_HEAVYCLIP_5      AR_SM_OFFSET(overlay_0xa35c.Dragonfly.BB_heavy_clip_5)
#define AR_PHY_HEAVYCLIP_HEAVY_CLIP_FACTOR_3			  	0xFF
#define AR_PHY_HEAVYCLIP_HEAVY_CLIP_FACTOR_3_S		     	24
#define AR_PHY_HEAVYCLIP_HEAVY_CLIP_FACTOR_2			  	0xFF
#define AR_PHY_HEAVYCLIP_HEAVY_CLIP_FACTOR_2_S		     	16
#define AR_PHY_HEAVYCLIP_HEAVY_CLIP_FACTOR_1			  	0xFF
#define AR_PHY_HEAVYCLIP_HEAVY_CLIP_FACTOR_1_S		     	8
#define AR_PHY_HEAVYCLIP_HEAVY_CLIP_FACTOR_0			  	0xFF
#define AR_PHY_HEAVYCLIP_HEAVY_CLIP_FACTOR_0_S		     	0
#define AR_PHY_ILLEGAL_TXRATE    AR_SM_OFFSET(overlay_0xa3b0.Ospray.BB_illegal_tx_rate)
#define AR_PHY_ILLEGAL_TXRATE_HONEYBEE    AR_SM_OFFSET(overlay_0xa35c.Ospray.overlay_0xa384.Honybee1.BB_illegal_tx_rate)
#define AR_PHY_ILLEGAL_TXRATE_DRAGONFLY   AR_SM_OFFSET(overlay_0xa35c.Dragonfly.BB_illegal_tx_rate)
#define AR_PHY_HC_FIR_HT20_0     AR_SM_OFFSET(overlay_0xa3b0.Honeybee.BB_hc_fir_ht20_0)
#define QCN5500_PHY_HC_FIR_HT20_0         AR_SM_OFFSET(overlay_0xa3b0.Jet.BB_hc_fir_ht20_0)
#define QCN5500_PHY_HC_FIR_HT20_1         AR_SM_OFFSET(overlay_0xa3b0.Jet.BB_hc_fir_ht20_1)
#define QCN5500_PHY_HC_FIR_HT40_0         AR_SM_OFFSET(overlay_0xa3b0.Jet.BB_hc_fir_ht40_0)
#define QCN5500_PHY_HC_FIR_HT40_1         AR_SM_OFFSET(overlay_0xa3b0.Jet.BB_hc_fir_ht40_1)
#define AR_PHY_HC_FIR_HT20_0_HC_FIR_HT20_TAP_2	  	    		0x1FF
#define AR_PHY_HC_FIR_HT20_0_HC_FIR_HT20_TAP_2_S	    		18
#define AR_PHY_HC_FIR_HT20_0_HC_FIR_HT20_TAP_1	  	    		0x1FF
#define AR_PHY_HC_FIR_HT20_0_HC_FIR_HT20_TAP_1_S	    		9
#define AR_PHY_HC_FIR_HT20_0_HC_FIR_HT20_TAP_0	  	    		0x1FF
#define AR_PHY_HC_FIR_HT20_0_HC_FIR_HT20_TAP_0_S	    		0
#define AR_PHY_HC_FIR_HT20_1     AR_SM_OFFSET(overlay_0xa3b0.Honeybee.BB_hc_fir_ht20_1)
#define AR_PHY_HC_FIR_HT20_1_HC_FIR_HT20_TAP_3	  	    		0x1FF
#define AR_PHY_HC_FIR_HT20_1_HC_FIR_HT20_TAP_3_S	    		0
#define AR_PHY_HC_FIR_HT40_0     AR_SM_OFFSET(overlay_0xa3b0.Honeybee.BB_hc_fir_ht40_0)
#define AR_PHY_HC_FIR_HT40_0_HC_FIR_HT40_TAP_2	  	    		0x1FF
#define AR_PHY_HC_FIR_HT40_0_HC_FIR_HT40_TAP_2_S	    		18
#define AR_PHY_HC_FIR_HT40_0_HC_FIR_HT40_TAP_1	  	    		0x1FF
#define AR_PHY_HC_FIR_HT40_0_HC_FIR_HT40_TAP_1_S	    		9
#define AR_PHY_HC_FIR_HT40_0_HC_FIR_HT40_TAP_0	  	    		0x1FF
#define AR_PHY_HC_FIR_HT40_0_HC_FIR_HT40_TAP_0_S	    		0
#define AR_PHY_HC_FIR_HT40_1     AR_SM_OFFSET(overlay_0xa3b0.Honeybee.BB_hc_fir_ht40_1)
#define AR_PHY_HC_FIR_HT40_1_HC_FIR_HT40_TAP_3	  	    		0x1FF
#define AR_PHY_HC_FIR_HT40_1_HC_FIR_HT40_TAP_3_S	    		0

#define AR_PHY_POWER_TX_RATE(_i) AR_SM_OFFSET(BB_powertx_rate##_i)
#define AR_PHY_PWRTX_MAX         AR_SM_OFFSET(BB_powertx_max) /* TPC register */
#define AR_PHY_PWRTX_MAX_TPC_ENABLE 0x00000040
#define AR_PHY_POWER_TX_SUB      AR_SM_OFFSET(BB_powertx_sub)
#define AR_PHY_PER_PACKET_POWERTX_MAX   0x00000040
#define AR_PHY_PER_PACKET_POWERTX_MAX_S 6
#define AR_PHY_POWER_TX_SUB_2_DISABLE 0xFFFFFFC0    /* 2 chain */
#define AR_PHY_POWER_TX_SUB_3_DISABLE 0xFFFFF000    /* 3 chain */
#define QCN5500_PHY_POWER_TX_SUB_4_DISABLE 0xFFFC0000   /* 4 Chains */

#define AR_PHY_TPC(_i)           AR_SM_OFFSET(BB_tpc_##_i)    /* values 1-3, 7-10 and 12-15 */
#define AR_PHY_TPC_4_B0          AR_SM_OFFSET(BB_tpc_4_b0)
#define AR_PHY_TPC_5_B0          AR_SM_OFFSET(BB_tpc_5_b0)
#define AR_PHY_TPC_6_B0          AR_SM_OFFSET(BB_tpc_6_b0)
#define AR_PHY_TPC_18            AR_SM_OFFSET(BB_tpc_18)
#define AR_PHY_TPC_19            AR_SM_OFFSET(BB_tpc_19_b0)
#define AR_PHY_TX_FORCED_GAIN    AR_SM_OFFSET(BB_tx_forced_gain)
#define QCN5500_PHY_PWRTX_RATE13     AR_SM_OFFSET(BB_powertx_rate13)
#define QCN5500_PHY_PWRTX_RATE14     AR_SM_OFFSET(BB_powertx_rate14)

#define QCN5500_PHY_TPC_21           AR_SM_OFFSET(BB_tpc_21)
#define QCN5500_PHY_TPC_22           AR_SM_OFFSET(BB_tpc_22)
#define AR_PHY_PDADC_TAB_0       AR_SM_OFFSET(BB_pdadc_tab_b0)

#define AR_PHY_CALTX_GAIN_SET_0   AR_SM_OFFSET(overlay_0xa580.Osprey.BB_caltx_gain_set_0)
#define AR_PHY_CALTX_GAIN_SET_0_CALTX_GAIN_SET_0_GET(x)    (((x) & 0x00003fff) >> 0)
#define AR_PHY_CALTX_GAIN_SET_0_CALTX_GAIN_SET_0_SET(x)    (((x) << 0) & 0x00003fff)
#define AR_PHY_CALTX_GAIN_SET_0_CALTX_GAIN_SET_1_GET(x)    (((x) & 0x0fffc000) >> 14)
#define AR_PHY_CALTX_GAIN_SET_0_CALTX_GAIN_SET_1_SET(x)    (((x) << 14) & 0x0fffc000)

#define AR_PHY_TABLES_INTF_ADDR_B0   AR_SM_OFFSET(BB_tables_intf_addr_b0)
#define AR_PHY_TABLES_INTF_DATA_B0   AR_SM_OFFSET(BB_tables_intf_data_b0)
#define AR_PHY_TABLES_INTF_ADDR_B1  AR_SM1_OFFSET(overlay_0x4b0.Jet.BB_tables_intf_addr_b1)
#define AR_PHY_TABLES_INTF_DATA_B1   AR_SM1_OFFSET(overlay_0x4b0.Jet.BB_tables_intf_data_b1)
#define AR_PHY_TABLES_INTF_ADDR_B2  AR_SM2_OFFSET(BB_tables_intf_addr_b2)
#define AR_PHY_TABLES_INTF_DATA_B2   AR_SM2_OFFSET(BB_tables_intf_data_b2)
#define AR_PHY_TABLES_INTF_ADDR_B3   QCN5500_SM3_OFFSET(BB_tables_intf_addr_b3)
#define AR_PHY_TABLES_INTF_DATA_B3   QCN5500_SM3_OFFSET(BB_tables_intf_data_b3)

#define AR_PHY_RTT_CTRL                 AR_SM_OFFSET(overlay_0xa580.Jupiter_20.BB_rtt_ctrl)
#define AR_PHY_RTT_TABLE_SW_INTF_B0     AR_SM_OFFSET(overlay_0xa580.Jupiter_20.BB_rtt_table_sw_intf_b0)
#define AR_PHY_RTT_TABLE_SW_INTF_1_B0   AR_SM_OFFSET(overlay_0xa580.Jupiter_20.BB_rtt_table_sw_intf_1_b0)
#define AR_PHY_TXIQCAL_CONTROL_0     AR_SM_OFFSET(overlay_0xa580.Jet.BB_txiqcal_control_0)
#define AR_PHY_TX_IQCAL_CONTROL_0(_ah)                               \
        (AR_SREV_POSEIDON(_ah) ?                                         \
                 AR_SM_OFFSET(overlay_0xa580.Poseidon.BB_txiqcal_control_0) : \
                 AR_SM_OFFSET(overlay_0xa580.Osprey.BB_txiqcal_control_0))

#define AR_PHY_TX_IQCAL_CONTROL_1(_ah)                               \
        (AR_SREV_POSEIDON(_ah) ?                                         \
                 AR_SM_OFFSET(overlay_0xa580.Poseidon.BB_txiqcal_control_1) : \
                 AR_SM_OFFSET(overlay_0xa580.Osprey.BB_txiqcal_control_1))

#define AR_PHY_TX_IQCAL_START(_ah)                                   \
        (AR_SREV_POSEIDON(_ah) ?                                         \
                 AR_SM_OFFSET(overlay_0xa580.Poseidon.BB_txiqcal_control_0) : \
                 AR_SM_OFFSET(overlay_0xa580.Osprey.BB_txiqcal_start))

#define AR_PHY_TX_IQCAL_STATUS_B0(_ah)                               \
        (AR_SREV_POSEIDON(_ah) ?                                         \
                 AR_SM_OFFSET(overlay_0xa580.Poseidon.BB_txiqcal_status_b0) : \
                 AR_SM_OFFSET(overlay_0xa580.Osprey.BB_txiqcal_status_b0))

#define TXIQCAL_CONTROL_0__CALTXSHIFT_DELAY__SHIFT                           19
#define TXIQCAL_CONTROL_0__CALTXSHIFT_DELAY__WIDTH                            4
#define TXIQCAL_CONTROL_0__CALTXSHIFT_DELAY__MASK                   0x00780000U
#define AR_PHY_TX_IQCAL_CORR_COEFF_01_B0    AR_SM_OFFSET(overlay_0xa580.Osprey.BB_txiq_corr_coeff_01_b0)
#define AR_PHY_TX_IQCAL_CORR_COEFF_23_B0    AR_SM_OFFSET(overlay_0xa580.Osprey.BB_txiq_corr_coeff_23_b0)
#define AR_PHY_TX_IQCAL_CORR_COEFF_45_B0    AR_SM_OFFSET(overlay_0xa580.Osprey.BB_txiq_corr_coeff_45_b0)
#define AR_PHY_TX_IQCAL_CORR_COEFF_67_B0    AR_SM_OFFSET(overlay_0xa580.Osprey.BB_txiq_corr_coeff_67_b0)

#define AR_PHY_TX_IQCAL_CORR_COEFF_01_B0_POSEIDON    AR_SM_OFFSET(overlay_0xa580.Poseidon.BB_txiq_corr_coeff_01_b0)
#define AR_PHY_TX_IQCAL_CORR_COEFF_23_B0_POSEIDON    AR_SM_OFFSET(overlay_0xa580.Poseidon.BB_txiq_corr_coeff_23_b0)
#define AR_PHY_TX_IQCAL_CORR_COEFF_45_B0_POSEIDON    AR_SM_OFFSET(overlay_0xa580.Poseidon.BB_txiq_corr_coeff_45_b0)
#define AR_PHY_TX_IQCAL_CORR_COEFF_67_B0_POSEIDON    AR_SM_OFFSET(overlay_0xa580.Poseidon.BB_txiq_corr_coeff_67_b0)

#define AR_PHY_TXGAIN_TAB(_i)       AR_SM_OFFSET(BB_tx_gain_tab_##_i) /* values 1-22 */
#define AR_PHY_GREEN_TXGAIN_TAB(_i)       AR_EXT_SM_OFFSET(BB_green_tx_gain_tab_##_i) /* values 1-22 */
#define AR_PHY_TXGAIN_TAB_PAL(_i)   AR_SM_OFFSET(BB_tx_gain_tab_pal_##_i) /* values 1-22 */
#define AR_PHY_PANIC_WD_STATUS      AR_SM_OFFSET(BB_panic_watchdog_status)
#define AR_PHY_PANIC_WD_CTL_1       AR_SM_OFFSET(BB_panic_watchdog_ctrl_1)
#define AR_PHY_PANIC_WD_CTL_2       AR_SM_OFFSET(BB_panic_watchdog_ctrl_2)

#define AR_PHY_BT_CTL               AR_SM_OFFSET(BB_bluetooth_cntl)
#define AR_PHY_ONLY_WARMRESET       AR_SM_OFFSET(BB_phyonly_warm_reset)
#define AR_PHY_ONLY_CTL             AR_SM_OFFSET(BB_phyonly_control)
#define AR_PHY_ECO_CTRL             AR_SM_OFFSET(BB_eco_ctrl)
#define AR_PHY_BB_THERM_ADC_1       AR_SM_OFFSET(BB_therm_adc_1)
#define AR_PHY_BB_THERM_ADC_2       AR_SM_OFFSET(BB_therm_adc_2)
#define AR_PHY_BB_THERM_ADC_3       AR_SM_OFFSET(BB_therm_adc_3)
#define AR_PHY_BB_THERM_ADC_4       AR_SM_OFFSET(BB_therm_adc_4)

#define AR_PHY_65NM(_field)         offsetof(struct radio65_reg, _field)
#define AR_PHY_65NM_CH0_RXRF_GAINSTAGES                AR_PHY_65NM(ch0_RXRF_GAINSTAGES)
#define AR_PHY_65NM_CH1_RXRF_GAINSTAGES                AR_PHY_65NM(ch1_RXRF_GAINSTAGES)
#define AR_PHY_65NM_CH2_RXRF_GAINSTAGES                AR_PHY_65NM(ch2_RXRF_GAINSTAGES)
#define AR_PHY_65NM_CH3_RXRF_GAINSTAGES                AR_PHY_65NM(ch3_RXRF_GAINSTAGES)
#define AR_PHY_65NM_RXRF_GAINSTAGES_RX_OVERRIDE			0x80000000
#define AR_PHY_65NM_RXRF_GAINSTAGES_RX_OVERRIDE_S  		31
#define AR_PHY_65NM_RXRF_GAINSTAGES_LNAON_CALDC     	0x00000002
#define AR_PHY_65NM_RXRF_GAINSTAGES_LNAON_CALDC_S  		1
#define AR_PHY_65NM_CH0_RXRF_AGC                       AR_PHY_65NM(ch0_RXRF_AGC)
#define AR_PHY_65NM_CH1_RXRF_AGC                       AR_PHY_65NM(ch1_RXRF_AGC)
#define AR_PHY_65NM_CH2_RXRF_AGC                       AR_PHY_65NM(ch2_RXRF_AGC)
#define AR_PHY_65NM_CH3_RXRF_AGC                       AR_PHY_65NM(ch3_RXRF_AGC)
#define AR_PHY_65NM_CH0_RXRF_AGC_AGC_OVERRIDE          0x80000000
#define AR_PHY_65NM_CH0_RXRF_AGC_AGC_OVERRIDE_S                31
#define AR_PHY_65NM_CH0_RXRF_AGC_AGC_ON_OVR                    0x40000000
#define AR_PHY_65NM_CH0_RXRF_AGC_AGC_ON_OVR_S          30
#define AR_PHY_65NM_CH0_RXRF_AGC_AGC_CAL_OVR           0x20000000
#define AR_PHY_65NM_CH0_RXRF_AGC_AGC_CAL_OVR_S         29
#define AR_PHY_65NM_CH0_RXRF_AGC_AGC2G_DBDAC_OVR	    0x1E000000
#define AR_PHY_65NM_CH0_RXRF_AGC_AGC2G_DBDAC_OVR_S     	25
#define AR_PHY_65NM_CH0_RXRF_AGC_AGC5G_DBDAC_OVR            0x00078000
#define AR_PHY_65NM_CH0_RXRF_AGC_AGC5G_DBDAC_OVR_S      15
#define AR_PHY_65NM_CH0_RXRF_AGC_AGC2G_CALDAC_OVR              0x01F80000
#define AR_PHY_65NM_CH0_RXRF_AGC_AGC2G_CALDAC_OVR_S            19
#define AR_PHY_65NM_CH0_RXRF_AGC_AGC5G_CALDAC_OVR              0x00007e00
#define AR_PHY_65NM_CH0_RXRF_AGC_AGC5G_CALDAC_OVR_S            9
#define AR_PHY_65NM_CH0_RXRF_AGC_AGC_OUT       0x00000004
#define AR_PHY_65NM_CH0_RXRF_AGC_AGC_OUT_S     2
#define AR_PHY_65NM_CH0_RXRF_BIAS1             AR_PHY_65NM(ch0_RXRF_BIAS1)
#define AR_PHY_65NM_CH0_RXRF_BIAS1_PWD_IC25MXR2GH              0x0E000000
#define AR_PHY_65NM_CH0_RXRF_BIAS1_PWD_IC25MXR2GH_S            25
#define AR_PHY_65NM_CH0_RXRF_BIAS2             AR_PHY_65NM(ch0_RXRF_BIAS2)
#define AR_PHY_65NM_CH0_TXRF1       AR_PHY_65NM(ch0_TXRF1)
#define AR_PHY_65NM_CH0_TXRF1_OBDBOFFST2G_AUX1   0x7
#define AR_PHY_65NM_CH0_TXRF1_OBDBOFFST2G_AUX1_S 5
#define AR_PHY_65NM_CH0_TXRF1_OBDBOFFST2G_AUX2   0x7
#define AR_PHY_65NM_CH0_TXRF1_OBDBOFFST2G_AUX2_S 2
#define AR_PHY_65NM_CH0_TXRF2       AR_PHY_65NM(ch0_TXRF2)
#define AR_PHY_65NM_CH0_TXRF2_SHSHOBDB2G        0x1
#define AR_PHY_65NM_CH0_TXRF2_SHSHOBDB2G_S      31
#define AR_PHY_65NM_CH0_TXRF2_DB2G              0x07000000
#define AR_PHY_65NM_CH0_TXRF2_DB2G_S            24
#define AR_PHY_65NM_CH0_TXRF2_OB2G_CCK          0x00E00000
#define AR_PHY_65NM_CH0_TXRF2_OB2G_CCK_S        21
#define AR_PHY_65NM_CH0_TXRF2_OB2G_PSK          0x001C0000
#define AR_PHY_65NM_CH0_TXRF2_OB2G_PSK_S        18
#define AR_PHY_65NM_CH0_TXRF2_OB2G_QAM          0x00038000
#define AR_PHY_65NM_CH0_TXRF2_OB2G_QAM_S        15
#define AR_PHY_65NM_CH0_TXRF2_OCAS2G            0x7
#define AR_PHY_65NM_CH0_TXRF2_OCAS2G_S           6
#define AR_PHY_65NM_CH0_TXRF2_OBDBOFFST2G_AUX1   0x7
#define AR_PHY_65NM_CH0_TXRF2_OBDBOFFST2G_AUX1_S 3
#define AR_PHY_65NM_CH0_TXRF2_OBDBOFFST2G_AUX2   0x7
#define AR_PHY_65NM_CH0_TXRF2_OBDBOFFST2G_AUX2_S 0
#define AR_PHY_65NM_CH0_TXRF3       AR_PHY_65NM(ch0_TXRF3)
#define AR_PHY_65NM_CH0_TXRF3_PDPREDIST2G        0x1
#define AR_PHY_65NM_CH0_TXRF3_PDPREDIST2G_S      13     
#define AR_PHY_65NM_CH0_TXRF3_RDIV2G             0x3
#define AR_PHY_65NM_CH0_TXRF3_RDIV2G_S           11
#define AR_PHY_65NM_TXRF3_D2B5G          0xE0000000
#define AR_PHY_65NM_TXRF3_D2B5G_S        29
#define AR_PHY_65NM_TXRF3_OB5G_CCK_JUPITER          0x1C000000
#define AR_PHY_65NM_TXRF3_OB5G_CCK_JUPITER_S        26
#define AR_PHY_65NM_TXRF3_OB5G_PSK_JUPITER          0x03800000
#define AR_PHY_65NM_TXRF3_OB5G_PSK_JUPITER_S        23
#define AR_PHY_65NM_TXRF3_OB5G_QAM_JUPITER          0x00700000
#define AR_PHY_65NM_TXRF3_OB5G_QAM_JUPITER_S        20
#define AR_PHY_65NM_TXRF3_D4CAS5G_JUPITER          0x000E0000
#define AR_PHY_65NM_TXRF3_D4CAS5G_JUPITER_S        17
#define AR_PHY_65NM_TXRF3_D3CAS5G_JUPITER          0x0001C000
#define AR_PHY_65NM_TXRF3_D3CAS5G_JUPITER_S        14
#define AR_PHY_65NM_TXRF3_D2CAS5G_JUPITER          0x00003800
#define AR_PHY_65NM_TXRF3_D2CAS5G_JUPITER_S        11
#define AR_PHY_65NM_CH0_TXRF3_CAPDIV2G          0x0000001E
#define AR_PHY_65NM_CH0_TXRF3_CAPDIV2G_S        1
#define AR_PHY_65NM_CH0_TXRF3_OLD_PAL_SPARE     0x00000001
#define AR_PHY_65NM_CH0_TXRF3_OLD_PAL_SPARE_S   0
//TXRF3 for poseidon
#define AR_PHY_65NM_TXRF3_D4CAS5G_POSEIDON          0x03800000
#define AR_PHY_65NM_TXRF3_D4CAS5G_POSEIDON_S        23
#define AR_PHY_65NM_TXRF3_D3CAS5G_POSEIDON          0x00700000
#define AR_PHY_65NM_TXRF3_D3CAS5G_POSEIDON_S        20
#define AR_PHY_65NM_TXRF3_D2CAS5G_POSEIDON          0x000E0000
#define AR_PHY_65NM_TXRF3_D2CAS5G_POSEIDON_S        17
#define AR_PHY_65NM_TXRF3_PDPREDIST2G_POSEIDON     0x00002000
#define AR_PHY_65NM_TXRF3_PDPREDIST2G_POSEIDON_S   13
#define AR_PHY_65NM_TXRF3_RDIV2G_POSEIDON     0x00001800
#define AR_PHY_65NM_TXRF3_RDIV2G_POSEIDON_S   11

#define AR_PHY_65NM_CH0_TXRF4       AR_PHY_65NM(ch0_TXRF4)
#define AR_PHY_65NM_CH1_TXRF4       AR_PHY_65NM(ch1_TXRF4)
#define AR_PHY_65NM_TXRF4_PDPREDIST2G_JUPITER     0x80000000
#define AR_PHY_65NM_TXRF4_PDPREDIST2G_JUPITER_S   31
#define AR_PHY_65NM_TXRF4_CAPDIV2G_PREDIST_JUPITER     0x78000000
#define AR_PHY_65NM_TXRF4_CAPDIV2G_PREDIST_JUPITER_S   27
#define AR_PHY_65NM_TXRF4_RDIV2G_JUPITER     0x06000000
#define AR_PHY_65NM_TXRF4_RDIV2G_JUPITER_S   25
#define AR_PHY_65NM_CH0_TXRF4_PDPREDIST2G        0x1
#define AR_PHY_65NM_CH0_TXRF4_PDPREDIST2G_S      19     
#define AR_PHY_65NM_CH0_TXRF4_RDIV2G             0x3
#define AR_PHY_65NM_CH0_TXRF4_RDIV2G_S           17
#define AR_PHY_65NM_CH0_TXRF4_CAPDIV2G          0x000000780
#define AR_PHY_65NM_CH0_TXRF4_CAPDIV2G_S        7

#define AR_PHY_65NM_CH0_TXRF5       AR_PHY_65NM(ch0_TXRF5)
#define AR_PHY_65NM_CH0_TXRF6       AR_PHY_65NM(ch0_TXRF6)
#define AR_PHY_65NM_CH1_TXRF6       AR_PHY_65NM(ch1_TXRF6)
#define AR_PHY_65NM_TXRF6_SHSHOBDB2G     0x00000100
#define AR_PHY_65NM_TXRF6_SHSHOBDB2G_S   8
#define AR_PHY_65NM_TXRF6_SHSHOBDB5G     0x00000080
#define AR_PHY_65NM_TXRF6_SHSHOBDB5G_S   7

#define AR_PHY_65NM_CH1_TXRF1       AR_PHY_65NM(ch1_TXRF1)
#define AR_PHY_65NM_CH1_TXRF1_OBDBOFFST2G_AUX1   0x7
#define AR_PHY_65NM_CH1_TXRF1_OBDBOFFST2G_AUX1_S 5
#define AR_PHY_65NM_CH1_TXRF1_OBDBOFFST2G_AUX2   0x7
#define AR_PHY_65NM_CH1_TXRF1_OBDBOFFST2G_AUX2_S 2
#define AR_PHY_65NM_CH1_TXRF2       AR_PHY_65NM(ch1_TXRF2)
#define AR_PHY_65NM_CH1_TXRF2_SHSHOBDB2G         0x1
#define AR_PHY_65NM_CH1_TXRF2_SHSHOBDB2G_S       31
#define AR_PHY_65NM_CH1_TXRF2_OBDBOFFST2G_AUX1   0x7
#define AR_PHY_65NM_CH1_TXRF2_OBDBOFFST2G_AUX1_S 3
#define AR_PHY_65NM_CH1_TXRF2_OBDBOFFST2G_AUX2   0x7
#define AR_PHY_65NM_CH1_TXRF2_OBDBOFFST2G_AUX2_S 0
#define AR_PHY_65NM_CH1_TXRF3       AR_PHY_65NM(ch1_TXRF3)
#define AR_PHY_65NM_CH1_TXRF3_PDPREDIST2G        0x1
#define AR_PHY_65NM_CH1_TXRF3_PDPREDIST2G_S      13     
#define AR_PHY_65NM_CH1_TXRF3_RDIV2G             0x3
#define AR_PHY_65NM_CH1_TXRF3_RDIV2G_S           11
#define AR_PHY_65NM_CH1_TXRF3_CAPDIV2G          0xF
#define AR_PHY_65NM_CH1_TXRF3_CAPDIV2G_S        1
#define AR_PHY_65NM_CH1_TXRF4       AR_PHY_65NM(ch1_TXRF4)
#define AR_PHY_65NM_CH1_TXRF5       AR_PHY_65NM(ch1_TXRF5)
#define AR_PHY_65NM_CH1_TXRF6       AR_PHY_65NM(ch1_TXRF6)
#define AR_PHY_65NM_CH1_TXRF4_PDPREDIST2G        0x1
#define AR_PHY_65NM_CH1_TXRF4_PDPREDIST2G_S      19     
#define AR_PHY_65NM_CH1_TXRF4_RDIV2G             0x3
#define AR_PHY_65NM_CH1_TXRF4_RDIV2G_S           17
#define AR_PHY_65NM_CH1_TXRF4_CAPDIV2G          0x000000780
#define AR_PHY_65NM_CH1_TXRF4_CAPDIV2G_S        7
#define AR_PHY_65NM_CH2_TXRF1       AR_PHY_65NM(ch2_TXRF1)
#define AR_PHY_65NM_CH2_TXRF2       AR_PHY_65NM(ch2_TXRF2)
#define AR_PHY_65NM_CH2_TXRF2_SHSHOBDB2G         0x1
#define AR_PHY_65NM_CH2_TXRF2_SHSHOBDB2G_S       31
#define AR_PHY_65NM_CH2_TXRF2_OBDBOFFST2G_AUX1   0x7
#define AR_PHY_65NM_CH2_TXRF2_OBDBOFFST2G_AUX1_S 3
#define AR_PHY_65NM_CH2_TXRF2_OBDBOFFST2G_AUX2   0x7
#define AR_PHY_65NM_CH2_TXRF2_OBDBOFFST2G_AUX2_S 0
#define AR_PHY_65NM_CH2_TXRF3       AR_PHY_65NM(ch2_TXRF3)
#define AR_PHY_65NM_CH2_TXRF4       AR_PHY_65NM(ch2_TXRF4)
#define AR_PHY_65NM_CH2_TXRF5       AR_PHY_65NM(ch2_TXRF5)
#define AR_PHY_65NM_CH2_TXRF6       AR_PHY_65NM(ch2_TXRF6)
#define AR_PHY_65NM_CH2_TXRF3_PDPREDIST2G        0x1
#define AR_PHY_65NM_CH2_TXRF3_PDPREDIST2G_S      13     
#define AR_PHY_65NM_CH2_TXRF3_RDIV2G             0x3
#define AR_PHY_65NM_CH2_TXRF3_RDIV2G_S           11
#define AR_PHY_65NM_CH2_TXRF3_CAPDIV2G          0xF
#define AR_PHY_65NM_CH2_TXRF3_CAPDIV2G_S        1
#define AR_PHY_65NM_CH3_TXRF1       AR_PHY_65NM(ch3_TXRF1)
#define AR_PHY_65NM_CH3_TXRF2       AR_PHY_65NM(ch3_TXRF2)
#define AR_PHY_65NM_CH3_TXRF3       AR_PHY_65NM(ch3_TXRF3)
#define AR_PHY_65NM_CH3_TXRF4       AR_PHY_65NM(ch3_TXRF4)
#define AR_PHY_65NM_CH3_TXRF5       AR_PHY_65NM(ch3_TXRF5)
#define AR_PHY_65NM_CH3_TXRF6       AR_PHY_65NM(ch3_TXRF6)
#define AR_PHY_65NM_CH3_TXRF3_CAPDIV2G          0xF
#define AR_PHY_65NM_CH3_TXRF3_CAPDIV2G_S        1
#define AR_PHY_65NM_CH3_TXRF2_SHSHOBDB2G         0x1
#define AR_PHY_65NM_CH3_TXRF2_SHSHOBDB2G_S       31
#define AR_PHY_65NM_CH3_TXRF2_OBDBOFFST2G_AUX1   0x7
#define AR_PHY_65NM_CH3_TXRF2_OBDBOFFST2G_AUX1_S 3
#define AR_PHY_65NM_CH3_TXRF2_OBDBOFFST2G_AUX2   0x7
#define AR_PHY_65NM_CH3_TXRF2_OBDBOFFST2G_AUX2_S 0
#define AR_PHY_65NM_CH3_TXRF3_RDIV2G             0x3
#define AR_PHY_65NM_CH3_TXRF3_RDIV2G_S           11
#define AR_PHY_65NM_CH3_TXRF3_PDPREDIST2G        0x1
#define AR_PHY_65NM_CH3_TXRF3_PDPREDIST2G_S      13
#define AR_PHY_65NM_CH0_SYNTH4      AR_PHY_65NM(ch0_SYNTH4)
#define AR_PHY_SYNTH4_LONG_SHIFT_SELECT   0x00000002
#define AR_PHY_SYNTH4_LONG_SHIFT_SELECT_S 1
#define AR_PHY_65NM_CH0_SYNTH7      AR_PHY_65NM(ch0_SYNTH7)
#define AR_PHY_SYNTH7      AR_PHY_65NM(ch0_SYNTH7)
#define AR_PHY_SYNTH8      AR_PHY_65NM(ch0_SYNTH8)
#define AR_PHY_SYNTH9      AR_PHY_65NM(ch0_SYNTH9)
#define AR_PHY_SYNTH10      AR_PHY_65NM(ch0_SYNTH10)
#define AR_PHY_SYNTH12      AR_PHY_65NM(ch0_SYNTH12)
#define AR_PHY_SYNTH13      AR_PHY_65NM(ch0_SYNTH13)
#define AR_PHY_65NM_CH0_BIAS1       AR_PHY_65NM(ch0_BIAS1)
#define AR_PHY_65NM_CH0_BIAS2       AR_PHY_65NM(ch0_BIAS2)
#define AR_PHY_65NM_CH0_BIAS4       AR_PHY_65NM(ch0_BIAS4)

#define AR_PHY_65NM_CH0_RXTX4       AR_PHY_65NM(ch0_RXTX4)
#define AR_PHY_65NM_CH0_SYNTH12      AR_PHY_65NM(ch0_SYNTH12)
#define AR_PHY_SYNTH12      AR_PHY_65NM(ch0_SYNTH12)
#define AR_PHY_65NM_CH0_SYNTH12_VREFMUL3          0x00780000
#define AR_PHY_65NM_CH0_SYNTH12_VREFMUL3_S        19
#define AR_PHY_65NM_CH1_RXTX4       AR_PHY_65NM(ch1_RXTX4)
#define AR_PHY_65NM_CH2_RXTX4       AR_PHY_65NM(ch2_RXTX4)
#define AR_PHY_65NM_CH3_RXTX4       AR_PHY_65NM(ch3_RXTX4)
#define AR_PHY_65NM_CH1_RXTX5       AR_PHY_65NM(ch1_RXTX5)
#define AR_PHY_65NM_RXTX4_AIC_GMGN_6DB          0x0000000f
#define AR_PHY_65NM_RXTX4_AIC_GMGN_6DB_S        0
#define AR_PHY_65NM_RXTX4_AIC_INV_Q             0x00000010
#define AR_PHY_65NM_RXTX4_AIC_INV_Q_S           4
#define AR_PHY_65NM_RXTX4_AIC_INV_I             0x00000020
#define AR_PHY_65NM_RXTX4_AIC_INV_I_S           5
#define AR_PHY_65NM_RXTX4_AIC_VGA_GN_Q          0x000007c0
#define AR_PHY_65NM_RXTX4_AIC_VGA_GN_Q_S        6
#define AR_PHY_65NM_RXTX4_AIC_VGA_GN_I          0x0000f800
#define AR_PHY_65NM_RXTX4_AIC_VGA_GN_I_S        11
#define AR_PHY_65NM_RXTX4_AIC_CAPDIV_2GQ        0x000f0000
#define AR_PHY_65NM_RXTX4_AIC_CAPDIV_2GQ_S      16
#define AR_PHY_65NM_RXTX4_AIC_CAPDIV_2GI        0x00f00000
#define AR_PHY_65NM_RXTX4_AIC_CAPDIV_2GI_S      20
#define AR_PHY_65NM_RXTX4_XLNA_BIAS   0xC0000000
#define AR_PHY_65NM_RXTX4_XLNA_BIAS_S 30
#define AR_PHY_65NM_RXTX5_AIC_LOAD_CHANNEL      0x02000000
#define AR_PHY_65NM_RXTX5_AIC_LOAD_CHANNEL_S    25
#define AR_PHY_65NM_RXTX5_AIC_CHAN_OVR          0x0c000000
#define AR_PHY_65NM_RXTX5_AIC_CHAN_OVR_S        26
#define AR_PHY_65NM_RXTX5_AIC_AIC_STBY          0x10000000
#define AR_PHY_65NM_RXTX5_AIC_AIC_STBY_S        28
#define AR_PHY_65NM_RXTX5_AIC_ON                0x20000000
#define AR_PHY_65NM_RXTX5_AIC_ON_S              29
#define AR_PHY_65NM_RXTX5_AIC_LOCAL_MODE        0x40000000
#define AR_PHY_65NM_RXTX5_AIC_LOCAL_MODE_S      30
#define AR_PHY_65NM_RXTX5_AIC_GM_HP             0x80000000
#define AR_PHY_65NM_RXTX5_AIC_GM_HP_S           31

#define AR_PHY_65NM_CH0_TOP         AR_PHY_65NM(overlay_0x16180.Osprey.ch0_TOP)
#define AR_PHY_65NM_CH0_TOP_JUPITER AR_PHY_65NM(overlay_0x16180.Jupiter.ch0_TOP1)
#define AR_PHY_65NM_CH0_TOP_XPABIASLVL         0x00000300
#define AR_PHY_65NM_CH0_TOP_XPABIASLVL_S       8
#define AR_PHY_65NM_CH0_TOP2_JUPITER AR_PHY_65NM(overlay_0x16180.Jupiter.ch0_TOP2)
#define AR_PHY_65NM_CH0_TOP2_APHRODITE AR_PHY_65NM(overlay_0x16180.Aphrodite.ch0_TOP2)
//ch0_TOP2 for Jupiter
#define AR_PHY_65NM_CH0_TOP2_LOCAL_XPAON_JUPITER         0x00008000
#define AR_PHY_65NM_CH0_TOP2_LOCAL_XPAON_JUPITER_S       15
#define AR_PHY_65NM_CH0_TOP2_XPA2ON_JUPITER         0x000c0000
#define AR_PHY_65NM_CH0_TOP2_XPA2ON_JUPITER_S       18
#define AR_PHY_65NM_CH0_TOP2        AR_PHY_65NM(overlay_0x16180.Osprey.ch0_TOP2)
#define AR_PHY_65NM_CH0_TOP3        AR_PHY_65NM(ch0_TOP3)

#define AR_OSPREY_CH0_XTAL              AR_PHY_65NM(overlay_0x16180.Osprey.ch0_XTAL)
#define AR_OSPREY_CHO_XTAL_CAPINDAC     0x7F000000
#define AR_OSPREY_CHO_XTAL_CAPINDAC_S   24
#define AR_OSPREY_CHO_XTAL_CAPOUTDAC    0x00FE0000
#define AR_OSPREY_CHO_XTAL_CAPOUTDAC_S  17

/* macros for field xtal_capoutdac */
#define XTAL__XTAL_CAPOUTDAC__SHIFT     17
#define XTAL__XTAL_CAPOUTDAC__WIDTH     7
#define XTAL__XTAL_CAPOUTDAC__MASK      0x00fe0000U
/* macros for field xtal_capindac */
#define XTAL__XTAL_CAPINDAC__SHIFT      24
#define XTAL__XTAL_CAPINDAC__WIDTH      7
#define XTAL__XTAL_CAPINDAC__MASK       0x7f000000U

#define AR_PHY_65NM_CH0_THERM       AR_PHY_65NM(overlay_0x16180.Osprey.ch0_THERM)
#define AR_PHY_65NM_CH0_THERM_JUPITER AR_PHY_65NM(overlay_0x16180.Jupiter.ch0_THERM)
#define AR_PHY_65NM_CH0_THERM_APHRODITE AR_PHY_65NM(overlay_0x16180.Aphrodite.ch0_THERM)
#define QCN5500_CH0_THERM           AR_PHY_65NM(overlay_0x16180.Jet.ch0_THERM)

#define AR_PHY_65NM_CH0_THERM_XPABIASLVL_MSB   0x00000003
#define AR_PHY_65NM_CH0_THERM_XPABIASLVL_MSB_S 0
#define AR_PHY_65NM_CH0_THERM_XPASHORT2GND     0x00000004
#define AR_PHY_65NM_CH0_THERM_XPASHORT2GND_S   2
#define AR_PHY_65NM_CH0_THERM_FLIP_ADC_STROBE_CLK     0x00000018
#define AR_PHY_65NM_CH0_THERM_FLIP_ADC_STROBE_CLK_S   3
#define AR_PHY_65NM_CH0_THERM_SAR_ADC_DONE      0x00000080
#define AR_PHY_65NM_CH0_THERM_SAR_ADC_DONE_S    7
#define AR_PHY_65NM_CH0_THERM_SAR_ADC_OUT      0x0000ff00
#define AR_PHY_65NM_CH0_THERM_SAR_ADC_OUT_S    8
#define AR_PHY_65NM_CH0_THERM_SAR_ADCCAL_EN            0x02000000
#define AR_PHY_65NM_CH0_THERM_SAR_ADCCAL_EN_S          25 
#define AR_PHY_65NM_CH0_THERM_SEL            0x0c000000
#define AR_PHY_65NM_CH0_THERM_SEL_S          26 
#define AR_PHY_65NM_CH0_THERM_SAR_SLOW_EN            0x10000000
#define AR_PHY_65NM_CH0_THERM_SAR_SLOW_EN_S          28 
#define AR_PHY_65NM_CH0_THERM_START            0x20000000
#define AR_PHY_65NM_CH0_THERM_START_S          29 
#define AR_PHY_65NM_CH0_THERM_SAR_AUTOPWD_EN            0x40000000
#define AR_PHY_65NM_CH0_THERM_SAR_AUTOPWD_EN_S          30
#define AR_PHY_65NM_CH0_THERM_LOCAL            0x80000000
#define AR_PHY_65NM_CH0_THERM_LOCAL_S          31

#define AR_PHY_65NM_CH0_RXTX1       AR_PHY_65NM(ch0_RXTX1)
#define AR_PHY_65NM_CH0_RXTX2       AR_PHY_65NM(ch0_RXTX2)
#define AR_PHY_65NM_CH0_RXTX2_RXON_OVR       0x00001000
#define AR_PHY_65NM_CH0_RXTX2_RXON_OVR_S     12
#define AR_PHY_65NM_CH0_RXTX2_RXON           0x00000800
#define AR_PHY_65NM_CH0_RXTX2_RXON_S         11
#define AR_PHY_65NM_CH0_RXTX2_SYNTHON_MASK         0x00000004
#define AR_PHY_65NM_CH0_RXTX2_SYNTHON_MASK_S       2
#define AR_PHY_65NM_CH0_RXTX2_SYNTHOVR_MASK        0x00000008
#define AR_PHY_65NM_CH0_RXTX2_SYNTHOVR_MASK_S      3
#define AR_PHY_65NM_CH1_RXTX2_RXON_OVR       0x00001000
#define AR_PHY_65NM_CH1_RXTX2_RXON_OVR_S     12
#define AR_PHY_65NM_CH1_RXTX2_RXON           0x00000800
#define AR_PHY_65NM_CH1_RXTX2_RXON_S         11
#define AR_PHY_65NM_CH1_RXTX2_SYNTHON        0x00000004
#define AR_PHY_65NM_CH1_RXTX2_SYNTHON_S      2
#define AR_PHY_65NM_CH1_RXTX2_SYNTHOVR       0x00000008
#define AR_PHY_65NM_CH1_RXTX2_SYNTHOVR_S     3
#define AR_PHY_65NM_CH2_RXTX2_RXON_OVR       0x00001000
#define AR_PHY_65NM_CH2_RXTX2_RXON_OVR_S     12
#define AR_PHY_65NM_CH2_RXTX2_RXON           0x00000800
#define AR_PHY_65NM_CH2_RXTX2_RXON_S         11
#define AR_PHY_65NM_CH2_RXTX2_SYNTHON        0x00000004
#define AR_PHY_65NM_CH2_RXTX2_SYNTHON_S      2
#define AR_PHY_65NM_CH2_RXTX2_SYNTHOVR       0x00000008
#define AR_PHY_65NM_CH2_RXTX2_SYNTHOVR_S     3
#define AR_PHY_65NM_CH3_RXTX2_RXON_OVR       0x00001000
#define AR_PHY_65NM_CH3_RXTX2_RXON_OVR_S     12
#define AR_PHY_65NM_CH3_RXTX2_RXON           0x00000800
#define AR_PHY_65NM_CH3_RXTX2_RXON_S         11
#define AR_PHY_65NM_CH3_RXTX2_SYNTHON        0x00000004
#define AR_PHY_65NM_CH3_RXTX2_SYNTHON_S      2
#define AR_PHY_65NM_CH3_RXTX2_SYNTHOVR       0x00000008
#define AR_PHY_65NM_CH3_RXTX2_SYNTHOVR_S     3

#define AR_PHY_65NM_CH0_RXTX3       AR_PHY_65NM(ch0_RXTX3)
#define AR_PHY_65NM_CH1_RXTX1       AR_PHY_65NM(ch1_RXTX1)
#define AR_PHY_65NM_CH1_RXTX2       AR_PHY_65NM(ch1_RXTX2)
#define AR_PHY_65NM_CH1_RXTX3       AR_PHY_65NM(ch1_RXTX3)
#define AR_PHY_65NM_CH2_RXTX1       AR_PHY_65NM(ch2_RXTX1)
#define AR_PHY_65NM_CH2_RXTX2       AR_PHY_65NM(ch2_RXTX2)
#define AR_PHY_65NM_CH2_RXTX3       AR_PHY_65NM(ch2_RXTX3)
#define AR_PHY_65NM_CH3_RXTX1       AR_PHY_65NM(ch3_RXTX1)
#define AR_PHY_65NM_CH3_RXTX2       AR_PHY_65NM(ch3_RXTX2)
#define AR_PHY_65NM_CH3_RXTX3       AR_PHY_65NM(ch3_RXTX3)

#define MAC_PCU_TST_ADDAC__TESTMODE__SHIFT    1
#define MAC_PCU_TST_ADDAC__TESTMODE__MASK     0x00000002U
/* LO Force ON bit */
#if ATH_TxBF_DYNAMIC_LOF_ON_N_CHAIN_MASK
#define AR_PHY_65NM_CHAIN_RXTX3_LOFORCE_MASK        0x00080000
#define AR_PHY_65NM_CHAIN_RXTX3_LOFORCE_MASK_S      19
#endif
#define AR_PHY_65NM_RXTX3_CALTX_OVR     0x02000000
#define AR_PHY_65NM_RXTX3_CALTX_OVR_S   25
#define AR_PHY_65NM_RXTX3_CALTX         0x01000000
#define AR_PHY_65NM_RXTX3_CALTX_S       24
#define AR_PHY_65NM_CH0_RXRFSA_AGC       AR_PHY_65NM(ch0_RXRF_AGC)
#define AR_PHY_65NM_CH1_RXRFJU_AGC       AR_PHY_65NM(ch1_RXRF_AGC)
#define AR_PHY_65NM_RXRF_AGC_AGC_OVERRIDE   0x80000000
#define AR_PHY_65NM_RXRF_AGC_AGC_OVERRIDE_S 31
#define AR_PHY_65NM_RXRF_AGC_AGC_ON_OVR   0x40000000
#define AR_PHY_65NM_RXRF_AGC_AGC_ON_OVR_S 30
#define AR_PHY_65NM_RXRF_AGC_AGC_CAL_OVR   0x20000000
#define AR_PHY_65NM_RXRF_AGC_AGC_CAL_OVR_S 29
#define AR_PHY_65NM_RXRF_AGC_AGC2G_DBDAC_OVR   0x1E000000
#define AR_PHY_65NM_RXRF_AGC_AGC2G_DBDAC_OVR_S 25
#define AR_PHY_65NM_RXRF_AGC_AGC5G_DBDAC_OVR   0x00078000
#define AR_PHY_65NM_RXRF_AGC_AGC5G_DBDAC_OVR_S 15
#define AR_PHY_65NM_RXRF_AGC_AGC2G_CALDAC_OVR   0x01F80000
#define AR_PHY_65NM_RXRF_AGC_AGC2G_CALDAC_OVR_S 19
#define AR_PHY_65NM_RXRF_AGC_AGC5G_CALDAC_OVR   0x00007e00
#define AR_PHY_65NM_RXRF_AGC_AGC5G_CALDAC_OVR_S 9
#define AR_PHY_65NM_RXRF_AGC_AGC_OUT   0x00000004
#define AR_PHY_65NM_RXRF_AGC_AGC_OUT_S 2
#define AR_PHY_65NM_RXRF_AGC_RF2G_ON_DURING_CALPA   0x00000002
#define AR_PHY_65NM_RXRF_AGC_RF2G_ON_DURING_CALPA_S 1

#define AR_PHY_65NM_CH0_RXRFSA_BIAS1       AR_PHY_65NM(ch0_RXRF_BIAS1)
#define AR_PHY_65NM_CH0_RXRFSA_BIAS2       AR_PHY_65NM(ch0_RXRF_BIAS2)
#define AR_PHY_65NM_CH1_RXRFJU_BIAS1       AR_PHY_65NM(ch1_RXRF_BIAS1)
#define AR_PHY_65NM_CH1_RXRFJU_BIAS2       AR_PHY_65NM(ch1_RXRF_BIAS2)
#define AR_PHY_65NM_RXRF_BIAS1_PWD_IC25MXR2GH   0x0e000000
#define AR_PHY_65NM_RXRF_BIAS1_PWD_IC25MXR2GH_S 25
#define AR_PHY_65NM_RXRF_BIAS1_PWD_IC25MXR5GH   0x0000e000
#define AR_PHY_65NM_RXRF_BIAS1_PWD_IC25MXR5GH_S 13
#define AR_PHY_65NM_RXRF_BIAS2_VCMVALUE   0x00000070
#define AR_PHY_65NM_RXRF_BIAS2_VCMVALUE_S 4

#define AR_PHY_65NM_CH0_PLLCLKMODA3       AR_PHY_65NM(overlay_0x16180.Jupiter.ch0_PLLCLKMODA3)
#define AR_PHY_65NM_CH0_PLLCLKMODA3_APHRODITE       AR_PHY_65NM(overlay_0x16180.Aphrodite.ch0_PLLCLKMODA3)
#define AR_PHY_65NM_CH0_PLLCLKMODA3_LOCAL_ADCPWD  0x00020000
#define AR_PHY_65NM_CH0_PLLCLKMODA3_LOCAL_ADCPWD_S 17
#define AR_PHY_65NM_CH0_PLLCLKMODA3_ADCPWD  0x00018000
#define AR_PHY_65NM_CH0_PLLCLKMODA3_ADCPWD_S 15
#define AR_PHY_65NM_CH0_PLLCLKMODA3_LOCAL_DACPWD  0x00001000
#define AR_PHY_65NM_CH0_PLLCLKMODA3_LOCAL_DACPWD_S 12
#define AR_PHY_65NM_CH0_PLLCLKMODA3_DACPWD  0x00000c00
#define AR_PHY_65NM_CH0_PLLCLKMODA3_DACPWD_S 10

#define AR_PHY_65NM_CH0_RBIST_CNTRL       AR_PHY_65NM(ch0_rbist_cntrl)
#define AR_PHY_65NM_CH1_RBIST_CNTRL       AR_PHY_65NM(ch1_rbist_cntrl)
#define AR_PHY_65NM_RBIST_CNTRL_ATE_RBIST_ENABLE   0x00010000
#define AR_PHY_65NM_RBIST_CNTRL_ATE_RBIST_ENABLE_S 16
#define AR_PHY_65NM_RBIST_CNTRL_ATE_RXDAC_CALIBRATE   0x00008000
#define AR_PHY_65NM_RBIST_CNTRL_ATE_RXDAC_CALIBRATE_S 15
#define AR_PHY_65NM_RBIST_CNTRL_ATE_CMAC_POWER_HPF_ENABLE   0x00004000
#define AR_PHY_65NM_RBIST_CNTRL_ATE_CMAC_POWER_HPF_ENABLE_S 14
#define AR_PHY_65NM_RBIST_CNTRL_ATE_CMAC_I2Q2_ENABLE   0x00002000
#define AR_PHY_65NM_RBIST_CNTRL_ATE_CMAC_I2Q2_ENABLE_S 13
#define AR_PHY_65NM_RBIST_CNTRL_ATE_CMAC_IQ_ENABLE   0x00001000
#define AR_PHY_65NM_RBIST_CNTRL_ATE_CMAC_IQ_ENABLE_S 12
#define AR_PHY_65NM_RBIST_CNTRL_ATE_CMAC_POWER_ENABLE   0x00000800
#define AR_PHY_65NM_RBIST_CNTRL_ATE_CMAC_POWER_ENABLE_S 11
#define AR_PHY_65NM_RBIST_CNTRL_ATE_CMAC_CORR_ENABLE   0x00000400
#define AR_PHY_65NM_RBIST_CNTRL_ATE_CMAC_CORR_ENABLE_S 10
#define AR_PHY_65NM_RBIST_CNTRL_ATE_CMAC_DC_ENABLE   0x00000200
#define AR_PHY_65NM_RBIST_CNTRL_ATE_CMAC_DC_ENABLE_S 9
#define AR_PHY_65NM_RBIST_CNTRL_ATE_CMAC_DC_WRITE_TO_CANCEL   0x00000100
#define AR_PHY_65NM_RBIST_CNTRL_ATE_CMAC_DC_WRITE_TO_CANCEL_S 8
#define AR_PHY_65NM_RBIST_CNTRL_ATE_TONEGEN_PRBS_ENABLE_Q   0x00000080
#define AR_PHY_65NM_RBIST_CNTRL_ATE_TONEGEN_PRBS_ENABLE_Q_S 7
#define AR_PHY_65NM_RBIST_CNTRL_ATE_TONEGEN_PRBS_ENABLE_I   0x00000040
#define AR_PHY_65NM_RBIST_CNTRL_ATE_TONEGEN_PRBS_ENABLE_I_S 6
#define AR_PHY_65NM_RBIST_CNTRL_ATE_TONEGEN_LINRAMP_ENABLE_Q   0x00000020
#define AR_PHY_65NM_RBIST_CNTRL_ATE_TONEGEN_LINRAMP_ENABLE_Q_S 5
#define AR_PHY_65NM_RBIST_CNTRL_ATE_TONEGEN_LINRAMP_ENABLE_I   0x00000010
#define AR_PHY_65NM_RBIST_CNTRL_ATE_TONEGEN_LINRAMP_ENABLE_I_S 4
#define AR_PHY_65NM_RBIST_CNTRL_ATE_TONEGEN_LFTONE0_ENABLE   0x00000008
#define AR_PHY_65NM_RBIST_CNTRL_ATE_TONEGEN_LFTONE0_ENABLE_S 3
#define AR_PHY_65NM_RBIST_CNTRL_ATE_TONEGEN_TONE1_ENABLE   0x00000004
#define AR_PHY_65NM_RBIST_CNTRL_ATE_TONEGEN_TONE1_ENABLE_S 2
#define AR_PHY_65NM_RBIST_CNTRL_ATE_TONEGEN_TONE0_ENABLE   0x00000002
#define AR_PHY_65NM_RBIST_CNTRL_ATE_TONEGEN_TONE0_ENABLE_S 1
#define AR_PHY_65NM_RBIST_CNTRL_ATE_TONEGEN_DC_ENABLE   0x00000001
#define AR_PHY_65NM_RBIST_CNTRL_ATE_TONEGEN_DC_ENABLE_S 0
#define AR_PHY_65NM_CH0_TX_DC_OFFSET       AR_PHY_65NM(ch0_tx_dc_offset)
#define AR_PHY_65NM_CH1_TX_DC_OFFSET       AR_PHY_65NM(ch1_tx_dc_offset)

#define AR_PHY_65NM_CH0_TX_TONEGEN0       AR_PHY_65NM(ch0_tx_tonegen0)
#define AR_PHY_65NM_CH1_TX_TONEGEN0       AR_PHY_65NM(ch1_tx_tonegen0)
#define AR_PHY_65NM_CH_TX_TONEGEN0_ATE_TONEGEN_TONE_TAU_K   0x7f000000
#define AR_PHY_65NM_CH_TX_TONEGEN0_ATE_TONEGEN_TONE_TAU_K_S 24
#define AR_PHY_65NM_CH_TX_TONEGEN0_ATE_TONEGEN_TONE_A_MAN   0x00ff0000
#define AR_PHY_65NM_CH_TX_TONEGEN0_ATE_TONEGEN_TONE_A_MAN_S 16
#define AR_PHY_65NM_CH_TX_TONEGEN0_ATE_TONEGEN_TONE_A_EXP   0x00000f00
#define AR_PHY_65NM_CH_TX_TONEGEN0_ATE_TONEGEN_TONE_A_EXP_S 8
#define AR_PHY_65NM_CH_TX_TONEGEN0_ATE_TONEGEN_TONE_FREQ   0x0000007f
#define AR_PHY_65NM_CH_TX_TONEGEN0_ATE_TONEGEN_TONE_FREQ_S 0
#define AR_PHY_65NM_CH0_CMAC_DC_CANCEL       AR_PHY_65NM(ch0_cmac_dc_cancel)
#define AR_PHY_65NM_CH1_CMAC_DC_CANCEL       AR_PHY_65NM(ch1_cmac_dc_cancel)
#define AR_PHY_65NM_CMAC_DC_CANCEL_ATE_CMAC_DC_CANCEL_Q   0x03ff0000
#define AR_PHY_65NM_CMAC_DC_CANCEL_ATE_CMAC_DC_CANCEL_Q_S 16
#define AR_PHY_65NM_CMAC_DC_CANCEL_ATE_CMAC_DC_CANCEL_I   0x000003ff
#define AR_PHY_65NM_CMAC_DC_CANCEL_ATE_CMAC_DC_CANCEL_I_S 0
#define AR_PHY_65NM_CH0_CMAC_DC_OFFSET       AR_PHY_65NM(ch0_cmac_dc_offset)
#define AR_PHY_65NM_CH1_CMAC_DC_OFFSET       AR_PHY_65NM(ch1_cmac_dc_offset)
#define AR_PHY_65NM_CMAC_DC_OFFSET_ATE_CMAC_DC_CYCLES   0x0000000f
#define AR_PHY_65NM_CMAC_DC_OFFSET_ATE_CMAC_DC_CYCLES_S 0
#define AR_PHY_65NM_CH0_CMAC_POWER       AR_PHY_65NM(ch0_cmac_power)
#define AR_PHY_65NM_CH1_CMAC_POWER       AR_PHY_65NM(ch1_cmac_power)
#define AR_PHY_65NM_CMAC_POWER_ATE_CMAC_POWER_CYCLES   0x0000000f
#define AR_PHY_65NM_CMAC_POWER_ATE_CMAC_POWER_CYCLES_S 0

#define AR_PHY_65NM_CH0_CMAC_RESULT_I       AR_PHY_65NM(ch0_cmac_results_i)
#define AR_PHY_65NM_CH1_CMAC_RESULT_I       AR_PHY_65NM(ch1_cmac_results_i)
#define AR_PHY_65NM_CMAC_RESULT_I_ATE_CMAC_RESULTS   0xffffffff
#define AR_PHY_65NM_CMAC_RESULT_I_ATE_CMAC_RESULTS_S 0
#define AR_PHY_65NM_CH0_CMAC_RESULT_Q       AR_PHY_65NM(ch0_cmac_results_q)
#define AR_PHY_65NM_CH1_CMAC_RESULT_Q       AR_PHY_65NM(ch1_cmac_results_q)
#define AR_PHY_65NM_CMAC_RESULT_Q_ATE_CMAC_RESULTS   0xffffffff
#define AR_PHY_65NM_CMAC_RESULT_Q_ATE_CMAC_RESULTS_S 0





#define AR_PHY_65NM_CH0_RXRFSA_GAINSTAGES       AR_PHY_65NM(ch0_RXRF_GAINSTAGES)
#define AR_PHY_65NM_CH1_RXRFJU_GAINSTAGES       AR_PHY_65NM(ch1_RXRF_GAINSTAGES)

#define AR_PHY_65NM_RXRF_GAINSTAGES_RX_OVERRIDE   0x80000000
#define AR_PHY_65NM_RXRF_GAINSTAGES_RX_OVERRIDE_S 31
#define AR_PHY_65NM_RXRF_GAINSTAGES_LNA2G_GAIN_OVR   0x70000000
#define AR_PHY_65NM_RXRF_GAINSTAGES_LNA2G_GAIN_OVR_S 28
#define AR_PHY_65NM_RXRF_GAINSTAGES_LNA5G_GAIN_OVR   0x03800000
#define AR_PHY_65NM_RXRF_GAINSTAGES_LNA5G_GAIN_OVR_S 23
#define AR_PHY_65NM_RXRF_GAINSTAGES_LNAON_CALDC   0x00000002
#define AR_PHY_65NM_RXRF_GAINSTAGES_LNAON_CALDC_S 1
//ch0_RXTX1
#define AR_PHY_65NM_RXTX1_MAINTXGAIN   0x80000000
#define AR_PHY_65NM_RXTX1_MAINTXGAIN_S 31
#define AR_PHY_65NM_RXTX1_TXBB_GC   0x78000000
#define AR_PHY_65NM_RXTX1_TXBB_GC_S 27
#define AR_PHY_65NM_RXTX1_PADRV4GN5G   0x07800000
#define AR_PHY_65NM_RXTX1_PADRV4GN5G_S 23
#define AR_PHY_65NM_RXTX1_PADRV3GN5G   0x00780000
#define AR_PHY_65NM_RXTX1_PADRV3GN5G_S 19
#define AR_PHY_65NM_RXTX1_PADRV2GN   0x00078000
#define AR_PHY_65NM_RXTX1_PADRV2GN_S 15
#define AR_PHY_65NM_RXTX1_PADRVHALFGN2G   0x00004000
#define AR_PHY_65NM_RXTX1_PADRVHALFGN2G_S 14
#define AR_PHY_65NM_RXTX1_TX6DB_BIQUAD   0x00003000
#define AR_PHY_65NM_RXTX1_TX6DB_BIQUAD_S 12
#define AR_PHY_65NM_RXTX1_TX1DB_BIQUAD   0x00000E00
#define AR_PHY_65NM_RXTX1_TX1DB_BIQUAD_S 9
#define AR_PHY_65NM_RXTX1_MAINRXGAIN_JUPITER   0x00000008
#define AR_PHY_65NM_RXTX1_MAINRXGAIN_JUPITER_S 3
#define AR_PHY_65NM_RXTX1_SCFIR_GAIN_JUPITER   0x00000004
#define AR_PHY_65NM_RXTX1_SCFIR_GAIN_JUPITER_S 2
//RXTX1 for poseidon
#define AR_PHY_65NM_RXTX1_MAINRXGAIN_POSEIDON   0x00000002
#define AR_PHY_65NM_RXTX1_MAINRXGAIN_POSEIDON_S 1
#define AR_PHY_65NM_RXTX1_SCFIR_GAIN_POSEIDON   0x00000001
#define AR_PHY_65NM_RXTX1_SCFIR_GAIN_POSEIDON_S 0

//ch0_RXTX2
#define AR_PHY_65NM_RXTX2_LNAGAIN   0xE0000000
#define AR_PHY_65NM_RXTX2_LNAGAIN_S 29
#define AR_PHY_65NM_RXTX2_VGAGAIN   0x1c000000
#define AR_PHY_65NM_RXTX2_VGAGAIN_S 26
#define AR_PHY_65NM_RXTX2_MXRGAIN   0x03000000
#define AR_PHY_65NM_RXTX2_MXRGAIN_S 24
#define AR_PHY_65NM_RXTX2_RX6DB_BIQUAD   0x00c00000
#define AR_PHY_65NM_RXTX2_RX6DB_BIQUAD_S 22
#define AR_PHY_65NM_RXTX2_RX1DB_BIQUAD   0x00380000
#define AR_PHY_65NM_RXTX2_RX1DB_BIQUAD_S 19
#define AR_PHY_65NM_RXTX2_RXON_OVR   0x00001000
#define AR_PHY_65NM_RXTX2_RXON_OVR_S 12
#define AR_PHY_65NM_RXTX2_RXON   0x00000800
#define AR_PHY_65NM_RXTX2_RXON_S 11
#define AR_PHY_65NM_RXTX2_PAON_OVR   0x00000400
#define AR_PHY_65NM_RXTX2_PAON_OVR_S 10
#define AR_PHY_65NM_RXTX2_PAON   0x00000200
#define AR_PHY_65NM_RXTX2_PAON_S 9
#define AR_PHY_65NM_RXTX2_TXON_OVR   0x00000100
#define AR_PHY_65NM_RXTX2_TXON_OVR_S 8
#define AR_PHY_65NM_RXTX2_TXON   0x00000080
#define AR_PHY_65NM_RXTX2_TXON_S 7
#define AR_PHY_65NM_RXTX2_SYNTHON_OVR   0x00000008
#define AR_PHY_65NM_RXTX2_SYNTHON_OVR_S 3
#define AR_PHY_65NM_RXTX2_SYNTHON   0x00000004
#define AR_PHY_65NM_RXTX2_SYNTHON_S 2
#define AR_PHY_65NM_RXTX2_BMODE_OVR   0x00000002
#define AR_PHY_65NM_RXTX2_BMODE_OVR_S 1
#define AR_PHY_65NM_RXTX2_BMODE   0x00000001
#define AR_PHY_65NM_RXTX2_BMODE_S 0

//ch0_RXTX3
#define AR_PHY_65NM_RXTX3_CALPA_OVR_JUPITER   0x80000000
#define AR_PHY_65NM_RXTX3_CALPA_OVR_JUPITER_S 31
#define AR_PHY_65NM_RXTX3_CALPA_JUPITER   0x40000000
#define AR_PHY_65NM_RXTX3_CALPA_JUPITER_S 30
#define AR_PHY_65NM_RXTX3_CALTX_OVR_JUPITER   0x08000000
#define AR_PHY_65NM_RXTX3_CALTX_OVR_JUPITER_S 27
#define AR_PHY_65NM_RXTX3_CALTX_JUPITER   0x04000000
#define AR_PHY_65NM_RXTX3_CALTX_JUPITER_S 26
#define AR_PHY_65NM_RXTX3_DACFULLSCALE_JUPITER   0x00000200
#define AR_PHY_65NM_RXTX3_DACFULLSCALE_JUPITER_S 9
//RXTX3 for poseidon
#define AR_PHY_65NM_RXTX3_CALPA_OVR_POSEIDON   0x20000000
#define AR_PHY_65NM_RXTX3_CALPA_OVR_POSEIDON_S 29
#define AR_PHY_65NM_RXTX3_CALPA_POSEIDON   0x10000000
#define AR_PHY_65NM_RXTX3_CALPA_POSEIDON_S 28
#define AR_PHY_65NM_RXTX3_CALTX_OVR_POSEIDON   0x02000000
#define AR_PHY_65NM_RXTX3_CALTX_OVR_POSEIDON_S 25
#define AR_PHY_65NM_RXTX3_CALTX_POSEIDON   0x01000000
#define AR_PHY_65NM_RXTX3_CALTX_POSEIDON_S 24
#define AR_PHY_65NM_RXTX3_ADCPWD_OVR_POSEIDON   0x00000400
#define AR_PHY_65NM_RXTX3_ADCPWD_OVR_POSEIDON_S 10
#define AR_PHY_65NM_RXTX3_ADCPWD_POSEIDON   0x00000200
#define AR_PHY_65NM_RXTX3_ADCPWD_POSEIDON_S 9
#define AR_PHY_65NM_RXTX3_DACPWD_OVR_POSEIDON   0x00000100
#define AR_PHY_65NM_RXTX3_DACPWD_OVR_POSEIDON_S 8
#define AR_PHY_65NM_RXTX3_DACPWD_POSEIDON   0x00000080
#define AR_PHY_65NM_RXTX3_DACPWD_POSEIDON_S 7
#define AR_PHY_65NM_RXTX3_DACFULLSCALE_POSEIDON   0x00000008
#define AR_PHY_65NM_RXTX3_DACFULLSCALE_POSEIDON_S 3


#define AR_PHY_65NM_CH0_BB1         AR_PHY_65NM(ch0_BB1)
#define AR_PHY_65NM_CH0_BB2         AR_PHY_65NM(ch0_BB2)
#define AR_PHY_65NM_CH0_BB2_SWITCH_OVERRIDE         0x1
#define AR_PHY_65NM_CH0_BB2_SWITCH_OVERRIDE_S       31
#define AR_PHY_65NM_CH0_BB2_RXIN2BQ_EN         		0x1
#define AR_PHY_65NM_CH0_BB2_RXIN2BQ_EN_S       		30
#define AR_PHY_65NM_CH0_BB2_RXIN2I2V_EN        		0x1
#define AR_PHY_65NM_CH0_BB2_RXIN2I2V_EN_S      		29
#define AR_PHY_65NM_CH0_BB2_BQ2RXOUT_EN        		0x1
#define AR_PHY_65NM_CH0_BB2_BQ2RXOUT_EN_S      		28
#define AR_PHY_65NM_CH0_BB2_I2V2RXOUT_EN       		0x1
#define AR_PHY_65NM_CH0_BB2_I2V2RXOUT_EN_S     		27
#define AR_PHY_65NM_CH0_BB2_OVERRIDE_FILTERFC  		0x1
#define AR_PHY_65NM_CH0_BB2_OVERRIDE_FILTERFC_S		26
#define AR_PHY_65NM_CH0_BB2_FILTERFC  				0x1F
#define AR_PHY_65NM_CH0_BB2_FILTERFC_S				21
#define AR_PHY_65NM_CH0_BB2_OVERRIDE_FNOTCH			0x1
#define AR_PHY_65NM_CH0_BB2_OVERRIDE_FNOTCH_S		20
#define AR_PHY_65NM_CH0_BB2_FNOTCH  				0xF
#define AR_PHY_65NM_CH0_BB2_FNOTCH_S				16
#define AR_PHY_65NM_CH0_BB2_OVERRIDE_RCFILTER_CAP	0x1
#define AR_PHY_65NM_CH0_BB2_OVERRIDE_RCFILTER_CAP_S	15
#define AR_PHY_65NM_CH0_BB2_RCFILTER_CAP			0x1F
#define AR_PHY_65NM_CH0_BB2_RCFILTER_CAP_S			10
#define AR_PHY_65NM_CH0_BB2_SEL_TEST				0x3
#define AR_PHY_65NM_CH0_BB2_SEL_TEST_S				8
#define AR_PHY_65NM_CH0_BB2_MXR_HIGHGAINMASK		0xF
#define AR_PHY_65NM_CH0_BB2_MXR_HIGHGAINMASK_S 		4
#define AR_PHY_65NM_CH0_BB2_SPARE					0xF
#define AR_PHY_65NM_CH0_BB2_SPARE_S 				0
#define AR_PHY_65NM_CH0_BB3         AR_PHY_65NM(ch0_BB3)

#define AR_PHY_65NM_CH0_BB3_RXONLY_FILTERFC                0x1F
#define AR_PHY_65NM_CH0_BB3_RXONLY_FILTERFC_S              11
#define AR_PHY_65NM_CH0_BB3_OVERRIDE_RXONLY_FILTERFC       0x1
#define AR_PHY_65NM_CH0_BB3_OVERRIDE_RXONLY_FILTERFC_S     10

#define AR_PHY_65NM_CH0_BB3_ADC_DELAY                      0x3
#define AR_PHY_65NM_CH0_BB3_ADC_DELAY_S                    6
#define AR_PHY_65NM_CH0_BB3_CLK_DAC160_INVERT      0x1
#define AR_PHY_65NM_CH0_BB3_CLK_DAC160_INVERT_S    5
#define AR_PHY_65NM_CH0_BB3_FCTXOVRX                       0x3
#define AR_PHY_65NM_CH0_BB3_FCTXOVRX_S                 3
#define AR_PHY_65NM_CH0_BB3_FORCEFIRON                 0x1
#define AR_PHY_65NM_CH0_BB3_FORCEFIRON_S               2
#define AR_PHY_65NM_CH0_BB3_SPARE_JET                      0x3
#define AR_PHY_65NM_CH0_BB3_SPARE_JET_S                    0
#define AR_PHY_65NM_CH0_BB3_SPARE			0x000000FF
#define AR_PHY_65NM_CH0_BB3_SPARE_S			0
#define AR_PHY_65NM_CH1_BB1         AR_PHY_65NM(ch1_BB1)
#define AR_PHY_65NM_CH1_BB2         AR_PHY_65NM(ch1_BB2)
#define AR_PHY_65NM_CH1_BB2_SWITCH_OVERRIDE         0x1
#define AR_PHY_65NM_CH1_BB2_SWITCH_OVERRIDE_S       31
#define AR_PHY_65NM_CH1_BB2_RXIN2BQ_EN         		0x1
#define AR_PHY_65NM_CH1_BB2_RXIN2BQ_EN_S       		30
#define AR_PHY_65NM_CH1_BB2_RXIN2I2V_EN        		0x1
#define AR_PHY_65NM_CH1_BB2_RXIN2I2V_EN_S      		29
#define AR_PHY_65NM_CH1_BB2_BQ2RXOUT_EN        		0x1
#define AR_PHY_65NM_CH1_BB2_BQ2RXOUT_EN_S      		28
#define AR_PHY_65NM_CH1_BB2_I2V2RXOUT_EN       		0x1
#define AR_PHY_65NM_CH1_BB2_I2V2RXOUT_EN_S     		27
#define AR_PHY_65NM_CH1_BB2_OVERRIDE_FILTERFC  		0x1
#define AR_PHY_65NM_CH1_BB2_OVERRIDE_FILTERFC_S		26
#define AR_PHY_65NM_CH1_BB2_FILTERFC  				0x1F
#define AR_PHY_65NM_CH1_BB2_FILTERFC_S				21
#define AR_PHY_65NM_CH1_BB2_OVERRIDE_FNOTCH			0x1
#define AR_PHY_65NM_CH1_BB2_OVERRIDE_FNOTCH_S		20
#define AR_PHY_65NM_CH1_BB2_FNOTCH  				0xF
#define AR_PHY_65NM_CH1_BB2_FNOTCH_S				16
#define AR_PHY_65NM_CH1_BB2_OVERRIDE_RCFILTER_CAP	0x1
#define AR_PHY_65NM_CH1_BB2_OVERRIDE_RCFILTER_CAP_S	15
#define AR_PHY_65NM_CH1_BB2_RCFILTER_CAP			0xF
#define AR_PHY_65NM_CH1_BB2_RCFILTER_CAP_S			10
#define AR_PHY_65NM_CH1_BB2_SEL_TEST				0x3
#define AR_PHY_65NM_CH1_BB2_SEL_TEST_S				8
#define AR_PHY_65NM_CH1_BB2_MXR_HIGHGAINMASK		0xF
#define AR_PHY_65NM_CH1_BB2_MXR_HIGHGAINMASK_S 		4
#define AR_PHY_65NM_CH1_BB2_SPARE					0xF
#define AR_PHY_65NM_CH1_BB2_SPARE_S 				0
#define AR_PHY_65NM_CH1_BB3         AR_PHY_65NM(ch1_BB3)
#define AR_PHY_65NM_CH1_BB3_ADC_DELAY                      0x3
#define AR_PHY_65NM_CH1_BB3_ADC_DELAY_S                    6
#define AR_PHY_65NM_CH1_BB3_CLK_DAC160_INVERT      0x1
#define AR_PHY_65NM_CH1_BB3_CLK_DAC160_INVERT_S    5
#define AR_PHY_65NM_CH1_BB3_FCTXOVRX                       0x3
#define AR_PHY_65NM_CH1_BB3_FCTXOVRX_S                 3
#define AR_PHY_65NM_CH1_BB3_FORCEFIRON                 0x1
#define AR_PHY_65NM_CH1_BB3_FORCEFIRON_S               2
#define AR_PHY_65NM_CH1_BB3_SPARE_JET                      0x3
#define AR_PHY_65NM_CH1_BB3_SPARE_JET_S                    0

#define AR_PHY_65NM_CH1_BB3_SPARE			0x000000FF
#define AR_PHY_65NM_CH1_BB3_SPARE_S			0
#define AR_PHY_65NM_CH2_BB1         AR_PHY_65NM(ch2_BB1)
#define AR_PHY_65NM_CH2_BB2         AR_PHY_65NM(ch2_BB2)
#define AR_PHY_65NM_CH2_BB2_SWITCH_OVERRIDE         0x1
#define AR_PHY_65NM_CH2_BB2_SWITCH_OVERRIDE_S       31
#define AR_PHY_65NM_CH2_BB2_RXIN2BQ_EN         		0x1
#define AR_PHY_65NM_CH2_BB2_RXIN2BQ_EN_S       		30
#define AR_PHY_65NM_CH2_BB2_RXIN2I2V_EN        		0x1
#define AR_PHY_65NM_CH2_BB2_RXIN2I2V_EN_S      		29
#define AR_PHY_65NM_CH2_BB2_BQ2RXOUT_EN        		0x1
#define AR_PHY_65NM_CH2_BB2_BQ2RXOUT_EN_S      		28
#define AR_PHY_65NM_CH2_BB2_I2V2RXOUT_EN       		0x1
#define AR_PHY_65NM_CH2_BB2_I2V2RXOUT_EN_S     		27
#define AR_PHY_65NM_CH2_BB2_OVERRIDE_FILTERFC  		0x1
#define AR_PHY_65NM_CH2_BB2_OVERRIDE_FILTERFC_S		26
#define AR_PHY_65NM_CH2_BB2_FILTERFC  				0x1F
#define AR_PHY_65NM_CH2_BB2_FILTERFC_S				21
#define AR_PHY_65NM_CH2_BB2_OVERRIDE_FNOTCH			0x1
#define AR_PHY_65NM_CH2_BB2_OVERRIDE_FNOTCH_S		20
#define AR_PHY_65NM_CH2_BB2_FNOTCH  				0xF
#define AR_PHY_65NM_CH2_BB2_FNOTCH_S				16
#define AR_PHY_65NM_CH2_BB2_OVERRIDE_RCFILTER_CAP	0x1
#define AR_PHY_65NM_CH2_BB2_OVERRIDE_RCFILTER_CAP_S	15
#define AR_PHY_65NM_CH2_BB2_RCFILTER_CAP			0xF
#define AR_PHY_65NM_CH2_BB2_RCFILTER_CAP_S			10
#define AR_PHY_65NM_CH2_BB2_SEL_TEST				0x3
#define AR_PHY_65NM_CH2_BB2_SEL_TEST_S				8
#define AR_PHY_65NM_CH2_BB2_MXR_HIGHGAINMASK		0xF
#define AR_PHY_65NM_CH2_BB2_MXR_HIGHGAINMASK_S 		4
#define AR_PHY_65NM_CH2_BB2_SPARE					0xF
#define AR_PHY_65NM_CH2_BB2_SPARE_S 				0
#define AR_PHY_65NM_CH2_BB3	    AR_PHY_65NM(ch2_BB3)
#define AR_PHY_65NM_CH2_BB3_ADC_DELAY                      0x3
#define AR_PHY_65NM_CH2_BB3_ADC_DELAY_S                    6
#define AR_PHY_65NM_CH2_BB3_CLK_DAC160_INVERT      0x1
#define AR_PHY_65NM_CH2_BB3_CLK_DAC160_INVERT_S    5
#define AR_PHY_65NM_CH2_BB3_FCTXOVRX                       0x3
#define AR_PHY_65NM_CH2_BB3_FCTXOVRX_S                 3
#define AR_PHY_65NM_CH2_BB3_FORCEFIRON                 0x1
#define AR_PHY_65NM_CH2_BB3_FORCEFIRON_S               2
#define AR_PHY_65NM_CH2_BB3_SPARE_JET                      0x3
#define AR_PHY_65NM_CH2_BB3_SPARE_JET_S                    0

#define AR_PHY_65NM_CH2_BB3_SPARE			0x000000FF
#define AR_PHY_65NM_CH2_BB3_SPARE_S			0
#define AR_PHY_65NM_CH3_BB1         AR_PHY_65NM(ch3_BB1)
#define AR_PHY_65NM_CH3_BB2         AR_PHY_65NM(ch3_BB2)
#define AR_PHY_65NM_CH3_BB3	        AR_PHY_65NM(ch3_BB3)
#define AR_PHY_65NM_CH3_BB2_OVERRIDE_FILTERFC  		0x1
#define AR_PHY_65NM_CH3_BB2_OVERRIDE_FILTERFC_S		26
#define AR_PHY_65NM_CH3_BB2_OVERRIDE_FNOTCH			0x1
#define AR_PHY_65NM_CH3_BB2_OVERRIDE_FNOTCH_S		20
#define AR_PHY_65NM_CH3_BB2_OVERRIDE_RCFILTER_CAP	0x1
#define AR_PHY_65NM_CH3_BB2_OVERRIDE_RCFILTER_CAP_S	15
#define AR_PHY_65NM_CH3_BB3_ADC_DELAY                      0x3
#define AR_PHY_65NM_CH3_BB3_ADC_DELAY_S                    6
#define AR_PHY_65NM_CH3_BB3_CLK_DAC160_INVERT      0x1
#define AR_PHY_65NM_CH3_BB3_CLK_DAC160_INVERT_S    5
#define AR_PHY_65NM_CH3_BB3_FCTXOVRX                       0x3
#define AR_PHY_65NM_CH3_BB3_FCTXOVRX_S                 3
#define AR_PHY_65NM_CH3_BB3_FORCEFIRON                 0x1
#define AR_PHY_65NM_CH3_BB3_FORCEFIRON_S               2
#define AR_PHY_65NM_CH3_BB3_SPARE_JET                      0x3
#define AR_PHY_65NM_CH3_BB3_SPARE_JET_S                    0

#define AR_PHY_65NM_CH3_BB3_SPARE			0x000000FF
#define AR_PHY_65NM_CH3_BB3_SPARE_S			0

#define AR_PHY_CH_BB1_FORCE_I2V       0x00001000
#define AR_PHY_CH_BB1_FORCE_I2V_S     12
#define AR_PHY_CH_BB1_ENABLE_I2V       0x00000800
#define AR_PHY_CH_BB1_ENABLE_I2V_S     11
#define AR_PHY_CH_BB1_FORCE_V2I       0x00000400
#define AR_PHY_CH_BB1_FORCE_V2I_S     10
#define AR_PHY_CH_BB1_ENABLE_V2I       0x00000200
#define AR_PHY_CH_BB1_ENABLE_V2I_S     9
#define AR_PHY_CH_BB1_FORCE_NOTCH       0x00000010
#define AR_PHY_CH_BB1_FORCE_NOTCH_S     4
#define AR_PHY_CH_BB1_ENABLE_NOTCH       0x00000008
#define AR_PHY_CH_BB1_ENABLE_NOTCH_S     3

#define AR_PHY_CH_BB2_SWITCH_OVERRIDE       0x80000000
#define AR_PHY_CH_BB2_SWITCH_OVERRIDE_S     31
#define AR_PHY_CH_BB2_RXIN2BQ_EN       0x40000000
#define AR_PHY_CH_BB2_RXIN2BQ_EN_S     30
#define AR_PHY_CH_BB2_RXIN2I2V_EN       0x20000000
#define AR_PHY_CH_BB2_RXIN2I2V_EN_S     29
#define AR_PHY_CH_BB2_BQ2RXOUT_EN       0x10000000
#define AR_PHY_CH_BB2_BQ2RXOUT_EN_S     28
#define AR_PHY_CH_BB2_I2V2RXOUT_EN       0x08000000
#define AR_PHY_CH_BB2_I2V2RXOUT_EN_S     27

#define AR_PHY_CH_BB3_SEL_OFST_READBK       0x00000300
#define AR_PHY_CH_BB3_SEL_OFST_READBK_S     8
#define AR_PHY_CH_BB3_OFSTCORRI2VQ          0x03e00000
#define AR_PHY_CH_BB3_OFSTCORRI2VQ_S        21
#define AR_PHY_CH_BB3_OFSTCORRI2VI          0x7c000000
#define AR_PHY_CH_BB3_OFSTCORRI2VI_S        26

#define AR_PHY_RX1DB_BIQUAD_LONG_SHIFT   	0x00380000
#define AR_PHY_RX1DB_BIQUAD_LONG_SHIFT_S 	19
#define AR_PHY_RX6DB_BIQUAD_LONG_SHIFT   	0x00c00000
#define AR_PHY_RX6DB_BIQUAD_LONG_SHIFT_S 	22
#define AR_PHY_LNAGAIN_LONG_SHIFT   		0xe0000000
#define AR_PHY_LNAGAIN_LONG_SHIFT_S 		29
#define AR_PHY_MXRGAIN_LONG_SHIFT   		0x03000000
#define AR_PHY_MXRGAIN_LONG_SHIFT_S 		24
#define AR_PHY_VGAGAIN_LONG_SHIFT   		0x1c000000
#define AR_PHY_VGAGAIN_LONG_SHIFT_S 		26
#define AR_PHY_SCFIR_GAIN_LONG_SHIFT   		0x00000001
#define AR_PHY_SCFIR_GAIN_LONG_SHIFT_S 		0
#define AR_PHY_MANRXGAIN_LONG_SHIFT   		0x00000002
#define AR_PHY_MANRXGAIN_LONG_SHIFT_S 		1
#define AR_PHY_MANTXGAIN_LONG_SHIFT   		0x80000000
#define AR_PHY_MANTXGAIN_LONG_SHIFT_S 		31

/*
 * SM Field Definitions
 */
 
/* BB_cl_cal_ctrl - AR_PHY_CL_CAL_CTL */
#define AR_PHY_CL_CAL_ENABLE          0x00000002    /* do carrier leak calibration after agc_calibrate_done */ 
#define AR_PHY_PARALLEL_CAL_ENABLE    0x00000001 
#define AR_PHY_TPCRG1_PD_CAL_ENABLE   0x00400000
#define AR_PHY_TPCRG1_PD_CAL_ENABLE_S 22
#define AR_PHY_CL_MAP_HW_GEN		  0x80000000
#define AR_PHY_CL_MAP_HW_GEN_S		  31
#define AR_PHY_ENABLE_PARALLEL_CAL    0x00000001
#define AR_PHY_ENABLE_PARALLEL_CAL_S  0
#define AR_PHY_ENABLE_CL_CALIBRATE    0x00000001
#define AR_PHY_ENABLE_CL_CALIBRATE_S  1

/* BB_addac_parallel_control - AR_PHY_ADDAC_PARA_CTL */
#define AR_PHY_ADDAC_PARACTL_OFF_PWDADC 0x00008000
#define AR_PHY_ADDAC_PARACTL_OFF_PWDDAC 0x00002000
#define AR_PHY_ADDAC_PARACTL_OFF_PWDDAC_S 13

/* BB_fcal_2_b0 - AR_PHY_FCAL_2_0 */
#define AR_PHY_FCAL20_CAP_STATUS_0    0x01f00000
#define AR_PHY_FCAL20_CAP_STATUS_0_S  20
#define AR_PHY_FLC_PWR_THRESH         0x00000007
#define AR_PHY_FLC_PWR_THRESH_S       0
/* BB_rfbus_request */
#define AR_PHY_RFBUS_REQ_EN     0x00000001  /* request for RF bus */
/* BB_rfbus_grant */
#define AR_PHY_RFBUS_GRANT_EN   0x00000001  /* RF bus granted */
/* BB_gen_controls */
#define AR_PHY_GC_TURBO_MODE       0x00000001  /* set turbo mode bits */
#define AR_PHY_GC_TURBO_SHORT      0x00000002  /* set short symbols to turbo mode setting */
#define AR_PHY_GC_DYN2040_EN       0x00000004  /* enable dyn 20/40 mode */
#define AR_PHY_GC_DYN2040_PRI_ONLY 0x00000008  /* dyn 20/40 - primary only */
#define AR_PHY_GC_DYN2040_PRI_CH   0x00000010  /* dyn 20/40 - primary ch offset (0=+10MHz, 1=-10MHz)*/
#define AR_PHY_GC_DYN2040_PRI_CH_S 4 

#define AR_PHY_GC_DYN2040_EXT_CH   0x00000020  /* dyn 20/40 - ext ch spacing (0=20MHz/ 1=25MHz) */
#define AR_PHY_GC_HT_EN            0x00000040  /* ht enable */
#define AR_PHY_GC_SHORT_GI_40      0x00000080  /* allow short GI for HT 40 */
#define AR_PHY_GC_WALSH            0x00000100  /* walsh spatial spreading for 2 chains,2 streams TX */
#define AR_PHY_GC_SINGLE_HT_LTF1   0x00000200  /* single length (4us) 1st HT long training symbol */
#define AR_PHY_GC_GF_DETECT_EN     0x00000400  /* enable Green Field detection. Only affects rx, not tx */
#define AR_PHY_GC_ENABLE_DAC_FIFO  0x00000800  /* fifo between bb and dac */

#define AR_PHY_MS_HALF_RATE        0x00000020
#define AR_PHY_MS_QUARTER_RATE     0x00000040

/* BB_analog_power_on_time */
#define AR_PHY_RX_DELAY_DELAY      0x00003FFF  /* delay from wakeup to rx ena */
/* BB_agc_control */
#define AR_PHY_AGC_CONTROL_CAL              0x00000001  /* do internal calibration */
#define AR_PHY_AGC_CONTROL_NF               0x00000002  /* do noise-floor calibration */
#define AR_PHY_AGC_CONTROL_NF_S             1
#define AR_PHY_AGC_CONTROL_LEAKY_BUCKET_EN  0x00000400  /* enable leaky bucket */
#define AR_PHY_AGC_CONTROL_LEAKY_BUCKET_EN_S 10
#define AR_PHY_AGC_CONTROL_OFFSET_CAL       0x00000800  /* allow offset calibration */
#define AR_PHY_AGC_CONTROL_OFFSET_CAL_S     11
#define AR_PHY_AGC_CONTROL_ENABLE_NF        0x00008000  /* enable noise floor calibration to happen */
#define AR_PHY_AGC_CONTROL_FLTR_CAL         0x00010000  /* allow tx filter calibration */
#define AR_PHY_AGC_CONTROL_FLTR_CAL_S       16
#define AR_PHY_AGC_CONTROL_NO_UPDATE_NF     0x00020000  /* don't update noise floor automatically */
#define AR_PHY_AGC_CONTROL_EXT_NF_PWR_MEAS  0x00040000  /* extend noise floor power measurement */
#define AR_PHY_AGC_CONTROL_CLC_SUCCESS      0x00080000  /* carrier leak calibration done */
#define AR_PHY_AGC_CONTROL_PKDET_CAL        0x00100000  /* allow peak deteter calibration */
#define AR_PHY_AGC_CONTROL_PKDET_CAL_S      20
#define AR_PHY_AGC_CONTROL_YCOK_MAX                                 0x000003c0
#define AR_PHY_AGC_CONTROL_YCOK_MAX_S                                        6
#define AR_PHY_ENABLE_NOISEFLOOR            0x00000001
#define AR_PHY_ENABLE_NOISEFLOOR_S          15
#define AR_PHY_CAL_ENABLE                   0x00000001
#define AR_PHY_CAL_ENABLE_S                 11
#define AR_PHY_LEAKY_BUCKET_ENABLE          0x00000001
#define AR_PHY_LEAKY_BUCKET_ENABLE_S        10
#define AR_PHY_ENABLE_FLTR_CAL              0x00000001
#define AR_PHY_ENABLE_FLTR_CAL_S            16
#define AR_PHY_ENABLE_PKDET_CAL             0x00000001
#define AR_PHY_ENABLE_PKDET_CAL_S           20
#define AR_PHY_DO_CALIBRATE                 0x00000001
#define AR_PHY_DO_CALIBRATE_S               0

/* BB_iq_adc_cal_mode */
#define AR_PHY_CALMODE_IQ           0x00000000
#define AR_PHY_CALMODE_ADC_GAIN     0x00000001
#define AR_PHY_CALMODE_ADC_DC_PER   0x00000002
#define AR_PHY_CALMODE_ADC_DC_INIT  0x00000003
/* BB_analog_swap */
#define AR_PHY_SWAP_ALT_CHAIN       0x00000040
/* BB_modes_select */
#define AR_PHY_MODE_OFDM            0x00000000  /* OFDM */
#define AR_PHY_MODE_CCK             0x00000001  /* CCK */
#define AR_PHY_MODE_DYNAMIC         0x00000004  /* dynamic CCK/OFDM mode */
#define AR_PHY_MODE_DYNAMIC_S		2
#define AR_PHY_MODE_HALF            0x00000020  /* enable half rate */
#define AR_PHY_MODE_QUARTER         0x00000040  /* enable quarter rate */
#define AR_PHY_MAC_CLK_MODE         0x00000080  /* MAC runs at 128/141MHz clock */
#define AR_PHY_MODE_DYN_CCK_DISABLE 0x00000100  /* Disable dynamic CCK detection */
#define AR_PHY_MODE_SVD_HALF        0x00000200  /* enable svd half rate */
#define AR_PHY_MODE_DISABLE_CCK     0x00000100
#define AR_PHY_MODE_DISABLE_CCK_S   8
/* BB_active */
#define AR_PHY_ACTIVE_EN    0x00000001  /* Activate PHY chips */
#define AR_PHY_ACTIVE_DIS   0x00000000  /* Deactivate PHY chips */
/* BB_force_analog */
#define AR_PHY_FORCE_XPA_CFG    0x000000001
#define AR_PHY_FORCE_XPA_CFG_S  0
/* BB_xpa_timing_control */
#define AR_PHY_XPA_TIMING_CTL_TX_END_XPAB_OFF    0xFF000000
#define AR_PHY_XPA_TIMING_CTL_TX_END_XPAB_OFF_S  24
#define AR_PHY_XPA_TIMING_CTL_TX_END_XPAA_OFF    0x00FF0000
#define AR_PHY_XPA_TIMING_CTL_TX_END_XPAA_OFF_S  16
#define AR_PHY_XPA_TIMING_CTL_FRAME_XPAB_ON      0x0000FF00
#define AR_PHY_XPA_TIMING_CTL_FRAME_XPAB_ON_S    8
#define AR_PHY_XPA_TIMING_CTL_FRAME_XPAA_ON      0x000000FF
#define AR_PHY_XPA_TIMING_CTL_FRAME_XPAA_ON_S    0
/* BB_tx_timing_3 */
#define AR_PHY_TX_END_TO_A2_RX_ON       0x00FF0000
#define AR_PHY_TX_END_TO_A2_RX_ON_S     16
/* BB_tx_timing_2 */
#define AR_PHY_TX_END_DATA_START  0x000000FF
#define AR_PHY_TX_END_DATA_START_S  0
#define AR_PHY_TX_END_PA_ON       0x0000FF00
#define AR_PHY_TX_END_PA_ON_S       8
/* BB_tpc_5_b0 */
/* ar2413 power control */
#define AR_PHY_TPCRG5_PD_GAIN_OVERLAP   0x0000000F
#define AR_PHY_TPCRG5_PD_GAIN_OVERLAP_S     0
#define AR_PHY_TPCRG5_PD_GAIN_BOUNDARY_1    0x000003F0
#define AR_PHY_TPCRG5_PD_GAIN_BOUNDARY_1_S  4
#define AR_PHY_TPCRG5_PD_GAIN_BOUNDARY_2    0x0000FC00
#define AR_PHY_TPCRG5_PD_GAIN_BOUNDARY_2_S  10
#define AR_PHY_TPCRG5_PD_GAIN_BOUNDARY_3    0x003F0000
#define AR_PHY_TPCRG5_PD_GAIN_BOUNDARY_3_S  16
#define AR_PHY_TPCRG5_PD_GAIN_BOUNDARY_4    0x0FC00000
#define AR_PHY_TPCRG5_PD_GAIN_BOUNDARY_4_S  22
/* BB_tpc_1 */
#define AR_PHY_TPCRG1_NUM_PD_GAIN   0x0000c000
#define AR_PHY_TPCRG1_NUM_PD_GAIN_S 14
#define AR_PHY_TPCRG1_PD_GAIN_1    0x00030000
#define AR_PHY_TPCRG1_PD_GAIN_1_S  16
#define AR_PHY_TPCRG1_PD_GAIN_2    0x000C0000
#define AR_PHY_TPCRG1_PD_GAIN_2_S  18
#define AR_PHY_TPCRG1_PD_GAIN_3    0x00300000
#define AR_PHY_TPCRG1_PD_GAIN_3_S  20
#define AR_PHY_TPCGR1_FORCED_DAC_GAIN   0x0000003e
#define AR_PHY_TPCGR1_FORCED_DAC_GAIN_S 1
#define AR_PHY_TPCGR1_FORCE_DAC_GAIN    0x00000001
 
/* BB_tx_forced_gain */
#define AR_PHY_TXGAIN_FORCE               0x00000001
#define AR_PHY_TXGAIN_FORCE_S             0
#define AR_PHY_TXGAIN_FORCED_PADVGNRA     0x00003c00
#define AR_PHY_TXGAIN_FORCED_PADVGNRA_S   10
#define AR_PHY_TXGAIN_FORCED_PADVGNRB     0x0003c000
#define AR_PHY_TXGAIN_FORCED_PADVGNRB_S   14
#define AR_PHY_TXGAIN_FORCED_PADVGNRC     0x003c0000
#define AR_PHY_TXGAIN_FORCED_PADVGNRC_S   18
#define AR_PHY_TXGAIN_FORCED_PADVGNRD     0x00c00000
#define AR_PHY_TXGAIN_FORCED_PADVGNRD_S   22
#define AR_PHY_TXGAIN_FORCED_TXMXRGAIN    0x000003c0
#define AR_PHY_TXGAIN_FORCED_TXMXRGAIN_S  6
#define AR_PHY_TXGAIN_FORCED_TXBB1DBGAIN  0x0000000e
#define AR_PHY_TXGAIN_FORCED_TXBB1DBGAIN_S 1
#define AR_PHY_TXGAIN_FORCED_TXBB6DBGAIN  0x00000030
#define AR_PHY_TXGAIN_FORCED_TXBB6DBGAIN_S 4

/* BB_powertx_rate13 & BB_powertx_rate14 */
#define QCN5500_RATE13_POWERTXHT20_14     0x0000003f
#define QCN5500_RATE13_POWERTXHT20_15     0x00003f00
#define QCN5500_RATE13_POWERTXHT20_16     0x003f0000
#define QCN5500_RATE13_POWERTXHT20_17     0x3f000000
#define QCN5500_RATE14_POWERTXHT40_14     0x0000003f
#define QCN5500_RATE14_POWERTXHT40_15     0x00003f00
#define QCN5500_RATE14_POWERTXHT40_16     0x003f0000
#define QCN5500_RATE14_POWERTXHT40_17     0x3f000000

/* BB_tpc_21 & BB_tpc_22 */
#define QCN5500_PHY_TPC21_DESIRED_SCALE_HT20_14     0x0000001f
#define QCN5500_PHY_TPC21_DESIRED_SCALE_HT20_15     0x000003e0
#define QCN5500_PHY_TPC21_DESIRED_SCALE_HT20_16     0x00007c00
#define QCN5500_PHY_TPC21_DESIRED_SCALE_HT20_17     0x000f8000
#define QCN5500_PHY_TPC22_DESIRED_SCALE_HT40_14     0x0000001f
#define QCN5500_PHY_TPC22_DESIRED_SCALE_HT40_15     0x000003e0
#define QCN5500_PHY_TPC22_DESIRED_SCALE_HT40_16     0x00007c00
#define QCN5500_PHY_TPC22_DESIRED_SCALE_HT40_17     0x000f8000

/* BB_powertx_rate1 */
#define AR_PHY_POWER_TX_RATE1   0x9934
#define AR_PHY_POWER_TX_RATE2   0x9938
#define AR_PHY_POWER_TX_RATE_MAX    AR_PHY_PWRTX_MAX     
#define AR_PHY_POWER_TX_RATE_MAX_TPC_ENABLE 0x00000040
/* BB_test_controls */
#define PHY_AGC_CLR             0x10000000      /* disable AGC to A2 */
#define RFSILENT_BB             0x00002000      /* shush bb */
/* BB_chan_info_gain_diff */
#define AR_PHY_CHAN_INFO_GAIN_DIFF_PPM_MASK          0xFFF    /* PPM value is 12-bit signed integer */
#define AR_PHY_CHAN_INFO_GAIN_DIFF_PPM_SIGNED_BIT    0x800    /* Sign bit */
#define AR_PHY_CHAN_INFO_GAIN_DIFF_UPPER_LIMIT         320    /* Maximum absolute value */
/* BB_chaninfo_ctrl */
#define AR_PHY_CHAN_INFO_MEMORY_CAPTURE_MASK         0x0001
/* BB_search_start_delay */
#define AR_PHY_RX_DELAY_DELAY   0x00003FFF  /* delay from wakeup to rx ena */
/* BB_bbb_tx_ctrl */
#define AR_PHY_CCK_TX_CTRL_JAPAN    0x00000010
/* BB_spectral_scan */
#define AR_PHY_SPECTRAL_SCAN_ENABLE         0x00000001  /* Enable spectral scan */
#define AR_PHY_SPECTRAL_SCAN_ENABLE_S       0
#define AR_PHY_SPECTRAL_SCAN_ACTIVE         0x00000002  /* Activate spectral scan */
#define AR_PHY_SPECTRAL_SCAN_ACTIVE_S       1
#define AR_PHY_SPECTRAL_SCAN_FFT_PERIOD     0x000000F0  /* Interval for FFT reports */
#define AR_PHY_SPECTRAL_SCAN_FFT_PERIOD_S   4
#define AR_PHY_SPECTRAL_SCAN_PERIOD         0x0000FF00  /* Interval for FFT reports */
#define AR_PHY_SPECTRAL_SCAN_PERIOD_S       8
#define AR_PHY_SPECTRAL_SCAN_COUNT          0x0FFF0000  /* Number of reports */
#define AR_PHY_SPECTRAL_SCAN_COUNT_S        16
#define AR_PHY_SPECTRAL_SCAN_SHORT_REPEAT   0x10000000  /* Short repeat */
#define AR_PHY_SPECTRAL_SCAN_SHORT_REPEAT_S 28
#define AR_PHY_SPECTRAL_SCAN_PRIORITY_HI    0x20000000  /* high priority */
#define AR_PHY_SPECTRAL_SCAN_PRIORITY_HI_S  29
#define QCN5500_PHY_SPECTRAL_SCAN_COMPRESSED_RPT    0x80000000
#define QCN5500_PHY_SPECTRAL_SCAN_COMPRESSED_RPT_S  31

#define QCN5500_PHY_SPECTRAL_SCAN2_RPT_MODE           0x00000001
#define QCN5500_PHY_SPECTRAL_SCAN2_NOISE_FLOOR_REF    0x000001FE

/* BB_channel_status */
#define AR_PHY_CHANNEL_STATUS_RX_CLEAR      0x00000004
/* BB_rtt_ctrl */
#define AR_PHY_RTT_CTRL_ENA_RADIO_RETENTION     0x00000001
#define AR_PHY_RTT_CTRL_ENA_RADIO_RETENTION_S   0
#define AR_PHY_RTT_CTRL_RESTORE_MASK            0x0000007E
#define AR_PHY_RTT_CTRL_RESTORE_MASK_S          1
#define AR_PHY_RTT_CTRL_FORCE_RADIO_RESTORE     0x00000080
#define AR_PHY_RTT_CTRL_FORCE_RADIO_RESTORE_S   7
/* BB_rtt_table_sw_intf_b0 */
#define AR_PHY_RTT_SW_RTT_TABLE_ACCESS_0        0x00000001
#define AR_PHY_RTT_SW_RTT_TABLE_ACCESS_0_S      0
#define AR_PHY_RTT_SW_RTT_TABLE_WRITE_0         0x00000002
#define AR_PHY_RTT_SW_RTT_TABLE_WRITE_0_S       1
#define AR_PHY_RTT_SW_RTT_TABLE_ADDR_0          0x0000001C
#define AR_PHY_RTT_SW_RTT_TABLE_ADDR_0_S        2
/* BB_rtt_table_sw_intf_1_b0 */
#define AR_PHY_RTT_SW_RTT_TABLE_DATA_0          0xFFFFFFF0
#define AR_PHY_RTT_SW_RTT_TABLE_DATA_0_S        4
/* BB_txiqcal_control_0 */
#define AR_PHY_TX_IQCAL_CONTROL_0_ENABLE_TXIQ_CAL   0x80000000
#define AR_PHY_TX_IQCAL_CONTROL_0_ENABLE_TXIQ_CAL_S 31
#define AR_PHY_ENABLE_TXIQ_CALIBRATE            0x00000001
#define AR_PHY_ENABLE_TXIQ_CALIBRATE_S          31
/* BB_txiqcal_control_1 */
#define AR_PHY_TX_IQCAL_CONTROL_1_IQCORR_I_Q_COFF_DELPT             0x01fc0000
#define AR_PHY_TX_IQCAL_CONTROL_1_IQCORR_I_Q_COFF_DELPT_S                   18
/* BB_txiqcal_start */
#define AR_PHY_TX_IQCAL_START_DO_CAL        0x00000001
#define AR_PHY_TX_IQCAL_START_DO_CAL_S      0
/* BB_txiqcal_start for Poseidon */
#define AR_PHY_TX_IQCAL_START_DO_CAL_POSEIDON        0x80000000
#define AR_PHY_TX_IQCAL_START_DO_CAL_POSEIDON_S      31

/* Generic B0, B1, B2 IQ Cal bit fields */
/* BB_txiqcal_status_b* */
#define AR_PHY_TX_IQCAL_STATUS_FAILED    0x00000001
#define AR_PHY_CALIBRATED_GAINS_0_S 1
#define AR_PHY_CALIBRATED_GAINS_0 (0x1f<<AR_PHY_CALIBRATED_GAINS_0_S)
/* BB_txiq_corr_coeff_01_b* */
#define AR_PHY_TX_IQCAL_CORR_COEFF_00_COEFF_TABLE_S    0
#define AR_PHY_TX_IQCAL_CORR_COEFF_00_COEFF_TABLE      0x00003fff
#define AR_PHY_TX_IQCAL_CORR_COEFF_01_COEFF_TABLE_S    14
#define AR_PHY_TX_IQCAL_CORR_COEFF_01_COEFF_TABLE      (0x00003fff<<AR_PHY_TX_IQCAL_CORR_COEFF_01_COEFF_TABLE_S)

/* temp compensation */
/* BB_tpc_18 */
#define AR_PHY_TPC_18_THERM_CAL_VALUE           0xff //Mask bits 7:0
#define AR_PHY_TPC_18_THERM_CAL_VALUE_S         0
/* BB_tpc_19 */
#define AR_PHY_TPC_19_ALPHA_THERM               0xff //Mask bits 7:0
#define AR_PHY_TPC_19_ALPHA_THERM_S             0
/* BB_tpc_19_b0 */
#define AR_PHY_TPC_19_B0_ALPHA_THERM_0          0xff //Mask bits 7:0
#define AR_PHY_TPC_19_B0_ALPHA_THERM_0_S        0

/* BB_tpc_19_b1 */
#define AR_PHY_TPC_19_B1_ALPHA_THERM_0          0xff //Mask bits 7:0
#define AR_PHY_TPC_19_B1_ALPHA_THERM_0_S        0

/* ch0_RXTX4 */
#define AR_PHY_65NM_CH0_RXTX4_THERM_ON          0x10000000
#define AR_PHY_65NM_CH0_RXTX4_THERM_ON_S        28
#define AR_PHY_65NM_CH0_RXTX4_THERM_ON_OVR          0x20000000
#define AR_PHY_65NM_CH0_RXTX4_THERM_ON_OVR_S        29

/* BB_therm_adc_1 */
#define AR_PHY_BB_THERM_ADC_1_INIT_THERM        0x000000ff
#define AR_PHY_BB_THERM_ADC_1_INIT_THERM_S      0

/* BB_therm_adc_2 */
#define AR_PHY_BB_THERM_ADC_2_MEASURE_THERM     0x00000fff
#define AR_PHY_BB_THERM_ADC_2_MEASURE_THERM_S   12

/* BB_therm_adc_3 */
#define AR_PHY_BB_THERM_ADC_3_THERM_ADC_SCALE_GAIN        0x0001ff00
#define AR_PHY_BB_THERM_ADC_3_THERM_ADC_SCALE_GAIN_S      8
#define AR_PHY_BB_THERM_ADC_3_THERM_ADC_OFFSET                0x000000ff
#define AR_PHY_BB_THERM_ADC_3_THERM_ADC_OFFSET_S              0

/* BB_therm_adc_4 */
#define AR_PHY_BB_THERM_ADC_4_LATEST_THERM      0x000000ff
#define AR_PHY_BB_THERM_ADC_4_LATEST_THERM_S    0
#define AR_PHY_BB_THERM_ADC_4_PREFERRED_THERM_CHAIN    0x1E000000
#define AR_PHY_BB_THERM_ADC_4_PREFERRED_THERM_CHAIN_S  25
#define AR_PHY_BB_THERM_ADC_4_FORCE_THERM_CHAIN        0x01000000
#define AR_PHY_BB_THERM_ADC_4_FORCE_THERM_CHAIN_S      24

/* BB_switch_table_chn_b */
#define AR_PHY_SWITCH_TABLE_R0                  0x00000010
#define AR_PHY_SWITCH_TABLE_R0_S                4
#define AR_PHY_SWITCH_TABLE_R1                  0x00000040
#define AR_PHY_SWITCH_TABLE_R1_S                6
#define AR_PHY_SWITCH_TABLE_R12                 0x00000100
#define AR_PHY_SWITCH_TABLE_R12_S               8      

/* BB_switch_table_com1 */
#define AR_PHY_SWITCH_TABLE_COM_T1                  0x000000F0
#define AR_PHY_SWITCH_TABLE_COM_T1_S                4

/* BB_switch_table_com2 */
#define AR_PHY_SWITCH_TABLE_COM_RA12                  0x000f0000
#define AR_PHY_SWITCH_TABLE_COM_RA12_S                16
#define AR_PHY_SWITCH_TABLE_COM_RA2L2                  0x0000f000
#define AR_PHY_SWITCH_TABLE_COM_RA2L2_S                12
#define AR_PHY_SWITCH_TABLE_COM_RA1L2                  0x00000f00
#define AR_PHY_SWITCH_TABLE_COM_RA1L2_S                8
#define AR_PHY_SWITCH_TABLE_COM_RA2L1                  0x000000f0
#define AR_PHY_SWITCH_TABLE_COM_RA2L1_S                4
#define AR_PHY_SWITCH_TABLE_COM_RA1L1                  0x0000000f
#define AR_PHY_SWITCH_TABLE_COM_RA1L1_S                0

/* BB_switch_table_chn_b0 */
#define AR_PHY_SWITCH_TABLE_CHN_R0                  0x00000030
#define AR_PHY_SWITCH_TABLE_CHN_R0_S                4
#define AR_PHY_SWITCH_TABLE_CHN_T0                  0x0000000C
#define AR_PHY_SWITCH_TABLE_CHN_T0_S                2

/*
 * Channel 1 Register Map
 */
#define AR_CHAN1_BASE         offsetof(struct bb_reg_map, overlay_0xa800.Osprey.bb_chn1_reg_map)
#define AR_CHAN1_OFFSET(_x)   (AR_CHAN1_BASE + offsetof(struct chn1_reg_map, _x))

#define AR_PHY_TIMING4_1            AR_CHAN1_OFFSET(BB_timing_control_4_b1)
#define AR_PHY_EXT_CCA_1            AR_CHAN1_OFFSET(BB_ext_chan_pwr_thr_2_b1)
#define QCN5500_TX_PREEMP_COEF_01_B1    AR_CHAN1_OFFSET(BB_tx_preemp_coef_01_b1)
#define QCN5500_TX_PREEMP_COEF_23_B1    AR_CHAN1_OFFSET(BB_tx_preemp_coef_23_b1)
#define QCN5500_TX_PREEMP_COEF_45_B1    AR_CHAN1_OFFSET(BB_tx_preemp_coef_45_b1)
#define QCN5500_TX_PREEMP_COEF_67_B1    AR_CHAN1_OFFSET(BB_tx_preemp_coef_67_b1)
#define QCN5500_TX_PREEMP_COEF_8_B1     AR_CHAN1_OFFSET(BB_tx_preemp_coef_8_b1)
#define QCN5500_TX_NOTCH_COEF_B1        AR_CHAN1_OFFSET(BB_tx_notch_coef_b1)
#define QCN5500_SPUR_FREQ_PHASE_B1      AR_CHAN1_OFFSET(BB_spur_freq_phase_b1)

#define AR_PHY_TX_PHASE_RAMP_1      AR_CHAN1_OFFSET(BB_tx_phase_ramp_b1)
#define AR_PHY_ADC_GAIN_DC_CORR_1   AR_CHAN1_OFFSET(BB_adc_gain_dc_corr_b1)

#define AR_PHY_IQ_ADC_MEAS_0_B1     AR_CHAN1_OFFSET(BB_iq_adc_meas_0_b1)
#define AR_PHY_IQ_ADC_MEAS_1_B1     AR_CHAN1_OFFSET(BB_iq_adc_meas_1_b1)
#define AR_PHY_IQ_ADC_MEAS_2_B1     AR_CHAN1_OFFSET(BB_iq_adc_meas_2_b1)
#define AR_PHY_IQ_ADC_MEAS_3_B1     AR_CHAN1_OFFSET(BB_iq_adc_meas_3_b1)

#define AR_PHY_TX_IQ_CORR_1         AR_CHAN1_OFFSET(BB_tx_iq_corr_b1)
#define AR_PHY_SPUR_REPORT_1        AR_CHAN1_OFFSET(BB_spur_report_b1)
#define AR_PHY_CHAN_INFO_TAB_1      AR_CHAN1_OFFSET(BB_chan_info_chan_tab_b1)
#define AR_PHY_RX_IQCAL_CORR_B1     AR_CHAN1_OFFSET(BB_rx_iq_corr_b1)

/*
 * Channel 1 Field Definitions
 */
/* BB_ext_chan_pwr_thr_2_b1 */
#define AR_PHY_CH1_EXT_MINCCA_PWR   0x01FF0000
#define AR_PHY_CH1_EXT_MINCCA_PWR_S 16
#define AR_PHY_CF_MAXCCAPWR_EXT_1    0x000001FF
#define AR_PHY_CF_MAXCCAPWR_EXT_1_S  0

/*
 * AGC 1 Register Map
 */
#define AR_AGC1_BASE      offsetof(struct bb_reg_map, overlay_0xa800.Osprey.bb_agc1_reg_map)
#define AR_AGC1_OFFSET(_x)   (AR_AGC1_BASE + offsetof(struct agc1_reg_map, _x))

#define AR_PHY_FORCEMAX_GAINS_1      AR_AGC1_OFFSET(BB_gain_force_max_gains_b1)
#define AR_PHY_GAINS_MINOFF_1        AR_AGC1_OFFSET(BB_gains_min_offsets_b1)
#define AR_PHY_EXT_ATTEN_CTL_1       AR_AGC1_OFFSET(BB_ext_atten_switch_ctl_b1)
#define AR_PHY_CCA_1                 AR_AGC1_OFFSET(BB_cca_b1)
#define AR_PHY_CCA_CTRL_1            AR_AGC1_OFFSET(BB_cca_ctrl_2_b1)
#define AR_PHY_RSSI_1                AR_AGC1_OFFSET(BB_rssi_b1)
#define AR_PHY_SPUR_CCK_REP_1        AR_AGC1_OFFSET(BB_spur_est_cck_report_b1)
#define AR_PHY_RX_OCGAIN_2           AR_AGC1_OFFSET(BB_rx_ocgain2)
#define AR_PHY_BB_RX_OCGAIN_2(_i)    (AR_PHY_RX_OCGAIN_2 + ((_i)<<2))

#define AR_PHY_DIG_DC_STATUS_I_B1 AR_AGC1_OFFSET(BB_agc_dig_dc_status_i_b1)
#define AR_PHY_DIG_DC_STATUS_Q_B1 AR_AGC1_OFFSET(BB_agc_dig_dc_status_q_b1)

/*
 * AGC 1 Register Map for Poseidon
 */
#define AR_AGC1_BASE_POSEIDON      offsetof(struct bb_reg_map, overlay_0xa800.Poseidon.bb_agc1_reg_map)
#define AR_AGC1_OFFSET_POSEIDON(_x)   (AR_AGC1_BASE_POSEIDON + offsetof(struct agc1_reg_map, _x))

#define AR_PHY_FORCEMAX_GAINS_1_POSEIDON      AR_AGC1_OFFSET_POSEIDON(BB_gain_force_max_gains_b1)
#define AR_PHY_EXT_ATTEN_CTL_1_POSEIDON       AR_AGC1_OFFSET_POSEIDON(BB_ext_atten_switch_ctl_b1)
#define AR_PHY_RSSI_1_POSEIDON                AR_AGC1_OFFSET_POSEIDON(BB_rssi_b1)
#define AR_PHY_RX_OCGAIN_2_POSEIDON           AR_AGC1_OFFSET_POSEIDON(BB_rx_ocgain2)

/*
 * AGC 1 Field Definitions
 */
/* BB_cca_b1 */
#define AR_PHY_CH1_MINCCA_PWR   0x1FF00000
#define AR_PHY_CH1_MINCCA_PWR_S 20 
#define AR_PHY_CF_MAXCCAPWR_1       0x000001FF
#define AR_PHY_CF_MAXCCAPWR_1_S     0

/*
 * SM 1 Register Map
 */
#define AR_SM1_BASE      offsetof(struct bb_reg_map, overlay_0xa800.Osprey.bb_sm1_reg_map)
#define AR_SM1_OFFSET(_x)   (AR_SM1_BASE + offsetof(struct sm1_reg_map, _x))

#define AR_PHY_SWITCH_CHAIN_1    AR_SM1_OFFSET(BB_switch_table_chn_b1)
#define AR_PHY_FCAL_2_1          AR_SM1_OFFSET(BB_fcal_2_b1)
#define AR_PHY_DFT_TONE_CTL_1    AR_SM1_OFFSET(BB_dft_tone_ctrl_b1)
#define AR_PHY_CL_TAB_1          AR_SM1_OFFSET(BB_cl_tab_b1)
#define AR_PHY_CHAN_INFO_GAIN_1  AR_SM1_OFFSET(BB_chan_info_gain_b1)
#define AR_PHY_TPC_4_B1          AR_SM1_OFFSET(BB_tpc_4_b1)
#define AR_PHY_TPC_5_B1          AR_SM1_OFFSET(BB_tpc_5_b1)
#define AR_PHY_TPC_6_B1          AR_SM1_OFFSET(BB_tpc_6_b1)
#define AR_PHY_TPC_18_B1         AR_SM1_OFFSET(BB_tpc_18_b1)
#define AR_SCORPION_PHY_TPC_19_B1    AR_SM1_OFFSET(overlay_b440.Scorpion.BB_tpc_19_b1)
#define AR_PHY_PDADC_TAB_1       AR_SM1_OFFSET(overlay_b440.BB_pdadc_tab_b1)

#define AR_PHY_RTT_TABLE_SW_INTF_B1     AR_SM1_OFFSET(overlay_b440.Jupiter_20.BB_rtt_table_sw_intf_b1)
#define AR_PHY_RTT_TABLE_SW_INTF_1_B1   AR_SM1_OFFSET(overlay_b440.Jupiter_20.BB_rtt_table_sw_intf_1_b1)

#define AR_PHY_TX_IQCAL_STATUS_B1   AR_SM1_OFFSET(BB_txiqcal_status_b1)
#define AR_PHY_TX_IQCAL_CORR_COEFF_01_B1    AR_SM1_OFFSET(BB_txiq_corr_coeff_01_b1)
#define AR_PHY_TX_IQCAL_CORR_COEFF_23_B1    AR_SM1_OFFSET(BB_txiq_corr_coeff_23_b1)
#define AR_PHY_TX_IQCAL_CORR_COEFF_45_B1    AR_SM1_OFFSET(BB_txiq_corr_coeff_45_b1)
#define AR_PHY_TX_IQCAL_CORR_COEFF_67_B1    AR_SM1_OFFSET(BB_txiq_corr_coeff_67_b1)
#define AR_PHY_CL_MAP_0_B1		 AR_SM1_OFFSET(BB_cl_map_0_b1)
#define AR_PHY_CL_MAP_1_B1		 AR_SM1_OFFSET(BB_cl_map_1_b1)
#define AR_PHY_CL_MAP_2_B1		 AR_SM1_OFFSET(BB_cl_map_2_b1)
#define AR_PHY_CL_MAP_3_B1		 AR_SM1_OFFSET(BB_cl_map_3_b1)
/*
 * SM 1 Field Definitions
 */
/* BB_rtt_table_sw_intf_b1 */
#define AR_PHY_RTT_SW_RTT_TABLE_ACCESS_1        0x00000001
#define AR_PHY_RTT_SW_RTT_TABLE_ACCESS_1_S      0
#define AR_PHY_RTT_SW_RTT_TABLE_WRITE_1         0x00000002
#define AR_PHY_RTT_SW_RTT_TABLE_WRITE_1_S       1
#define AR_PHY_RTT_SW_RTT_TABLE_ADDR_1          0x0000001C
#define AR_PHY_RTT_SW_RTT_TABLE_ADDR_1_S        2
/* BB_rtt_table_sw_intf_1_b1 */
#define AR_PHY_RTT_SW_RTT_TABLE_DATA_1          0xFFFFFFF0
#define AR_PHY_RTT_SW_RTT_TABLE_DATA_1_S        4
/*
 * SM 1 Register Map for Poseidon
 */
#define AR_SM1_BASE_POSEIDON      offsetof(struct bb_reg_map, overlay_0xa800.Poseidon.bb_sm1_reg_map)
#define AR_SM1_OFFSET_POSEIDON(_x)   (AR_SM1_BASE_POSEIDON + offsetof(struct sm1_reg_map, _x))

#define AR_PHY_SWITCH_CHAIN_1_POSEIDON    AR_SM1_OFFSET_POSEIDON(BB_switch_table_chn_b1)

/*
 * Channel 2 Register Map
 */
#define AR_CHAN2_BASE      offsetof(struct bb_reg_map, overlay_0xa800.Osprey.bb_chn2_reg_map)
#define AR_CHAN2_OFFSET(_x)   (AR_CHAN2_BASE + offsetof(struct chn2_reg_map, _x))

#define AR_PHY_TIMING4_2            AR_CHAN2_OFFSET(BB_timing_control_4_b2)
#define AR_PHY_EXT_CCA_2            AR_CHAN2_OFFSET(BB_ext_chan_pwr_thr_2_b2)
#define QCN5500_TX_PREEMP_COEF_01_B2        AR_CHAN2_OFFSET(BB_tx_preemp_coef_01_b2)
#define QCN5500_TX_PREEMP_COEF_23_B2        AR_CHAN2_OFFSET(BB_tx_preemp_coef_23_b2)
#define QCN5500_TX_PREEMP_COEF_45_B2        AR_CHAN2_OFFSET(BB_tx_preemp_coef_45_b2)
#define QCN5500_TX_PREEMP_COEF_67_B2        AR_CHAN2_OFFSET(BB_tx_preemp_coef_67_b2)
#define QCN5500_TX_PREEMP_COEF_8_B2         AR_CHAN2_OFFSET(BB_tx_preemp_coef_8_b2)
#define QCN5500_TX_NOTCH_COEF_B2            AR_CHAN2_OFFSET(BB_tx_notch_coef_b2)
#define QCN5500_SPUR_FREQ_PHASE_B2          AR_CHAN2_OFFSET(BB_spur_freq_phase_b2)

#define AR_PHY_TX_PHASE_RAMP_2      AR_CHAN2_OFFSET(BB_tx_phase_ramp_b2)
#define AR_PHY_ADC_GAIN_DC_CORR_2   AR_CHAN2_OFFSET(BB_adc_gain_dc_corr_b2)
#define AR_PHY_IQ_ADC_MEAS_0_B2     AR_CHAN2_OFFSET(BB_iq_adc_meas_0_b2)
#define AR_PHY_IQ_ADC_MEAS_1_B2     AR_CHAN2_OFFSET(BB_iq_adc_meas_1_b2)
#define AR_PHY_IQ_ADC_MEAS_2_B2     AR_CHAN2_OFFSET(BB_iq_adc_meas_2_b2)
#define AR_PHY_IQ_ADC_MEAS_3_B2     AR_CHAN2_OFFSET(BB_iq_adc_meas_3_b2)
#define AR_PHY_SPUR_REPORT_2        AR_CHAN2_OFFSET(BB_spur_report_b2)
#define AR_PHY_CHAN_INFO_TAB_2      AR_CHAN2_OFFSET(BB_chan_info_chan_tab_b2)
#define AR_PHY_RX_IQCAL_CORR_B2     AR_CHAN2_OFFSET(BB_rx_iq_corr_b2)

/*
 * Channel 2 Field Definitions
 */
/* BB_ext_chan_pwr_thr_2_b2 */
#define AR_PHY_CH2_EXT_MINCCA_PWR   0x01FF0000
#define AR_PHY_CH2_EXT_MINCCA_PWR_S 16
#define AR_PHY_CF_MAXCCAPWR_EXT_2    0x000001FF
#define AR_PHY_CF_MAXCCAPWR_EXT_2_S  0
/*
 * AGC 2 Register Map
 */
#define AR_AGC2_BASE      offsetof(struct bb_reg_map, overlay_0xa800.Osprey.bb_agc2_reg_map)
#define AR_AGC2_OFFSET(_x)   (AR_AGC2_BASE + offsetof(struct agc2_reg_map, _x))

#define AR_PHY_FORCEMAX_GAINS_2      AR_AGC2_OFFSET(BB_gain_force_max_gains_b2)
#define AR_PHY_GAINS_MINOFF_2        AR_AGC2_OFFSET(BB_gains_min_offsets_b2)
#define AR_PHY_EXT_ATTEN_CTL_2       AR_AGC2_OFFSET(BB_ext_atten_switch_ctl_b2)
#define AR_PHY_CCA_2                 AR_AGC2_OFFSET(BB_cca_b2)
#define AR_PHY_CCA_CTRL_2            AR_AGC2_OFFSET(BB_cca_ctrl_2_b2)
#define AR_PHY_RSSI_2                AR_AGC2_OFFSET(BB_rssi_b2)
#define AR_PHY_SPUR_CCK_REP_2        AR_AGC2_OFFSET(BB_spur_est_cck_report_b2)

/*
 * AGC 2 Field Definitions
 */
/* BB_cca_b2 */
#define AR_PHY_CH2_MINCCA_PWR   0x1FF00000
#define AR_PHY_CH2_MINCCA_PWR_S 20
#define AR_PHY_CF_MAXCCAPWR_2       0x000001FF
#define AR_PHY_CF_MAXCCAPWR_2_S     0

/*
 * SM 2 Register Map
 */
#define AR_SM2_BASE      offsetof(struct bb_reg_map, overlay_0xa800.Osprey.bb_sm2_reg_map)
#define AR_SM2_OFFSET(_x)   (AR_SM2_BASE + offsetof(struct sm2_reg_map, _x))

#define AR_PHY_SWITCH_CHAIN_2    AR_SM2_OFFSET(BB_switch_table_chn_b2)
#define AR_PHY_FCAL_2_2          AR_SM2_OFFSET(BB_fcal_2_b2)
#define AR_PHY_DFT_TONE_CTL_2    AR_SM2_OFFSET(BB_dft_tone_ctrl_b2)
#define AR_PHY_CL_TAB_2          AR_SM2_OFFSET(BB_cl_tab_b2)
#define AR_PHY_CHAN_INFO_GAIN_2  AR_SM2_OFFSET(BB_chan_info_gain_b2)
#define AR_PHY_TPC_4_B2          AR_SM2_OFFSET(BB_tpc_4_b2)
#define AR_PHY_TPC_5_B2          AR_SM2_OFFSET(BB_tpc_5_b2)
#define AR_PHY_TPC_6_B2          AR_SM2_OFFSET(BB_tpc_6_b2)
#define AR_PHY_TPC_18_B2         AR_SM2_OFFSET(BB_tpc_18_b2)
#define AR_SCORPION_PHY_TPC_19_B2   AR_SM2_OFFSET(overlay_c440.Scorpion.BB_tpc_19_b2)
#define QCN5500_PHY_TPC_19_B2        AR_SM2_OFFSET(overlay_c440.Jet.BB_tpc_19_b2)
#define AR_PHY_PDADC_TAB_2          AR_SM2_OFFSET(overlay_c440.Osprey.BB_pdadc_tab_b2)
#define AR_PHY_TX_IQCAL_STATUS_B2   AR_SM2_OFFSET(BB_txiqcal_status_b2)
#define AR_PHY_TX_IQCAL_CORR_COEFF_01_B2    AR_SM2_OFFSET(BB_txiq_corr_coeff_01_b2)
#define AR_PHY_TX_IQCAL_CORR_COEFF_23_B2    AR_SM2_OFFSET(BB_txiq_corr_coeff_23_b2)
#define AR_PHY_TX_IQCAL_CORR_COEFF_45_B2    AR_SM2_OFFSET(BB_txiq_corr_coeff_45_b2)
#define AR_PHY_TX_IQCAL_CORR_COEFF_67_B2    AR_SM2_OFFSET(BB_txiq_corr_coeff_67_b2)
#define AR_PHY_CL_MAP_0_B2         AR_SM2_OFFSET(BB_cl_map_0_b2)
#define AR_PHY_CL_MAP_1_B2         AR_SM2_OFFSET(BB_cl_map_1_b2)
#define AR_PHY_CL_MAP_2_B2         AR_SM2_OFFSET(BB_cl_map_2_b2)
#define AR_PHY_CL_MAP_3_B2         AR_SM2_OFFSET(BB_cl_map_3_b2)


/*
 * JET : Channel 3 Register Map
 */
#define QCN5500_CHAN3_BASE      offsetof(struct extended_bb_reg_map, bb_chn3_ext_reg_map)
#define QCN5500_CHAN3_OFFSET(_x)   (QCN5500_CHAN3_BASE + offsetof(struct chn3_ext_reg_map, _x))

#define QCN5500_PHY_EXT_CCA_3               QCN5500_CHAN3_OFFSET(BB_ext_chan_pwr_thr_2_b3)
#define AR_PHY_CF_MAXCCAPWR_EXT_3    0x000001FF
#define AR_PHY_CF_MAXCCAPWR_EXT_3_S  0
#define AR_PHY_CF_MAXCCAPWR_3       0x000001FF
#define AR_PHY_CF_MAXCCAPWR_3_S     0

#define QCN5500_PHY_IQ_ADC_MEAS_0_B3        QCN5500_CHAN3_OFFSET(BB_iq_adc_meas_0_b3)
#define QCN5500_PHY_IQ_ADC_MEAS_1_B3        QCN5500_CHAN3_OFFSET(BB_iq_adc_meas_1_b3)
#define QCN5500_PHY_IQ_ADC_MEAS_2_B3        QCN5500_CHAN3_OFFSET(BB_iq_adc_meas_2_b3)
#define QCN5500_PHY_IQ_ADC_MEAS_3_B3        QCN5500_CHAN3_OFFSET(BB_iq_adc_meas_3_b3)

#define QCN5500_PHY_CHAN_INFO_TAB_3         QCN5500_CHAN3_OFFSET(BB_chan_info_chan_tab_b3)
#define QCN5500_PHY_RX_IQCAL_CORR_B3        QCN5500_CHAN3_OFFSET(BB_rx_iq_corr_b3)

/*
 * bb_chn_ext_reg_map
 */
#define AR_CHN_EXT_BASE_POSEIDON      offsetof(struct bb_reg_map, overlay_0xa800.Poseidon.bb_chn_ext_reg_map) 
#define AR_CHN_EXT_OFFSET_POSEIDON(_x)   (AR_CHN_EXT_BASE_POSEIDON + offsetof(struct chn_ext_reg_map, _x)) 

#define AR_PHY_PAPRD_VALID_OBDB_POSEIDON    AR_CHN_EXT_OFFSET_POSEIDON(BB_paprd_valid_obdb_b0)
#define AR_PHY_PAPRD_VALID_OBDB_0    0x3f
#define AR_PHY_PAPRD_VALID_OBDB_0_S  0
#define AR_PHY_PAPRD_VALID_OBDB_1    0x3f
#define AR_PHY_PAPRD_VALID_OBDB_1_S  6
#define AR_PHY_PAPRD_VALID_OBDB_2    0x3f
#define AR_PHY_PAPRD_VALID_OBDB_2_S  12
#define AR_PHY_PAPRD_VALID_OBDB_3    0x3f
#define AR_PHY_PAPRD_VALID_OBDB_3_S  18
#define AR_PHY_PAPRD_VALID_OBDB_4    0x3f
#define AR_PHY_PAPRD_VALID_OBDB_4_S  24

/* BB_txiqcal_status_b1 */
#define AR_PHY_TX_IQCAL_STATUS_B2_FAILED    0x00000001

/*
 * AGC 3 Register Map
 */
#define AR_AGC3_BASE      offsetof(struct bb_reg_map, overlay_0xc800.Osprey.bb_agc3_reg_map)
#define AR_AGC3_OFFSET(_x)   (AR_AGC3_BASE + offsetof(struct agc3_reg_map, _x))
#define QCN5500_AGC3_BASE         offsetof(struct extended_bb_reg_map, bb_agc3_ext_reg_map)
#define QCN5500_AGC3_OFFSET(_x)   (QCN5500_AGC3_BASE + offsetof(struct agc3_ext_reg_map, _x))

#define AR_PHY_FORCEMAX_GAINS_3      AR_AGC3_OFFSET(BB_gain_force_max_gains_b3)
#define QCN5500_PHY_EXT_ATTEN_CTL_3  QCN5500_AGC3_OFFSET(BB_ext_atten_switch_ctl_b3)
#define QCN5500_PHY_CCA_3            QCN5500_AGC3_OFFSET(BB_cca_b3)
#define QCN5500_PHY_CCA_CTRL_3       QCN5500_AGC3_OFFSET(BB_cca_ctrl_2_b3)
#define AR_PHY_RSSI_3                AR_AGC3_OFFSET(BB_rssi_b3)
#define QCN5500_PHY_RSSI_3           QCN5500_AGC3_OFFSET(BB_rssi_b3)
/*
 * SM 3 Register Map
 */

#define QCN5500_SM3_BASE            offsetof(struct extended_bb_reg_map, bb_sm3_ext_reg_map)
#define QCN5500_SM3_OFFSET(_x)      (QCN5500_SM3_BASE + offsetof(struct sm3_ext_reg_map, _x))

#define QCN5500_PHY_SWITCH_CHAIN_3    QCN5500_SM3_OFFSET(BB_switch_table_chn_b3)
#define AR_PHY_FCAL_2_3               QCN5500_SM3_OFFSET(BB_fcal_2_b3)
#define QCN5500_PHY_TPC_11_B3         QCN5500_SM3_OFFSET(BB_tpc_11_b3)
#define QCN5500_PHY_TPC_6_B3          QCN5500_SM3_OFFSET(BB_tpc_6_b3)
#define QCN5500_PHY_TPC_18_B3         QCN5500_SM3_OFFSET(BB_tpc_18_b3)
#define QCN5500_PHY_TPC_19_B3         QCN5500_SM3_OFFSET(BB_tpc_19_b3)
#define QCN5500_PHY_CL_TAB_3          QCN5500_SM3_OFFSET(BB_cl_tab_b3)
#define QCN5500_PHY_TX_IQCAL_CORR_COEFF_01_B3    QCN5500_SM3_OFFSET(BB_txiq_corr_coeff_01_b3)
#define QCN5500_PHY_TX_IQCAL_CORR_COEFF_23_B3    QCN5500_SM3_OFFSET(BB_txiq_corr_coeff_23_b3)
#define QCN5500_PHY_TX_IQCAL_CORR_COEFF_45_B3    QCN5500_SM3_OFFSET(BB_txiq_corr_coeff_45_b3)
#define QCN5500_PHY_TX_IQCAL_CORR_COEFF_67_B3    QCN5500_SM3_OFFSET(BB_txiq_corr_coeff_67_b3)
#define QCN5500_PHY_TX_IQCAL_STATUS_B3      QCN5500_SM3_OFFSET(BB_txiqcal_status_b3)

#define QCN5500_PHY_CL_TAB_3_CL_GAIN_MOD               0x1F
#define QCN5500_PHY_CL_TAB_3_CL_GAIN_MOD_S         0
#define QCN5500_PHY_CL_MAP_0_B3         QCN5500_SM3_OFFSET(BB_cl_map_0_b3)
#define QCN5500_PHY_CL_MAP_1_B3         QCN5500_SM3_OFFSET(BB_cl_map_1_b3)
#define QCN5500_PHY_CL_MAP_2_B3         QCN5500_SM3_OFFSET(BB_cl_map_2_b3)
#define QCN5500_PHY_CL_MAP_3_B3         QCN5500_SM3_OFFSET(BB_cl_map_3_b3)

/*
 * AGC 2 Field Definitions
 */
/* BB_cca_b2 */
#define AR_PHY_CH3_MINCCA_PWR   0x1FF00000
#define AR_PHY_CH3_MINCCA_PWR_S 20
/*
 * Misc helper defines
 */
#define AR_PHY_CHAIN_OFFSET     (AR_CHAN1_BASE - AR_CHAN_BASE)

#define AR_PHY_NEW_ADC_DC_GAIN_CORR(_i) (AR_PHY_ADC_GAIN_DC_CORR_0 + (AR_PHY_CHAIN_OFFSET * (_i)))
#define AR_PHY_SWITCH_CHAIN(_i)     (AR_PHY_SWITCH_CHAIN_0 + (AR_PHY_CHAIN_OFFSET * (_i)))
#define AR_PHY_EXT_ATTEN_CTL(_i)    (AR_PHY_EXT_ATTEN_CTL_0 + (AR_PHY_CHAIN_OFFSET * (_i)))

#define AR_PHY_RXGAIN(_i)           (AR_PHY_FORCEMAX_GAINS_0 + (AR_PHY_CHAIN_OFFSET * (_i)))
#define AR_PHY_TPCRG5(_i)           (AR_PHY_TPC_5_B0 + (AR_PHY_CHAIN_OFFSET * (_i)))
#define AR_PHY_PDADC_TAB(_i)        (AR_PHY_PDADC_TAB_0 + (AR_PHY_CHAIN_OFFSET * (_i)))

#define AR_PHY_CAL_MEAS_0(_i)       (AR_PHY_IQ_ADC_MEAS_0_B0 + (AR_PHY_CHAIN_OFFSET * (_i)))
#define AR_PHY_CAL_MEAS_1(_i)       (AR_PHY_IQ_ADC_MEAS_1_B0 + (AR_PHY_CHAIN_OFFSET * (_i)))
#define AR_PHY_CAL_MEAS_2(_i)       (AR_PHY_IQ_ADC_MEAS_2_B0 + (AR_PHY_CHAIN_OFFSET * (_i)))
#define AR_PHY_CAL_MEAS_3(_i)       (AR_PHY_IQ_ADC_MEAS_3_B0 + (AR_PHY_CHAIN_OFFSET * (_i)))

#define AR_PHY_CHIP_ID          0x9818      /* PHY chip revision ID */
#define AR_PHY_CHIP_ID_REV_0        0x80 /* 5416 Rev 0 (owl 1.0) BB */
#define AR_PHY_CHIP_ID_REV_1        0x81 /* 5416 Rev 1 (owl 2.0) BB */
#define AR_PHY_CHIP_ID_SOWL_REV_0       0xb0 /* 9160 Rev 0 (sowl 1.0) BB */

/* BB Panic Watchdog control register 1 */
#define AR_PHY_BB_PANIC_NON_IDLE_ENABLE 0x00000001
#define AR_PHY_BB_PANIC_IDLE_ENABLE     0x00000002
#define AR_PHY_BB_PANIC_IDLE_MASK       0xFFFF0000
#define AR_PHY_BB_PANIC_NON_IDLE_MASK   0x0000FFFC
/* BB Panic Watchdog control register 2 */
#define AR_PHY_BB_PANIC_RST_ENABLE      0x00000002
#define AR_PHY_BB_PANIC_IRQ_ENABLE      0x00000004
#define AR_PHY_BB_PANIC_CNTL2_MASK      0xFFFFFFF9
/* BB Panic Watchdog status register */
#define AR_PHY_BB_WD_STATUS             0x00000007 /* snapshot of r_panic_watchdog_sm */
#define AR_PHY_BB_WD_STATUS_S           0
#define AR_PHY_BB_WD_DET_HANG           0x00000008 /* panic_watchdog_det_hang */
#define AR_PHY_BB_WD_DET_HANG_S         3
#define AR_PHY_BB_WD_RADAR_SM           0x000000F0 /* snapshot of radar state machine r_rdr_sm */
#define AR_PHY_BB_WD_RADAR_SM_S         4
#define AR_PHY_BB_WD_RX_OFDM_SM         0x00000F00 /* snapshot of rx state machine (OFDM) r_rx_sm */
#define AR_PHY_BB_WD_RX_OFDM_SM_S       8
#define AR_PHY_BB_WD_RX_CCK_SM          0x0000F000 /* snapshot of rx state machine (CCK) r_rx_sm_cck */
#define AR_PHY_BB_WD_RX_CCK_SM_S        12
#define AR_PHY_BB_WD_TX_OFDM_SM         0x000F0000 /* snapshot of tx state machine (OFDM) r_tx_sm */
#define AR_PHY_BB_WD_TX_OFDM_SM_S       16
#define AR_PHY_BB_WD_TX_CCK_SM          0x00F00000 /* snapshot of tx state machine (CCK) r_tx_sm_cck */
#define AR_PHY_BB_WD_TX_CCK_SM_S        20
#define AR_PHY_BB_WD_AGC_SM             0x0F000000 /* snapshot of AGC state machine r_agc_sm */
#define AR_PHY_BB_WD_AGC_SM_S           24
#define AR_PHY_BB_WD_SRCH_SM            0xF0000000 /* snapshot of agc search state machine r_srch_sm */
#define AR_PHY_BB_WD_SRCH_SM_S          28

#define AR_PHY_BB_WD_STATUS_CLR         0x00000008 /* write 0 to reset watchdog */


/***** PAPRD *****/
#define AR_PHY_PAPRD_AM2AM                          AR_CHAN_OFFSET(BB_paprd_am2am_mask) 
#define AR_PHY_PAPRD_AM2AM_MASK                     0x01ffffff     
#define AR_PHY_PAPRD_AM2AM_MASK_S                   0

#define AR_PHY_PAPRD_AM2PM                          AR_CHAN_OFFSET(BB_paprd_am2pm_mask) 
#define AR_PHY_PAPRD_AM2PM_MASK                     0x01ffffff     
#define AR_PHY_PAPRD_AM2PM_MASK_S                   0

#define AR_PHY_PAPRD_HT40                           AR_CHAN_OFFSET(BB_paprd_ht40_mask) 
#define AR_PHY_PAPRD_HT40_MASK                      0x01ffffff     
#define AR_PHY_PAPRD_HT40_MASK_S                    0

#define AR_PHY_PAPRD_4SS_MASK                       AR_CHAN_OFFSET(BB_paprd_4ss_mask_ctrl)
#define AR_PHY_PAPRD_AM2AM_4SS_MASK                 0xFF
#define AR_PHY_PAPRD_AM2AM_4SS_MASK_S               0
#define AR_PHY_PAPRD_AM2PM_4SS_MASK                 0xFF
#define AR_PHY_PAPRD_AM2PM_4SS_MASK_S               8
#define AR_PHY_PAPRD_HT40_4SS_MASK                  0xFF
#define AR_PHY_PAPRD_HT40_4SS_MASK_S                16

#define AR_PHY_PAPRD_CTRL0_B0                       AR_CHAN_OFFSET(BB_paprd_ctrl0_b0)
#define AR_PHY_PAPRD_CTRL0_B0_PAPRD_ENABLE_0			1
#define AR_PHY_PAPRD_CTRL0_B0_PAPRD_ENABLE_0_S			0
#define AR_PHY_PAPRD_CTRL0_B0_USE_SINGLE_TABLE_MASK     0x00000001
#define AR_PHY_PAPRD_CTRL0_B0_USE_SINGLE_TABLE_MASK_S   0x00000001
#define AR_PHY_PAPRD_CTRL0_B0_PAPRD_MAG_THRSH_0			0x1F
#define AR_PHY_PAPRD_CTRL0_B0_PAPRD_MAG_THRSH_0_S		27

#define AR_PHY_PAPRD_CTRL1_B0                       AR_CHAN_OFFSET(BB_paprd_ctrl1_b0)
#define AR_PHY_PAPRD_CTRL1_B0_PAPRD_POWER_AT_AM2AM_CAL_0	0x3f
#define AR_PHY_PAPRD_CTRL1_B0_PAPRD_POWER_AT_AM2AM_CAL_0_S	3
#define AR_PHY_PAPRD_CTRL1_B0_ADAPTIVE_AM2PM_ENABLE_0		1
#define AR_PHY_PAPRD_CTRL1_B0_ADAPTIVE_AM2PM_ENABLE_0_S		2
#define AR_PHY_PAPRD_CTRL1_B0_ADAPTIVE_AM2AM_ENABLE_0		1
#define AR_PHY_PAPRD_CTRL1_B0_ADAPTIVE_AM2AM_ENABLE_0_S		1
#define AR_PHY_PAPRD_CTRL1_B0_ADAPTIVE_SCALING_ENA		1
#define AR_PHY_PAPRD_CTRL1_B0_ADAPTIVE_SCALING_ENA_S		0
#define AR_PHY_PAPRD_CTRL1_B0_PA_GAIN_SCALE_FACT_0_MASK       0xFF
#define AR_PHY_PAPRD_CTRL1_B0_PA_GAIN_SCALE_FACT_0_MASK_S     9
#define AR_PHY_PAPRD_CTRL1_B0_PAPRD_MAG_SCALE_FACT_0		0x7FF
#define AR_PHY_PAPRD_CTRL1_B0_PAPRD_MAG_SCALE_FACT_0_S		17

#define AR_PHY_PAPRD_CTRL0_B1                       AR_CHAN1_OFFSET(BB_paprd_ctrl0_b1) 
#define AR_PHY_PAPRD_CTRL0_B1_PAPRD_MAG_THRSH_1				0x1F
#define AR_PHY_PAPRD_CTRL0_B1_PAPRD_MAG_THRSH_1_S			27
#define AR_PHY_PAPRD_CTRL0_B1_PAPRD_ADAPTIVE_USE_SINGLE_TABLE_1		1
#define AR_PHY_PAPRD_CTRL0_B1_PAPRD_ADAPTIVE_USE_SINGLE_TABLE_1_S	1
#define AR_PHY_PAPRD_CTRL0_B1_PAPRD_ENABLE_1				1
#define AR_PHY_PAPRD_CTRL0_B1_PAPRD_ENABLE_1_S				0

#define AR_PHY_PAPRD_CTRL1_B1                       AR_CHAN1_OFFSET(BB_paprd_ctrl1_b1) 
#define AR_PHY_PAPRD_CTRL1_B1_PAPRD_POWER_AT_AM2AM_CAL_1	0x3f
#define AR_PHY_PAPRD_CTRL1_B1_PAPRD_POWER_AT_AM2AM_CAL_1_S	3
#define AR_PHY_PAPRD_CTRL1_B1_ADAPTIVE_AM2PM_ENABLE_1		1
#define AR_PHY_PAPRD_CTRL1_B1_ADAPTIVE_AM2PM_ENABLE_1_S		2
#define AR_PHY_PAPRD_CTRL1_B1_ADAPTIVE_AM2AM_ENABLE_1		1
#define AR_PHY_PAPRD_CTRL1_B1_ADAPTIVE_AM2AM_ENABLE_1_S		1
#define AR_PHY_PAPRD_CTRL1_B1_ADAPTIVE_SCALING_ENA		1
#define AR_PHY_PAPRD_CTRL1_B1_ADAPTIVE_SCALING_ENA_S		0
#define AR_PHY_PAPRD_CTRL1_B1_PA_GAIN_SCALE_FACT_1_MASK       0xFF
#define AR_PHY_PAPRD_CTRL1_B1_PA_GAIN_SCALE_FACT_1_MASK_S     9
#define AR_PHY_PAPRD_CTRL1_B1_PAPRD_MAG_SCALE_FACT_1		0x7FF
#define AR_PHY_PAPRD_CTRL1_B1_PAPRD_MAG_SCALE_FACT_1_S		17

#define AR_PHY_PAPRD_CTRL0_B2                       AR_CHAN2_OFFSET(BB_paprd_ctrl0_b2) 
#define AR_PHY_PAPRD_CTRL0_B2_PAPRD_MAG_THRSH_2				0x1F
#define AR_PHY_PAPRD_CTRL0_B2_PAPRD_MAG_THRSH_2_S			27
#define AR_PHY_PAPRD_CTRL0_B2_PAPRD_ADAPTIVE_USE_SINGLE_TABLE_2		1
#define AR_PHY_PAPRD_CTRL0_B2_PAPRD_ADAPTIVE_USE_SINGLE_TABLE_2_S	1
#define AR_PHY_PAPRD_CTRL0_B2_PAPRD_ENABLE_2				1
#define AR_PHY_PAPRD_CTRL0_B2_PAPRD_ENABLE_2_S				0

#define AR_PHY_PAPRD_CTRL1_B2                       AR_CHAN2_OFFSET(BB_paprd_ctrl1_b2) 
#define AR_PHY_PAPRD_CTRL1_B2_PAPRD_POWER_AT_AM2AM_CAL_2	0x3f
#define AR_PHY_PAPRD_CTRL1_B2_PAPRD_POWER_AT_AM2AM_CAL_2_S	3
#define AR_PHY_PAPRD_CTRL1_B2_ADAPTIVE_AM2PM_ENABLE_2		1
#define AR_PHY_PAPRD_CTRL1_B2_ADAPTIVE_AM2PM_ENABLE_2_S		2
#define AR_PHY_PAPRD_CTRL1_B2_ADAPTIVE_AM2AM_ENABLE_2		1
#define AR_PHY_PAPRD_CTRL1_B2_ADAPTIVE_AM2AM_ENABLE_2_S		1
#define AR_PHY_PAPRD_CTRL1_B2_ADAPTIVE_SCALING_ENA		1
#define AR_PHY_PAPRD_CTRL1_B2_ADAPTIVE_SCALING_ENA_S		0
#define AR_PHY_PAPRD_CTRL1_B2_PA_GAIN_SCALE_FACT_2_MASK       0xFF
#define AR_PHY_PAPRD_CTRL1_B2_PA_GAIN_SCALE_FACT_2_MASK_S     9
#define AR_PHY_PAPRD_CTRL1_B2_PAPRD_MAG_SCALE_FACT_2		0x7FF
#define AR_PHY_PAPRD_CTRL1_B2_PAPRD_MAG_SCALE_FACT_2_S		17

#define QCN5500_PHY_PAPRD_CTRL0_B3                  QCN5500_CHAN3_OFFSET(BB_paprd_ctrl0_b3)
#define QCN5500_PHY_PAPRD_CTRL0_B3_PAPRD_MAG_THRSH_3             0x1F
#define QCN5500_PHY_PAPRD_CTRL0_B3_PAPRD_MAG_THRSH_3_S           27
#define QCN5500_PHY_PAPRD_CTRL0_B3_PAPRD_ADAPTIVE_USE_SINGLE_TABLE_3     1
#define QCN5500_PHY_PAPRD_CTRL0_B3_PAPRD_ADAPTIVE_USE_SINGLE_TABLE_3_S   1
#define QCN5500_PHY_PAPRD_CTRL0_B3_PAPRD_ENABLE_3                1
#define QCN5500_PHY_PAPRD_CTRL0_B3_PAPRD_ENABLE_3_S              0

#define QCN5500_PHY_PAPRD_CTRL1_B3              QCN5500_CHAN3_OFFSET(BB_paprd_ctrl1_b3)
#define QCN5500_PHY_PAPRD_CTRL1_B3_ADAPTIVE_AM2PM_ENABLE_3       1
#define QCN5500_PHY_PAPRD_CTRL1_B3_ADAPTIVE_AM2PM_ENABLE_3_S     2
#define QCN5500_PHY_PAPRD_CTRL1_B3_ADAPTIVE_AM2AM_ENABLE_3       1
#define QCN5500_PHY_PAPRD_CTRL1_B3_ADAPTIVE_AM2AM_ENABLE_3_S     1
#define QCN5500_PHY_PAPRD_CTRL1_B3_ADAPTIVE_SCALING_ENA      1
#define QCN5500_PHY_PAPRD_CTRL1_B3_ADAPTIVE_SCALING_ENA_S        0
#define QCN5500_PHY_PAPRD_CTRL1_B3_PA_GAIN_SCALE_FACT_3_MASK       0xFF
#define QCN5500_PHY_PAPRD_CTRL1_B3_PA_GAIN_SCALE_FACT_3_MASK_S     9
#define QCN5500_PHY_PAPRD_CTRL1_B3_PAPRD_MAG_SCALE_FACT_3        0x7FF
#define QCN5500_PHY_PAPRD_CTRL1_B3_PAPRD_MAG_SCALE_FACT_3_S      17

#define AR_PHY_PAPRD_TRAINER_CNTL1                 AR_SM_OFFSET(overlay_0xa580.Osprey.BB_paprd_trainer_cntl1)
#define AR_PHY_PAPRD_TRAINER_CNTL1_POSEIDON        AR_SM_OFFSET(overlay_0xa580.Poseidon.BB_paprd_trainer_cntl1)
#define AR_PHY_PAPRD_TRAINER_CNTL1_JUPITER20        AR_SM_OFFSET(overlay_0xa580.Jupiter_20.BB_paprd_trainer_cntl1)
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_COARSE_GAIN_32			0x1
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_COARSE_GAIN_32_S		27
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_FORCED_FINE_IDX			0x3
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_FORCED_FINE_IDX_S		25
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_FORCED_COARSE_IDX		0x1F
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_FORCED_COARSE_IDX_S		20
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_CORR_FORCE				0x1
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_CORR_FORCE_S			19
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_LB_SKIP			0x7f
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_LB_SKIP_S		12
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_DF_LB_SKIP		0x7ff
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_DF_LB_SKIP_S	12
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_LB_ENABLE		1
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_LB_ENABLE_S		11
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_TX_GAIN_FORCE	1
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_TX_GAIN_FORCE_S	10
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_RX_BB_GAIN_FORCE	1
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_RX_BB_GAIN_FORCE_S	9
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_IQCORR_ENABLE		1
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_IQCORR_ENABLE_S		8
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_AGC2_SETTLING		0x3F
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_PAPRD_AGC2_SETTLING_S		1
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_CF_PAPRD_TRAIN_ENABLE		1
#define AR_PHY_PAPRD_TRAINER_CNTL1_CF_CF_PAPRD_TRAIN_ENABLE_S	0

#define AR_PHY_PAPRD_TRAINER_CNTL2                 AR_SM_OFFSET(overlay_0xa580.Osprey.BB_paprd_trainer_cntl2)
#define AR_PHY_PAPRD_TRAINER_CNTL2_POSEIDON        AR_SM_OFFSET(overlay_0xa580.Poseidon.BB_paprd_trainer_cntl2)
#define AR_PHY_PAPRD_TRAINER_CNTL2_JUPITER20        AR_SM_OFFSET(overlay_0xa580.Jupiter_20.BB_paprd_trainer_cntl2)

#define AR_PHY_PAPRD_TRAINER_CNTL2_CF_PAPRD_INIT_RX_BB_GAIN		0xFFFFFFFF
#define AR_PHY_PAPRD_TRAINER_CNTL2_CF_PAPRD_INIT_RX_BB_GAIN_S	0

#define AR_PHY_PAPRD_TRAINER_CNTL3                 AR_SM_OFFSET(overlay_0xa580.Osprey.BB_paprd_trainer_cntl3)
#define AR_PHY_PAPRD_TRAINER_CNTL3_POSEIDON        AR_SM_OFFSET(overlay_0xa580.Poseidon.BB_paprd_trainer_cntl3)
#define AR_PHY_PAPRD_TRAINER_CNTL3_JUPITER20        AR_SM_OFFSET(overlay_0xa580.Jupiter_20.BB_paprd_trainer_cntl3)

#define AR_PHY_PAPRD_TRAINER_CNTL3_CF_PAPRD_BBTXMIX_DISABLE		1
#define AR_PHY_PAPRD_TRAINER_CNTL3_CF_PAPRD_BBTXMIX_DISABLE_S	29
#define AR_PHY_PAPRD_TRAINER_CNTL3_CF_PAPRD_FINE_CORR_LEN		0xF
#define AR_PHY_PAPRD_TRAINER_CNTL3_CF_PAPRD_FINE_CORR_LEN_S		24
#define AR_PHY_PAPRD_TRAINER_CNTL3_CF_PAPRD_COARSE_CORR_LEN		0xF
#define AR_PHY_PAPRD_TRAINER_CNTL3_CF_PAPRD_COARSE_CORR_LEN_S	20
#define AR_PHY_PAPRD_TRAINER_CNTL3_CF_PAPRD_COARSE_CORR_LEN		0xF
#define AR_PHY_PAPRD_TRAINER_CNTL3_CF_PAPRD_COARSE_CORR_LEN_S	20
#define AR_PHY_PAPRD_TRAINER_CNTL3_CF_PAPRD_NUM_CORR_STAGES		0x7
#define AR_PHY_PAPRD_TRAINER_CNTL3_CF_PAPRD_NUM_CORR_STAGES_S	17
#define AR_PHY_PAPRD_TRAINER_CNTL3_CF_PAPRD_MIN_LOOPBACK_DEL	0x1F
#define AR_PHY_PAPRD_TRAINER_CNTL3_CF_PAPRD_MIN_LOOPBACK_DEL_S	12
#define AR_PHY_PAPRD_TRAINER_CNTL3_CF_PAPRD_QUICK_DROP			0x3F
#define AR_PHY_PAPRD_TRAINER_CNTL3_CF_PAPRD_QUICK_DROP_S		6
#define AR_PHY_PAPRD_TRAINER_CNTL3_CF_PAPRD_ADC_DESIRED_SIZE	0x3F
#define AR_PHY_PAPRD_TRAINER_CNTL3_CF_PAPRD_ADC_DESIRED_SIZE_S	0

#define AR_PHY_PAPRD_TRAINER_CNTL4					AR_SM_OFFSET(overlay_0xa580.Osprey.BB_paprd_trainer_cntl4)
#define AR_PHY_PAPRD_TRAINER_CNTL4_POSEIDON         AR_SM_OFFSET(overlay_0xa580.Poseidon.BB_paprd_trainer_cntl4)
#define AR_PHY_PAPRD_TRAINER_CNTL4_JUPITER20         AR_SM_OFFSET(overlay_0xa580.Jupiter_20.BB_paprd_trainer_cntl4)
#define AR_PHY_PAPRD_TRAINER_CNTL4_CF_PAPRD_NUM_TRAIN_SAMPLES	0x3FF
#define AR_PHY_PAPRD_TRAINER_CNTL4_CF_PAPRD_NUM_TRAIN_SAMPLES_S	16
#define AR_PHY_PAPRD_TRAINER_CNTL4_CF_PAPRD_SAFETY_DELTA		0xF
#define AR_PHY_PAPRD_TRAINER_CNTL4_CF_PAPRD_SAFETY_DELTA_S		12
#define AR_PHY_PAPRD_TRAINER_CNTL4_CF_PAPRD_MIN_CORR			0xFFF
#define AR_PHY_PAPRD_TRAINER_CNTL4_CF_PAPRD_MIN_CORR_S			0

#define	AR_PHY_PAPRD_PRE_POST_SCALE_0_B0			AR_CHAN_OFFSET(BB_paprd_pre_post_scale_0_b0)
#define AR_PHY_PAPRD_PRE_POST_SCALE_0_B0_PAPRD_PRE_POST_SCALING_0_0		0x3FFFF
#define AR_PHY_PAPRD_PRE_POST_SCALE_0_B0_PAPRD_PRE_POST_SCALING_0_0_S	0

#define	AR_PHY_PAPRD_PRE_POST_SCALE_1_B0			AR_CHAN_OFFSET(BB_paprd_pre_post_scale_1_b0)
#define AR_PHY_PAPRD_PRE_POST_SCALE_1_B0_PAPRD_PRE_POST_SCALING_1_0		0x3FFFF
#define AR_PHY_PAPRD_PRE_POST_SCALE_1_B0_PAPRD_PRE_POST_SCALING_1_0_S	0

#define	AR_PHY_PAPRD_PRE_POST_SCALE_2_B0			AR_CHAN_OFFSET(BB_paprd_pre_post_scale_2_b0)
#define AR_PHY_PAPRD_PRE_POST_SCALE_2_B0_PAPRD_PRE_POST_SCALING_2_0		0x3FFFF
#define AR_PHY_PAPRD_PRE_POST_SCALE_2_B0_PAPRD_PRE_POST_SCALING_2_0_S	0

#define AR_PHY_PAPRD_PRE_POST_SCALE_3_B0			AR_CHAN_OFFSET(BB_paprd_pre_post_scale_3_b0)
#define AR_PHY_PAPRD_PRE_POST_SCALE_3_B0_PAPRD_PRE_POST_SCALING_3_0		0x3FFFF
#define AR_PHY_PAPRD_PRE_POST_SCALE_3_B0_PAPRD_PRE_POST_SCALING_3_0_S	0

#define AR_PHY_PAPRD_PRE_POST_SCALE_4_B0			AR_CHAN_OFFSET(BB_paprd_pre_post_scale_4_b0)
#define AR_PHY_PAPRD_PRE_POST_SCALE_4_B0_PAPRD_PRE_POST_SCALING_4_0		0x3FFFF
#define AR_PHY_PAPRD_PRE_POST_SCALE_4_B0_PAPRD_PRE_POST_SCALING_4_0_S	0

#define AR_PHY_PAPRD_PRE_POST_SCALE_5_B0			AR_CHAN_OFFSET(BB_paprd_pre_post_scale_5_b0)
#define AR_PHY_PAPRD_PRE_POST_SCALE_5_B0_PAPRD_PRE_POST_SCALING_5_0		0x3FFFF
#define AR_PHY_PAPRD_PRE_POST_SCALE_5_B0_PAPRD_PRE_POST_SCALING_5_0_S	0

#define AR_PHY_PAPRD_PRE_POST_SCALE_6_B0			AR_CHAN_OFFSET(BB_paprd_pre_post_scale_6_b0)
#define AR_PHY_PAPRD_PRE_POST_SCALE_6_B0_PAPRD_PRE_POST_SCALING_6_0		0x3FFFF
#define AR_PHY_PAPRD_PRE_POST_SCALE_6_B0_PAPRD_PRE_POST_SCALING_6_0_S	0

#define AR_PHY_PAPRD_PRE_POST_SCALE_7_B0			AR_CHAN_OFFSET(BB_paprd_pre_post_scale_7_b0)
#define AR_PHY_PAPRD_PRE_POST_SCALE_7_B0_PAPRD_PRE_POST_SCALING_7_0		0x3FFFF
#define AR_PHY_PAPRD_PRE_POST_SCALE_7_B0_PAPRD_PRE_POST_SCALING_7_0_S	0

#define AR_PHY_PAPRD_TRAINER_STAT1					AR_SM_OFFSET(overlay_0xa580.Osprey.BB_paprd_trainer_stat1)
#define AR_PHY_PAPRD_TRAINER_STAT1_POSEIDON			AR_SM_OFFSET(overlay_0xa580.Poseidon.BB_paprd_trainer_stat1)

#define AR_PHY_PAPRD_TRAINER_STAT1_PAPRD_AGC2_PWR			0xff
#define AR_PHY_PAPRD_TRAINER_STAT1_PAPRD_AGC2_PWR_S			9		
#define AR_PHY_PAPRD_TRAINER_STAT1_PAPRD_RX_GAIN_IDX		0x1f
#define AR_PHY_PAPRD_TRAINER_STAT1_PAPRD_RX_GAIN_IDX_S		4		
#define AR_PHY_PAPRD_TRAINER_STAT1_PAPRD_TRAIN_ACTIVE		0x1
#define AR_PHY_PAPRD_TRAINER_STAT1_PAPRD_TRAIN_ACTIVE_S		3
#define AR_PHY_PAPRD_TRAINER_STAT1_PAPRD_CORR_ERR			0x1
#define AR_PHY_PAPRD_TRAINER_STAT1_PAPRD_CORR_ERR_S			2
#define AR_PHY_PAPRD_TRAINER_STAT1_PAPRD_TRAIN_INCOMPLETE	0x1
#define AR_PHY_PAPRD_TRAINER_STAT1_PAPRD_TRAIN_INCOMPLETE_S 1
#define AR_PHY_PAPRD_TRAINER_STAT1_PAPRD_TRAIN_DONE			1
#define AR_PHY_PAPRD_TRAINER_STAT1_PAPRD_TRAIN_DONE_S		0

#define AR_PHY_PAPRD_TRAINER_STAT2					AR_SM_OFFSET(overlay_0xa580.Osprey.BB_paprd_trainer_stat2)
#define AR_PHY_PAPRD_TRAINER_STAT2_POSEIDON			AR_SM_OFFSET(overlay_0xa580.Poseidon.BB_paprd_trainer_stat2)
#define AR_PHY_PAPRD_TRAINER_STAT2_JUPITER20			AR_SM_OFFSET(overlay_0xa580.Jupiter_20.BB_paprd_trainer_stat2)

#define AR_PHY_PAPRD_TRAINER_STAT2_PAPRD_FINE_IDX			0x3	
#define AR_PHY_PAPRD_TRAINER_STAT2_PAPRD_FINE_IDX_S			21
#define AR_PHY_PAPRD_TRAINER_STAT2_PAPRD_COARSE_IDX			0x1F	
#define AR_PHY_PAPRD_TRAINER_STAT2_PAPRD_COARSE_IDX_S		16	
#define AR_PHY_PAPRD_TRAINER_STAT2_PAPRD_FINE_VAL			0xffff
#define AR_PHY_PAPRD_TRAINER_STAT2_PAPRD_FINE_VAL_S			0

#define AR_PHY_PAPRD_TRAINER_STAT3					AR_SM_OFFSET(overlay_0xa580.Osprey.BB_paprd_trainer_stat3)
#define AR_PHY_PAPRD_TRAINER_STAT3_POSEIDON			AR_SM_OFFSET(overlay_0xa580.Poseidon.BB_paprd_trainer_stat3)
#define AR_PHY_PAPRD_TRAINER_STAT3_JUPITER20		AR_SM_OFFSET(overlay_0xa580.Jupiter_20.BB_paprd_trainer_stat3)
#define AR_PHY_PAPRD_TRAINER_STAT3_PAPRD_TRAIN_SAMPLES_CNT		0xfffff
#define AR_PHY_PAPRD_TRAINER_STAT3_PAPRD_TRAIN_SAMPLES_CNT_S	0

#define AR_PHY_TPC_12				AR_SM_OFFSET(BB_tpc_12)
#define AR_PHY_TPC_12_DESIRED_SCALE_HT40_5				0x1F
#define AR_PHY_TPC_12_DESIRED_SCALE_HT40_5_S			25

#define AR_PHY_TPC_19_B0_ALT_ALPHA_VOLT_0           0x7f 
#define AR_PHY_TPC_19_B0_ALT_ALPHA_VOLT_0_S         16

#define AR_PHY_TPC_19_ALT_ALPHA_VOLT               0x1f 
#define AR_PHY_TPC_19_ALT_ALPHA_VOLT_S             16

#define AR_PHY_TPC_18_ALT_THERM_CAL_VALUE           0xff 
#define AR_PHY_TPC_18_ALT_THERM_CAL_VALUE_S         0

#define AR_PHY_TPC_18_ALT_VOLT_CAL_VALUE           0xff 
#define AR_PHY_TPC_18_ALT_VOLT_CAL_VALUE_S         8

#define AR_PHY_THERM_ADC_4                          AR_SM_OFFSET(BB_therm_adc_4)
#define AR_PHY_THERM_ADC_4_LATEST_THERM_VALUE       0xFF
#define AR_PHY_THERM_ADC_4_LATEST_THERM_VALUE_S     0
#define AR_PHY_THERM_ADC_4_LATEST_VOLT_VALUE        0xFF
#define AR_PHY_THERM_ADC_4_LATEST_VOLT_VALUE_S      8

#define AR_PHY_TPC_9                                AR_SM_OFFSET(BB_tpc_9)
#define AR_PHY_TPC_9_DESIRED_SCALE_7                0x1F
#define AR_PHY_TPC_9_DESIRED_SCALE_7_S              5

#define AR_PHY_TPC_DESIRED_SCALE                    0x1F
#define AR_PHY_TPC_DESIRED_SCALE_0_S                0
#define AR_PHY_TPC_DESIRED_SCALE_5_S                5
#define AR_PHY_TPC_DESIRED_SCALE_10_S               10
#define AR_PHY_TPC_DESIRED_SCALE_15_S               15
#define AR_PHY_TPC_DESIRED_SCALE_20_S               20
#define AR_PHY_TPC_DESIRED_SCALE_25_S               25

#define AR_PHY_TPC_10                               AR_SM_OFFSET(BB_tpc_10)
#define AR_PHY_TPC_10_DESIRED_SCALE_HT20_5          0x1F
#define AR_PHY_TPC_10_DESIRED_SCALE_HT20_5_S        25

#define AR_PHY_TPC_12                               AR_SM_OFFSET(BB_tpc_12)
#define AR_PHY_TPC_12_DESIRED_SCALE_HT40_5          0x1F
#define AR_PHY_TPC_12_DESIRED_SCALE_HT40_5_S        25

#define AR_PHY_TPC_13                               AR_SM_OFFSET(BB_tpc_13)
#define AR_PHY_TPC_13_DESIRED_SCALE_HT40_6          0x1F
#define AR_PHY_TPC_13_DESIRED_SCALE_HT40_6_S        0
#define AR_PHY_TPC_13_DESIRED_SCALE_HT40_7          0x1F
#define AR_PHY_TPC_13_DESIRED_SCALE_HT40_7_S        5

#define AR_PHY_TPC_14                               AR_SM_OFFSET(BB_tpc_14)
#define AR_PHY_TPC_14_DESIRED_SCALE_HT20_13         0x1F
#define AR_PHY_TPC_14_DESIRED_SCALE_HT20_13_S       25
#define AR_PHY_TPC_14_DESIRED_SCALE_HT20_9          0x1F
#define AR_PHY_TPC_14_DESIRED_SCALE_HT20_9_S        5

#define AR_PHY_TPC_15                               AR_SM_OFFSET(BB_tpc_15)
#define AR_PHY_TPC_15_DESIRED_SCALE_HT40_13         0x1F
#define AR_PHY_TPC_15_DESIRED_SCALE_HT40_13_S       25
#define AR_PHY_TPC_15_DESIRED_SCALE_HT40_9          0x1F
#define AR_PHY_TPC_15_DESIRED_SCALE_HT40_9_S        5

#define AR_PHY_TPC_11_B0                            AR_SM_OFFSET(BB_tpc_11_b0)
#define AR_PHY_TPC_11_B0_OLPC_GAIN_DELTA_0          0xFF
#define AR_PHY_TPC_11_B0_OLPC_GAIN_DELTA_0_S        16
#define AR_PHY_TPC_11_DESIRED_SCALE_HT20_6          0x1F
#define AR_PHY_TPC_11_DESIRED_SCALE_HT20_6_S        0
#define AR_PHY_TPC_11_DESIRED_SCALE_HT20_7          0x1F
#define AR_PHY_TPC_11_DESIRED_SCALE_HT20_7_S        5


#define AR_PHY_TPC_11_B1                            AR_SM1_OFFSET(BB_tpc_11_b1)
#define AR_PHY_TPC_11_B1_OLPC_GAIN_DELTA_1          0xFF
#define AR_PHY_TPC_11_B1_OLPC_GAIN_DELTA_1_S        16

#define AR_PHY_TPC_11_B2                            AR_SM2_OFFSET(BB_tpc_11_b2)
#define AR_PHY_TPC_11_B2_OLPC_GAIN_DELTA_2          0xFF
#define AR_PHY_TPC_11_B2_OLPC_GAIN_DELTA_2_S        16

#define QCN5500_PHY_TPC_11_B3_OLPC_GAIN_DELTA_3     0xFF
#define QCN5500_PHY_TPC_11_B3_OLPC_GAIN_DELTA_3_S   16

#define AR_PHY_TX_FORCED_GAIN_FORCED_TXBB1DBGAIN	0x7
#define AR_PHY_TX_FORCED_GAIN_FORCED_TXBB1DBGAIN_S	1
#define AR_PHY_TX_FORCED_GAIN_FORCED_TXBB6DBGAIN    0x3
#define AR_PHY_TX_FORCED_GAIN_FORCED_TXBB6DBGAIN_S	4 
#define AR_PHY_TX_FORCED_GAIN_FORCED_TXMXRGAIN		0xf
#define AR_PHY_TX_FORCED_GAIN_FORCED_TXMXRGAIN_S	6
#define AR_PHY_TX_FORCED_GAIN_FORCED_PADRVGNA		0xf				
#define AR_PHY_TX_FORCED_GAIN_FORCED_PADRVGNA_S		10
#define AR_PHY_TX_FORCED_GAIN_FORCED_PADRVGNB		0xf		
#define AR_PHY_TX_FORCED_GAIN_FORCED_PADRVGNB_S		14
#define AR_PHY_TX_FORCED_GAIN_FORCED_PADRVGNC		0xf		
#define AR_PHY_TX_FORCED_GAIN_FORCED_PADRVGNC_S		18
#define AR_PHY_TX_FORCED_GAIN_FORCED_PADRVGND		0x3		
#define AR_PHY_TX_FORCED_GAIN_FORCED_PADRVGND_S		22
#define AR_PHY_TX_FORCED_GAIN_FORCED_ENABLE_PAL		1		
#define AR_PHY_TX_FORCED_GAIN_FORCED_ENABLE_PAL_S	24
#define AR_PHY_TX_FORCED_GAIN_FORCE_TX_GAIN			1		
#define AR_PHY_TX_FORCED_GAIN_FORCE_TX_GAIN_S		0

#define AR_PHY_TPC_1							AR_SM_OFFSET(BB_tpc_1)
#define AR_PHY_TPC_1_FORCED_DAC_GAIN				0x1f
#define AR_PHY_TPC_1_FORCED_DAC_GAIN_S				1
#define AR_PHY_TPC_1_FORCE_DAC_GAIN					1
#define AR_PHY_TPC_1_FORCE_DAC_GAIN_S				0
#define AR_PHY_TPC_1_ENABLE_PD_CALIBRATE    0x00400000U
#define AR_PHY_TPC_1_ENABLE_PD_CALIBRATE_S  22

#define AR_PHY_CHAN_INFO_MEMORY_CHANINFOMEM_S2_READ		1
#define AR_PHY_CHAN_INFO_MEMORY_CHANINFOMEM_S2_READ_S	3

#define AR_PHY_PAPRD_MEM_TAB_B0		AR_CHAN_OFFSET(BB_paprd_mem_tab_b0)
#define AR_PHY_PAPRD_MEM_TAB_B1		AR_CHAN1_OFFSET(BB_paprd_mem_tab_b1)
#define AR_PHY_PAPRD_MEM_TAB_B2		AR_CHAN2_OFFSET(BB_paprd_mem_tab_b2)
#define QCN5500_PHY_PAPRD_MEM_TAB_B3  QCN5500_CHAN3_OFFSET(BB_paprd_mem_tab_b3)

#define AR_PHY_PA_GAIN123_B0		AR_CHAN_OFFSET(BB_pa_gain123_b0)
#define AR_PHY_PA_GAIN123_B0_PA_GAIN1_0			0x3FF
#define AR_PHY_PA_GAIN123_B0_PA_GAIN1_0_S		0

#define AR_PHY_PA_GAIN123_B1		AR_CHAN1_OFFSET(BB_pa_gain123_b1)
#define AR_PHY_PA_GAIN123_B1_PA_GAIN1_1			0x3FF
#define AR_PHY_PA_GAIN123_B1_PA_GAIN1_1_S		0

#define AR_PHY_PA_GAIN123_B2		AR_CHAN2_OFFSET(BB_pa_gain123_b2)
#define AR_PHY_PA_GAIN123_B2_PA_GAIN1_2			0x3FF
#define AR_PHY_PA_GAIN123_B2_PA_GAIN1_2_S		0

#define QCN5500_PHY_PA_GAIN123_B3       QCN5500_CHAN3_OFFSET(BB_pa_gain123_b3)
#define QCN5500_PHY_PA_GAIN123_B3_PA_GAIN1_3         0x3FF
#define QCN5500_PHY_PA_GAIN123_B3_PA_GAIN1_3_S       0

//Legacy 6Mb/s , 9Mb/s ,12Mb/s ,18Mb/s
#define AR_PHY_POWERTX_RATE1        AR_SM_OFFSET(BB_powertx_rate1)
#define AR_PHY_POWERTX_RATE1_POWERTX54M_3       0x3F
#define AR_PHY_POWERTX_RATE1_POWERTX54M_3_S     24

//Legacy 54M
#define AR_PHY_POWERTX_RATE2		AR_SM_OFFSET(BB_powertx_rate2)
#define AR_PHY_POWERTX_RATE2_POWERTX54M_7		0x3F
#define AR_PHY_POWERTX_RATE2_POWERTX54M_7_S	    24
#define AR_PHY_POWERTX_RATE3        AR_SM_OFFSET(BB_powertx_rate3)
#define AR_PHY_POWERTX_RATE4        AR_SM_OFFSET(BB_powertx_rate4)
#define AR_PHY_POWERTX_RATE5		AR_SM_OFFSET(BB_powertx_rate5)
//HT20_MCS0_8_16_24
#define AR_PHY_POWERTX_RATE5_POWERTXHT20_0		0x3F
#define AR_PHY_POWERTX_RATE5_POWERTXHT20_0_S	0
//HT20 MCS5
#define AR_PHY_POWERTX_RATE5_POWERTXHT20_3		0x3F
#define AR_PHY_POWERTX_RATE5_POWERTXHT20_3_S	24

//HT20 MCS7
#define AR_PHY_POWERTX_RATE6		AR_SM_OFFSET(BB_powertx_rate6)
#define AR_PHY_POWERTX_RATE6_POWERTXHT20_5		0x3F
#define AR_PHY_POWERTX_RATE6_POWERTXHT20_5_S	8
//HT20 MCS6
#define AR_PHY_POWERTX_RATE6_POWERTXHT20_4		0x3F
#define AR_PHY_POWERTX_RATE6_POWERTXHT20_4_S	0

#define AR_PHY_POWERTX_RATE7		AR_SM_OFFSET(BB_powertx_rate7)
//HT40 MCS5
#define AR_PHY_POWERTX_RATE7_POWERTXHT40_3		0x3F
#define AR_PHY_POWERTX_RATE7_POWERTXHT40_3_S	24

//HT40 MCS7
#define AR_PHY_POWERTX_RATE8		AR_SM_OFFSET(BB_powertx_rate8)
#define AR_PHY_POWERTX_RATE8_POWERTXHT40_5		0x3F
#define AR_PHY_POWERTX_RATE8_POWERTXHT40_5_S	8
//HT40 MCS6
#define AR_PHY_POWERTX_RATE8_POWERTXHT40_4		0x3F
#define AR_PHY_POWERTX_RATE8_POWERTXHT40_4_S	0

#define AR_PHY_POWERTX_RATE9        AR_SM_OFFSET(BB_powertx_rate9)
//HT20 MCS15
#define AR_PHY_POWERTX_RATE10		AR_SM_OFFSET(BB_powertx_rate10)
#define AR_PHY_POWERTX_RATE10_POWERTXHT20_9		0x3F
#define AR_PHY_POWERTX_RATE10_POWERTXHT20_9_S	8

//HT20 MCS23
#define AR_PHY_POWERTX_RATE11		AR_SM_OFFSET(BB_powertx_rate11)
#define AR_PHY_POWERTX_RATE11_POWERTXHT20_13	0x3F
#define AR_PHY_POWERTX_RATE11_POWERTXHT20_13_S	8

#define AR_PHY_POWERTX_RATE12       AR_SM_OFFSET(BB_powertx_rate12)
//HT20 MCS24
#define AR_PHY_POWERTX_RATE13       AR_SM_OFFSET(BB_powertx_rate13)
#define AR_PHY_POWERTX_RATE13_POWERTXHT20_13    0x3F
#define AR_PHY_POWERTX_RATE13_POWERTXHT20_13_S  8
//HT40 MCS24
#define AR_PHY_POWERTX_RATE14       AR_SM_OFFSET(BB_powertx_rate14)
#define AR_PHY_POWERTX_RATE14_POWERTXHT40_13    0x3F
#define AR_PHY_POWERTX_RATE14_POWERTXHT40_13_S  8

#define AR_PHY_CL_TAB_0_CL_GAIN_MOD				0x1F
#define AR_PHY_CL_TAB_0_CL_GAIN_MOD_S			0

#define AR_PHY_CL_TAB_1_CL_GAIN_MOD				0x1F
#define AR_PHY_CL_TAB_1_CL_GAIN_MOD_S			0

#define AR_PHY_CL_TAB_2_CL_GAIN_MOD				0x1F
#define AR_PHY_CL_TAB_2_CL_GAIN_MOD_S			0

#define AR_PHY_CL_TAB_3_CL_GAIN_MOD                0x1F
#define AR_PHY_CL_TAB_3_CL_GAIN_MOD_S          0
/*
 * Hornet/Poseidon Analog Registers
 */
#define AR_HORNET_CH0_TOP               AR_PHY_65NM(overlay_0x16180.Poseidon.ch0_TOP)
#define AR_HORNET_CH0_TOP2              AR_PHY_65NM(overlay_0x16180.Poseidon.ch0_TOP2)
#define AR_HORNET_CH0_TOP3              AR_PHY_65NM(overlay_0x16180.Poseidon.ch0_TOP3)

#define AR_HORNET_CH0_TOP2_FLIP_ADC_STROBE_CLK   0x00C00000
#define AR_HORNET_CH0_TOP2_FLIP_ADC_STROBE_CLK_S 22
#define AR_HORNET_CH0_TOP2_XPABIASLVL   0xf000
#define AR_HORNET_CH0_TOP2_XPABIASLVL_S 12
#define AR_HORNET_CH0_TOP3_LOCAL_XPAON   0x08000000
#define AR_HORNET_CH0_TOP3_LOCAL_XPAON_S 27
#define AR_HORNET_CH0_TOP3_XPA2ON   0xc0000000
#define AR_HORNET_CH0_TOP3_XPA2ON_S 30

#define AR_SCORPION_CH0_TOP              AR_PHY_65NM(overlay_0x16180.Poseidon.ch0_TOP)
#define AR_SCORPION_CH0_TOP_XPABIASLVL   0x3c0
#define AR_SCORPION_CH0_TOP_XPABIASLVL_S 6

#define AR_HONEYBEE_CH0_TOP2             AR_PHY_65NM(overlay_0x16180.Poseidon.ch0_TOP2)
#define AR_HONEYBEE_CH0_TOP2_XPABIASLVL   0xf000
#define AR_HONEYBEE_CH0_TOP2_XPABIASLVL_S 12

#define AR_DRAGONFLY_CH0_TOP             AR_PHY_65NM(overlay_0x16180.Dragonfly.ch0_TOP)
#define AR_DRAGONFLY_CH0_TOP_FLIP_ADC_STROBE_CLK   0x38000000
#define AR_DRAGONFLY_CH0_TOP_FLIP_ADC_STROBE_CLK_S 27
#define AR_DRAGONFLY_CH0_TOP2            AR_PHY_65NM(overlay_0x16180.Dragonfly.ch0_TOP2)
#define AR_DRAGONFLY_CH0_TOP2_XPABIASLVL   0x1e00
#define AR_DRAGONFLY_CH0_TOP2_XPABIASLVL_S 9
#define AR_DRAGONFLY_CH0_TOP3            AR_PHY_65NM(overlay_0x16180.Dragonfly.ch0_TOP3)
#define AR_DRAGONFLY_CH0_TOP3_LOCAL_XPAON   0x10000000
#define AR_DRAGONFLY_CH0_TOP3_LOCAL_XPAON_S 28
#define AR_DRAGONFLY_CH0_TOP3_XPA2ON   0xE0000000
#define AR_DRAGONFLY_CH0_TOP3_XPA2ON_S 29
#define QCN5500_CH0_TOP2                 AR_PHY_65NM(overlay_0x16180.Jet.ch0_TOP2)
#define QCN5500_CH0_TOP3                 AR_PHY_65NM(overlay_0x16180.Jet.ch0_TOP3)
#define QCN5500_CH0_TOP3_XPABIASLVL   0x000F0000
#define QCN5500_CH0_TOP3_XPABIASLVL_S 16
#define AR_PHY_XTAL     AR_PHY_65NM(overlay_0x16180.Jet.ch0_XTAL)
#define AR_PHY_XTAL2    AR_PHY_65NM(overlay_0x16180.Jet.ch0_XTAL2)

#define QCN5500_XTAL_BIBXOSC                    0x00000007
#define QCN5500_XTAL_BIBXOSC_S                  0

#define QCN5500_XTAL_LEN                        0x00000c00
#define QCN5500_XTAL_LEN_S                      10

#define QCN5500_XTAL_XOSCREFV                   0x00018000
#define QCN5500_XTAL_XOSCREFV_S                 15


#define AR_CH0_XTAL                     AR_PHY_65NM(ch0_XTAL)
#define AR_CH0_THERM                    AR_PHY_65NM(ch0_THERM)
#define AR_SCORPION_CH0_XTAL            AR_PHY_65NM(overlay_0x16180.Poseidon.ch0_XTAL)

#define AR_DRAGONFLY_CH0_XTAL           AR_PHY_65NM(overlay_0x16180.Dragonfly.ch0_XTAL)
#define QCN5500_CH0_XTAL                AR_PHY_65NM(overlay_0x16180.Jet.ch0_XTAL)
#define AR_HORNET_CH0_THERM             AR_PHY_65NM(overlay_0x16180.Poseidon.ch0_THERM)

#define AR_HORNET_CH0_XTAL              AR_PHY_65NM(overlay_0x16180.Poseidon.ch0_XTAL)
#define AR_APHRODITE_CH0_XTAL           AR_PHY_65NM(overlay_0x16180.Aphrodite.ch0_XTAL)
#define AR_HORNET_CHO_XTAL_CAPINDAC     0x7F000000
#define AR_HORNET_CHO_XTAL_CAPINDAC_S   24
#define AR_HORNET_CHO_XTAL_CAPOUTDAC    0x00FE0000
#define AR_HORNET_CHO_XTAL_CAPOUTDAC_S  17

#define AR_HORNET_CH0_DDR_DPLL2         AR_PHY_65NM(overlay_0x16180.Poseidon.ch0_DDR_DPLL2)
#define AR_HORNET_CH0_DDR_DPLL3         AR_PHY_65NM(overlay_0x16180.Poseidon.ch0_DDR_DPLL3)
#define AR_PHY_CCA_NOM_VAL_HORNET_2GHZ      -118
#define AR_PHY_BB_DPLL1                 AR_PHY_65NM(overlay_0x16180.Poseidon.ch0_BB_DPLL1)
#define AR_PHY_BB_DPLL1_REFDIV          0xF8000000
#define AR_PHY_BB_DPLL1_REFDIV_S        27
#define AR_PHY_BB_DPLL1_NINI            0x07FC0000
#define AR_PHY_BB_DPLL1_NINI_S          18
#define AR_PHY_BB_DPLL1_NFRAC           0x0003FFFF
#define AR_PHY_BB_DPLL1_NFRAC_S         0

#define AR_PHY_BB_DPLL2                 AR_PHY_65NM(overlay_0x16180.Poseidon.ch0_BB_DPLL2)
#define AR_PHY_BB_DPLL2_RANGE           0x80000000
#define AR_PHY_BB_DPLL2_RANGE_S         31
#define AR_PHY_BB_DPLL2_LOCAL_PLL       0x40000000
#define AR_PHY_BB_DPLL2_LOCAL_PLL_S     30
#define AR_PHY_BB_DPLL2_KI              0x3C000000
#define AR_PHY_BB_DPLL2_KI_S            26
#define AR_PHY_BB_DPLL2_KD              0x03F80000
#define AR_PHY_BB_DPLL2_KD_S            19
#define AR_PHY_BB_DPLL2_EN_NEGTRIG      0x00040000
#define AR_PHY_BB_DPLL2_EN_NEGTRIG_S    18
#define AR_PHY_BB_DPLL2_SEL_1SDM        0x00020000
#define AR_PHY_BB_DPLL2_SEL_1SDM_S      17
#define AR_PHY_BB_DPLL2_PLL_PWD         0x00010000
#define AR_PHY_BB_DPLL2_PLL_PWD_S       16
#define AR_PHY_BB_DPLL2_OUTDIV          0x0000E000
#define AR_PHY_BB_DPLL2_OUTDIV_S        13
#define AR_PHY_BB_DPLL2_DELTA           0x00001F80
#define AR_PHY_BB_DPLL2_DELTA_S         7
#define AR_PHY_BB_DPLL2_SPARE           0x0000007F
#define AR_PHY_BB_DPLL2_SPARE_S         0

#define AR_PHY_BB_DPLL3                 AR_PHY_65NM(overlay_0x16180.Poseidon.ch0_BB_DPLL3)
#define AR_PHY_BB_DPLL3_MEAS_AT_TXON    0x80000000
#define AR_PHY_BB_DPLL3_MEAS_AT_TXON_S  31
#define AR_PHY_BB_DPLL3_DO_MEAS         0x40000000
#define AR_PHY_BB_DPLL3_DO_MEAS_S       30
#define AR_PHY_BB_DPLL3_PHASE_SHIFT     0x3F800000
#define AR_PHY_BB_DPLL3_PHASE_SHIFT_S   23
#define AR_PHY_BB_DPLL3_SQSUM_DVC       0x007FFFF8
#define AR_PHY_BB_DPLL3_SQSUM_DVC_S     3
#define AR_PHY_BB_DPLL3_SPARE           0x00000007
#define AR_PHY_BB_DPLL3_SPARE_S         0x0

#define AR_PHY_BB_DPLL4                 AR_PHY_65NM(overlay_0x16180.Poseidon.ch0_BB_DPLL4)
#define AR_PHY_BB_DPLL4_MEAN_DVC        0xFFE00000
#define AR_PHY_BB_DPLL4_MEAN_DVC_S      21
#define AR_PHY_BB_DPLL4_VC_MEAS0        0x001FFFF0
#define AR_PHY_BB_DPLL4_VC_MEAS0_S      4
#define AR_PHY_BB_DPLL4_MEAS_DONE       0x00000008
#define AR_PHY_BB_DPLL4_MEAS_DONE_S     3
#define AR_PHY_BB_DPLL4_SPARE           0x00000007
#define AR_PHY_BB_DPLL4_SPARE_S         0

/*
 * Wasp Analog Registers
 */
#define AR_PHY_PLL_CONTROL              AR_PHY_65NM(overlay_0x16180.Osprey.ch0_pll_cntl)
#define AR_PHY_PLL_MODE                 AR_PHY_65NM(overlay_0x16180.Osprey.ch0_pll_mode)
#define AR_PHY_PLL_BB_DPLL3             AR_PHY_65NM(overlay_0x16180.Osprey.ch0_bb_dpll3)
#define AR_PHY_PLL_BB_DPLL4             AR_PHY_65NM(overlay_0x16180.Osprey.ch0_bb_dpll4)

/*
 * JET Analog Registers
 */
#define QCN5500_PHY_PLL_CONTROL         AR_PHY_65NM(overlay_0x16180.Jet.ch0_BB_PLL)
#define QCN5500_PHY_PLL_MODE            AR_PHY_65NM(overlay_0x16180.Jet.ch0_BB_PLL2)
#define QCN5500_PHY_BB_PLL3             AR_PHY_65NM(overlay_0x16180.Jet.ch0_BB_PLL3)
/*
 * PMU Register Map
 */
#define AR_PHY_PMU(_field)         offsetof(struct pmu_reg, _field)
#define AR_PHY_PMU1                AR_PHY_PMU(ch0_PMU1)
#define AR_PHY_PMU2                AR_PHY_PMU(ch0_PMU2)
#define AR_PHY_JUPITER_PMU(_field) offsetof(struct radio65_reg, _field)
#define AR_PHY_PMU1_JUPITER        AR_PHY_JUPITER_PMU(overlay_0x16180.Jupiter.ch0_PMU1)
#define AR_PHY_PMU2_JUPITER        AR_PHY_JUPITER_PMU(overlay_0x16180.Jupiter.ch0_PMU2)
#define AR_PHY_DRAGONFLY_PMU(_field)    offsetof(struct pmu_reg_dragonfly, _field)
#define AR_PHY_PMU1_DRAGONFLY      AR_PHY_DRAGONFLY_PMU(ch0_PMU1)
#define AR_PHY_PMU2_DRAGONFLY      AR_PHY_DRAGONFLY_PMU(ch0_PMU2)
#define QCN5500_PHY_PMU(_field)    offsetof(struct pmu_reg_jet, _field)
#define QCN5500_PHY_PMU1           QCN5500_PHY_PMU(ch0_PMU1)
#define QCN5500_PHY_PMU2           QCN5500_PHY_PMU(ch0_PMU2)

/*
 * GLB Register Map
 */
#define AR_PHY_GLB(_field) offsetof(struct glb_reg, _field)
#define AR_PHY_GLB_CONTROL_JUPITER                AR_PHY_GLB(overlap_0x20044.Jupiter.GLB_CONTROL)

/*
 * PMU Field Definitions
 */
/* ch0_PMU1 */
#define AR_PHY_PMU1_PWD            0x00000001 /* power down switch regulator */
#define AR_PHY_PMU1_PWD_S          0

/* ch0_PMU2 */
#define AR_PHY_PMU2_PGM            0x00200000
#define AR_PHY_PMU2_PGM_S          21

/* ch0_PHY_CTRL2 */
#define AR_PHY_CTRL2_TX_MAN_CAL         0x03C00000
#define AR_PHY_CTRL2_TX_MAN_CAL_S       22
#define AR_PHY_CTRL2_TX_CAL_SEL         0x00200000
#define AR_PHY_CTRL2_TX_CAL_SEL_S       21
#define AR_PHY_CTRL2_TX_CAL_EN          0x00100000
#define AR_PHY_CTRL2_TX_CAL_EN_S        20

#define PCIE_CO_ERR_CTR_CTRL                 0x40e8
#define PCIE_CO_ERR_CTR_CTR0                 0x40e0
#define PCIE_CO_ERR_CTR_CTR1                 0x40e4


#define RCVD_ERR_CTR_RUN                     0x0001
#define RCVD_ERR_CTR_AUTO_STOP               0x0002
#define BAD_TLP_ERR_CTR_RUN                  0x0004
#define BAD_TLP_ERR_CTR_AUTO_STOP            0x0008
#define BAD_DLLP_ERR_CTR_RUN                 0x0010
#define BAD_DLLP_ERR_CTR_AUTO_STOP           0x0020
#define RPLY_TO_ERR_CTR_RUN                  0x0040
#define RPLY_TO_ERR_CTR_AUTO_STOP            0x0080
#define RPLY_NUM_RO_ERR_CTR_RUN              0x0100
#define RPLY_NUM_RO_ERR_CTR_AUTO_STOP        0x0200

#define RCVD_ERR_MASK                        0x000000ff
#define RCVD_ERR_MASK_S                      0
#define BAD_TLP_ERR_MASK                     0x0000ff00
#define BAD_TLP_ERR_MASK_S                   8
#define BAD_DLLP_ERR_MASK                    0x00ff0000
#define BAD_DLLP_ERR_MASK_S                  16

#define RPLY_TO_ERR_MASK                     0x000000ff
#define RPLY_TO_ERR_MASK_S                   0
#define RPLY_NUM_RO_ERR_MASK                 0x0000ff00
#define RPLY_NUM_RO_ERR_MASK_S               8

#define AR_MERLIN_RADIO_SYNTH4    offsetof(struct merlin2_0_radio_reg_map, SYNTH4)
#define AR_MERLIN_RADIO_SYNTH6    offsetof(struct merlin2_0_radio_reg_map, SYNTH6)
#define AR_MERLIN_RADIO_SYNTH7    offsetof(struct merlin2_0_radio_reg_map, SYNTH7)
#define AR_MERLIN_RADIO_TOP0      offsetof(struct merlin2_0_radio_reg_map, TOP0)
#define AR_MERLIN_RADIO_TOP1      offsetof(struct merlin2_0_radio_reg_map, TOP1)
#define AR_MERLIN_RADIO_TOP2      offsetof(struct merlin2_0_radio_reg_map, TOP2)
#define AR_MERLIN_RADIO_TOP3      offsetof(struct merlin2_0_radio_reg_map, TOP3)

/* Efuse Register Map */
#define AR_JET_EFUSE(_field) offsetof(struct efuse_reg_jet, _field)
#define AR_PHY_EFUSE_WR_ENABLE_REG AR_JET_EFUSE(EFUSE_WR_ENABLE_REG)
#define AR_PHY_EFUSE_INT_ENABLE_REG AR_JET_EFUSE(EFUSE_INT_ENABLE_REG)

// 32'h18700000 (EFUSE_WR_ENABLE_REG)
#define EFUSE_WR_ENABLE_REG__EFUSE_WR_ENABLE_REG_V__SHIFT                      0
#define EFUSE_WR_ENABLE_REG__EFUSE_WR_ENABLE_REG_V__WIDTH                      1
#define EFUSE_WR_ENABLE_REG__EFUSE_WR_ENABLE_REG_V__MASK             0x00000001U

#define EFUSE_WR_ENABLE_REG__EFUSE_BUSY_REG_V__SHIFT                           1
#define EFUSE_WR_ENABLE_REG__EFUSE_BUSY_REG_V__WIDTH                           1
#define EFUSE_WR_ENABLE_REG__EFUSE_BUSY_REG_V__MASK                  0x00000002U

#define EFUSE_WR_ENABLE_REG__EFUSE_LDO25_EN_REG_V__SHIFT                       2
#define EFUSE_WR_ENABLE_REG__EFUSE_LDO25_EN_REG_V__WIDTH                       1
#define EFUSE_WR_ENABLE_REG__EFUSE_LDO25_EN_REG_V__MASK              0x00000004U

#define EFUSE_WR_ENABLE_REG__EFUSE_LDO25_STATUS_REG_V__SHIFT                   3
#define EFUSE_WR_ENABLE_REG__EFUSE_LDO25_STATUS_REG_V__WIDTH                   1
#define EFUSE_WR_ENABLE_REG__EFUSE_LDO25_STATUS_REG_V__MASK          0x00000008U

#define EFUSE_WR_ENABLE_REG__EFUSE_AUTOLOAD_DONE_REG_V__SHIFT                  4
#define EFUSE_WR_ENABLE_REG__EFUSE_AUTOLOAD_DONE_REG_V__WIDTH                  1
#define EFUSE_WR_ENABLE_REG__EFUSE_AUTOLOAD_DONE_REG_V__MASK         0x00000010U

#define EFUSE_INT_ENABLE_REG__EFUSE_INT_ENABLE_REG_V__SHIFT                    0
#define EFUSE_INT_ENABLE_REG__EFUSE_INT_ENABLE_REG_V__WIDTH                    1
#define EFUSE_INT_ENABLE_REG__EFUSE_INT_ENABLE_REG_V__MASK           0x00000001U
#define EFUSE_WR_ENABLE_REG_ADDRESS                                  0x18700000
#define EFUSE_WR_ENABLE_REG_OFFSET                                   0x0000
// SW modifiable bits
#define EFUSE_WR_ENABLE_REG_SW_MASK                                  0x0000001f
// bits defined at reset
#define EFUSE_WR_ENABLE_REG_RSTMASK                                  0xffffffff
// reset value (ignore bits undefined at reset)
#define EFUSE_WR_ENABLE_REG_RESET                                    0x00000000

#define EFUSE_INT_ENABLE_REG_EFUSE_INT_ENABLE_REG_V_RESET            0x1 // 1
/*ar9300_SW_ATE_RXDCO*/
#define AR_PHY_RANGE_OSDAC                  0x03
#define AR_PHY_RANGE_OSDAC_S                30
#define AR_PHY_SEL_OFST_READBK              0x03
#define AR_PHY_SEL_OFST_READBK_S            8
#define AR_PHY_OFSTCORRI2VI                 0x1f
#define AR_PHY_OFSTCORRI2VI_S               26
#define AR_PHY_OFSTCORRI2VQ                 0x1f
#define AR_PHY_OFSTCORRI2VQ_S               21
#define AR_PHY_NEG_IQ_LIMIT                 0x1c//0x1f
#define AR_PHY_POS_IQ_LIMIT                 0x03//0x00
#define AR_PHY_LOCALOFFSET                  0x01
#define AR_PHY_LOCALOFFSET_S                29

/*Jet specific, sticky write config addr*/
#define CONFIG_ADDR_BEFORE_INIT_CAL 0
#define CONFIG_ADDR_AFTER_INIT_CAL 1
#define CONFIG_ADDR_INIT            0xffffffff
#define CONFIG_ADDR_ADDRESS_MASK    0x000fffff
#define CONFIG_ADDR_MODE_MASK       0x00f00000
#define CONFIG_ADDR_PREPOST_MASK    0x0f000000

#define DESIRED_SCALE_OFFSET_ALL_0_8_16_24               0
#define DESIRED_SCALE_OFFSET_ALL_1_2_9_10_17_18_25_26    1
#define DESIRED_SCALE_OFFSET_ALL_3_4_11_12_19_20_27_28   2
#define DESIRED_SCALE_OFFSET_ALL_5_13_21_29              3
#define DESIRED_SCALE_OFFSET_ALL_6_14_22_30              4
#define DESIRED_SCALE_OFFSET_ALL_7_15_23_31              5

#endif  /* _ATH_AR9300PHY_H_ */
