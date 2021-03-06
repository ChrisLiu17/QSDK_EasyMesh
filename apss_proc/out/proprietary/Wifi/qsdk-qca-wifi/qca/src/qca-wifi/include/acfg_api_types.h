/*
 * Copyright (c) 2017-2018 Qualcomm Innovation Center, Inc.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Innovation Center, Inc.
 *
 * Copyright (c) 2008-2010, Atheros Communications Inc.
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
#ifndef __ACFG_API_TYPES_H
#define __ACFG_API_TYPES_H

#include <qdf_types.h>
#include <ieee80211_defines.h>
#include <ieee80211_ioctl.h>
#include <ieee80211_rrm.h>
#include <if_athioctl.h>

#define acfg_min(x, y)   (x < y ? x : y)
#define acfg_max(x, y)   (x > y ? x : y)

/**
 * @brief constants
 */
enum {
    ACFG_MAX_BB_PANICS      = 3,    /**< Number of BB Panics to get logged */
    ACFG_LATENCY_CATS       = 5,    /**< Number of categories to log latency */
    ACFG_LATENCY_BINS       = 45,   /**< Number of 1024us bins to log latency */
    ACFG_MACADDR_LEN        = 6,    /**< Mac Address len */
    ACFG_MAX_IFNAME         = 16,   /**< Max interface name string */
    ACFG_MAX_SSID_LEN       = 32,   /**< Max SSID length */
    ACFG_MAX_SCANLIST       = 16,   /**< Max number scanned entries per call */
    ACFG_MAX_IELEN          = 256,  /**< Max IE length */
    ACFG_MAX_RATE_SIZE      = 44,   /**< Max MCS rate element size */
    ACFG_MAX_HT_MCSSET      = 16,   /**< Represents 128-bit rates */
    ACFG_MAX_IEEE_CHAN      = 255,  /**< Max number of IEEE
                                      Channels supported */
    ACFG_MAX_CHMODE_LEN     = 16,   /**< Max Channel Mode String length */
    ACFG_ENCODING_TOKEN_MAX = 64, /**< max size of encoding token */
    ACFG_MAX_MODE_LEN       = 16,   /**< Max length of phymode string */
    ACFG_MAX_ACPARAMS       = 4,   /**< Max number of AC params */
    ACFG_WSUPP_UNIQUE_LEN = 8,      /**< Unique id len for security */

    ACFG_WSUPP_PARAM_LEN = 64,      /**< Supplicant param length */
    ACFG_WPS_CONFIG_METHODS_LEN = 256, /**< wps config methods length */
    ACFG_WPS_RF_BANDS_LEN   = 3,    /**< wps rf bands length */
    ACFG_WSUPP_REPLY_LEN = 256,     /**< Supplicant reply length */
    ACFG_DL_MAX_CMDSZ   = 1024,
    ACFG_MAX_VIDNAME    = 6 ,       /** Max VLAN ID = "4096" */
    ACFG_SZ_REPLAYCMD   = 128,
    ACFG_MACSTR_LEN        = 18,    /**< Mac Address len */
    ACFG_VENDOR_PARAM_LEN = 100,    /**< Vendor param len */
    ACFG_MAX_VENDOR_PARAMS = 20,    /**< Max Vendor params */
    ACFG_KEYDATA_LEN    = 32,       /**< Key Data len */
    ACFG_MAX_BITRATE    = 32,       /**< Max bitrates */
    ACFG_MAX_FREQ       = 32,       /**< Max frequencies */
    ACFG_MAX_ENCODING   = 8,        /**< Max encoding tokens */
    ACFG_MAX_TXPOWER    = 8,        /**< Max tx powers */
    ACFG_MAX_INTR_BKT   = 512,      /**< Counter buckets, for packets handled in a single iteration */
    ACFG_VI_LOG_LEN     = 10,       /**< Max number of VI logs */
    ACFG_MCS_RATES      = 31,       /**< MCS rates */
    ACFG_MAX_DFS_FILTER = 32,       /**< Max number of DFS filters */
    ACFG_MAX_ATF_STALIST_SIZE = 1024, /**< Max size of ATF sta list */
    ACFG_MAX_PERCENT_SIZE = 4,      /**< Max size of percent str */
    ACFG_MAX_SCAN_CHANS = 32,
    ACFG_LTEU_MAX_BINS = 10,
    ACFG_MAX_PSK_LEN = 65,
    ACFG_MAX_WEP_KEY_LEN = 256,
    ACFG_MAX_ACL_NODE = 64,
    ACFG_MAX_VAPS = 32,
    ACFG_MAX_CHANS     = 255,
    ACFG_CUSTDATA_LENGTH = 20,
    ACFG_MAX_SAE_OWE_GROUPS = 14,    /* Max of 5 sae/owe groups */
    ACFG_BSSID_ADDR_LEN = 17,
};

/**
 * @Types of data
 */
enum {
    ACFG_TYPE_STR = 1,
    ACFG_TYPE_INT,
    ACFG_TYPE_INT16,
    ACFG_TYPE_CHAR,
    ACFG_TYPE_MACADDR,
    ACFG_TYPE_ACL,
};


#define    ACFG_OPMODE_STA  IEEE80211_M_STA        /**< Infrastructure station */
#define    ACFG_OPMODE_IBSS  IEEE80211_M_IBSS       /**< IBSS (adhoc) station */
#define    ACFG_OPMODE_AHDEMO  IEEE80211_M_AHDEMO     /**< Old lucent compatible adhoc demo */
#define    ACFG_OPMODE_HOSTAP  IEEE80211_M_HOSTAP     /**< Software Access Point*/
#define    ACFG_OPMODE_MONITOR  IEEE80211_M_MONITOR     /**< Monitor mode*/
#define    ACFG_OPMODE_WDS  IEEE80211_M_WDS        /**< WDS link */
#define    ACFG_OPMODE_INVALID -1

typedef enum ieee80211_opmode acfg_opmode_t;
typedef struct ieee80211_rateset acfg_rateset_t;
typedef struct _ieee80211_ssid acfg_ssid_t;
typedef struct ieee80211_clone_params acfg_vapinfo_t;
typedef uint32_t  acfg_shortgi_t;
typedef uint32_t  acfg_ampdu_t;
typedef uint32_t  acfg_amsdu_t;
typedef uint32_t  acfg_ampdu_frames_t;
typedef uint32_t  acfg_ampdu_limit_t;
typedef uint32_t  acfg_freq_t;
typedef uint32_t  acfg_rssi_t;
typedef uint32_t  acfg_rts_t;
typedef uint32_t  acfg_frag_t;
typedef uint32_t  acfg_txpow_t;
typedef uint8_t   acfg_chan_t;
typedef uint32_t  acfg_phymode_t;
typedef uint32_t  acfg_rate_t;

/**
 * @brief Mac address
 */
typedef struct acfg_macaddr {
    uint8_t addr[ACFG_MACADDR_LEN] ;
} acfg_macaddr_t;

/**
 * brief PHY modes for VAP
 */
enum acfg_phymode {
     ACFG_PHYMODE_AUTO             = 0,  /**< autoselect */
     ACFG_PHYMODE_11A              = 1,  /**< 5GHz, OFDM */
     ACFG_PHYMODE_11B              = 2,  /**< 2GHz, CCK */
     ACFG_PHYMODE_11G              = 3,  /**< 2GHz, OFDM */
     ACFG_PHYMODE_FH               = 4,  /**< 2GHz, GFSK */
     ACFG_PHYMODE_TURBO_A          = 5,  /**< 5GHz dyn turbo*/
     ACFG_PHYMODE_TURBO_G          = 6,  /**< 2GHz dyn turbo*/
     ACFG_PHYMODE_11NA_HT20        = 7,  /**< 5Ghz, HT20 */
     ACFG_PHYMODE_11NG_HT20        = 8,  /**< 2Ghz, HT20 */
     ACFG_PHYMODE_11NA_HT40PLUS    = 9,  /**< 5Ghz, HT40 (ext ch +1) */
     ACFG_PHYMODE_11NA_HT40MINUS   = 10,  /**< 5Ghz, HT40 (ext ch -1) */
     ACFG_PHYMODE_11NG_HT40PLUS    = 11,  /**< 2Ghz, HT40 (ext ch +1) */
     ACFG_PHYMODE_11NG_HT40MINUS   = 12, /**< 2Ghz, HT40 (ext ch -1) */
     ACFG_PHYMODE_11NG_HT40        = 13, /**< 2Ghz, HT40 auto */
     ACFG_PHYMODE_11NA_HT40        = 14, /**< 5Ghz, HT40 auto */
     ACFG_PHYMODE_11AC_VHT20       = 15, /* 5Ghz, VHT20 */
     ACFG_PHYMODE_11AC_VHT40PLUS   = 16, /* 5Ghz, VHT40 (Ext ch +1) */
     ACFG_PHYMODE_11AC_VHT40MINUS  = 17, /* 5Ghz  VHT40 (Ext ch -1) */
     ACFG_PHYMODE_11AC_VHT40       = 18, /* 5Ghz, VHT40 */
     ACFG_PHYMODE_11AC_VHT80       = 19, /* 5Ghz, VHT80 */
     ACFG_PHYMODE_11AC_VHT160      = 20,   /* 5Ghz, VHT160 */
     ACFG_PHYMODE_11AC_VHT80_80    = 21,   /* 5Ghz, VHT80_80 */

     /* keep it last */
     ACFG_PHYMODE_INVALID                /**< Invalid Phymode */
};

#include <acfg_event_types.h>

#define  ACFG_PHYMODE_INVALID IEEE80211_MODE_MAX               /**< Invalid Phymode */
#define ACFG_RTS_INVALID -1
#define ACFG_FRAG_INVALID -1

#define    ACFG_VENDOR_PARAM_LEN  100    /**< Vendor param len */
#define    ACFG_MAX_VENDOR_PARAMS  20    /**< Max Vendor params */

/* vendors can fill this enum for commands, below cmds are only for sample, it can be removed */
enum {
    ACFG_VENDOR_PARAM_CMD_PRINT = 1,
    ACFG_VENDOR_PARAM_CMD_INT,
    ACFG_VENDOR_PARAM_CMD_MAC,
};
typedef uint32_t acfg_vendor_param_vap_t;

/* vendors can fill this union for typecasting later in application */
typedef union {
    uint8_t data[ACFG_VENDOR_PARAM_LEN];
} acfg_vendor_param_data_t;

typedef struct acfg_vendor_param_req {
    acfg_vendor_param_vap_t  param;
    uint32_t               type;
    acfg_vendor_param_data_t data;
} acfg_vendor_param_req_t;

#define ACFG_VENDOR_PARAM_NAME_LEN 32

/* vendor param name/type/len are not known */
typedef struct {
    acfg_vendor_param_vap_t cmd;
    uint32_t type;
    uint32_t len;
    acfg_vendor_param_data_t data;
} acfg_wlan_profile_vendor_param_t;

typedef uint32_t acfg_vendor_param_init_flag_t;

/* vendors need to create this table */
typedef struct {
    uint8_t name[ACFG_VENDOR_PARAM_NAME_LEN];
    acfg_vendor_param_vap_t cmd;
}acfg_vendor_param_cmd_map_t;

enum {
    ACFG_PARAM_WMMPARAMS_CWMIN   = 1,
    ACFG_PARAM_WMMPARAMS_CWMAX   = 2,
    ACFG_PARAM_WMMPARAMS_AIFS    = 3,
    ACFG_PARAM_WMMPARAMS_TXOPLIMIT   = 4,
    ACFG_PARAM_WMMPARAMS_ACM     = 5,
    ACFG_PARAM_WMMPARAMS_NOACKPOLICY = 6,
};

enum acfg_param_vap {
    ACFG_PARAM_VAP_SHORT_GI         = IEEE80211_PARAM_SHORT_GI,
    ACFG_PARAM_VAP_AMPDU            = IEEE80211_PARAM_AMPDU,
    ACFG_PARAM_AUTHMODE             = IEEE80211_PARAM_AUTHMODE,
    ACFG_PARAM_ROAMING              = IEEE80211_PARAM_ROAMING,
    ACFG_PARAM_DROPUNENCRYPTED      = IEEE80211_PARAM_DROPUNENCRYPTED,
    ACFG_PARAM_MACCMD               = IEEE80211_PARAM_MACCMD,
    ACFG_PARAM_HIDESSID             = IEEE80211_PARAM_HIDESSID,
    ACFG_PARAM_APBRIDGE             = IEEE80211_PARAM_APBRIDGE,
    ACFG_PARAM_DTIM_PERIOD          = IEEE80211_PARAM_DTIM_PERIOD,
    ACFG_PARAM_BEACON_INTERVAL      = IEEE80211_PARAM_BEACON_INTERVAL,
    ACFG_PARAM_BURST                = IEEE80211_PARAM_BURST,
    ACFG_PARAM_PUREG                = IEEE80211_PARAM_PUREG,
    ACFG_PARAM_WDS                  = IEEE80211_PARAM_WDS,
    ACFG_PARAM_UAPSD                = IEEE80211_PARAM_UAPSDINFO,
    ACFG_PARAM_COUNTRY_IE           = IEEE80211_PARAM_COUNTRY_IE,
    ACFG_PARAM_CHANBW               = IEEE80211_PARAM_CHANBW,
    ACFG_PARAM_SHORTPREAMBLE        = IEEE80211_PARAM_SHORTPREAMBLE,
    ACFG_PARAM_CWM_ENABLE           = IEEE80211_PARAM_CWM_ENABLE,
    ACFG_PARAM_AMSDU                = IEEE80211_PARAM_AMSDU,
    ACFG_PARAM_COUNTRYCODE          = IEEE80211_PARAM_COUNTRYCODE,
    ACFG_PARAM_SETADDBAOPER         = IEEE80211_PARAM_SETADDBAOPER,
    ACFG_PARAM_11N_RATE             = IEEE80211_PARAM_11N_RATE,
    ACFG_PARAM_11N_RETRIES          = IEEE80211_PARAM_11N_RETRIES,
    ACFG_PARAM_DBG_LVL              = IEEE80211_PARAM_DBG_LVL,
    ACFG_PARAM_STA_FORWARD          = IEEE80211_PARAM_STA_FORWARD,
    ACFG_PARAM_ME                   = IEEE80211_PARAM_ME,
    ACFG_PARAM_MEDUMP               = IEEE80211_PARAM_MEDUMP,
    ACFG_PARAM_MEDEBUG              = IEEE80211_PARAM_MEDEBUG,
    ACFG_PARAM_ME_SNOOPLEN          = IEEE80211_PARAM_ME_SNOOPLENGTH,
    ACFG_PARAM_ME_TIMEOUT           = IEEE80211_PARAM_ME_TIMEOUT,
    ACFG_PARAM_PUREN                = IEEE80211_PARAM_PUREN,
    ACFG_PARAM_NO_EDGE_CH           = IEEE80211_PARAM_NO_EDGE_CH,
    ACFG_PARAM_WEP_TKIP_HT          = IEEE80211_PARAM_WEP_TKIP_HT,
    ACFG_PARAM_GREEN_AP_PS_ENABLE   = IEEE80211_IOCTL_GREEN_AP_PS_ENABLE,
    ACFG_PARAM_GREEN_AP_PS_TIMEOUT  = IEEE80211_IOCTL_GREEN_AP_PS_TIMEOUT,
    ACFG_PARAM_WPS                  = IEEE80211_PARAM_WPS,
    ACFG_PARAM_CHWIDTH              = IEEE80211_PARAM_CHWIDTH,
    ACFG_PARAM_EXTAP                = IEEE80211_PARAM_EXTAP,
    ACFG_PARAM_DISABLECOEXT         = IEEE80211_PARAM_COEXT_DISABLE,
    ACFG_PARAM_PERIODIC_SCAN        = IEEE80211_PARAM_PERIODIC_SCAN,
    ACFG_PARAM_SW_WOW               = IEEE80211_PARAM_SW_WOW,
    ACFG_PARAM_SCANVALID            = IEEE80211_PARAM_SCANVALID,
    ACFG_PARAM_AUTO_ASSOC           = IEEE80211_PARAM_AUTO_ASSOC,
    ACFG_PARAM_SCANBAND             = IEEE80211_PARAM_SCAN_BAND,
    ACFG_PARAM_QBSS_LOAD            = IEEE80211_PARAM_QBSS_LOAD,
    ACFG_PARAM_RRM_CAP              = IEEE80211_PARAM_RRM_CAP,
    ACFG_PARAM_HIDE_SSID            = IEEE80211_PARAM_HIDESSID,
    ACFG_PARAM_DOTH                 = IEEE80211_PARAM_DOTH,
    ACFG_PARAM_COEXT_DISABLE        = IEEE80211_PARAM_COEXT_DISABLE,
    ACFG_PARAM_AMPDU                = IEEE80211_PARAM_AMPDU,
    ACFG_PARAM_UCASTCIPHERS         = IEEE80211_PARAM_UCASTCIPHERS,
    ACFG_PARAM_MAXSTA               = IEEE80211_PARAM_MAXSTA,
    ACFG_PARAM_RRM_STATS            = IEEE80211_PARAM_RRM_STATS,
    ACFG_PARAM_RRM_SLWINDOW         = IEEE80211_PARAM_RRM_SLWINDOW,
    ACFG_PARAM_PROXYARP_CAP         = IEEE80211_PARAM_PROXYARP_CAP,
    ACFG_PARAM_DGAF_DISABLE         = IEEE80211_PARAM_DGAF_DISABLE,
    ACFG_PARAM_L2TIF_CAP            = IEEE80211_PARAM_L2TIF_CAP,
    ACFG_PARAM_WNM_ENABLE           = IEEE80211_PARAM_WNM_CAP,
    ACFG_PARAM_WNM_BSSMAX           = IEEE80211_PARAM_WNM_BSS_CAP,
    ACFG_PARAM_WNM_TFS              = IEEE80211_PARAM_WNM_TFS_CAP,
    ACFG_PARAM_WNM_TIM              = IEEE80211_PARAM_WNM_TIM_CAP,
    ACFG_PARAM_WNM_SLEEP            = IEEE80211_PARAM_WNM_SLEEP,
    ACFG_PARAM_RRM_DEBUG            = IEEE80211_PARAM_RRM_DEBUG,
    ACFG_PARAM_VHT_MCS              = IEEE80211_PARAM_VHT_MCS,
    ACFG_PARAM_NSS                  = IEEE80211_PARAM_NSS,
    ACFG_PARAM_LDPC                 = IEEE80211_PARAM_LDPC,
    ACFG_PARAM_TX_STBC              = IEEE80211_PARAM_TX_STBC,
    ACFG_PARAM_PURE11AC             = IEEE80211_PARAM_PURE11AC,
    ACFG_PARAM_OPMODE_NOTIFY        = IEEE80211_PARAM_OPMODE_NOTIFY,
    ACFG_PARAM_ENABLE_RTSCTS        = IEEE80211_PARAM_ENABLE_RTSCTS,
    ACFG_PARAM_MAX_AMPDU            = IEEE80211_PARAM_MAX_AMPDU,
    ACFG_PARAM_VHT_MAX_AMPDU        = IEEE80211_PARAM_VHT_MAX_AMPDU,
    ACFG_PARAM_EXT_IFACEUP_ACS      = IEEE80211_PARAM_EXT_IFACEUP_ACS,
    ACFG_PARAM_VAP_ENHIND           = IEEE80211_PARAM_VAP_ENHIND,
    ACFG_PARAM_SHORT_SLOT           = IEEE80211_PARAM_SHORT_SLOT,   /* Set short slot time */
    ACFG_PARAM_ERP                  = IEEE80211_PARAM_SET_ERP,   /* Set ERP protection */
#if QCA_AIRTIME_FAIRNESS
    ACFG_PARAM_ATF_OPT              = IEEE80211_PARAM_ATF_OPT,
    ACFG_PARAM_ATF_TXBUF_SHARE      = IEEE80211_PARAM_ATF_TXBUF_SHARE,
    ACFG_PARAM_ATF_TXBUF_MAX        = IEEE80211_PARAM_ATF_TXBUF_MAX,
    ACFG_PARAM_ATF_TXBUF_MIN        = IEEE80211_PARAM_ATF_TXBUF_MIN,
    ACFG_PARAM_ATF_PER_UNIT         = IEEE80211_PARAM_ATF_PER_UNIT,
    ACFG_PARAM_ATF_MAX_CLIENT       = IEEE80211_PARAM_ATF_MAX_CLIENT,
#endif
    ACFG_PARAM_CONFIGURE_SELECTIVE_VHTMCS_FOR_VAP = IEEE80211_PARAM_CONFIGURE_SELECTIVE_VHTMCS_FOR_VAP,
    ACFG_PARAM_DISABLE_SELECTIVE_HTMCS_FOR_VAP = IEEE80211_PARAM_DISABLE_SELECTIVE_HTMCS_FOR_VAP,
#if UMAC_SUPPORT_ACL
    ACFG_PARAM_MACCMD_SEC           = IEEE80211_PARAM_MACCMD_SEC,
#endif
    ACFG_PARAM_DISABLE_SELECTIVE_LEGACY_RATE = IEEE80211_PARAM_DISABLE_SELECTIVE_LEGACY_RATE_FOR_VAP,    /* Enable/Disable selective Legacy Rates for this vap. */
    ACFG_PARAM_MODIFY_BEACON_RATE   = IEEE80211_PARAM_BEACON_RATE_FOR_VAP,    /* Control beacon rate*/
#if DYNAMIC_BEACON_SUPPORT
    ACFG_PARAM_DBEACON_EN           = IEEE80211_PARAM_DBEACON_EN,
    ACFG_PARAM_DBEACON_RSSI_THR     = IEEE80211_PARAM_DBEACON_RSSI_THR,
    ACFG_PARAM_DBEACON_TIMEOUT      = IEEE80211_PARAM_DBEACON_TIMEOUT,
#endif
    ACFG_PARAM_SIFS_TRIGGER         = IEEE80211_PARAM_SIFS_TRIGGER,
#if ATH_SUPPORT_NR_SYNC
    ACFG_PARAM_NR_SHARE_RADIO_FLAG  = IEEE80211_PARAM_NR_SHARE_RADIO_FLAG,
#endif
    ACFG_PARAM_SIFS_TRIGGER_RATE    = IEEE80211_PARAM_SIFS_TRIGGER_RATE,      /* Control sifs_trigger rate */
    ACFG_PARAM_IMPLICITBF           = IEEE80211_PARAM_IMPLICITBF,
};
typedef uint32_t acfg_param_vap_t ;

#define ATH_PARAM_SHIFT     0x1000
#define SPECIAL_PARAM_SHIFT     0x2000
enum acfg_param_radio {
    ACFG_PARAM_RADIO_TXCHAINMASK          = 1  | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_RXCHAINMASK          = 2  | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_TXCHAINMASK_LEGACY   = 3  | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_RXCHAINMASK_LEGACY   = 4  | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_AMPDU                = 6  | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_AMPDU_LIMIT          = 7  | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_AMPDU_SUBFRAMES      = 8  | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_TXPOWER_LIMIT2G      = 12 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_TXPOWER_LIMIT5G      = 13 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_ATH_DEBUG            = 19 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_TXPOWER_SCALE_DA     = 20 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_GPIO_LED_CUSTOM      = 37 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_SWAP_DEFAULT_LED     = 38 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_VOWEXT               = 40 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_RCA                  = 41 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_VSP_ENABLE           = 42 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_VSP_THRESHOLD        = 43 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_VSP_EVALINTERVAL     = 44 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_VOWEXT_STATS         = 45 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_BRIDGE               = 48 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_PHYRESTART_WAR       = 70 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_KEYSEARCH_ALWAYS_WAR = 72 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_AMPDU_RX_BSIZE       = 74 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_SMARTANTENNA         = 76 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_AGGR_BURST 	  = 77 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_AGGR_BURST_DUR       = 78 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_BEACON_BURST_MODE    = 80 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_DCS_ENABLE           = 82 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_DCS_COCH             = 93 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_DCS_TXERR            = 94 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_DCS_PHYERR           = 95 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_SMARTANT_TRAIN_MODE        = 96 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_SMARTANT_TRAIN_TYPE        = 97 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_SMARTANT_PKT_LEN           = 98 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_SMARTANT_NUM_PKTS          = 99 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_SMARTANT_TRAIN_START       = 100 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_SMARTANT_NUM_ITR           = 101 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_SMARTANT_CURRENT_ANT       = 102 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_SMARTANT_DEFAULT_ANT       = 103 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_SMARTANT_TRAFFIC_GEN_TIMER = 104 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_SMARTANT_RETRAIN           = 105 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_SMARTANT_RETRAIN_THRESH    = 106 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_SMARTANT_RETRAIN_INTERVAL  = 107 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_SMARTANT_RETRAIN_DROP      = 108 | ATH_PARAM_SHIFT,

    /* Minimum Good put threshold to tigger performance training */
    ACFG_PARAM_RADIO_SMARTANT_MIN_GP_THRESH     = 109 | ATH_PARAM_SHIFT,

    /* Number of intervals Good put need to be averaged
     * to use in performance training tigger
     */
    ACFG_PARAM_RADIO_SMARTANT_GP_AVG_INTERVAL   = 110 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_NODEBUG    = 113 | ATH_PARAM_SHIFT,
    /*
     * Enable Acs back Ground Channel selection Scan timer in AP mode
     * */
    ACFG_PARAM_RADIO_ACS_ENABLE_BK_SCANTIMER = 118 | ATH_PARAM_SHIFT,
    /*
     * ACS scan timer value in Seconds
     * */
    ACFG_PARAM_RADIO_ACS_SCANTIME = 119 | ATH_PARAM_SHIFT,
    /*
     * Negligence Delta RSSI between two channel
     * */
    ACFG_PARAM_RADIO_ACS_RSSIVAR = 120 | ATH_PARAM_SHIFT,
    /*
     * Negligence Delta Channel load between two channel
     * */
    ACFG_PARAM_RADIO_ACS_CHLOADVAR = 121 | ATH_PARAM_SHIFT,
    /*
     * Enable Limited OBSS check
     * */
    ACFG_PARAM_RADIO_ACS_LIMITEDOBSS = 122 | ATH_PARAM_SHIFT,
    /*
     * Acs control flag for Scan timer
     * */
    ACFG_PARAM_RADIO_ACS_CTRLFLAG   = 123 | ATH_PARAM_SHIFT,
    /*
     * Acs Run time Debug level
     * */
    ACFG_PARAM_RADIO_ACS_DEBUGTRACE = 124  | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_FW_HANG_ID     = 137  | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_STADFS_ENABLE  = 300  | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_ATF_STRICT_SCHED = 301 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_ATF_OBSS_SCHED = 306 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_ATF_OBSS_SCALE = 307  | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_ENABLE_AMSDU   = 145  | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_ENABLE_AMPDU   = 146  | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_TXPOWER_SCALE_OL   = 171 | ATH_PARAM_SHIFT,
     ACFG_PARAM_RADIO_FW_RECOVERY_ID = 180  | ATH_PARAM_SHIFT,

    ACFG_PARAM_RADIO_COUNTRYID            = 00 | SPECIAL_PARAM_SHIFT,
    ACFG_PARAM_RADIO_REGDOMAIN            = 12 | SPECIAL_PARAM_SHIFT,
    ACFG_PARAM_RADIO_ENABLE_OL_STATS      = 13 | SPECIAL_PARAM_SHIFT,
    ACFG_PARAM_RADIO_ENABLE_MAC_REQ       = 14 | SPECIAL_PARAM_SHIFT,
    ACFG_PARAM_RADIO_ENABLE_SHPREAMBLE    = 15 | SPECIAL_PARAM_SHIFT,
    ACFG_PARAM_RADIO_ENABLE_SHSLOT        = 16 | SPECIAL_PARAM_SHIFT,
    ACFG_PARAM_RADIO_MGMT_RETRY_LIMIT     = 17 | SPECIAL_PARAM_SHIFT,

    ACFG_PARAM_RADIO_SENS_LEVEL           = 18 | SPECIAL_PARAM_SHIFT,
    ACFG_PARAM_RADIO_TX_POWER_5G          = 19 | SPECIAL_PARAM_SHIFT,
    ACFG_PARAM_RADIO_TX_POWER_2G          = 20 | SPECIAL_PARAM_SHIFT,
    ACFG_PARAM_RADIO_CCA_THRESHOLD        = 21 | SPECIAL_PARAM_SHIFT,

    ACFG_PARAM_RADIO_DYN_TX_CHAINMASK     = 73 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_AMSDU                = 26 | ATH_PARAM_SHIFT,
    ACFG_PARAM_HAL_CONFIG_FORCEBIASAUTO   = 6,
    ACFG_PARAM_HAL_CONFIG_DEBUG           = 32,
    ACFG_PARAM_HAL_CONFIG_CRDC_ENABLE     = 36,
    ACFG_PARAM_HAL_CONFIG_CRDC_WINDOW     = 37,
    ACFG_PARAM_HAL_CONFIG_CRDC_DIFFTHRESH = 38,
    ACFG_PARAM_HAL_CONFIG_CRDC_RSSITHRESH = 39,
    ACFG_PARAM_HAL_CONFIG_CRDC_NUMERATOR  = 40,
    ACFG_PARAM_HAL_CONFIG_CRDC_DENOMINATOR= 41,
    ACFG_PARAM_HAL_CONFIG_PRINT_BBDEBUG   = 45,
    ACFG_PARAM_HAL_CONFIG_ENABLEADAPTIVECCATHRES = 24,
    ACFG_PARAM_HAL_CONFIG_CCA_DETECTION_LEVEL = 5,
    ACFG_PARAM_HAL_CONFIG_CCA_DETECTION_MARGIN = 6,
    ACFG_PARAM_RADIO_PRECAC_ENABLE           = 344 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_PRECAC_TIMEOUT           = 345 | ATH_PARAM_SHIFT,
    ACFG_PARAM_RADIO_PRECAC_INTER_CHANNEL     = 381 | ATH_PARAM_SHIFT,
};
typedef uint32_t acfg_param_radio_t ;
/**
 * @brief Encoding Flags
 */
enum acfg_encode_flags {
    ACFG_ENCODE_ENABLED    = 0x0000,
    ACFG_ENCODE_TEMP       = 0x0400,
    ACFG_ENCODE_NOKEY      = 0x0800,
    ACFG_ENCODE_OPEN       = 0x2000,
    ACFG_ENCODE_RESTRICTED = 0x4000,
    ACFG_ENCODE_DISABLED   = 0x8000,
    ACFG_ENCODE_MODE       = 0xF000,
    ACFG_ENCODE_INDEX      = 0x00FF,
    ACFG_ENCODE_FLAGS      = 0xFF00
};

typedef uint32_t acfg_encode_flags_t ;

/**
 * @brief Encoding information
 */
typedef struct acfg_encode {
    uint8_t *buff ;
    uint32_t len ;
    acfg_encode_flags_t flags ;
} acfg_encode_t ;

#define VAP_ID_AUTO (-1)

typedef enum {
    ACFG_FLAG_ATF_STRICT_SCHED  = 0x01,
    ACFG_FLAG_ATF_OBSS_SCHED    = 0x02,
    ACFG_FLAG_ATF_MAXSTA        = 0x04,
} ATF_SCHED_FLAGS;

typedef struct {
    char atf_percent[ACFG_MAX_PERCENT_SIZE];
    char atf_stalist[ACFG_MAX_ATF_STALIST_SIZE];
} acfg_atf_params_t;


typedef struct acfg_atf_ssid_val {
    uint16_t  id_type;
    uint8_t   ssid[ACFG_MAX_SSID_LEN+1];
    uint32_t  value;
} acfg_atf_ssid_val_t;

typedef struct acfg_atf_sta_val {
    uint16_t    id_type;
    uint8_t     sta_mac[ACFG_MACADDR_LEN];
    uint8_t     ssid[IEEE80211_NWID_LEN+1];
    uint32_t    value;
} acfg_atf_sta_val_t;

typedef struct ieee80211req_sta_info acfg_sta_info_t;

/**
 * @brief Structure used in request to get station information
 */
typedef struct acfg_sta_info_req {
    uint16_t len ;        /**< Length of buffer */
    acfg_sta_info_t *info ; /**< Pointer to array of acfg_sta_info_t */
} acfg_sta_info_req_t ;


typedef enum {
    ACFG_TX99_ENABLE,
    ACFG_TX99_DISABLE,
    ACFG_TX99_SET_FREQ,
    ACFG_TX99_SET_RATE,
    ACFG_TX99_SET_POWER,
    ACFG_TX99_SET_TXMODE,
    ACFG_TX99_SET_CHANMASK,
    ACFG_TX99_SET_TYPE,
    ACFG_TX99_GET
} acfg_tx99_type_t;

#define SET_TX99_TX(flags) (flags|=0x1)
#define SET_TX99_RX(flags) (flags&=~0x1)
#define ENABLE_TX99_TX(flags) (flags|=0x2)
#define DISABLE_TX99_TX(flags) (flags&=~0x2)
#define SET_TX99_RX_REPORT(flags) (flags|=0x4)
#define CLEAR_TX99_RX_REPORT(flags) (flags&=~0x4)
#define IS_TX99_TX(flags) ((flags&0x1) == 0x1)
#define IS_TX99_TX_ENABLED(flags) ((flags&0x2) == 0x2)
#define IS_TX99_RX_REPORT(flags) ((flags&0x4) == 0x4)
#define TX99_MAX_PARAM_NUM  18

typedef struct acfg_tx99_data {
    uint32_t freq;	/* tx frequency (MHz) */
    uint32_t chain;   /* tx chain */
    uint32_t htmode;	/* tx bandwidth (HT20/HT40) */
    uint32_t htext;	/* extension channel offset (0(none), 1(plus) and 2(minus)) */
    uint32_t rate;	/* Kbits/s */
    uint32_t rc;	        /* rate code */
    uint32_t power;  	/* (dBm) */
    uint32_t txmode;  /* wireless mode, 11NG(8), auto-select(0) */
    uint32_t chanmask; /* tx chain mask */
    uint32_t type;
    uint32_t flags;    /* flags:  enable/disable, tx/rx, rx_report */
    uint32_t pattern;  /* tx pattern:
                            0: all zeros;
                            1: all ones;
                            2: repeating 10;
                            3: PN7;
                            4: PN9;
                            5: PN15
                        */
    uint32_t shortguard; /* shortguard */
    uint8_t  mode[16]; /* wireless mode string */
} acfg_tx99_data_t;

typedef struct acfg_tx99 {
    char                if_name[ACFG_MAX_IFNAME];/**< Interface name */
    acfg_tx99_type_t    type;             /**< Type of wcmd */
    acfg_tx99_data_t    data;             /**< Data */
} acfg_tx99_t;


#define MGMT_FRAME_MAX_SIZE 1500
typedef struct acfg_mgmt_rx_info{
    uint8_t    raw_mgmt_frame[MGMT_FRAME_MAX_SIZE];
    int32_t    ri_rssi;
    int32_t    ri_datarate;
    int32_t    ri_flags;
    uint16_t   ri_channel;
} acfg_mgmt_rx_info_t;

#define CTL_5G_SIZE 1536
#define CTL_2G_SIZE 684
#define MAX_CTL_SIZE (CTL_5G_SIZE > CTL_2G_SIZE ? CTL_5G_SIZE : CTL_2G_SIZE)

typedef struct acfg_ctl_table{
    int32_t   len;
    int8_t   band;
    uint8_t   ctl_tbl[MAX_CTL_SIZE];
} acfg_ctl_table_t;

#define MAX_NUM_CHAINS   4
#define MAX_RXG_CAL_CHANS 4
typedef struct acfg_nf_dbr_dbm {
    int8_t nfdbr[MAX_RXG_CAL_CHANS*MAX_NUM_CHAINS];
    int8_t nfdbm[MAX_RXG_CAL_CHANS*MAX_NUM_CHAINS];
    uint32_t  freqNum[MAX_RXG_CAL_CHANS];
} acfg_nf_dbr_dbm_t;
#undef MAX_NUM_CHAINS
#undef MAX_RXG_CAL_CHANS

/**
 * @brief Command Request per OS
 */
enum acfg_req_cmd {
    /*
     * *****************************
     * Wifi specific Request Numbers
     * *****************************
     */
    ACFG_REQ_FIRST_WIFI,        /**< First Radio request marker */

    ACFG_REQ_CREATE_VAP,        /**< Create VAP */
    ACFG_REQ_SET_RADIO_PARAM,   /**< Set Radio Parameters */
    ACFG_REQ_GET_RADIO_PARAM,   /**< Get Radio Parameters */
    ACFG_REQ_SET_REG,           /**< Set target register */
    ACFG_REQ_GET_REG,           /**< Get target register */
    ACFG_REQ_TX99TOOL,          /**< tx99tool */
    ACFG_REQ_SET_HW_ADDR,       /**< Set target wifi0 hw address */
    ACFG_REQ_GET_ATHSTATS,      /**< Get athstats */
    ACFG_REQ_CLR_ATHSTATS,      /**< Clear athstats */
    ACFG_REQ_GET_PROFILE,       /**< Get Radio info */
    ACFG_REQ_SET_COUNTRY,       /**< Set Country */
    ACFG_REQ_SET_TX_POWER,      /* Set mode specific tx power */
    ACFG_REQ_SET_SENS_LEVEL,    /* Set sensitivity level */
    ACFG_REQ_GPIO_CONFIG,       /* GPIO config */
    ACFG_REQ_GPIO_SET,          /* GPIO set */
    ACFG_REQ_SET_CTL_TABLE,     /* Set CTL table */
    ACFG_REQ_SET_OP_SUPPORT_RATES,   /* Set basic&supported rates and use lowest basic rate for mcast/bcast/mgmt */
    ACFG_REQ_GET_RADIO_SUPPORTED_RATES, /* Get supported rates for this radio */
    ACFG_REQ_GET_NF_DBR_DBM_INFO,  /*Get NF dBr and dBm info */
    ACFG_REQ_GET_PACKET_POWER_INFO,  /*Get per packet power dBm */

    ACFG_REQ_LAST_WIFI,         /**< Last Radio request marker */

    /*
     * *****************************
     * VAP specific Request Numbers
     * *****************************
     */
    ACFG_REQ_FIRST_VAP,         /**< First VAP request marker */

    ACFG_REQ_SET_SSID,          /**< Set the SSID */
    ACFG_REQ_GET_SSID,          /**< Get the SSID */
    ACFG_REQ_SET_CHANNEL,       /**< Set the channel (ieee) */
    ACFG_REQ_GET_CHANNEL,       /**< Set the channel (ieee) */
    ACFG_REQ_DELETE_VAP,        /**< Delete VAP */
    ACFG_REQ_SET_OPMODE,        /**< Set the Opmode */
    ACFG_REQ_GET_OPMODE,        /**< Get the Opmode */
    ACFG_REQ_SET_AMPDU,         /**< Set AMPDU setting */
    ACFG_REQ_GET_AMPDU,         /**< Get Current AMPDU setting */
    ACFG_REQ_SET_AMSDU,         /**< Set AMSDU setting */
    ACFG_REQ_GET_AMSDU,         /**< Get Current AMSDU setting */
    ACFG_REQ_SET_AMPDUFRAMES,   /**< Set AMPDU Frames */
    ACFG_REQ_GET_AMPDUFRAMES,   /**< Set AMPDU Frames */
    ACFG_REQ_SET_AMPDULIMIT,    /**< Set AMPDU Limit */
    ACFG_REQ_GET_AMPDULIMIT,    /**< Get AMPDU Limit */
    ACFG_REQ_SET_SHORTGI,       /**< Set Shortgi */
    ACFG_REQ_GET_SHORTGI,       /**< Get Shortgi setting */
    ACFG_REQ_SET_FREQUENCY,     /**< Set Frequency */
    ACFG_REQ_GET_FREQUENCY,     /**< Set Frequency */
    ACFG_REQ_GET_NAME,          /**< Get Wireless Name */
    ACFG_REQ_SET_RTS,           /**< Set RTS Threshold */
    ACFG_REQ_GET_RTS,           /**< Get RTS Threshold */
    ACFG_REQ_SET_FRAG,          /**< Set Fragmentation Threshold */
    ACFG_REQ_GET_FRAG,          /**< Get Fragmentation Threshold */
    ACFG_REQ_SET_TXPOW,         /**< Set default Tx Power */
    ACFG_REQ_GET_TXPOW,         /**< Get default Tx Power */
    ACFG_REQ_SET_AP,            /**< Set AP mac addr */
    ACFG_REQ_GET_AP,            /**< Get AP mac addr */
    ACFG_REQ_GET_RANGE,         /**< Get range of parameters */
    ACFG_REQ_GET_ENCODE,        /**< Get encoding token and mode */
    ACFG_REQ_SET_VAP_PARAM,     /**< Set Vap Parameters */
    ACFG_REQ_GET_VAP_PARAM,     /**< Get Vap Parameters */
    ACFG_REQ_GET_RATE,          /**< Get default bit rate */
    ACFG_REQ_SET_SCAN,          /**< Set Scan Request */
    ACFG_REQ_GET_SCANRESULTS,   /**< Get Scan Results */
    ACFG_REQ_GET_STATS,         /**< Get wireless statistics */
    ACFG_REQ_SET_PHYMODE,       /**< Set phymode */
    ACFG_REQ_GET_ASSOC_STA_INFO,/**< Get info on associated stations */
    ACFG_REQ_SET_ENCODE,        /**< Set encoding token and mode */
    ACFG_REQ_SET_RATE,          /**< Set default bit rate */
    ACFG_REQ_SET_POWMGMT,       /**< Set Power Management */
    ACFG_REQ_GET_POWMGMT,       /**< Get Power Management */
    ACFG_REQ_SET_CHMODE,        /**< Set Channel Mode */
    ACFG_REQ_GET_CHMODE,        /**< Get Channel Mode */
    ACFG_REQ_GET_RSSI,          /**< Get the RSSI */
    ACFG_REQ_SET_TESTMODE,      /**< Set the TESTMODE */
    ACFG_REQ_GET_TESTMODE,      /**< Get the TESTMODE */
    ACFG_REQ_GET_CUSTDATA,      /**< Get the CUSTDATA */
    ACFG_REQ_SET_VAP_WMMPARAMS, /**< Set Vap WMM Parameters */
    ACFG_REQ_GET_VAP_WMMPARAMS, /**< Get Vap WMM Parameters */
    ACFG_REQ_NAWDS_CONFIG,      /**< Get Nawds config */
    ACFG_REQ_DOTH_CHSWITCH,     /**< Channel Switch */
    ACFG_REQ_ADDMAC,            /**< Add ACL entry */
    ACFG_REQ_DELMAC,            /**< Delete ACL entry */
    ACFG_REQ_KICKMAC,           /**< Disassociate station */
    ACFG_REQ_GET_PHYMODE,       /**< Get phymode */
    ACFG_REQ_ACL_ADDMAC,        /**< MAC ACL addmac */
    ACFG_REQ_ACL_GETMAC,        /**< MAC ACL getmac */
    ACFG_REQ_ACL_DELMAC,        /**< MAC ACL delmac */
    ACFG_REQ_ACL_ADDMAC_SEC,    /**< MAC secondary ACL addmac */
    ACFG_REQ_ACL_GETMAC_SEC,    /**< MAC secondary ACL getmac */
    ACFG_REQ_ACL_DELMAC_SEC,    /**< MAC secondary ACL delmac */
    ACFG_REQ_GET_VAP_ACL,
    ACFG_REQ_WDS_ADDR_SET,
    ACFG_REQ_IS_OFFLOAD_VAP,
    ACFG_REQ_ADD_CLIENT,
    ACFG_REQ_DEL_CLIENT,
    ACFG_REQ_AUTHORIZE_CLIENT,
    ACFG_REQ_PHYERR,

    /* security request */
    ACFG_REQ_WSUPP_INIT,
    ACFG_REQ_WSUPP_FINI,
    ACFG_REQ_WSUPP_IF_ADD,
    ACFG_REQ_WSUPP_IF_RMV,
    ACFG_REQ_WSUPP_NW_CRT,
    ACFG_REQ_WSUPP_NW_DEL,
    ACFG_REQ_WSUPP_NW_SET,
    ACFG_REQ_WSUPP_NW_GET,
    ACFG_REQ_WSUPP_NW_LIST,
    ACFG_REQ_WSUPP_WPS_REQ,
    ACFG_REQ_WSUPP_SET,

    ACFG_REQ_SET_MLME,
    ACFG_REQ_GET_WPA_IE,
    ACFG_REQ_SET_FILTERFRAME,
    ACFG_REQ_SET_APPIEBUF,
    ACFG_REQ_SET_KEY,
    ACFG_REQ_DEL_KEY,
    ACFG_REQ_SET_OPTIE,
    ACFG_REQ_SET_ACPARAMS,
    ACFG_REQ_GET_CHAN_INFO,
    ACFG_REQ_GET_CHAN_LIST,
    ACFG_REQ_GET_MAC_ADDR,
    ACFG_REQ_SET_VAP_P2P_PARAM,
    ACFG_REQ_GET_VAP_P2P_PARAM,
    ACFG_REQ_DBGREQ,
    ACFG_REQ_SEND_MGMT,
    ACFG_REQ_SET_VAP_VENDOR_PARAM,
    ACFG_REQ_GET_VAP_VENDOR_PARAM,
    ACFG_REQ_SET_CHNWIDTHSWITCH,
    ACFG_REQ_SET_BEACON_BUF,
    ACFG_REQ_SET_RATEMASK,
    ACFG_REQ_GET_BEACON_SUPPORTED_RATES, /* Get supported rates in beacon IE */
    ACFG_REQ_SET_MU_WHTLIST,
    ACFG_REQ_MON_ADDMAC,
    ACFG_REQ_MON_DELMAC,
    ACFG_REQ_MON_LISTMAC,
    ACFG_REQ_MON_ENABLE_FILTER,
    ACFG_REQ_SET_ATF_ADDSSID,
    ACFG_REQ_SET_ATF_DELSSID,
    ACFG_REQ_SET_ATF_ADDSTA,
    ACFG_REQ_SET_ATF_DELSTA,

    ACFG_REQ_UNUSED,            /**< Unused marker */
    ACFG_REQ_LAST_VAP,          /**< Last VAP request marker */
};
typedef uint32_t  acfg_req_cmd_t;

typedef struct acfg_packet_power{
    int32_t packet_power;
} acfg_packet_power_t;

/* Structure for periodic channel stats event */
typedef struct acfg_chan_stats {
    uint32_t    frequency;     /* Current primary channel centre frequency */
    int32_t     noise_floor;   /* Current noise floor on operating channel */
    uint32_t    obss_util;     /* Other bss utilization for last interval */
    uint32_t    self_bss_util; /* Self bss utilization for last interval */
} acfg_chan_stats_t;

/**
 * Events
 */

/**
 * @brief Event ID(s)
 */
enum acfg_ev_id {
    ACFG_EV_SCAN_DONE           = 0,    /**< Scan Done */
    ACFG_EV_ASSOC_AP            = 1,    /**< Associated with an AP */
    ACFG_EV_ASSOC_STA           = 2,    /**< Station joined an AP */
    ACFG_EV_DISASSOC_AP         = 3,    /**< AP recv disassoc from Station */
    ACFG_EV_DISASSOC_STA        = 4,    /**< Station recv disassoc from AP */
    ACFG_EV_DEAUTH_AP           = 5,    /**< AP recv deauth from Station */
    ACFG_EV_DEAUTH_STA          = 6,    /**< Station recv deauth from AP */
    ACFG_EV_NODE_LEAVE          = 7,    /**< Station leaves the Bss */
    ACFG_EV_AUTH_AP           = 8,
    ACFG_EV_AUTH_STA           = 9,

    ACFG_EV_CHAN_START          = 10,    /**< Radio On Channel */
    ACFG_EV_CHAN_END            = 11,    /**< Radio Off Channel */
    ACFG_EV_RX_MGMT             = 12,    /**< Received Management Frame */
    ACFG_EV_SENT_ACTION         = 13,    /**< Sent Action Frame */
    ACFG_EV_LEAVE_AP            = 14,    /**< STA left AP */
    ACFG_EV_GEN_IE              = 15,    /**< IE */
    ACFG_EV_ASSOC_REQ_IE        = 16,    /**< Assoc Req IE */
    ACFG_EV_MIC_FAIL            = 17,   /**< MIC FAIL EVENT */
    ACFG_EV_PROBE_REQ           = 18,   /**< Probe Request Frame for Overlap PBC Detection */
    ACFG_EV_DEVICE_STATE_CHANGE = 19,   /**< Offload target's State Change */

    ACFG_EV_WSUPP_RAW_MESSAGE               = 100,  /**< WSUPP event: associated */
    ACFG_EV_WSUPP_AP_STA_CONNECTED          = 101,  /**< WSUPP event: associated */
    ACFG_EV_WSUPP_AP_STA_DISCONNECTED          = 102,  /**< WSUPP event: associated */
    ACFG_EV_WSUPP_WPA_EVENT_CONNECTED       = 103,  /**< WSUPP event: WPA connected */
    ACFG_EV_WSUPP_WPA_EVENT_DISCONNECTED    = 104,  /**< WSUPP event: WPA disconnected */
    ACFG_EV_WSUPP_WPA_EVENT_TERMINATING     = 105,  /**< WSUPP event: WPA terminating */
    ACFG_EV_WSUPP_WPA_EVENT_SCAN_RESULTS    = 106,  /**< WSUPP event: WPA get scan result */
    ACFG_EV_WSUPP_WPS_EVENT_ENROLLEE_SEEN   = 107,  /**< WSUPP event: enrollee seen */
    ACFG_EV_WSUPP_ASSOC_REJECT              = 108,
    ACFG_EV_WSUPP_EAP_SUCCESS               = 109,
    ACFG_EV_WSUPP_EAP_FAILURE               = 110,
    ACFG_EV_PUSH_BUTTON                     = 111,
    ACFG_EV_WSUPP_WPS_NEW_AP_SETTINGS       = 112,
    ACFG_EV_WSUPP_WPS_SUCCESS               = 113,
    ACFG_EV_WAPI                            = 114,
    ACFG_EV_RADAR                           = 115,  /**< list of radar freqs */
    ACFG_EV_STA_SESSION                     = 116,  /**< STA Session timeout */
    ACFG_EV_TX_OVERFLOW                     = 117,  /**< Tx overflow event */
    ACFG_EV_GPIO_INPUT                      = 118,  /**< GPIO input */
    ACFG_EV_MGMT_RX_INFO                    = 119,  /**< mgmt rx info */
    ACFG_EV_WDT_EVENT                       = 120,  /**< watchdog events */
    ACFG_EV_NF_DBR_DBM_INFO                 = 121,  /**< NF dbr and dbm info  */
    ACFG_EV_PACKET_POWER_INFO               = 122,  /**< Packet power in dBm info */
    ACFG_EV_CAC_START                       = 123,  /**< DFS CAC start */
    ACFG_EV_UP_AFTER_CAC                    = 124,  /**< VAP up after CAC */
    ACFG_EV_QUICK_KICKOUT                   = 125,  /**< quick kickout */
    ACFG_EV_ASSOC_FAILURE                   = 126,  /* Association failure event */
    ACFG_EV_DIAGNOSTICS                     = 127,  /* Diagnostics event */
    ACFG_EV_CHAN_STATS                      = 128,  /* Channel stats */
    ACFG_EV_EXCEED_MAX_CLIENT		    = 129,  /* Exceed Max Client*/
#if !defined(REMOVE_PKT_LOG) && OL_ATH_SMART_LOGGING
    ACFG_EV_SMART_LOG_FW_PKTLOG_STOP        = 130,  /**< Smart log FW initiated
                                                         packetlog stop event */
#endif /* !defined(REMOVE_PKT_LOG) && OL_ATH_SMART_LOGGING */
};
typedef uint32_t  acfg_ev_id_t;

/**
 * @brief Event Data
 */
typedef union acfg_ev_data {
    acfg_device_state_data_t device_state;
    acfg_scan_done_t    scan;
    acfg_assoc_t     assoc_ap;
    acfg_assoc_t     assoc_sta;
    acfg_dauth_t        dauth;
    acfg_auth_t        auth;
    acfg_disassoc_t     disassoc_ap;
    acfg_disassoc_t     disassoc_sta;
    acfg_wsupp_custom_message_t mic_fail;
    acfg_chan_start_t   chan_start;
    acfg_chan_end_t     chan_end;
    acfg_leave_ap_t     leave_ap;
    acfg_gen_ie_t       gen_ie;
    acfg_radar_t        radar;
    acfg_session_t      session;
    /* security events */
    acfg_wsupp_ap_sta_conn_t   wsupp_ap_sta_conn;
    acfg_wsupp_wpa_conn_t      wsupp_wpa_conn;
    acfg_wsupp_wps_enrollee_t  wsupp_wps_enrollee;
    acfg_wsupp_raw_message_t   wsupp_raw_message;
    acfg_probe_req_t           pr_req;
    acfg_wsupp_assoc_t         wsupp_assoc;
    acfg_wsupp_eap_t           wsupp_eap_status;
    acfg_pbc_ev_t              pbc;
    acfg_wsupp_wps_success_t   wsupp_wps_success;
    acfg_wsupp_wps_new_ap_settings_t wsupp_wps_new_ap_setting;
    acfg_mgmt_rx_info_t        mgmt_ri;
    acfg_wdt_event_t           wdt;
    acfg_nf_dbr_dbm_t          nf_dbr_dbm;
    acfg_packet_power_t        packet_power;
    acfg_kickout_t             kickout;
    acfg_assoc_failure_t       assoc_fail;
    acfg_diag_event_t         diagnostics;
    acfg_chan_stats_t          chan_stats;
#if !defined(REMOVE_PKT_LOG) && OL_ATH_SMART_LOGGING
    acfg_smart_log_fw_pktlog_stop_t slfwpktlog_stop;
#endif /* !defined(REMOVE_PKT_LOG) && OL_ATH_SMART_LOGGING */
} acfg_ev_data_t;


/**
 * @brief OS event reception structure
 */
typedef struct acfg_os_event {
    acfg_ev_id_t        id;             /**< Event ID */
    acfg_ev_data_t      data;           /**< Event Data */
} acfg_os_event_t;

typedef struct acfg_packet_power_param {
    uint16_t rate_flags;
    uint8_t  nss;
    uint8_t  preamble;
    uint8_t  hw_rate;
} acfg_packet_power_param_t;


typedef struct {
    uint8_t stamac[ACFG_MACADDR_LEN];
    uint16_t aid;
    uint8_t qos;
    acfg_rateset_t lrates;
    acfg_rateset_t htrates;
    acfg_rateset_t vhtrates;
} acfg_add_client_t;

typedef struct {
    uint8_t stamac[ACFG_MACADDR_LEN];
} acfg_del_client_t;

typedef struct {
    uint32_t authorize;
    uint8_t  mac[ACFG_MACADDR_LEN];
} acfg_authorize_client_t ;

#define ACFG_KEYBUF_SIZE   16
#define ACFG_MICBUF_SIZE   (8+8)   /* space for both tx+rx keys */

typedef struct {
	uint8_t	cipher;
	uint16_t	keyix;
	uint8_t	keylen;	/* key length in bytes */
	uint8_t	macaddr[ACFG_MACADDR_LEN];
	uint8_t	keydata[ACFG_KEYBUF_SIZE+ACFG_MICBUF_SIZE];
} acfg_set_key_t;

typedef struct {
	uint16_t	keyix;
	uint8_t	macaddr[ACFG_MACADDR_LEN];
} acfg_del_key_t;

typedef struct {
    uint16_t	tidmask;
    uint8_t	macaddr[ACFG_MACADDR_LEN];
} acfg_set_mu_whtlist_t;

typedef struct {
	uint8_t	*buf;
	uint32_t	total_buf_len;
} acfg_set_beacon_buf_t;

typedef struct acfg_rateset_phymode_t {
    acfg_rateset_t          rs;
    acfg_phymode_t          phymode;
} acfg_rateset_phymode_t;


/**
 * @brief Structure used in get/set of param values
 */
typedef struct acfg_param_req {
    uint32_t param;
    uint32_t val;
} acfg_param_req_t ;

typedef struct {
    uint8_t vap_name[ACFG_MAX_IFNAME];
    uint8_t vap_mac[ACFG_MACADDR_LEN];
    uint32_t phymode;
    uint8_t sec_method;
    uint8_t   cipher;
    uint8_t wep_key[ACFG_MAX_PSK_LEN];
    uint8_t wep_key_len;
    uint8_t wep_key_idx;
    uint8_t wds_enabled;
    uint8_t wds_addr[ACFG_MACSTR_LEN];
    uint32_t wds_flags;
} acfg_wlan_profile_vap_info_t;


typedef struct acfg_radio_vap_info {
    uint8_t radio_name[ACFG_MAX_IFNAME];
    uint8_t chan;
    uint32_t freq;
    uint16_t country_code;
    uint8_t radio_mac[ACFG_MACADDR_LEN];
    acfg_wlan_profile_vap_info_t vap_info[ACFG_MAX_VAPS];
    uint8_t num_vaps;
    uint8_t ampdu;
    uint32_t ampdu_limit_bytes;
    uint8_t ampdu_subframes;
    uint8_t amsdu;
} acfg_radio_vap_info_t;

/**
 *  * @brief Structure used in get mac acl list;
 *   */
typedef struct acfg_acl_mac {
    uint32_t num;
    uint8_t  macaddr[ACFG_MAX_ACL_NODE][ACFG_MACADDR_LEN];
} acfg_macacl_t ;

typedef struct ieee80211_wlanconfig  acfg_nawds_cfg_t;

typedef struct acfg_ratemask {
    uint8_t preamble;
    uint32_t mask_lower32;
    uint32_t mask_higher32;
} acfg_ratemask_t;

#define ACFG_MAX_APPIE_BUF 2048

typedef struct ieee80211req_getset_appiebuf acfg_appie_t;

typedef struct acfg_tx_power {
    uint32_t mode;
    int32_t tx_power;
} acfg_tx_power_t;

typedef struct acfg_sens_level {
    int32_t sens_level;
} acfg_sens_level_t;

typedef struct acfg_gpio_config {
    uint32_t gpio_num;
    uint32_t input;
    uint32_t pull_type;
    uint32_t intr_mode;
} acfg_gpio_config_t;

typedef struct acfg_gpio_set {
    uint32_t gpio_num;
    uint32_t set;
} acfg_gpio_set_t;


/**
 * @brief  set/get testmode-info
 */
typedef struct  acfg_testmode {
    uint8_t     bssid[ACFG_MACADDR_LEN];
    int32_t     chan;         /* ChanID */
    uint16_t    operation;    /* Operation */
    uint8_t     antenna;      /* RX-ANT */
    uint8_t     rx;           /* RX START/STOP */
    int32_t     rssi_combined;/* RSSI */
    int32_t     rssi0;        /* RSSI */
    int32_t     rssi1;        /* RSSI */
    int32_t     rssi2;        /* RSSI */
} acfg_testmode_t;

/**
 * @brief get custdata
 */
typedef struct acfg_custdata{
    uint8_t      custdata[ACFG_CUSTDATA_LENGTH];
}acfg_custdata_t;

/**
 * @brief Generic channel mode
 */
typedef struct acfg_chmode {
    uint32_t  len;
    uint8_t   mode[ACFG_MAX_CHMODE_LEN];
} acfg_chmode_t;

/**
 * @brief Buffer descriptor that
 *        holds iw_range data
 */
typedef struct acfg_range {
    uint8_t *buff ;
    uint32_t len ;
} acfg_range_t ;

typedef struct acfg_range_buf {
    uint32_t           throughput;
    uint32_t           min_nwid;
    uint32_t           max_nwid;
    uint16_t           old_num_channels;
    uint8_t            old_num_frequency;
#if WIRELESS_EXT >= 17
    uint32_t           event_capa[6];
#endif
    int32_t            sensitivity;
    struct {
        uint8_t        qual;
        uint8_t        level;
        uint8_t        noise;
        uint8_t        updated;
    } max_qual, avg_qual;
    uint8_t            num_bitrates;
    int32_t            bitrate[ACFG_MAX_BITRATE];
    int32_t            min_rts;
    int32_t            max_rts;
    int32_t            min_frag;
    int32_t            max_frag;
    int32_t            min_pmp;
    int32_t            max_pmp;
    int32_t            min_pmt;
    int32_t            max_pmt;
    uint16_t           pmp_flags;
    uint16_t           pmt_flags;
    uint16_t           pm_capa;
    uint16_t           encoding_size[ACFG_MAX_ENCODING];
    uint8_t            num_encoding_sizes;
    uint8_t            max_encoding_tokens;
    uint8_t            encoding_login_index;
    uint16_t           txpower_capa;
    uint8_t            num_txpower;
#if WIRELESS_EXT >= 10
    int32_t            txpower[ACFG_MAX_TXPOWER];
#endif
    uint8_t            we_version_compiled;
    uint8_t            we_version_source;
    uint16_t           retry_capa;
    uint16_t           retry_flags;
    uint16_t           r_time_flags;
    int32_t            min_retry;
    int32_t            max_retry;
    int32_t            min_r_time;
    int32_t            max_r_time;
    uint16_t           num_channels;
    uint8_t            num_frequency;
    struct {
        int32_t        m;
        int16_t        e;
        uint8_t        i;
        uint8_t        flags;
    } freq[ACFG_MAX_FREQ];
#if WIRELESS_EXT >= 18
    uint32_t           enc_capa;
#endif
} acfg_range_buf_t;

/**
 * @brief Power management
 */
typedef struct acfg_powmgmt {
    uint32_t val;
    uint16_t flags;
    uint8_t disabled;
} acfg_powmgmt_t;

/**
 * @brief Wireless statistics
 */
typedef struct acfg_stats {
    uint32_t status;             /*< Device status */
    uint32_t link_quality;        /*< link quality */
    uint32_t signal_level;        /*< signal level dBm */
    uint32_t noise_level;         /*< noise level dBm*/
    uint32_t updated;
    uint32_t discard_nwid;        /*< packet discarded due to
                                      wrong nwid/essid */
    uint32_t discard_code;        /*< packet discarded when
                                      unable to code/decode */
    uint32_t missed_beacon;      /*< missed beacon/superframe */
} acfg_stats_t;

typedef struct ieee80211req_key acfg_key_t;
typedef struct ieee80211req_mlme acfg_mlme_t;
typedef struct ieee80211req_athdbg acfg_athdbg_req_t;
typedef struct ieee80211_bcnrpt_s acfg_bcnrpt_t;
typedef struct ieee80211_acs_dbg acfg_acs_dbg_t;
typedef struct ieee80211_rrmstats_s acfg_rrmstats_t;

typedef struct acfg_mmt {
    uint8_t        macaddr[ACFG_MACADDR_LEN];
    uint32_t       buflen;
    uint8_t        buf[0];
} acfg_mgmt_t;

typedef struct {
    uint8_t   setcountry[3];
} acfg_set_country_t;

/**
 * @brief Structure containing information about each channel
 */
typedef struct acfg_channel {
    uint16_t   freq;               /**< Frequency in MHz */
    uint32_t   flags;              /**< Cahnnel attributes */
    uint8_t    flag_ext;           /**< Extended channel attributes */
    uint8_t    ieee_num;           /**< Channel number */
    int8_t     max_reg_pwr;        /**< Maximum regulatory tx power in dBm */
    int8_t     max_pwr;            /**< Maximum tx power in dBm */
    int8_t     min_pwr;            /**< Minimum tx power in dBm */
    uint8_t    reg_class;          /**< Regulatory class id of this channel */
    uint8_t    antenna_max;        /**< Max antenna gain from regulatory */
    uint8_t    vht_ch_freq_seg1;   /**< Center freqyency of channel */
    uint8_t    vht_ch_freq_seg2;   /**< Center freqyency of channel for
                                       80+80MHz mode of operation */
} acfg_channel_t;

/**
 * @brief Channel Information Structure
 */
typedef struct acfg_chan_info {
    uint32_t   num_channels;              /**< Number of channels */
    acfg_channel_t channel[ACFG_MAX_CHANS];   /**< Induvidual channel info */
} acfg_chan_info_t;

typedef struct {
    uint32_t    setchnwidth[3];
} acfg_set_chn_width_t;


/**
 * @brief Structure used in get/set of wmm param values
 */
typedef struct acfg_wmmparams_req {
    uint32_t param[5];
    uint32_t val;
} acfg_wmmparams_req_t ;

/**
 * @brief ath stats info
 */
typedef struct acfg_ath_stats {
    uint32_t          size;        /**< size of ath_stats */
    uint8_t           *address;   /**< location of ath_stats data */
    uint32_t          offload_if;  /**< flag to indicate 11ac offload interface */
} acfg_ath_stats_t;

typedef struct acfg_set_scan {
    uint8_t            scan_type; /* IW_SCAN_TYPE_{ACTIVE,PASSIVE} */
    uint8_t            essid_len;
    uint8_t            num_channels; /* num entries in channel_list; */
    uint8_t            flags;
    struct {
        uint16_t sa_family;    /* Common data: address family and length.  */
        char sa_data[14];        /* Address data.  */
    } bssid;
    uint8_t            essid[ACFG_MAX_SSID_LEN];
    uint32_t           min_channel_time; /* in TU */
    uint32_t           max_channel_time; /* in TU */
    struct {
        int32_t        m;
        int16_t        e;
        uint8_t        i;
        uint8_t        flags;
    } channel_list[ACFG_MAX_FREQ];
    uint16_t           len;
    uint16_t           point_flags;
} acfg_set_scan_t;

typedef struct ieee80211req_scan_result acfg_scanresult_t;

/**
 * @brief Information Element
 */
typedef struct acfg_ie {
    uint32_t  len;
    uint8_t   data[ACFG_MAX_IELEN];
} acfg_ie_t ;

/**
 * @brief scan request info
 */
typedef struct acfg_scan {
    uint32_t          len;        /**< Valid entries*/
    uint16_t          more;       /**< if more entries left*/
    uint16_t          offset;     /**< how much the user has read */
    uint8_t           *results;
    //    acfg_scan_list_t    list[ACFG_MAX_SCANLIST];
} acfg_scan_t;

typedef struct acfg_filter {
    uint32_t       filter;
} acfg_filter_t;

/**
 * @brief DELKEY
 */
typedef struct acfg_delkey {
    uint8_t idx;                     /**< Key index */
    uint8_t addr[ACFG_MACADDR_LEN];  /**< Mac Address */
} acfg_delkey_t;

typedef struct acfg_opaque{
    uint32_t len;
    void      *pointer;
}acfg_opaque_t;

/**
 * @brief Opaque Structure for passing P2P Params
 */
typedef struct acfg_p2p_param {
    uint32_t length;                      /**< Size of data passed */
    void      *pointer;                     /**< Pointer to the data */
    uint32_t param;                       /**< Sub-Ioctl number */
} acfg_p2p_param_t;

typedef struct ath_diag acfg_ath_diag_t;

/**
 * @brief Channel switch information
 */
typedef struct acfg_doth_chsw {
    uint32_t channel ;                    /**< Channel to switch to*/
    uint32_t time ;                       /**< Number of beacons to wait*/
} acfg_doth_chsw_t ;


#define NETLINK_ACFG_EVENT 20

#define ACFG_ATF_ADDSSID     0xFF01
#define ACFG_ATF_DELSSID     0xFF02
#define ACFG_ATF_ADDSTA      0xFF03
#define ACFG_ATF_DELSTA      0xFF04

/**
 * @brief The Data to transport to the ADF, Instances should be on the top
 *        part & pointers should be on the lower parts
 */
typedef union acfg_data {
    acfg_vapinfo_t          vap_info;       /**< Vap Information */
    acfg_ssid_t             ssid;           /**< SSID info */
    uint32_t             rssi;           /**< RSSI info */
    acfg_custdata_t         custdata;       /**< CUSTDATA info */
    acfg_testmode_t         testmode;       /**< TESTMODE info */
    uint32_t          phymode;        /**< PHY mode info */
    acfg_chan_t             chan;           /**< IEEE Channel number */
    acfg_opmode_t           opmode;         /**< Opmode info */
    acfg_ampdu_t            ampdu;          /**< AMPDU status */
    acfg_amsdu_t            amsdu;          /**< AMSDU status */
    acfg_ampdu_frames_t     ampdu_frames;   /**< AMPDU Frames */
    acfg_ampdu_limit_t      ampdu_limit;    /**< AMPDU Limit */
    acfg_shortgi_t          shortgi;        /**< Shortgi Setting */
    acfg_freq_t             freq ;          /**< Frequency */
    acfg_macaddr_t          macaddr ;   /**< Mac Address */
    acfg_range_t            range;          /**< Range of parameters */
    acfg_encode_t           encode;         /**< Encoding and token mode */
    uint32_t              rts;            /**< RTS Threshold */
    uint32_t             frag;           /**< Fragmentation Threshold */
    uint32_t            txpow;          /**< TxPower in dBm */
    acfg_powmgmt_t          powmgmt;        /**< Powert Management */
    acfg_param_req_t        param_req;      /**< Used with set/get param on
                                              redio and vap */
    acfg_wmmparams_req_t    wmmparams_req;	/**< wmm params */
    acfg_stats_t            stats;          /**< Wireless statistics */
    acfg_ath_stats_t        ath_stats;      /**< Wireless interface statistics */
    acfg_vendor_param_req_t vendor_param_req; /**< Used to set/get vendor_param */
    uint32_t              bitrate;        /**< Default bit rate */
    acfg_rate_t             rate;           /**< Default bit rate */
    acfg_sta_info_req_t     sta_info;       /**< Station info request structure */
    acfg_set_scan_t         sscan;
    acfg_scan_t             *scan;          /**< Scan list pointer */
    uint32_t              acparams[ACFG_MAX_ACPARAMS]; /**< AC params */
    acfg_macacl_t            maclist;
    /*vap list per radio*/
    acfg_radio_vap_info_t  radio_vap_info;
    acfg_appie_t            appie;
    acfg_add_client_t       addclient;
    acfg_del_client_t       delclient;
    acfg_authorize_client_t authorize_client;
    acfg_set_key_t          set_key;
    acfg_del_key_t          del_key;
    acfg_ratemask_t         ratemask;
    acfg_set_beacon_buf_t   beacon_buf;
    acfg_tx_power_t         tx_power;
    acfg_sens_level_t       sens_level;
    acfg_gpio_config_t      gpio_config;
    acfg_gpio_set_t         gpio_set;
    acfg_ctl_table_t        ctl_table;
    acfg_rateset_t          rs;
    acfg_rateset_phymode_t  rs_phymode;
    acfg_set_mu_whtlist_t   set_mu_whtlist;
    int32_t    cca_threshold;
    acfg_packet_power_param_t packet_power_param;
    uint32_t              enable_mon_filter;
    acfg_atf_ssid_val_t     atf_ssid;
    acfg_atf_sta_val_t      atf_sta;
    acfg_mlme_t             *mlme;
    acfg_mgmt_t             mgmt;
    acfg_filter_t           filter;
    acfg_ie_t               ie;             /**< IE */
    acfg_athdbg_req_t       dbgreq;
    acfg_key_t              key;
    acfg_delkey_t           delkey;
    acfg_opaque_t           opq;
    acfg_chan_info_t        chan_info;      /**< Channel Information */
    acfg_set_chn_width_t    chnw;
    acfg_ath_diag_t         ath_diag;
    acfg_set_country_t      setcntry;
    acfg_doth_chsw_t        doth_chsw;      /**< Switch channel */
    acfg_chmode_t           chmode;         /**< Channel Mode */
    acfg_p2p_param_t        p2p_param;      /**< P2P param */
} acfg_data_t;

/**
 * @brief The request structure to communicate between the user(ACFG)
 *        & kernel (ADF).
 */
typedef struct acfg_os_req {
    acfg_req_cmd_t  cmd;    /**< First element always */
    acfg_data_t     data;   /**< Data block */
} acfg_os_req_t;

typedef enum {
    ACFG_WLAN_PROFILE_SEC_METH_OPEN,
    ACFG_WLAN_PROFILE_SEC_METH_SHARED,
    ACFG_WLAN_PROFILE_SEC_METH_AUTO,
    ACFG_WLAN_PROFILE_SEC_METH_WPA,
    ACFG_WLAN_PROFILE_SEC_METH_WPA2,
    ACFG_WLAN_PROFILE_SEC_METH_WPAWPA2,
    ACFG_WLAN_PROFILE_SEC_METH_WPA_EAP,
    ACFG_WLAN_PROFILE_SEC_METH_WPA2_EAP,
    ACFG_WLAN_PROFILE_SEC_METH_WPS,
    ACFG_WLAN_PROFILE_SEC_METH_WPA3,
    ACFG_WLAN_PROFILE_SEC_METH_WPA2WPA3,
    ACFG_WLAN_PROFILE_SEC_METH_OWE,
    ACFG_WLAN_PROFILE_SEC_METH_SUITEB_192,
    /* keep this last */
    ACFG_WLAN_PROFILE_SEC_METH_INVALID,
}SEC_TYPE;

typedef uint8_t acfg_wlan_profile_sec_meth_e ;

typedef enum {
    ACFG_WLAN_PROFILE_CIPHER_METH_INVALID = 0x00000000,
    ACFG_WLAN_PROFILE_CIPHER_METH_TKIP    = 0x00000001,
    ACFG_WLAN_PROFILE_CIPHER_METH_AES     = 0x00000002,
    ACFG_WLAN_PROFILE_CIPHER_METH_WEP     = 0x00000004,
    ACFG_WLAN_PROFILE_CIPHER_METH_NONE    = 0x00000008,
    ACFG_WLAN_PROFILE_CIPHER_METH_AES_CMAC  = 0x00000010,
} CIPHER_METH;

typedef uint8_t acfg_wlan_profile_cipher_meth_e ;

typedef enum {
    /* keep this first */
    ACFG_WLAN_PROFILE_GRP_MGMT_CIPHER_INVALID = 0,

    ACFG_WLAN_PROFILE_GRP_MGMT_CIPHER_AES_128_CMAC = 1,
    ACFG_WLAN_PROFILE_GRP_MGMT_CIPHER_BIP_GMAC_128 = 2,
    ACFG_WLAN_PROFILE_GRP_MGMT_CIPHER_BIP_GMAC_256 = 3,
    ACFG_WLAN_PROFILE_GRP_MGMT_CIPHER_BIP_CMAC_256 = 4
} GRP_MGMT_CIPHER;

typedef enum {
    ACFG_WLAN_PROFILE_IEEE80211W_NONE = 0,
    ACFG_WLAN_PROFILE_IEEE80211W_OPTIONAL = 1,
    ACFG_WLAN_PROFILE_IEEE80211W_REQUIRED = 2,

    /* keep this last*/
    ACFG_WLAN_PROFILE_IEEE80211W_INVALID = 255,
} IEEE80211W;

enum acfg_frame_type {
    ACFG_FRAME_BEACON = 1,
    ACFG_FRAME_PROBE_RESP,
    ACFG_FRAME_PROBE_REQ,
    ACFG_FRAME_ASSOC_RESP,
    ACFG_FRAME_ASSOC_REQ,
    ACFG_FRAME_WNM,
    ACFG_FRAME_AUTH,
};

/* acfg_offchan_hdr->msg_types */
/* from app to driver */
#define ACFG_PKT_TYPE_MGMT 1
#define ACFG_PKT_TYPE_DATA 2
#define ACFG_CMD_CANCEL 3
#define ACFG_CMD_OFFCHAN_RX 4
/* from driver to app */
#define ACFG_PKT_STATUS_SUCCESS 0
#define ACFG_CHAN_FOREIGN 1
#define ACFG_CHAN_HOME 2
#define ACFG_PKT_STATUS_ERROR 3
#define ACFG_PKT_STATUS_XRETRY 4
#define ACFG_PKT_STATUS_UNKNOWN 5
#define ACFG_PKT_STATUS_TIMEOUT 6
#define ACFG_PKT_STATUS_BAD 6

struct acfg_offchan_stat {
    int16_t noise_floor;
    uint32_t tx_frame_count;
    uint32_t rx_frame_count;
    uint32_t rx_clear_count;
    uint32_t cycle_count;
}__attribute__((packed));
typedef struct acfg_offchan_stat acfg_offchan_stat_t;

struct acfg_offchan_hdr {
    uint8_t vap_name[ACFG_MAX_IFNAME];
	uint8_t msg_type;
	uint16_t msg_length;
	uint16_t channel;
	uint8_t scan_dur;
    uint8_t num_frames;
}__attribute__((packed));

struct acfg_offchan_tx_hdr {
    uint8_t id;
    uint8_t type;
    uint16_t length;
	uint8_t nss;
	uint8_t preamble;
	uint8_t mcs;
	uint8_t retry;
	uint8_t power;
}__attribute__((packed));

typedef struct acfg_offchan_hdr acfg_offchan_hdr_t;
#define ACFG_HDR_LEN sizeof(struct acfg_offchan_hdr)

struct acfg_offchan_tx_status {
    uint8_t id;
    uint8_t status;
}__attribute__((packed));

#define ACFG_MAX_OFFCHAN_TX_FRAMES 50
struct acfg_offchan_resp {
    struct acfg_offchan_hdr hdr;
    struct acfg_offchan_stat stat;
    struct acfg_offchan_tx_status status[ACFG_MAX_OFFCHAN_TX_FRAMES];
}__attribute__((packed));

struct acfg_raw_pkt {
    struct acfg_offchan_hdr hdr;
    uint8_t pkt_buf[0];
}__attribute__((packed));
typedef struct acfg_raw_pkt acfg_raw_pkt_t;

#define NETLINK_ACFG    (NETLINK_GENERIC + 8)

#define ACFG_MAX_SCAN_SSID      4
#define ACFG_MAX_SCAN_FREQ      16

/**
 * @brief SCAN request
 */
typedef struct acfg_scan_req {
    uint32_t freq[ACFG_MAX_SCAN_FREQ]; /**< Frequencies to scan */
    uint8_t  num_freq;                 /**< Number of frequencies to scan */
    uint8_t  num_ssid;                 /**< Number os SSIDs to scan */
    uint16_t len;                      /**< Size of IEs to add to probe Req */
    uint8_t  ssid[ACFG_MAX_SCAN_SSID][ACFG_MAX_SSID_LEN + 1]; /**< SSIDs */
    uint8_t  ssid_len[ACFG_MAX_SCAN_SSID];                /**< SSID Length */
} acfg_scan_req_t;

#endif
