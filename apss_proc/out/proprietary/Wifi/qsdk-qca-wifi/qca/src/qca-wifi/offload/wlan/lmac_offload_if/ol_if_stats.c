/*
 * Copyright (c) 2015, 2017-2018 Qualcomm Innovation Center, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Innovation Center, Inc.
 */
/*
 * Copyright (c) 2011, Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * 2015 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 */

/*
 * LMAC offload interface functions for UMAC - for power and performance offload model
 */
#if ATH_SUPPORT_SPECTRAL
#include "spectral.h"
#endif
#include "ol_if_athvar.h"
#include "ol_if_athpriv.h"
#include "ol_if_athutf.h"
#include "ol_ath.h"
#include "qdf_mem.h"   /* qdf_mem_malloc,free */
#include "qdf_lock.h"  /* qdf_spinlock_* */
#include "qdf_types.h" /* qdf_vprint */
#include "dbglog_host.h"
#include "a_debug.h"
#include <wdi_event_api.h>
#include <ol_txrx_types.h>
#include <htt_internal.h>
#include <net.h>
#include <pktlog_ac_api.h>
#include <pktlog_ac_fmt.h>
#include <pktlog_ac_i.h>
#include "ol_tx_desc.h"
#include "ol_ratetable.h"
#include "ol_if_stats.h"
#include "osif_private.h"

#include "cepci.h"
#include "ath_pci.h"
#include "htt.h"

#ifdef QCA_NSS_WIFI_OFFLOAD_SUPPORT
#include <osif_nss_wifiol_if.h>
#endif


#if QCA_PARTNER_DIRECTLINK
#define QCA_PARTNER_DIRECTLINK_OL_IF_STATS 1
#include "ath_carr_pltfrm.h"
#undef QCA_PARTNER_DIRECTLINK_OL_IF_STATS
#endif /* QCA_PARTNER_DIRECTLINK */

#define TX_DESC_ID_LOW_MASK 0xffff
#define TX_DESC_ID_LOW_SHIFT 0
#define TX_DESC_ID_HIGH_MASK 0xffff0000
#define TX_DESC_ID_HIGH_SHIFT 16

#define TX_FRAME_OFFSET 13
#define TX_TYPE_OFFSET 14
#define TX_PEER_ID_MASK
#define TX_PEER_ID_OFFSET 1
#define TX_FRAME_TYPE_MASK 0x3c00000
#define TX_FRAME_TYPE_SHIFT 22
#define TX_FRAME_TYPE_NOACK_MASK 0x00010000
#define TX_FRAME_TYPE_NOACK_SHIFT 16
#define TX_TYPE_MASK 0xc0000
#define TX_TYPE_SHIFT 18
#define TX_AMPDU_SHIFT 15
#define TX_AMPDU_MASK 0x8000
#define PPDU_END_OFFSET 16
#define TX_OK_OFFSET (PPDU_END_OFFSET + 0)
#define TX_OK_MASK (0x80000000)
#define TX_RSSI_OFFSET (PPDU_END_OFFSET + 11)
#define TX_RSSI_MASK 0xff
#define RX_RSSI_COMB_MASK 0x000000ff
#define RX_RSSI_COMB_OFFSET 4
#define RX_RSSI_CHAIN_PRI20_MASK 0x000000ff
#define RX_RSSI_CHAIN_PRI20_SHIFT 0
#define RX_RSSI_CHAIN_SEC20_MASK 0x0000ff00
#define RX_RSSI_CHAIN_SEC20_SHIFT 8
#define RX_RSSI_CHAIN_SEC40_MASK 0x00ff0000
#define RX_RSSI_CHAIN_SEC40_SHIFT 16
#define RX_RSSI_CHAIN_SEC80_MASK 0xff000000
#define RX_RSSI_CHAIN_SEC80_SHIFT 24
#define RX_RSSI_CHAIN0_OFFSET 0
#define RX_RSSI_CHAIN1_OFFSET 1
#define RX_RSSI_CHAIN2_OFFSET 2
#define RX_RSSI_CHAIN3_OFFSET 3
#define RX_OVERFLOW_MASK 0x00010000
#define RX_NULL_DATA_MASK 0x00000080
#define SEQ_NUM_OFFSET 2
#define SEQ_NUM_MASK 0xfff
#define BA_BMAP_LSB_OFFSET 17
#define BA_BMAP_MSB_OFFSET 18
#define RATE_IDX_OFFSET 0
#define RATE_IDX_MASK 0x000000ff
#define SGI_SERIES_OFFSET 20
#define SGI_SERIES_MASK 0xff000000
#define SGI_SERIES_SHIFT 24
#define SERIES_BW_START_OFFSET 21
#define NUM_OF_DWORDS_SERIES_BW 4
#define SERIES_BW_MASK 0x30000000
#define SERIES_BW_SHIFT 28

#define PREAMBLE_MASK 0xff000000
#define PREAMBLE_SHIFT 24
#define RX_D_PREAMBLE_OFFSET 5
#define RX_D_MASK_LSIG_SEL  0x00000010
#define RX_D_MASK_LSIG_RATE 0x0000000f
#define RX_D_HT_RATE_OFFSET 6
#define RX_D_HT_MCS_MASK 0x7f
#define RX_D_VHT_RATE_OFFSET 7
#define RX_D_VHT_MCS_MASK 0xf
#define RX_D_VHT_NSS_MASK 0x7
#define RX_D_VHT_BW_MASK 3
#define RX_D_VHT_SGI_MASK 1

#define OL_ATH_RATE_EP_MULTIPLIER     (1<<7)  /* pow2 to optimize out * and / */
#define OL_ATH_EP_MUL(x, mul)         ((x) * (mul))
#define OL_ATH_RATE_LPF_LEN           10          // Low pass filter length for averaging rates
#define DUMMY_MARKER      0
#define OL_ATH_RATE_IN(x)             (OL_ATH_EP_MUL((x), OL_ATH_RATE_EP_MULTIPLIER))
#define OL_ATH_LPF_RATE(x, y, len) \
    (((x) != DUMMY_MARKER) ? ((((x) << 3) + (y) - (x)) >> 3) : (y))
#define OL_ATH_RATE_LPF(x, y) \
    ((x) = OL_ATH_LPF_RATE((x), OL_ATH_RATE_IN((y)), OL_ATH_RATE_LPF_LEN))
#define OL_ATH_RATE_OUT(x)  (((x) != DUMMY_MARKER) ? (OL_ATH_EP_RND((x), OL_ATH_RATE_EP_MULTIPLIER)) : DUMMY_MARKER)
#define OL_ATH_EP_RND(x, mul)       ((((x)%(mul)) >= ((mul)/2)) ? ((x) + ((mul) - 1)) / (mul) : (x)/(mul))

wdi_event_subscribe STATS_RX_SUBSCRIBER;
wdi_event_subscribe STATS_TX_SUBSCRIBER;
#if ATH_PERF_PWR_OFFLOAD
#define RXDESC_GET_DATA_LEN(rx_desc) \
    (txrx_pdev->htt_pdev->ar_rx_ops->msdu_desc_msdu_length(rx_desc))

char *intr_display_strings[] = {
    "ISR profile data",
    "DSR profile data",
    "SWTASK profile data",
    "DSR LATENCY data",
    "SWTASK LATENCY data",
    0,
};

/*
 * Function num_elements get the number of elements in array intr_display_strings
 */
static
int num_elements(char *intr_display_strings[])
{
    int i;
    for(i=0; intr_display_strings[i]==0;i++);
    return i;
}

#define MAX_STRING_IDX num_elements(intr_display_strings)

int
ol_ath_wlan_profile_data_event_handler (ol_scn_t scn, u_int8_t *data, u_int16_t datalen)
{
    u_int32_t i;
    int32_t base;
    int32_t stridx = -1;
    wmi_host_wlan_profile_ctx_t profile_ctx;
    wmi_host_wlan_profile_t profile_data;

    if (wmi_extract_profile_ctx(scn->wmi_handle, data, &profile_ctx)) {
	qdf_print("Extracting profile ctx failed\n");
	return -1;
    }

    /* A small state machine implemented here to enable the
     * WMI_WLAN_PROFILE_DATA_EVENTID message to be used iteratively
     * to report > 32 profile points. The target sends a series of
     * events; on the first one in the series, the tot field will
     * be non-zero; subsequent events in the series have tot == 0
     * and are assumed to be a continuation of the first.
     * On the initial record, print the header.
     */
    if (profile_ctx.tot != 0) {
    QDF_PRINT_INFO(QDF_PRINT_IDX_SHARED, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "\n\t PROFILE DATA\n");
    QDF_PRINT_INFO(QDF_PRINT_IDX_SHARED, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "Profile duration : %d\n", profile_ctx.tot);
    QDF_PRINT_INFO(QDF_PRINT_IDX_SHARED, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "Tx Msdu Count    : %d\n", profile_ctx.tx_msdu_cnt);
    QDF_PRINT_INFO(QDF_PRINT_IDX_SHARED, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "Tx Mpdu Count    : %d\n", profile_ctx.tx_mpdu_cnt);
    QDF_PRINT_INFO(QDF_PRINT_IDX_SHARED, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "Tx Ppdu Count    : %d\n", profile_ctx.tx_ppdu_cnt);
    QDF_PRINT_INFO(QDF_PRINT_IDX_SHARED, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "Rx Msdu Count    : %d\n", profile_ctx.rx_msdu_cnt);
    QDF_PRINT_INFO(QDF_PRINT_IDX_SHARED, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "Rx Mpdu Count    : %d\n", profile_ctx.rx_mpdu_cnt);

    QDF_PRINT_INFO(QDF_PRINT_IDX_SHARED, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "Profile ID   Count   Total      Min      Max   hist_intvl  hist[0]   hist[1]   hist[2]\n");
        base = 0;
    } else {
        base = -1;
    }

    for(i=0; i<profile_ctx.bin_count; i++) {
        uint32_t id;
	if (wmi_extract_profile_data(scn->wmi_handle, data, i, &profile_data)) {
	    qdf_print("Extracting profile data failed\n");
	    return -1;
	}
	id = profile_data.id;
        if (profile_ctx.tot == 0) {
            if (base == -1) {
                stridx = 0;
                base = 0;
                QDF_PRINT_INFO(QDF_PRINT_IDX_SHARED, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "\n%s\n", intr_display_strings[stridx]);
            } else if (((id - base) > 32) && (stridx < MAX_STRING_IDX)) {
                stridx++;
                base += 32;
                QDF_PRINT_INFO(QDF_PRINT_IDX_SHARED, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "\n%s\n", intr_display_strings[stridx]);
            }
        }

        QDF_PRINT_INFO(QDF_PRINT_IDX_SHARED, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "%8d   %8d %8d %8d %8d     %8d %8d %8d %8d\n", profile_data.id - base,
            profile_data.cnt, profile_data.tot,
            profile_data.min, profile_data.max,
            profile_data.hist_intvl, profile_data.hist[0],
            profile_data.hist[1], profile_data.hist[2]);

    }

    QDF_PRINT_INFO(QDF_PRINT_IDX_SHARED, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "\n");
    return 0;
}

#ifdef ATH_SUPPORT_RDKB
int
convert_wmi_host_chan_info_to_periodic_chan_stats(
        wmi_host_chan_info_event *chan_info_ev,
        struct ieee80211_mib_cycle_cnts *new_stats)
{
    new_stats->cycle_count = chan_info_ev->cycle_count;
    new_stats->rx_clear_count = chan_info_ev->rx_clear_count;
    new_stats->rx_frame_count = chan_info_ev->rx_frame_count;
    new_stats->my_bss_rx_cycle_count = chan_info_ev->my_bss_rx_cycle_count;
    new_stats->tx_frame_count = chan_info_ev->tx_frame_count;
    new_stats->rx_clear_ext_count = chan_info_ev->rx_clear_ext20_count;
    new_stats->rx_clear_ext40_count = chan_info_ev->rx_clear_ext40_count;
    new_stats->rx_clear_ext80_count = chan_info_ev->rx_clear_ext80_count;

    return EOK;
}

int ol_scan_chan_stats_update(struct ieee80211com *ic,
        wmi_host_chan_info_event *chan_info_ev)
{
    int i = 0;
    uint8_t flag = (uint8_t)chan_info_ev->cmd_flags;
    struct scan_chan_entry_stats *entry_stats = NULL;
    struct ieee80211_mib_cycle_cnts new_stats = {0};
    struct ieee80211_mib_cycle_cnts delta = {0};
    struct channel_stats *chan_stats = &(ic->ic_scan_chan_stats.stats[0]);

    convert_wmi_host_chan_info_to_periodic_chan_stats(chan_info_ev, &new_stats);
    entry_stats = &(ic->ic_scan_chan_stats.entry_stats);

    if (flag == WMI_CHAN_INFO_FLAG_START_RESP) {
       entry_stats->freq = chan_info_ev->freq;
       entry_stats->stats = new_stats;
    } else if (flag == WMI_CHAN_INFO_FLAG_END_RESP) {
        if ((!entry_stats->stats.cycle_count) ||
                (entry_stats->freq != chan_info_ev->freq)) {
            qdf_mem_zero(entry_stats, sizeof(*entry_stats));
            return -EINVAL;
        }
        if (ol_find_chan_stats_delta(ic, &entry_stats->stats, &new_stats, &delta)) {
            qdf_mem_zero(entry_stats, sizeof(*entry_stats));
            return -EINVAL;
        }
        for (i = 0; i < MAX_DUAL_BAND_SCAN_CHANS; i++) {
            if ((chan_stats[i].freq == entry_stats->freq) ||
                    (chan_stats[i].freq == 0)) {
                chan_stats[i].freq = entry_stats->freq;
                chan_stats[i].cycle_cnt += delta.cycle_count;
                chan_stats[i].clear_cnt += delta.rx_clear_count;
                chan_stats[i].tx_frm_cnt += delta.tx_frame_count;
                chan_stats[i].rx_frm_cnt += delta.rx_frame_count;
                chan_stats[i].bss_rx_cnt += delta.my_bss_rx_cycle_count;
                break;
            }
        }
        qdf_mem_zero(entry_stats, sizeof(*entry_stats));
    }
    return EOK;
}
#endif
/*
 * WMI event handler for Channel info WMI event
 */
static int
ol_ath_chan_info_event_handler(ol_scn_t scn, u_int8_t *data, u_int16_t datalen)
{
    struct ieee80211com *ic = &scn->sc_ic;
    u_int8_t flags;
    u_int ieee_chan;
    int16_t chan_nf;
    struct ieee80211_chan_stats chan_stats;

    wmi_host_chan_info_event chan_info_ev;
    wmi_host_chan_info_event *event = (wmi_host_chan_info_event *)&chan_info_ev;

    wmi_extract_chan_info_event(scn->wmi_handle, data, &chan_info_ev);

    if(event->err_code == 0) {
#if 0
       QDF_PRINT_INFO(ic->ic_print_idx, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "WMI event chan freq:%d, flags %d, nf %d, clr_cnt %d, cycle_cnt is %d tx power %d %d\n",
                   event->freq,event->cmd_flags,event->noise_floor,event->rx_clear_count,
                   event->cycle_count, event->chan_tx_pwr_range, event->chan_tx_pwr_tp);
#endif
#ifdef ATH_SUPPORT_RDKB
       ol_scan_chan_stats_update(ic, event);
#endif
       ieee_chan = ol_ath_mhz2ieee(ic, event->freq, 0);
       flags = (u_int8_t)event->cmd_flags;
       chan_nf = (int16_t)event->noise_floor;
       chan_stats.chan_clr_cnt = event->rx_clear_count;
       chan_stats.cycle_cnt = event->cycle_count;
       chan_stats.chan_tx_power_range = event->chan_tx_pwr_range;
       chan_stats.chan_tx_power_tput = event->chan_tx_pwr_tp;
       chan_stats.duration_11b_data  = event->rx_11b_mode_data_duration;
       ieee80211_acs_stats_update(ic->ic_acs, flags, ieee_chan, chan_nf, &chan_stats);
       if (flags == WMI_CHAN_INFO_FLAG_BEFORE_END_RESP) {
           scn->chan_nf = chan_nf;
       }

#if ATH_SUPPORT_SPECTRAL
       if (scn->scn_icm_active) {
           if (flags == WMI_CHAN_INFO_FLAG_START_RESP) {
               ic->chan_clr_cnt = chan_stats.chan_clr_cnt;
               ic->cycle_cnt = chan_stats.cycle_cnt;
               ic->chan_num = ieee_chan;
           } else if (flags == WMI_CHAN_INFO_FLAG_END_RESP) {
               struct ath_spectral* spectral =
                   (struct ath_spectral*)ic->ic_spectral;
               SPECTRAL_LOCK(spectral);
               spectral_record_chan_info(spectral,
                                         ieee_chan,
                                         true,
                                         chan_stats.chan_clr_cnt,
                                         ic->chan_clr_cnt,
                                         chan_stats.cycle_cnt,
                                         ic->cycle_cnt,
                                         true,        /* Whether NF is valid */
                                         chan_nf,
                                         false,       /* Whether PER is valid */
                                         0);          /* PER */
               SPECTRAL_UNLOCK(spectral);
           }
       }
#endif
    }
    else {
          /** FIXME Handle it, whenever channel freq mismatch occurs in target */
       QDF_PRINT_INFO(ic->ic_print_idx, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "Err code is non zero, Failed to read stats from target \n");
    }
    return 0;
}

static void ol_ath_net80211_set_noise_detection_param(struct ieee80211com *ic, int cmd,int val)
{
    struct ol_ath_softc_net80211 *scn = OL_ATH_SOFTC_NET80211(ic);
    u_int8_t flush_stats = 0;
#define IEEE80211_ENABLE_NOISE_DETECTION 1
#define IEEE80211_NOISE_THRESHOLD 2
    switch(cmd)
    {
        case IEEE80211_ENABLE_NOISE_DETECTION:
            if(val) {
                scn->sc_enable_noise_detection = val;
                /* Disabling DCS as soon as we enable noise detection algo */
                ic->ic_disable_dcscw(ic);

                flush_stats = 1;
                ol_ath_pdev_set_param(scn, wmi_pdev_param_noise_detection, val, 0);
            }
          break;
        case IEEE80211_NOISE_THRESHOLD:
          if(val) {
              scn->sc_noise_floor_th = val;
              ol_ath_pdev_set_param(scn, wmi_pdev_param_noise_threshold, val, 0);
              flush_stats = 1;
          }
          break;

        default:
              QDF_PRINT_INFO(ic->ic_print_idx, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "UNKNOWN param %s %d \n",__func__,__LINE__);
          break;
    }
    if(flush_stats) {
        scn->sc_noise_floor_report_iter = 0;
        scn->sc_noise_floor_total_iter = 0;
    }
#undef IEEE80211_ENABLE_NOISE_DETECTION
#undef IEEE80211_NOISE_THRESHOLD
    return;
}


static void ol_ath_net80211_get_noise_detection_param(struct ieee80211com *ic, int cmd,int *val)
{
    struct ol_ath_softc_net80211 *scn = OL_ATH_SOFTC_NET80211(ic);
#define IEEE80211_ENABLE_NOISE_DETECTION 1
#define IEEE80211_NOISE_THRESHOLD 2
#define IEEE80211_GET_COUNTER_VALUE 3
    switch(cmd)
    {
        case IEEE80211_ENABLE_NOISE_DETECTION:
          *val = scn->sc_enable_noise_detection;
          break;
        case IEEE80211_NOISE_THRESHOLD:
          *val =(int) scn->sc_noise_floor_th;
          break;
        case IEEE80211_GET_COUNTER_VALUE:
          if( scn->sc_noise_floor_total_iter) {
              *val =  (scn->sc_noise_floor_report_iter *100)/scn->sc_noise_floor_total_iter;
          } else
              *val = 0;
          break;
        default:
              QDF_PRINT_INFO(ic->ic_print_idx, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "UNKNOWN param %s %d \n",__func__,__LINE__);
          break;
    }
#undef IEEE80211_ENABLE_NOISE_DETECTION
#undef IEEE80211_NOISE_THRESHOLD
    return;

}

static int
ol_ath_channel_hopping_event_handler(ol_scn_t scn, u_int8_t *data,
                                    u_int16_t datalen)
{
    wmi_host_pdev_channel_hopping_event channel_hopping_event;
    wmi_host_pdev_channel_hopping_event *event =
            (wmi_host_pdev_channel_hopping_event *) &channel_hopping_event;

    if(!wmi_extract_channel_hopping_event(scn->wmi_handle, data, event)) {

        scn->sc_noise_floor_report_iter = event->noise_floor_report_iter;
        scn->sc_noise_floor_total_iter = event->noise_floor_total_iter;
    }

    return 0;
}
/*
 * Registers WMI event handler for Channel info event
 */
void
ol_ath_chan_info_attach(struct ieee80211com *ic)
{
    struct ol_ath_softc_net80211 *scn = OL_ATH_SOFTC_NET80211(ic);
    /* Register WMI event handlers */
    wmi_unified_register_event_handler(scn->wmi_handle, wmi_chan_info_event_id,
                                            ol_ath_chan_info_event_handler, WMI_RX_UMAC_CTX);

    /*used as part of channel hopping algo to trigger noise detection in counter window */
    ic->ic_set_noise_detection_param     = ol_ath_net80211_set_noise_detection_param;
    ic->ic_get_noise_detection_param     = ol_ath_net80211_get_noise_detection_param;
    wmi_unified_register_event_handler(scn->wmi_handle,
                                        wmi_pdev_channel_hopping_event_id,
                                        ol_ath_channel_hopping_event_handler, WMI_RX_UMAC_CTX);
    return;
}

/*
 * Unregisters WMI event handler for Channel info event
 */
void
ol_ath_chan_info_detach(struct ieee80211com *ic)
{
    struct ol_ath_softc_net80211 *scn = OL_ATH_SOFTC_NET80211(ic);
    /* Unregister ACS event handler */
    wmi_unified_unregister_event_handler(scn->wmi_handle, wmi_chan_info_event_id);

    wmi_unified_unregister_event_handler(scn->wmi_handle, wmi_pdev_channel_hopping_event_id);
    return;
}

/*
 * Prepares and Sends the WMI cmd to retrieve channel info from Target
 */
static void
ol_ath_get_chan_info(struct ieee80211com *ic, u_int8_t flags)
{
    /* In offload architecture this is stub function
       because target chan information is indicated as events timely
       (No need to poll for the chan info
     */
}

/**
* @brief    sets chan_stats to zero to mark invalid stats
*
* @param ic pointer to struct ieee80211com
*
* @return   0
*/
inline int
ol_ath_invalidate_channel_stats(struct ieee80211com *ic)
{
    struct ol_ath_softc_net80211 *scn = OL_ATH_SOFTC_NET80211(ic);
    qdf_mem_zero(&(scn->scn_dcs.chan_stats), sizeof(scn->scn_dcs.chan_stats));
    return 0;
}
#ifdef ATH_SUPPORT_RDKB
int ol_find_chan_stats_delta(struct ieee80211com *ic,
                    periodic_chan_stats_t *pstats,
                    periodic_chan_stats_t *nstats,
                    periodic_chan_stats_t *delta)
{
    struct ol_ath_softc_net80211 *scn = OL_ATH_SOFTC_NET80211(ic);

    /* Peregrine hardware design is such that when cycle counter reaches
     * 0xffffffff, cycle counters along with other counters are right shifted by
     * one bit in same cycle. As we don't know the max value of different
     * HW counters before wrapping, there is no way to calculate correct
     * channel utilization for Peregrine family.
     *
     * For Beeliner, Cascade, Besra etc, all HW counters reach their max value
     * of 0xffffffff on their own and then wrap to 0x7fffffff. Here we know
     * the max value of different counters (0xffffffff) before wrap around so
     * channel utilization can be calculated correctly.
     */

    /* Drop Peregrine wrap around cases */
    if ((!scn->is_ar900b) && (pstats->cycle_count > nstats->cycle_count)) {
        IEEE80211_DPRINTF_IC(ic, IEEE80211_VERBOSE_NORMAL, IEEE80211_MSG_EXTIOCTL_CHANSWITCH,
                "%s: ignoring due to counter wrap around: p_cycle: %u, n_cycle: %u,"
                " p_rx_clear: %u, n_rx_clear: %u, p_mybss_rx: %u, n_mybss_rx: %u\n",
                 __func__, pstats->cycle_count, nstats->cycle_count, pstats->rx_clear_count,
                 nstats->rx_clear_count, pstats->my_bss_rx_cycle_count,
                 nstats->my_bss_rx_cycle_count);
        return -EINVAL;
    }

    /* We are here it means either it's beeliner family or non wrap aroud case.
     * calculate different delta counts based on previous mib counters and new
     * mib counters.
     */

    if (pstats->cycle_count > nstats->cycle_count) {
        delta->cycle_count = (IEEE80211_MAX_32BIT_UNSIGNED_VALUE - pstats->cycle_count) +
                            (nstats->cycle_count - (IEEE80211_MAX_32BIT_UNSIGNED_VALUE >> 1));
    } else {
        delta->cycle_count = nstats->cycle_count - pstats->cycle_count;
    }

    if (pstats->rx_clear_count > nstats->rx_clear_count) {
        delta->rx_clear_count = (IEEE80211_MAX_32BIT_UNSIGNED_VALUE - pstats->rx_clear_count) +
                      (nstats->rx_clear_count - (IEEE80211_MAX_32BIT_UNSIGNED_VALUE >> 1));
    } else {
        delta->rx_clear_count = nstats->rx_clear_count - pstats->rx_clear_count;
    }

    if (pstats->tx_frame_count > nstats->tx_frame_count) {
        delta->tx_frame_count = (IEEE80211_MAX_32BIT_UNSIGNED_VALUE - pstats->tx_frame_count) +
                         (nstats->tx_frame_count - (IEEE80211_MAX_32BIT_UNSIGNED_VALUE >> 1));
    } else {
        delta->tx_frame_count = nstats->tx_frame_count - pstats->tx_frame_count;
    }

    if (pstats->my_bss_rx_cycle_count > nstats->my_bss_rx_cycle_count) {
        delta->my_bss_rx_cycle_count = IEEE80211_MAX_32BIT_UNSIGNED_VALUE - pstats->my_bss_rx_cycle_count
                          + nstats->my_bss_rx_cycle_count;
    } else {
        delta->my_bss_rx_cycle_count = nstats->my_bss_rx_cycle_count - pstats->my_bss_rx_cycle_count;
    }

    if (pstats->rx_frame_count > nstats->rx_frame_count) {
        delta->rx_frame_count = (IEEE80211_MAX_32BIT_UNSIGNED_VALUE - pstats->rx_frame_count) +
                         (nstats->rx_frame_count - (IEEE80211_MAX_32BIT_UNSIGNED_VALUE >> 1));
    } else {
        delta->rx_frame_count = nstats->rx_frame_count - pstats->rx_frame_count;
    }

    if (pstats->rx_clear_ext_count > nstats->rx_clear_ext_count) {
        delta->rx_clear_ext_count = (IEEE80211_MAX_32BIT_UNSIGNED_VALUE - pstats->rx_clear_ext_count) +
                         (nstats->rx_clear_ext_count - (IEEE80211_MAX_32BIT_UNSIGNED_VALUE >> 1));
    } else {
        delta->rx_clear_ext_count = nstats->rx_clear_ext_count - pstats->rx_clear_ext_count;
    }

    /* Suggested workarounds for corner cases by HW team */
    if (delta->rx_clear_count < (delta->tx_frame_count + delta->rx_frame_count)) {
	        delta->rx_clear_count = delta->tx_frame_count + delta->rx_frame_count;
    }

    if (delta->my_bss_rx_cycle_count > delta->rx_frame_count) {
	        delta->my_bss_rx_cycle_count = delta->rx_frame_count;
    }

    return EOK;
}

/**
* @brief            Calculate obss and self bss utilization
*                   and invoke API's to send it to user space
*
* @param ic         pointer to ieee80211com
* @param pstats     previous saved stats
* @param nstats     new stats
*/
void ol_chan_stats_event (struct ieee80211com *ic,
                    periodic_chan_stats_t *pstats,
                    periodic_chan_stats_t *nstats)
{
    uint8_t total_cu = 0;
    uint8_t ap_tx_cu = 0;
    uint8_t ap_rx_cu = 0;
    uint8_t self_util = 0;
    uint8_t num_of_ss = 0;
    uint8_t rx_obss_cu = 0;
    uint8_t non_wifi_cu = 0;
    struct ol_ath_softc_net80211 *scn = NULL;
    periodic_chan_stats_t delta = {0};
    union iwreq_data wrqu;
    char buf[128];

    if (!pstats || !nstats) {
        return;
    }

    /* If previous stats is 0, we don't have previous counter
     * values and we can't find utilization for last period.
     */
     if (pstats->cycle_count == 0) {
         return;
     }

    scn = OL_ATH_SOFTC_NET80211(ic);
    ol_find_chan_stats_delta(ic, pstats, nstats, &delta);

    /* Calculate self bss and obss channel utilization
     * based on previous mib counters and new mib counters
     */
    if(!delta.cycle_count) {
        return;
    }
    /* Calculate total wifi + non wifi channel utilization percentage */
    total_cu = ((delta.rx_clear_count) / (delta.cycle_count / 100));
    /* Calculate total AP wlan Tx channel utilization percentage */
    ap_tx_cu = ((delta.tx_frame_count) / (delta.cycle_count / 100));
    /* Calculate total AP wlan Rx channel utilization percentage */
    ap_rx_cu = ((delta.my_bss_rx_cycle_count) / (delta.cycle_count / 100));
    /* Calculate total Rx channel utilization percentage */
    rx_obss_cu = ((delta.rx_frame_count - delta.my_bss_rx_cycle_count) / (delta.cycle_count / 100));

    /* Calculate Non-wifi channel utilization percentage */
    non_wifi_cu = (((delta.rx_clear_count) - (delta.tx_frame_count + delta.rx_frame_count)) / (delta.cycle_count / 100));

    /* Self bss channel utilization is sum of AP Tx and AP Rx utilization */
    self_util = ap_tx_cu + ap_rx_cu;
    scn->scn_stats.self_bss_util = self_util;

    scn->scn_stats.obss_util = rx_obss_cu;

    num_of_ss = ieee80211com_get_spatialstreams(ic);
    if (num_of_ss == 0 || delta.tx_frame_count == 0)
        ic->ic_ss_uu = 0;
    else
        ic->ic_ss_uu = ((((num_of_ss * delta.rx_clear_count) - delta.tx_frame_count)/(num_of_ss * delta.tx_frame_count))*255);

    ic->ic_sec_20u = nstats->rx_clear_ext_count;
    ic->ic_sec_40u = nstats->rx_clear_ext40_count;
    ic->ic_sec_80u = nstats->rx_clear_ext80_count;

    ic->ic_home_chan_stats.cycle_cnt += delta.cycle_count;
    ic->ic_home_chan_stats.clear_cnt += delta.rx_clear_count;
    ic->ic_home_chan_stats.tx_frm_cnt += delta.tx_frame_count;
    ic->ic_home_chan_stats.rx_frm_cnt += delta.rx_frame_count;
    ic->ic_home_chan_stats.bss_rx_cnt += delta.my_bss_rx_cycle_count;
    ic->ic_home_chan_stats.ext_busy_cnt += delta.rx_clear_ext_count;

    IEEE80211_DPRINTF_IC(ic, IEEE80211_VERBOSE_NORMAL, IEEE80211_MSG_EXTIOCTL_CHANSWITCH,
            "p_cycle: %u, n_cycle: %u, p_rx_clear: %u, n_rx_clear: %u "
            "p_tx: %u, n_tx: %u, p_mybss_rx: %u, n_mybss_rx: %u, p_rx: %u, n_rx: %u, obss: %u, self: %u, ap_rx_cu=%u, ap_tx_cu=%u, total_cu=%u, non_wifi_cu=%u rxclr_delta=%d, tx_frame_delta=%d, rx_frame_delta=%d cycle_count_delta=%d\n",
             pstats->cycle_count, nstats->cycle_count, pstats->rx_clear_count, nstats->rx_clear_count,
             pstats->tx_frame_count, nstats->tx_frame_count, pstats->my_bss_rx_cycle_count,
             nstats->my_bss_rx_cycle_count, pstats->rx_frame_count, nstats->rx_frame_count, rx_obss_cu, self_util, ap_rx_cu, ap_tx_cu, total_cu, non_wifi_cu, delta.rx_clear_count, delta.tx_frame_count, delta.rx_frame_count, delta.cycle_count);

#if UMAC_SUPPORT_ACFG
    /* send acfg event with channel stats now */
    acfg_chan_stats_event(ic, self_util, rx_obss_cu);
#endif
    ic->ic_ch_util.ap_rx = ap_rx_cu;
    ic->ic_ch_util.free_medium = (100 - total_cu);
    ic->ic_ch_util.ap_tx = ap_tx_cu;
    ic->ic_ch_util.obss_rx = rx_obss_cu;
    ic->ic_ch_util.non_wifi = non_wifi_cu;
    /* Send iwevent with the channel Utilization info */
    snprintf(buf, sizeof(buf), "Channel Utilization:"
                                "AP Rx:%d Unused:%d AP Tx:%d OBSS Rx:%d",
                                ap_rx_cu, (100 - total_cu),
                                ap_tx_cu, rx_obss_cu);
    qdf_mem_zero(&wrqu, sizeof(wrqu));
    wrqu.data.length = strlen(buf);
    WIRELESS_SEND_EVENT(ic->ic_osdev->netdev, IWEVCUSTOM, &wrqu, buf);
}
#else
/**
 * * @brief            Calculate obss and self bss utilization
 * *                   and invoke API's to send it to user space
 * *
 * * @param ic         pointer to ieee80211com
 * * @param pstats     previous saved stats
 * * @param nstats     new stats
 * */
void ol_chan_stats_event (struct ieee80211com *ic,
                            periodic_chan_stats_t *pstats,
                                                periodic_chan_stats_t *nstats)
{
    u_int32_t tx_frame_delta = 0;
    u_int32_t rxclr_delta = 0;
    u_int32_t cycle_count_delta = 0;
    u_int32_t my_bss_rx_delta = 0;
    u_int32_t rx_frame_delta = 0;
    uint8_t total_cu = 0;
    uint8_t ap_tx_cu = 0;
    uint8_t ap_rx_cu = 0;
    uint8_t self_util = 0;
    uint8_t num_of_ss = 0;
    uint8_t rx_obss_cu = 0;
    uint8_t non_wifi_cu = 0;
    struct ol_ath_softc_net80211 *scn = NULL;
    union iwreq_data wrqu;
    char buf[128];

    if (!pstats || !nstats) {
        return;
    }

    /* If previous stats is 0, we don't have previous counter
     * values and we can't find utilization for last period.
     */
    if (pstats->cycle_count == 0) {
        return;
    }

    scn = OL_ATH_SOFTC_NET80211(ic);

    /* Peregrine hardware design is such that when cycle counter reaches
     * 0xffffffff, cycle counters along with other counters are right shifted by
     * one bit in same cycle. As we don't know the max value of different
     * HW counters before wrapping, there is no way to calculate correct
     * channel utilization for Peregrine family.
     *
     * For Beeliner, Cascade, Besra etc, all HW counters reach their max value
     * of 0xffffffff on their own and then wrap to 0x7fffffff. Here we know
     * the max value of different counters (0xffffffff) before wrap around so
     * channel utilization can be calculated correctly.
     */

    /* Drop Peregrine wrap around cases */
    if ((!scn->is_ar900b) && (pstats->cycle_count > nstats->cycle_count)) {
        IEEE80211_DPRINTF_IC(ic, IEEE80211_VERBOSE_NORMAL, IEEE80211_MSG_EXTIOCTL_CHANSWITCH,
                "%s: ignoring due to counter wrap around: p_cycle: %u, n_cycle: %u,"
                " p_rx_clear: %u, n_rx_clear: %u, p_mybss_rx: %u, n_mybss_rx: %u\n",
                __func__, pstats->cycle_count, nstats->cycle_count, pstats->rx_clear_count,
                nstats->rx_clear_count, pstats->my_bss_rx_cycle_count,
                nstats->my_bss_rx_cycle_count);
        return;
    }

    /* We are here it means either it's beeliner family or non wrap aroud case.
     * calculate different delta counts based on previous mib counters and new
     * mib counters.
     */

    if (pstats->cycle_count > nstats->cycle_count) {
        cycle_count_delta = (IEEE80211_MAX_32BIT_UNSIGNED_VALUE - pstats->cycle_count) +
            (nstats->cycle_count - (IEEE80211_MAX_32BIT_UNSIGNED_VALUE >> 1));
    } else {
        cycle_count_delta = nstats->cycle_count - pstats->cycle_count;
    }

    if (pstats->rx_clear_count > nstats->rx_clear_count) {
        rxclr_delta = (IEEE80211_MAX_32BIT_UNSIGNED_VALUE - pstats->rx_clear_count) +
            (nstats->rx_clear_count - (IEEE80211_MAX_32BIT_UNSIGNED_VALUE >> 1));
    } else {
        rxclr_delta = nstats->rx_clear_count - pstats->rx_clear_count;
    }

    if (pstats->tx_frame_count > nstats->tx_frame_count) {
        tx_frame_delta = (IEEE80211_MAX_32BIT_UNSIGNED_VALUE - pstats->tx_frame_count) +
            (nstats->tx_frame_count - (IEEE80211_MAX_32BIT_UNSIGNED_VALUE >> 1));
    } else {
        tx_frame_delta = nstats->tx_frame_count - pstats->tx_frame_count;
    }

    if (pstats->my_bss_rx_cycle_count > nstats->my_bss_rx_cycle_count) {
        my_bss_rx_delta = IEEE80211_MAX_32BIT_UNSIGNED_VALUE - pstats->my_bss_rx_cycle_count
            + nstats->my_bss_rx_cycle_count;
    } else {
        my_bss_rx_delta = nstats->my_bss_rx_cycle_count - pstats->my_bss_rx_cycle_count;
    }

    if (pstats->rx_frame_count > nstats->rx_frame_count) {
        rx_frame_delta = (IEEE80211_MAX_32BIT_UNSIGNED_VALUE - pstats->rx_frame_count) +
            (nstats->rx_frame_count - (IEEE80211_MAX_32BIT_UNSIGNED_VALUE >> 1));
    } else {
        rx_frame_delta = nstats->rx_frame_count - pstats->rx_frame_count;
    }

    /* Suggested workarounds for corner cases by HW team */
    if (rxclr_delta < (tx_frame_delta + rx_frame_delta)) {
        rxclr_delta = tx_frame_delta + rx_frame_delta;
    }

    if (my_bss_rx_delta > rx_frame_delta) {
        my_bss_rx_delta = rx_frame_delta;
    }

    /* Calculate self bss and obss channel utilization
     * based on previous mib counters and new mib counters
     */

    /* Calculate total wifi + non wifi channel utilization percentage */
    total_cu = ((rxclr_delta) / (cycle_count_delta / 100));
    /* Calculate total AP wlan Tx channel utilization percentage */
    ap_tx_cu = ((tx_frame_delta) / (cycle_count_delta / 100));
    /* Calculate total AP wlan Rx channel utilization percentage */
    ap_rx_cu = ((my_bss_rx_delta) / (cycle_count_delta / 100));
    /* Calculate total Rx channel utilization percentage */
    rx_obss_cu = ((rx_frame_delta - my_bss_rx_delta) / (cycle_count_delta / 100));

    /* Calculate Non-wifi channel utilization percentage */
    non_wifi_cu = (((rxclr_delta) - (tx_frame_delta + rx_frame_delta)) / (cycle_count_delta / 100));

    /* Self bss channel utilization is sum of AP Tx and AP Rx utilization */
    self_util = ap_tx_cu + ap_rx_cu;
    scn->scn_stats.self_bss_util = self_util;

    scn->scn_stats.obss_util = rx_obss_cu;

    num_of_ss = ieee80211com_get_spatialstreams(ic);
    if (num_of_ss == 0 || tx_frame_delta == 0)
        ic->ic_ss_uu = 0;
    else
        ic->ic_ss_uu = ((((num_of_ss * rxclr_delta) - tx_frame_delta)/(num_of_ss * tx_frame_delta))*255);

    ic->ic_sec_20u = nstats->rx_clear_ext_count;
    ic->ic_sec_40u = nstats->rx_clear_ext40_count;
    ic->ic_sec_80u = nstats->rx_clear_ext80_count;

    IEEE80211_DPRINTF_IC(ic, IEEE80211_VERBOSE_NORMAL, IEEE80211_MSG_EXTIOCTL_CHANSWITCH,
            "p_cycle: %u, n_cycle: %u, p_rx_clear: %u, n_rx_clear: %u "
            "p_tx: %u, n_tx: %u, p_mybss_rx: %u, n_mybss_rx: %u, p_rx: %u, n_rx: %u, obss: %u, self: %u,"
            "ap_rx_cu=%u, ap_tx_cu=%u, total_cu=%u, non_wifi_cu=%u rxclr_delta=%d, tx_frame_delta=%d,"
            "rx_frame_delta=%d cycle_count_delta=%d\n",
            pstats->cycle_count, nstats->cycle_count, pstats->rx_clear_count, nstats->rx_clear_count,
            pstats->tx_frame_count, nstats->tx_frame_count, pstats->my_bss_rx_cycle_count,
            nstats->my_bss_rx_cycle_count, pstats->rx_frame_count, nstats->rx_frame_count, rx_obss_cu,
            self_util, ap_rx_cu, ap_tx_cu, total_cu, non_wifi_cu, rxclr_delta, tx_frame_delta,
            rx_frame_delta, cycle_count_delta);

#if UMAC_SUPPORT_ACFG
    /* send acfg event with channel stats now */
    acfg_chan_stats_event(ic, self_util, rx_obss_cu);
#endif
    ic->ic_ch_util.ap_rx = ap_rx_cu;
    ic->ic_ch_util.free_medium = (100 - total_cu);
    ic->ic_ch_util.ap_tx = ap_tx_cu;
    ic->ic_ch_util.obss_rx = rx_obss_cu;
    ic->ic_ch_util.non_wifi = non_wifi_cu;
    /* Send iwevent with the channel Utilization info */
    snprintf(buf, sizeof(buf), "Channel Utilization:"
            "AP Rx:%d Unused:%d AP Tx:%d OBSS Rx:%d",
            ap_rx_cu, (100 - total_cu),
            ap_tx_cu, rx_obss_cu);
    qdf_mem_zero(&wrqu, sizeof(wrqu));
    wrqu.data.length = strlen(buf);
    WIRELESS_SEND_EVENT(ic->ic_osdev->netdev, IWEVCUSTOM, &wrqu, buf);
}
#endif


static void ol_ath_vap_iter_update_txpow(void *arg, wlan_if_t vap)
{
    struct ieee80211_node *ni;
#if ATH_BAND_STEERING
    u_int16_t oldTxpow;
#endif
    u_int16_t txpowlevel = *((u_int32_t *) arg);
    if(vap){
        ni = ieee80211vap_get_bssnode(vap);
        ASSERT(ni);
#if ATH_BAND_STEERING
        oldTxpow = ieee80211_node_get_txpower(ni);
#endif
        ieee80211node_set_txpower(ni, txpowlevel);
#if ATH_BAND_STEERING
        if (txpowlevel != oldTxpow) {
            ieee80211_bsteering_send_txpower_change_event(vap, txpowlevel);
        }
#endif
    }
}

#define RSSI_INV     0x80 /*invalid RSSI */

#define RSSI_DROP_INV(_curr_val, _new_val) (_new_val == RSSI_INV ? _curr_val: _new_val)

#define RSSI_CHAIN_STATS(ppdu, rx_chain)        do {        \
        (rx_chain).rx_rssi_pri20 = RSSI_DROP_INV((rx_chain).rx_rssi_pri20, ((ppdu) & RX_RSSI_CHAIN_PRI20_MASK)); \
        (rx_chain).rx_rssi_sec20 = RSSI_DROP_INV((rx_chain).rx_rssi_sec20, ((ppdu) & RX_RSSI_CHAIN_SEC20_MASK) >> RX_RSSI_CHAIN_SEC20_SHIFT) ; \
        (rx_chain).rx_rssi_sec40 = RSSI_DROP_INV((rx_chain).rx_rssi_sec40, ((ppdu) & RX_RSSI_CHAIN_SEC40_MASK) >> RX_RSSI_CHAIN_SEC40_SHIFT); \
        (rx_chain).rx_rssi_sec80 = RSSI_DROP_INV((rx_chain).rx_rssi_sec80, ((ppdu) & RX_RSSI_CHAIN_SEC80_MASK) >> RX_RSSI_CHAIN_SEC80_SHIFT); \
    } while ( 0 )


#define RSSI_CHAIN_PRI20(ppdu, rx_chain)       do { \
      rx_chain = RSSI_DROP_INV((rx_chain), ((ppdu) & HTT_T2H_EN_STATS_RSSI_PRI_20_M) >> HTT_T2H_EN_STATS_RSSI_PRI_20_S); \
    } while ( 0 )

static int
ol_update_peer_stats(struct ieee80211_node *ni,wmi_host_peer_stats *peer_stats) {
    struct ol_ath_softc_net80211 *scn = NULL;
    struct ieee80211com *ic = NULL;
    struct ieee80211vap *vap = NULL;
    struct ieee80211_mac_stats *mac_stats = NULL;
    int i = 0;
#if !ALL_POSSIBLE_RATES_SUPPORTED
    int nss, ch_width;
    struct ieee80211_bwnss_map nssmap = {0};
    u_int8_t tx_chainmask;
#endif

    if(ni) {
        vap = ni->ni_vap;
        ic = vap->iv_ic;
        scn = OL_ATH_SOFTC_NET80211(ic);

        mac_stats =  ( ni == vap->iv_bss ) ? &vap->iv_multicast_stats : &vap->iv_unicast_stats;

#ifdef ATH_SUPPORT_RDKB
        /* Firmware derived reported rssi from HW Rx descriptor's
         * rssi_comb which is known to not consider all
         * chains/bandwidths effectively lowering rssi for high data
         * rates.
         *
         * This knob fakes firmware reports with host maintained rssi
         * that does the right thing.
         */
        if (ic->ic_uniform_rssi) {
             peer_stats->peer_rssi = ATH_RSSI_OUT(ni->ni_rssi_ave);
        }
#endif
        ni->ni_rssi = peer_stats->peer_rssi;
        ni->ni_rssi_measured_time = OS_GET_TIMESTAMP();

        if (ic->ic_min_rssi_enable) {
            if (ni != ni->ni_bss_node && vap->iv_opmode == IEEE80211_M_HOSTAP) {
                /* compare the user provided rssi with peer rssi received */
                if (ni->ni_associd && ni->ni_rssi && (ic->ic_min_rssi > ni->ni_rssi)) {
                    /* send de-auth to ni_macaddr */
                    QDF_PRINT_INFO(ic->ic_print_idx, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO,  "Client %s(snr = %u) de-authed due to insufficient SNR\n",
                                   ether_sprintf(ni->ni_macaddr), ni->ni_rssi);
                    wlan_mlme_deauth_request(vap, ni->ni_macaddr, IEEE80211_REASON_UNSPECIFIED);
                    return -1;
                }
            }
        }
        if(ni->ni_rssi < ni->ni_rssi_min)
            ni->ni_rssi_min = ni->ni_rssi;
        else if (ni->ni_rssi > ni->ni_rssi_max)
            ni->ni_rssi_max = ni->ni_rssi;

        (OL_ATH_NODE_NET80211(ni))->an_ni_rx_rate = peer_stats->peer_rx_rate;
        (OL_ATH_NODE_NET80211(ni))->an_ni_tx_rate = peer_stats->peer_tx_rate;
        ni->ni_stats.ns_last_rx_rate = peer_stats->peer_rx_rate;
        ni->ni_stats.ns_last_tx_rate = peer_stats->peer_tx_rate;
        mac_stats->ims_last_tx_rate  = peer_stats->peer_tx_rate;

#if ALL_POSSIBLE_RATES_SUPPORTED
        mac_stats->ims_last_tx_rate_mcs = whal_kbps_to_mcs(mac_stats->ims_last_tx_rate, vap->iv_data_sgi, phymode_to_htflag(vap->iv_cur_mode));
#else
        nss = (vap->iv_nss >
                ieee80211_getstreams(ic, ic->ic_tx_chainmask)) ?
                ieee80211_getstreams(ic, ic->ic_tx_chainmask)  :
                vap->iv_nss;

        ch_width = wlan_get_param(vap, IEEE80211_CHWIDTH);

        if (ch_width == IEEE80211_CWM_WIDTH160 || ch_width == IEEE80211_CWM_WIDTH80_80) {
            tx_chainmask = ieee80211com_get_tx_chainmask(ic);
            ieee80211_get_bw_nss_mapping(vap, &nssmap, tx_chainmask);
            nss = nssmap.bw_nss_160;
        }

        mac_stats->ims_last_tx_rate_mcs = whal_kbps_to_mcs(mac_stats->ims_last_tx_rate,
                                                           vap->iv_data_sgi,
                                                           phymode_to_htflag(vap->iv_cur_mode),
                                                           nss,
                                                           ch_width);
#endif

#if ATH_SUPPORT_HYFI_ENHANCEMENTS
        (OL_ATH_NODE_NET80211(ni))->an_tx_cnt++;
        (OL_ATH_NODE_NET80211(ni))->an_tx_rates_used += peer_stats->peer_tx_rate/1000;
        (OL_ATH_NODE_NET80211(ni))->an_tx_bytes += peer_stats->txbytes;
        if (!ni->ni_ald.ald_txcount && peer_stats->totalsubframes) {
            /* No previous packet count to include in PER
               and some frames transmitted */
            ni->ni_ald.ald_lastper = peer_stats->currentper;
        } else if (peer_stats->totalsubframes) {
            /* Calculate a weighted average PER */
            ni->ni_ald.ald_lastper =
                ((peer_stats->currentper * peer_stats->totalsubframes +
                 ni->ni_ald.ald_lastper * ni->ni_ald.ald_txcount) /
                (peer_stats->totalsubframes + ni->ni_ald.ald_txcount));
        } else {
            /* Else there is no updated packet count - decrease by 25% */
            ni->ni_ald.ald_lastper = (ni->ni_ald.ald_lastper * 3) >> 2;
        }
	ni->ni_stats.ns_last_per = ni->ni_ald.ald_lastper;

        ni->ni_ald.ald_txcount +=  peer_stats->totalsubframes;
        (OL_ATH_NODE_NET80211(ni))->an_phy_err_cnt = scn->chan_stats.phy_err_cnt;

        ni->ni_ald.ald_max4msframelen += peer_stats->max4msframelen;
        (OL_ATH_NODE_NET80211(ni))->an_tx_ratecount += peer_stats->txratecount;

        ni->ni_ald.ald_retries += peer_stats->retries;
        scn->scn_stats.tx_retries += peer_stats->retries;

	ni->ni_ald.ald_ac_nobufs[0] += peer_stats->nobuffs[0];
	ni->ni_ald.ald_ac_nobufs[1] += peer_stats->nobuffs[1];
	ni->ni_ald.ald_ac_nobufs[2] += peer_stats->nobuffs[2];
	ni->ni_ald.ald_ac_nobufs[3] += peer_stats->nobuffs[3];
	ni->ni_ald.ald_ac_excretries[0] += peer_stats->excretries[0];
	ni->ni_ald.ald_ac_excretries[1] += peer_stats->excretries[1];
	ni->ni_ald.ald_ac_excretries[2] += peer_stats->excretries[2];
	ni->ni_ald.ald_ac_excretries[3] += peer_stats->excretries[3];
#endif
        for (i = 0; i < 4; i++) {
          ni->ni_stats.ns_excretries[i] += peer_stats->excretries[i];
        }
    }

    return 0;
}
/*
 * WMI event handler for periodic target stats event
 */
static int
ol_ath_update_stats_event_handler(ol_scn_t scn, u_int8_t *data,
                                    u_int16_t datalen)
{
    struct ieee80211com *ic;
    struct ieee80211_node *ni;
    wmi_host_stats_event *ev = (wmi_host_stats_event *)data;
    A_UINT8 i;
    u_int8_t c_macaddr[IEEE80211_ADDR_LEN];

    wmi_host_stats_event stats_param;

    ic = &scn->sc_ic;
    OS_MEMZERO(&stats_param, sizeof(wmi_host_stats_event));
    wmi_extract_stats_param(scn->wmi_handle, ev, &stats_param);
    if (stats_param.num_pdev_stats > 0)
    {
        wmi_host_pdev_stats st_pdev_stats;
        wmi_host_pdev_stats *pdev_stats = &st_pdev_stats;

        for (i = 0; i < stats_param.num_pdev_stats; i++)
        {
            wmi_extract_pdev_stats(scn->wmi_handle, data, i, pdev_stats);

            scn->chan_nf = pdev_stats->chan_nf;
            scn->mib_cycle_cnts.tx_frame_count = pdev_stats->tx_frame_count;
            scn->mib_cycle_cnts.rx_frame_count = pdev_stats->rx_frame_count;
            scn->mib_cycle_cnts.rx_clear_count = pdev_stats->rx_clear_count;
            scn->mib_cycle_cnts.rx_clear_ext_count = pdev_stats->rx_clear_count;
            scn->mib_cycle_cnts.rx_clear_ext40_count = pdev_stats->rx_clear_count;
            scn->mib_cycle_cnts.rx_clear_ext80_count = pdev_stats->rx_clear_count;
            scn->mib_cycle_cnts.cycle_count = pdev_stats->cycle_count;
            scn->chan_stats.chan_clr_cnt = pdev_stats->rx_clear_count;
            scn->chan_stats.cycle_cnt = pdev_stats->cycle_count;
            scn->chan_stats.phy_err_cnt = pdev_stats->phy_err_count;
            scn->chan_nf = pdev_stats->chan_nf;
            scn->chan_tx_pwr = pdev_stats->chan_tx_pwr;
            wlan_iterate_vap_list(ic, ol_ath_vap_iter_update_txpow,(void *) &scn->chan_tx_pwr);
            scn->scn_stats.ackRcvBad = pdev_stats->ackRcvBad;
            scn->scn_stats.rtsBad = pdev_stats->rtsBad;
            scn->scn_stats.rtsGood = pdev_stats->rtsGood;
            scn->scn_stats.fcsBad = pdev_stats->fcsBad;
            scn->scn_stats.noBeacons = pdev_stats->noBeacons;
            scn->scn_stats.mib_int_count = pdev_stats->mib_int_count;

           /* OS_MEMCPY(&scn->ath_stats,
                       &pdev_stats->pdev_stats,
                       sizeof(pdev_stats->pdev_stats));*/

#define scn_tx_stats scn->ath_stats.tx
#define ev_tx_stats pdev_stats->pdev_stats.tx

        /* Tx Stats */
        scn_tx_stats.comp_queued = ev_tx_stats.comp_queued;
        scn_tx_stats.comp_delivered = ev_tx_stats.comp_delivered;
        scn_tx_stats.msdu_enqued = ev_tx_stats.msdu_enqued;
        scn_tx_stats.mpdu_enqued = ev_tx_stats.mpdu_enqued;
        scn_tx_stats.wmm_drop = ev_tx_stats.wmm_drop;
        scn_tx_stats.local_enqued = ev_tx_stats.local_enqued;
        scn_tx_stats.local_freed = ev_tx_stats.local_freed;
        scn_tx_stats.hw_queued = ev_tx_stats.hw_queued;
        scn_tx_stats.hw_reaped = ev_tx_stats.hw_reaped;
        scn_tx_stats.underrun = ev_tx_stats.underrun;
        scn_tx_stats.hw_paused = ev_tx_stats.hw_paused;
        scn_tx_stats.tx_abort = ev_tx_stats.tx_abort;
        scn_tx_stats.mpdus_requed = ev_tx_stats.mpdus_requed;
        scn_tx_stats.tx_xretry = ev_tx_stats.tx_xretry;
        scn_tx_stats.data_rc = ev_tx_stats.data_rc;
        scn_tx_stats.self_triggers = ev_tx_stats.self_triggers;
        scn_tx_stats.sw_retry_failure = ev_tx_stats.sw_retry_failure;
        scn_tx_stats.illgl_rate_phy_err = ev_tx_stats.illgl_rate_phy_err;
        scn_tx_stats.pdev_cont_xretry = ev_tx_stats.pdev_cont_xretry;
        scn_tx_stats.pdev_tx_timeout = ev_tx_stats.pdev_tx_timeout;
        scn_tx_stats.pdev_resets = ev_tx_stats.pdev_resets;
        scn_tx_stats.stateless_tid_alloc_failure = ev_tx_stats.stateless_tid_alloc_failure;
        scn_tx_stats.phy_underrun = ev_tx_stats.phy_underrun;
        scn_tx_stats.txop_ovf = ev_tx_stats.txop_ovf;
        scn_tx_stats.seq_posted = ev_tx_stats.seq_posted;
        scn_tx_stats.seq_failed_queueing = ev_tx_stats.seq_failed_queueing;
        scn_tx_stats.seq_completed = ev_tx_stats.seq_completed;
        scn_tx_stats.seq_restarted = ev_tx_stats.seq_restarted;
        scn_tx_stats.mu_seq_posted = ev_tx_stats.mu_seq_posted;
        scn_tx_stats.mpdus_sw_flush = ev_tx_stats.mpdus_sw_flush;
        scn_tx_stats.mpdus_hw_filter = ev_tx_stats.mpdus_hw_filter;
        scn_tx_stats.mpdus_truncated = ev_tx_stats.mpdus_truncated;
        scn_tx_stats.mpdus_ack_failed = ev_tx_stats.mpdus_ack_failed;
        scn_tx_stats.mpdus_expired = ev_tx_stats.mpdus_expired;
        /* Only NON-TLV */
        scn_tx_stats.mc_drop = ev_tx_stats.mc_drop;

#define rx_stats scn->ath_stats.rx
#define ev_rx_stats pdev_stats->pdev_stats.rx

        /* Rx Stats */
        rx_stats.mid_ppdu_route_change = ev_rx_stats.mid_ppdu_route_change;
        rx_stats.status_rcvd = ev_rx_stats.status_rcvd;
        rx_stats.r0_frags = ev_rx_stats.r0_frags;
        rx_stats.r1_frags = ev_rx_stats.r1_frags;
        rx_stats.r2_frags = ev_rx_stats.r2_frags;
        rx_stats.htt_msdus = ev_rx_stats.htt_msdus;
        rx_stats.htt_mpdus = ev_rx_stats.htt_mpdus;
        rx_stats.loc_msdus = ev_rx_stats.loc_msdus;
        rx_stats.loc_mpdus = ev_rx_stats.loc_mpdus;
        rx_stats.oversize_amsdu = ev_rx_stats.oversize_amsdu;
        rx_stats.phy_errs = ev_rx_stats.phy_errs;
        rx_stats.phy_err_drop = ev_rx_stats.phy_err_drop;
        rx_stats.mpdu_errs = ev_rx_stats.mpdu_errs;
        rx_stats.pdev_rx_timeout = ev_rx_stats.pdev_rx_timeout;
        rx_stats.rx_ovfl_errs = ev_rx_stats.rx_ovfl_errs;

        /* mem stats */
        scn->ath_stats.mem.iram_free_size = pdev_stats->pdev_stats.mem.iram_free_size;
        scn->ath_stats.mem.dram_free_size = pdev_stats->pdev_stats.mem.dram_free_size;
        /* Only Non-TLV */
        scn->ath_stats.mem.sram_free_size = pdev_stats->pdev_stats.mem.sram_free_size;

        /* Peer stats */
            scn->scn_stats.rx_phyerr = scn->ath_stats.rx.phy_errs;
            scn->scn_stats.rx_mgmt = scn->ath_stats.rx.loc_msdus;
            scn->scn_stats.tx_mgmt = scn->ath_stats.tx.local_enqued;
        }
    }

    if (stats_param.num_pdev_ext_stats > 0) {
            wmi_host_pdev_ext_stats st_pdev_ext_stats;
            wmi_host_pdev_ext_stats *pdev_ext_stats = &st_pdev_ext_stats;
            i = 0;

            wmi_extract_pdev_ext_stats(scn->wmi_handle, data, i, pdev_ext_stats);

            OS_MEMCPY(scn->scn_stats.rx_mcs,  pdev_ext_stats->rx_mcs, sizeof(scn->scn_stats.rx_mcs));
            scn->scn_stats.rx_rssi_comb = (pdev_ext_stats->rx_rssi_comb & RX_RSSI_COMB_MASK);
            /* rssi of separate chains */
            RSSI_CHAIN_STATS(pdev_ext_stats->rx_rssi_chain0, scn->scn_stats.rx_rssi_chain0);
            RSSI_CHAIN_STATS(pdev_ext_stats->rx_rssi_chain1, scn->scn_stats.rx_rssi_chain1);
            RSSI_CHAIN_STATS(pdev_ext_stats->rx_rssi_chain2, scn->scn_stats.rx_rssi_chain2);
            RSSI_CHAIN_STATS(pdev_ext_stats->rx_rssi_chain3, scn->scn_stats.rx_rssi_chain3);
            OS_MEMCPY(scn->scn_stats.tx_mcs,  pdev_ext_stats->tx_mcs, sizeof(scn->scn_stats.tx_mcs));
            scn->scn_stats.tx_rssi= pdev_ext_stats->ack_rssi;
    }

    if (stats_param.num_vdev_stats > 0) {
          for (i = 0; i < stats_param.num_vdev_stats; i++) {
            struct ieee80211vap *vap;
            wmi_host_vdev_stats vdev_stats;
            struct ol_ath_vap_net80211 *avn;

			wmi_extract_vdev_stats(scn->wmi_handle, data, i, &vdev_stats);

            vap = ol_ath_vap_get(scn, vdev_stats.vdev_id);
            if (vap) {
                avn = OL_ATH_VAP_NET80211(vap);
                OS_MEMCPY(&avn->vdev_stats, &vdev_stats, sizeof(wmi_host_vdev_stats));
            }
        }
    }

    if (stats_param.num_peer_stats > 0)
    {
#if ATH_BAND_STEERING
        /* Initialize structure for STA stats */
        struct bs_sta_stats_ind sta_stats;
        struct ieee80211vap *current_vap = NULL;
        sta_stats.peer_count = 0;
#endif
        ic = &scn->sc_ic;
        for (i = 0; i < stats_param.num_peer_stats; i++)
        {
            wmi_host_peer_stats st_peer_stats;
            wmi_host_peer_stats *peer_stats = &st_peer_stats;

            wmi_extract_peer_stats(scn->wmi_handle, data, i, peer_stats);

            WMI_HOST_MAC_ADDR_TO_CHAR_ARRAY(&peer_stats->peer_macaddr, c_macaddr);
            ni = ieee80211_find_node(&ic->ic_sta, c_macaddr);
#if ATH_SUPPORT_WRAP
            if(ni && ni->ni_vap && wlan_is_psta(ni->ni_vap)) {
                struct ieee80211vap *tmpvap = NULL;
                struct ieee80211_node *tmp_bss_ni = NULL;

                if(!peer_stats->peer_rssi) {
                    ieee80211_free_node(ni);
                    //temp += sizeof(wmi_peer_stats);
                    continue;
                }

                TAILQ_FOREACH(tmpvap, &ic->ic_vaps, iv_next) {
                    if((ni->ni_vap != tmpvap) && wlan_is_psta(tmpvap)) {
                        tmp_bss_ni = ieee80211_ref_bss_node(tmpvap);

                        if(tmp_bss_ni) {
                            ol_update_peer_stats(tmp_bss_ni,peer_stats);
                            ieee80211_free_node(tmp_bss_ni);
                            tmp_bss_ni = NULL;
                        }
                    }
                }
            }
#endif

            if (ni) {
                if(ol_update_peer_stats(ni,peer_stats)) {
                    ieee80211_free_node(ni);
                    //temp += sizeof(wmi_peer_stats);
                    continue;
                }
#if ATH_BAND_STEERING
                /* Only do band steering updates if this node is not for a BSS,
                   only for peers */
                if (memcmp(&ni->ni_macaddr[0],
                           &ni->ni_bssid[0], IEEE80211_ADDR_LEN)) {
                    bool rssi_changed = false;
                    if (scn->is_ar900b) {
                        if (peer_stats->peer_rssi &&
                            peer_stats->peer_rssi_seq_num != ni->ni_rssi_seq) {
                            /* New RSSI measurement */
                            ieee80211_bsteering_record_rssi(ni, peer_stats->peer_rssi);
                            ni->ni_rssi_seq = peer_stats->peer_rssi_seq_num;
                            rssi_changed = true;
                        }
                    } else {
                        if (peer_stats->peer_rssi && peer_stats->peer_rssi_changed) {
                            /* New RSSI measurement */
                            ieee80211_bsteering_record_rssi(ni, peer_stats->peer_rssi);
                            rssi_changed = true;
                        }
                    }
                    if (peer_stats->peer_tx_rate &&
                        (peer_stats->peer_tx_rate != ni->ni_stats.ns_last_tx_rate)) {
                        /* Tx rate has changed */
                        ieee80211_bsteering_update_rate(ni, peer_stats->peer_tx_rate);
                    }

                    /* Only need to send a STA stats update for this peer if the RSSI changed
                       and the Tx rate is valid */
                    if (rssi_changed && peer_stats->peer_tx_rate) {
                        if (ieee80211_bsteering_update_sta_stats(
                                ni, current_vap, &sta_stats)) {
                            current_vap = ni->ni_vap;
                        }
                    }
                }
#endif

                ieee80211_free_node(ni);
            }
        }

#if ATH_BAND_STEERING
        if (current_vap) {
            ieee80211_bsteering_send_sta_stats(current_vap, &sta_stats);
        }
#endif
    }
    /*Go to the end of buffer after bcnfilter*/
//    temp += (ev->num_bcnflt_stats * sizeof(wmi_bcnfilter_stats_t));
    if (stats_param.stats_id & WMI_HOST_REQUEST_PEER_EXTD_STAT)
    {
        if(stats_param.num_peer_stats  > 0)
        {
            ic = &scn->sc_ic;
            for (i = 0; i < stats_param.num_peer_stats; i++)
            {
                wmi_host_peer_extd_stats st_peer_extd_stats;
                wmi_host_peer_extd_stats *peer_extd_stats = &st_peer_extd_stats;

                wmi_extract_peer_extd_stats(scn->wmi_handle, data, i, peer_extd_stats);

                WMI_HOST_MAC_ADDR_TO_CHAR_ARRAY(&peer_extd_stats->peer_macaddr, c_macaddr);

                ni = ieee80211_find_node(&ic->ic_sta, c_macaddr);
                if (ni) {
                    ni->ni_stats.inactive_time = peer_extd_stats->inactive_time;
                    ni->ni_peer_chain_rssi = peer_extd_stats->peer_chain_rssi;
                    ni->ni_stats.ns_dot11_tx_bytes = peer_extd_stats->peer_tx_bytes;
                    ni->ni_stats.ns_dot11_rx_bytes = peer_extd_stats->peer_rx_bytes;

                    if (peer_extd_stats->last_tx_rate_code) {
                        (OL_ATH_NODE_NET80211(ni))->an_ni_tx_ratecode =
                                        peer_extd_stats->last_tx_rate_code & 0xff;
                        (OL_ATH_NODE_NET80211(ni))->an_ni_tx_flags =
                                        ((peer_extd_stats->last_tx_rate_code >> 8) & 0xff);
                    }
                    if (peer_extd_stats->last_tx_power) {
                        (OL_ATH_NODE_NET80211(ni))->an_ni_tx_power =
                                        (A_UINT8)peer_extd_stats->last_tx_power;
                    }
                    if (WMI_PEER_EXTD_STATS_SGI_CONFIG_GET(peer_extd_stats->sgi_count)) {
                        (OL_ATH_NODE_NET80211(ni))->an_ni_tx_sgi_count =
                                        WMI_PEER_EXTD_STATS_SGI_COUNT_GET(peer_extd_stats->sgi_count);
                    }
#if QCA_AIRTIME_FAIRNESS
                    if (ic->atf_mode) {
                        (OL_ATH_NODE_NET80211(ni))->an_ni_atf_token_allocated =
                                        (A_UINT16)peer_extd_stats->atf_tokens_allocated;
                        (OL_ATH_NODE_NET80211(ni))->an_ni_atf_token_utilized =
                                        (A_UINT16)peer_extd_stats->atf_tokens_utilized;
                    }
#endif
                    ieee80211_free_node(ni);
                }
//                temp += sizeof(wmi_peer_extd_stats);
            }
        }
    }
    if (stats_param.stats_id & WMI_HOST_REQUEST_VDEV_EXTD_STAT)
    {
        for (i = 0; i < stats_param.num_vdev_stats; i++) {
            struct ieee80211vap *vap;
            wmi_host_vdev_extd_stats vdev_extd_stats;
            struct ol_ath_vap_net80211 *avn;

			wmi_extract_vdev_extd_stats(scn->wmi_handle, data, i, &vdev_extd_stats);

            vap = ol_ath_vap_get(scn, vdev_extd_stats.vdev_id);
            if (vap) {
                avn = OL_ATH_VAP_NET80211(vap);
                OS_MEMCPY(&avn->vdev_extd_stats, &vdev_extd_stats, sizeof(wmi_host_vdev_extd_stats));
            }
        }
    }
/*
    Extension stats extract API needs to be added - TBD
    if(ev->stats_id & WMI_REQUEST_PDEV_EXT2_STAT) {
       wmi_pdev_ext2_stats *pdev_ext2_stats =  (wmi_pdev_ext2_stats *)temp;
       scn->chan_nf_sec80 = pdev_ext2_stats->chan_nf_sec80;
       temp += sizeof(wmi_pdev_ext2_stats);
    }
*/
#if ATH_SUPPORT_NAC_RSSI
    if (stats_param.stats_id & WMI_HOST_REQUEST_NAC_RSSI)
    {
        struct ieee80211vap *vap = 0;
        char nullmac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        wmi_host_vdev_nac_rssi_event nac_rssi_stats;
        wmi_extract_vdev_nac_rssi_stats(scn->wmi_handle, data,  &nac_rssi_stats);
        vap = ol_ath_vap_get(scn, nac_rssi_stats.vdev_id);
        if (vap) {
            if ((IEEE80211_ADDR_EQ(vap->iv_nac_rssi.bssid_mac, nullmac)) && (IEEE80211_ADDR_EQ(vap->iv_nac_rssi.bssid_mac, nullmac)))
            {
                qdf_print("Extract NAC_RSSI Wrong VAP%d\n", nac_rssi_stats.vdev_id);
                return 0;
            }
            if (nac_rssi_stats.avg_rssi != 0)
            {
                vap->iv_nac_rssi.client_rssi = nac_rssi_stats.avg_rssi;
                vap->iv_nac_rssi.client_rssi_valid = (nac_rssi_stats.rssi_seq_num == 0) ? 0 : 1;
            }
        }
    }
#endif

    return 0;
}

static void
ol_ath_net80211_get_cur_chan_stats(struct ieee80211com *ic, struct ieee80211_chan_stats *chan_stats)
{
    struct ol_ath_softc_net80211 *scn = OL_ATH_SOFTC_NET80211(ic);
    chan_stats->chan_clr_cnt = scn->chan_stats.chan_clr_cnt;
    chan_stats->cycle_cnt = scn->chan_stats.cycle_cnt;
    chan_stats->phy_err_cnt = scn->chan_stats.phy_err_cnt;
}

static int
ol_ath_bss_chan_info_event_handler(ol_scn_t scn, u_int8_t *data, u_int16_t datalen)
{
    wmi_host_pdev_bss_chan_info_event bss_chan_info;
    wmi_host_pdev_bss_chan_info_event *chan_stats = &bss_chan_info;

    struct hw_params {
        char name[10];
        uint64_t channel_counters_freq;
    };

    static const struct hw_params hw_params_list[] = {
        {
            .name = "ar9888",
            .channel_counters_freq = 88000,
        },
        {
            .name = "ipq4019",
            .channel_counters_freq = 125000,
        },
        {
            .name = "qca9984",
            .channel_counters_freq = 150000,
        },
        {
            .name = "qca9888",
            .channel_counters_freq = 150000,
        },
        {
            .name = "ar900b",
            .channel_counters_freq = 150000,
        },
    };

    const struct hw_params *target;

	wmi_extract_bss_chan_info_event(scn->wmi_handle, data, &bss_chan_info);

    if(scn->target_type == TARGET_TYPE_AR9888)
            target = &hw_params_list[0];
    else if(scn->target_type == TARGET_TYPE_IPQ4019)
            target = &hw_params_list[1];
    else if(scn->target_type == TARGET_TYPE_QCA9984)
            target = &hw_params_list[2];
    else if(scn->target_type == TARGET_TYPE_QCA9888)
            target = &hw_params_list[3];
    else {
            qdf_print("Error: not enabled for target type\n");
            return 0;
        }

    if(chan_stats && !scn->tx_rx_time_info_flag) {
        qdf_print("BSS Chan info stats :\n");
        qdf_print("Frequency               : %d\n",chan_stats->freq);
        qdf_print("noise_floor             : %d\n",chan_stats->noise_floor);
        qdf_print("rx_clear_count_low      : %u\n",chan_stats->rx_clear_count_low);
        qdf_print("rx_clear_count_high     : %u\n",chan_stats->rx_clear_count_high);
        qdf_print("cycle_count_low         : %u\n",chan_stats->cycle_count_low);
        qdf_print("cycle_count_high        : %u\n",chan_stats->cycle_count_high);
        qdf_print("tx_cycle_count_low      : %u\n",chan_stats->tx_cycle_count_low);
        qdf_print("tx_cycle_count_high     : %u\n",chan_stats->tx_cycle_count_high);
        qdf_print("rx_cycle_count_low      : %u\n",chan_stats->rx_cycle_count_low);
        qdf_print("rx_cycle_count_high     : %u\n",chan_stats->rx_cycle_count_high);
        qdf_print("rx_bss_cycle_count_low  : %u\n",chan_stats->rx_bss_cycle_count_low);
        qdf_print("rx_bss_cycle_count_high : %u\n",chan_stats->rx_bss_cycle_count_high);
    }
    else if(chan_stats && scn->tx_rx_time_info_flag) {
        uint64_t tx_cycle_count = (uint64_t) chan_stats->tx_cycle_count_high << 32 |
            chan_stats->tx_cycle_count_low;
        uint64_t rx_cycle_count = (uint64_t) chan_stats->rx_cycle_count_high << 32 |
            chan_stats->rx_cycle_count_low;
        uint64_t rx_bss_cycle_count = (uint64_t) chan_stats->rx_bss_cycle_count_high << 32 |
            chan_stats->rx_bss_cycle_count_low;
        uint64_t rx_outside_bss_cycle_count = rx_cycle_count - rx_bss_cycle_count;
        uint64_t cycle_count = (uint64_t) chan_stats->cycle_count_high << 32 |
            chan_stats->cycle_count_low;
        uint64_t tx_duration = div_u64(tx_cycle_count, target->channel_counters_freq);
        uint64_t rx_duration = div_u64(rx_cycle_count, target->channel_counters_freq);
        uint64_t rx_in_bss_duration = div_u64(rx_bss_cycle_count,
            target->channel_counters_freq);
        uint64_t rx_outside_bss_dur = div_u64(rx_outside_bss_cycle_count,
            target->channel_counters_freq);
        uint64_t total_duration = div_u64(cycle_count, target->channel_counters_freq);

        qdf_print("tx rx info stats :\n");

        qdf_print("tx cycle count                : %llu\n", tx_cycle_count);
        qdf_print("rx cycle count                : %llu\n", rx_cycle_count);
        qdf_print("rx inside bss cycle count     : %llu\n", rx_bss_cycle_count);
        qdf_print("rx outside bss cycle count    : %llu\n\n", rx_outside_bss_cycle_count);

        qdf_print("tx duration(msec)             : %llu\n", tx_duration);
        qdf_print("rx duration(msec)             : %llu\n", rx_duration);
        qdf_print("rx inside bss duration(msec)  : %llu\n", rx_in_bss_duration);
        qdf_print("rx outside bss duration(msec) : %llu\n", rx_outside_bss_dur);
        qdf_print("total duration(msec)          : %llu\n\n", total_duration);

        qdf_print("tx duration(%%)               : %llu\n", div_u64(tx_duration*100,
            total_duration));
        qdf_print("rx duration(%%)               : %llu\n", div_u64(rx_duration*100,
            total_duration));
        qdf_print("rx inside bss duration(%%)    : %llu\n",
            div_u64(rx_in_bss_duration*100, total_duration));
        qdf_print("rx outside bss duration(%%)   : %llu\n",
            div_u64(rx_outside_bss_dur*100, total_duration));
        scn->tx_rx_time_info_flag = 0;
    }
    return 0;
}

/*
 * stats_id is a bitmap of wmi_stats_id- pdev/vdev/peer
 */

static int
ol_ath_rssi_cb(struct ol_ath_softc_net80211 *scn,
                u_int8_t *data,
                u_int16_t datalen)
{
    struct ieee80211com *ic;
    struct ieee80211_node *ni;

    wmi_host_inst_stats_resp inst_rssi_resp;
    wmi_host_inst_stats_resp *ev = &inst_rssi_resp;
    u_int8_t c_macaddr[IEEE80211_ADDR_LEN];

    wmi_extract_inst_rssi_stats_event(scn->wmi_handle, data, &inst_rssi_resp);
    ic = &scn->sc_ic;
    WMI_HOST_MAC_ADDR_TO_CHAR_ARRAY(&ev->peer_macaddr, c_macaddr);
    ni = ieee80211_find_node(&ic->ic_sta, c_macaddr);
    if (ni) {
#if ATH_BAND_STEERING
#define WMI_INST_STATS_VALID_RSSI_MAX 127
        /* 0x80 will be reported as invalid RSSI by hardware,
           so we want to make sure the RSSI reported to band steering
           is valid, i.e. in the range of (0, 127]. */
        if (ev->iRSSI > WMI_HOST_INST_STATS_INVALID_RSSI &&
            ev->iRSSI <= WMI_INST_STATS_VALID_RSSI_MAX) {
            ieee80211_bsteering_record_inst_rssi(ni, ev->iRSSI);
        } else {
            ieee80211_bsteering_record_inst_rssi_err(ni);
        }
#undef WMI_INST_STATS_VALID_RSSI_MAX
#endif
        ieee80211_update_ack_rssi(ni, ev->iRSSI);
        ieee80211_free_node(ni);
    }
    return 0;
}

int32_t
ol_ath_request_stats(struct ieee80211com *ic,
                               void *cmd_param)
{
    /*This API will be deprecated. This is currently not used
     *And it expects caller to pack stats param, which is incorrect.
     *If this API still needs to be supported, then it should follow
     *different mechanism to send the message.
     */
#if 0
    struct ol_ath_softc_net80211 *scn = OL_ATH_SOFTC_NET80211(ic);
    wmi_buf_t buf;
    wmi_request_stats_cmd *cmd_buf, *cmd;
    u_int8_t len = sizeof(wmi_request_stats_cmd);
    cmd = (wmi_request_stats_cmd *)cmd_param;
    buf = wmi_buf_alloc(scn->wmi_handle, len);
    if (!buf) {
        qdf_print("%s:wmi_buf_alloc failed\n", __FUNCTION__);
        return EINVAL;
    }
    cmd_buf = (wmi_request_stats_cmd *)wmi_buf_data(buf);
    qdf_mem_copy(cmd_buf, cmd, sizeof(wmi_request_stats_cmd));

    if (wmi_unified_cmd_send(scn->wmi_handle, buf, len,
        WMI_REQUEST_STATS_CMDID)) {
        return EINVAL;
    }
#endif
    qdf_print("%s:NOTE -- THIS API WILL BE DEPRECATED\n", __func__);
    return 0;
}
EXPORT_SYMBOL(ol_ath_request_stats);

static int32_t
ol_ath_send_rssi(struct ieee80211com *ic,
                u_int8_t *macaddr, struct ieee80211vap *vap)
{
    struct ol_ath_softc_net80211 *scn = OL_ATH_SOFTC_NET80211(ic);
    struct stats_request_params param = {0};

    param.vdev_id = (OL_ATH_VAP_NET80211(vap))->av_if_id;
    param.stats_id = WMI_HOST_REQUEST_INST_STAT;

    return wmi_unified_stats_request_send(scn->wmi_handle, macaddr, &param);
}

static int32_t
ol_ath_bss_chan_info_request_stats(struct ieee80211com *ic, int param)
{
    struct ol_ath_softc_net80211 *scn = OL_ATH_SOFTC_NET80211(ic);
    struct bss_chan_info_request_params chan_info_param;

    qdf_mem_set(&chan_info_param, sizeof(chan_info_param), 0);
    chan_info_param.param = param;

    return wmi_unified_bss_chan_info_request_cmd_send(scn->wmi_handle,
            &chan_info_param);

    return 0;
}

void
reset_node_stat(void *arg, wlan_node_t node)
{
    struct ieee80211_node *ni = node;
    OS_MEMZERO(&(ni->ni_stats), sizeof(struct ieee80211_nodestats ));
}

void
reset_vap_stat(void *arg , struct ieee80211vap *vap)
{
    wlan_iterate_station_list(vap, reset_node_stat, NULL);
    OS_MEMZERO(&(vap->iv_stats), sizeof(struct ieee80211_stats ));
    OS_MEMZERO(&(vap->iv_unicast_stats), sizeof(struct ieee80211_mac_stats ));
    OS_MEMZERO(&(vap->iv_multicast_stats), sizeof(struct ieee80211_mac_stats ));
    return;
}

void
ol_ath_reset_vap_stat(struct ieee80211com *ic)
{
     wlan_iterate_vap_list(ic, reset_vap_stat, NULL);
}

A_STATUS
process_rx_stats(void *pdev, qdf_nbuf_t amsdu,
                uint16_t peer_id,
                enum htt_rx_status status)
{

    int is_mcast;
    void *rx_desc;
    qdf_nbuf_t msdu;
    struct ol_txrx_pdev_t *txrx_pdev =  (struct ol_txrx_pdev_t *)pdev;
    struct ieee80211vap *vap;
    struct ieee80211_mac_stats *mac_stats;
    struct ieee80211_node *ni;
    struct ol_ath_softc_net80211 *scn = (struct ol_ath_softc_net80211 *)txrx_pdev->ctrl_pdev;
    struct ol_txrx_vdev_t *vdev = NULL;
    struct ol_txrx_peer_t *peer;
    uint32_t data_length = 0;
    char *hdr_des;
    uint32_t attn;
    struct ieee80211_frame *wh;
    uint32_t *ppdu;
    uint32_t ratekbps;
    uint8_t rate_code = 0, nss=0, bw=0, sgi = 0, mcs = 0, l_sig_rate_select,l_sig_rate;
    uint8_t preamble_type, rate_flags = 0;
#ifdef QCA_NSS_WIFI_OFFLOAD_SUPPORT
    int is_bcast;
    struct ethhdr * eh;
#endif

    if (!amsdu) {
        qdf_print("Invalid data in %s\n", __func__);
        return A_ERROR;
    }

    peer = (peer_id == HTT_INVALID_PEER) ?
               NULL : txrx_pdev->peer_id_to_obj_map[peer_id];

    rx_desc = htt_rx_msdu_desc_retrieve(txrx_pdev->htt_pdev,
                                                        amsdu);

    switch (status) {
    case HTT_RX_IND_MPDU_STATUS_OK:
        if (peer) {
            vdev = peer->vdev;
            vap = ol_ath_getvap(vdev);
            if (!vap) {
                return A_ERROR;
            }
            ni = ieee80211_vap_find_node(vap,peer->mac_addr.raw);
            if (!ni) {
                   return A_ERROR;
                }
            /* remove extra node ref count added by find_node above */

            if (!pdev) {
                qdf_print("Invalid pdev in %s\n",
                                __func__);
                return A_ERROR;
            }

            if (!amsdu) {
                qdf_print("Invalid data in %s\n",
                                __func__);
                return A_ERROR;
            }

            ppdu = txrx_pdev->htt_pdev->ar_rx_ops->get_ppdu_start(rx_desc);
            if (ppdu) {
            preamble_type = (ppdu[RX_D_PREAMBLE_OFFSET] & PREAMBLE_MASK) >> PREAMBLE_SHIFT;
            switch(preamble_type)
            {
                case 4:
                    l_sig_rate_select = (ppdu[RX_D_PREAMBLE_OFFSET] & RX_D_MASK_LSIG_SEL) ; /* Bit 4 */
                    l_sig_rate = (ppdu[RX_D_PREAMBLE_OFFSET]  & RX_D_MASK_LSIG_RATE); /* Bit[3:0] */
                    rate_flags = 0x00;    /* bw is HT20 and sgi 0 for legacy rates */
                    if(l_sig_rate_select) { /* CCK */
                        switch(l_sig_rate) {
                            case 0xb:
                            case 0x1:
                                rate_code=0x43;
                                break;
                            case 0xa:
                            case 0xe:
                            case 0x2:
                            case 0x5:
                                rate_code=0x42;
                                break;
                            case 0x9:
                            case 0xd:
                            case 0x3:
                            case 0x6:
                                rate_code=0x41;
                                break;
                            case 0x8:
                            case 0xc:
                            case 0x4:
                            case 0x7:
                                rate_code=0x40;
                                break;
                        }
                    } else { /* OFDM, l_sig_rate_select==0 */
                        switch(l_sig_rate) {
                            case 0x8:
                                rate_code=0x00;
                                break;
                            case 0x9:
                                rate_code=0x01;
                                break;
                            case 0xa:
                                rate_code=0x02;
                                break;
                            case 0xb:
                                rate_code=0x03;
                                break;
                            case 0xc:
                                rate_code=0x04;
                                break;
                            case 0xd:
                                rate_code=0x05;
                                break;
                            case 0xe:
                                rate_code=0x06;
                                break;
                            case 0xf:
                                rate_code=0x07;
                                break;
                        }
                    }
                    break;
                    /* HT */
                case 8: /* HT w/o TxBF */
                case 9:/* HT w/ TxBF */
                    mcs = (u_int8_t)(ppdu[RX_D_HT_RATE_OFFSET] & RX_D_HT_MCS_MASK);
                    nss = mcs>>3;
                    mcs %= 8;
                    bw  = (u_int8_t)((ppdu[RX_D_HT_RATE_OFFSET] >> 7) & 1);
                    sgi = (u_int8_t)((ppdu[RX_D_VHT_RATE_OFFSET] >> 7) & 1);
                    rate_flags = bw << 5 | sgi << 3;
                    rate_code = AR600P_ASSEMBLE_HW_RATECODE(mcs, nss, AR600P_HW_RATECODE_PREAM_HT);
                    break;
                    /* VHT */
                case 0x0c: /* VHT w/o TxBF */
                case 0x0d: /* VHT w/ TxBF */
                    mcs = (u_int8_t)((ppdu[RX_D_VHT_RATE_OFFSET] >> 4) & RX_D_VHT_MCS_MASK);
                    nss = (u_int8_t)((ppdu[RX_D_HT_RATE_OFFSET] >> 10) & RX_D_VHT_NSS_MASK);
                    bw  = (u_int8_t)((ppdu[RX_D_HT_RATE_OFFSET] & RX_D_VHT_BW_MASK));
                    sgi = (u_int8_t)((ppdu[RX_D_VHT_RATE_OFFSET]) & RX_D_VHT_SGI_MASK);
                    if (bw == 0x3)
                    rate_flags = 0x80 | sgi << 3;
                    else
                    rate_flags = bw << 5 | sgi << 3;
                    rate_code = AR600P_ASSEMBLE_HW_RATECODE(mcs, nss, AR600P_HW_RATECODE_PREAM_VHT);
                    break;
                default:
                    break;
            }
            }

            OL_ATH_NODE_NET80211(ni)->an_ni_rx_flags = rate_flags;
            OL_ATH_NODE_NET80211(ni)->an_ni_rx_ratecode = rate_code;

            ratekbps = whal_ratecode_to_kbps(rate_code, bw, sgi);
            if (ratekbps)
                OL_ATH_RATE_LPF((OL_ATH_NODE_NET80211(ni))->an_ni_avg_rx_rate, ratekbps);
            ni->ni_stats.ns_ppdu_rx_rate = OL_ATH_RATE_OUT((OL_ATH_NODE_NET80211(ni))->an_ni_avg_rx_rate);

            msdu = amsdu;
            while (msdu) {
                rx_desc = htt_rx_msdu_desc_retrieve(txrx_pdev->htt_pdev,
                                                                    msdu);
#ifdef QCA_NSS_WIFI_OFFLOAD_SUPPORT
                eh = (struct ethhdr *)qdf_nbuf_data(msdu);
                if (txrx_pdev->nss_wifiol_ctx) {
                    is_mcast = IEEE80211_IS_MULTICAST(eh->h_dest) ? 1 : 0;
                }
                else
#endif
                {

                    /*  Here the mcast packets are decided on the basis that
                        the target sets "only" the forward bit for mcast packets.
                        If it is a normal packet then "both" the forward bit and
                        the discard bit is set. Forward bit indicates that the
                        packet needs to be forwarded and the discard bit
                        indicates that the packet should not be delivered to
                        the OS.*/
                    is_mcast =  ((htt_rx_msdu_forward(txrx_pdev->htt_pdev,
                                    rx_desc)) &&
                            !htt_rx_msdu_discard(txrx_pdev->htt_pdev,
                                rx_desc));
                }
                mac_stats = is_mcast ? &vap->iv_multicast_stats :
                                            &vap->iv_unicast_stats;
                data_length = RXDESC_GET_DATA_LEN(rx_desc);
                /*  Updating peer stats */
                ni->ni_stats.ns_rx_data++;
                ni->ni_stats.ns_rx_bytes += data_length;
#if UMAC_SUPPORT_STA_STATS_ENHANCEMENT
                if (is_mcast) {
                    ni->ni_stats.ns_rx_mcast++;
                    ni->ni_stats.ns_rx_mcast_bytes += data_length;
                } else {
                    ni->ni_stats.ns_rx_ucast++;
                    ni->ni_stats.ns_rx_ucast_bytes += data_length;
                }
#endif
                /*  Updating vap stats  */
                mac_stats->ims_rx_data_packets++;
                mac_stats->ims_rx_data_bytes += data_length;
                mac_stats->ims_rx_datapyld_bytes += (data_length -
                                                        ETHERNET_HDR_LEN);

                mac_stats->ims_rx_packets++;

                mac_stats->ims_rx_bytes += data_length;
                scn->scn_stats.rx_packets++;
                scn->scn_stats.rx_num_data++;
                scn->scn_stats.rx_bytes += data_length;
                scn->scn_stats.rx_data_bytes += data_length;

#ifdef QCA_NSS_WIFI_OFFLOAD_SUPPORT
                if (txrx_pdev->nss_wifiol_ctx) {
                    is_bcast = IEEE80211_IS_BROADCAST(eh->h_dest);
                    if (is_bcast) {
                        mac_stats = &vap->iv_multicast_stats;
                        mac_stats->ims_rx_bcast_data_packets++;
                    }
                }
#endif
                msdu = qdf_nbuf_next(msdu);
            }

            ieee80211_free_node(ni);
            scn->scn_stats.rx_aggr = txrx_pdev->stats.priv.rx.normal.rx_aggr;

        }
        break;
    case HTT_RX_IND_MPDU_STATUS_ERR_FCS:
        rx_desc = htt_rx_msdu_desc_retrieve(txrx_pdev->htt_pdev,
                                                        amsdu);
        data_length = RXDESC_GET_DATA_LEN(rx_desc);

       /*
        * Make data_length = 0, because an invalid FCS Error data length is received.
        * We ignore this data length as it updates the Rx bytes stats in wrong way.
        */
        data_length = 0;

        scn->scn_stats.rx_bytes += data_length;
        scn->scn_stats.rx_num_data++;
        scn->scn_stats.rx_data_bytes += data_length;
        scn->scn_stats.rx_crcerr++;
        scn->scn_stats.rx_packets++;
	if (peer ) {
		vdev = peer->vdev;
		if (vdev == NULL ) {
			return A_ERROR;
		}
		vap = ol_ath_getvap(vdev);
		is_mcast =  ((htt_rx_msdu_forward(txrx_pdev->htt_pdev, rx_desc))
				&&
				!htt_rx_msdu_discard(txrx_pdev->htt_pdev, rx_desc));
		if (vap !=NULL) {
			mac_stats = is_mcast ? &vap->iv_multicast_stats :
				&vap->iv_unicast_stats;
			mac_stats->ims_rx_fcserr++;
		}
	}
	break;
    case HTT_RX_IND_MPDU_STATUS_TKIP_MIC_ERR:
        if (peer) {
            vdev = peer->vdev;
            vap = ol_ath_getvap(vdev);
            if (!vap) {
                return A_ERROR;
            }
            rx_desc = htt_rx_msdu_desc_retrieve(txrx_pdev->htt_pdev, amsdu);
            data_length = RXDESC_GET_DATA_LEN(rx_desc);
            scn->scn_stats.rx_bytes += data_length;

            is_mcast =  ((htt_rx_msdu_forward(txrx_pdev->htt_pdev, rx_desc))
                        &&
                        !htt_rx_msdu_discard(txrx_pdev->htt_pdev, rx_desc));
            mac_stats = is_mcast ? &vap->iv_multicast_stats :
                                    &vap->iv_unicast_stats;

            mac_stats->ims_rx_tkipmic++;
            scn->scn_stats.rx_badmic++;
            ni = ieee80211_vap_find_node(vap, peer->mac_addr.raw);
            if (!ni) {
               return A_ERROR;
            }
            ni->ni_stats.ns_rx_tkipmic++;
            /* remove extra node ref count added by find_node above */
            ieee80211_free_node(ni);

        }
        scn->scn_stats.rx_packets++;
        break;
    case HTT_RX_IND_MPDU_STATUS_DECRYPT_ERR:
        if (peer) {
            vdev = peer->vdev;
            vap = ol_ath_getvap(vdev);
            if (!vap) {
                return A_ERROR;
            }
            rx_desc = htt_rx_msdu_desc_retrieve(txrx_pdev->htt_pdev, amsdu);
            data_length = RXDESC_GET_DATA_LEN(rx_desc);
            scn->scn_stats.rx_bytes += data_length;
            is_mcast =  ((htt_rx_msdu_forward(txrx_pdev->htt_pdev, rx_desc))
                        &&
                        !htt_rx_msdu_discard(txrx_pdev->htt_pdev, rx_desc));
            mac_stats = is_mcast ? &vap->iv_multicast_stats :
                                    &vap->iv_unicast_stats;

            mac_stats->ims_rx_decryptcrc++;
            scn->scn_stats.rx_badcrypt++;
            ni = ieee80211_vap_find_node(vap, peer->mac_addr.raw);
            if (!ni) {
               return A_ERROR;
            }
            ni->ni_stats.ns_rx_decryptcrc++;
            /* remove extra node ref count added by find_node above */
            ieee80211_free_node(ni);
        }
        scn->scn_stats.rx_packets++;
        break;
    case HTT_RX_IND_MPDU_STATUS_MGMT_CTRL:
        if (qdf_likely(peer)) {
            rx_desc = htt_rx_msdu_desc_retrieve(txrx_pdev->htt_pdev, amsdu);

            data_length = RXDESC_GET_DATA_LEN(rx_desc);
            scn->scn_stats.rx_bytes += data_length;
            scn->scn_stats.rx_packets++;

            hdr_des = txrx_pdev->htt_pdev->ar_rx_ops->wifi_hdr_retrieve(rx_desc);
            wh = (struct ieee80211_frame*)hdr_des;
            if ((wh->i_fc[0] & IEEE80211_FC0_TYPE_MASK) == IEEE80211_FC0_TYPE_CTL)
                scn->scn_stats.rx_num_ctl++;
            if ((wh->i_fc[0] & IEEE80211_FC0_TYPE_MASK) == IEEE80211_FC0_TYPE_MGT) {
                scn->scn_stats.rx_num_mgmt++;
                vdev = peer->vdev;
                vap = ol_ath_getvap(vdev);
                if (qdf_likely(vap)) {
		  (&vap->iv_unicast_stats)->ims_rx_mgmt++;
		  ni = ieee80211_vap_find_node(vap, peer->mac_addr.raw);
		  if (qdf_likely(ni)) {
		    ni->ni_stats.ns_rx_mgmt++;
		    ieee80211_free_node(ni);
		  }
                }
            }
            if ((wh->i_fc[0] & IEEE80211_FC0_TYPE_MASK) == IEEE80211_FC0_TYPE_DATA) {
                scn->scn_stats.rx_num_data++;
                scn->scn_stats.rx_data_bytes += data_length;
            }

            attn = txrx_pdev->htt_pdev->ar_rx_ops->get_attn_word(rx_desc);
            if (attn & RX_NULL_DATA_MASK) {
                /*update counter for per-STA null data pkts*/
                vdev = peer->vdev;
                vap = ol_ath_getvap(vdev);
                if (qdf_likely(vap)) {
                    ni = ieee80211_vap_find_node(vap,peer->mac_addr.raw);
                    if (qdf_likely(ni)) {
                        ni->ni_stats.ns_rx_data++;
                        ni->ni_stats.ns_rx_bytes += data_length;
#if UMAC_SUPPORT_STA_STATS_ENHANCEMENT
                        /* NULL data frame must be 'unicast' */
                        ni->ni_stats.ns_rx_ucast++;
                        ni->ni_stats.ns_rx_ucast_bytes += data_length;
#endif
                        ieee80211_free_node(ni);
                    }
                }
            }

            msdu = amsdu;
            while (msdu) {
                rx_desc = htt_rx_msdu_desc_retrieve(txrx_pdev->htt_pdev, msdu);
                attn = txrx_pdev->htt_pdev->ar_rx_ops->get_attn_word(rx_desc);
                if (attn & RX_OVERFLOW_MASK) {
                    /*RX overflow*/
                    scn->scn_stats.rx_overrun++;
                }
                msdu = qdf_nbuf_next(msdu);
            }
        }
        break;
    default:
        rx_desc = htt_rx_msdu_desc_retrieve(txrx_pdev->htt_pdev,
                                                        amsdu);
        data_length = RXDESC_GET_DATA_LEN(rx_desc);
        scn->scn_stats.rx_bytes += data_length;
        scn->scn_stats.rx_packets++;

        break;
    }

    return A_OK;
}

static int32_t count_bitmap(uint32_t bitmap)
{
    static int32_t count;
    uint32_t mask = (uint32_t)1 << ((sizeof(uint32_t)) * BITS_PER_BYTE - 1);
    while (mask) {
        if (bitmap & mask)
        count++;
        mask >>= 1;
    }
    return count;
}

A_STATUS
process_tx_stats(struct ol_txrx_pdev_t *txrx_pdev,
                    void *data, uint16_t peer_id,
                    enum htt_tx_status status)
{
#ifndef REMOVE_PKT_LOG
    struct ol_pktlog_dev_t *pl_dev;
    struct ath_pktlog_stats_hdr pl_hdr;
    struct ol_txrx_peer_t *peer = NULL;
    struct ieee80211vap *vap = NULL;
    struct ieee80211_node *ni = NULL;
#if defined(CONFIG_AR900B_SUPPORT) || defined(CONFIG_AR9888_SUPPORT)
    struct ieee80211_mac_stats *mac_stats = NULL;
#endif
    struct ol_ath_softc_net80211 *scn;
    struct ieee80211com *ic;
    uint32_t *pl_tgt_hdr;
    int32_t diff = 0;
#if ATH_DATA_TX_INFO_EN
    struct ieee80211_tx_status *ts = NULL;
#endif
    uint8_t gintval = 0;

    if (!txrx_pdev) {
        qdf_print("Invalid pdev in %s\n", __func__);
        return A_ERROR;
    }
    scn = (struct ol_ath_softc_net80211 *)txrx_pdev->ctrl_pdev;
    ic = &scn->sc_ic;
#if ATH_DATA_TX_INFO_EN
    ts = scn->tx_status_buf;
#endif

    qdf_assert(txrx_pdev->pl_dev);
    qdf_assert(data);
    pl_dev = txrx_pdev->pl_dev;
    pl_tgt_hdr = (uint32_t *)data;
    /*
     * Makes the short words (16 bits) portable b/w little endian
     * and big endian
     */

    pl_hdr.log_type =  (*(pl_tgt_hdr + ATH_PKTLOG_STATS_HDR_LOG_TYPE_OFFSET) &
                                        ATH_PKTLOG_STATS_HDR_LOG_TYPE_MASK) >>
                                        ATH_PKTLOG_STATS_HDR_LOG_TYPE_SHIFT;

    if (pl_hdr.log_type == PKTLOG_STATS_TYPE_TX_CTRL) {
        int frame_type;
        int series_bw_offset;
        uint8_t sgi_series;
        u_int8_t is_aggr;
#if defined(CONFIG_AR900B_SUPPORT) || defined(CONFIG_AR9888_SUPPORT)
        u_int64_t num_msdus;
        u_int64_t byte_cnt = 0;
#endif
        u_int32_t start_seq_num;
        void *tx_ppdu_ctrl_desc;
        int       peer_id;
        int max_peers;

        tx_ppdu_ctrl_desc = (void *)data + pl_dev->pktlog_hdr_size;
        /*	The peer_id is filled in the target in the ppdu_done function, the
            peer_id istaken from the tid structure
        */
        peer_id = *((u_int32_t *)tx_ppdu_ctrl_desc + TX_PEER_ID_OFFSET);
        max_peers = ol_cfg_max_peer_id(txrx_pdev->ctrl_pdev) + 1;
        if (peer_id > max_peers) {
            IEEE80211_DPRINTF_IC(ic, IEEE80211_VERBOSE_NORMAL, IEEE80211_MSG_ANY, "\nPeer ID invalid  \n");
            return -1;
        }

        txrx_pdev->tx_stats.peer_id = peer_id;
        peer = (peer_id == HTT_INVALID_PEER) ?
                NULL : txrx_pdev->peer_id_to_obj_map[peer_id];
        if (peer) {
            /* extract the seq_num  */
            start_seq_num = ((*((u_int32_t *)tx_ppdu_ctrl_desc + SEQ_NUM_OFFSET))
                                          & SEQ_NUM_MASK);
            /* We don't Check for the wrap around condition, we are
            *   interested only in seeing if we have advanced the
            *   block ack window.
            */
            if (txrx_pdev->tx_stats.seq_num != start_seq_num) {
                    scn->scn_stats.tx_bawadv++;
            }
            /* cache the seq_num in the structure for the next ppdu */
            txrx_pdev->tx_stats.seq_num = start_seq_num;
            /* cache the no_ack in the structure for the next ppdu */
            txrx_pdev->tx_stats.no_ack = ((*((u_int32_t *)tx_ppdu_ctrl_desc + TX_FRAME_OFFSET)) &
                                                TX_FRAME_TYPE_NOACK_MASK) >> TX_FRAME_TYPE_NOACK_SHIFT;
            if (peer && peer->vdev) {
                vap = ol_ath_getvap(peer->vdev);
            }
            if (!vap) {
                qdf_print("\n VAP is already deleted  \n");
                return -1;
            }

#if defined(CONFIG_AR900B_SUPPORT) || defined(CONFIG_AR9888_SUPPORT)
            num_msdus = peer->peer_data_stats.data_packets;
            byte_cnt = peer->peer_data_stats.data_bytes;
            scn->scn_stats.tx_num_data += num_msdus;
            scn->scn_stats.tx_bytes += byte_cnt;
            mac_stats = peer->bss_peer ? &vap->iv_multicast_stats :
                                         &vap->iv_unicast_stats;
            mac_stats->ims_tx_packets += num_msdus;
            mac_stats->ims_tx_bytes += byte_cnt;
            mac_stats->ims_tx_data_packets += num_msdus;
            mac_stats->ims_tx_data_bytes += byte_cnt;
            mac_stats->ims_tx_datapyld_bytes = mac_stats->ims_tx_data_bytes -
                                             (mac_stats->ims_tx_data_packets *
                                                            ETHERNET_HDR_LEN);
#endif

            ni = ieee80211_vap_find_node(vap, peer->mac_addr.raw);
            if (!ni) {
                qdf_print("\n Could not find the peer \n");
                return -1;
            }

            ieee80211_update_ack_rssi(ni, txrx_pdev->tx_stats.ack_rssi);

            diff = peer->peer_data_stats.discard_cnt -  ni->ni_stats.ns_tx_discard ;
            vap->iv_unicast_stats.ims_tx_discard += diff;
            ni->ni_stats.ns_tx_discard = (u_int32_t)peer->peer_data_stats.discard_cnt;

#if defined(CONFIG_AR900B_SUPPORT) || defined(CONFIG_AR9888_SUPPORT)
            if (peer->bss_peer) {
                ni->ni_stats.ns_tx_mcast += num_msdus;
#if UMAC_SUPPORT_STA_STATS_ENHANCEMENT
                ni->ni_stats.ns_tx_mcast_bytes += byte_cnt;
#endif
            } else {
#if ATH_BAND_STEERING
                if (num_msdus) {
                    ol_txrx_mark_peer_inact(peer, false);
                }
#endif
                ni->ni_stats.ns_tx_ucast += num_msdus;
#if UMAC_SUPPORT_STA_STATS_ENHANCEMENT
                ni->ni_stats.ns_tx_ucast_bytes += byte_cnt;
#endif
                ni->ni_stats.ns_tx_data_success += num_msdus;
                ni->ni_stats.ns_tx_bytes_success += byte_cnt;
            }
                ni->ni_stats.ns_tx_data += num_msdus;
                ni->ni_stats.ns_tx_bytes += byte_cnt;

#endif

            /* Right now, I am approx. the header length to 802.11 as the host never receives
               the 80211 header and it is very difficult and will require a lot
               of host CPU cycles to compute it and further amsdu logic will
               make the header calculations even more complicated. Therefore for
               now leaving it at 24.
            */
            peer->peer_data_stats.thrup_bytes = 0;
            peer->peer_data_stats.data_packets = 0;
            peer->peer_data_stats.data_bytes = 0;
            peer->peer_data_stats.discard_cnt = 0;
            ieee80211_free_node(ni);
        }
        frame_type = ((*((u_int32_t *)tx_ppdu_ctrl_desc + TX_FRAME_OFFSET))
                          & TX_FRAME_TYPE_MASK) >> TX_FRAME_TYPE_SHIFT;
        is_aggr = ((*((u_int32_t *)tx_ppdu_ctrl_desc + TX_FRAME_OFFSET))
                            & TX_AMPDU_MASK) >> TX_AMPDU_SHIFT;
        if (is_aggr) {
            scn->scn_stats.tx_compaggr++;
        }
        else {
            scn->scn_stats.tx_compunaggr++;
        }
        /*	Here the frame type is 3 for beacon frames, this is defined
            in the tx_ppdu_start.h
            Frame type indication.  Indicates what type of frame is
           	being sent.  Supported values:
            0: default
            1: Reserved (Used to be used for ATIM)
            2: PS-Poll
            3: Beacon
            4: Probe response
            5-15: Reserved
            <legal:0,2,3,4>
        */

        if (frame_type == 3) {
            scn->scn_stats.tx_beacon++;
        }

        sgi_series = ((*((u_int32_t *)tx_ppdu_ctrl_desc + SGI_SERIES_OFFSET))
                           & SGI_SERIES_MASK) >> SGI_SERIES_SHIFT;
        switch (sgi_series) {
            case 0x1:
                series_bw_offset = SERIES_BW_START_OFFSET + 0 * NUM_OF_DWORDS_SERIES_BW;
            break;
            case 0x2:
                series_bw_offset = SERIES_BW_START_OFFSET + 1 * NUM_OF_DWORDS_SERIES_BW;
            break;
            case 0x4:
                series_bw_offset = SERIES_BW_START_OFFSET + 2 * NUM_OF_DWORDS_SERIES_BW;
            break;
            case 0x8:
                series_bw_offset = SERIES_BW_START_OFFSET + 3 * NUM_OF_DWORDS_SERIES_BW;
            break;
            case 0x10:
                series_bw_offset = SERIES_BW_START_OFFSET + 4 * NUM_OF_DWORDS_SERIES_BW;
            break;
            case 0x20:
                series_bw_offset = SERIES_BW_START_OFFSET + 5 * NUM_OF_DWORDS_SERIES_BW;
            break;
            case 0x40:
                series_bw_offset = SERIES_BW_START_OFFSET + 6 * NUM_OF_DWORDS_SERIES_BW;
            break;
            case 0x80:
                series_bw_offset = SERIES_BW_START_OFFSET + 7 * NUM_OF_DWORDS_SERIES_BW;
            break;
            default:
                series_bw_offset = 0;   /* the valid bw series s0/s1 bits are not set properly */
            break;
        }
        if (series_bw_offset) {
            gintval = ((*((u_int32_t *)tx_ppdu_ctrl_desc + series_bw_offset))
                               & SERIES_BW_MASK) >> SERIES_BW_SHIFT;
        } else
            gintval = 0;
    }
    if (pl_hdr.log_type == PKTLOG_TYPE_TX_STAT) {
        void *tx_ppdu_status_desc = (void *)data + pl_dev->pktlog_hdr_size;

        uint32_t ppdu_bmap_failed_lo;
        uint32_t ppdu_bmap_failed_hi;
        static int32_t mpdus_ack_failed;
        ppdu_bmap_failed_lo = (*((u_int32_t *)tx_ppdu_status_desc + BA_BMAP_LSB_OFFSET));
        ppdu_bmap_failed_hi = (*((u_int32_t *)tx_ppdu_status_desc + BA_BMAP_MSB_OFFSET));
        mpdus_ack_failed = count_bitmap(ppdu_bmap_failed_lo);
        mpdus_ack_failed = count_bitmap(ppdu_bmap_failed_hi);
#if ATH_DATA_TX_INFO_EN
        ts->ppdu_bmap_failed_hi = ppdu_bmap_failed_hi;
        ts->ppdu_bmap_failed_lo = ppdu_bmap_failed_lo;
#endif
        scn->ath_stats.tx.mpdus_ack_failed = mpdus_ack_failed;
        txrx_pdev->tx_stats.ack_rssi = *((u_int32_t *)tx_ppdu_status_desc + TX_RSSI_OFFSET) & TX_RSSI_MASK;

        /* If no_ack, no way to know if tx has error so ignore. If !tx_ok, incr tx_error.  */
        /* TODO: Define ole_stats and ole_desc strcut and get stats from ole_stats.*/
        if ( !txrx_pdev->tx_stats.no_ack &&
            !((*((u_int32_t *)tx_ppdu_status_desc + TX_OK_OFFSET)) & TX_OK_MASK))
        {
            /* Peer_id is cached in PKTLOG_STATS_TYPE_TX_CTRL.
            It's not clean but we need to live with it for now. */
            peer = (txrx_pdev->tx_stats.peer_id == HTT_INVALID_PEER) ?
                NULL : txrx_pdev->peer_id_to_obj_map[txrx_pdev->tx_stats.peer_id];
            if (peer && peer->vdev) {
                vap = ol_ath_getvap(peer->vdev);
                if (vap) {
                    vap->iv_stats.is_tx_not_ok++;
                    ni = ieee80211_vap_find_node(vap, peer->mac_addr.raw);
                    if (!ni) {
                        return A_OK;
                    }
                    ni->ni_stats.ns_is_tx_not_ok++;
                    ieee80211_free_node(ni);
                }
            }
        }
    }
    if (pl_hdr.log_type == PKTLOG_TYPE_RC_UPDATE) {
        void *tx_ppdu_rcu_desc = (void *)data + pl_dev->pktlog_hdr_size;

        uint8_t  rate_idx;
        uint32_t ratekbps;
        rate_idx = ((*((u_int32_t *)tx_ppdu_rcu_desc + RATE_IDX_OFFSET)) & RATE_IDX_MASK);
        ratekbps = whal_rate_idx_to_kbps(rate_idx, gintval);
#if ATH_DATA_TX_INFO_EN
        ts->ts_rateKbps = ratekbps;
#endif
        /* Peer_id is cached in PKTLOG_STATS_TYPE_TX_CTRL.
            It's not clean but we need to live with it for now. */
        peer = (txrx_pdev->tx_stats.peer_id == HTT_INVALID_PEER) ?
            NULL : txrx_pdev->peer_id_to_obj_map[txrx_pdev->tx_stats.peer_id];
        if (peer && peer->vdev) {
            vap = ol_ath_getvap(peer->vdev);
            if (vap) {
                ni = ieee80211_vap_find_node(vap, peer->mac_addr.raw);
                if (!ni) {
                    return A_OK;
                }
                if (ratekbps)
                    OL_ATH_RATE_LPF((OL_ATH_NODE_NET80211(ni))->an_ni_avg_tx_rate, ratekbps);
                ni->ni_stats.ns_ppdu_tx_rate = OL_ATH_RATE_OUT((OL_ATH_NODE_NET80211(ni))->an_ni_avg_tx_rate);
                ieee80211_free_node(ni);
            }
        }
    }

#endif

    return A_OK;
}


void
ol_ath_get_all_stats(void *pdev, enum WDI_EVENT event,
                        void *log_data, uint16_t peer_id,
                        enum htt_rx_status status)
{
    struct ol_txrx_pdev_t *txrx_pdev =  (struct ol_txrx_pdev_t *)pdev;
    struct ol_ath_softc_net80211 *scn = NULL;
    struct ieee80211com *ic = NULL;

    scn = (struct ol_ath_softc_net80211 *)txrx_pdev->ctrl_pdev;
    if (!scn) {
        qdf_print("Invalid scn handle return\n");
        return;
    }
    ic  = &scn->sc_ic;
    if (!ic) {
        qdf_print("Invalid ic handle return\n");
        return;
    }
    switch(event) {
    case WDI_EVENT_RX_DESC_REMOTE:
        /*
         * process RX message for local frames
         */
        if(process_rx_stats(pdev, log_data, peer_id, status)) {
            qdf_print("Unable to process RX info\n");
            return;
        }
        break;

    case WDI_EVENT_TX_STATUS:
    case WDI_EVENT_OFFLOAD_ALL:
        /*
         *Process TX message
         */
        if(process_tx_stats(pdev, log_data, peer_id, status)) {
            IEEE80211_DPRINTF_IC(ic, IEEE80211_VERBOSE_NORMAL, IEEE80211_MSG_ANY, "\n Unable to process TX info \n");
            return;
        }
        break;

    default:
        break;
    }
    return;
}

static A_STATUS
extstats_hif_callback(void *pdev, qdf_nbuf_t netbuf, u_int8_t pipeID)
{
    uint8_t *netdata;
    A_STATUS rc = A_OK;

    ASSERT(pipeID == CE_PKTLOG_PIPE);

    netdata = qdf_nbuf_data(netbuf);

    /* TODO: Resolve differences in process_tx_stats() due to Beeliner, and
     * enable it */
    /* rc = process_tx_stats(pdev, netdata, 0, 0); */

    qdf_nbuf_free(netbuf);

    return rc;
}

static int ol_ath_enable_ap_stats(struct ieee80211com *ic, u_int8_t stats_cmd)
{
    struct ol_ath_softc_net80211 *scn = OL_ATH_SOFTC_NET80211(ic);
    struct hif_opaque_softc *h_sc = (struct hif_opaque_softc *)scn->hif_hdl;
    uint32_t types = 0;

    if(!h_sc) {
        qdf_print("Invalid h_sc in %s\n", __func__);
        return A_ERROR;
    }

    if (stats_cmd == 1) {
        /* Call back for stats */
	    if (scn->scn_stats.ap_stats_tx_cal_enable) {
            return A_OK;
        }

#if ATH_SUPPORT_EXT_STAT
        /* reset the statistics */
        ieee80211_reset_client_rt_rate(ic);
        /* start timer to calculate the bytes/packets in last 1 second */
        OS_SET_TIMER (&ic->ic_client_stat_timer, 1000);
#endif
        STATS_RX_SUBSCRIBER.callback = ol_ath_get_all_stats;
        if(wdi_event_sub(scn->pdev_txrx_handle,
                        &STATS_RX_SUBSCRIBER,
                        WDI_EVENT_RX_DESC_REMOTE)) {
            return A_ERROR;
        }

        if (scn->is_ar900b) {
            if(ol_ath_pdev_set_param(scn,
                    wmi_pdev_param_en_stats, scn->pdev_txrx_handle->fw_supported_enh_stats_version, 0) != EOK ) {
                return A_ERROR;
            }
        } else {
            STATS_TX_SUBSCRIBER.callback = ol_ath_get_all_stats;
            if(wdi_event_sub(scn->pdev_txrx_handle,
                            &STATS_TX_SUBSCRIBER,
                            WDI_EVENT_OFFLOAD_ALL)) {
                return A_ERROR;
            }

#if defined(CONFIG_AR900B_SUPPORT) || defined(CONFIG_AR9888_SUPPORT)
            types |= WMI_HOST_PKTLOG_EVENT_TX;
            types |= WMI_HOST_PKTLOG_EVENT_RCU;
            /*enabling the pktlog for stats*/
            if(wmi_unified_packet_log_enable_send(scn->wmi_handle, types)) {
                return A_ERROR;
            }
#endif
        }
        scn->scn_stats.ap_stats_tx_cal_enable = 1;
        ol_txrx_enable_enhanced_stats(scn->pdev_txrx_handle);

#ifdef QCA_NSS_WIFI_OFFLOAD_SUPPORT
        osif_nss_ol_stats_cfg(scn, stats_cmd);
#endif

        return A_OK;
    } else if (stats_cmd == 0) {
	    if (!scn->scn_stats.ap_stats_tx_cal_enable) {
            return A_OK;
        }
#if ATH_SUPPORT_EXT_STAT
        /* stop timer to calculate the bytes/packets in last 1 second */
        OS_CANCEL_TIMER (&ic->ic_client_stat_timer);
#endif
        qdf_mem_zero(&scn->scn_stats, sizeof(scn->scn_stats));
        scn->scn_stats.ap_stats_tx_cal_enable = 0;
        ol_txrx_disable_enhanced_stats(scn->pdev_txrx_handle);

        if (scn->is_ar900b) {
            if(ol_ath_pdev_set_param(scn,
                    wmi_pdev_param_en_stats, 0, 0) != EOK ) {
                return A_ERROR;
            }
        } else {
            if(wdi_event_unsub(
                        scn->pdev_txrx_handle,
                        &STATS_TX_SUBSCRIBER,
                        WDI_EVENT_OFFLOAD_ALL)) {
                return A_ERROR;
            }
        }

        if(wdi_event_unsub(
                    scn->pdev_txrx_handle,
                    &STATS_RX_SUBSCRIBER,
                    WDI_EVENT_RX_DESC_REMOTE)) {
            return A_ERROR;
        }

#if defined(CONFIG_AR900B_SUPPORT) || defined(CONFIG_AR9888_SUPPORT)
        if (!scn->is_ar900b) {
            if(!wmi_unified_packet_log_disable_send(scn->wmi_handle)) {
                return A_ERROR;
            }
        }
#endif

#ifdef QCA_NSS_WIFI_OFFLOAD_SUPPORT
        osif_nss_ol_stats_cfg(scn, stats_cmd);
#endif

        return A_OK;
    } else {
        return A_ERROR;
    }
}

#define IEEE80211_DEFAULT_CHAN_STATS_PERIOD     (1000)

void
ol_ath_stats_attach(struct ieee80211com *ic)
{
    struct ol_ath_softc_net80211 *scn = OL_ATH_SOFTC_NET80211(ic);

    ic->ic_get_cur_chan_stats = ol_ath_net80211_get_cur_chan_stats;
    ic->ic_ath_request_stats = ol_ath_request_stats;
    ic->ic_hal_get_chan_info = ol_ath_get_chan_info;
    ic->ic_ath_send_rssi = ol_ath_send_rssi;
    ic->ic_ath_bss_chan_info_stats = ol_ath_bss_chan_info_request_stats;
    /* Enable and disable stats*/
    ic->ic_ath_enable_ap_stats = ol_ath_enable_ap_stats;
    /* register target stats event handler */
    wmi_unified_register_event_handler(scn->wmi_handle, wmi_update_stats_event_id,
                                       ol_ath_update_stats_event_handler, WMI_RX_UMAC_CTX);
    wmi_unified_register_event_handler(scn->wmi_handle,
                                        wmi_inst_rssi_stats_event_id,
                                        ol_ath_rssi_cb, WMI_RX_UMAC_CTX);
    wmi_unified_register_event_handler(scn->wmi_handle, wmi_pdev_bss_chan_info_event_id,
				      ol_ath_bss_chan_info_event_handler, WMI_RX_UMAC_CTX);

	/* enable the pdev stats event */
    ol_ath_pdev_set_param(scn,
                               wmi_pdev_param_pdev_stats_update_period,
                               PDEV_DEFAULT_STATS_UPDATE_PERIOD, 0);
    /* enable the pdev stats event */
    ol_ath_pdev_set_param(scn,
                               wmi_pdev_param_vdev_stats_update_period,
                               VDEV_DEFAULT_STATS_UPDATE_PERIOD, 0);
    /* enable the pdev stats event */
    ol_ath_pdev_set_param(scn,
                               wmi_pdev_param_peer_stats_update_period,
                               PEER_DEFAULT_STATS_UPDATE_PERIOD, 0);

    /* enable periodic chan stats event */
    ol_ath_periodic_chan_stats_config(scn, true,
                 IEEE80211_DEFAULT_CHAN_STATS_PERIOD);

}

void
ol_ath_stats_detach(struct ieee80211com *ic)
{
    struct ol_ath_softc_net80211 *scn = OL_ATH_SOFTC_NET80211(ic);

    ic->ic_get_cur_chan_stats = NULL;
    ic->ic_ath_request_stats = NULL;
    ic->ic_hal_get_chan_info = NULL;
	ic->ic_ath_send_rssi = NULL;
    ic->ic_ath_enable_ap_stats = NULL;
    /* unregister target stats event handler */
    wmi_unified_unregister_event_handler(scn->wmi_handle, wmi_update_stats_event_id);
	wmi_unified_unregister_event_handler(scn->wmi_handle, wmi_inst_rssi_stats_event_id);
    wmi_unified_unregister_event_handler(scn->wmi_handle, wmi_pdev_bss_chan_info_event_id);
    /* disable target stats event */
    ol_ath_pdev_set_param(scn, wmi_pdev_param_pdev_stats_update_period, 0, 0);
    ol_ath_pdev_set_param(scn, wmi_pdev_param_vdev_stats_update_period, 0, 0);
    ol_ath_pdev_set_param(scn, wmi_pdev_param_peer_stats_update_period, 0, 0);
    /* disable periodic chan stats event */
    ol_ath_periodic_chan_stats_config(scn, false, IEEE80211_DEFAULT_CHAN_STATS_PERIOD);
}


void
ol_get_wlan_dbg_stats(struct ol_ath_softc_net80211 *scn,
                            struct wlan_dbg_stats *dbg_stats)
{
    OS_MEMCPY(dbg_stats, &scn->ath_stats, sizeof(struct wlan_dbg_stats));
}

int
ol_get_tx_free_desc(struct ol_ath_softc_net80211 *scn)
{
    struct ol_txrx_pdev_t *pdev = scn->pdev_txrx_handle;
    int total;
    int used = 0;

    total = ol_cfg_target_tx_credit(pdev->ctrl_pdev);
    used = ol_txrx_get_tx_pending(pdev);
    return (total - used);

}
void ol_get_radio_stats(struct ol_ath_softc_net80211 *scn,
                            struct ol_ath_radiostats *stats)
{
    scn->scn_stats.tx_buf_count = ol_get_tx_free_desc(scn);
    scn->scn_stats.chan_nf = scn->chan_nf;
    scn->scn_stats.chan_nf_sec80 = scn->chan_nf_sec80;
    OS_MEMCPY(stats, &scn->scn_stats, sizeof(struct ol_ath_radiostats));
}

void ol_vap_txdiscard_stats_update(void *vosdev, qdf_nbuf_t nbuf)
{
    osif_dev  *osdev = (osif_dev  *)vosdev;
    struct ieee80211vap *vap = osdev->os_if;
    if ( vap != NULL) {
        struct ieee80211_node *ni = NULL;
        char *data = qdf_nbuf_data(nbuf);
        vap->iv_stats.is_tx_nobuf++;
        ni = ieee80211_find_node(&vap->iv_ic->ic_sta, data);
        if (ni) {
            ni->ni_stats.ns_is_tx_nobuf++;
            ieee80211_free_node(ni);
        }
    }
}
#ifdef ATH_SUPPORT_RDKB
/* This is wave2 (beeliner, dakota, cascade) compatible only */
static void
ol_if_collect_peer_tx_stats(struct ieee80211com *ic,
                                   struct ieee80211_node *ni,
                                   ppdu_common_stats_v3 *in,
                                   int version)
{
    static const unsigned char ofdm2rix[] = {
        [0] = 6, /* 48 mbps */
        [1] = 4, /* 24 mbps */
        [2] = 2, /* 12 mbps */
        [3] = 0, /* 6 mbps */
        [4] = 7, /* 54 mbps */
        [5] = 5, /* 36 mbps */
        [6] = 3, /* 18mbps */
        [7] = 1, /* 9 mbps */
    };
    static const unsigned char cck2rix[] = {
        [0] = 3, /* 11 mbps (long) */
        [1] = 2, /* 5.5 mbps (long) */
        [2] = 1, /* 2 mbps (long) */
        [3] = 0, /* 1 mbps (long) */
        [4] = 6, /* 11 mbps (short) */
        [5] = 5, /* 5 mbps (short) */
        [6] = 4, /* 2 mbps (short) */
    };
    struct wlanstats_cmn_rate_info ri = {};
    int preamble;
    int nss;
    int mcs;
    int attempts;
    int success;
    int ppdus;
    int rix;
    u32 long_retries;
    u32 mpdus_tried;
    u32 mpdus_failed;
    u32 is_aggregate;
    u32 tx_status;
    u32 bw_idx;
    u32 rate;

    if (version < 1)
        return;

    rate = HTT_T2H_EN_STATS_RATE_GET(in);

    /* Undocumented. It'd end up with mcs=15 which is nonsense so ignore this
     * report altogether.
     */
    if (rate == 0xff)
        return;

    long_retries = HTT_T2H_EN_STATS_LONG_RETRIES_GET(in);
    mpdus_tried = HTT_T2H_EN_STATS_MPDUS_TRIED_GET(in);
    mpdus_failed = HTT_T2H_EN_STATS_MPDUS_FAILED_GET(in);
    is_aggregate = HTT_T2H_EN_STATS_IS_AGGREGATE_GET(in);
    tx_status = HTT_T2H_EN_STATS_TX_STATUS_GET(in);
    bw_idx = HTT_T2H_EN_STATS_BW_IDX_GET(in);

    preamble = (rate >> 6) & 0x3;
    nss = (rate >> 4) & 0x3;
    mcs = (rate >> 0) & 0xf;

    switch (preamble) {
    case 0:
        ri.mcs = ofdm2rix[mcs % ARRAY_SIZE(ofdm2rix)];
        break;
    case 1:
        ri.is_cck = 1;
        ri.mcs = cck2rix[mcs % ARRAY_SIZE(cck2rix)];
        break;
    case 2:
    case 3:
        ri.is_ht = 1;
        ri.mcs = mcs;
        ri.nss = nss;
        ri.bw = bw_idx;
        break;
    }

    rix = wlanstats_cmn_calc_rix(&ri);

    ppdus = long_retries + 1;
    attempts = ppdus * mpdus_tried;
    success = mpdus_tried - mpdus_failed;

    if (!is_aggregate && tx_status == 0)
        success = 1;

    ni->tx_stats->stats[rix].attempts += attempts;
    ni->tx_stats->stats[rix].success += success;
    ni->tx_stats->stats[rix].ppdus += ppdus;
}
#endif

#if ENHANCED_STATS
#define PPDU_STATS_TX_ERROR_MASK 0xFEC
int ol_ath_enh_stats_handler(struct ol_txrx_pdev_t *txrx_pdev, uint32_t* msg_word, uint32_t msg_len)
{
    struct ol_ath_softc_net80211 *scn = NULL;
    struct ieee80211com *ic = NULL;
    struct ol_txrx_peer_t *peer;
    struct ieee80211_node *ni = NULL;
    ppdu_common_stats_v3 *ppdu_stats = NULL;
    uint16_t start_seq_num;
    struct ol_txrx_vdev_t *vdev = NULL;
    struct ieee80211vap *vap;
    struct ieee80211_mac_stats *mac_stats = NULL;
    u_int8_t num_mpdus = 0;
    u_int16_t num_msdus;
    u_int64_t byte_cnt = 0;
#if ATH_DATA_TX_INFO_EN
    struct ieee80211_tx_status  *ts = NULL;
    uint8_t tid = 0, ac = 0;
#endif
    uint32_t rate;
    uint8_t bandwidth;
    uint32_t ratekbps;
    enum htt_t2h_en_stats_type version = 0;
    enum htt_t2h_en_stats_status status = 0;
    uint8_t tx_status = 0;

    scn = (struct ol_ath_softc_net80211 *)txrx_pdev->ctrl_pdev;
    ic  = &scn->sc_ic;
#if ATH_DATA_TX_INFO_EN
    ts = scn->tx_status_buf;
#endif

    ppdu_stats = (ppdu_common_stats_v3 *)ol_txrx_get_en_stats_base(txrx_pdev, msg_word, msg_len, &version, &status);

    if (status == HTT_T2H_EN_STATS_STATUS_INVALID) {
        txrx_pdev->fw_supported_enh_stats_version = PPDU_STATS_VERSION_1;
        if(ol_ath_pdev_set_param(scn, wmi_pdev_param_en_stats,
		 txrx_pdev->fw_supported_enh_stats_version, 0) != EOK ) {
            return A_ERROR;
        }
        return A_ERROR;
    }

    if (!ppdu_stats) {
       return A_EINVAL;
    }
#if ATH_DATA_TX_INFO_EN
    switch (version) {
        case HTT_T2H_EN_STATS_TYPE_COMMON:
            ts->version = PPDU_STATS_VERSION_1;
            break;

        case HTT_T2H_EN_STATS_TYPE_COMMON_V2:
            ts->version = PPDU_STATS_VERSION_2;
            break;

        case HTT_T2H_EN_STATS_TYPE_COMMON_V3:
            ts->version = PPDU_STATS_VERSION_3;
            break;

        default:
            IEEE80211_DPRINTF_IC(ic, IEEE80211_VERBOSE_NORMAL, IEEE80211_MSG_ANY, "Invalid stats version received from FW\n");
            break;
    }
#endif
    /* TODO -
     * Get discard stats from FW instead of this hack */
    txrx_pdev->tx_stats.peer_id = HTT_T2H_EN_STATS_PEER_ID_GET(ppdu_stats);
    peer = (HTT_T2H_EN_STATS_PEER_ID_GET(ppdu_stats) == HTT_INVALID_PEER) ?
        NULL : txrx_pdev->peer_id_to_obj_map[HTT_T2H_EN_STATS_PEER_ID_GET(ppdu_stats)];

    if (peer) {
        /* extract the seq_num  */
        start_seq_num = HTT_T2H_EN_STATS_STARTING_SEQ_NUM_GET(ppdu_stats);
        /* We don't Check for the wrap around condition, we are
         *   interested only in seeing if we have advanced the
         *   block ack window.
         */
        if (txrx_pdev->tx_stats.seq_num != start_seq_num) {
            scn->scn_stats.tx_bawadv++;
        }
        /* cache the seq_num in the structure for the next ppdu */
        txrx_pdev->tx_stats.seq_num = start_seq_num;

        vdev = peer->vdev;
        vap = ol_ath_getvap(vdev);
        if (!vap) {
            return A_ERROR;
        }

        if (HTT_T2H_EN_STATS_PKT_TYPE_GET(ppdu_stats) == TX_FRAME_TYPE_BEACON) {
            scn->scn_stats.tx_beacon++;
        } else if(HTT_T2H_EN_STATS_PKT_TYPE_GET(ppdu_stats) == TX_FRAME_TYPE_DATA) {

            num_mpdus = HTT_T2H_EN_STATS_MPDUS_QUEUED_GET(ppdu_stats) - HTT_T2H_EN_STATS_MPDUS_FAILED_GET(ppdu_stats);
            num_msdus = HTT_T2H_EN_STATS_MSDU_SUCCESS_GET(ppdu_stats);

            byte_cnt = ppdu_stats->success_bytes;

            rate = HTT_T2H_EN_STATS_RATE_GET(ppdu_stats);
            bandwidth = HTT_T2H_EN_STATS_BW_IDX_GET(ppdu_stats);
            ratekbps = whal_ratecode_to_kbps((uint8_t)rate, bandwidth, vap->iv_data_sgi);

#if ATH_DATA_TX_INFO_EN
            ts->ppdu_rate = rate;
            ts->bandwidth = bandwidth;
            ts->ts_rateKbps = ratekbps;
            ts->ppdu_num_mpdus_success = num_mpdus;
            ts->ppdu_num_mpdus_fail = HTT_T2H_EN_STATS_MPDUS_FAILED_GET(ppdu_stats);
            ts->ppdu_num_msdus_success = num_msdus;
            ts->ppdu_bytes_success = byte_cnt;
            ts->ppdu_duration = ppdu_stats->ppdu_duration;
            ts->ppdu_retries = HTT_T2H_EN_STATS_LONG_RETRIES_GET(ppdu_stats);
            ts->ppdu_is_aggregate = HTT_T2H_EN_STATS_IS_AGGREGATE_GET(ppdu_stats);
            ts->ts_retries = HTT_T2H_EN_STATS_LONG_RETRIES_GET(ppdu_stats);

            if (ts->version == PPDU_STATS_VERSION_3) {
                ts->ppdu_ack_timestamp = ppdu_stats->ppdu_ack_timestamp;
                ts->start_seq_num = start_seq_num;
                ts->ppdu_bmap_enqueued_hi = ppdu_stats->ppdu_bmap_enqueued_hi;
                ts->ppdu_bmap_enqueued_lo = ppdu_stats->ppdu_bmap_enqueued_lo;
                ts->ppdu_bmap_tried_hi = ppdu_stats->ppdu_bmap_tried_hi;
                ts->ppdu_bmap_tried_lo = ppdu_stats->ppdu_bmap_tried_lo;
                ts->ppdu_bmap_failed_hi = ppdu_stats->ppdu_bmap_failed_hi;
                ts->ppdu_bmap_failed_lo = ppdu_stats->ppdu_bmap_failed_lo;
            }
#endif

            scn->scn_stats.tx_num_data += num_msdus;
            scn->scn_stats.tx_bytes += byte_cnt;
            mac_stats = peer->bss_peer ? &vap->iv_multicast_stats :
                &vap->iv_unicast_stats;
            mac_stats->ims_tx_packets += num_msdus;
            mac_stats->ims_tx_bytes += byte_cnt;
            mac_stats->ims_tx_data_packets += num_msdus;
            mac_stats->ims_tx_data_bytes += byte_cnt;
            mac_stats->ims_tx_datapyld_bytes = mac_stats->ims_tx_data_bytes -
                (mac_stats->ims_tx_data_packets *
                 (ETHERNET_HDR_LEN + 24));

            ni = ieee80211_vap_find_node(vap,peer->mac_addr.raw);
            if (!ni) {
                return A_ERROR;
            }

            if (ratekbps)
                OL_ATH_RATE_LPF((OL_ATH_NODE_NET80211(ni))->an_ni_avg_tx_rate, ratekbps);
            ni->ni_stats.ns_ppdu_tx_rate = OL_ATH_RATE_OUT((OL_ATH_NODE_NET80211(ni))->an_ni_avg_tx_rate);

            /* TODO - Get this from FW */
            vap->iv_unicast_stats.ims_tx_discard += peer->peer_data_stats.discard_cnt;
            ni->ni_stats.ns_tx_discard += peer->peer_data_stats.discard_cnt;
            peer->peer_data_stats.discard_cnt = 0;
            /* */

#ifdef ATH_SUPPORT_RDKB
            if (qdf_unlikely(IS_WLANSTATS_ENABLED(ic)))
                ol_if_collect_peer_tx_stats(ic, ni, ppdu_stats, ts->version);
#endif
            if (peer->bss_peer) {
                ni->ni_stats.ns_tx_mcast += num_msdus;
#if UMAC_SUPPORT_STA_STATS_ENHANCEMENT
                ni->ni_stats.ns_tx_mcast_bytes += byte_cnt;
#endif
            } else {
#if ATH_BAND_STEERING
                if (num_msdus) {
                    ol_txrx_mark_peer_inact(peer, false);
                }
#endif
                ni->ni_stats.ns_tx_ucast += num_msdus;
#if UMAC_SUPPORT_STA_STATS_ENHANCEMENT
                ni->ni_stats.ns_tx_ucast_bytes += byte_cnt;
#endif
                ni->ni_stats.ns_tx_data_success += num_msdus;
                ni->ni_stats.ns_tx_bytes_success += byte_cnt;
            }
            ni->ni_stats.ns_tx_data += num_msdus;
            ni->ni_stats.ns_tx_bytes += byte_cnt;
            mac_stats->ims_retries +=
                HTT_T2H_EN_STATS_LONG_RETRIES_GET(ppdu_stats);
            /* ack rssi of separate chains */
            RSSI_CHAIN_PRI20(ppdu_stats->rssi[0], ni->ni_stats.ns_rssi_chain[0]);
            RSSI_CHAIN_PRI20(ppdu_stats->rssi[1], ni->ni_stats.ns_rssi_chain[1]);
            RSSI_CHAIN_PRI20(ppdu_stats->rssi[2], ni->ni_stats.ns_rssi_chain[2]);
            RSSI_CHAIN_PRI20(ppdu_stats->rssi[3], ni->ni_stats.ns_rssi_chain[3]);
            /* Mask out excessive retry error. Dont treat excessive retry as tx error */
            tx_status =  HTT_T2H_EN_STATS_TX_STATUS_GET(ppdu_stats);
#if ATH_DATA_TX_INFO_EN
            if ((ts->version >= HTT_T2H_EN_STATS_TYPE_COMMON_V2) &&
                (tx_status == 0)) {
                tid = HTT_T2H_EN_STATS_TID_NUM_GET(ppdu_stats);
                if (tid < 8) {
                    ac = TID_TO_WME_AC(tid);
                    ni->ni_stats.ns_tx_wme[ac]++;
                    mac_stats->ims_tx_wme[ac]++;
                }
            }
#endif
            /* Mask out excessive retry error. Dont treat excessive retry as tx error */
            tx_status &= PPDU_STATS_TX_ERROR_MASK;
            if(tx_status) {
                vap->iv_stats.is_tx_not_ok++;
                ni->ni_stats.ns_is_tx_not_ok++;
            }

            ieee80211_free_node(ni);

            if (HTT_T2H_EN_STATS_IS_AGGREGATE_GET(ppdu_stats)) {
                scn->scn_stats.tx_compaggr++;
            }
            else {
                scn->scn_stats.tx_compunaggr++;
            }
        } else if (HTT_T2H_EN_STATS_PKT_TYPE_GET(ppdu_stats) == TX_FRAME_TYPE_MGMT) {

          if (!peer->bss_peer)
              (&vap->iv_unicast_stats)->ims_tx_mgmt++;

          ni = ieee80211_vap_find_node(vap, peer->mac_addr.raw);
          if (!ni) {
              return A_ERROR;
          }

          ni->ni_stats.ns_tx_mgmt++;
          ieee80211_free_node(ni);
        }
    }
    return A_OK;
}
#endif /* ENHANCE_STATS */

#endif /* ATH_PERF_PWR_OFFLOAD */

