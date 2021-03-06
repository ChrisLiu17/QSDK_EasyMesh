/*
 * Copyright (c) 2011, 2017 Qualcomm Innovation Center, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Innovation Center, Inc.
 */
/*
 * Copyright (c) 2008-2010, Atheros Communications Inc. 
 * All Rights Reserved.
 * 
 * 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * 
 */

#include "opt_ah.h"

#ifdef AH_SUPPORT_AR9300

#include "ah.h"
#include "ah_internal.h"
#include "ah_devid.h"

#include "ar9300/ar9300desc.h"
#include "ar9300/ar9300.h"
#include "ar9300/ar9300reg.h"
#include "ar9300/ar9300phy.h"
#include "ar9300/ar9300paprd.h"

#if ATH_DRIVER_SIM
#include "ah_sim.h"
#endif

/* Add static register initialization vectors */
#include "ar9300/ar9300_osprey22.ini"
#include "ar9300/ar9330_11.ini"
#include "ar9300/ar9330_12.ini"
#include "ar9300/ar9340.ini"
#include "ar9300/ar9485.ini"
#include "ar9300/ar9485_1_1.ini"
#include "ar9300/ar9300_jupiter10.ini"
#include "ar9300/ar9300_jupiter20.ini"
#include "ar9300/ar9580.ini"
#include "ar9300/ar955x.ini"
#include "ar9300/ar953x.ini"
#include "ar9300/ar9300_aphrodite10.ini"
#include "ar9300/ar956x.ini"
#include "ar9300/qcn5500.ini"
#ifdef MDK_AP          // MDK_AP is defined only for NART. 
#include "linux_hw.h"
#endif

static bool ar9300_get_chip_power_limits(struct ath_hal *ah,
    HAL_CHANNEL *chans, u_int32_t nchans);

static inline HAL_STATUS ar9300_init_mac_addr(struct ath_hal *ah);
static inline HAL_STATUS ar9300_hw_attach(struct ath_hal *ah);
static inline void ar9300_hw_detach(struct ath_hal *ah);
static int16_t ar9300_get_nf_adjust(struct ath_hal *ah,
    const HAL_CHANNEL_INTERNAL *c);
int ar9300_get_cal_intervals(struct ath_hal *ah, HAL_CALIBRATION_TIMER **timerp,
    HAL_CAL_QUERY query);
#if ATH_TRAFFIC_FAST_RECOVER
unsigned long ar9300_get_pll3_sqsum_dvc(struct ath_hal *ah);
#endif
static int ar9300_init_offsets(struct ath_hal *ah, u_int16_t devid);

#ifdef ART_BUILD
extern int MyRegisterRead(unsigned int address, unsigned int *value);
#endif

static void
ar9300_disable_pcie_phy(struct ath_hal *ah);
#ifdef ATH_CCX
static bool
ar9300_record_serial_number(struct ath_hal *ah);
#endif
bool ar9300_set_temp_Ko_Kg(struct ath_hal *ah);

static const HAL_PERCAL_DATA iq_cal_single_sample =
                          {IQ_MISMATCH_CAL,
                          MIN_CAL_SAMPLES,
                          PER_MAX_LOG_COUNT,
                          ar9300_iq_cal_collect,
                          ar9300_iq_calibration};

static HAL_CALIBRATION_TIMER ar9300_cals[] =
                          { {IQ_MISMATCH_CAL,               /* Cal type */
                             1200000,                       /* Cal interval */
                             0                              /* Cal timestamp */
                            },
                          {TEMP_COMP_CAL,
                             5000,
                             0
                            },
                          };
                          
#if ATH_PCIE_ERROR_MONITOR

int ar9300_start_pcie_error_monitor(struct ath_hal *ah, int b_auto_stop)
{
    u_int32_t val;

    /* Clear the counters */
    OS_REG_WRITE(ah, PCIE_CO_ERR_CTR_CTR0, 0);
    OS_REG_WRITE(ah, PCIE_CO_ERR_CTR_CTR1, 0);
    
    /* Read the previous value */
    val = OS_REG_READ(ah, PCIE_CO_ERR_CTR_CTRL);

    /* Set auto_stop */
    if (b_auto_stop) {
        val |=
            RCVD_ERR_CTR_AUTO_STOP | BAD_TLP_ERR_CTR_AUTO_STOP |
            BAD_DLLP_ERR_CTR_AUTO_STOP | RPLY_TO_ERR_CTR_AUTO_STOP |
            RPLY_NUM_RO_ERR_CTR_AUTO_STOP;
    } else {
        val &= ~(
            RCVD_ERR_CTR_AUTO_STOP | BAD_TLP_ERR_CTR_AUTO_STOP |
            BAD_DLLP_ERR_CTR_AUTO_STOP | RPLY_TO_ERR_CTR_AUTO_STOP |
            RPLY_NUM_RO_ERR_CTR_AUTO_STOP);
    }
    OS_REG_WRITE(ah, PCIE_CO_ERR_CTR_CTRL, val );

    /*
     * Start to run.
     * This has to be done separately from the above auto_stop flag setting,
     * to avoid a HW race condition.
     */
    val |=
        RCVD_ERR_CTR_RUN | BAD_TLP_ERR_CTR_RUN | BAD_DLLP_ERR_CTR_RUN |
        RPLY_TO_ERR_CTR_RUN | RPLY_NUM_RO_ERR_CTR_RUN;
    OS_REG_WRITE(ah, PCIE_CO_ERR_CTR_CTRL, val);

    return 0;
}

int ar9300_read_pcie_error_monitor(struct ath_hal *ah, void* p_read_counters)
{
    u_int32_t val;
    ar_pcie_error_moniter_counters *p_counters =
        (ar_pcie_error_moniter_counters*) p_read_counters;
    
    val = OS_REG_READ(ah, PCIE_CO_ERR_CTR_CTR0);
    
    p_counters->uc_receiver_errors = MS(val, RCVD_ERR_MASK);
    p_counters->uc_bad_tlp_errors  = MS(val, BAD_TLP_ERR_MASK);
    p_counters->uc_bad_dllp_errors = MS(val, BAD_DLLP_ERR_MASK);

    val = OS_REG_READ(ah, PCIE_CO_ERR_CTR_CTR1);
    
    p_counters->uc_replay_timeout_errors        = MS(val, RPLY_TO_ERR_MASK);
    p_counters->uc_replay_number_rollover_errors= MS(val, RPLY_NUM_RO_ERR_MASK);

    return 0;
}

int ar9300_stop_pcie_error_monitor(struct ath_hal *ah)
{
    u_int32_t val;
        
    /* Read the previous value */
    val = OS_REG_READ(ah, PCIE_CO_ERR_CTR_CTRL);
    
    val &= ~(
        RCVD_ERR_CTR_RUN |
        BAD_TLP_ERR_CTR_RUN |
        BAD_DLLP_ERR_CTR_RUN |
        RPLY_TO_ERR_CTR_RUN |
        RPLY_NUM_RO_ERR_CTR_RUN);
   
    /* Start to stop */
    OS_REG_WRITE(ah, PCIE_CO_ERR_CTR_CTRL, val );

    return 0;
}

#endif /* ATH_PCIE_ERROR_MONITOR */

/* WIN32 does not support C99 */
static const struct ath_hal_private ar9300hal = {
    {
        ar9300_get_rate_table,             /* ah_get_rate_table */
        ar9300_detach,                     /* ah_detach */

        /* Reset Functions */
        ar9300_reset,                      /* ah_reset */
        ar9300_phy_disable,                /* ah_phy_disable */
        ar9300_disable,                    /* ah_disable */
        ar9300_config_pci_power_save,      /* ah_config_pci_power_save */
        ar9300_set_pcu_config,             /* ah_set_pcu_config */
        ar9300_calibration,                /* ah_per_calibration */
        ar9300_reset_cal_valid,            /* ah_reset_cal_valid */
        ar9300_set_tx_power_limit,         /* ah_set_tx_power_limit */

#if ATH_ANT_DIV_COMB
        ar9300_ant_ctrl_set_lna_div_use_bt_ant,     /* ah_ant_ctrl_set_lna_div_use_bt_ant */
#endif /* ATH_ANT_DIV_COMB */
#ifdef ATH_SUPPORT_DFS
        ar9300_radar_wait,                 /* ah_radar_wait */

        /* New DFS functions */
        ar9300_check_dfs,                  /* ah_ar_check_dfs */
        ar9300_dfs_found,                  /* ah_ar_dfs_found */
        ar9300_enable_dfs,                 /* ah_ar_enable_dfs */
        ar9300_get_dfs_thresh,             /* ah_ar_get_dfs_thresh */
        ar9300_get_dfs_radars,             /* ah_ar_get_dfs_radars */
        ar9300_adjust_difs,                /* ah_adjust_difs */
        ar9300_dfs_config_fft,             /* ah_dfs_config_fft */
        ar9300_dfs_cac_war,                /* ah_dfs_cac_war */
        ar9300_cac_tx_quiet,               /* ah_cac_tx_quiet */
#endif
        ar9300_get_extension_channel,      /* ah_get_extension_channel */
        ar9300_is_fast_clock_enabled,      /* ah_is_fast_clock_enabled */

        /* Transmit functions */
        ar9300_update_tx_trig_level,       /* ah_update_tx_trig_level */
        ar9300_get_tx_trig_level,          /* ah_get_tx_trig_level */
        ar9300_setup_tx_queue,             /* ah_setup_tx_queue */
        ar9300_set_tx_queue_props,         /* ah_set_tx_queue_props */
        ar9300_get_tx_queue_props,         /* ah_get_tx_queue_props */
        ar9300_release_tx_queue,           /* ah_release_tx_queue */
        ar9300_reset_tx_queue,             /* ah_reset_tx_queue */
        ar9300_get_tx_dp,                  /* ah_get_tx_dp */
        ar9300_set_tx_dp,                  /* ah_set_tx_dp */
        ar9300_num_tx_pending,             /* ah_num_tx_pending */
        ar9300_start_tx_dma,               /* ah_start_tx_dma */
        ar9300_stop_tx_dma,                /* ah_stop_tx_dma */
        ar9300_stop_tx_dma_indv_que,       /* ah_stop_tx_dma_indv_que */
        ar9300_abort_tx_dma,               /* ah_abort_tx_dma */
        ar9300_fill_tx_desc,               /* ah_fill_tx_desc */
        ar9300_set_desc_link,              /* ah_set_desc_link */
        ar9300_get_desc_link_ptr,          /* ah_get_desc_link_ptr */
        ar9300_clear_tx_desc_status,       /* ah_clear_tx_desc_status */
#ifdef ATH_SWRETRY
        ar9300_clear_dest_mask,            /* ah_clear_dest_mask */
#endif
        ar9300_proc_tx_desc,               /* ah_proc_tx_desc */
        ar9300_get_raw_tx_desc,            /* ah_get_raw_tx_desc */
        ar9300_get_tx_rate_code,           /* ah_get_tx_rate_code */
        AH_NULL,                           /* ah_get_tx_intr_queue */
        ar9300_tx_req_intr_desc,           /* ah_req_tx_intr_desc */
        ar9300_calc_tx_airtime,            /* ah_calc_tx_airtime */
        ar9300_setup_tx_status_ring,       /* ah_setup_tx_status_ring */

        /* RX Functions */
        ar9300_get_rx_dp,                  /* ah_get_rx_dp */
        ar9300_set_rx_dp,                  /* ah_set_rx_dp */
        ar9300_enable_receive,             /* ah_enable_receive */
        ar9300_stop_dma_receive,           /* ah_stop_dma_receive */
        ar9300_start_pcu_receive,          /* ah_start_pcu_receive */
        ar9300_stop_pcu_receive,           /* ah_stop_pcu_receive */
        ar9300_set_multicast_filter,       /* ah_set_multicast_filter */
        ar9300_get_rx_filter,              /* ah_get_rx_filter */
        ar9300_set_rx_filter,              /* ah_set_rx_filter */
        ar9300_set_rx_sel_evm,             /* ah_set_rx_sel_evm */
        ar9300_set_rx_abort,               /* ah_set_rx_abort */
        AH_NULL,                           /* ah_setup_rx_desc */
        ar9300_proc_rx_desc,               /* ah_proc_rx_desc */
        ar9300_get_rx_key_idx,             /* ah_get_rx_key_idx */
        ar9300_proc_rx_desc_fast,          /* ah_proc_rx_desc_fast */
        ar9300_ani_ar_poll,                /* ah_rx_monitor */
        ar9300_process_mib_intr,           /* ah_proc_mib_event */

        /* Misc Functions */
        ar9300_get_capability,             /* ah_get_capability */
        ar9300_set_capability,             /* ah_set_capability */
        ar9300_get_diag_state,             /* ah_get_diag_state */
        ar9300_get_mac_address,            /* ah_get_mac_address */
        ar9300_set_mac_address,            /* ah_set_mac_address */
        ar9300_get_bss_id_mask,            /* ah_get_bss_id_mask */
        ar9300_set_bss_id_mask,            /* ah_set_bss_id_mask */
        ar9300_set_regulatory_domain,      /* ah_set_regulatory_domain */
        ar9300_set_led_state,              /* ah_set_led_state */
        ar9300_set_power_led_state,        /* ah_setpowerledstate */
        ar9300_set_network_led_state,      /* ah_setnetworkledstate */
        ar9300_write_associd,              /* ah_write_associd */
        ar9300_force_tsf_sync,             /* ah_force_tsf_sync */
        ar9300_gpio_cfg_input,             /* ah_gpio_cfg_input */
        ar9300_gpio_cfg_output,            /* ah_gpio_cfg_output */
        ar9300_gpio_cfg_output_led_off,    /* ah_gpio_cfg_output_led_off */
        ar9300_gpio_get,                   /* ah_gpio_get */
        ar9300_gpio_set,                   /* ah_gpio_set */
        ar9300_gpio_get_intr,              /* ah_gpio_get_intr */
        ar9300_gpio_set_intr,              /* ah_gpio_set_intr */
        ar9300_gpio_get_polarity,          /* ah_gpio_get_polarity */
        ar9300_gpio_set_polarity,          /* ah_gpio_set_polarity */
        ar9300_gpio_get_mask,              /* ah_gpio_get_mask */
        ar9300_gpio_set_mask,              /* ah_gpio_set_mask */
        ar9300_get_tsf32,                  /* ah_get_tsf32 */
        ar9300_get_tsf64,                  /* ah_get_tsf64 */
        ar9300_get_tsf2_32,                /* ah_get_tsf2_32 */
        ar9300_reset_tsf,                  /* ah_reset_tsf */
        ar9300_detect_card_present,        /* ah_detect_card_present */
        ar9300_update_mib_mac_stats,       /* ah_update_mib_mac_stats */
        ar9300_get_mib_mac_stats,          /* ah_get_mib_mac_stats */
        ar9300_get_rfgain,                 /* ah_get_rf_gain */
        ar9300_get_def_antenna,            /* ah_get_def_antenna */
        ar9300_set_def_antenna,            /* ah_set_def_antenna */
        ar9300_set_slot_time,              /* ah_set_slot_time */
        ar9300_set_ack_timeout,            /* ah_set_ack_timeout */
        ar9300_get_ack_timeout,            /* ah_get_ack_timeout */
        ar9300_set_coverage_class,         /* ah_set_coverage_class */
        ar9300_set_quiet,                  /* ah_set_quiet */
        ar9300_set_antenna_switch,         /* ah_set_antenna_switch */
        ar9300_get_desc_info,              /* ah_get_desc_info */
        ar9300_select_ant_config,          /* ah_select_ant_config */
        ar9300_ant_ctrl_common_get,        /* ah_ant_ctrl_common_get */
        ar9300_ant_swcom_sel,              /* ah_ant_swcom_sel */
        ar9300_enable_tpc,                 /* ah_enable_tpc */
        AH_NULL,                           /* ah_olpc_temp_compensation */
#if ATH_SUPPORT_CRDC
        ar9300_chain_rssi_diff_compensation,/*ah_chain_rssi_diff_compensation*/
#endif
        ar9300_disable_phy_restart,        /* ah_disable_phy_restart */
        ar9300_enable_keysearch_always,
        ar9300_interference_is_present,    /* ah_interference_is_present */
        ar9300_disp_tpc_tables,             /* ah_disp_tpc_tables */
        ar9300_get_tpc_tables,              /* ah_get_tpc_tables */
        /* Key Cache Functions */
        ar9300_get_key_cache_size,         /* ah_get_key_cache_size */
        ar9300_reset_key_cache_entry,      /* ah_reset_key_cache_entry */
        ar9300_is_key_cache_entry_valid,   /* ah_is_key_cache_entry_valid */
        ar9300_set_key_cache_entry,        /* ah_set_key_cache_entry */
        ar9300_set_key_cache_entry_mac,    /* ah_set_key_cache_entry_mac */
        ar9300_print_keycache,             /* ah_print_key_cache */
#if ATH_SUPPORT_KEYPLUMB_WAR
        ar9300_check_key_cache_entry,      /* ah_check_key_cache_entry */
#endif
        /* Power Management Functions */
        ar9300_set_power_mode,             /* ah_set_power_mode */
        ar9300_set_sm_power_mode,          /* ah_set_sm_ps_mode */
#if ATH_WOW
        ar9300_wow_apply_pattern,          /* ah_wow_apply_pattern */
        ar9300_wow_enable,                 /* ah_wow_enable */
        ar9300_wow_wake_up,                /* ah_wow_wake_up */
#if ATH_WOW_OFFLOAD
        ar9300_wowoffload_prep,                 /* ah_wow_offload_prep */
        ar9300_wowoffload_post,                 /* ah_wow_offload_post */
        ar9300_wowoffload_download_rekey_data,  /* ah_wow_offload_download_rekey_data */
        ar9300_wowoffload_retrieve_data,        /* ah_wow_offload_retrieve_data */
        ar9300_wowoffload_download_acer_magic,  /* ah_wow_offload_download_acer_magic */
        ar9300_wowoffload_download_acer_swka,   /* ah_wow_offload_download_acer_swka */
        ar9300_wowoffload_download_arp_info,    /* ah_wow_offload_download_arp_info */
        ar9300_wowoffload_download_ns_info,     /* ah_wow_offload_download_ns_info */
#endif /* ATH_WOW_OFFLOAD */
#endif

        /* Get Channel Noise */
        ath_hal_get_chan_noise,            /* ah_get_chan_noise */
        ar9300_chain_noise_floor,          /* ah_get_chain_noise_floor */
        ar9300_get_nf_from_reg,            /* ah_get_nf_from_reg */
        ar9300_get_rx_nf_offset,           /* ah_get_rx_nf_offset */

        /* Beacon Functions */
        ar9300_beacon_init,                /* ah_beacon_init */
        ar9300_set_sta_beacon_timers,      /* ah_set_station_beacon_timers */

        /* Interrupt Functions */
        ar9300_is_interrupt_pending,       /* ah_is_interrupt_pending */
        ar9300_get_pending_interrupts,     /* ah_get_pending_interrupts */
        ar9300_get_interrupts,             /* ah_get_interrupts */
        ar9300_set_interrupts,             /* ah_set_interrupts */
        ar9300_set_intr_mitigation_timer,  /* ah_set_intr_mitigation_timer */
        ar9300_get_intr_mitigation_timer,  /* ah_get_intr_mitigation_timer */
#if ATH_SUPPORT_WIFIPOS
        ar9300_read_loc_timer_reg,         /* ah_read_loc_timer_reg */
        ar9300_get_eeprom_chain_mask,    /* ah_get_eeprom_chain_mask*/
#endif
	    ar9300ForceVCS,
        ar9300SetDfs3StreamFix,
        ar9300Get3StreamSignature,

        /* 11n specific functions (NOT applicable to ar9300) */
        ar9300_set_11n_tx_desc,            /* ah_set_11n_tx_desc */
#if ATH_SUPPORT_WIFIPOS
        /* Update rxchain */
        ar9300_set_rx_chainmask,           /*ah_set_rx_chainmask*/
        /*Updating locationing register */
        ar9300_update_loc_ctl_reg,         /* ah_update_loc_ctl_reg */
#endif
        /* Start PAPRD functions  */
        ar9300_set_paprd_tx_desc,          /* ah_set_paprd_tx_desc */
        ar9300_paprd_init_table,           /* ah_paprd_init_table */
        ar9300_paprd_setup_gain_table,     /* ah_paprd_setup_gain_table */
        ar9300_paprd_create_curve,         /* ah_paprd_create_curve */
        ar9300_paprd_is_done,              /* ah_paprd_is_done */
        ar9300_enable_paprd,               /* ah_PAPRDEnable */
        ar9300_populate_paprd_single_table,/* ah_paprd_populate_table */
        ar9300_is_tx_done,                 /* ah_is_tx_done */
        ar9300_paprd_dec_tx_pwr,            /* ah_paprd_dec_tx_pwr*/
        ar9300_paprd_thermal_send,         /* ah_paprd_thermal_send */
        /* End PAPRD functions */
        ar9300_factory_defaults,           /* ah_factory_defaults */
#ifdef ATH_SUPPORT_TxBF
        /* TxBf specific functions */
        ar9300_set_11n_txbf_sounding,      /* ah_set_11n_tx_bf_sounding*/
#ifdef TXBF_TODO
        ar9300_set_11n_txbf_cal,           /* ah_set_11n_tx_bf_cal */
        ar9300_txbf_save_cv_compress,      /* ah_tx_bf_save_cv_compress */
        ar9300_txbf_save_cv_non_compress,  /* ah_tx_bf_save_cv_non_compress */
        ar9300_txbf_rc_update,             /* ah_tx_bf_rc_update */
        ar9300_fill_csi_frame,             /* ah_fill_csi_frame */
#endif
        ar9300_get_txbf_capabilities,      /* ah_get_tx_bf_capability */
        ar9300_read_key_cache_mac,         /* ah_read_key_cache_mac */
        ar9300_txbf_set_key,               /* ah_tx_bf_set_key*/
        ar9300_set_hw_cv_timeout,          /* ah_set_hw_cv_timeout*/
        ar9300_fill_txbf_capabilities,     /* ah_fill_tx_bf_cap*/
        ar9300_txbf_get_cv_cache_nr,       /* ah_tx_bf_get_cv_cache_nr */
        ar9300_reconfig_h_xfer_timeout,    /* ah_reconfig_h_xfer_timeout */
        ar9300_reset_lowest_txrate,        /* ah_reset_lowest_txrate */
        ar9300_get_perrate_txbf_flags,     /* ah_get_perrate_txbfflags */
#endif
        ar9300_set_11n_rate_scenario,      /* ah_set_11n_rate_scenario */
        ar9300_set_11n_aggr_first,         /* ah_set_11n_aggr_first */
        ar9300_set_11n_aggr_middle,        /* ah_set_11n_aggr_middle */
        ar9300_set_11n_aggr_last,          /* ah_set_11n_aggr_last */
        ar9300_clr_11n_aggr,               /* ah_clr_11n_aggr */
        ar9300_set_11n_rifs_burst_middle,  /* ah_set_11n_rifs_burst_middle */
        ar9300_set_11n_rifs_burst_last,    /* ah_set_11n_rifs_burst_last */
        ar9300_clr_11n_rifs_burst,         /* ah_clr_11n_rifs_burst */
        ar9300_set_11n_aggr_rifs_burst,    /* ah_set_11n_aggr_rifs_burst */
        ar9300_set_11n_rx_rifs,            /* ah_set_11n_rx_rifs */
        ar9300_set_smart_antenna,             /* ah_setSmartAntenna */
        ar9300_detect_bb_hang,             /* ah_detect_bb_hang */
        ar9300_detect_mac_hang,            /* ah_detect_mac_hang */
        ar9300_set_immunity,               /* ah_immunity */
        ar9300_get_hw_hangs,               /* ah_get_hang_types */
        ar9300_set_11n_burst_duration,     /* ah_set_11n_burst_duration */
        ar9300_set_11n_virtual_more_frag,  /* ah_set_11n_virtual_more_frag */
        ar9300_get_11n_ext_busy,           /* ah_get_11n_ext_busy */
        ar9300_get_ch_busy_pct,            /* ah_get_ch_busy_pct */
        ar9300_set_11n_mac2040,            /* ah_set_11n_mac2040 */
        ar9300_get_11n_rx_clear,           /* ah_get_11n_rx_clear */
        ar9300_set_11n_rx_clear,           /* ah_set_11n_rx_clear */
        ar9300_get_mib_cycle_counts_pct,   /* ah_get_mib_cycle_counts_pct */
        ar9300_dma_reg_dump,               /* ah_dma_reg_dump */

        /* force_ppm specific functions */
        ar9300_ppm_get_rssi_dump,          /* ah_ppm_get_rssi_dump */
        ar9300_ppm_arm_trigger,            /* ah_ppm_arm_trigger */
        ar9300_ppm_get_trigger,            /* ah_ppm_get_trigger */
        ar9300_ppm_force,                  /* ah_ppm_force */
        ar9300_ppm_un_force,               /* ah_ppm_un_force */
        ar9300_ppm_get_force_state,        /* ah_ppm_get_force_state */

        ar9300_get_spur_info,              /* ah_get_spur_info */
        ar9300_set_spur_info,              /* ah_get_spur_info */

        ar9300_get_min_cca_pwr,            /* ah_ar_get_noise_floor_val */

        ar9300_green_ap_ps_on_off,         /* ah_set_rx_green_ap_ps_on_off */
        ar9300_is_single_ant_power_save_possible, /* ah_is_single_ant_power_save_possible */

        /* radio measurement specific functions */
        ar9300_get_mib_cycle_counts,       /* ah_get_mib_cycle_counts */
        ar9300_get_vow_stats,              /* ah_get_vow_stats */
        ar9300_clear_mib_counters,         /* ah_clear_mib_counters */
#ifdef ATH_CCX
        ar9300_get_cca_threshold,          /* ah_get_cca_threshold */
        ar9300_get_cur_rssi,               /* ah_get_cur_rssi */
#endif
#if ATH_GEN_RANDOMNESS
        ar9300_get_rssi_chain0,            /* ah_get_rssi_chain0 */
#endif
#ifdef ATH_BT_COEX
        /* Bluetooth Coexistence functions */
        ar9300_set_bt_coex_info,           /* ah_set_bt_coex_info */
        ar9300_bt_coex_config,             /* ah_bt_coex_config */
        ar9300_bt_coex_set_qcu_thresh,     /* ah_bt_coex_set_qcu_thresh */
        ar9300_bt_coex_set_weights,        /* ah_bt_coex_set_weights */
        ar9300_bt_coex_setup_bmiss_thresh, /* ah_bt_coex_set_bmiss_thresh */
        ar9300_bt_coex_set_parameter,      /* ah_bt_coex_set_parameter */
        ar9300_bt_coex_disable,            /* ah_bt_coex_disable */
        ar9300_bt_coex_enable,             /* ah_bt_coex_enable */
        ar9300_get_bt_active_gpio,         /* ah_bt_coex_info*/
        ar9300_get_wlan_active_gpio,       /* ah__coex_wlan_info*/
#endif
        /* Generic Timer functions */
        ar9300_alloc_generic_timer,        /* ah_gentimer_alloc */
        ar9300_free_generic_timer,         /* ah_gentimer_free */
        ar9300_start_generic_timer,        /* ah_gentimer_start */
        ar9300_stop_generic_timer,         /* ah_gentimer_stop */
        ar9300_get_gen_timer_interrupts,   /* ah_gentimer_get_intr */

        ar9300_set_dcs_mode,               /* ah_set_dcs_mode */
        ar9300_get_dcs_mode,               /* ah_get_dcs_mode */
        
#if ATH_ANT_DIV_COMB
        ar9300_ant_div_comb_get_config,    /* ah_get_ant_dvi_comb_conf */
        ar9300_ant_div_comb_set_config,    /* ah_set_ant_dvi_comb_conf */
#endif

        ar9300_get_bb_panic_info,          /* ah_get_bb_panic_info */
        ar9300_handle_radar_bb_panic,      /* ah_handle_radar_bb_panic */
        ar9300_set_hal_reset_reason,       /* ah_set_hal_reset_reason */

#if ATH_PCIE_ERROR_MONITOR
        ar9300_start_pcie_error_monitor,   /* ah_start_pcie_error_monitor */
        ar9300_read_pcie_error_monitor,    /* ah_read_pcie_error_monitor*/
        ar9300_stop_pcie_error_monitor,    /* ah_stop_pcie_error_monitor*/
#endif /* ATH_PCIE_ERROR_MONITOR */

#if ATH_SUPPORT_SPECTRAL        
        /* Spectral scan */
        ar9300_configure_spectral_scan,    /* ah_ar_configure_spectral */
        ar9300_get_spectral_params,        /* ah_ar_get_spectral_config */
        ar9300_start_spectral_scan,        /* ah_ar_start_spectral_scan */
        ar9300_stop_spectral_scan,         /* ah_ar_stop_spectral_scan */
        ar9300_is_spectral_enabled,        /* ah_ar_is_spectral_enabled */
        ar9300_is_spectral_supported,      /* ah_ar_is_spectral_supported */
        ar9300_is_spectral_active,         /* ah_ar_is_spectral_active */
        ar9300_get_ctl_chan_nf,            /* ah_ar_get_ctl_nf */
        ar9300_get_ext_chan_nf,            /* ah_ar_get_ext_nf */
        ar9300_get_nominal_nf,             /* ah_ar_get_nominal_nf */
#endif  /*  ATH_SUPPORT_SPECTRAL */ 

#if ATH_SUPPORT_RAW_ADC_CAPTURE
        ar9300_enable_test_addac_mode,     /* ah_ar_enable_test_addac_mode */
        ar9300_disable_test_addac_mode,    /* ah_ar_disable_test_addac_mode */
        ar9300_begin_adc_capture,          /* ah_ar_begin_adc_capture */
        ar9300_retrieve_capture_data,      /* ah_ar_retrieve_capture_data */
        ar9300_calc_adc_ref_powers,        /* ah_ar_calculate_adc_ref_powers */
        ar9300_get_min_agc_gain,           /* ah_ar_get_min_agc_gain */
#endif

        ar9300_promisc_mode,               /* ah_promisc_mode */
        ar9300_read_pktlog_reg,            /* ah_read_pktlog_reg */
        ar9300_write_pktlog_reg,           /* ah_write_pktlog_reg */
        ar9300_set_proxy_sta,              /* ah_set_proxy_sta */
        ar9300_get_cal_intervals,          /* ah_get_cal_intervals */
#if ATH_SUPPORT_WIRESHARK
        ar9300_fill_radiotap_hdr,          /* ah_fill_radiotap_hdr */
#endif
#if ATH_TRAFFIC_FAST_RECOVER
        ar9300_get_pll3_sqsum_dvc,         /* ah_get_pll3_sqsum_dvc */
#endif
#ifdef ATH_SUPPORT_HTC
        AH_NULL,
#endif

#ifdef ATH_TX99_DIAG
        /* Tx99 functions */
#ifdef ATH_SUPPORT_HTC
        AH_NULL,
        AH_NULL,
        AH_NULL,
        AH_NULL,
        AH_NULL,
        AH_NULL,
        AH_NULL,
#else
        AH_NULL,
        AH_NULL,
        ar9300_tx99_channel_pwr_update,		/* ah_tx99channelpwrupdate */
        ar9300_tx99_start,					/* ah_tx99start */
        ar9300_tx99_stop,					/* ah_tx99stop */
        ar9300_tx99_chainmsk_setup,			/* ah_tx99_chainmsk_setup */
        ar9300_tx99_set_single_carrier,		/* ah_tx99_set_single_carrier */
#endif
#endif
        ar9300_chk_rssi_update_tx_pwr,
        ar9300_is_skip_paprd_by_greentx,   /* ah_is_skip_paprd_by_greentx */
        ar9300_hwgreentx_set_pal_spare,    /* ah_hwgreentx_set_pal_spare */
#if ATH_SUPPORT_MCI
        /* MCI Coexistence Functions */
        ar9300_mci_setup,                   /* ah_mci_setup */
        ar9300_mci_send_message,            /* ah_mci_send_message */
        ar9300_mci_get_interrupt,           /* ah_mci_get_interrupt */
        ar9300_mci_state,                   /* ah_mci_state */
        ar9300_mci_detach,                  /* ah_mci_detach */
#endif
        ar9300_reset_hw_beacon_proc_crc,   /* ah_reset_hw_beacon_proc_crc */
        ar9300_get_hw_beacon_rssi,         /* ah_get_hw_beacon_rssi */
        ar9300_set_hw_beacon_rssi_threshold,/*ah_set_hw_beacon_rssi_threshold*/
        ar9300_reset_hw_beacon_rssi,       /* ah_reset_hw_beacon_rssi */
        ar9300_mat_enable,                 /* ah_mat_enable */
        ar9300_dump_keycache,              /* ah_dump_keycache */
        ar9300_is_ani_noise_spur,         /* ah_is_ani_noise_spur */
#if ATH_SUPPORT_WIFIPOS
        ar9300_lean_channel_change,       /* ah_lean_channel_change */
        ar9300_disable_hwq,               /* ah_enable_hwq */
#endif
        ar9300_set_hw_beacon_proc,         /* ah_set_hw_beacon_proc */
        ar9300_set_ctl_pwr,                 /* ah_set_ctl_pwr */
        ar9300_set_txchainmaskopt,          /* ah_set_txchainmaskopt */
		ar9300_reset_nav,					/* ah_reset_nav */
        ar9300_get_smart_ant_tx_info,      /* ah_get_smart_ant_tx_info */
#if ATH_TxBF_DYNAMIC_LOF_ON_N_CHAIN_MASK
        ar9300_txbf_loforceon_update,       /* ah_txbf_loforceon_update */
#endif
        ar9300_disable_interrupts_on_exit, /* ah_disable_interrupts_on_exit */
#if ATH_GEN_RANDOMNESS
        ar9300_adc_data_read,              /*ah_adc_data_read*/
#endif
#if ATH_BT_COEX_3WIRE_MODE
        ar9300_enable_basic_3wire_btcoex, /* ah_enable_basic_3wire_btcoex */
        ar9300_disable_basic_3wire_btcoex, /* ah_disable_basic_3wire_btcoex */
#endif
        ar9300_set_temp_deg_c_chain,       /* ah_set_temp_deg_c_chain */
        ar9300_get_temp_deg_c,             /* ah_get_temp_deg_c */
    },

    ar9300_get_channel_edges,              /* ah_get_channel_edges */
    ar9300_get_wireless_modes,             /* ah_get_wireless_modes */
    ar9300_eeprom_read_word,               /* ah_eeprom_read */
#ifdef AH_SUPPORT_WRITE_EEPROM
    ar9300_eeprom_write,                   /* ah_eeprom_write */
#else
    AH_NULL,
#endif
    ar9300_eeprom_dump_support,            /* ah_eeprom_dump */
    ar9300_get_chip_power_limits,          /* ah_get_chip_power_limits */

    ar9300_get_nf_adjust,                  /* ah_get_nf_adjust */
    /* rest is zero'd by compiler */
};

/*
 * Read MAC version/revision information from Chip registers and initialize
 * local data structures.
 */
void
ar9300_read_revisions(struct ath_hal *ah)
{
    u_int32_t val;

    /* XXX verify if this is the correct way to read revision on Osprey */
    /* new SREV format for Sowl and later */
    val = OS_REG_READ(ah, AR_HOSTIF_REG(ah, AR_SREV));

    if (AH_PRIVATE(ah)->ah_devid == AR9300_DEVID_AR9340) {
        /* XXX: AR_SREV register in Wasp reads 0 */
        AH_PRIVATE(ah)->ah_mac_version = AR_SREV_VERSION_WASP;
    } else if(AH_PRIVATE(ah)->ah_devid == AR9300_DEVID_AR955X) {
        /* XXX: AR_SREV register in Scorpion reads 0 */
       AH_PRIVATE(ah)->ah_mac_version = AR_SREV_VERSION_SCORPION;
    } else if(AH_PRIVATE(ah)->ah_devid == AR9300_DEVID_AR953X) {
        /* XXX: AR_SREV register in HoneyBEE reads 0 */
       AH_PRIVATE(ah)->ah_mac_version = AR_SREV_VERSION_HONEYBEE;
    } else if(AH_PRIVATE(ah)->ah_devid == AR9300_DEVID_AR956X)  {
       /* XXX: AR_SREV register in Dragonfly reads 0 */
       AH_PRIVATE(ah)->ah_mac_version = AR_SREV_VERSION_DRAGONFLY;
    } else if(AH_PRIVATE(ah)->ah_devid == AR9300_DEVID_AR5500)  {
       /* XXX: AR_SREV register in Jet reads 0 */
       AH_PRIVATE(ah)->ah_mac_version = AR_SREV_VERSION_JET;
    } else {
        /*
         * Include 6-bit Chip Type (masked to 0)
         * to differentiate from pre-Sowl versions
         */
        AH_PRIVATE(ah)->ah_mac_version =
            (val & AR_SREV_VERSION2) >> AR_SREV_TYPE2_S;
    }
#ifdef AR9300_EMULATION_BB
    /* Osprey full system emulation does not have the correct SREV */
    AH_PRIVATE(ah)->ah_mac_version = AR_SREV_VERSION_OSPREY;
#endif
#if ATH_DRIVER_SIM
    AH_PRIVATE(ah)->ah_mac_version = AR_SREV_VERSION_OSPREY;
#endif
#ifdef JUPITER_EMULATION
    AH_PRIVATE(ah)->ah_mac_version = AR_SREV_VERSION_JUPITER;
#endif

#ifdef AR956X_EMULATION
    HDPRINTF(AH_NULL, HAL_DBG_UNMASKABLE,
        "macVersion: 0x%x\n", AH_PRIVATE(ah)->ah_mac_version);
    AH_PRIVATE(ah)->ah_mac_version = AR_SREV_VERSION_APHRODITE;
#endif

#ifdef AR9330_EMULATION
    HDPRINTF(AH_NULL, HAL_DBG_UNMASKABLE,
        "mac_version: 0x%x\n", AH_PRIVATE(ah)->ah_mac_version);
    AH_PRIVATE(ah)->ah_mac_version = AR_SREV_VERSION_HORNET;
#endif

#ifdef AR9340_EMULATION
    HDPRINTF(AH_NULL, HAL_DBG_UNMASKABLE,
        "mac_version: 0x%x\n", AH_PRIVATE(ah)->ah_mac_version);
#ifdef AR9550_EMULATION
    AH_PRIVATE(ah)->ah_mac_version = AR_SREV_VERSION_SCORPION;
#elif defined(AR9530_EMULATION)
    AH_PRIVATE(ah)->ah_mac_version = AR_SREV_VERSION_HONEYBEE;
#else
    AH_PRIVATE(ah)->ah_mac_version = AR_SREV_VERSION_WASP;
#endif
#ifdef AR9560_EMULATION
    AH_PRIVATE(ah)->ah_mac_version = AR_SREV_VERSION_DRAGONFLY;
#endif
#endif

#ifdef AR9485_EMULATION
    HDPRINTF(AH_NULL, HAL_DBG_UNMASKABLE,
        "mac_version: 0x%x\n", AH_PRIVATE(ah)->ah_mac_version);
    AH_PRIVATE(ah)->ah_mac_version = AR_SREV_VERSION_POSEIDON;
#endif

#ifdef AH_SUPPORT_HORNET
    /*
     *  EV74984, due to Hornet 1.1 didn't update WMAC revision,
     *  so that have to read SoC's revision ID instead
     */
    if (AH_PRIVATE(ah)->ah_mac_version == AR_SREV_VERSION_HORNET) {
#ifdef _WIN64
#define AR_SOC_RST_REVISION_ID         0xB8060090ULL
#else
#define AR_SOC_RST_REVISION_ID         0xB8060090
#endif
#ifdef ART_BUILD
        MyRegisterRead(AR_SOC_RST_REVISION_ID, &val);
        if ((val & AR_SREV_REVISION_HORNET_11_MASK) ==
            AR_SREV_REVISION_HORNET_11)
        {
            AH_PRIVATE(ah)->ah_mac_rev = AR_SREV_REVISION_HORNET_11;
        } else {
            AH_PRIVATE(ah)->ah_mac_rev = MS(val, AR_SREV_REVISION2);
        }
#else
#define REG_READ(_reg)                 *((volatile u_int32_t *)(_reg))
        if ((REG_READ(AR_SOC_RST_REVISION_ID) & AR_SREV_REVISION_HORNET_11_MASK)
            == AR_SREV_REVISION_HORNET_11)
        {
            AH_PRIVATE(ah)->ah_mac_rev = AR_SREV_REVISION_HORNET_11;
        } else {
            AH_PRIVATE(ah)->ah_mac_rev = MS(val, AR_SREV_REVISION2);
        }
#undef REG_READ
#endif
#undef AR_SOC_RST_REVISION_ID
    } else
#endif
    if (AH_PRIVATE(ah)->ah_mac_version == AR_SREV_VERSION_WASP)
    {
#ifdef _WIN64
#define AR_SOC_RST_REVISION_ID         0xB8060090ULL
#else
#define AR_SOC_RST_REVISION_ID         0xB8060090
#endif
#ifdef ART_BUILD
        MyRegisterRead(AR_SOC_RST_REVISION_ID, &val);
        AH_PRIVATE(ah)->ah_mac_rev = val & AR_SREV_REVISION_WASP_MASK;
#else
#define REG_READ(_reg)                 *((volatile u_int32_t *)(_reg))

        AH_PRIVATE(ah)->ah_mac_rev = 
            REG_READ(AR_SOC_RST_REVISION_ID) & AR_SREV_REVISION_WASP_MASK; 
#undef REG_READ
#endif
#undef AR_SOC_RST_REVISION_ID
    } else if (AH_PRIVATE(ah)->ah_mac_version == AR_SREV_VERSION_HONEYBEE) {
#ifdef _WIN64
#define AR_SOC_RST_REVISION_ID         0xB8060090ULL
#else
#define AR_SOC_RST_REVISION_ID         0xB8060090
#endif
#ifdef ART_BUILD
#ifdef MDK_AP
        val = FullAddrRead(AR_SOC_RST_REVISION_ID);
#else
        MyRegisterRead(AR_SOC_RST_REVISION_ID, &val);
#endif
        AH_PRIVATE(ah)->ah_mac_rev = val & AR_SREV_REVISION_HONEYBEE_MASK;
#else
#define REG_READ(_reg)                 *((volatile u_int32_t *)(_reg))

        AH_PRIVATE(ah)->ah_mac_rev =
            REG_READ(AR_SOC_RST_REVISION_ID) & AR_SREV_REVISION_HONEYBEE_MASK;
#undef REG_READ
#endif
#undef AR_SOC_RST_REVISION_ID
    } else {
        AH_PRIVATE(ah)->ah_mac_rev = MS(val, AR_SREV_REVISION2);
    }

    if (AR_SREV_JUPITER(ah) || AR_SREV_APHRODITE(ah)) {
        AH_PRIVATE(ah)->ah_is_pci_express = true;
    }
    else {
        AH_PRIVATE(ah)->ah_is_pci_express = 
            (val & AR_SREV_TYPE2_HOST_MODE) ? 0 : 1;
    }
    
#ifdef JUPITER_EMULATION
    /* optionally check emulation versin and revision match */
    if (OS_REG_READ(ah, AR_HOSTIF_REG(ah, AR_SREV)) ==
        AR_SREV_VERSION_JUPITER_10_EMU)
    {
        HALASSERT( AH_PRIVATE(ah)->ah_mac_rev == AR_SREV_REVISION_JUPITER_10);
    }
    if (OS_REG_READ(ah, AR_HOSTIF_REG(ah, AR_SREV)) ==
        AR_SREV_VERSION_JUPITER_20_EMU)
    {
        HALASSERT( AH_PRIVATE(ah)->ah_mac_rev == AR_SREV_REVISION_JUPITER_20);
    }
#endif
}

/*
 * Attach for an AR9300 part.
 */
struct ath_hal *
ar9300_attach(u_int16_t devid, HAL_ADAPTER_HANDLE osdev, HAL_SOFTC sc,
    HAL_BUS_TAG st, HAL_BUS_HANDLE sh, HAL_BUS_TYPE bustype,
    asf_amem_instance_handle amem_handle,
    struct hal_reg_parm *hal_conf_parm, HAL_STATUS *status)
{
    struct ath_hal_9300     *ahp;
    struct ath_hal          *ah;
    struct ath_hal_private  *ahpriv;
    HAL_STATUS              ecode;
    #ifdef ART_BUILD
    u_int32_t tmp_reg;   // store bootstrap
    u_int32_t tmp_reg2;  // store revision_id
    #endif          
#ifndef AR9340_EMULATION
    u_int32_t refclkmask;
#endif
     HAL_NO_INTERSPERSED_READS;

    /* NB: memory is returned zero'd */
    ahp = ar9300_new_state(
        devid, osdev, sc, st, sh, bustype, amem_handle, hal_conf_parm, status);
    if (ahp == AH_NULL) {
        return AH_NULL;
    }
    ah = &ahp->ah_priv.priv.h;
    ar9300_init_offsets(ah, devid);
    ahpriv = AH_PRIVATE(ah);
    AH_PRIVATE(ah)->ah_bustype = bustype;

#if ATH_DRIVER_SIM
    if (devid == AR9300_DEVID_SIM_PCIE) {
        if (AHSIM_AR9300_attach(ah) == false) {
            ecode = HAL_EIO;
            return ah;
        }

        if (AHSIM_register_sim_device_instance(devid, ah) == false) {
            ecode = HAL_EIO;
            goto bad;
        }
    }
#endif

    /* interrupt mitigation */
#ifdef AR9300_INT_MITIGATION
    if (ahpriv->ah_config.ath_hal_intr_mitigation_rx != 0) {
        ahp->ah_intr_mitigation_rx = true;
    }
#else
    /* Enable Rx mitigation (default) */
    ahp->ah_intr_mitigation_rx = true;
    ahpriv->ah_config.ath_hal_intr_mitigation_rx = 1;

#endif
#ifdef HOST_OFFLOAD
    /* Reset default Rx mitigation values for Hornet */
    if (AR_SREV_HORNET(ah)) {
        ahp->ah_intr_mitigation_rx = false;
#ifdef AR9300_INT_MITIGATION
        ahpriv->ah_config.ath_hal_intr_mitigation_rx = 0;
#endif
    }
#endif

    if (ahpriv->ah_config.ath_hal_intr_mitigation_tx != 0) {
        ahp->ah_intr_mitigation_tx = true;
    }

    /*
     * Read back AR_WA into a permanent copy and set bits 14 and 17. 
     * We need to do this to avoid RMW of this register. 
     * Do this before calling ar9300_set_reset_reg. 
     * If not, the AR_WA register which was inited via EEPROM
     * will get wiped out.
     */
    ahp->ah_wa_reg_val = OS_REG_READ(ah,  AR_HOSTIF_REG(ah, AR_WA));
    /* Set Bits 14 and 17 in the AR_WA register. */
    ahp->ah_wa_reg_val |=
        AR_WA_D3_TO_L1_DISABLE | AR_WA_ASPM_TIMER_BASED_DISABLE;
    
    if (!ar9300_set_reset_reg(ah, HAL_RESET_POWER_ON)) {    /* reset chip */
        HDPRINTF(ah, HAL_DBG_RESET, "%s: couldn't reset chip\n", __func__);
        ecode = HAL_EIO;
        goto bad;
    }

    if (AR_SREV_JUPITER(ah)
#if ATH_WOW_OFFLOAD
        && !HAL_WOW_CTRL(ah, HAL_WOW_OFFLOAD_SET_4004_BIT14)
#endif
        )
    {
        /* Jupiter doesn't need bit 14 to be set. */
        ahp->ah_wa_reg_val &= ~AR_WA_D3_TO_L1_DISABLE;
        OS_REG_WRITE(ah, AR_HOSTIF_REG(ah, AR_WA), ahp->ah_wa_reg_val);
    }

#if ATH_SUPPORT_MCI
    if (AR_SREV_JUPITER(ah) || AR_SREV_APHRODITE(ah)) {
        ah->ah_bt_coex_set_weights = ar9300_mci_bt_coex_set_weights;
        ah->ah_bt_coex_disable = ar9300_mci_bt_coex_disable;
        ah->ah_bt_coex_enable = ar9300_mci_bt_coex_enable;
        ahp->ah_mci_ready = false;
        ahp->ah_mci_bt_state = MCI_BT_SLEEP;
        ahp->ah_mci_coex_major_version_wlan = MCI_GPM_COEX_MAJOR_VERSION_WLAN;
        ahp->ah_mci_coex_minor_version_wlan = MCI_GPM_COEX_MINOR_VERSION_WLAN;
        ahp->ah_mci_coex_major_version_bt = MCI_GPM_COEX_MAJOR_VERSION_DEFAULT;
        ahp->ah_mci_coex_minor_version_bt = MCI_GPM_COEX_MINOR_VERSION_DEFAULT;
        ahp->ah_mci_coex_bt_version_known = false;
        ahp->ah_mci_coex_2g5g_update = true; /* track if 2g5g status sent */
        /* will be updated before boot up sequence */
        ahp->ah_mci_coex_is_2g = true;
        ahp->ah_mci_coex_wlan_channels_update = false;
        ahp->ah_mci_coex_wlan_channels[0] = 0x00000000;
        ahp->ah_mci_coex_wlan_channels[1] = 0xffffffff;
        ahp->ah_mci_coex_wlan_channels[2] = 0xffffffff;
        ahp->ah_mci_coex_wlan_channels[3] = 0x7fffffff;
        ahp->ah_mci_query_bt = true; /* In case WLAN start after BT */
        ahp->ah_mci_unhalt_bt_gpm = true; /* Send UNHALT at beginning */
        ahp->ah_mci_halted_bt_gpm = false; /* Allow first HALT */
        ahp->ah_mci_need_flush_btinfo = false;
        ahp->ah_mci_wlan_cal_seq = 0;
        ahp->ah_mci_wlan_cal_done = 0;
    }
#endif /* ATH_SUPPORT_MCI */

#if ATH_WOW_OFFLOAD
    ahp->ah_mcast_filter_l32_set = 0;
    ahp->ah_mcast_filter_u32_set = 0;
#endif
#if ATH_BT_COEX_3WIRE_MODE
    if (AR_SREV_AR9580(ah)) {
        /* write AAAA_1111 to MAC_PCU_BLUETOOTH_BT_WEIGHT0 register
         * and ffff_ff20 to MAC_PCU_BLUETOOTH_WL_WEIGHT0 register.
         * This should allow the following.
         * 1. Low priority BT transaction to go through when WLAN is idle.
         * 2. WLAN TX/RX will stomp low priority BT.
         * 3. High priority BT will stomp normal WLAN TX/RX.
         * 4. High priority WLAN TX/RX will stomp any BT.
         */
#define DEFAULT_BT_WEIGHT   0xAAAA1111
#define DEFAULT_WL_WEIGHT   0xffffff20
        ah->ah_3wire_bt_coex_enable = 0;
        ah->ah_bt_coex_bt_weight = DEFAULT_BT_WEIGHT;
        ah->ah_bt_coex_wl_weight = DEFAULT_WL_WEIGHT;
    }
#endif

#ifndef AR9330_EMULATION
    if (AR_SREV_HORNET(ah)) {
#ifdef AH_SUPPORT_HORNET
        if (!AR_SREV_HORNET_11(ah)) {
            /*
             * Do not check bootstrap register, which cannot be trusted
             * due to s26 switch issue on CUS164/AP121.
             */
            ahp->clk_25mhz = 1;
            HDPRINTF(AH_NULL, HAL_DBG_UNMASKABLE, "Bootstrap clock 25MHz\n");
        } else {
            /* check bootstrap clock setting */
#ifdef _WIN64
#define AR_SOC_SEL_25M_40M         0xB80600ACULL
#else
#define AR_SOC_SEL_25M_40M         0xB80600AC
#endif /* _WIN64 */
#ifdef ART_BUILD
            MyRegisterRead(AR_SOC_SEL_25M_40M, &tmp_reg);
            if (tmp_reg & 0x1) {
                ahp->clk_25mhz = 0;
                ath_hal_printf(ah, "Bootstrap clock 40MHz\n");
            } else {
                ahp->clk_25mhz = 1;
                ath_hal_printf(ah, "Bootstrap clock 25MHz\n");
            }
#else
#define REG_WRITE(_reg, _val)    *((volatile u_int32_t *)(_reg)) = (_val);
#define REG_READ(_reg)          (*((volatile u_int32_t *)(_reg)))
            if (REG_READ(AR_SOC_SEL_25M_40M) & 0x1) {
                ahp->clk_25mhz = 0;
                HDPRINTF(AH_NULL, HAL_DBG_UNMASKABLE,
                    "Bootstrap clock 40MHz\n");
            } else {
                ahp->clk_25mhz = 1;
                HDPRINTF(AH_NULL, HAL_DBG_UNMASKABLE,
                    "Bootstrap clock 25MHz\n");
            }
#undef REG_READ
#undef REG_WRITE
#undef AR_SOC_SEL_25M_40M
#endif /* ART_BUILD */
        }
#endif /* AH_SUPPORT_HORNET */
    }
#endif /* AR9330_EMULATION */

#ifndef AR9340_EMULATION
    if (AR_SREV_WASP(ah) || AR_SREV_SCORPION(ah) || AR_SREV_DRAGONFLY(ah) || AR_SREV_JET(ah)) {
        /* check bootstrap clock setting */
#ifdef _WIN64
#define AR9340_SOC_SEL_25M_40M         0xB80600B0ULL
#else
#define AR9340_SOC_SEL_25M_40M         0xB80600B0
#endif
		if (AR_SREV_DRAGONFLY(ah) || AR_SREV_JET(ah)){
			refclkmask = (1 << 2); /* 0 - 25MHz   1 - 40 MHz */
		}
		else {
			refclkmask = (1 << 4); /* 0 - 25MHz   1 - 40 MHz */
		}
#ifdef ART_BUILD
#ifdef MDK_AP
        tmp_reg = FullAddrRead(AR9340_SOC_SEL_25M_40M);
#else
        MyRegisterRead(AR9340_SOC_SEL_25M_40M, &tmp_reg);
#endif        
#if defined(AR5500_EMULATION)
        ahp->clk_25mhz = 0; // Assume 40Mhz due to wrong bootstrap in JET EMU
#else
        if (tmp_reg & refclkmask) {
             ahp->clk_25mhz = 0;
        }
        else {
             ahp->clk_25mhz = 1;
        }
#endif
#else
#define REG_READ(_reg)          (*((volatile u_int32_t *)(_reg)))
#ifndef MDK_AP          // MDK_AP is defined only for NART.
        if (REG_READ(AR9340_SOC_SEL_25M_40M) & refclkmask) {
#else
        if (FullAddrRead(AR9340_SOC_SEL_25M_40M) & refclkmask) {
#endif
            ahp->clk_25mhz = 0;
            HDPRINTF(AH_NULL, HAL_DBG_UNMASKABLE, "Bootstrap clock 40MHz\n");
        } else {
            ahp->clk_25mhz = 1;
            HDPRINTF(AH_NULL, HAL_DBG_UNMASKABLE, "Bootstrap clock 25MHz\n");
        }
#undef REG_READ
#undef AR9340_SOC_SEL_25M_40M
#endif
    }
#endif

        if (AR_SREV_HONEYBEE(ah)) {
            ahp->clk_25mhz = 1;
        }

        ar9300_init_pll(ah, AH_NULL);
#ifdef AR9300_EMULATION
    OS_DELAY(1000);
#endif

    if (!ar9300_set_power_mode(ah, HAL_PM_AWAKE, true)) {
        HDPRINTF(ah, HAL_DBG_RESET, "%s: couldn't wakeup chip\n", __func__);
        ecode = HAL_EIO;
        goto bad;
    }

    /* No serialization of Register Accesses needed. */
    ahpriv->ah_config.ath_hal_serialize_reg_mode = SER_REG_MODE_OFF;
    HDPRINTF(ah, HAL_DBG_RESET, "%s: ath_hal_serialize_reg_mode is %d\n",
             __func__, ahpriv->ah_config.ath_hal_serialize_reg_mode);

    /*
     * Add mac revision check when needed.
     * - Osprey 1.0 and 2.0 no longer supported.
     */
    if (((ahpriv->ah_mac_version == AR_SREV_VERSION_OSPREY) &&
          (ahpriv->ah_mac_rev <= AR_SREV_REVISION_OSPREY_20)) ||
        (ahpriv->ah_mac_version != AR_SREV_VERSION_OSPREY &&
        ahpriv->ah_mac_version != AR_SREV_VERSION_WASP && 
        ahpriv->ah_mac_version != AR_SREV_VERSION_HORNET &&
        ahpriv->ah_mac_version != AR_SREV_VERSION_POSEIDON &&
        ahpriv->ah_mac_version != AR_SREV_VERSION_SCORPION &&
	ahpriv->ah_mac_version != AR_SREV_VERSION_HONEYBEE &&
	    ahpriv->ah_mac_version != AR_SREV_VERSION_DRAGONFLY &&
        ahpriv->ah_mac_version != AR_SREV_VERSION_JUPITER &&
        ahpriv->ah_mac_version != AR_SREV_VERSION_APHRODITE &&
        ahpriv->ah_mac_version != AR_SREV_VERSION_JET) ) {
        HDPRINTF(ah, HAL_DBG_RESET,
            "%s: Mac Chip Rev 0x%02x.%x is not supported by this driver\n",
            __func__,
            ahpriv->ah_mac_version,
            ahpriv->ah_mac_rev);
        ecode = HAL_ENOTSUPP;
        goto bad;
    }
    ahpriv->ah_phy_rev = OS_REG_READ(ah, AR_PHY_CHIP_ID);

    /* Setup supported calibrations */
    ahp->ah_iq_cal_data.cal_data = &iq_cal_single_sample;
    ahp->ah_supp_cals = IQ_MISMATCH_CAL;

    /* Enable ANI */
    ahp->ah_ani_function = HAL_ANI_ALL;

#ifdef AR9300_EMULATION
        /* Disable RIFS */
        ahp->ah_rifs_enabled = false;
#else
    /* Enable RIFS */
    ahp->ah_rifs_enabled = true;
#endif

    /* by default, stop RX also in abort txdma, due to
       "Unable to stop TxDMA" msg observed */
    ahp->ah_abort_txdma_norx = true;

    /* do not use optional tx chainmask by default */
    ahp->ah_tx_chainmaskopt = 0;

    ahp->ah_skip_rx_iq_cal = false;
    ahp->ah_rx_cal_complete = false;
    ahp->ah_rx_cal_chan = 0;
    ahp->ah_rx_cal_chan_flag = 0;

    HDPRINTF(ah, HAL_DBG_RESET,
        "%s: This Mac Chip Rev 0x%02x.%x is \n", __func__,
        ahpriv->ah_mac_version,
        ahpriv->ah_mac_rev);

    if (AR_SREV_HORNET_12(ah)) {
        /* mac */
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_CORE],
            ar9331_hornet1_2_mac_core,
            ARRAY_LENGTH(ar9331_hornet1_2_mac_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_POST],
            ar9331_hornet1_2_mac_postamble,
            ARRAY_LENGTH(ar9331_hornet1_2_mac_postamble), 5);

        /* bb */
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_CORE],
            ar9331_hornet1_2_baseband_core,
            ARRAY_LENGTH(ar9331_hornet1_2_baseband_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_POST],
            ar9331_hornet1_2_baseband_postamble,
            ARRAY_LENGTH(ar9331_hornet1_2_baseband_postamble), 5);

        /* radio */
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_CORE], 
            ar9331_hornet1_2_radio_core,
            ARRAY_LENGTH(ar9331_hornet1_2_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_POST], NULL, 0, 0);

        /* soc */
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_PRE],
            ar9331_hornet1_2_soc_preamble,
            ARRAY_LENGTH(ar9331_hornet1_2_soc_preamble), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_POST],
            ar9331_hornet1_2_soc_postamble,
            ARRAY_LENGTH(ar9331_hornet1_2_soc_postamble), 2);

        /* rx/tx gain */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
            ar9331_common_rx_gain_hornet1_2, 
            ARRAY_LENGTH(ar9331_common_rx_gain_hornet1_2), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
            ar9331_modes_lowest_ob_db_tx_gain_hornet1_2, 
            ARRAY_LENGTH(ar9331_modes_lowest_ob_db_tx_gain_hornet1_2), 5);

        ahpriv->ah_config.ath_hal_pcie_power_save_enable = 0;

        /* Japan 2484Mhz CCK settings */
        INIT_INI_ARRAY(&ahp->ah_ini_japan2484,
            ar9331_hornet1_2_baseband_core_txfir_coeff_japan_2484,
            ARRAY_LENGTH(
                ar9331_hornet1_2_baseband_core_txfir_coeff_japan_2484), 2);
    
#if 0 /* ATH_WOW */
        /* SerDes values during WOW sleep */
        INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_wow, ar9300_pcie_phy_awow,
                ARRAY_LENGTH(ar9300_pcie_phy_awow), 2);
#endif
#ifdef AR9330_EMULATION
        INIT_INI_ARRAY(&ahp->ah_ini_modes_additional, NULL, 0, 0);
#else
        /* additional clock settings */
        if (AH9300(ah)->clk_25mhz) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_additional,
                ar9331_hornet1_2_xtal_25M,
                ARRAY_LENGTH(ar9331_hornet1_2_xtal_25M), 2);
        } else {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_additional,
                ar9331_hornet1_2_xtal_40M,
                ARRAY_LENGTH(ar9331_hornet1_2_xtal_40M), 2);
        }
#endif

#ifdef AR9330_EMULATION
        /* mac emu */
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_CORE],
            ar9330Common_mac_osprey2_0_emulation,
            ARRAY_LENGTH(ar9330Common_mac_osprey2_0_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_POST], NULL, 0, 0);

        /* bb emu */
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_CORE],
            ar9330Common_osprey2_0_emulation,
            ARRAY_LENGTH(ar9330Common_osprey2_0_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_POST],
            ar9330Modes_osprey2_0_emulation,
            ARRAY_LENGTH(ar9330Modes_osprey2_0_emulation), 5);

        /* radio emu */
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_CORE],
            ar9330_130nm_radio_emulation,
            ARRAY_LENGTH(ar9330_130nm_radio_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_POST], NULL, 0, 0);

        /* soc emu */
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_POST], NULL, 0, 0);

        /* rx gain emu */
        INIT_INI_ARRAY(&ahp->ah_ini_rx_gain_emu,
            ar9330Common_rx_gain_osprey2_0_emulation,
            ARRAY_LENGTH(ar9330Common_rx_gain_osprey2_0_emulation), 2);
#endif
    } else if (AR_SREV_HORNET_11(ah)) {
        /* mac */
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_CORE],
            ar9331_hornet1_1_mac_core,
            ARRAY_LENGTH(ar9331_hornet1_1_mac_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_POST],
            ar9331_hornet1_1_mac_postamble,
            ARRAY_LENGTH(ar9331_hornet1_1_mac_postamble), 5);

        /* bb */
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_CORE],
            ar9331_hornet1_1_baseband_core,
            ARRAY_LENGTH(ar9331_hornet1_1_baseband_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_POST],
            ar9331_hornet1_1_baseband_postamble,
            ARRAY_LENGTH(ar9331_hornet1_1_baseband_postamble), 5);

        /* radio */
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_CORE], 
            ar9331_hornet1_1_radio_core,
            ARRAY_LENGTH(ar9331_hornet1_1_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_POST], NULL, 0, 0);

        /* soc */
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_PRE],
            ar9331_hornet1_1_soc_preamble,
            ARRAY_LENGTH(ar9331_hornet1_1_soc_preamble), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_POST],
            ar9331_hornet1_1_soc_postamble,
            ARRAY_LENGTH(ar9331_hornet1_1_soc_postamble), 2);

        /* rx/tx gain */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
            ar9331_common_rx_gain_hornet1_1, 
            ARRAY_LENGTH(ar9331_common_rx_gain_hornet1_1), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
            ar9331_modes_lowest_ob_db_tx_gain_hornet1_1, 
            ARRAY_LENGTH(ar9331_modes_lowest_ob_db_tx_gain_hornet1_1), 5);

        ahpriv->ah_config.ath_hal_pcie_power_save_enable = 0;

        /* Japan 2484Mhz CCK settings */
        INIT_INI_ARRAY(&ahp->ah_ini_japan2484,
            ar9331_hornet1_1_baseband_core_txfir_coeff_japan_2484,
            ARRAY_LENGTH(
                ar9331_hornet1_1_baseband_core_txfir_coeff_japan_2484), 2);


#ifdef AR9330_EMULATION
        INIT_INI_ARRAY(&ahp->ah_ini_modes_additional, NULL, 0, 0);
#else
        /* additional clock settings */
        if (AH9300(ah)->clk_25mhz) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_additional,
                ar9331_hornet1_1_xtal_25M,
                ARRAY_LENGTH(ar9331_hornet1_1_xtal_25M), 2);
        } else {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_additional,
                ar9331_hornet1_1_xtal_40M,
                ARRAY_LENGTH(ar9331_hornet1_1_xtal_40M), 2);
        }
#endif

#ifdef AR9330_EMULATION
        /* mac emu */
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_CORE],
            ar9330Common_mac_osprey2_0_emulation,
            ARRAY_LENGTH(ar9330Common_mac_osprey2_0_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_POST], NULL, 0, 0);

        /* bb emu */
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_CORE],
            ar9330Common_osprey2_0_emulation,
            ARRAY_LENGTH(ar9330Common_osprey2_0_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_POST],
            ar9330Modes_osprey2_0_emulation,
            ARRAY_LENGTH(ar9330Modes_osprey2_0_emulation), 5);

        /* radio emu */
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_CORE],
            ar9330_130nm_radio_emulation,
            ARRAY_LENGTH(ar9330_130nm_radio_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_POST], NULL, 0, 0);

        /* soc emu */
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_POST], NULL, 0, 0);

        /* rx gain emu */
        INIT_INI_ARRAY(&ahp->ah_ini_rx_gain_emu,
            ar9330Common_rx_gain_osprey2_0_emulation,
            ARRAY_LENGTH(ar9330Common_rx_gain_osprey2_0_emulation), 2);
#endif
       } else if (AR_SREV_POSEIDON_11_OR_LATER(ah)) {
        /* mac */
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_CORE],
            ar9485_poseidon1_1_mac_core, 
            ARRAY_LENGTH( ar9485_poseidon1_1_mac_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_POST],
            ar9485_poseidon1_1_mac_postamble, 
            ARRAY_LENGTH(ar9485_poseidon1_1_mac_postamble), 5);

        /* bb */
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_PRE], 
            ar9485_poseidon1_1, ARRAY_LENGTH(ar9485_poseidon1_1), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_CORE],
            ar9485_poseidon1_1_baseband_core, 
            ARRAY_LENGTH(ar9485_poseidon1_1_baseband_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_POST],
            ar9485_poseidon1_1_baseband_postamble, 
            ARRAY_LENGTH(ar9485_poseidon1_1_baseband_postamble), 5);

        /* radio */
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_CORE], 
            ar9485_poseidon1_1_radio_core, 
            ARRAY_LENGTH(ar9485_poseidon1_1_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_POST],
            ar9485_poseidon1_1_radio_postamble, 
            ARRAY_LENGTH(ar9485_poseidon1_1_radio_postamble), 2);

        /* soc */
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_PRE],
            ar9485_poseidon1_1_soc_preamble, 
            ARRAY_LENGTH(ar9485_poseidon1_1_soc_preamble), 2);

        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_POST], NULL, 0, 0);

        /* rx/tx gain */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain, 
            ar9485_common_wo_xlna_rx_gain_poseidon1_1, 
            ARRAY_LENGTH(ar9485_common_wo_xlna_rx_gain_poseidon1_1), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain, 
            ar9485_modes_lowest_ob_db_tx_gain_poseidon1_1, 
            ARRAY_LENGTH(ar9485_modes_lowest_ob_db_tx_gain_poseidon1_1), 5);

        /* Japan 2484Mhz CCK settings */
        INIT_INI_ARRAY(&ahp->ah_ini_japan2484,
            ar9485_poseidon1_1_baseband_core_txfir_coeff_japan_2484,
            ARRAY_LENGTH(
                ar9485_poseidon1_1_baseband_core_txfir_coeff_japan_2484), 2);

        /* Load PCIE SERDES settings from INI */
        if (ahpriv->ah_config.ath_hal_pcie_clock_req) {
            /* Pci-e Clock Request = 1 */
            if (ahpriv->ah_config.ath_hal_pll_pwr_save 
                & AR_PCIE_PLL_PWRSAVE_CONTROL)
            {
                /* Sleep Setting */
                if (ahpriv->ah_config.ath_hal_pll_pwr_save & 
                    AR_PCIE_PLL_PWRSAVE_ON_D3) 
                {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes, 
                        ar9485_poseidon1_1_pcie_phy_clkreq_enable_L1,
                        ARRAY_LENGTH(
                           ar9485_poseidon1_1_pcie_phy_clkreq_enable_L1),
                        2);
                } else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes, 
                        ar9485_poseidon1_1_pcie_phy_pll_on_clkreq_enable_L1,
                        ARRAY_LENGTH(
                           ar9485_poseidon1_1_pcie_phy_pll_on_clkreq_enable_L1),
                        2);
                }    
                /* Awake Setting */
                if (ahpriv->ah_config.ath_hal_pll_pwr_save & 
                    AR_PCIE_PLL_PWRSAVE_ON_D0)
                {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power, 
                        ar9485_poseidon1_1_pcie_phy_clkreq_enable_L1,
                        ARRAY_LENGTH(
                           ar9485_poseidon1_1_pcie_phy_clkreq_enable_L1),
                        2);
                } else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power, 
                        ar9485_poseidon1_1_pcie_phy_pll_on_clkreq_enable_L1,
                        ARRAY_LENGTH(
                           ar9485_poseidon1_1_pcie_phy_pll_on_clkreq_enable_L1),
                        2);
                }    
                
            } else {
                /*Use driver default setting*/
                /* Sleep Setting */
                INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes, 
                    ar9485_poseidon1_1_pcie_phy_clkreq_enable_L1,
                    ARRAY_LENGTH(ar9485_poseidon1_1_pcie_phy_clkreq_enable_L1), 
                    2);
                /* Awake Setting */
                INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power, 
                    ar9485_poseidon1_1_pcie_phy_clkreq_enable_L1,
                    ARRAY_LENGTH(ar9485_poseidon1_1_pcie_phy_clkreq_enable_L1), 
                    2);
            }
        } else {
            /* Pci-e Clock Request = 0 */
            if (ahpriv->ah_config.ath_hal_pll_pwr_save 
                & AR_PCIE_PLL_PWRSAVE_CONTROL)
            {
                /* Sleep Setting */
                if (ahpriv->ah_config.ath_hal_pll_pwr_save & 
                    AR_PCIE_PLL_PWRSAVE_ON_D3) 
                {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes, 
                        ar9485_poseidon1_1_pcie_phy_clkreq_disable_L1,
                        ARRAY_LENGTH(
                          ar9485_poseidon1_1_pcie_phy_clkreq_disable_L1),
                        2);
                } else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes, 
                        ar9485_poseidon1_1_pcie_phy_pll_on_clkreq_disable_L1,
                        ARRAY_LENGTH(
                          ar9485_poseidon1_1_pcie_phy_pll_on_clkreq_disable_L1),
                        2);
                }    
                /* Awake Setting */
                if (ahpriv->ah_config.ath_hal_pll_pwr_save & 
                    AR_PCIE_PLL_PWRSAVE_ON_D0)
                {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power, 
                        ar9485_poseidon1_1_pcie_phy_clkreq_disable_L1,
                        ARRAY_LENGTH(
                          ar9485_poseidon1_1_pcie_phy_clkreq_disable_L1),
                        2);
                } else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power, 
                        ar9485_poseidon1_1_pcie_phy_pll_on_clkreq_disable_L1,
                        ARRAY_LENGTH(
                          ar9485_poseidon1_1_pcie_phy_pll_on_clkreq_disable_L1),
                        2);
                }    
                
            } else {
                /*Use driver default setting*/
                /* Sleep Setting */
                INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes, 
                    ar9485_poseidon1_1_pcie_phy_clkreq_disable_L1,
                    ARRAY_LENGTH(ar9485_poseidon1_1_pcie_phy_clkreq_disable_L1),
                    2);
                /* Awake Setting */
                INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power, 
                    ar9485_poseidon1_1_pcie_phy_clkreq_disable_L1,
                    ARRAY_LENGTH(ar9485_poseidon1_1_pcie_phy_clkreq_disable_L1),
                    2);
            }
        }
        /* pcie ps setting will honor registry setting, default is 0 */
        //ahpriv->ah_config.ath_hal_pciePowerSaveEnable = 0;    
   } else if (AR_SREV_POSEIDON(ah)) {
        /* mac */
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_CORE],
            ar9485_poseidon1_0_mac_core,
            ARRAY_LENGTH(ar9485_poseidon1_0_mac_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_POST],
            ar9485_poseidon1_0_mac_postamble,
            ARRAY_LENGTH(ar9485_poseidon1_0_mac_postamble), 5);

        /* bb */
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_PRE], 
            ar9485_poseidon1_0, 
            ARRAY_LENGTH(ar9485_poseidon1_0), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_CORE],
            ar9485_poseidon1_0_baseband_core,
            ARRAY_LENGTH(ar9485_poseidon1_0_baseband_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_POST],
            ar9485_poseidon1_0_baseband_postamble,
            ARRAY_LENGTH(ar9485_poseidon1_0_baseband_postamble), 5);

        /* radio */
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_CORE], 
            ar9485_poseidon1_0_radio_core,
            ARRAY_LENGTH(ar9485_poseidon1_0_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_POST],
            ar9485_poseidon1_0_radio_postamble,
            ARRAY_LENGTH(ar9485_poseidon1_0_radio_postamble), 2);

        /* soc */
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_PRE],
            ar9485_poseidon1_0_soc_preamble,
            ARRAY_LENGTH(ar9485_poseidon1_0_soc_preamble), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_POST], NULL, 0, 0);

        /* rx/tx gain */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
            ar9485Common_wo_xlna_rx_gain_poseidon1_0, 
            ARRAY_LENGTH(ar9485Common_wo_xlna_rx_gain_poseidon1_0), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
            ar9485Modes_lowest_ob_db_tx_gain_poseidon1_0, 
            ARRAY_LENGTH(ar9485Modes_lowest_ob_db_tx_gain_poseidon1_0), 5);

        /* Japan 2484Mhz CCK settings */
        INIT_INI_ARRAY(&ahp->ah_ini_japan2484,
            ar9485_poseidon1_0_baseband_core_txfir_coeff_japan_2484,
            ARRAY_LENGTH(
                ar9485_poseidon1_0_baseband_core_txfir_coeff_japan_2484), 2);

#ifndef AR9485_EMULATION
        /* Load PCIE SERDES settings from INI */
        if (ahpriv->ah_config.ath_hal_pcie_clock_req) {
            /* Pci-e Clock Request = 1 */
            if (ahpriv->ah_config.ath_hal_pll_pwr_save 
                & AR_PCIE_PLL_PWRSAVE_CONTROL)
            {
                /* Sleep Setting */
                if (ahpriv->ah_config.ath_hal_pll_pwr_save & 
                    AR_PCIE_PLL_PWRSAVE_ON_D3) 
                {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes, 
                        ar9485_poseidon1_0_pcie_phy_clkreq_enable_L1,
                        ARRAY_LENGTH(
                           ar9485_poseidon1_0_pcie_phy_clkreq_enable_L1),
                        2);
                } else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes, 
                        ar9485_poseidon1_0_pcie_phy_pll_on_clkreq_enable_L1,
                        ARRAY_LENGTH(
                           ar9485_poseidon1_0_pcie_phy_pll_on_clkreq_enable_L1),
                        2);
                }    
                /* Awake Setting */
                if (ahpriv->ah_config.ath_hal_pll_pwr_save & 
                    AR_PCIE_PLL_PWRSAVE_ON_D0)
                {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power, 
                        ar9485_poseidon1_0_pcie_phy_clkreq_enable_L1,
                        ARRAY_LENGTH(
                           ar9485_poseidon1_0_pcie_phy_clkreq_enable_L1),
                        2);
                } else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power, 
                        ar9485_poseidon1_0_pcie_phy_pll_on_clkreq_enable_L1,
                        ARRAY_LENGTH(
                           ar9485_poseidon1_0_pcie_phy_pll_on_clkreq_enable_L1),
                        2);
                }    
                
            } else {
                /*Use driver default setting*/
                /* Sleep Setting */
                INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes, 
                    ar9485_poseidon1_0_pcie_phy_pll_on_clkreq_enable_L1,
                    ARRAY_LENGTH(
                        ar9485_poseidon1_0_pcie_phy_pll_on_clkreq_enable_L1), 
                    2);
                /* Awake Setting */
                INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power, 
                    ar9485_poseidon1_0_pcie_phy_pll_on_clkreq_enable_L1,
                    ARRAY_LENGTH(
                        ar9485_poseidon1_0_pcie_phy_pll_on_clkreq_enable_L1), 
                    2);
            }
        } else {
            /* Pci-e Clock Request = 0 */
            if (ahpriv->ah_config.ath_hal_pll_pwr_save 
                & AR_PCIE_PLL_PWRSAVE_CONTROL)
            {
                /* Sleep Setting */
                if (ahpriv->ah_config.ath_hal_pll_pwr_save & 
                    AR_PCIE_PLL_PWRSAVE_ON_D3) 
                {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes, 
                        ar9485_poseidon1_0_pcie_phy_clkreq_disable_L1,
                        ARRAY_LENGTH(
                          ar9485_poseidon1_0_pcie_phy_clkreq_disable_L1),
                        2);
                } else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes, 
                        ar9485_poseidon1_0_pcie_phy_pll_on_clkreq_disable_L1,
                        ARRAY_LENGTH(
                          ar9485_poseidon1_0_pcie_phy_pll_on_clkreq_disable_L1),
                        2);
                }    
                /* Awake Setting */
                if (ahpriv->ah_config.ath_hal_pll_pwr_save & 
                    AR_PCIE_PLL_PWRSAVE_ON_D0)
                {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power, 
                        ar9485_poseidon1_0_pcie_phy_clkreq_disable_L1,
                        ARRAY_LENGTH(
                          ar9485_poseidon1_0_pcie_phy_clkreq_disable_L1),
                        2);
                } else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power, 
                        ar9485_poseidon1_0_pcie_phy_pll_on_clkreq_disable_L1,
                        ARRAY_LENGTH(
                          ar9485_poseidon1_0_pcie_phy_pll_on_clkreq_disable_L1),
                        2);
                }    
                
            } else {
                /*Use driver default setting*/
                /* Sleep Setting */
                INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes, 
                    ar9485_poseidon1_0_pcie_phy_pll_on_clkreq_disable_L1,
                    ARRAY_LENGTH(
                        ar9485_poseidon1_0_pcie_phy_pll_on_clkreq_disable_L1), 
                    2);
                /* Awake Setting */
                INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power, 
                    ar9485_poseidon1_0_pcie_phy_pll_on_clkreq_disable_L1,
                    ARRAY_LENGTH(
                        ar9485_poseidon1_0_pcie_phy_pll_on_clkreq_disable_L1), 
                    2);
            }
        }
#endif

#ifdef AR9485_EMULATION
        /* mac emu */
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_CORE],
            ar9485_poseidon1_0_mac_core_emulation,
            ARRAY_LENGTH(ar9485_poseidon1_0_mac_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_POST],
            ar9485_poseidon1_0_mac_postamble_emulation,
            ARRAY_LENGTH(ar9485_poseidon1_0_mac_postamble_emulation), 5);

        /* bb emu */
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_CORE],
            ar9485_poseidon1_0_baseband_core_emulation,
            ARRAY_LENGTH(ar9485_poseidon1_0_baseband_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_POST],
            ar9485_poseidon1_0_baseband_postamble_emulation,
            ARRAY_LENGTH(ar9485_poseidon1_0_baseband_postamble_emulation), 5);

        /* radio emu */
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_CORE],
            ar9485_130nm_radio_emulation,
            ARRAY_LENGTH(ar9485_130nm_radio_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_POST], NULL, 0, 0);

        /* soc emu */
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_POST], NULL, 0, 0);

        /* rx gain emu */
        INIT_INI_ARRAY(&ahp->ah_ini_rx_gain_emu,
            ar9485Common_rx_gain_poseidon1_0_emulation,
            ARRAY_LENGTH(ar9485Common_rx_gain_poseidon1_0_emulation), 2);
#endif
    } else if (AR_SREV_WASP(ah)) {
        /* mac */
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_CORE],
            ar9340_wasp_1p0_mac_core,
            ARRAY_LENGTH(ar9340_wasp_1p0_mac_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_POST],
            ar9340_wasp_1p0_mac_postamble,
            ARRAY_LENGTH(ar9340_wasp_1p0_mac_postamble), 5);

        /* bb */
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_CORE],
            ar9340_wasp_1p0_baseband_core,
            ARRAY_LENGTH(ar9340_wasp_1p0_baseband_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_POST],
            ar9340_wasp_1p0_baseband_postamble,
            ARRAY_LENGTH(ar9340_wasp_1p0_baseband_postamble), 5);

        /* radio */
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_CORE],
            ar9340_wasp_1p0_radio_core,
            ARRAY_LENGTH(ar9340_wasp_1p0_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_POST],
            ar9340_wasp_1p0_radio_postamble,
            ARRAY_LENGTH(ar9340_wasp_1p0_radio_postamble), 5);

        /* soc */
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_PRE],
            ar9340_wasp_1p0_soc_preamble,
            ARRAY_LENGTH(ar9340_wasp_1p0_soc_preamble), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_POST],
            ar9340_wasp_1p0_soc_postamble,
            ARRAY_LENGTH(ar9340_wasp_1p0_soc_postamble), 5);

        /* rx/tx gain */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
            ar9340Common_wo_xlna_rx_gain_table_wasp_1p0,
            ARRAY_LENGTH(ar9340Common_wo_xlna_rx_gain_table_wasp_1p0), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
            ar9340Modes_high_ob_db_tx_gain_table_wasp_1p0,
            ARRAY_LENGTH(ar9340Modes_high_ob_db_tx_gain_table_wasp_1p0), 5);

        ahpriv->ah_config.ath_hal_pcie_power_save_enable = 0;

        /* Fast clock modal settings */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_additional,
            ar9340Modes_fast_clock_wasp_1p0,
            ARRAY_LENGTH(ar9340Modes_fast_clock_wasp_1p0), 3);
        /* Japan 2484Mhz CCK settings */
        INIT_INI_ARRAY(&ahp->ah_ini_japan2484,
            ar9340_wasp_1p0_baseband_core_txfir_coeff_japan_2484,
            ARRAY_LENGTH(
                ar9340_wasp_1p0_baseband_core_txfir_coeff_japan_2484), 2);

        /* Additional setttings for 40Mhz */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_additional_40mhz, 
            ar9340_wasp_1p0_radio_core_40M,
            ARRAY_LENGTH(ar9340_wasp_1p0_radio_core_40M), 2);

        /* DFS */
        INIT_INI_ARRAY(&ahp->ah_ini_dfs,
            ar9340_wasp_1p0_baseband_postamble_dfs_channel,
            ARRAY_LENGTH(ar9340_wasp_1p0_baseband_postamble_dfs_channel), 3);
#ifdef AR9340_EMULATION
        /* mac emu */
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_CORE],
            ar9340_wasp_1p0_mac_core_emulation,
            ARRAY_LENGTH(ar9340_wasp_1p0_mac_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_POST],
            ar9340_wasp_1p0_mac_postamble_emulation,
            ARRAY_LENGTH(ar9340_wasp_1p0_mac_postamble_emulation), 5);

        /* bb emu */
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_CORE],
            ar9340_wasp_1p0_baseband_core_emulation,
            ARRAY_LENGTH(ar9340_wasp_1p0_baseband_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_POST],
            ar9340_wasp_1p0_mac_postamble_emulation,
            ARRAY_LENGTH(ar9340_wasp_1p0_mac_postamble_emulation), 5);

        /* radio emu */
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_CORE],
            ar9340_merlin_2p0_radio_core,
            ARRAY_LENGTH(ar9340_merlin_2p0_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_POST], NULL, 0, 0);

        /* soc emu */
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_POST], NULL, 0, 0);

        /* rx gain emu */
        INIT_INI_ARRAY(&ahp->ah_ini_rx_gain_emu,
            ar9340Common_rx_gain_table_merlin_2p0,
            ARRAY_LENGTH(ar9340Common_rx_gain_table_merlin_2p0), 2);
#endif
    } else if (AR_SREV_SCORPION(ah)) {
        /* mac */
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_CORE],
                        ar955x_scorpion_1p0_mac_core,
                        ARRAY_LENGTH(ar955x_scorpion_1p0_mac_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_POST],
                        ar955x_scorpion_1p0_mac_postamble,
                        ARRAY_LENGTH(ar955x_scorpion_1p0_mac_postamble), 5);

        /* bb */
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_CORE],
                        ar955x_scorpion_1p0_baseband_core,
                        ARRAY_LENGTH(ar955x_scorpion_1p0_baseband_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_POST],
                        ar955x_scorpion_1p0_baseband_postamble,
                        ARRAY_LENGTH(ar955x_scorpion_1p0_baseband_postamble), 5);

        /* radio */
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_CORE],
                        ar955x_scorpion_1p0_radio_core,
                        ARRAY_LENGTH(ar955x_scorpion_1p0_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_POST],
                        ar955x_scorpion_1p0_radio_postamble,
                        ARRAY_LENGTH(ar955x_scorpion_1p0_radio_postamble), 5);

        /* soc */
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_PRE],
                        ar955x_scorpion_1p0_soc_preamble,
                        ARRAY_LENGTH(ar955x_scorpion_1p0_soc_preamble), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_POST],
                        ar955x_scorpion_1p0_soc_postamble,
                        ARRAY_LENGTH(ar955x_scorpion_1p0_soc_postamble), 5);

        /* rx/tx gain */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                        ar955xCommon_wo_xlna_rx_gain_table_scorpion_1p0,
                        ARRAY_LENGTH(ar955xCommon_wo_xlna_rx_gain_table_scorpion_1p0), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain_bounds,
                        ar955xCommon_wo_xlna_rx_gain_bounds_scorpion_1p0,
                        ARRAY_LENGTH(ar955xCommon_wo_xlna_rx_gain_bounds_scorpion_1p0), 5);
        INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                        ar955xModes_no_xpa_tx_gain_table_scorpion_1p0,
                        ARRAY_LENGTH(ar955xModes_no_xpa_tx_gain_table_scorpion_1p0), 5);

        /*ath_hal_pciePowerSaveEnable should be 2 for OWL/Condor and 0 for merlin */
        ahpriv->ah_config.ath_hal_pcie_power_save_enable = 0;

        /* Fast clock modal settings */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_additional,
                        ar955xModes_fast_clock_scorpion_1p0,
                        ARRAY_LENGTH(ar955xModes_fast_clock_scorpion_1p0), 3);

        /* Additional setttings for 40Mhz */
        //INIT_INI_ARRAY(&ahp->ah_ini_modes_additional_40M,
        //                ar955x_scorpion_1p0_radio_core_40M,
        //                ARRAY_LENGTH(ar955x_scorpion_1p0_radio_core_40M), 2);
#ifdef AR9550_EMULATION
        /* mac emu */
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_CORE],
                        ar9340_wasp_1p0_mac_core_emulation,
                        ARRAY_LENGTH(ar9340_wasp_1p0_mac_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_POST],
                        ar9340_wasp_1p0_mac_postamble_emulation,
                        ARRAY_LENGTH(ar9340_wasp_1p0_mac_postamble_emulation), 5);

        /* bb emu */
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_CORE],
                        ar9340_wasp_1p0_baseband_core_emulation,
                        ARRAY_LENGTH(ar9340_wasp_1p0_baseband_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_POST],
                        ar9340_wasp_1p0_mac_postamble_emulation,
                        ARRAY_LENGTH(ar9340_wasp_1p0_mac_postamble_emulation), 5);

        /* radio emu */
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_CORE],
                        ar9340_merlin_2p0_radio_core,
                        ARRAY_LENGTH(ar9340_merlin_2p0_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_POST], NULL, 0, 0);

        /* soc emu */
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_POST], NULL, 0, 0);

        /* rx gain emu */
        INIT_INI_ARRAY(&ahp->ah_ini_rx_gain_emu,
                        ar9340Common_rx_gain_table_merlin_2p0,
                        ARRAY_LENGTH(ar9340Common_rx_gain_table_merlin_2p0), 2);
#endif
#ifndef AR9530_EMULATION
    } else if (AR_SREV_HONEYBEE(ah)) {
        /* mac */
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_CORE],
                        qca953x_honeybee_1p0_mac_core,
                        ARRAY_LENGTH(qca953x_honeybee_1p0_mac_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_POST],
                        qca953x_honeybee_1p0_mac_postamble,
                        ARRAY_LENGTH(qca953x_honeybee_1p0_mac_postamble), 5);

        /* bb */
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_PRE], NULL, 0, 0);

        if(AR_SREV_HONEYBEE_20(ah)) {
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_CORE],
                qca953x_honeybee_2p0_baseband_core,
                ARRAY_LENGTH(qca953x_honeybee_2p0_baseband_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_POST],
                qca953x_honeybee_2p0_baseband_postamble,
				ARRAY_LENGTH(qca953x_honeybee_2p0_baseband_postamble), 5);
    } else {
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_CORE],
                qca953x_honeybee_1p0_baseband_core,
                ARRAY_LENGTH(qca953x_honeybee_1p0_baseband_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_POST],
                qca953x_honeybee_1p0_baseband_postamble,
                ARRAY_LENGTH(qca953x_honeybee_1p0_baseband_postamble), 5);
    }

        /* radio */
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_CORE],
                        qca953x_honeybee_1p0_radio_core,
                        ARRAY_LENGTH(qca953x_honeybee_1p0_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_POST],
                        qca953x_honeybee_1p0_radio_postamble,
                        ARRAY_LENGTH(qca953x_honeybee_1p0_radio_postamble), 5);

        /* soc */
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_PRE],
                        qca953x_honeybee_1p0_soc_preamble,
                        ARRAY_LENGTH(qca953x_honeybee_1p0_soc_preamble), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_POST],
                        qca953x_honeybee_1p0_soc_postamble,
                        ARRAY_LENGTH(qca953x_honeybee_1p0_soc_postamble), 5);

        /* rx/tx gain */
        if(AR_SREV_HONEYBEE_20(ah)) {
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                qca953xCommon_wo_xlna_rx_gain_table_honeybee_2p0,
                ARRAY_LENGTH(qca953xCommon_wo_xlna_rx_gain_table_honeybee_2p0), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain_bounds,
                qca953xCommon_wo_xlna_rx_gain_bounds_honeybee_2p0,
                ARRAY_LENGTH(qca953xCommon_wo_xlna_rx_gain_bounds_honeybee_2p0), 5);
		INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                qca953xModes_no_xpa_tx_gain_table_honeybee_2p0,
                ARRAY_LENGTH(qca953xModes_no_xpa_tx_gain_table_honeybee_2p0), 2);
        } else {
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                qca953xCommon_wo_xlna_rx_gain_table_honeybee_1p0,
                ARRAY_LENGTH(qca953xCommon_wo_xlna_rx_gain_table_honeybee_1p0), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain_bounds,
                qca953xCommon_wo_xlna_rx_gain_bounds_honeybee_1p0,
                ARRAY_LENGTH(qca953xCommon_wo_xlna_rx_gain_bounds_honeybee_1p0), 5);
        INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                qca953xModes_no_xpa_tx_gain_table_honeybee_1p0,
                ARRAY_LENGTH(qca953xModes_no_xpa_tx_gain_table_honeybee_1p0), 2);
        }

        /*ath_hal_pciePowerSaveEnable should be 2 for OWL/Condor and 0 for merlin */
        ahpriv->ah_config.ath_hal_pcie_power_save_enable = 0;

        /* Fast clock modal settings */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_additional,
                        qca953xModes_fast_clock_honeybee_1p0,
                        ARRAY_LENGTH(qca953xModes_fast_clock_honeybee_1p0), 3);

        /* Additional setttings for 40Mhz */
        //INIT_INI_ARRAY(&ahp->ah_ini_modes_additional_40M,
        //                qca953x_honeybee_1p0_radio_core_40M,
        //                ARRAY_LENGTH(qca953x_honeybee_1p0_radio_core_40M), 2);
#endif
    } else if (AR_SREV_DRAGONFLY(ah)) {
        /* mac */
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_CORE],
                        qca956x_dragonfly_1p0_mac_core,
                        ARRAY_LENGTH(qca956x_dragonfly_1p0_mac_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_POST],
                        qca956x_dragonfly_1p0_mac_postamble,
                        ARRAY_LENGTH(qca956x_dragonfly_1p0_mac_postamble), 5);
        /* bb */
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_CORE],
                        qca956x_dragonfly_1p0_baseband_core,
                        ARRAY_LENGTH(qca956x_dragonfly_1p0_baseband_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_POST],
                        qca956x_dragonfly_1p0_baseband_postamble,
                        ARRAY_LENGTH(qca956x_dragonfly_1p0_baseband_postamble), 5);
        /* radio */
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_PRE], NULL, 0, 0);
#ifndef  AR9560_EMULATION
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_CORE],
						qca956x_dragonfly_1p0_radio_core,
						ARRAY_LENGTH(qca956x_dragonfly_1p0_radio_core), 2);
		INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_POST],
						qca956x_dragonfly_1p0_radio_postamble,
						ARRAY_LENGTH(qca956x_dragonfly_1p0_radio_postamble), 5);
#else
		INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_CORE],NULL, 0, 0);
		INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_POST],NULL, 0, 0);
#endif
        /* soc */
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_PRE],
                        qca956x_dragonfly_1p0_soc_preamble,
                        ARRAY_LENGTH(qca956x_dragonfly_1p0_soc_preamble), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_POST],
                        qca956x_dragonfly_1p0_soc_postamble,
                        ARRAY_LENGTH(qca956x_dragonfly_1p0_soc_postamble), 5);
        /* rx/tx gain */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                        qca956xCommon_wo_xlna_rx_gain_table_dragonfly_1p0,
                        ARRAY_LENGTH(qca956xCommon_wo_xlna_rx_gain_table_dragonfly_1p0), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain_bounds,
                        qca956xCommon_wo_xlna_rx_gain_bounds_dragonfly_1p0,
                        ARRAY_LENGTH(qca956xCommon_wo_xlna_rx_gain_bounds_dragonfly_1p0), 5);
        INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                        qca956xModes_no_xpa_tx_gain_table_dragonfly_1p0,
                        ARRAY_LENGTH(qca956xModes_no_xpa_tx_gain_table_dragonfly_1p0), 3);

        /* DFS */
        INIT_INI_ARRAY(&ahp->ah_ini_dfs,
            qca956x_dragonfly_1p0_baseband_postamble_dfs_channel,
            ARRAY_LENGTH(qca956x_dragonfly_1p0_baseband_postamble_dfs_channel), 3);

        INIT_INI_ARRAY(&ahp->ah_ini_japan2484,
            qca956x_dragonfly_1p0_baseband_core_txfir_coeff_japan_2484,
            ARRAY_LENGTH(
            qca956x_dragonfly_1p0_baseband_core_txfir_coeff_japan_2484), 2);

        /*ath_hal_pciePowerSaveEnable should be 2 for OWL/Condor and 0 for merlin */
        ahpriv->ah_config.ath_hal_pcie_power_save_enable = 0;
        /* Fast clock modal settings */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_additional,
                        qca956xModes_fast_clock_dragonfly_1p0,
                        ARRAY_LENGTH(qca956xModes_fast_clock_dragonfly_1p0), 3);
        /* Additional setttings for 40Mhz */
        //INIT_INI_ARRAY(&ahp->ah_ini_modes_additional_40M,
        //                ar955x_scorpion_1p0_radio_core_40M,
        //                ARRAY_LENGTH(ar955x_scorpion_1p0_radio_core_40M), 2);
#ifdef AR9560_EMULATION
        /* mac emu */
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_CORE],
                        qca956x_dragonfly_1p0_mac_core_emulation,
                        ARRAY_LENGTH(qca956x_dragonfly_1p0_mac_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_POST],
                        qca956x_dragonfly_1p0_mac_postamble_emulation,
                        ARRAY_LENGTH(qca956x_dragonfly_1p0_mac_postamble_emulation), 5);
        /* bb emu */
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_CORE],
                        qca956x_dragonfly_1p0_baseband_core_emulation,
                        ARRAY_LENGTH(qca956x_dragonfly_1p0_baseband_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_POST],
                        qca956x_dragonfly_1p0_baseband_postamble_emulation,
                        ARRAY_LENGTH(qca956x_dragonfly_1p0_baseband_postamble_emulation), 5);
        /* radio emu */
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_CORE],
                        qca956x_merlin_radio_emulation,
                        ARRAY_LENGTH(qca956x_merlin_radio_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_POST], NULL, 0, 0);
        /* soc emu */
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_POST], NULL, 0, 0);
        /* rx gain emu */
        INIT_INI_ARRAY(&ahp->ah_ini_rx_gain_emu,
                        qca956xCommon_rx_gain_table_merlin_1p0,
                        ARRAY_LENGTH(qca956xCommon_rx_gain_table_merlin_1p0), 2);
        /* tx gain emu */
        INIT_INI_ARRAY(&ahp->ah_ini_tx_gain_emu,
        qca956x_dragonfly_1p0_tx_gain_table_baseband_postamble_emulation,
        ARRAY_LENGTH(qca956x_dragonfly_1p0_tx_gain_table_baseband_postamble_emulation),5);
#endif

    } else if (AR_SREV_JET(ah)) {
        /* mac */
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_CORE],
                qcn5500_jet_1p0_mac_core,
                ARRAY_LENGTH(qcn5500_jet_1p0_mac_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_POST],
                qcn5500_jet_1p0_mac_postamble,
                ARRAY_LENGTH(qcn5500_jet_1p0_mac_postamble), 5);

        /* bb */
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_PRE], NULL, 0, 0);

        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_CORE],
                qcn5500_jet_1p0_baseband_core,
                ARRAY_LENGTH(qcn5500_jet_1p0_baseband_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_POST],
                qcn5500_jet_1p0_baseband_postamble,
                ARRAY_LENGTH(qcn5500_jet_1p0_baseband_postamble), 5);

        /* radio */
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_PRE], NULL, 0, 0);
#if !defined(AR5500_EMULATION)
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_CORE],
                qcn5500_jet_1p0_radio_core,
                ARRAY_LENGTH(qcn5500_jet_1p0_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_POST],
                qcn5500_jet_1p0_radio_postamble,
                ARRAY_LENGTH(qcn5500_jet_1p0_radio_postamble), 5);
#else
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_CORE],NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_POST],NULL, 0, 0);
#endif
        /* soc */
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_PRE],
                qcn5500_jet_1p0_soc_preamble,
                ARRAY_LENGTH(qcn5500_jet_1p0_soc_preamble), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_POST],
                qcn5500_jet_1p0_soc_postamble,
                ARRAY_LENGTH(qcn5500_jet_1p0_soc_postamble), 5);
#if !defined(AR5500_EMULATION)
        /* rx/tx gain */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                qcn5500Common_wo_xlna_rx_gain_table_jet_1p0,
                ARRAY_LENGTH(qcn5500Common_wo_xlna_rx_gain_table_jet_1p0), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain_bounds,
                qcn5500Common_wo_xlna_rx_gain_bounds_jet_1p0,
                ARRAY_LENGTH(qcn5500Common_wo_xlna_rx_gain_bounds_jet_1p0), 5);
        INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                qcn5500Modes_xpa_tx_gain_table_jet_1p0,
                ARRAY_LENGTH(qcn5500Modes_xpa_tx_gain_table_jet_1p0), 3);
        /* DFS */
        INIT_INI_ARRAY(&ahp->ah_ini_dfs,
                qcn5500_jet_1p0_baseband_postamble_dfs_channel,
                ARRAY_LENGTH(qcn5500_jet_1p0_baseband_postamble_dfs_channel), 3);

        INIT_INI_ARRAY(&ahp->ah_ini_japan2484,
                qcn5500_jet_1p0_baseband_core_txfir_coeff_japan_2484,
                ARRAY_LENGTH(qcn5500_jet_1p0_baseband_core_txfir_coeff_japan_2484), 2);
#endif

        /*ath_hal_pciePowerSaveEnable should be 2 for OWL/Condor and 0 for merlin */
        ahpriv->ah_config.ath_hal_pcie_power_save_enable = 0;
        /* Fast clock modal settings */ /* For M2M Emulation only initialize MAC reg only */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_additional,
                qcn5500Modes_fast_clock_jet_1p0,
                ARRAY_LENGTH(qcn5500Modes_fast_clock_jet_1p0), 3);

#if defined(AR5500_EMULATION)
        /* mac emu */
#ifndef QCN5500_M2M
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_PRE],
                qcn5500_jet_1p0_mac_preamble_emulation,
                ARRAY_LENGTH(qcn5500_jet_1p0_mac_preamble_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_POST],
                qcn5500_jet_1p0_mac_postamble_m2m,
                ARRAY_LENGTH(qcn5500_jet_1p0_mac_postamble_m2m), 5);
#else
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_CORE],
                qcn5500_jet_1p0_mac_core_emulation,
                ARRAY_LENGTH(qcn5500_jet_1p0_mac_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_POST],
                qcn5500_jet_1p0_mac_postamble_emulation,
                ARRAY_LENGTH(qcn5500_jet_1p0_mac_postamble_emulation), 5);
#endif
        /* bb emu */
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_CORE],
                qcn5500_jet_1p0_baseband_core_emulation,
                ARRAY_LENGTH(qcn5500_jet_1p0_baseband_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_POST],
                qcn5500_jet_1p0_baseband_postamble_emulation,
                ARRAY_LENGTH(qcn5500_jet_1p0_baseband_postamble_emulation), 5);
        /* radio emu */
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_CORE],
                ar9300_130nm_radio_emulation,
                ARRAY_LENGTH(ar9300_130nm_radio_emulation), 2);

        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_POST], NULL, 0, 0);
        /* soc emu */
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_POST], NULL, 0, 0);
        /* rx gain emu */
        INIT_INI_ARRAY(&ahp->ah_ini_rx_gain_emu,
                qcn5500Common_rx_gain_table_merlin_1p0,
                ARRAY_LENGTH(qcn5500Common_rx_gain_table_merlin_1p0), 2);
        /* tx gain emu */
        INIT_INI_ARRAY(&ahp->ah_ini_tx_gain_emu,
                qcn5500_jet_1p0_tx_gain_table_baseband_postamble_emulation,
                ARRAY_LENGTH(qcn5500_jet_1p0_tx_gain_table_baseband_postamble_emulation),5);
#endif
    } else if (AR_SREV_JUPITER_10(ah)) {
        /* Jupiter: new INI format (pre, core, post arrays per subsystem) */

        /* mac */
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_CORE],
            ar9300_jupiter_1p0_mac_core, 
            ARRAY_LENGTH(ar9300_jupiter_1p0_mac_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_POST],
            ar9300_jupiter_1p0_mac_postamble,
            ARRAY_LENGTH(ar9300_jupiter_1p0_mac_postamble), 5);
                       
        /* bb */
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_CORE],
            ar9300_jupiter_1p0_baseband_core,
            ARRAY_LENGTH(ar9300_jupiter_1p0_baseband_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_POST],
            ar9300_jupiter_1p0_baseband_postamble,
            ARRAY_LENGTH(ar9300_jupiter_1p0_baseband_postamble), 5);

#ifndef AR9300_EMULATION
        /* radio */
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_CORE],
            ar9300_jupiter_1p0_radio_core,
            ARRAY_LENGTH(ar9300_jupiter_1p0_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_POST],
            ar9300_jupiter_1p0_radio_postamble,
            ARRAY_LENGTH(ar9300_jupiter_1p0_radio_postamble), 5);
#endif

        /* soc */
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_PRE],
            ar9300_jupiter_1p0_soc_preamble,
            ARRAY_LENGTH(ar9300_jupiter_1p0_soc_preamble), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_POST],
            ar9300_jupiter_1p0_soc_postamble,
            ARRAY_LENGTH(ar9300_jupiter_1p0_soc_postamble), 5);

        /* rx/tx gain */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
            ar9300_common_rx_gain_table_jupiter_1p0,
            ARRAY_LENGTH(ar9300_common_rx_gain_table_jupiter_1p0), 2);

        /* Load PCIE SERDES settings from INI */
        if (ahpriv->ah_config.ath_hal_pcie_clock_req) {
            /* Pci-e Clock Request = 1 */
            /*
             * PLL ON + clkreq enable is not a valid combination,
             * thus to ignore ath_hal_pll_pwr_save, use PLL OFF.
             */
            {
                /*Use driver default setting*/
                /* Awake -> Sleep Setting */
                INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes,
                    ar9300_pcie_phy_clkreq_enable_L1_jupiter_1p0,
                    ARRAY_LENGTH(ar9300_pcie_phy_clkreq_enable_L1_jupiter_1p0),
                    2);
                /* Sleep -> Awake Setting */
                INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power,
                    ar9300_pcie_phy_clkreq_enable_L1_jupiter_1p0,
                    ARRAY_LENGTH(ar9300_pcie_phy_clkreq_enable_L1_jupiter_1p0),
                    2);
            }
        }
        else {
            /*
             * Since Jupiter 1.0 and 2.0 share the same device id and will be
             * installed with same INF, but Jupiter 1.0 has issue with PLL OFF.
             *
             * Force Jupiter 1.0 to use ON/ON setting.
             */
            ahpriv->ah_config.ath_hal_pll_pwr_save = 0;
            /* Pci-e Clock Request = 0 */
            if (ahpriv->ah_config.ath_hal_pll_pwr_save &
                AR_PCIE_PLL_PWRSAVE_CONTROL)
            {
                /* Awake -> Sleep Setting */
                if (ahpriv->ah_config.ath_hal_pll_pwr_save &
                     AR_PCIE_PLL_PWRSAVE_ON_D3) 
                {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes,
                        ar9300_pcie_phy_clkreq_disable_L1_jupiter_1p0,
                        ARRAY_LENGTH(
                            ar9300_pcie_phy_clkreq_disable_L1_jupiter_1p0),
                        2);
                }
                else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes,
                        ar9300_pcie_phy_pll_on_clkreq_disable_L1_jupiter_1p0,
                        ARRAY_LENGTH(
                          ar9300_pcie_phy_pll_on_clkreq_disable_L1_jupiter_1p0),
                        2);
                }    
                /* Sleep -> Awake Setting */
                if (ahpriv->ah_config.ath_hal_pll_pwr_save &
                    AR_PCIE_PLL_PWRSAVE_ON_D0)
                {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power,
                        ar9300_pcie_phy_clkreq_disable_L1_jupiter_1p0,
                        ARRAY_LENGTH(
                            ar9300_pcie_phy_clkreq_disable_L1_jupiter_1p0),
                        2);
                }
                else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power,
                        ar9300_pcie_phy_pll_on_clkreq_disable_L1_jupiter_1p0,
                        ARRAY_LENGTH(
                          ar9300_pcie_phy_pll_on_clkreq_disable_L1_jupiter_1p0),
                        2);
                }    
                
            }
            else {
                /*Use driver default setting*/
                /* Awake -> Sleep Setting */
                INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes,
                    ar9300_pcie_phy_pll_on_clkreq_disable_L1_jupiter_1p0,
                    ARRAY_LENGTH(
                        ar9300_pcie_phy_pll_on_clkreq_disable_L1_jupiter_1p0),
                    2);
                /* Sleep -> Awake Setting */
                INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power,
                    ar9300_pcie_phy_pll_on_clkreq_disable_L1_jupiter_1p0,
                    ARRAY_LENGTH(
                        ar9300_pcie_phy_pll_on_clkreq_disable_L1_jupiter_1p0),
                    2);
            }
        }
        /* 
         * ath_hal_pcie_power_save_enable should be 2 for OWL/Condor and 
         * 0 for merlin 
         */
        ahpriv->ah_config.ath_hal_pcie_power_save_enable = 0;

        /* Fast clock modal settings */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_additional, 
            ar9300_modes_fast_clock_jupiter_1p0,
            ARRAY_LENGTH(ar9300_modes_fast_clock_jupiter_1p0), 3);
        INIT_INI_ARRAY(&ahp->ah_ini_japan2484,
            ar9300_jupiter_1p0_baseband_core_txfir_coeff_japan_2484,
            ARRAY_LENGTH(
            ar9300_jupiter_1p0_baseband_core_txfir_coeff_japan_2484), 2);

#ifdef AR9300_EMULATION
        /* mac emu */
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_CORE],
            ar9300_jupiter_1p0_mac_core_emulation, 
            ARRAY_LENGTH(ar9300_jupiter_1p0_mac_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_POST],
            ar9300_jupiter_1p0_mac_postamble_emulation, 
            ARRAY_LENGTH(ar9300_jupiter_1p0_mac_postamble_emulation), 5);

        /* bb emu */
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_CORE],
            ar9300_jupiter_1p0_baseband_core_emulation, 
            ARRAY_LENGTH(ar9300_jupiter_1p0_baseband_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_POST],
            ar9300_jupiter_1p0_baseband_postamble_emulation, 
            ARRAY_LENGTH(ar9300_jupiter_1p0_baseband_postamble_emulation), 5);

        /* radio emu */
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_CORE],
            ar9200_merlin_2p0_radio_core_jupiter_1p0, 
            ARRAY_LENGTH(ar9200_merlin_2p0_radio_core_jupiter_1p0), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_POST], NULL, 0, 0);

        /* soc emu */
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_POST], NULL, 0, 0);

        /* rx gain emu */
        INIT_INI_ARRAY(&ahp->ah_ini_rx_gain_emu, 
            ar9300Common_rx_gain_table_merlin_2p0_jupiter_1p0,
            ARRAY_LENGTH(ar9300Common_rx_gain_table_merlin_2p0_jupiter_1p0), 2);
        /* tx gain emu */
        INIT_INI_ARRAY(&ahp->ah_ini_tx_gain_emu, 
            ar9300_jupiter_1p0_tx_gain_table_baseband_postamble_emulation,
            ARRAY_LENGTH(
            ar9300_jupiter_1p0_tx_gain_table_baseband_postamble_emulation), 5);
#endif
    } else if (AR_SREV_JUPITER_20(ah)) {
        /* Jupiter: new INI format (pre, core, post arrays per subsystem) */

        /* mac */
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_CORE],
            ar9300_jupiter_2p0_mac_core, 
            ARRAY_LENGTH(ar9300_jupiter_2p0_mac_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_POST],
            ar9300_jupiter_2p0_mac_postamble,
            ARRAY_LENGTH(ar9300_jupiter_2p0_mac_postamble), 5);
                       
        /* bb */
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_CORE],
            ar9300_jupiter_2p0_baseband_core,
            ARRAY_LENGTH(ar9300_jupiter_2p0_baseband_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_POST],
            ar9300_jupiter_2p0_baseband_postamble,
            ARRAY_LENGTH(ar9300_jupiter_2p0_baseband_postamble), 5);

#ifndef AR9300_EMULATION
        /* radio */
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_CORE],
            ar9300_jupiter_2p0_radio_core, 
            ARRAY_LENGTH(ar9300_jupiter_2p0_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_POST],
            ar9300_jupiter_2p0_radio_postamble, 
            ARRAY_LENGTH(ar9300_jupiter_2p0_radio_postamble), 5);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_post_sys2ant,
            ar9300_jupiter_2p0_radio_postamble_sys2ant, 
            ARRAY_LENGTH(ar9300_jupiter_2p0_radio_postamble_sys2ant), 5);
#endif

        /* soc */
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_PRE],
            ar9300_jupiter_2p0_soc_preamble, 
            ARRAY_LENGTH(ar9300_jupiter_2p0_soc_preamble), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_POST],
            ar9300_jupiter_2p0_soc_postamble, 
            ARRAY_LENGTH(ar9300_jupiter_2p0_soc_postamble), 5);

        /* rx/tx gain */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain, 
            ar9300Common_rx_gain_table_jupiter_2p0,
            ARRAY_LENGTH(ar9300Common_rx_gain_table_jupiter_2p0), 2);

        /* BTCOEX */
        INIT_INI_ARRAY(&ahp->ah_ini_BTCOEX_MAX_TXPWR,
            ar9300_jupiter_2p0_BTCOEX_MAX_TXPWR_table, 
            ARRAY_LENGTH(ar9300_jupiter_2p0_BTCOEX_MAX_TXPWR_table), 2);

        /* Load PCIE SERDES settings from INI */
        if (ahpriv->ah_config.ath_hal_pcie_clock_req) {
            /* Pci-e Clock Request = 1 */
            /*
             * PLL ON + clkreq enable is not a valid combination,
             * thus to ignore ath_hal_pll_pwr_save, use PLL OFF.
             */
            {
                /*Use driver default setting*/
                /* Awake -> Sleep Setting */
                INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes,
                    ar9300_PciePhy_clkreq_enable_L1_jupiter_2p0,
                    ARRAY_LENGTH(ar9300_PciePhy_clkreq_enable_L1_jupiter_2p0),
                    2);
                /* Sleep -> Awake Setting */
                INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power,
                    ar9300_PciePhy_clkreq_enable_L1_jupiter_2p0,
                    ARRAY_LENGTH(ar9300_PciePhy_clkreq_enable_L1_jupiter_2p0),
                    2);
            }
        }
        else {
            /* Pci-e Clock Request = 0 */
            if (ahpriv->ah_config.ath_hal_pll_pwr_save &
                AR_PCIE_PLL_PWRSAVE_CONTROL)
            {
                /* Awake -> Sleep Setting */
                if (ahpriv->ah_config.ath_hal_pll_pwr_save &
                     AR_PCIE_PLL_PWRSAVE_ON_D3) 
                {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes,
                        ar9300_PciePhy_clkreq_disable_L1_jupiter_2p0,
                        ARRAY_LENGTH(
                            ar9300_PciePhy_clkreq_disable_L1_jupiter_2p0),
                        2);
                }
                else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes,
                        ar9300_PciePhy_pll_on_clkreq_disable_L1_jupiter_2p0,
                        ARRAY_LENGTH(
                          ar9300_PciePhy_pll_on_clkreq_disable_L1_jupiter_2p0),
                        2);
                }    
                /* Sleep -> Awake Setting */
                if (ahpriv->ah_config.ath_hal_pll_pwr_save &
                    AR_PCIE_PLL_PWRSAVE_ON_D0)
                {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power,
                        ar9300_PciePhy_clkreq_disable_L1_jupiter_2p0,
                        ARRAY_LENGTH(
                            ar9300_PciePhy_clkreq_disable_L1_jupiter_2p0),
                        2);
                }
                else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power,
                        ar9300_PciePhy_pll_on_clkreq_disable_L1_jupiter_2p0,
                        ARRAY_LENGTH(
                          ar9300_PciePhy_pll_on_clkreq_disable_L1_jupiter_2p0),
                        2);
                }    
                
            }
            else {
                /*Use driver default setting*/
                /* Awake -> Sleep Setting */
                INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes,
                    ar9300_PciePhy_pll_on_clkreq_disable_L1_jupiter_2p0,
                    ARRAY_LENGTH(
                        ar9300_PciePhy_pll_on_clkreq_disable_L1_jupiter_2p0),
                    2);
                /* Sleep -> Awake Setting */
                INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power,
                    ar9300_PciePhy_pll_on_clkreq_disable_L1_jupiter_2p0,
                    ARRAY_LENGTH(
                        ar9300_PciePhy_pll_on_clkreq_disable_L1_jupiter_2p0),
                    2);
            }
        }

        /* 
         * ath_hal_pcie_power_save_enable should be 2 for OWL/Condor and 
         * 0 for merlin 
         */
        ahpriv->ah_config.ath_hal_pcie_power_save_enable = 0;

        /* Fast clock modal settings */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_additional, 
            ar9300Modes_fast_clock_jupiter_2p0,
            ARRAY_LENGTH(ar9300Modes_fast_clock_jupiter_2p0), 3);
        INIT_INI_ARRAY(&ahp->ah_ini_japan2484,
            ar9300_jupiter_2p0_baseband_core_txfir_coeff_japan_2484,
            ARRAY_LENGTH(
            ar9300_jupiter_2p0_baseband_core_txfir_coeff_japan_2484), 2);

#ifdef AR9300_EMULATION
        /* mac emu */
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_CORE],
            ar9300_jupiter_2p0_mac_core_emulation,
            ARRAY_LENGTH(ar9300_jupiter_2p0_mac_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_POST],
            ar9300_jupiter_2p0_mac_postamble_emulation,
            ARRAY_LENGTH(ar9300_jupiter_2p0_mac_postamble_emulation), 5);

        /* bb emu */
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_CORE],
            ar9300_jupiter_2p0_baseband_core_emulation,
            ARRAY_LENGTH(ar9300_jupiter_2p0_baseband_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_POST],
            ar9300_jupiter_2p0_baseband_postamble_emulation,
            ARRAY_LENGTH(ar9300_jupiter_2p0_baseband_postamble_emulation), 5);

        /* radio emu */
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_CORE],
            ar9200_merlin_2p0_radio_core,
            ARRAY_LENGTH(ar9200_merlin_2p0_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_POST], NULL, 0, 0);

        /* soc emu */
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_POST],
            ar9300_jupiter_1p0_soc_postamble_emulation,
            ARRAY_LENGTH(ar9300_jupiter_1p0_soc_postamble_emulation), 5);

        /* rx gain emu */
        INIT_INI_ARRAY(&ahp->ah_ini_rx_gain_emu,
            ar9300Common_rx_gain_table_merlin_2p0,
            ARRAY_LENGTH(ar9300Common_rx_gain_table_merlin_2p0), 2);
        /* tx gain emu */
        INIT_INI_ARRAY(&ahp->ah_ini_tx_gain_emu,
            ar9300_jupiter_2p0_tx_gain_table_baseband_postamble_emulation,
            ARRAY_LENGTH(
            ar9300_jupiter_2p0_tx_gain_table_baseband_postamble_emulation), 5);
#endif
    } else if (AR_SREV_APHRODITE(ah)) {
        /* Aphrodite: new INI format (pre, core, post arrays per subsystem) */

        /* mac */
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_CORE],
            ar956X_aphrodite_1p0_mac_core,
            ARRAY_LENGTH(ar956X_aphrodite_1p0_mac_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_POST],
            ar956X_aphrodite_1p0_mac_postamble,
            ARRAY_LENGTH(ar956X_aphrodite_1p0_mac_postamble), 5);

        /* bb */
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_CORE],
            ar956X_aphrodite_1p0_baseband_core,
            ARRAY_LENGTH(ar956X_aphrodite_1p0_baseband_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_POST],
            ar956X_aphrodite_1p0_baseband_postamble,
            ARRAY_LENGTH(ar956X_aphrodite_1p0_baseband_postamble), 5);

        /* soc */
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_PRE],
            ar956X_aphrodite_1p0_soc_preamble, 
            ARRAY_LENGTH(ar956X_aphrodite_1p0_soc_preamble), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_POST],
            ar956X_aphrodite_1p0_soc_postamble, 
            ARRAY_LENGTH(ar956X_aphrodite_1p0_soc_postamble), 5);

        /* rx/tx gain */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain, 
            ar956XCommon_rx_gain_table_aphrodite_1p0,
            ARRAY_LENGTH(ar956XCommon_rx_gain_table_aphrodite_1p0), 2);

        /* 
         * ath_hal_pcie_power_save_enable should be 2 for OWL/Condor and 
         * 0 for merlin 
         */
        ahpriv->ah_config.ath_hal_pcie_power_save_enable = 0;

       /* Fast clock modal settings */
       INIT_INI_ARRAY(&ahp->ah_ini_modes_additional, 
            ar956XModes_fast_clock_aphrodite_1p0,
            ARRAY_LENGTH(ar956XModes_fast_clock_aphrodite_1p0), 3);

#ifdef AR956X_EMULATION
        /* mac emu */
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_CORE],
            ar956X_aphrodite_1p0_mac_core_emulation,
            ARRAY_LENGTH(ar956X_aphrodite_1p0_mac_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_POST],
            ar956X_aphrodite_1p0_mac_postamble_emulation,
            ARRAY_LENGTH(ar956X_aphrodite_1p0_mac_postamble_emulation), 5);

        /* bb emu */
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_CORE],
            ar956X_aphrodite_1p0_baseband_core_emulation,
            ARRAY_LENGTH(ar956X_aphrodite_1p0_baseband_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_POST],
            ar956X_aphrodite_1p0_baseband_postamble_emulation,
            ARRAY_LENGTH(ar956X_aphrodite_1p0_baseband_postamble_emulation), 5);

        /* radio emu */
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_CORE],
            ar9200_merlin_2p0_aphrodite_radio_core,
            ARRAY_LENGTH(ar9200_merlin_2p0_aphrodite_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_POST], NULL, 0, 0);

        /* soc emu */
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_POST], NULL, 0, 0);

        /* rx gain emu */
        INIT_INI_ARRAY(&ahp->ah_ini_rx_gain_emu,
            ar956XCommon_rx_gain_table_merlin_2p0,
            ARRAY_LENGTH(ar956XCommon_rx_gain_table_merlin_2p0), 2);
        /* tx gain emu */
        INIT_INI_ARRAY(&ahp->ah_ini_tx_gain_emu,
            ar956X_aphrodite_1p0_tx_gain_table_baseband_postamble_emulation,
            ARRAY_LENGTH(
            ar956X_aphrodite_1p0_tx_gain_table_baseband_postamble_emulation), 5);
#endif
    } else if (AR_SREV_AR9580(ah)) {
        /*
         * AR9580/Peacock -
         * new INI format (pre, core, post arrays per subsystem)
         */

        /* mac */
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_CORE],
            ar9300_ar9580_1p0_mac_core,
            ARRAY_LENGTH(ar9300_ar9580_1p0_mac_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_POST],
            ar9300_ar9580_1p0_mac_postamble,
            ARRAY_LENGTH(ar9300_ar9580_1p0_mac_postamble), 5);

        /* bb */
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_CORE],
            ar9300_ar9580_1p0_baseband_core,
            ARRAY_LENGTH(ar9300_ar9580_1p0_baseband_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_POST],
            ar9300_ar9580_1p0_baseband_postamble,
            ARRAY_LENGTH(ar9300_ar9580_1p0_baseband_postamble), 5);

        /* radio */
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_CORE],
            ar9300_ar9580_1p0_radio_core,
            ARRAY_LENGTH(ar9300_ar9580_1p0_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_POST],
            ar9300_ar9580_1p0_radio_postamble,
            ARRAY_LENGTH(ar9300_ar9580_1p0_radio_postamble), 5);

        /* soc */
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_PRE],
            ar9300_ar9580_1p0_soc_preamble,
            ARRAY_LENGTH(ar9300_ar9580_1p0_soc_preamble), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_POST],
            ar9300_ar9580_1p0_soc_postamble,
            ARRAY_LENGTH(ar9300_ar9580_1p0_soc_postamble), 5);

        /* rx/tx gain */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
            ar9300_common_rx_gain_table_ar9580_1p0,
            ARRAY_LENGTH(ar9300_common_rx_gain_table_ar9580_1p0), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
            ar9300Modes_lowest_ob_db_tx_gain_table_ar9580_1p0,
            ARRAY_LENGTH(ar9300Modes_lowest_ob_db_tx_gain_table_ar9580_1p0), 5);

        /* DFS */
        INIT_INI_ARRAY(&ahp->ah_ini_dfs,
            ar9300_ar9580_1p0_baseband_postamble_dfs_channel,
            ARRAY_LENGTH(ar9300_ar9580_1p0_baseband_postamble_dfs_channel), 3);


#ifndef AR9300_EMULATION
        /* Load PCIE SERDES settings from INI */

        /*D3 Setting */
        if  (ahpriv->ah_config.ath_hal_pcie_clock_req) {
            if (ahpriv->ah_config.ath_hal_pll_pwr_save &
                AR_PCIE_PLL_PWRSAVE_CONTROL)
            { //registry control
                if (ahpriv->ah_config.ath_hal_pll_pwr_save &
                    AR_PCIE_PLL_PWRSAVE_ON_D3)
                { //bit1, in to D3
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes,
                        ar9300PciePhy_clkreq_enable_L1_ar9580_1p0,
                        ARRAY_LENGTH(ar9300PciePhy_clkreq_enable_L1_ar9580_1p0),
                    2);
                } else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes,
                        ar9300PciePhy_pll_on_clkreq_disable_L1_ar9580_1p0,
                        ARRAY_LENGTH(
                            ar9300PciePhy_pll_on_clkreq_disable_L1_ar9580_1p0),
                    2);
                }
            } else {//no registry control, default is pll on
                INIT_INI_ARRAY(
                    &ahp->ah_ini_pcie_serdes,
                    ar9300PciePhy_pll_on_clkreq_disable_L1_ar9580_1p0,
                    ARRAY_LENGTH(
                        ar9300PciePhy_pll_on_clkreq_disable_L1_ar9580_1p0),
                    2);
            }
        } else {
            if (ahpriv->ah_config.ath_hal_pll_pwr_save &
                AR_PCIE_PLL_PWRSAVE_CONTROL)
            { //registry control
                if (ahpriv->ah_config.ath_hal_pll_pwr_save &
                    AR_PCIE_PLL_PWRSAVE_ON_D3)
                { //bit1, in to D3
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes,
                        ar9300PciePhy_clkreq_disable_L1_ar9580_1p0,
                        ARRAY_LENGTH(
                            ar9300PciePhy_clkreq_disable_L1_ar9580_1p0),
                        2);
                } else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes,
                        ar9300PciePhy_pll_on_clkreq_disable_L1_ar9580_1p0,
                        ARRAY_LENGTH(
                            ar9300PciePhy_pll_on_clkreq_disable_L1_ar9580_1p0),
                        2);
                }
            } else {//no registry control, default is pll on
                INIT_INI_ARRAY(
                    &ahp->ah_ini_pcie_serdes,
                    ar9300PciePhy_pll_on_clkreq_disable_L1_ar9580_1p0,
                    ARRAY_LENGTH(
                        ar9300PciePhy_pll_on_clkreq_disable_L1_ar9580_1p0),
                    2);
            }
        }

        /*D0 Setting */
        if  (ahpriv->ah_config.ath_hal_pcie_clock_req) {
             if (ahpriv->ah_config.ath_hal_pll_pwr_save &
                AR_PCIE_PLL_PWRSAVE_CONTROL)
             { //registry control
                if (ahpriv->ah_config.ath_hal_pll_pwr_save &
                    AR_PCIE_PLL_PWRSAVE_ON_D0)
                { //bit2, out of D3
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power,
                        ar9300PciePhy_clkreq_enable_L1_ar9580_1p0,
                        ARRAY_LENGTH(ar9300PciePhy_clkreq_enable_L1_ar9580_1p0),
                    2);

                } else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power,
                        ar9300PciePhy_pll_on_clkreq_disable_L1_ar9580_1p0,
                        ARRAY_LENGTH(
                            ar9300PciePhy_pll_on_clkreq_disable_L1_ar9580_1p0),
                    2);
                }
            } else { //no registry control, default is pll on
                INIT_INI_ARRAY(
                    &ahp->ah_ini_pcie_serdes_low_power,
                    ar9300PciePhy_pll_on_clkreq_disable_L1_ar9580_1p0,
                    ARRAY_LENGTH(
                        ar9300PciePhy_pll_on_clkreq_disable_L1_ar9580_1p0),
                    2);
            }
        } else {
            if (ahpriv->ah_config.ath_hal_pll_pwr_save &
                AR_PCIE_PLL_PWRSAVE_CONTROL)
            {//registry control
                if (ahpriv->ah_config.ath_hal_pll_pwr_save &
                    AR_PCIE_PLL_PWRSAVE_ON_D0)
                {//bit2, out of D3
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power,
                        ar9300PciePhy_clkreq_disable_L1_ar9580_1p0,
                       ARRAY_LENGTH(ar9300PciePhy_clkreq_disable_L1_ar9580_1p0),
                    2);
                } else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power,
                        ar9300PciePhy_pll_on_clkreq_disable_L1_ar9580_1p0,
                        ARRAY_LENGTH(
                            ar9300PciePhy_pll_on_clkreq_disable_L1_ar9580_1p0),
                    2);
                }
            } else { //no registry control, default is pll on
                INIT_INI_ARRAY(
                    &ahp->ah_ini_pcie_serdes_low_power,
                    ar9300PciePhy_pll_on_clkreq_disable_L1_ar9580_1p0,
                    ARRAY_LENGTH(
                        ar9300PciePhy_pll_on_clkreq_disable_L1_ar9580_1p0),
                    2);
            }
        }

#endif
        ahpriv->ah_config.ath_hal_pcie_power_save_enable = 0;

        /* Fast clock modal settings */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_additional,
            ar9300Modes_fast_clock_ar9580_1p0,
            ARRAY_LENGTH(ar9300Modes_fast_clock_ar9580_1p0), 3);
        INIT_INI_ARRAY(&ahp->ah_ini_japan2484,
            ar9300_ar9580_1p0_baseband_core_txfir_coeff_japan_2484,
            ARRAY_LENGTH(
                ar9300_ar9580_1p0_baseband_core_txfir_coeff_japan_2484), 2);

#ifdef AR9300_EMULATION
        /* mac emu */
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_CORE],
            ar9300_ar9580_1p0_mac_core_emulation,
            ARRAY_LENGTH(ar9300_ar9580_1p0_mac_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_POST],
            ar9300_ar9580_1p0_mac_postamble_emulation,
            ARRAY_LENGTH(ar9300_ar9580_1p0_mac_postamble_emulation), 5);

        /* bb emu */
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_CORE],
            ar9300_ar9580_1p0_baseband_core_emulation,
            ARRAY_LENGTH(ar9300_ar9580_1p0_baseband_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_POST],
            ar9300_ar9580_1p0_baseband_postamble_emulation,
            ARRAY_LENGTH(ar9300_ar9580_1p0_baseband_postamble_emulation), 5);

        /* radio emu */
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_CORE],
            ar9200_merlin_2p2_radio_core,
            ARRAY_LENGTH(ar9200_merlin_2p2_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_POST], NULL, 0, 0);

        /* soc emu */
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_POST], NULL, 0, 0);

        /* rx gain emu */
        INIT_INI_ARRAY(&ahp->ah_ini_rx_gain_emu,
           ar9300Common_rx_gain_table_merlin_2p2,
           ARRAY_LENGTH(ar9300Common_rx_gain_table_merlin_2p2), 2);
#endif
    } else {
        /*
         * Osprey 2.2 -  new INI format (pre, core, post arrays per subsystem)
         */

        /* mac */
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_CORE],
            ar9300_osprey_2p2_mac_core,
            ARRAY_LENGTH(ar9300_osprey_2p2_mac_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac[ATH_INI_POST],
            ar9300_osprey_2p2_mac_postamble,
            ARRAY_LENGTH(ar9300_osprey_2p2_mac_postamble), 5);

        /* bb */
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_CORE],
            ar9300_osprey_2p2_baseband_core,
            ARRAY_LENGTH(ar9300_osprey_2p2_baseband_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb[ATH_INI_POST],
            ar9300_osprey_2p2_baseband_postamble,
            ARRAY_LENGTH(ar9300_osprey_2p2_baseband_postamble), 5);

        /* radio */
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_CORE],
            ar9300_osprey_2p2_radio_core,
            ARRAY_LENGTH(ar9300_osprey_2p2_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio[ATH_INI_POST],
            ar9300_osprey_2p2_radio_postamble,
            ARRAY_LENGTH(ar9300_osprey_2p2_radio_postamble), 5);

        /* soc */
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_PRE],
            ar9300_osprey_2p2_soc_preamble,
            ARRAY_LENGTH(ar9300_osprey_2p2_soc_preamble), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc[ATH_INI_POST],
            ar9300_osprey_2p2_soc_postamble,
            ARRAY_LENGTH(ar9300_osprey_2p2_soc_postamble), 5);

        /* rx/tx gain */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
            ar9300_common_rx_gain_table_osprey_2p2,
            ARRAY_LENGTH(ar9300_common_rx_gain_table_osprey_2p2), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
            ar9300_modes_lowest_ob_db_tx_gain_table_osprey_2p2,
            ARRAY_LENGTH(ar9300_modes_lowest_ob_db_tx_gain_table_osprey_2p2), 5);

        /* DFS */
        INIT_INI_ARRAY(&ahp->ah_ini_dfs,
            ar9300_osprey_2p2_baseband_postamble_dfs_channel,
            ARRAY_LENGTH(ar9300_osprey_2p2_baseband_postamble_dfs_channel), 3);

#ifndef AR9300_EMULATION
        /* Load PCIE SERDES settings from INI */

        /*D3 Setting */
        if  (ahpriv->ah_config.ath_hal_pcie_clock_req) {
            if (ahpriv->ah_config.ath_hal_pll_pwr_save & 
                AR_PCIE_PLL_PWRSAVE_CONTROL) 
            { //registry control
                if (ahpriv->ah_config.ath_hal_pll_pwr_save & 
                    AR_PCIE_PLL_PWRSAVE_ON_D3) 
                { //bit1, in to D3
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes, 
                        ar9300PciePhy_clkreq_enable_L1_osprey_2p2,
                        ARRAY_LENGTH(ar9300PciePhy_clkreq_enable_L1_osprey_2p2),
                    2);
                } else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes, 
                        ar9300PciePhy_pll_on_clkreq_disable_L1_osprey_2p2,
                        ARRAY_LENGTH(
                            ar9300PciePhy_pll_on_clkreq_disable_L1_osprey_2p2),
                    2);
                }
             } else {//no registry control, default is pll on
#ifndef ATH_BUS_PM
                    INIT_INI_ARRAY(
                        &ahp->ah_ini_pcie_serdes,
                        ar9300PciePhy_pll_on_clkreq_disable_L1_osprey_2p2,
                        ARRAY_LENGTH(
                            ar9300PciePhy_pll_on_clkreq_disable_L1_osprey_2p2),
                    2);
#else
        //no registry control, default is pll off
        INIT_INI_ARRAY(
                &ahp->ah_ini_pcie_serdes,
                ar9300PciePhy_clkreq_disable_L1_osprey_2p2,
                ARRAY_LENGTH(
                    ar9300PciePhy_clkreq_disable_L1_osprey_2p2),
                  2);
#endif

            }
        } else {
            if (ahpriv->ah_config.ath_hal_pll_pwr_save & 
                AR_PCIE_PLL_PWRSAVE_CONTROL) 
            { //registry control
                if (ahpriv->ah_config.ath_hal_pll_pwr_save & 
                    AR_PCIE_PLL_PWRSAVE_ON_D3) 
                { //bit1, in to D3
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes, 
                        ar9300PciePhy_clkreq_disable_L1_osprey_2p2,
                        ARRAY_LENGTH(
                            ar9300PciePhy_clkreq_disable_L1_osprey_2p2),
                        2);
                } else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes, 
                       ar9300PciePhy_pll_on_clkreq_disable_L1_osprey_2p2,
                       ARRAY_LENGTH(
                           ar9300PciePhy_pll_on_clkreq_disable_L1_osprey_2p2),
                       2);
                }
             } else {
#ifndef ATH_BUS_PM
        //no registry control, default is pll on
                INIT_INI_ARRAY(
                    &ahp->ah_ini_pcie_serdes,
                    ar9300PciePhy_pll_on_clkreq_disable_L1_osprey_2p2,
                    ARRAY_LENGTH(
                        ar9300PciePhy_pll_on_clkreq_disable_L1_osprey_2p2),
                    2);
#else
        //no registry control, default is pll off
        INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes, ar9300PciePhy_clkreq_disable_L1_osprey_2p2,
                           ARRAY_LENGTH(ar9300PciePhy_clkreq_disable_L1_osprey_2p2), 2);
#endif
            }
        }

        /*D0 Setting */
        if  (ahpriv->ah_config.ath_hal_pcie_clock_req) {
             if (ahpriv->ah_config.ath_hal_pll_pwr_save & 
                AR_PCIE_PLL_PWRSAVE_CONTROL) 
             { //registry control
                if (ahpriv->ah_config.ath_hal_pll_pwr_save & 
                    AR_PCIE_PLL_PWRSAVE_ON_D0)
                { //bit2, out of D3
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power, 
                        ar9300PciePhy_clkreq_enable_L1_osprey_2p2,
                        ARRAY_LENGTH(ar9300PciePhy_clkreq_enable_L1_osprey_2p2),
                    2);

                } else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power, 
                        ar9300PciePhy_pll_on_clkreq_disable_L1_osprey_2p2,
                        ARRAY_LENGTH(
                            ar9300PciePhy_pll_on_clkreq_disable_L1_osprey_2p2),
                    2);
                }
            } else { //no registry control, default is pll on
                INIT_INI_ARRAY(
                    &ahp->ah_ini_pcie_serdes_low_power,
                    ar9300PciePhy_pll_on_clkreq_disable_L1_osprey_2p2,
                    ARRAY_LENGTH(
                        ar9300PciePhy_pll_on_clkreq_disable_L1_osprey_2p2),
                    2);
            }
        } else {
            if (ahpriv->ah_config.ath_hal_pll_pwr_save & 
                AR_PCIE_PLL_PWRSAVE_CONTROL) 
            {//registry control
                if (ahpriv->ah_config.ath_hal_pll_pwr_save & 
                    AR_PCIE_PLL_PWRSAVE_ON_D0)
                {//bit2, out of D3
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power, 
                        ar9300PciePhy_clkreq_disable_L1_osprey_2p2,
                       ARRAY_LENGTH(ar9300PciePhy_clkreq_disable_L1_osprey_2p2),
                    2);
                } else {
                    INIT_INI_ARRAY(&ahp->ah_ini_pcie_serdes_low_power, 
                        ar9300PciePhy_pll_on_clkreq_disable_L1_osprey_2p2,
                        ARRAY_LENGTH(
                            ar9300PciePhy_pll_on_clkreq_disable_L1_osprey_2p2),
                    2);
                }
            } else { //no registry control, default is pll on
                INIT_INI_ARRAY(
                    &ahp->ah_ini_pcie_serdes_low_power,
                    ar9300PciePhy_pll_on_clkreq_disable_L1_osprey_2p2,
                    ARRAY_LENGTH(
                        ar9300PciePhy_pll_on_clkreq_disable_L1_osprey_2p2),
                    2);
            }
        }

#endif
        ahpriv->ah_config.ath_hal_pcie_power_save_enable = 0;

#ifdef ATH_BUS_PM
        /*Use HAL to config PCI powersave by writing into the SerDes Registers */
        ahpriv->ah_config.ath_hal_pcie_ser_des_write = 1;
#endif

        /* Fast clock modal settings */
        INIT_INI_ARRAY(&ahp->ah_ini_modes_additional,
            ar9300Modes_fast_clock_osprey_2p2,
            ARRAY_LENGTH(ar9300Modes_fast_clock_osprey_2p2), 3);
        INIT_INI_ARRAY(&ahp->ah_ini_japan2484,
            ar9300_osprey_2p2_baseband_core_txfir_coeff_japan_2484,
            ARRAY_LENGTH(
                ar9300_osprey_2p2_baseband_core_txfir_coeff_japan_2484), 2);

#ifdef AR9300_EMULATION
        /* mac emu */
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_CORE],
            ar9300_osprey_2p2_mac_core_emulation,
            ARRAY_LENGTH(ar9300_osprey_2p2_mac_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_mac_emu[ATH_INI_POST],
            ar9300_osprey_2p2_mac_postamble_emulation,
            ARRAY_LENGTH(ar9300_osprey_2p2_mac_postamble_emulation), 5);

        /* bb emu */
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_CORE],
            ar9300_osprey_2p2_baseband_core_emulation,
            ARRAY_LENGTH(ar9300_osprey_2p2_baseband_core_emulation), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_bb_emu[ATH_INI_POST],
            ar9300_osprey_2p2_baseband_postamble_emulation,
            ARRAY_LENGTH(ar9300_osprey_2p2_baseband_postamble_emulation), 5);

        /* radio emu */
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_CORE],
            ar9200_merlin_2p2_radio_core,
            ARRAY_LENGTH(ar9200_merlin_2p2_radio_core), 2);
        INIT_INI_ARRAY(&ahp->ah_ini_radio_emu[ATH_INI_POST], NULL, 0, 0);

        /* soc emu */
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_PRE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_CORE], NULL, 0, 0);
        INIT_INI_ARRAY(&ahp->ah_ini_soc_emu[ATH_INI_POST], NULL, 0, 0);

        /* rx gain emu */
        INIT_INI_ARRAY(&ahp->ah_ini_rx_gain_emu,
           ar9300Common_rx_gain_table_merlin_2p2,
           ARRAY_LENGTH(ar9300Common_rx_gain_table_merlin_2p2), 2);
#endif
    }

    if(AR_SREV_WASP(ah) || AR_SREV_SCORPION(ah)|| AR_SREV_HONEYBEE(ah) )
    {
#ifdef _WIN64
#define AR_SOC_RST_OTP_INTF  0xB80600B4ULL
#else
#define AR_SOC_RST_OTP_INTF  0xB80600B4
#endif
#define REG_READ(_reg)       *((volatile u_int32_t *)(_reg))

#ifdef ART_BUILD
#ifdef MDK_AP
        ahp->ah_enterprise_mode = FullAddrRead(AR_SOC_RST_OTP_INTF);
#endif
#else
        ahp->ah_enterprise_mode = REG_READ(AR_SOC_RST_OTP_INTF);
#endif
        if (AR_SREV_SCORPION(ah)) {
            ahp->ah_enterprise_mode = ahp->ah_enterprise_mode << 12;
        }
        ath_hal_printf (ah, "Enterprise mode: 0x%08x\n", ahp->ah_enterprise_mode);
#undef REG_READ
#undef AR_SOC_RST_OTP_INTF
    } 
    else if(AR_SREV_DRAGONFLY(ah) || AR_SREV_JET(ah))
    {
        ahp->ah_enterprise_mode = 0x3BDA000;
        ath_hal_printf (ah, "Enterprise mode: 0x%08x\n", ahp->ah_enterprise_mode);
    }
    else
    {
        ahp->ah_enterprise_mode = OS_REG_READ(ah, AR_ENT_OTP);
    }

    if (ahpriv->ah_is_pci_express) {
        ar9300_config_pci_power_save(ah, 0, 0);
    } else {
        ar9300_disable_pcie_phy(ah);
    }
    ecode = ar9300_hw_attach(ah);
    if (ecode != HAL_OK) {
        goto bad;
    }

    /* set gain table pointers according to values read from the eeprom */
    ar9300_tx_gain_table_apply(ah);
    ar9300_rx_gain_table_apply(ah);

    /*
    **
    ** Got everything we need now to setup the capabilities.
    */

    if (!ar9300_fill_capability_info(ah)) {
        HDPRINTF(ah, HAL_DBG_RESET,
            "%s:failed ar9300_fill_capability_info\n", __func__);
        ecode = HAL_EEREAD;
        goto bad;
    }
    ecode = ar9300_init_mac_addr(ah);
    if (ecode != HAL_OK) {
        HDPRINTF(ah, HAL_DBG_RESET,
            "%s: failed initializing mac address\n", __func__);
        goto bad;
    }

    /*
     * Initialize receive buffer size to MAC default
     */
    ahp->rx_buf_size = HAL_RXBUFSIZE_DEFAULT;

#if ATH_WOW

    /*
     * Clear the Wow Status.
     */
    OS_REG_WRITE(ah, AR_HOSTIF_REG(ah, AR_PCIE_PM_CTRL),
        OS_REG_READ(ah, AR_HOSTIF_REG(ah, AR_PCIE_PM_CTRL)) |
        AR_PMCTRL_WOW_PME_CLR);
    OS_REG_WRITE(ah, AR_WOW_PATTERN_REG,
        AR_WOW_CLEAR_EVENTS(OS_REG_READ(ah, AR_WOW_PATTERN_REG)));
#endif

    /*
     * Set the cur_trig_level to a value that works all modes - 11a/b/g or 11n
     * with aggregation enabled or disabled.
     */
    ahpriv->ah_tx_trig_level = (AR_FTRIG_512B >> AR_FTRIG_S);

    if (AR_SREV_HORNET(ah)) {
        ahpriv->nf_2GHz.nominal = AR_PHY_CCA_NOM_VAL_HORNET_2GHZ;
        if (is_reg_dmn_fcc(ahp->reg_dmn)) {
            ahpriv->nf_2GHz.max     = AR_PHY_CCA_MAX_GOOD_VAL_OSPREY_FCC_2GHZ;
        } else {
            ahpriv->nf_2GHz.max     = AR_PHY_CCA_MAX_GOOD_VAL_OSPREY_2GHZ;
        }
        ahpriv->nf_2GHz.min     = AR_PHY_CCA_MIN_GOOD_VAL_OSPREY_2GHZ;
        ahpriv->nf_5GHz.nominal = AR_PHY_CCA_NOM_VAL_OSPREY_5GHZ;
        if (is_reg_dmn_fcc(ahp->reg_dmn)) {
            ahpriv->nf_5GHz.max     = AR_PHY_CCA_MAX_GOOD_VAL_OSPREY_FCC_5GHZ;
        } else {
            ahpriv->nf_5GHz.max     = AR_PHY_CCA_MAX_GOOD_VAL_OSPREY_5GHZ;
        }
        ahpriv->nf_5GHz.min     = AR_PHY_CCA_MIN_GOOD_VAL_OSPREY_5GHZ;
        ahpriv->nf_cw_int_delta = AR_PHY_CCA_CW_INT_DELTA;
    } else if(AR_SREV_JUPITER(ah) || AR_SREV_APHRODITE(ah)){
        ahpriv->nf_2GHz.nominal = AR_PHY_CCA_NOM_VAL_JUPITER_2GHZ;
        if (is_reg_dmn_fcc(ahp->reg_dmn)) {
            ahpriv->nf_2GHz.max     = AR_PHY_CCA_MAX_GOOD_VAL_OSPREY_FCC_2GHZ;
        } else {
            ahpriv->nf_2GHz.max     = AR_PHY_CCA_MAX_GOOD_VAL_OSPREY_2GHZ;
        }
        ahpriv->nf_2GHz.min     = AR_PHY_CCA_MIN_GOOD_VAL_JUPITER_2GHZ;
        ahpriv->nf_5GHz.nominal = AR_PHY_CCA_NOM_VAL_JUPITER_5GHZ;
        if (is_reg_dmn_fcc(ahp->reg_dmn)) {
            ahpriv->nf_5GHz.max     = AR_PHY_CCA_MAX_GOOD_VAL_OSPREY_FCC_5GHZ;
        } else {
            ahpriv->nf_5GHz.max     = AR_PHY_CCA_MAX_GOOD_VAL_OSPREY_5GHZ;
        }
        ahpriv->nf_5GHz.min     = AR_PHY_CCA_MIN_GOOD_VAL_JUPITER_5GHZ;
        ahpriv->nf_cw_int_delta = AR_PHY_CCA_CW_INT_DELTA;
    }	else {
        ahpriv->nf_2GHz.nominal = AR_PHY_CCA_NOM_VAL_OSPREY_2GHZ;
        if (is_reg_dmn_fcc(ahp->reg_dmn)) {
            ahpriv->nf_2GHz.max     = AR_PHY_CCA_MAX_GOOD_VAL_OSPREY_FCC_2GHZ;
        } else {
            ahpriv->nf_2GHz.max     = AR_PHY_CCA_MAX_GOOD_VAL_OSPREY_2GHZ;
        }
        ahpriv->nf_2GHz.min     = AR_PHY_CCA_MIN_GOOD_VAL_OSPREY_2GHZ;
        if (AR_SREV_AR9580(ah) || AR_SREV_WASP(ah) || AR_SREV_SCORPION(ah)) {
            ahpriv->nf_5GHz.nominal = AR_PHY_CCA_NOM_VAL_PEACOCK_5GHZ;
        } else {
            ahpriv->nf_5GHz.nominal = AR_PHY_CCA_NOM_VAL_OSPREY_5GHZ;
        }
        if (is_reg_dmn_fcc(ahp->reg_dmn)) {
            ahpriv->nf_5GHz.max     = AR_PHY_CCA_MAX_GOOD_VAL_OSPREY_FCC_5GHZ;
        } else {
            ahpriv->nf_5GHz.max     = AR_PHY_CCA_MAX_GOOD_VAL_OSPREY_5GHZ;
        }
        ahpriv->nf_5GHz.min     = AR_PHY_CCA_MIN_GOOD_VAL_OSPREY_5GHZ;
        ahpriv->nf_cw_int_delta = AR_PHY_CCA_CW_INT_DELTA;
        printk("%s: nf_2_nom %d nf_2_max %d nf_2_min %d \n",
                __func__, ahpriv->nf_2GHz.nominal, ahpriv->nf_2GHz.max, ahpriv->nf_2GHz.min);
     }

    /* init BB Panic Watchdog timeout */
    if (AR_SREV_HORNET(ah)) {
        ahpriv->ah_bb_panic_timeout_ms = HAL_BB_PANIC_WD_TMO_HORNET;
    } else {
        ahpriv->ah_bb_panic_timeout_ms = HAL_BB_PANIC_WD_TMO;
    }

#if ATH_SUPPORT_RADIO_RETENTION
    if (AR_SREV_JUPITER_20(ah) &&
        AH_PRIVATE(ah)->ah_caps.hal_radio_retention_support &&
        HAL_RTT_CTRL(ah, AH_RTT_ENABLE_RADIO_RETENTION))
    {
        ahp->radio_retention_chains = 2;
        ahp->radio_retention_enable = 1;
    } else {
        ahp->radio_retention_chains = 0;
        ahp->radio_retention_enable = 0;
    }
#endif /* ATH_SUPPORT_RADIO_RETENTION */

    /*
     * Determine whether tx IQ calibration HW should be enabled,
     * and whether tx IQ calibration should be performed during
     * AGC calibration, or separately.
     */
    if (AR_SREV_JUPITER(ah) || AR_SREV_APHRODITE(ah)) {
        /*
         * Register not initialized yet. This flag will be re-initialized
         * after INI loading following each reset.
         */
        ahp->tx_iq_cal_enable = 1;
        /* if tx IQ cal is enabled, do it together with AGC cal */
        ahp->tx_iq_cal_during_agc_cal = 1;
    } else if (AR_SREV_POSEIDON_OR_LATER(ah) && !AR_SREV_WASP(ah)) {
        ahp->tx_iq_cal_enable = 1;
        ahp->tx_iq_cal_during_agc_cal = 1;
    } else {
        /* osprey, hornet, wasp */
        ahp->tx_iq_cal_enable = 1;
        ahp->tx_iq_cal_during_agc_cal = 0;
    }
#if ATH_TxBF_DYNAMIC_LOF_ON_N_CHAIN_MASK
    ahp->ah_loforce_enabled = 0;
#endif    
    ahp->ah_scanning = 0;
    if (AR_SREV_JET(ah)) {
#ifdef ATH_SUPPORT_SWTXIQ
        ahp->ah_swtxiq_done = SW_TX_IQ_START;
        if(ahp->template_iq == NULL) {
            ahp->template_iq = (struct sample_iq *)ath_hal_malloc(ah, ADDAC_BUF_SIZE * sizeof(struct sample_iq));
        }
        if(ahp->I1toI4 == NULL) {
            ahp->I1toI4 = (int32_t *)ath_hal_malloc(ah, N_CHUNK * MEASURELTH * sizeof(int32_t));
        }
        if(ahp->Q1toQ4 == NULL) {
            ahp->Q1toQ4 = (int32_t *)ath_hal_malloc(ah, N_CHUNK * MEASURELTH * sizeof(int32_t));
        }
#endif
        ar9300_set_temp_Ko_Kg(ah); //calculate and set Thermal Ko,Kg
    }
    return ah;

bad:
    if (ahp) {
        ar9300_detach((struct ath_hal *) ahp);
    }
    if (status) {
        *status = ecode;
    }
    return AH_NULL;
}

void
ar9300_detach(struct ath_hal *ah)
{
#ifdef ATH_SUPPORT_SWTXIQ
    struct ath_hal_9300     *ahp = AH9300(ah);
#endif
    HALASSERT(ah != AH_NULL);
    HALASSERT(AH_PRIVATE(ah)->ah_magic == AR9300_MAGIC);

    /* Make sure that chip is awake before writing to it */
    if (!ar9300_set_power_mode(ah, HAL_PM_AWAKE, true)) {
        HDPRINTF(ah, HAL_DBG_UNMASKABLE,
                 "%s: failed to wake up chip\n",
                 __func__);
    }
    ar9300_hw_detach(ah);
    ar9300_set_power_mode(ah, HAL_PM_FULL_SLEEP, true);

#if ATH_DRIVER_SIM
    if (AH_PRIVATE(ah)->ah_devid == AR9300_DEVID_SIM_PCIE) {
        AHSIM_REG_LOCK_DESTROY(&AH_PRIVATE(ah)->ah_hal_sim);
        AHSIM_unregister_sim_device_instance(ah);
        AHSIM_AR9300_detach(ah);
    }
#endif
    ath_hal_hdprintf_deregister(ah);
    if (AR_SREV_JET(ah)) {
#ifdef ATH_SUPPORT_SWTXIQ
        ath_hal_hdprintf_deregister(ah);
        if(ahp->I1toI4 != NULL) {
            ath_hal_free(ah,ahp->I1toI4);
        }
        if(ahp->Q1toQ4!= NULL) {
            ath_hal_free(ah,ahp->Q1toQ4);
        }
        if(ahp->template_iq != NULL) {
            ath_hal_free(ah,ahp->template_iq);
        }
#endif
    }
    ath_hal_free(ah, ah);
}

struct ath_hal_9300 *
ar9300_new_state(u_int16_t devid, HAL_ADAPTER_HANDLE osdev, HAL_SOFTC sc,
    HAL_BUS_TAG st, HAL_BUS_HANDLE sh, HAL_BUS_TYPE bustype,
    asf_amem_instance_handle amem_handle,
    struct hal_reg_parm *hal_conf_parm, HAL_STATUS *status)
{
    static const u_int8_t defbssidmask[IEEE80211_ADDR_LEN] =
        { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    struct ath_hal_9300 *ahp;
    struct ath_hal *ah;

    /* NB: memory is returned zero'd */
#ifndef ART_BUILD
    ahp = amalloc_adv(
        amem_handle, sizeof(struct ath_hal_9300), qdf_mem_zero_outline);
#else
    ahp = amalloc_adv(
        amem_handle, sizeof(struct ath_hal_9300), adf_os_mem_zero_outline);
#endif
    if (ahp == AH_NULL) {
        HDPRINTF(AH_NULL, HAL_DBG_UNMASKABLE,
                 "%s: cannot allocate memory for state block\n",
                 __func__);
        *status = HAL_ENOMEM;
        return AH_NULL;
    }

    ah = &ahp->ah_priv.priv.h;
    /* set initial values */

    /* Attach Osprey structure as default hal structure */
    OS_MEMCPY(&ahp->ah_priv.priv, &ar9300hal, sizeof(ahp->ah_priv.priv));

    AH_PRIVATE(ah)->amem_handle = amem_handle;
    AH_PRIVATE(ah)->ah_osdev = osdev;
    AH_PRIVATE(ah)->ah_sc = sc;
    AH_PRIVATE(ah)->ah_st = st;
    AH_PRIVATE(ah)->ah_sh = sh;

    AH_PRIVATE(ah)->ah_magic = AR9300_MAGIC;
    AH_PRIVATE(ah)->ah_devid = devid;

    AH_PRIVATE(ah)->ah_flags = 0;
   
    /*
    ** Initialize factory defaults in the private space
    */
    ath_hal_factory_defaults(AH_PRIVATE(ah), hal_conf_parm);

    if (!hal_conf_parm->calInFlash) {
        AH_PRIVATE(ah)->ah_flags |= AH_USE_EEPROM;
    }
   
#if 0
#ifndef WIN32
    if (ar9300_eep_data_in_flash(ah)) {
        ahp->ah_priv.priv.ah_eeprom_read  = ar9300_flash_read;
        ahp->ah_priv.priv.ah_eeprom_dump  = AH_NULL;
#ifdef AH_SUPPORT_WRITE_EEPROM
        ahp->ah_priv.priv.ah_eeprom_write = ar9300_flash_write;
#endif
    } else {
        ahp->ah_priv.priv.ah_eeprom_read  = ar9300_eeprom_read_word;
#ifdef AH_SUPPORT_WRITE_EEPROM
        ahp->ah_priv.priv.ah_eeprom_write = ar9300_eeprom_write;
#endif
    }
#else /* WIN32 */

    ahp->ah_priv.priv.ah_eeprom_read  = ar9300_eeprom_read_word;
#ifdef AH_SUPPORT_WRITE_EEPROM
    ahp->ah_priv.priv.ah_eeprom_write = ar9300_eeprom_write;
#endif

#endif /* WIN32 */
#endif

    AH_PRIVATE(ah)->ah_power_limit = MAX_RATE_POWER;
    AH_PRIVATE(ah)->ah_tp_scale = HAL_TP_SCALE_MAX;  /* no scaling */

    ahp->ah_atim_window = 0;         /* [0..1000] */
    ahp->ah_diversity_control =
        AH_PRIVATE(ah)->ah_config.ath_hal_diversity_control;
    ahp->ah_antenna_switch_swap =
        AH_PRIVATE(ah)->ah_config.ath_hal_antenna_switch_swap;

    /*
     * Enable MIC handling.
     */
    ahp->ah_sta_id1_defaults = AR_STA_ID1_CRPT_MIC_ENABLE;
    ahp->ah_enable32k_hz_clock = DONT_USE_32KHZ;/* XXX */
    ahp->ah_slot_time = (u_int) -1;
    ahp->ah_ack_timeout = (u_int) -1;
    OS_MEMCPY(&ahp->ah_bssid_mask, defbssidmask, IEEE80211_ADDR_LEN);

    /*
     * 11g-specific stuff
     */
    ahp->ah_g_beacon_rate = 0;        /* adhoc beacon fixed rate */

    /* SM power mode: Attach time, disable any setting */
    ahp->ah_sm_power_mode = HAL_SMPS_DEFAULT;

    return ahp;
}

bool
ar9300_chip_test(struct ath_hal *ah)
{
    /*u_int32_t reg_addr[2] = { AR_STA_ID0, AR_PHY_BASE+(8 << 2) };*/
    u_int32_t reg_addr[2] = { AR_STA_ID0 };
    u_int32_t reg_hold[2];
    u_int32_t pattern_data[4] =
        { 0x55555555, 0xaaaaaaaa, 0x66666666, 0x99999999 };
    int i, j;

#if ATH_DRIVER_SIM
    /* Always true but eliminates compilation warning about unreachable code */
    if (1) {
        return true;
    }
#endif

    /* Test PHY & MAC registers */
    for (i = 0; i < 1; i++) {
        u_int32_t addr = reg_addr[i];
        u_int32_t wr_data, rd_data;

        reg_hold[i] = OS_REG_READ(ah, addr);
        for (j = 0; j < 0x100; j++) {
            wr_data = (j << 16) | j;
            OS_REG_WRITE(ah, addr, wr_data);
            rd_data = OS_REG_READ(ah, addr);
            if (rd_data != wr_data) {
                HDPRINTF(ah, HAL_DBG_REG_IO,
                    "%s: address test failed addr: "
                    "0x%08x - wr:0x%08x != rd:0x%08x\n",
                    __func__, addr, wr_data, rd_data);
                return false;
            }
        }
        for (j = 0; j < 4; j++) {
            wr_data = pattern_data[j];
            OS_REG_WRITE(ah, addr, wr_data);
            rd_data = OS_REG_READ(ah, addr);
            if (wr_data != rd_data) {
                HDPRINTF(ah, HAL_DBG_REG_IO,
                    "%s: address test failed addr: "
                    "0x%08x - wr:0x%08x != rd:0x%08x\n",
                    __func__, addr, wr_data, rd_data);
                return false;
            }
        }
        OS_REG_WRITE(ah, reg_addr[i], reg_hold[i]);
    }
    OS_DELAY(100);
    return true;
}

/*
 * Store the channel edges for the requested operational mode
 */
bool
ar9300_get_channel_edges(struct ath_hal *ah,
    u_int16_t flags, u_int16_t *low, u_int16_t *high)
{
    struct ath_hal_private *ahpriv = AH_PRIVATE(ah);
    HAL_CAPABILITIES *p_cap = &ahpriv->ah_caps;

    if (flags & CHANNEL_5GHZ) {
        *low = p_cap->hal_low_5ghz_chan;
        *high = p_cap->hal_high_5ghz_chan;
        return true;
    }

    if ((flags & CHANNEL_2GHZ)) {
        *low = p_cap->hal_low_2ghz_chan;
        *high = p_cap->hal_high_2ghz_chan;

        return true;
    }
    return false;
}

bool
ar9300_regulatory_domain_override(struct ath_hal *ah, u_int16_t regdmn)
{
    AH_PRIVATE(ah)->ah_current_rd = regdmn;
    return true;
}

/*
 * Fill all software cached or static hardware state information.
 * Return failure if capabilities are to come from EEPROM and
 * cannot be read.
 */
bool
ar9300_fill_capability_info(struct ath_hal *ah)
{
#define AR_KEYTABLE_SIZE    128
    struct ath_hal_9300 *ahp = AH9300(ah);
    struct ath_hal_private *ahpriv = AH_PRIVATE(ah);
    HAL_CAPABILITIES *p_cap = &ahpriv->ah_caps;
    u_int16_t cap_field = 0, eeval;

    ahpriv->ah_devType = (u_int16_t)ar9300_eeprom_get(ahp, EEP_DEV_TYPE);
    eeval = ar9300_eeprom_get(ahp, EEP_REG_0);

    /* XXX record serial number */
    AH_PRIVATE(ah)->ah_current_rd = eeval;

    p_cap->halintr_mitigation = true;
    eeval = ar9300_eeprom_get(ahp, EEP_REG_1);
    AH_PRIVATE(ah)->ah_current_rd_ext = eeval | AR9300_RDEXT_DEFAULT;

    /* Read the capability EEPROM location */
    cap_field = ar9300_eeprom_get(ahp, EEP_OP_CAP);

    /* Construct wireless mode from EEPROM */
    p_cap->hal_wireless_modes = 0;
    eeval = ar9300_eeprom_get(ahp, EEP_OP_MODE);

    if (eeval & AR9300_OPFLAGS_11A) {
        p_cap->hal_wireless_modes |= HAL_MODE_11A |
            ((!ahpriv->ah_config.ath_hal_ht_enable ||
              (eeval & AR9300_OPFLAGS_N_5G_HT20)) ?  0 :
             (HAL_MODE_11NA_HT20 | ((eeval & AR9300_OPFLAGS_N_5G_HT40) ? 0 :
                                    (HAL_MODE_11NA_HT40PLUS | HAL_MODE_11NA_HT40MINUS))));
    }
    if (eeval & AR9300_OPFLAGS_11G) {
        p_cap->hal_wireless_modes |= HAL_MODE_11B | HAL_MODE_11G |
            ((!ahpriv->ah_config.ath_hal_ht_enable ||
              (eeval & AR9300_OPFLAGS_N_2G_HT20)) ?  0 :
             (HAL_MODE_11NG_HT20 | ((eeval & AR9300_OPFLAGS_N_2G_HT40) ? 0 :
                                    (HAL_MODE_11NG_HT40PLUS | HAL_MODE_11NG_HT40MINUS))));
    }

    /* Get chainamsks from eeprom */
    p_cap->hal_tx_chain_mask = ar9300_eeprom_get(ahp, EEP_TX_MASK);
    p_cap->hal_rx_chain_mask = ar9300_eeprom_get(ahp, EEP_RX_MASK);
#ifdef AR9300_EMULATION
    if (AR_SREV_JUPITER(ah) || AR_SREV_DRAGONFLY(ah)) {
        p_cap->hal_tx_chain_mask = 0x03;
        p_cap->hal_rx_chain_mask = 0x03;
    }
    else if (AR_SREV_JET(ah)) {
        p_cap->hal_tx_chain_mask = 0x0F;
        p_cap->hal_rx_chain_mask = 0x0F;
    }
#endif
    /* Converting max supported chainmask to max number of chains */
    ah->ah_max_chainmask = p_cap->hal_tx_chain_mask;
    ah->ah_max_chains = ar9300_get_ntxchains(ah->ah_max_chainmask);
    /*
     * This being a newer chip supports TKIP non-splitmic mode.
     *
     */
    ahp->ah_misc_mode |= AR_PCU_MIC_NEW_LOC_ENA;


    p_cap->hal_low_2ghz_chan = 2312;
    p_cap->hal_high_2ghz_chan = 2732;

    p_cap->hal_low_5ghz_chan = 4920;
    p_cap->hal_high_5ghz_chan = 6100;

    p_cap->hal_cipher_ckip_support = false;
    p_cap->hal_cipher_tkip_support = true;
    p_cap->hal_cipher_aes_ccm_support = true;

    p_cap->hal_mic_ckip_support    = false;
    p_cap->hal_mic_tkip_support    = true;
    p_cap->hal_mic_aes_ccm_support  = true;

    p_cap->hal_chan_spread_support = true;
    p_cap->hal_sleep_after_beacon_broken = true;

    p_cap->hal_burst_support = true;
    p_cap->hal_chap_tuning_support = true;
    p_cap->hal_turbo_prime_support = true;
    p_cap->hal_fast_frames_support = false;

    p_cap->hal_turbo_g_support = p_cap->hal_wireless_modes & HAL_MODE_108G;

    p_cap->hal_xr_support = false;

    p_cap->hal_ht_support =
        ahpriv->ah_config.ath_hal_ht_enable ?  true : false;
    p_cap->hal_gtt_support = true;
    p_cap->hal_ps_poll_broken = true;    /* XXX fixed in later revs? */
    p_cap->hal_ht20_sgi_support = true;
    p_cap->hal_veol_support = true;
    p_cap->hal_bss_id_mask_support = true;
    /* Bug 26802, fixed in later revs? */
    p_cap->hal_mcast_key_srch_support = true;
    p_cap->hal_tsf_add_support = true;

    if (cap_field & AR_EEPROM_EEPCAP_MAXQCU) {
        p_cap->hal_total_queues = MS(cap_field, AR_EEPROM_EEPCAP_MAXQCU);
    } else {
        p_cap->hal_total_queues = HAL_NUM_TX_QUEUES;
    }

    if (cap_field & AR_EEPROM_EEPCAP_KC_ENTRIES) {
        p_cap->hal_key_cache_size =
            1 << MS(cap_field, AR_EEPROM_EEPCAP_KC_ENTRIES);
    } else {
        p_cap->hal_key_cache_size = AR_KEYTABLE_SIZE;
    }
    p_cap->hal_fast_cc_support = true;
    p_cap->hal_num_mr_retries = 4;
    p_cap->hal_tx_trig_level_max = MAX_TX_FIFO_THRESHOLD;

    p_cap->hal_num_gpio_pins = AR9382_MAX_GPIO_PIN_NUM;

#if 0
    /* XXX Verify support in Osprey */
    if (AR_SREV_MERLIN_10_OR_LATER(ah)) {
        p_cap->hal_wow_support = true;
        p_cap->hal_wow_match_pattern_exact = true;
        if (AR_SREV_MERLIN(ah)) {
            p_cap->hal_wow_pattern_match_dword = true;
        }
    } else {
        p_cap->hal_wow_support = false;
        p_cap->hal_wow_match_pattern_exact = false;
    }
#endif
    p_cap->hal_wow_support = true;
    p_cap->hal_wow_match_pattern_exact = true;
    if (AR_SREV_POSEIDON(ah)) {
        p_cap->hal_wow_match_pattern_exact = true;
    }

    p_cap->hal_cst_support = true;

    p_cap->hal_rifs_rx_support = true;
    p_cap->hal_rifs_tx_support = true;

    p_cap->hal_rts_aggr_limit = IEEE80211_AMPDU_LIMIT_MAX;

    p_cap->hal_mfp_support = ahpriv->ah_config.ath_hal_mfp_support;

    p_cap->halforce_ppm_support = true;
    p_cap->hal_hw_beacon_proc_support = true;
    
    /* ar9300 - has the HW UAPSD trigger support,
     * but it has the following limitations
     * The power state change from the following
     * frames are not put in High priority queue.
     *     i) Mgmt frames
     *     ii) NoN QoS frames
     *     iii) QoS frames form the access categories for which
     *          UAPSD is not enabled.
     * so we can not enable this feature currently.
     * could be enabled, if these limitations are fixed
     * in later versions of ar9300 chips
     */
    p_cap->hal_hw_uapsd_trig = false;

    /* Number of buffers that can be help in a single TxD */
    p_cap->hal_num_tx_maps = 4;

    if (AR_SREV_JET(ah)) {
        p_cap->hal_tx_desc_len = sizeof(struct qcn5500_txc);
        p_cap->hal_rx_status_len = sizeof(struct qcn5500_rxs);
    } else {
        p_cap->hal_tx_desc_len = sizeof(struct ar9300_txc);
        p_cap->hal_rx_status_len = sizeof(struct ar9300_rxs);
    }
    p_cap->hal_tx_status_len = sizeof(struct ar9300_txs);

    p_cap->hal_rx_hp_depth = HAL_HP_RXFIFO_DEPTH;
    p_cap->hal_rx_lp_depth = HAL_LP_RXFIFO_DEPTH;

    /* Enable extension channel DFS support */
    p_cap->hal_use_combined_radar_rssi = true;
    p_cap->hal_ext_chan_dfs_support = true;
#if ATH_SUPPORT_SPECTRAL
    p_cap->hal_spectral_scan = true;
#endif

    ahpriv->ah_rfsilent = ar9300_eeprom_get(ahp, EEP_RF_SILENT);
    if (ahpriv->ah_rfsilent & EEP_RFSILENT_ENABLED) {
        ahp->ah_gpio_select = MS(ahpriv->ah_rfsilent, EEP_RFSILENT_GPIO_SEL);
        ahp->ah_polarity   = MS(ahpriv->ah_rfsilent, EEP_RFSILENT_POLARITY);

        ath_hal_enable_rfkill(ah, true);
        p_cap->hal_rf_silent_support = true;
    }

    /* XXX */
    p_cap->hal_wps_push_button = false;

#ifdef ATH_BT_COEX
    p_cap->hal_bt_coex_support = true;
    p_cap->hal_bt_coex_aspm_war = false;
#endif

    p_cap->hal_gen_timer_support = true;
    ahp->ah_avail_gen_timers = ~((1 << AR_FIRST_NDP_TIMER) - 1);
    ahp->ah_avail_gen_timers &= (1 << AR_NUM_GEN_TIMERS) - 1;
    /*
     * According to Kyungwan, generic timer 0 and 8 are special
     * timers. Remove timer 8 from the available gen timer list.
     * Jupiter testing shows timer won't trigger with timer 8.
     */
    ahp->ah_avail_gen_timers &= ~(1 << AR_GEN_TIMER_RESERVED);

    if (AR_SREV_JUPITER(ah) || AR_SREV_APHRODITE(ah)) {
#if ATH_SUPPORT_MCI
        if (ahpriv->ah_config.ath_hal_mci_config & ATH_MCI_CONFIG_DISABLE_MCI) 
        {
            p_cap->hal_mci_support = false;
        }
        else
#endif
        {
            p_cap->hal_mci_support = (ahp->ah_enterprise_mode & 
                            AR_ENT_OTP_49GHZ_DISABLE) ? false : true;
        }
        HDPRINTF(AH_NULL, HAL_DBG_UNMASKABLE,
                 "%s: (MCI) MCI support = %d\n",
                 __func__, p_cap->hal_mci_support);
    }
    else {
        p_cap->hal_mci_support = false;
    }

    if (AR_SREV_JUPITER_20(ah)) {
        p_cap->hal_radio_retention_support = true;
    } else {
        p_cap->hal_radio_retention_support = false;
    }

    p_cap->hal_auto_sleep_support = true;

    p_cap->hal_mbssid_aggr_support = true;
    p_cap->hal_proxy_sta_support = true;

    /* XXX Mark it true after it is verfied as fixed */
    p_cap->hal4kb_split_trans_support = false;

    /* Read regulatory domain flag */
    if (AH_PRIVATE(ah)->ah_current_rd_ext & (1 << REG_EXT_JAPAN_MIDBAND)) {
        /*
         * If REG_EXT_JAPAN_MIDBAND is set, turn on U1 EVEN, U2, and MIDBAND.
         */
        p_cap->hal_reg_cap =
            AR_EEPROM_EEREGCAP_EN_KK_NEW_11A |
            AR_EEPROM_EEREGCAP_EN_KK_U1_EVEN |
            AR_EEPROM_EEREGCAP_EN_KK_U2      |
            AR_EEPROM_EEREGCAP_EN_KK_MIDBAND;
    } else {
        p_cap->hal_reg_cap =
            AR_EEPROM_EEREGCAP_EN_KK_NEW_11A | AR_EEPROM_EEREGCAP_EN_KK_U1_EVEN;
    }

    /* For AR9300 and above, midband channels are always supported */
    p_cap->hal_reg_cap |= AR_EEPROM_EEREGCAP_EN_FCC_MIDBAND;

    p_cap->hal_num_ant_cfg_5ghz =
        ar9300_eeprom_get_num_ant_config(ahp, HAL_FREQ_BAND_5GHZ);
    p_cap->hal_num_ant_cfg_2ghz =
        ar9300_eeprom_get_num_ant_config(ahp, HAL_FREQ_BAND_2GHZ);

    /* STBC supported */
    p_cap->hal_rx_stbc_support = 1; /* number of streams for STBC recieve. */
    if (AR_SREV_HORNET(ah) || AR_SREV_POSEIDON(ah) || AR_SREV_APHRODITE(ah)) {
        p_cap->hal_tx_stbc_support = 0;
    } else {
        p_cap->hal_tx_stbc_support = 1;        
    }

    p_cap->hal_enhanced_dma_support = true;
#ifdef ATH_SUPPORT_DFS
    p_cap->hal_enhanced_dfs_support = true;
#endif

    /*
     *  EV61133 (missing interrupts due to AR_ISR_RAC).
     *  Fixed in Osprey 2.0.
     */
    p_cap->hal_isr_rac_support = true;

    p_cap->hal_wep_tkip_aggr_support = true;
    p_cap->hal_wep_tkip_aggr_num_tx_delim = 10;    /* TBD */
    p_cap->hal_wep_tkip_aggr_num_rx_delim = 10;    /* TBD */
    p_cap->hal_wep_tkip_max_ht_rate = 15;         /* TBD */
    p_cap->hal_cfend_fix_support = false;
    p_cap->hal_aggr_extra_delim_war = false;
    p_cap->hal_rx_desc_timestamp_bits = 32;
    p_cap->hal_rx_tx_abort_support = true;
    p_cap->hal_ani_poll_interval = AR9300_ANI_POLLINTERVAL;
    p_cap->hal_channel_switch_time_usec = AR9300_CHANNEL_SWITCH_TIME_USEC;
#if ATH_SUPPORT_RAW_ADC_CAPTURE
    p_cap->hal_raw_adc_capture = true;
#endif
  
#if ATH_SUPPORT_WRAP
    if (AR_SREV_SCORPION(ah) || AR_SREV_DRAGONFLY(ah) || AR_SREV_JET(ah)) {
        p_cap->hal_proxy_sta_rx_war = true;
    } else {
        p_cap->hal_proxy_sta_rx_war = false;
    }
#endif
    /* Transmit Beamforming supported, fill capabilities */
#ifdef  ATH_SUPPORT_TxBF 
    if(!AR_SREV_DRAGONFLY(ah) && !AR_SREV_JET(ah)) {
        p_cap->hal_tx_bf_support = true;
        ar9300_fill_txbf_capabilities(ah);
    } else {
        p_cap->hal_tx_bf_support = false;
    }
#endif
    p_cap->hal_paprd_enabled = ar9300_eeprom_get(ahp, EEP_PAPRD_ENABLED);
    p_cap->hal_chan_half_rate =
        !(ahp->ah_enterprise_mode & AR_ENT_OTP_10MHZ_DISABLE);
    p_cap->hal_chan_quarter_rate =
        !(ahp->ah_enterprise_mode & AR_ENT_OTP_5MHZ_DISABLE);
	
    if(AR_SREV_JUPITER(ah) || AR_SREV_APHRODITE(ah)){
        /* There is no AR_ENT_OTP_49GHZ_DISABLE feature in Jupiter, now the bit is used to disable BT. */		
        p_cap->hal49Ghz = 1;
    } else {
        p_cap->hal49Ghz = !(ahp->ah_enterprise_mode & AR_ENT_OTP_49GHZ_DISABLE);
    }

    if (AR_SREV_POSEIDON(ah) || AR_SREV_HORNET(ah) || AR_SREV_APHRODITE(ah)) {
        /* LDPC supported */
        /* Poseidon doesn't support LDPC, or it will cause receiver CRC Error */
        p_cap->hal_ldpc_support = HAL_LDPC_NONE;
        /* PCI_E LCR offset */
        if (AR_SREV_POSEIDON(ah)) {
            p_cap->hal_pcie_lcr_offset = 0x80; /*for Poseidon*/
        }
        /*WAR method for APSM L0s with Poseidon 1.0*/
        if (AR_SREV_POSEIDON_10(ah)) {
            p_cap->hal_pcie_lcr_extsync_en = true;
        }
    } else if (AR_SREV_DRAGONFLY(ah) || AR_SREV_JET(ah)) {
        p_cap->hal_ldpc_support = HAL_LDPC_TX;
    } else {
        p_cap->hal_ldpc_support = HAL_LDPC_TXRX;
    }
    
    p_cap->hal_enable_apm = ar9300_eeprom_get(ahp, EEP_CHAIN_MASK_REDUCE);
#if ATH_ANT_DIV_COMB        
    if (AR_SREV_HORNET(ah) || AR_SREV_POSEIDON_11_OR_LATER(ah)) {
        if (ahp->ah_diversity_control == HAL_ANT_VARIABLE) {
            u_int8_t ant_div_control1 = 
                ar9300_eeprom_get(ahp, EEP_ANTDIV_control);
            /* if enable_lnadiv is 0x1 and enable_fast_div is 0x1, 
             * we enable the diversity-combining algorithm. 
             */
            if ((ant_div_control1 >> 0x6) == 0x3) {
                p_cap->hal_ant_div_comb_support = true;
            }            
            p_cap->hal_ant_div_comb_support_org = p_cap->hal_ant_div_comb_support;
        }
    }
#endif /* ATH_ANT_DIV_COMB */

#if ATH_SUPPORT_WAPI
    /*
     * WAPI engine support 2 stream rates at most currently
     */
    p_cap->hal_wapi_max_tx_chains = 2;
    p_cap->hal_wapi_max_rx_chains = 2;
#endif

#if ATH_WOW_OFFLOAD
    if (AR_SREV_JUPITER_20_OR_LATER(ah) || AR_SREV_APHRODITE(ah)) {
        p_cap->hal_wow_gtk_offload_support    = true;
        p_cap->hal_wow_arp_offload_support    = true;
        p_cap->hal_wow_ns_offload_support     = true;
        p_cap->hal_wow_4way_hs_wakeup_support = true;
        p_cap->hal_wow_acer_magic_support     = true;
        p_cap->hal_wow_acer_swka_support      = true;
    } else {
        p_cap->hal_wow_gtk_offload_support    = false;
        p_cap->hal_wow_arp_offload_support    = false;
        p_cap->hal_wow_ns_offload_support     = false;
        p_cap->hal_wow_4way_hs_wakeup_support = false;
        p_cap->hal_wow_acer_magic_support     = false;
        p_cap->hal_wow_acer_swka_support      = false;
    }
#endif /* ATH_WOW_OFFLOAD */


    return true;
#undef AR_KEYTABLE_SIZE
}

static bool
ar9300_get_chip_power_limits(struct ath_hal *ah, HAL_CHANNEL *chans,
    u_int32_t nchans)
{
    struct ath_hal_9300 *ahp = AH9300(ah);

    return ahp->ah_rf_hal.get_chip_power_lim(ah, chans, nchans);
}

/*
 * Disable PLL when in L0s as well as receiver clock when in L1.
 * This power saving option must be enabled through the Serdes.
 *
 * Programming the Serdes must go through the same 288 bit serial shift
 * register as the other analog registers.  Hence the 9 writes.
 *
 * XXX Clean up the magic numbers.
 */
void
ar9300_config_pci_power_save(struct ath_hal *ah, int restore, int power_off)
{
#ifndef AR9340_EMULATION
    struct ath_hal_9300 *ahp = AH9300(ah);
    int i;
#endif

    if (AH_PRIVATE(ah)->ah_is_pci_express != true) {
        return;
    }

    /*
     * Increase L1 Entry Latency. Some WB222 boards don't have
     * this change in eeprom/OTP.
     */
    if (AR_SREV_JUPITER(ah)) {
        u_int32_t val = AH_PRIVATE(ah)->ah_config.ath_hal_war70c;
        if ((val & 0xff000000) == 0x17000000) {
            val &= 0x00ffffff;
            val |= 0x27000000;
            OS_REG_WRITE(ah, 0x570c, val);
        }
    }

    /* Do not touch SERDES registers */
    if (AH_PRIVATE(ah)->ah_config.ath_hal_pcie_power_save_enable == 2) {
        return;
    }

    /* Nothing to do on restore for 11N */
    if (!restore) {
        /* set bit 19 to allow forcing of pcie core into L1 state */
        OS_REG_SET_BIT(ah,
            AR_HOSTIF_REG(ah, AR_PCIE_PM_CTRL), AR_PCIE_PM_CTRL_ENA);

#ifndef AR9340_EMULATION
        /*
         * Set PCIE workaround config only if requested, else use the reset
         * value of this register.
         */
        if (AH_PRIVATE(ah)->ah_config.ath_hal_pcie_waen) {
            OS_REG_WRITE(ah,
                AR_HOSTIF_REG(ah, AR_WA),
                AH_PRIVATE(ah)->ah_config.ath_hal_pcie_waen);
        } else {
            /* Set Bits 17 and 14 in the AR_WA register. */
            OS_REG_WRITE(ah, AR_HOSTIF_REG(ah, AR_WA), ahp->ah_wa_reg_val);
        }
    }

    /* Configure PCIE after Ini init. SERDES values now come from ini file */
    if (AH_PRIVATE(ah)->ah_config.ath_hal_pcie_ser_des_write) {
        if (power_off) {
            for (i = 0; i < ahp->ah_ini_pcie_serdes.ia_rows; i++) {
                OS_REG_WRITE(ah,
                    INI_RA(&ahp->ah_ini_pcie_serdes, i, 0),
                    INI_RA(&ahp->ah_ini_pcie_serdes, i, 1));
            }
        } else {
            for (i = 0; i < ahp->ah_ini_pcie_serdes_low_power.ia_rows; i++) {
                OS_REG_WRITE(ah,
                    INI_RA(&ahp->ah_ini_pcie_serdes_low_power, i, 0),
                    INI_RA(&ahp->ah_ini_pcie_serdes_low_power, i, 1));
            }
        }
#endif
    }

}

/*
 * Recipe from charles to turn off PCIe PHY in PCI mode for power savings
 */
void
ar9300_disable_pcie_phy(struct ath_hal *ah)
{
    /* Osprey does not support PCI mode */
}

static inline HAL_STATUS
ar9300_init_mac_addr(struct ath_hal *ah)
{
    u_int32_t sum;
    int i;
    u_int16_t eeval;
    struct ath_hal_9300 *ahp = AH9300(ah);
    u_int32_t EEP_MAC [] = { EEP_MAC_LSW, EEP_MAC_MID, EEP_MAC_MSW };

    sum = 0;
    for (i = 0; i < 3; i++) {
        eeval = ar9300_eeprom_get(ahp, EEP_MAC[i]);
        sum += eeval;
        ahp->ah_macaddr[2*i] = eeval >> 8;
        ahp->ah_macaddr[2*i + 1] = eeval & 0xff;
    }
    if (sum == 0 || sum == 0xffff*3) {
        HDPRINTF(ah, HAL_DBG_EEPROM, "%s: mac address read failed: %s\n",
            __func__, ath_hal_ether_sprintf(ahp->ah_macaddr));
        return HAL_EEBADMAC;
    }
    return HAL_OK;

}

/*
 * Code for the "real" chip i.e. non-emulation. Review and revisit
 * when actual hardware is at hand.
 */
static inline HAL_STATUS
ar9300_hw_attach(struct ath_hal *ah)
{
    HAL_STATUS ecode;

    if (!ar9300_chip_test(ah)) {
        HDPRINTF(ah, HAL_DBG_REG_IO,
            "%s: hardware self-test failed\n", __func__);
        return HAL_ESELFTEST;
    }

    ecode = ar9300_eeprom_attach(ah);
    if (ecode != HAL_OK) {
        return ecode;
    }
#ifdef ATH_CCX
    ar9300_record_serial_number(ah);
#endif
    if (!ar9300_rf_attach(ah, &ecode)) {
        HDPRINTF(ah, HAL_DBG_RESET, "%s: RF setup failed, status %u\n",
            __func__, ecode);
    }

    if (ecode != HAL_OK) {
        return ecode;
    }
#ifndef AR9300_EMULATION
    ar9300_ani_attach(ah);
#endif

    return HAL_OK;
}

static inline void
ar9300_hw_detach(struct ath_hal *ah)
{
#ifndef AR9300_EMULATION
    /* XXX EEPROM allocated state */
    ar9300_ani_detach(ah);
#endif
}

static int16_t
ar9300_get_nf_adjust(struct ath_hal *ah, const HAL_CHANNEL_INTERNAL *c)
{
    return 0;
}

#ifdef ATH_CCX
static bool
ar9300_record_serial_number(struct ath_hal *ah)
{
    int      i;
    struct ath_hal_9300 *ahp = AH9300(ah);
    u_int8_t    *sn = (u_int8_t*)ahp->ah_priv.priv.ser_no;
    u_int8_t    *data = ar9300_eeprom_get_cust_data(ahp);
    for (i = 0; i < AR_EEPROM_SERIAL_NUM_SIZE; i++) {
        sn[i] = data[i];
    }

    sn[AR_EEPROM_SERIAL_NUM_SIZE] = '\0';

    return true;
}
#endif

void
ar9300_set_immunity(struct ath_hal *ah, bool enable)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    u_int32_t m1_thresh_low = enable ? 127 : ahp->ah_immunity_vals[0],
              m2_thresh_low = enable ? 127 : ahp->ah_immunity_vals[1],
              m1_thresh = enable ? 127 : ahp->ah_immunity_vals[2],
              m2_thresh = enable ? 127 : ahp->ah_immunity_vals[3],
              m2_count_thr = enable ? 31 : ahp->ah_immunity_vals[4],
              m2_count_thr_low = enable ? 63 : ahp->ah_immunity_vals[5];

    if (ahp->ah_immunity_on == enable) {
        return;
    }

    ahp->ah_immunity_on = enable;

    OS_REG_RMW_FIELD(ah, AR_PHY_SFCORR_LOW,
                     AR_PHY_SFCORR_LOW_M1_THRESH_LOW, m1_thresh_low);
    OS_REG_RMW_FIELD(ah, AR_PHY_SFCORR_LOW,
                     AR_PHY_SFCORR_LOW_M2_THRESH_LOW, m2_thresh_low);
    OS_REG_RMW_FIELD(ah, AR_PHY_SFCORR,
                     AR_PHY_SFCORR_M1_THRESH, m1_thresh);
    OS_REG_RMW_FIELD(ah, AR_PHY_SFCORR,
                     AR_PHY_SFCORR_M2_THRESH, m2_thresh);
    OS_REG_RMW_FIELD(ah, AR_PHY_SFCORR,
                     AR_PHY_SFCORR_M2COUNT_THR, m2_count_thr);
    OS_REG_RMW_FIELD(ah, AR_PHY_SFCORR_LOW,
                     AR_PHY_SFCORR_LOW_M2COUNT_THR_LOW, m2_count_thr_low);

    OS_REG_RMW_FIELD(ah, AR_PHY_SFCORR_EXT,
                     AR_PHY_SFCORR_EXT_M1_THRESH_LOW, m1_thresh_low);
    OS_REG_RMW_FIELD(ah, AR_PHY_SFCORR_EXT,
                     AR_PHY_SFCORR_EXT_M2_THRESH_LOW, m2_thresh_low);
    OS_REG_RMW_FIELD(ah, AR_PHY_SFCORR_EXT,
                     AR_PHY_SFCORR_EXT_M1_THRESH, m1_thresh);
    OS_REG_RMW_FIELD(ah, AR_PHY_SFCORR_EXT,
                     AR_PHY_SFCORR_EXT_M2_THRESH, m2_thresh);

    if (!enable) {
        OS_REG_SET_BIT(ah, AR_PHY_SFCORR_LOW,
                       AR_PHY_SFCORR_LOW_USE_SELF_CORR_LOW);
    } else {
        OS_REG_CLR_BIT(ah, AR_PHY_SFCORR_LOW,
                       AR_PHY_SFCORR_LOW_USE_SELF_CORR_LOW);
    }
}

int
ar9300_get_cal_intervals(struct ath_hal *ah, HAL_CALIBRATION_TIMER **timerp,
    HAL_CAL_QUERY query)
{
#define AR9300_IS_CHAIN_RX_IQCAL_INVALID(_ah, _reg) \
    ((OS_REG_READ((_ah), _reg) & 0x3fff) == 0)
#define AR9300_IS_RX_IQCAL_DISABLED(_ah) \
    (!(OS_REG_READ((_ah), AR_PHY_RX_IQCAL_CORR_B0) & \
    AR_PHY_RX_IQCAL_CORR_IQCORR_ENABLE))
/* Avoid compilation warnings. Variables are not used when EMULATION. */
#ifndef AR9300_EMULATION_BB
    struct ath_hal_9300 *ahp = AH9300(ah);
    u_int8_t rxchainmask = ahp->ah_rx_chainmask, i;
    int rx_iqcal_invalid = 0, num_chains = 0;
    static const u_int32_t offset_array[4] = {
        AR_PHY_RX_IQCAL_CORR_B0,
        AR_PHY_RX_IQCAL_CORR_B1,
        AR_PHY_RX_IQCAL_CORR_B2,
        QCN5500_PHY_RX_IQCAL_CORR_B3};
#endif
    *timerp = ar9300_cals;

    switch (query) {
    case HAL_QUERY_CALS:
#ifdef AR9300_EMULATION_BB
    /* cals not supported by emulation */
        return 0;
#else
        return AR9300_NUM_CAL_TYPES;
#endif
    case HAL_QUERY_RERUN_CALS:
#ifdef AR9300_EMULATION_BB
    /* cals not supported by emulation */
        return 0;
#else
        for (i = 0; i < AR9300_MAX_CHAINS; i++) {
            if (rxchainmask & (1 << i)) {
                num_chains++;
            }
        }
        for (i = 0; i < num_chains; i++) {
            if (AR_SREV_POSEIDON(ah) || AR_SREV_APHRODITE(ah)) {
                HALASSERT(num_chains == 0x1);
            }
            if (AR9300_IS_CHAIN_RX_IQCAL_INVALID(ah, offset_array[i])) {
                rx_iqcal_invalid = 1;
            }
        }
        if (AR9300_IS_RX_IQCAL_DISABLED(ah)) {
            rx_iqcal_invalid = 1;
        }

        return rx_iqcal_invalid;
#endif
    default:
        HALASSERT(0);
    }
    return 0;
}

#if ATH_TRAFFIC_FAST_RECOVER
#define PLL3              0x16188
#define PLL3_DO_MEAS_MASK 0x40000000
#define PLL4              0x1618c
#define PLL4_MEAS_DONE    0x8
#define SQSUM_DVC_MASK    0x007ffff8
unsigned long
ar9300_get_pll3_sqsum_dvc(struct ath_hal *ah)
{
    if (AR_SREV_HORNET(ah) || AR_SREV_POSEIDON(ah) || AR_SREV_WASP(ah) || AR_SREV_SCORPION(ah)) {
        OS_REG_WRITE(ah, PLL3, (OS_REG_READ(ah, PLL3) & ~(PLL3_DO_MEAS_MASK)));
        OS_DELAY(100);
        OS_REG_WRITE(ah, PLL3, (OS_REG_READ(ah, PLL3) | PLL3_DO_MEAS_MASK));

        while ( (OS_REG_READ(ah, PLL4) & PLL4_MEAS_DONE) == 0) {
            OS_DELAY(100);
        }

        return (( OS_REG_READ(ah, PLL3) & SQSUM_DVC_MASK ) >> 3);
    } else {
        HDPRINTF(ah, HAL_DBG_UNMASKABLE,
                 "%s: unable to get pll3_sqsum_dvc\n",
                 __func__);
        return 0;
    }
}
#endif


#define RX_GAIN_TABLE_LENGTH	128
// this will be called if rfGainCAP is enabled and rfGainCAP setting is changed,
// or rxGainTable setting is changed
bool ar9300_rf_gain_cap_apply(struct ath_hal *ah, int is_2GHz)
{
	int i, done = 0, i_rx_gain = 32;
    u_int32_t rf_gain_cap;
    u_int32_t rx_gain_value, a_Byte, rx_gain_value_caped;
	static u_int32_t  rx_gain_table[RX_GAIN_TABLE_LENGTH * 2][2];
#ifndef ART_BUILD
    ar9300_eeprom_t *eep = &AH9300(ah)->ah_eeprom;
#else
    ar9300_eeprom_t *eep = &AH9300(ah)->ah_eeprom;
    ar5500_eeprom_t *eep_jet = &AH9300(ah)->ah_eeprom_jet;
    u_int8_t  misc_config;
#endif
    struct ath_hal_9300 *ahp = AH9300(ah);


#ifndef ART_BUILD
    if ( !((eep->base_eep_header.misc_configuration & 0x80) >> 7) )
        return false;
#else
    if (!AR_SREV_JET(ah))
        misc_config = eep->base_eep_header.misc_configuration;
    else
        misc_config = eep_jet->base_eep_header.misc_configuration;

    if ( !((misc_config & 0x80) >> 7) )
        return false;
#endif
    if (is_2GHz)
    {
#ifndef ART_BUILD
        rf_gain_cap = (u_int32_t) eep->modal_header_2g.rf_gain_cap;
#else
        if (!AR_SREV_JET(ah))
            rf_gain_cap = (u_int32_t) eep->modal_header_2g.rf_gain_cap;
        else
            rf_gain_cap = (u_int32_t) eep_jet->modal_header_2g.rf_gain_cap;
#endif
    }
    else
    {
#ifndef ART_BUILD
        rf_gain_cap = (u_int32_t) eep->modal_header_5g.rf_gain_cap;
#else
        if (!AR_SREV_JET(ah))
            rf_gain_cap = (u_int32_t) eep->modal_header_5g.rf_gain_cap;
        else
            rf_gain_cap = (u_int32_t) eep_jet->modal_header_5g.rf_gain_cap;
#endif
    }

	if (rf_gain_cap == 0)
        return false;

	for (i = 0; i< RX_GAIN_TABLE_LENGTH * 2; i++)
	{
        if (AR_SREV_AR9580(ah)) 
        {
            // BB_rx_ocgain2
            i_rx_gain = 128 + 32;
            switch (ar9300_rx_gain_index_get(ah))
            {
            case 0:
                rx_gain_table[i][0] = 
					ar9300_common_rx_gain_table_ar9580_1p0[i][0];
                rx_gain_table[i][1] = 
					ar9300_common_rx_gain_table_ar9580_1p0[i][1];
                break;
            case 1:
                rx_gain_table[i][0] = 
					ar9300_common_wo_xlna_rx_gain_table_ar9580_1p0[i][0];
                rx_gain_table[i][1] = 
					ar9300_common_wo_xlna_rx_gain_table_ar9580_1p0[i][1];
                break;
			}
        } 
        else if (AR_SREV_OSPREY_22(ah)) 
        { 
            i_rx_gain = 128 + 32;
            switch (ar9300_rx_gain_index_get(ah))
            {
            case 0:
                rx_gain_table[i][0] = ar9300_common_rx_gain_table_osprey_2p2[i][0];
                rx_gain_table[i][1] = ar9300_common_rx_gain_table_osprey_2p2[i][1];
                break;
            case 1:
                rx_gain_table[i][0] = 
					ar9300Common_wo_xlna_rx_gain_table_osprey_2p2[i][0];
                rx_gain_table[i][1] = 
					ar9300Common_wo_xlna_rx_gain_table_osprey_2p2[i][1];
                break;
			}
        }
        else
        {
            return false;
        }
    }
    
    while (1) 
	{
        rx_gain_value = rx_gain_table[i_rx_gain][1];
        rx_gain_value_caped = rx_gain_value;
        a_Byte = rx_gain_value & (0x000000FF);
        if (a_Byte>rf_gain_cap) 
        {
        	rx_gain_value_caped = (rx_gain_value_caped & 
				(0xFFFFFF00)) + rf_gain_cap;
        }
        a_Byte = rx_gain_value & (0x0000FF00);
        if ( a_Byte > ( rf_gain_cap << 8 ) ) 
        {
        	rx_gain_value_caped = (rx_gain_value_caped & 
				(0xFFFF00FF)) + (rf_gain_cap<<8);
        }
        a_Byte = rx_gain_value & (0x00FF0000);
        if ( a_Byte > ( rf_gain_cap << 16 ) ) 
        {
        	rx_gain_value_caped = (rx_gain_value_caped & 
				(0xFF00FFFF)) + (rf_gain_cap<<16);
        }
        a_Byte = rx_gain_value & (0xFF000000);
        if ( a_Byte > ( rf_gain_cap << 24 ) ) 
        {
        	rx_gain_value_caped = (rx_gain_value_caped & 
				(0x00FFFFFF)) + (rf_gain_cap<<24);
        } 
        else 
        {
            done = 1;
        }
		HDPRINTF(ah, HAL_DBG_RESET,
			"%s: rx_gain_address: %x, rx_gain_value: %x	rx_gain_value_caped: %x\n",
			__func__, rx_gain_table[i_rx_gain][0], rx_gain_value, rx_gain_value_caped);
        if (rx_gain_value_caped != rx_gain_value)
		{
            rx_gain_table[i_rx_gain][1] = rx_gain_value_caped;
		}
        if (done == 1)
            break;
        i_rx_gain ++;
	}
    INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain, rx_gain_table, ARRAY_LENGTH(rx_gain_table), 2);
    return true;
}


void ar9300_rx_gain_table_apply(struct ath_hal *ah)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    struct ath_hal_private *ahpriv = AH_PRIVATE(ah);
    u_int32_t xlan_gpio_cfg;
    u_int8_t  i;

    if (AR_SREV_OSPREY(ah) || AR_SREV_AR9580(ah))
    {
		// this will be called if rxGainTable setting is changed
        if (ar9300_rf_gain_cap_apply(ah, 1))
            return;
	}

    switch (ar9300_rx_gain_index_get(ah))
    {
    case 2:
        if (AR_SREV_JUPITER_10(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain, 
                ar9300_common_mixed_rx_gain_table_jupiter_1p0,
                ARRAY_LENGTH(ar9300_common_mixed_rx_gain_table_jupiter_1p0), 2);
            break;
        }        
        else if (AR_SREV_JUPITER_20(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain, 
                ar9300Common_mixed_rx_gain_table_jupiter_2p0,
                ARRAY_LENGTH(ar9300Common_mixed_rx_gain_table_jupiter_2p0), 2);
            break;
        }
    case 0:
    default:
        if (AR_SREV_HORNET_12(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                ar9331_common_rx_gain_hornet1_2, 
                ARRAY_LENGTH(ar9331_common_rx_gain_hornet1_2), 2);
        } else if (AR_SREV_HORNET_11(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                ar9331_common_rx_gain_hornet1_1, 
                ARRAY_LENGTH(ar9331_common_rx_gain_hornet1_1), 2);
        } else if (AR_SREV_POSEIDON_11_OR_LATER(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                ar9485_common_wo_xlna_rx_gain_poseidon1_1,
                ARRAY_LENGTH(ar9485_common_wo_xlna_rx_gain_poseidon1_1), 2);
            xlan_gpio_cfg = ahpriv->ah_config.ath_hal_ext_lna_ctl_gpio;
            if (xlan_gpio_cfg) {
                for (i = 0; i < 32; i++) {
                    if (xlan_gpio_cfg & (1 << i)) {
                        ath_hal_gpio_cfg_output(ah, i, 
                            HAL_GPIO_OUTPUT_MUX_AS_PCIE_ATTENTION_LED);
                    }
                }
            }    

        } else if (AR_SREV_POSEIDON(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                ar9485Common_wo_xlna_rx_gain_poseidon1_0,
                ARRAY_LENGTH(ar9485Common_wo_xlna_rx_gain_poseidon1_0), 2);
        } else if (AR_SREV_JUPITER_10(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain, 
                ar9300_common_rx_gain_table_jupiter_1p0,
                ARRAY_LENGTH(ar9300_common_rx_gain_table_jupiter_1p0), 2);
        } else if (AR_SREV_JUPITER_20(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain, 
                ar9300Common_rx_gain_table_jupiter_2p0,
                ARRAY_LENGTH(ar9300Common_rx_gain_table_jupiter_2p0), 2);
        } else if (AR_SREV_AR9580(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                ar9300_common_rx_gain_table_ar9580_1p0,
                ARRAY_LENGTH(ar9300_common_rx_gain_table_ar9580_1p0), 2);
        } else if (AR_SREV_WASP(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                ar9340Common_rx_gain_table_wasp_1p0,
                ARRAY_LENGTH(ar9340Common_rx_gain_table_wasp_1p0), 2);
        } else if (AR_SREV_SCORPION(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                ar955xCommon_rx_gain_table_scorpion_1p0,
                ARRAY_LENGTH(ar955xCommon_rx_gain_table_scorpion_1p0), 2);
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain_bounds,
                ar955xCommon_rx_gain_bounds_scorpion_1p0,
                ARRAY_LENGTH(ar955xCommon_rx_gain_bounds_scorpion_1p0), 5);
        } else if (AR_SREV_HONEYBEE(ah)) {
            if (AR_SREV_HONEYBEE_20(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                qca953xCommon_w_rx_gain_table_honeybee_2p0,
                ARRAY_LENGTH(qca953xCommon_w_rx_gain_table_honeybee_2p0), 2);
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain_bounds,
                qca953xCommon_w_rx_gain_bounds_honeybee_2p0,
                ARRAY_LENGTH(qca953xCommon_w_rx_gain_bounds_honeybee_2p0), 5);
            } else {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                qca953xCommon_rx_gain_table_honeybee_1p0,
                ARRAY_LENGTH(qca953xCommon_rx_gain_table_honeybee_1p0), 2);
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain_bounds,
                qca953xCommon_rx_gain_bounds_honeybee_1p0,
                ARRAY_LENGTH(qca953xCommon_rx_gain_bounds_honeybee_1p0), 5);
            }
        } else if (AR_SREV_DRAGONFLY(ah)) {
#ifndef AR9560_EMULATION
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                    qca956xCommon_rx_gain_table_dragonfly_1p0,
                    ARRAY_LENGTH(qca956xCommon_rx_gain_table_dragonfly_1p0), 2);
#else
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                qca956xCommon_rx_gain_table_merlin_1p0,
                ARRAY_LENGTH(qca956xCommon_rx_gain_table_merlin_1p0), 2);
#endif
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain_bounds,
                    qca956xCommon_rx_gain_bounds_dragonfly_1p0,
                    ARRAY_LENGTH(qca956xCommon_rx_gain_bounds_dragonfly_1p0), 5);
            INIT_INI_ARRAY(&ahp->ah_ini_xlna,
                    qca956x_dragonfly_1p0_xlna_only,
                    ARRAY_LENGTH(qca956x_dragonfly_1p0_xlna_only), 5);
        } else if (AR_SREV_JET(ah)) {
#if !defined(AR5500_EMULATION)
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                    qcn5500Common_rx_gain_table_jet_1p0,
                    ARRAY_LENGTH(qcn5500Common_rx_gain_table_jet_1p0), 2);
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain_bounds,
                    qcn5500Common_rx_gain_bounds_jet_1p0,
                    ARRAY_LENGTH(qcn5500Common_rx_gain_bounds_jet_1p0), 5);
            INIT_INI_ARRAY(&ahp->ah_ini_xlna,
                    qcn5500_jet_1p0_xlna_only,
                    ARRAY_LENGTH(qcn5500_jet_1p0_xlna_only), 5);

#elif !defined(QCN5500_M2M)
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                    qcn5500Common_rx_gain_table_merlin_1p0,
                    ARRAY_LENGTH(qcn5500Common_rx_gain_table_merlin_1p0), 2);
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain_bounds,
                    qcn5500Common_rx_gain_bounds_jet_1p0,
                    ARRAY_LENGTH(qcn5500Common_rx_gain_bounds_jet_1p0), 5);
            INIT_INI_ARRAY(&ahp->ah_ini_xlna,
                    qcn5500_jet_1p0_xlna_only,
                    ARRAY_LENGTH(qcn5500_jet_1p0_xlna_only), 5);
#endif
        } else {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                ar9300_common_rx_gain_table_osprey_2p2,
                ARRAY_LENGTH(ar9300_common_rx_gain_table_osprey_2p2), 2);
        }
        break;
    case 1:
        if (AR_SREV_HORNET_12(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                ar9331_common_wo_xlna_rx_gain_hornet1_2,
                ARRAY_LENGTH(ar9331_common_wo_xlna_rx_gain_hornet1_2), 2);
        } else if (AR_SREV_HORNET_11(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                ar9331_common_wo_xlna_rx_gain_hornet1_1,
                ARRAY_LENGTH(ar9331_common_wo_xlna_rx_gain_hornet1_1), 2);
        } else if (AR_SREV_POSEIDON_11_OR_LATER(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                ar9485_common_wo_xlna_rx_gain_poseidon1_1,
                ARRAY_LENGTH(ar9485_common_wo_xlna_rx_gain_poseidon1_1), 2);
        } else if (AR_SREV_POSEIDON(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                ar9485Common_wo_xlna_rx_gain_poseidon1_0,
                ARRAY_LENGTH(ar9485Common_wo_xlna_rx_gain_poseidon1_0), 2);
        } else if (AR_SREV_JUPITER_10(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain, 
                ar9300_common_wo_xlna_rx_gain_table_jupiter_1p0,
                ARRAY_LENGTH(ar9300_common_wo_xlna_rx_gain_table_jupiter_1p0),
                2);
        } else if (AR_SREV_JUPITER_20(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain, 
                ar9300Common_wo_xlna_rx_gain_table_jupiter_2p0,
                ARRAY_LENGTH(ar9300Common_wo_xlna_rx_gain_table_jupiter_2p0),
                2);
        } else if (AR_SREV_APHRODITE(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain, 
                ar956XCommon_wo_xlna_rx_gain_table_aphrodite_1p0,
                ARRAY_LENGTH(ar956XCommon_wo_xlna_rx_gain_table_aphrodite_1p0),
                2);
        } else if (AR_SREV_AR9580(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                ar9300_common_wo_xlna_rx_gain_table_ar9580_1p0,
                ARRAY_LENGTH(ar9300_common_wo_xlna_rx_gain_table_ar9580_1p0), 2);
        } else if (AR_SREV_WASP(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                ar9340Common_wo_xlna_rx_gain_table_wasp_1p0,
                ARRAY_LENGTH(ar9340Common_wo_xlna_rx_gain_table_wasp_1p0), 2);
        } else if (AR_SREV_SCORPION(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                ar955xCommon_wo_xlna_rx_gain_table_scorpion_1p0,
                ARRAY_LENGTH(ar955xCommon_wo_xlna_rx_gain_table_scorpion_1p0), 2);
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain_bounds,
                ar955xCommon_wo_xlna_rx_gain_bounds_scorpion_1p0,
                ARRAY_LENGTH(ar955xCommon_wo_xlna_rx_gain_bounds_scorpion_1p0), 5);
        } else if (AR_SREV_HONEYBEE(ah)) {
            if(AR_SREV_HONEYBEE_20(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                qca953xCommon_wo_xlna_rx_gain_table_honeybee_2p0,
                ARRAY_LENGTH(qca953xCommon_wo_xlna_rx_gain_table_honeybee_2p0), 2);
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain_bounds,
                qca953xCommon_wo_xlna_rx_gain_bounds_honeybee_2p0,
                ARRAY_LENGTH(qca953xCommon_wo_xlna_rx_gain_bounds_honeybee_2p0), 5);
            } else {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                qca953xCommon_wo_xlna_rx_gain_table_honeybee_1p0,
                ARRAY_LENGTH(qca953xCommon_wo_xlna_rx_gain_table_honeybee_1p0), 2);
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain_bounds,
                qca953xCommon_wo_xlna_rx_gain_bounds_honeybee_1p0,
                ARRAY_LENGTH(qca953xCommon_wo_xlna_rx_gain_bounds_honeybee_1p0), 5);
			}
        } else if (AR_SREV_DRAGONFLY(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                qca956xCommon_wo_xlna_rx_gain_table_dragonfly_1p0,
                ARRAY_LENGTH(qca956xCommon_wo_xlna_rx_gain_table_dragonfly_1p0), 2);
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain_bounds,
                qca956xCommon_wo_xlna_rx_gain_bounds_dragonfly_1p0,
                ARRAY_LENGTH(qca956xCommon_wo_xlna_rx_gain_bounds_dragonfly_1p0), 5);
        } else if (AR_SREV_JET(ah)) {
#if !defined(AR5500_EMULATION)
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                    qcn5500Common_wo_xlna_rx_gain_table_jet_1p0,
                    ARRAY_LENGTH(qcn5500Common_wo_xlna_rx_gain_table_jet_1p0), 2);
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain_bounds,
                    qcn5500Common_wo_xlna_rx_gain_bounds_jet_1p0,
                    ARRAY_LENGTH(qcn5500Common_wo_xlna_rx_gain_bounds_jet_1p0), 5);
#endif
        } else {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_rxgain,
                ar9300Common_wo_xlna_rx_gain_table_osprey_2p2,
                ARRAY_LENGTH(ar9300Common_wo_xlna_rx_gain_table_osprey_2p2), 2);
        }
        break;
    }
}

void ar9300_tx_gain_table_apply(struct ath_hal *ah)
{
    struct ath_hal_9300 *ahp = AH9300(ah);

    switch (ar9300_tx_gain_index_get(ah))
    {
    case 0:
    default:
        if (AR_SREV_HORNET_12(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9331_modes_lowest_ob_db_tx_gain_hornet1_2, 
                ARRAY_LENGTH(ar9331_modes_lowest_ob_db_tx_gain_hornet1_2), 5);
        } else if (AR_SREV_HORNET_11(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9331_modes_lowest_ob_db_tx_gain_hornet1_1, 
                ARRAY_LENGTH(ar9331_modes_lowest_ob_db_tx_gain_hornet1_1), 5);
        } else if (AR_SREV_POSEIDON_11_OR_LATER(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9485_modes_lowest_ob_db_tx_gain_poseidon1_1, 
                ARRAY_LENGTH(ar9485_modes_lowest_ob_db_tx_gain_poseidon1_1), 5);
        } else if (AR_SREV_POSEIDON(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9485Modes_lowest_ob_db_tx_gain_poseidon1_0, 
                ARRAY_LENGTH(ar9485Modes_lowest_ob_db_tx_gain_poseidon1_0), 5);
        } else if (AR_SREV_AR9580(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9300Modes_lowest_ob_db_tx_gain_table_ar9580_1p0,
                ARRAY_LENGTH(ar9300Modes_lowest_ob_db_tx_gain_table_ar9580_1p0),
                5);
        } else if (AR_SREV_WASP(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9340Modes_lowest_ob_db_tx_gain_table_wasp_1p0,
                ARRAY_LENGTH(ar9340Modes_lowest_ob_db_tx_gain_table_wasp_1p0),
                5);
        } else if (AR_SREV_SCORPION(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar955xModes_xpa_tx_gain_table_scorpion_1p0,
                ARRAY_LENGTH(ar955xModes_xpa_tx_gain_table_scorpion_1p0),
                9);
        } else if (AR_SREV_JUPITER_10(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9300_modes_low_ob_db_tx_gain_table_jupiter_1p0,
                ARRAY_LENGTH(ar9300_modes_low_ob_db_tx_gain_table_jupiter_1p0),
                5);
        } else if (AR_SREV_JUPITER_20(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9300Modes_low_ob_db_tx_gain_table_jupiter_2p0,
                ARRAY_LENGTH(ar9300Modes_low_ob_db_tx_gain_table_jupiter_2p0),
                5);
        } else if (AR_SREV_HONEYBEE(ah)) {
            if(AR_SREV_HONEYBEE_20(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                qca953xModes_w_xpa_tx_gain_table_honeybee_2p0,
                ARRAY_LENGTH(qca953xModes_w_xpa_tx_gain_table_honeybee_2p0),
                2);
        } else if(AR_SREV_HONEYBEE_11(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                qca953xModes_w_xpa_tx_gain_table_honeybee_1p1,
                ARRAY_LENGTH(qca953xModes_w_xpa_tx_gain_table_honeybee_1p1),
                2);
          } else {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                qca953xModes_xpa_tx_gain_table_honeybee_1p0,
                ARRAY_LENGTH(qca953xModes_xpa_tx_gain_table_honeybee_1p0),
               2);
            }
        } else if (AR_SREV_APHRODITE(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar956XModes_low_ob_db_tx_gain_table_aphrodite_1p0,
                ARRAY_LENGTH(ar956XModes_low_ob_db_tx_gain_table_aphrodite_1p0),
                5);
        } else if (AR_SREV_DRAGONFLY(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                qca956xModes_xpa_tx_gain_table_dragonfly_1p0,
                ARRAY_LENGTH(qca956xModes_xpa_tx_gain_table_dragonfly_1p0),
                3);
        } else if (AR_SREV_JET(ah)) {
#if !defined(AR5500_EMULATION)
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                    qcn5500Modes_xpa_tx_gain_table_jet_1p0,
                    ARRAY_LENGTH(qcn5500Modes_xpa_tx_gain_table_jet_1p0),
                3);
#endif
        } else {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9300_modes_lowest_ob_db_tx_gain_table_osprey_2p2,
                ARRAY_LENGTH(ar9300_modes_lowest_ob_db_tx_gain_table_osprey_2p2),
                5);
        }
        break;
    case 1:
        if (AR_SREV_HORNET_12(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9331_modes_high_ob_db_tx_gain_hornet1_2, 
                ARRAY_LENGTH(ar9331_modes_high_ob_db_tx_gain_hornet1_2), 5);
        } else if (AR_SREV_HORNET_11(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9331_modes_high_ob_db_tx_gain_hornet1_1, 
                ARRAY_LENGTH(ar9331_modes_high_ob_db_tx_gain_hornet1_1), 5);
        } else if (AR_SREV_POSEIDON_11_OR_LATER(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9485_modes_high_ob_db_tx_gain_poseidon1_1, 
                ARRAY_LENGTH(ar9485_modes_high_ob_db_tx_gain_poseidon1_1), 5);
        } else if (AR_SREV_POSEIDON(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9485Modes_high_ob_db_tx_gain_poseidon1_0, 
                ARRAY_LENGTH(ar9485Modes_high_ob_db_tx_gain_poseidon1_0), 5);
        } else if (AR_SREV_AR9580(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9300Modes_high_ob_db_tx_gain_table_ar9580_1p0,
                ARRAY_LENGTH(ar9300Modes_high_ob_db_tx_gain_table_ar9580_1p0),
                5);
        } else if (AR_SREV_WASP(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9340Modes_high_ob_db_tx_gain_table_wasp_1p0,
                ARRAY_LENGTH(ar9340Modes_high_ob_db_tx_gain_table_wasp_1p0), 5);
        } else if (AR_SREV_SCORPION(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar955xModes_no_xpa_tx_gain_table_scorpion_1p0,
                ARRAY_LENGTH(ar955xModes_no_xpa_tx_gain_table_scorpion_1p0), 9);
        } else if (AR_SREV_JUPITER_10(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9300_modes_high_ob_db_tx_gain_table_jupiter_1p0,
                ARRAY_LENGTH(
                ar9300_modes_high_ob_db_tx_gain_table_jupiter_1p0), 5);
        } else if (AR_SREV_JUPITER_20(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9300Modes_high_ob_db_tx_gain_table_jupiter_2p0,
                ARRAY_LENGTH(
                ar9300Modes_high_ob_db_tx_gain_table_jupiter_2p0), 5);
        } else if (AR_SREV_APHRODITE(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
	        ar956XModes_high_ob_db_tx_gain_table_aphrodite_1p0,
                ARRAY_LENGTH(
                ar956XModes_high_ob_db_tx_gain_table_aphrodite_1p0), 5);
        } else if (AR_SREV_HONEYBEE(ah)) {
            if (AR_SREV_HONEYBEE_11(ah)) {
                INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                    qca953xModes_no_xpa_tx_gain_table_honeybee_1p1,
                    ARRAY_LENGTH(qca953xModes_no_xpa_tx_gain_table_honeybee_1p1), 2);
        } else if (AR_SREV_HONEYBEE_20(ah)) {
                INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                    qca953xModes_no_xpa_tx_gain_table_honeybee_2p0,
                    ARRAY_LENGTH(qca953xModes_no_xpa_tx_gain_table_honeybee_2p0), 2);
            } else {
                INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                    qca953xModes_no_xpa_tx_gain_table_honeybee_1p0,
                    ARRAY_LENGTH(qca953xModes_no_xpa_tx_gain_table_honeybee_1p0), 2);
           }
        } else if (AR_SREV_DRAGONFLY(ah)) {
	        INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                qca956xModes_no_xpa_tx_gain_table_dragonfly_1p0,
                ARRAY_LENGTH(qca956xModes_no_xpa_tx_gain_table_dragonfly_1p0), 3);
        } else if (AR_SREV_JET(ah)) {
#if !defined(AR5500_EMULATION)
	        INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                    qcn5500Modes_no_xpa_tx_gain_table_jet_1p0,
                    ARRAY_LENGTH(qcn5500Modes_no_xpa_tx_gain_table_jet_1p0), 3);
#endif
        } else {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9300Modes_high_ob_db_tx_gain_table_osprey_2p2,
                ARRAY_LENGTH(ar9300Modes_high_ob_db_tx_gain_table_osprey_2p2),
                5);
        }
        break;
    case 2:
        if (AR_SREV_HORNET_12(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9331_modes_low_ob_db_tx_gain_hornet1_2, 
                ARRAY_LENGTH(ar9331_modes_low_ob_db_tx_gain_hornet1_2), 5);
        } else if (AR_SREV_HORNET_11(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9331_modes_low_ob_db_tx_gain_hornet1_1, 
                ARRAY_LENGTH(ar9331_modes_low_ob_db_tx_gain_hornet1_1), 5);
        } else if (AR_SREV_POSEIDON_11_OR_LATER(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9485_modes_low_ob_db_tx_gain_poseidon1_1, 
                ARRAY_LENGTH(ar9485_modes_low_ob_db_tx_gain_poseidon1_1), 5);
        } else if (AR_SREV_POSEIDON(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9485Modes_low_ob_db_tx_gain_poseidon1_0, 
                ARRAY_LENGTH(ar9485Modes_low_ob_db_tx_gain_poseidon1_0), 5);
        } else if (AR_SREV_AR9580(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9300Modes_low_ob_db_tx_gain_table_ar9580_1p0,
                ARRAY_LENGTH(ar9300Modes_low_ob_db_tx_gain_table_ar9580_1p0),
                5);
        } else if (AR_SREV_WASP(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9340Modes_low_ob_db_tx_gain_table_wasp_1p0,
                ARRAY_LENGTH(ar9340Modes_low_ob_db_tx_gain_table_wasp_1p0), 5);
        } else if (AR_SREV_APHRODITE(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar956XModes_low_ob_db_tx_gain_table_aphrodite_1p0, 
                ARRAY_LENGTH(ar956XModes_low_ob_db_tx_gain_table_aphrodite_1p0), 5);
		} else if (AR_SREV_DRAGONFLY(ah)) {
			INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                qca956xModes_no_xpa_low_ob_db_tx_gain_table_dragonfly_1p0,
                ARRAY_LENGTH(qca956xModes_no_xpa_low_ob_db_tx_gain_table_dragonfly_1p0), 3);
		} else if (AR_SREV_JET(ah)) {
#ifndef AR5500_EMULATION
			INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                    qcn5500Modes_no_xpa_low_ob_db_tx_gain_table_jet_1p0,
                    ARRAY_LENGTH(qcn5500Modes_no_xpa_low_ob_db_tx_gain_table_jet_1p0), 3);
#endif
        } else {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9300Modes_low_ob_db_tx_gain_table_osprey_2p2,
                ARRAY_LENGTH(ar9300Modes_low_ob_db_tx_gain_table_osprey_2p2),
                5);
        }
        break;
    case 3:
        if (AR_SREV_HORNET_12(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9331_modes_high_power_tx_gain_hornet1_2, 
                ARRAY_LENGTH(ar9331_modes_high_power_tx_gain_hornet1_2), 5);
        } else if (AR_SREV_HORNET_11(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9331_modes_high_power_tx_gain_hornet1_1, 
                ARRAY_LENGTH(ar9331_modes_high_power_tx_gain_hornet1_1), 5);
        } else if (AR_SREV_POSEIDON_11_OR_LATER(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9485_modes_high_power_tx_gain_poseidon1_1, 
                ARRAY_LENGTH(ar9485_modes_high_power_tx_gain_poseidon1_1), 5);
        } else if (AR_SREV_POSEIDON(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9485Modes_high_power_tx_gain_poseidon1_0, 
                ARRAY_LENGTH(ar9485Modes_high_power_tx_gain_poseidon1_0), 5);
        } else if (AR_SREV_AR9580(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9300Modes_high_power_tx_gain_table_ar9580_1p0,
                ARRAY_LENGTH(ar9300Modes_high_power_tx_gain_table_ar9580_1p0),
                5);
        } else if (AR_SREV_WASP(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9340Modes_high_power_tx_gain_table_wasp_1p0,
                ARRAY_LENGTH(ar9340Modes_high_power_tx_gain_table_wasp_1p0),
                5);
        } else if (AR_SREV_APHRODITE(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar956XModes_high_power_tx_gain_table_aphrodite_1p0, 
                ARRAY_LENGTH(ar956XModes_high_power_tx_gain_table_aphrodite_1p0), 5);
        } else if (AR_SREV_JET(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                    qcn5500Modes_no_xpa_tx_gain_table_mcaltx_gain_jet_1p0,
                    ARRAY_LENGTH(qcn5500Modes_no_xpa_tx_gain_table_mcaltx_gain_jet_1p0), 3);
        } else {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9300Modes_high_power_tx_gain_table_osprey_2p2,
                ARRAY_LENGTH(ar9300Modes_high_power_tx_gain_table_osprey_2p2),
                5);
        }
        break;
    case 4:
        if (AR_SREV_WASP(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9340Modes_mixed_ob_db_tx_gain_table_wasp_1p0,
                ARRAY_LENGTH(ar9340Modes_mixed_ob_db_tx_gain_table_wasp_1p0),
                5);
        } else if (AR_SREV_AR9580(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9300_modes_mixed_ob_db_tx_gain_table_ar9580_1p0,
                ARRAY_LENGTH(ar9300_modes_mixed_ob_db_tx_gain_table_ar9580_1p0),
                5);
        } else if (AR_SREV_JET(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                    qcn5500Modes_no_xpa_tx_gain_table_5_jet_1p0,
                    ARRAY_LENGTH(qcn5500Modes_no_xpa_tx_gain_table_5_jet_1p0), 3);
        } else {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain, 
		ar9300Modes_mixed_ob_db_tx_gain_table_osprey_2p2,
                ARRAY_LENGTH(ar9300Modes_mixed_ob_db_tx_gain_table_osprey_2p2),
		 5);
        }
        break;
    case 5:
        /* HW Green TX */
        if (AR_SREV_POSEIDON(ah)) {
            if (AR_SREV_POSEIDON_11_OR_LATER(ah)) {
                INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain, 
                    ar9485_modes_green_ob_db_tx_gain_poseidon1_1,
                    sizeof(ar9485_modes_green_ob_db_tx_gain_poseidon1_1) /
                    sizeof(ar9485_modes_green_ob_db_tx_gain_poseidon1_1[0]), 5);
            } else {
                INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain, 
                    ar9485_modes_green_ob_db_tx_gain_poseidon1_0,
                    sizeof(ar9485_modes_green_ob_db_tx_gain_poseidon1_0) /
                    sizeof(ar9485_modes_green_ob_db_tx_gain_poseidon1_0[0]), 5);
            }
            ahp->ah_hw_green_tx_enable = 1;
        }
        else if (AR_SREV_WASP(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain, 
            ar9340_modes_ub124_tx_gain_table_wasp_1p0,
            sizeof(ar9340_modes_ub124_tx_gain_table_wasp_1p0) /
            sizeof(ar9340_modes_ub124_tx_gain_table_wasp_1p0[0]), 5);
        }
        else if (AR_SREV_AR9580(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9300_modes_type5_tx_gain_table_ar9580_1p0,
                ARRAY_LENGTH( ar9300_modes_type5_tx_gain_table_ar9580_1p0),
                5);
		} 
		else if (AR_SREV_DRAGONFLY(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
				qca956xModes_no_xpa_Green_tx_gain_table_dragonfly_1p0,
				ARRAY_LENGTH(qca956xModes_no_xpa_Green_tx_gain_table_dragonfly_1p0), 3);
        } else if (AR_SREV_JET(ah)) {
#ifndef AR5500_EMULATION
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                    qcn5500Modes_no_xpa_Green_tx_gain_table_jet_1p0,
                    ARRAY_LENGTH(qcn5500Modes_no_xpa_Green_tx_gain_table_jet_1p0), 3);
#endif
		}
        else if (AR_SREV_OSPREY_22(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9300_modes_number_5_tx_gain_table_osprey_2p2,
                ARRAY_LENGTH( ar9300_modes_number_5_tx_gain_table_osprey_2p2),
                5);
        }
        break;
	case 6:
        if (AR_SREV_WASP(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
            ar9340_modes_low_ob_db_and_spur_tx_gain_table_wasp_1p0,
            sizeof(ar9340_modes_low_ob_db_and_spur_tx_gain_table_wasp_1p0) /
            sizeof(ar9340_modes_low_ob_db_and_spur_tx_gain_table_wasp_1p0[0]), 5);
        }
        /* HW Green TX */
        else if (AR_SREV_POSEIDON(ah)) {
            if (AR_SREV_POSEIDON_11_OR_LATER(ah)) {
                INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain, 
                ar9485_modes_green_spur_ob_db_tx_gain_poseidon1_1,
                sizeof(ar9485_modes_green_spur_ob_db_tx_gain_poseidon1_1) /
                sizeof(ar9485_modes_green_spur_ob_db_tx_gain_poseidon1_1[0]),
                5);
            }
            ahp->ah_hw_green_tx_enable = 1;
	}
        else if (AR_SREV_AR9580(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain,
                ar9300_modes_type6_tx_gain_table_ar9580_1p0,
                ARRAY_LENGTH( ar9300_modes_type6_tx_gain_table_ar9580_1p0),
                5);
        }
        break;
	case 7:
		if (AR_SREV_WASP(ah)) {
            INIT_INI_ARRAY(&ahp->ah_ini_modes_txgain, 
            ar9340Modes_cus227_tx_gain_table_wasp_1p0,
            sizeof(ar9340Modes_cus227_tx_gain_table_wasp_1p0) /
            sizeof(ar9340Modes_cus227_tx_gain_table_wasp_1p0[0]), 5);
		}
		break;
    }
}

#if ATH_ANT_DIV_COMB
void
ar9300_ant_div_comb_get_config(struct ath_hal *ah,
    HAL_ANT_COMB_CONFIG *div_comb_conf)
{
    u_int32_t reg_val = OS_REG_READ(ah, AR_PHY_MC_GAIN_CTRL);
    div_comb_conf->main_lna_conf = 
        MULTICHAIN_GAIN_CTRL__ANT_DIV_MAIN_LNACONF__READ(reg_val);
    div_comb_conf->alt_lna_conf = 
        MULTICHAIN_GAIN_CTRL__ANT_DIV_ALT_LNACONF__READ(reg_val);
    div_comb_conf->fast_div_bias = 
        MULTICHAIN_GAIN_CTRL__ANT_FAST_DIV_BIAS__READ(reg_val); 
    if (AR_SREV_HORNET_11(ah)) {
        div_comb_conf->antdiv_configgroup = HAL_ANTDIV_CONFIG_GROUP_1;
    } else if (AR_SREV_POSEIDON_11_OR_LATER(ah)) {
        div_comb_conf->antdiv_configgroup = HAL_ANTDIV_CONFIG_GROUP_2;
    } else {
        div_comb_conf->antdiv_configgroup = DEFAULT_ANTDIV_CONFIG_GROUP;
    }
}

void
ar9300_ant_div_comb_set_config(struct ath_hal *ah,
    HAL_ANT_COMB_CONFIG *div_comb_conf)
{
    u_int32_t reg_val;
    struct ath_hal_9300 *ahp = AH9300(ah);

    /* DO NOTHING when set to fixed antenna for manufacturing purpose */
    if (AR_SREV_POSEIDON(ah) && ( ahp->ah_diversity_control == HAL_ANT_FIXED_A 
         || ahp->ah_diversity_control == HAL_ANT_FIXED_B)) {
        return;
    }
    reg_val = OS_REG_READ(ah, AR_PHY_MC_GAIN_CTRL);
    reg_val &= ~(MULTICHAIN_GAIN_CTRL__ANT_DIV_MAIN_LNACONF__MASK    | 
                MULTICHAIN_GAIN_CTRL__ANT_DIV_ALT_LNACONF__MASK     |
                MULTICHAIN_GAIN_CTRL__ANT_FAST_DIV_BIAS__MASK       |
                MULTICHAIN_GAIN_CTRL__ANT_DIV_MAIN_GAINTB__MASK     |
                MULTICHAIN_GAIN_CTRL__ANT_DIV_ALT_GAINTB__MASK );
    reg_val |=
        MULTICHAIN_GAIN_CTRL__ANT_DIV_MAIN_GAINTB__WRITE(
        div_comb_conf->main_gaintb);
    reg_val |=
        MULTICHAIN_GAIN_CTRL__ANT_DIV_ALT_GAINTB__WRITE(
        div_comb_conf->alt_gaintb);
    reg_val |= 
        MULTICHAIN_GAIN_CTRL__ANT_DIV_MAIN_LNACONF__WRITE(
        div_comb_conf->main_lna_conf);
    reg_val |= 
        MULTICHAIN_GAIN_CTRL__ANT_DIV_ALT_LNACONF__WRITE(
        div_comb_conf->alt_lna_conf);
    reg_val |= 
        MULTICHAIN_GAIN_CTRL__ANT_FAST_DIV_BIAS__WRITE(
        div_comb_conf->fast_div_bias);
    OS_REG_WRITE(ah, AR_PHY_MC_GAIN_CTRL, reg_val);

}
#endif /* ATH_ANT_DIV_COMB */
static void 
ar9300_init_hostif_offsets(struct ath_hal *ah)
{
    AR_HOSTIF_REG(ah, AR_RC) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_RESET_CONTROL);
    if (!AR_SREV_JET(ah)){
        AR_HOSTIF_REG(ah, AR_WA) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_WORK_AROUND);
        AR_HOSTIF_REG(ah, AR_PM_STATE) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_PM_STATE);
        AR_HOSTIF_REG(ah, AR_H_INFOL) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_CXPL_DEBUG_INFOL);
        AR_HOSTIF_REG(ah, AR_H_INFOH) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_CXPL_DEBUG_INFOH);
        AR_HOSTIF_REG(ah, AR_EEPROM) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_EEPROM_CTRL);
        AR_HOSTIF_REG(ah, AR_SREV) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_SREV);
        AR_HOSTIF_REG(ah, AR_PCIE_SERDES) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_PCIE_PHY_RW);
        AR_HOSTIF_REG(ah, AR_PCIE_SERDES2) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_PCIE_PHY_LOAD);
        AR_HOSTIF_REG(ah, AR_GPIO_OE_OUT) =
            AR9300_HOSTIF_OFFSET(overlay_0x4050.Osprey.HOST_INTF_GPIO_OE);
        AR_HOSTIF_REG(ah, AR_GPIO_OE1_OUT) =
            AR9300_HOSTIF_OFFSET(overlay_0x4050.Osprey.HOST_INTF_GPIO_OE1);
        AR_HOSTIF_REG(ah, AR_GPIO_INTR_POL) =
            AR9300_HOSTIF_OFFSET(overlay_0x4050.Osprey.HOST_INTF_GPIO_INTR_POLAR);
        AR_HOSTIF_REG(ah, AR_GPIO_INPUT_MUX1) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_GPIO_INPUT_MUX1);
        AR_HOSTIF_REG(ah, AR_GPIO_INPUT_MUX2) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_GPIO_INPUT_MUX2);
        AR_HOSTIF_REG(ah, AR_GPIO_OUTPUT_MUX1) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_GPIO_OUTPUT_MUX1);
        AR_HOSTIF_REG(ah, AR_GPIO_OUTPUT_MUX2) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_GPIO_OUTPUT_MUX2);
        AR_HOSTIF_REG(ah, AR_GPIO_OUTPUT_MUX3) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_GPIO_OUTPUT_MUX3);
        AR_HOSTIF_REG(ah, AR_SPARE) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_SPARE);
        AR_HOSTIF_REG(ah, AR_PCIE_CORE_RESET_EN) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_PCIE_CORE_RST_EN);
        AR_HOSTIF_REG(ah, AR_CLKRUN) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_CLKRUN);
        AR_HOSTIF_REG(ah, AR_EEPROM_STATUS_DATA) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_EEPROM_STS);
        AR_HOSTIF_REG(ah, AR_GPIO_PDPU) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_GPIO_PDPU);
        AR_HOSTIF_REG(ah, AR_GPIO_DS) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_GPIO_DS);
        AR_HOSTIF_REG(ah, AR_PCIE_MSI) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_PCIE_MSI);
        AR_HOSTIF_REG(ah, AR_PCIE_PHY_LATENCY_NFTS_ADJ) =
            AR9300_HOSTIF_OFFSET(HOST_INTF_PCIE_PHY_LATENCY_NFTS_ADJ);
    } else {
        AR_HOSTIF_REG(ah, QCN5500_SWCOM_GPIO_FUNC_ENABLE) =
            AR9300_HOSTIF_OFFSET(overlay_0x4050.Jet.HOST_INTF_SWCOM_GPIO_FUNC_ENABLE);
        AR_HOSTIF_REG(ah, QCN5500_AXI_BYTE) =
            AR9300_HOSTIF_OFFSET(overlay_0x40d8.Jet.HOST_INTF_AXI_BYTE_SWAP);
    }
    AR_HOSTIF_REG(ah, AR_PCIE_PM_CTRL) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_PM_CTRL);
    AR_HOSTIF_REG(ah, AR_HOST_TIMEOUT) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_TIMEOUT);
    AR_HOSTIF_REG(ah, AR_INTR_SYNC_CAUSE) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_INTR_SYNC_CAUSE);
    AR_HOSTIF_REG(ah, AR_INTR_SYNC_CAUSE_CLR) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_INTR_SYNC_CAUSE);
    AR_HOSTIF_REG(ah, AR_INTR_SYNC_ENABLE) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_INTR_SYNC_ENABLE);
    AR_HOSTIF_REG(ah, AR_INTR_ASYNC_MASK) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_INTR_ASYNC_MASK);
    AR_HOSTIF_REG(ah, AR_INTR_SYNC_MASK) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_INTR_SYNC_MASK);
    AR_HOSTIF_REG(ah, AR_INTR_ASYNC_CAUSE_CLR) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_INTR_ASYNC_CAUSE);
    AR_HOSTIF_REG(ah, AR_INTR_ASYNC_CAUSE) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_INTR_ASYNC_CAUSE);
    AR_HOSTIF_REG(ah, AR_INTR_ASYNC_ENABLE) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_INTR_ASYNC_ENABLE);
    AR_HOSTIF_REG(ah, AR_GPIO_OUT) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_GPIO_OUT);
    AR_HOSTIF_REG(ah, AR_GPIO_IN) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_GPIO_IN);
    AR_HOSTIF_REG(ah, AR_GPIO_INPUT_EN_VAL) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_GPIO_INPUT_VALUE);
    AR_HOSTIF_REG(ah, AR_INPUT_STATE) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_GPIO_INPUT_STATE);
    AR_HOSTIF_REG(ah, AR_OBS) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_OBS_CTRL);
    AR_HOSTIF_REG(ah, AR_RFSILENT) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_RFSILENT);
    AR_HOSTIF_REG(ah, AR_MISC) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_MISC);
#if 0   /* Offsets are not defined in reg_map structure */ 
    AR_HOSTIF_REG(ah, AR_TSF_SNAPSHOT_BT_ACTIVE) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_TSF_SNAPSHOT_BT_ACTIVE);
    AR_HOSTIF_REG(ah, AR_TSF_SNAPSHOT_BT_PRIORITY) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_TSF_SNAPSHOT_BT_PRIORITY);
    AR_HOSTIF_REG(ah, AR_TSF_SNAPSHOT_BT_CNTL) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_MAC_TSF_SNAPSHOT_BT_CNTL);
#endif
    AR_HOSTIF_REG(ah, AR_TDMA_CCA_CNTL) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_MAC_TDMA_CCA_CNTL);
    AR_HOSTIF_REG(ah, AR_TXAPSYNC) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_MAC_TXAPSYNC);
    AR_HOSTIF_REG(ah, AR_TXSYNC_INIT_SYNC_TMR) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_MAC_TXSYNC_INITIAL_SYNC_TMR);
    AR_HOSTIF_REG(ah, AR_INTR_PRIO_SYNC_CAUSE) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_INTR_PRIORITY_SYNC_CAUSE);
    AR_HOSTIF_REG(ah, AR_INTR_PRIO_SYNC_ENABLE) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_INTR_PRIORITY_SYNC_ENABLE);
    AR_HOSTIF_REG(ah, AR_INTR_PRIO_ASYNC_MASK) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_INTR_PRIORITY_ASYNC_MASK);
    AR_HOSTIF_REG(ah, AR_INTR_PRIO_SYNC_MASK) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_INTR_PRIORITY_SYNC_MASK);
    AR_HOSTIF_REG(ah, AR_INTR_PRIO_ASYNC_CAUSE) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_INTR_PRIORITY_ASYNC_CAUSE);
    AR_HOSTIF_REG(ah, AR_INTR_PRIO_ASYNC_ENABLE) =
        AR9300_HOSTIF_OFFSET(HOST_INTF_INTR_PRIORITY_ASYNC_ENABLE);
}

static void 
ar9340_init_hostif_offsets(struct ath_hal *ah)
{
    AR_HOSTIF_REG(ah, AR_RC) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_RESET_CONTROL);
    AR_HOSTIF_REG(ah, AR_WA) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_WORK_AROUND);                   
    AR_HOSTIF_REG(ah, AR_PCIE_PM_CTRL) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_PM_CTRL);
    AR_HOSTIF_REG(ah, AR_HOST_TIMEOUT) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_TIMEOUT);
    AR_HOSTIF_REG(ah, AR_SREV) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_SREV);
    AR_HOSTIF_REG(ah, AR_INTR_SYNC_CAUSE) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_INTR_SYNC_CAUSE);
    AR_HOSTIF_REG(ah, AR_INTR_SYNC_CAUSE_CLR) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_INTR_SYNC_CAUSE);
    AR_HOSTIF_REG(ah, AR_INTR_SYNC_ENABLE) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_INTR_SYNC_ENABLE);
    AR_HOSTIF_REG(ah, AR_INTR_ASYNC_MASK) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_INTR_ASYNC_MASK);
    AR_HOSTIF_REG(ah, AR_INTR_SYNC_MASK) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_INTR_SYNC_MASK);
    AR_HOSTIF_REG(ah, AR_INTR_ASYNC_CAUSE_CLR) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_INTR_ASYNC_CAUSE);
    AR_HOSTIF_REG(ah, AR_INTR_ASYNC_CAUSE) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_INTR_ASYNC_CAUSE);
    AR_HOSTIF_REG(ah, AR_INTR_ASYNC_ENABLE) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_INTR_ASYNC_ENABLE);
    AR_HOSTIF_REG(ah, AR_GPIO_OUT) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_GPIO_OUT);
    AR_HOSTIF_REG(ah, AR_GPIO_IN) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_GPIO_IN);
    AR_HOSTIF_REG(ah, AR_GPIO_OE_OUT) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_GPIO_OE);
    AR_HOSTIF_REG(ah, AR_GPIO_OE1_OUT) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_GPIO_OE1);
    AR_HOSTIF_REG(ah, AR_GPIO_INTR_POL) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_GPIO_INTR_POLAR);
    AR_HOSTIF_REG(ah, AR_GPIO_INPUT_EN_VAL) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_GPIO_INPUT_VALUE);
    AR_HOSTIF_REG(ah, AR_GPIO_INPUT_MUX1) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_GPIO_INPUT_MUX1);
    AR_HOSTIF_REG(ah, AR_GPIO_INPUT_MUX2) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_GPIO_INPUT_MUX2);
    AR_HOSTIF_REG(ah, AR_GPIO_OUTPUT_MUX1) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_GPIO_OUTPUT_MUX1);
    AR_HOSTIF_REG(ah, AR_GPIO_OUTPUT_MUX2) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_GPIO_OUTPUT_MUX2);
    AR_HOSTIF_REG(ah, AR_GPIO_OUTPUT_MUX3) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_GPIO_OUTPUT_MUX3);
    AR_HOSTIF_REG(ah, AR_INPUT_STATE) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_GPIO_INPUT_STATE);
    AR_HOSTIF_REG(ah, AR_CLKRUN) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_CLKRUN);
    AR_HOSTIF_REG(ah, AR_EEPROM_STATUS_DATA) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_EEPROM_STS);
    AR_HOSTIF_REG(ah, AR_OBS) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_OBS_CTRL);
    AR_HOSTIF_REG(ah, AR_RFSILENT) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_RFSILENT);
    AR_HOSTIF_REG(ah, AR_MISC) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_MISC);
    AR_HOSTIF_REG(ah, AR_PCIE_MSI) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_PCIE_MSI);
    AR_HOSTIF_REG(ah, AR_TDMA_CCA_CNTL) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_MAC_TDMA_CCA_CNTL);
    AR_HOSTIF_REG(ah, AR_TXAPSYNC) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_MAC_TXAPSYNC);
    AR_HOSTIF_REG(ah, AR_TXSYNC_INIT_SYNC_TMR) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_MAC_TXSYNC_INITIAL_SYNC_TMR);
    AR_HOSTIF_REG(ah, AR_INTR_PRIO_SYNC_CAUSE) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_INTR_PRIORITY_SYNC_CAUSE);
    AR_HOSTIF_REG(ah, AR_INTR_PRIO_SYNC_ENABLE) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_INTR_PRIORITY_SYNC_ENABLE);
    AR_HOSTIF_REG(ah, AR_INTR_PRIO_ASYNC_MASK) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_INTR_PRIORITY_ASYNC_MASK);
    AR_HOSTIF_REG(ah, AR_INTR_PRIO_SYNC_MASK) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_INTR_PRIORITY_SYNC_MASK);
    AR_HOSTIF_REG(ah, AR_INTR_PRIO_ASYNC_CAUSE) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_INTR_PRIORITY_ASYNC_CAUSE);
    AR_HOSTIF_REG(ah, AR_INTR_PRIO_ASYNC_ENABLE) =
        AR9340_HOSTIF_OFFSET(HOST_INTF_INTR_PRIORITY_ASYNC_ENABLE);
}

/* 
 * Host interface register offsets are different for Osprey and Wasp 
 * and hence store the offsets in hal structure
 */
static int ar9300_init_offsets(struct ath_hal *ah, u_int16_t devid)
{
    if (devid == AR9300_DEVID_AR9340) {
        ar9340_init_hostif_offsets(ah);
    } else {
        ar9300_init_hostif_offsets(ah);
    }
    return 0;
}
/* Jet specific API for temperature reading correction */
bool ar9300_set_temp_Ko_Kg(struct ath_hal *ah)
{
    // OTP address
#define ate_voltage_high_chain0_7_0_addr 38
#define ate_voltage_low_chain0_7_0_addr  39
#define ate_voltage_high_9_8_addr        40
#define ate_voltage_low_9_8_addr         41
#define ate_pdadc_high_chain0_7_0_addr   42
#define ate_pdadc_low_chain0_7_0_addr    43

    u_int8_t val_high_byte, val_low_byte;
    int32_t ate_voltage_high, ate_voltage_low, ate_pdadc_high, ate_pdadc_low;
    int32_t Ko_a=0, Ko=0, Kg=0;
    int32_t ideal_pdadc_high_1000, ideal_pdadc_low_1000;

    // OTP value
    ar9300_otp_read_byte(ah,ate_voltage_high_9_8_addr,&val_high_byte,true);
    ar9300_otp_read_byte(ah,ate_voltage_high_chain0_7_0_addr,&val_low_byte,true);
    ate_voltage_high=((val_high_byte &0x3)<<8)|(val_low_byte &0xff);
    ar9300_otp_read_byte(ah,ate_voltage_low_9_8_addr,&val_high_byte,true);
    ar9300_otp_read_byte(ah,ate_voltage_low_chain0_7_0_addr,&val_low_byte,true);
    ate_voltage_low =((val_high_byte &0x3)<<8)|(val_low_byte &0xff);
    ar9300_otp_read_byte(ah,ate_pdadc_high_chain0_7_0_addr,&val_low_byte,true);
    ate_pdadc_high=(val_low_byte &0xff);
    ar9300_otp_read_byte(ah,ate_pdadc_low_chain0_7_0_addr,&val_low_byte,true);
    ate_pdadc_low =(val_low_byte &0xff);

    // Ko_Kg calculation
    ideal_pdadc_high_1000 = ((ate_voltage_high * 16000)/25) - 80000;
    ideal_pdadc_low_1000 = ((ate_voltage_low * 16000)/25) - 80000;

    ar9300_otp_read_byte(ah,ate_voltage_high_chain0_7_0_addr,&val_low_byte,true);
    if(val_low_byte==0)
    {
        Kg=256;
        Ko=0;
    } else {
        if (ate_pdadc_high==ate_pdadc_low)
            Kg=256;
        else
            Kg=((256*(ideal_pdadc_high_1000-ideal_pdadc_low_1000))/(ate_pdadc_high-ate_pdadc_low)+500)/1000;
        if (ideal_pdadc_high_1000==ideal_pdadc_low_1000)
            Ko_a=0;
        else
            Ko_a=((ideal_pdadc_low_1000*(ate_pdadc_high-ate_pdadc_low)*10)/(ideal_pdadc_high_1000-ideal_pdadc_low_1000)+5)/10-ate_pdadc_low;
        if(Ko_a>=0) Ko=Ko_a;
        else        Ko=Ko_a+256;
    }

    // Write to register
    OS_REG_RMW_FIELD(ah,AR_PHY_BB_THERM_ADC_3,AR_PHY_BB_THERM_ADC_3_THERM_ADC_OFFSET,Ko);
    OS_REG_RMW_FIELD(ah,AR_PHY_BB_THERM_ADC_3,AR_PHY_BB_THERM_ADC_3_THERM_ADC_SCALE_GAIN,Kg);

#ifndef ART_BUILD
    ar9300_eeprom_t *eep = &AH9300(ah)->ah_eeprom;
#else
    ar5500_eeprom_t *eep = &AH9300(ah)->ah_eeprom_jet;
#endif
    eep->chipCalData.thermAdcScaledGain=Kg;
    eep->chipCalData.thermAdcOffset=Ko;

    return true;
}

#endif /* AH_SUPPORT_AR9300 */
