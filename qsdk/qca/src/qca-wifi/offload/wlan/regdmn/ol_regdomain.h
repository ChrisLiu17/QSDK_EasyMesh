/*
 * Copyright (c) 2011-2013, 2017, 2019 Qualcomm Innovation Center, Inc.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Innovation Center, Inc.
 *
 * 2011-2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * Notifications and licenses are retained for attribution purposes only.
 */
/*
 * Copyright (c) 2002-2006 Sam Leffler, Errno Consulting
 * Copyright (c) 2005-2006 Atheros Communications, Inc.
 * Copyright (c) 2010, Atheros Communications Inc.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the following conditions are met:
 * 1. The materials contained herein are unmodified and are used
 *    unmodified.
 * 2. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following NO
 *    ''WARRANTY'' disclaimer below (''Disclaimer''), without
 *    modification.
 * 3. Redistributions in binary form must reproduce at minimum a
 *    disclaimer similar to the Disclaimer below and any redistribution
 *    must be conditioned upon including a substantially similar
 *    Disclaimer requirement for further binary redistribution.
 * 4. Neither the names of the above-listed copyright holders nor the
 *    names of any contributors may be used to endorse or promote
 *    product derived from this software without specific prior written
 *    permission.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF NONINFRINGEMENT,
 * MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE
 * FOR SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGES.
 *
 * This module contains the regulatory domain private structure definitions .
 *
 */

#include <osdep.h>
#include <wbuf.h>

#define REGDMN_SUPPORT_11D

#ifndef _OL_DUP_COUNTRY_CODE_F

typedef enum _HAL_BOOL
{
    AH_FALSE = 0,       /* NB: lots of code assumes false is zero */
    AH_TRUE  = 1,
} HAL_BOOL;

#endif /* _OL_DUP_COUNTRY_CODE_F */

typedef A_UINT16 REGDMN_CTRY_CODE;        /* country code */
typedef A_UINT16 REGDMN_REG_DOMAIN;       /* regulatory domain code */

#define REGDMN_MODE_11N_MASK \
  ( REGDMN_MODE_11NG_HT20 | REGDMN_MODE_11NA_HT20 | REGDMN_MODE_11NG_HT40PLUS | \
    REGDMN_MODE_11NG_HT40MINUS | REGDMN_MODE_11NA_HT40PLUS | REGDMN_MODE_11NA_HT40MINUS )

#define PEREGRINE_RDEXT_DEFAULT  0x1F

/* define in ah.h */
#ifndef _OL_DUP_COUNTRY_CODE_F

enum {
    CTRY_DEBUG      = 0x1ff,    /* debug country code */
    CTRY_DEFAULT    = 0         /* default country code */
};

typedef enum {
        REG_EXT_FCC_MIDBAND            = 0,
        REG_EXT_JAPAN_MIDBAND          = 1,
        REG_EXT_FCC_DFS_HT40           = 2,
        REG_EXT_JAPAN_NONDFS_HT40      = 3,
        REG_EXT_JAPAN_DFS_HT40         = 4,
        REG_EXT_FCC_CH_144             = 5,
} REG_EXT_BITMAP;

#define REGDMN_CH144_PRI20_ALLOWED   0x00000001
#define REGDMN_CH144_SEC20_ALLOWED   0x00000002

/*
 * Regulatory related information
 */
typedef struct _HAL_COUNTRY_ENTRY{
    u_int16_t   countryCode;  /* HAL private */
    u_int16_t   regDmnEnum;   /* HAL private */
    u_int16_t   regDmn5G;
    u_int16_t   regDmn2G;
    u_int8_t    isMultidomain;
    u_int8_t    iso[3];       /* ISO CC + (I)ndoor/(O)utdoor or both ( ) */
} HAL_COUNTRY_ENTRY;

#endif /* _OL_DUP_COUNTRY_CODE_F */

/* define in ah.h */

/* define in ah_internal.h */
#define AH_NULL 0
#define AH_MIN(a,b) ((a)<(b)?(a):(b))
#define AH_MAX(a,b) ((a)>(b)?(a):(b))

#define ATH_REGCLASSIDS_MAX     10

/* define in ah_eeprom.h */
#define	SD_NO_CTL       0xf0
#define	NO_CTL          0xff
#define	CTL_MODE_M      0x0f
#define	CTL_11A         0
#define	CTL_11B         1
#define	CTL_11G         2
#define	CTL_TURBO       3
#define	CTL_108G        4
#define CTL_2GHT20      5
#define CTL_5GHT20      6
#define CTL_2GHT40      7
#define CTL_5GHT40      8
#define CTL_5GVHT80     9
/* define in ah_eeprom.h */

 /*
  * Enums of vendors used to modify reg domain flags if necessary
  */
 typedef enum {
    HAL_VENDOR_MAVERICK    = 1,
} HAL_VENDORS;

#ifndef ATH_NO_5G_SUPPORT
    #define REGDMN_MODE_11A_TURBO    REGDMN_MODE_108A
    #define CHAN_11A_BMZERO BMZERO,
    #define CHAN_11A_BM(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l) \
        BM(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l),
#else
    /* remove 11a channel info if 11a is not supported */
    #define CHAN_11A_BMZERO
    #define CHAN_11A_BM(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l)
#endif
#ifndef ATH_REMOVE_2G_TURBO_RD_TABLE
    #define REGDMN_MODE_11G_TURBO    REGDMN_MODE_108G
    #define CHAN_TURBO_G_BMZERO BMZERO,
    #define CHAN_TURBO_G_BM(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l) \
        BM(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l),
#else
    /* remove turbo-g channel info if turbo-g is not supported */
    #define CHAN_TURBO_G(a, b)
    #define CHAN_TURBO_G_BMZERO
    #define CHAN_TURBO_G_BM(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l)
#endif

#define BMLEN 2        /* Use 2 64 bit uint for channel bitmask
               NB: Must agree with macro below (BM) */
#define BMZERO {(u_int64_t) 0, (u_int64_t) 0}    /* BMLEN zeros */

#define BM(_fa, _fb, _fc, _fd, _fe, _ff, _fg, _fh, _fi, _fj, _fk, _fl) \
      {((((_fa >= 0) && (_fa < 64)) ? (((u_int64_t) 1) << _fa) : (u_int64_t) 0) | \
    (((_fb >= 0) && (_fb < 64)) ? (((u_int64_t) 1) << _fb) : (u_int64_t) 0) | \
    (((_fc >= 0) && (_fc < 64)) ? (((u_int64_t) 1) << _fc) : (u_int64_t) 0) | \
    (((_fd >= 0) && (_fd < 64)) ? (((u_int64_t) 1) << _fd) : (u_int64_t) 0) | \
    (((_fe >= 0) && (_fe < 64)) ? (((u_int64_t) 1) << _fe) : (u_int64_t) 0) | \
    (((_ff >= 0) && (_ff < 64)) ? (((u_int64_t) 1) << _ff) : (u_int64_t) 0) | \
    (((_fg >= 0) && (_fg < 64)) ? (((u_int64_t) 1) << _fg) : (u_int64_t) 0) | \
    (((_fh >= 0) && (_fh < 64)) ? (((u_int64_t) 1) << _fh) : (u_int64_t) 0) | \
    (((_fi >= 0) && (_fi < 64)) ? (((u_int64_t) 1) << _fi) : (u_int64_t) 0) | \
    (((_fj >= 0) && (_fj < 64)) ? (((u_int64_t) 1) << _fj) : (u_int64_t) 0) | \
    (((_fk >= 0) && (_fk < 64)) ? (((u_int64_t) 1) << _fk) : (u_int64_t) 0) | \
    (((_fl >= 0) && (_fl < 64)) ? (((u_int64_t) 1) << _fl) : (u_int64_t) 0) ) \
     ,(((((_fa > 63) && (_fa < 128)) ? (((u_int64_t) 1) << (_fa - 64)) : (u_int64_t) 0) | \
        (((_fb > 63) && (_fb < 128)) ? (((u_int64_t) 1) << (_fb - 64)) : (u_int64_t) 0) | \
        (((_fc > 63) && (_fc < 128)) ? (((u_int64_t) 1) << (_fc - 64)) : (u_int64_t) 0) | \
        (((_fd > 63) && (_fd < 128)) ? (((u_int64_t) 1) << (_fd - 64)) : (u_int64_t) 0) | \
        (((_fe > 63) && (_fe < 128)) ? (((u_int64_t) 1) << (_fe - 64)) : (u_int64_t) 0) | \
        (((_ff > 63) && (_ff < 128)) ? (((u_int64_t) 1) << (_ff - 64)) : (u_int64_t) 0) | \
        (((_fg > 63) && (_fg < 128)) ? (((u_int64_t) 1) << (_fg - 64)) : (u_int64_t) 0) | \
        (((_fh > 63) && (_fh < 128)) ? (((u_int64_t) 1) << (_fh - 64)) : (u_int64_t) 0) | \
        (((_fi > 63) && (_fi < 128)) ? (((u_int64_t) 1) << (_fi - 64)) : (u_int64_t) 0) | \
        (((_fj > 63) && (_fj < 128)) ? (((u_int64_t) 1) << (_fj - 64)) : (u_int64_t) 0) | \
        (((_fk > 63) && (_fk < 128)) ? (((u_int64_t) 1) << (_fk - 64)) : (u_int64_t) 0) | \
        (((_fl > 63) && (_fl < 128)) ? (((u_int64_t) 1) << (_fl - 64)) : (u_int64_t) 0)))}

/*
 * The following table is the master list for all different freqeuncy
 * bands with the complete matrix of all possible flags and settings
 * for each band if it is used in ANY reg domain.
 */

#define DEF_REGDMN              FCC3_FCCA
#define    DEF_DMN_5            FCC1
#define    DEF_DMN_2            FCCA
#define    COUNTRY_ERD_FLAG     0x8000
#define WORLDWIDE_ROAMING_FLAG  0x4000
#define    SUPER_DOMAIN_MASK    0x0fff
#define    COUNTRY_CODE_MASK    0x3fff
#define CF_INTERFERENCE         (CHANNEL_CW_INT | CHANNEL_RADAR_INT)
#define CHANNEL_14              (2484)    /* 802.11g operation is not permitted on channel 14 */
#define IS_11G_CH14(_ch,_cf) \
    (((_ch) == CHANNEL_14) && ((_cf) == CHANNEL_G))

/*
 * The following describe the bit masks for different passive scan
 * capability/requirements per regdomain.
 */
#define NO_PSCAN    0x0ULL
#define PSCAN_FCC   0x0000000000000001ULL
#define PSCAN_FCC_T 0x0000000000000002ULL
#define PSCAN_ETSI  0x0000000000000004ULL
#define PSCAN_MKK1  0x0000000000000008ULL
#define PSCAN_MKK2  0x0000000000000010ULL
#define PSCAN_MKKA  0x0000000000000020ULL
#define PSCAN_MKKA_G    0x0000000000000040ULL
#define PSCAN_ETSIA 0x0000000000000080ULL
#define PSCAN_ETSIB 0x0000000000000100ULL
#define PSCAN_ETSIC 0x0000000000000200ULL
#define PSCAN_WWR   0x0000000000000400ULL
#define PSCAN_MKKA1 0x0000000000000800ULL
#define PSCAN_MKKA1_G   0x0000000000001000ULL
#define PSCAN_MKKA2 0x0000000000002000ULL
#define PSCAN_MKKA2_G   0x0000000000004000ULL
#define PSCAN_MKK3  0x0000000000008000ULL
#define PSCAN_EXT_CHAN  0x0000000000010000ULL
#define PSCAN_DEFER 0x7FFFFFFFFFFFFFFFULL
#define IS_ECM_CHAN 0x8000000000000000ULL

/*
 * THE following table is the mapping of regdomain pairs specified by
 * an 8 bit regdomain value to the individual unitary reg domains
 */

typedef struct reg_dmn_pair_mapping {
    REGDMN_REG_DOMAIN regDmnEnum;    /* 16 bit reg domain pair */
    REGDMN_REG_DOMAIN regDmn5GHz;    /* 5GHz reg domain */
    REGDMN_REG_DOMAIN regDmn2GHz;    /* 2GHz reg domain */
    u_int32_t flags5GHz;        /* Requirements flags (AdHoc
                       disallow, noise floor cal needed,
                       etc) */
    u_int32_t flags2GHz;        /* Requirements flags (AdHoc
                       disallow, noise floor cal needed,
                       etc) */
    u_int64_t pscanMask;        /* Passive Scan flags which
                       can override unitary domain
                       passive scan flags.  This
                       value is used as a mask on
                       the unitary flags*/
    u_int16_t singleCC;        /* Country code of single country if
                       a one-on-one mapping exists */
}  REG_DMN_PAIR_MAPPING;

/*
 * The following table of vendor specific regdomain pairs and
 * additional flags used to modify the flags5GHz and flags2GHz
 * of the original regdomain
 */

struct ccmap {
    char isoName[3];
    REGDMN_CTRY_CODE countryCode;
};

#define    MAX_MAPS        2
typedef struct vendor_pair_mapping {
    REGDMN_REG_DOMAIN regDmnEnum;    /* 16 bit reg domain pair */
    HAL_VENDORS vendor;        /* Vendor code */
    u_int32_t flags5GHzIntersect;    /* AND mask for requirements flags (AdHoc
                       disallow, noise floor cal needed,
                       etc) */
    u_int32_t flags5GHzUnion;    /* OR mask for requirements flags (AdHoc
                       disallow, noise floor cal needed,
                       etc) */
    u_int32_t flags2GHzIntersect;    /* AND mask for requirements flags (AdHoc
                       disallow, noise floor cal needed,
                       etc) */
    u_int32_t flags2GHzUnion;    /* AND mask for requirements flags (AdHoc
                       disallow, noise floor cal needed,
                       etc) */
    struct ccmap ccmappings[MAX_MAPS];    /* Vendor mapping of country strings to
                       country codes */
}  VENDOR_PAIR_MAPPING;


typedef struct {
    REGDMN_CTRY_CODE        countryCode;
    REGDMN_REG_DOMAIN        regDmnEnum;
    const char*        isoName;
    const char*        name;
    u_int16_t
        allow11aTurbo : 1,
        allow11gTurbo : 1,
        allow11ng20   : 1, /* HT-20 allowed in 2GHz? */
        allow11ng40   : 1, /* HT-40 allowed in 2GHz? */
        allow11na20   : 1, /* HT-20 allowed in 5GHz? */
        allow11na40   : 1, /* HT-40 VHT-40 allowed in 5GHz? */
        allow11na80   : 1, /* VHT-80 allowed in 5GHz */
        allow11na160  : 1, /* VHT-160 allowed in 5GHz */
        allow11na80_80: 1; /* VHT-80_80 allowed in 5GHz */
    u_int16_t outdoorChanStart;
} COUNTRY_CODE_TO_ENUM_RD;

typedef struct RegDmnFreqBand {
    u_int16_t    lowChannel;    /* Low channel center in MHz */
    u_int16_t    highChannel;    /* High Channel center in MHz */
    u_int8_t    powerDfs;    /* Max power (dBm) for channel
                       range when using DFS */
    u_int8_t    antennaMax;    /* Max allowed antenna gain */
    u_int8_t    channelBW;    /* Bandwidth of the channel */
    u_int8_t    channelSep;    /* Channel separation within
                       the band */
    u_int64_t    useDfs;        /* Use DFS in the RegDomain
                       if corresponding bit is set */
    u_int64_t    usePassScan;    /* Use Passive Scan in the RegDomain
                       if corresponding bit is set */
    u_int8_t    regClassId;    /* Regulatory class id */
} REG_DMN_FREQ_BAND;

typedef struct reg_domain {
    u_int16_t regDmnEnum;    /* value from EnumRd table */
    u_int8_t conformance_test_limit;
    u_int64_t dfsMask;    /* DFS bitmask for 5Ghz tables */
    u_int64_t pscan;    /* Bitmask for passive scan */
    u_int32_t flags;    /* Requirement flags (AdHoc disallow, noise
                   floor cal needed, etc) */
#ifndef ATH_NO_5G_SUPPORT
    u_int64_t chan11a[BMLEN];/* 128 bit bitmask for channel/band selection */
    u_int64_t chan11a_turbo[BMLEN];/* 128 bit bitmask for channel/band select */
    u_int64_t chan11a_dyn_turbo[BMLEN]; /* 128 bit mask for chan/band select */
#endif /* ATH_NO_5G_SUPPORT */

    u_int64_t chan11b[BMLEN];/* 128 bit bitmask for channel/band selection */
    u_int64_t chan11g[BMLEN];/* 128 bit bitmask for channel/band selection */

#ifndef ATH_REMOVE_2G_TURBO_RD_TABLE
    u_int64_t chan11g_turbo[BMLEN];/* 128 bit bitmask for channel/band select */
#endif /* ATH_REMOVE_2G_TURBO_RD_TABLE */
} REG_DOMAIN;

struct cmode {
    u_int32_t    mode;
    u_int32_t    flags;
};

#define    YES    true
#define    NO    false

/* mapping of old skus to new skus for Japan */
typedef struct {
    REGDMN_REG_DOMAIN    domain;
    REGDMN_REG_DOMAIN    newdomain_pre53;    /* pre eeprom version 5.3 */
    REGDMN_REG_DOMAIN    newdomain_post53;    /* post eeprom version 5.3 */
} JAPAN_SKUMAP;

/* mapping of countrycode to new skus for Japan */
typedef struct {
    REGDMN_CTRY_CODE    ccode;
    REGDMN_REG_DOMAIN    newdomain_pre53;    /* pre eeprom version 5.3 */
    REGDMN_REG_DOMAIN    newdomain_post53;    /* post eeprom version 5.3 */
} JAPAN_COUNTRYMAP;

/* check rd flags in eeprom for japan */
typedef struct {
    u_int16_t       freqbandbit;
    u_int32_t       eepromflagtocheck;
} JAPAN_BANDCHECK;

/* Common mode power table for 5Ghz */
typedef struct {
    u_int16_t lchan;
    u_int16_t hchan;
    u_int8_t  pwrlvl;
} COMMON_MODE_POWER;

typedef struct {
    REGDMN_REG_DOMAIN                regDmnEnum;
    const COUNTRY_CODE_TO_ENUM_RD *countryMappings;
} REG_DMN_CUSTOM_MAPPINGS;

/* Multi-Device RegDomain Support */
typedef struct ath_hal_reg_dmn_tables {
    /* regDomainPairs: Map of 8-bit regdomain values to unitary reg domain */
    const REG_DMN_PAIR_MAPPING    *regDomainPairs;
    /* regDmnVendorPairs: Vendor-specific additions to reg domain pairs */
    const VENDOR_PAIR_MAPPING     *regDmnVendorPairs;
    /* allCountries: Master list of freq. bands (flags, settings) */
    const COUNTRY_CODE_TO_ENUM_RD *allCountries;
    /* customMappings: Customized exceptions to the allCountries array */
    const REG_DMN_CUSTOM_MAPPINGS *customMappings;
    /* regDomains: Array of supported reg domains */
    const REG_DOMAIN              *regDomains;

    u_int16_t regDomainPairsCt;    /* Num reg domain pair entries */
    u_int16_t regDmnVendorPairsCt; /* Num vendor pairs */
    u_int16_t allCountriesCt;      /* Num country entries */
    u_int16_t customMappingsCt;    /* Num custom mappings */
    u_int16_t regDomainsCt;        /* Num supported reg domain entries */
} HAL_REG_DMN_TABLES;

/*
 * Country/Region Codes from MS WINNLS.H
 * Numbering from ISO 3166
 */
/**     @brief country code definitions
 *        - country definition: CTRY_DEBUG
 *            - country string: DB
 *            - country ID: 0
 *        - country definition: CTRY_DEFAULT
 *            - country string: NA
 *            - country ID: 0
 *        - country definition: CTRY_AFGHANISTAN
 *            - country string: AF
 *            - country ID: 4
 *        - country definition: CTRY_ALBANIA
 *            - country string: AL
 *            - country ID: 8
 *        - country definition: CTRY_ALGERIA
 *            - country string: DZ
 *            - country ID: 12
 *        - country definition: CTRY_ARGENTINA
 *            - country string: AR
 *            - country ID: 32
 *        - country definition: CTRY_ARMENIA
 *            - country string: AM
 *            - country ID: 51
 *        - country definition: CTRY_AUSTRALIA
 *            - country string: AU
 *            - country ID: 36
 *        - country definition: CTRY_AUSTRIA
 *            - country string: AT
 *            - country ID: 40
 *        - country definition: CTRY_AZERBAIJAN
 *            - country string: AZ
 *            - country ID: 31
 *        - country definition: CTRY_BAHAMAS
 *            - country string: BS
 *            - country ID: 44
 *        - country definition: CTRY_BAHRAIN
 *            - country string: BH
 *            - country ID: 48
 *        - country definition: CTRY_BELARUS
 *            - country string: BY
 *            - country ID: 112
 *        - country definition: CTRY_BELGIUM
 *            - country string: BE
 *            - country ID: 56
 *        - country definition: CTRY_BELIZE
 *            - country string: BZ
 *            - country ID: 84
 *        - country definition: CTRY_BERMUDA
 *            - country string: BM
 *            - country ID: 60
 *        - country definition: CTRY_BHUTAN
 *            - country string: BT
 *            - country ID: 64
 *        - country definition: CTRY_BOLIVIA
 *            - country string: BO
 *            - country ID: 68
 *        - country definition: CTRY_BOSNIA_HERZEGOWINA
 *            - country string: 70
 *            - country ID: BA
 *        - country definition: CTRY_BRAZIL
 *            - country string: BR
 *            - country ID: 76
 *        - country definition: CTRY_BRUNEI_DARUSSALAM
 *            - country string: BN
 *            - country ID: 96
 *        - country definition: CTRY_BULGARIA
 *            - country string: BG
 *            - country ID: 100
 *        - country definition: CTRY_BURKINA_FASO
 *            - country string: BF
 *            - country ID: 854
 *        - country definition: CTRY_CANADA
 *            - country string: CA
 *            - country ID: 124
 *        - country definition: CTRY_CAYMAN_ISLANDS
 *            - country string: KY
 *            - country ID: 136
 *        - country definition: CTRY_CENTRAL_AFRICA_REPUBLIC
 *            - country string: CF
 *            - country ID: 140
 *        - country definition: CTRY_CHILE
 *            - country string: CL
 *            - country ID: 152
 *        - country definition: CTRY_CHINA
 *            - country string: CN
 *            - country ID: 152
 *        - country definition: CTRY_CHRISTMAS_ISLAND
 *            - country string: CX
 *            - country ID: 162
 *        - country definition: CTRY_COLOMBIA
 *            - country string: CO
 *            - country ID: 170
 *        - country definition: CTRY_COSTA_RICA
 *            - country string: CR
 *            - country ID: 191
 *        - country definition: CTRY_CROATIA
 *            - country string: HR
 *            - country ID: 191
 *        - country definition: CTRY_CYPRUS
 *            - country string: CY
 *            - country ID: 196
 *        - country definition: CTRY_CZECH
 *            - country string: CZ
 *            - country ID: 203
 *        - country definition: CTRY_DENMARK
 *            - country string: DK
 *            - country ID: 208
 *        - country definition: CTRY_DOMINICAN_REPUBLIC
 *            - country string: DO
 *            - country ID: 214
 *        - country definition: CTRY_ECUADOR
 *            - country string: EC
 *            - country ID: 218
 *        - country definition: CTRY_EGYPT
 *            - country string: EG
 *            - country ID: 818
 *        - country definition: CTRY_EL_SALVADOR
 *            - country string: SV
 *            - country ID: 222
 *        - country definition: CTRY_ETHIOPIA
 *            - country string: ET
 *            - country ID: 231
 *        - country definition: CTRY_ESTONIA
 *            - country string: EE
 *            - country ID: 233
 *        - country definition: CTRY_FAEROE_ISLANDS
 *            - country string: FO
 *            - country ID: 234
 *        - country definition: CTRY_FINLAND
 *            - country string: FI
 *            - country ID: 246
 *        - country definition: CTRY_FRANCE
 *            - country string: FR
 *            - country ID: 250
 *        - country definition: CTRY_FRENCH_GUIANA
 *            - country string: GF
 *            - country ID: 254
 *        - country definition: CTRY_FRENCH_POLYNESIA
 *            - country string: PF
 *            - country ID: 258
 *        - country definition: CTRY_GEORGIA
 *            - country string: GE
 *            - country ID: 268
 *        - country definition: CTRY_GERMANY
 *            - country string: DE
 *            - country ID: 276
 *        - country definition: CTRY_GHANA
 *            - country string: GH
 *            - country ID: 288
 *        - country definition: CTRY_GIBRALTAR
 *            - country string: GI
 *            - country ID: 292
 *        - country definition: CTRY_GREECE
 *            - country string: GR
 *            - country ID: 300
 *        - country definition: CTRY_GREENLAND
 *            - country string: GL
 *            - country ID: 304
 *        - country definition: CTRY_GRENADA
 *            - country string: GD
 *            - country ID: 308
 *        - country definition: CTRY_GUADELOUPE
 *            - country string: GP
 *            - country ID: 312
 *        - country definition: CTRY_GUAM
 *            - country string: GU
 *            - country ID: 316
 *        - country definition: CTRY_GUATEMALA
 *            - country string: GT
 *            - country ID: 320
 *        - country definition: CTRY_HONDURAS
 *            - country string: HN
 *            - country ID: 340
 *        - country definition: CTRY_HONG_KONG
 *            - country string: HK
 *            - country ID: 344
 *        - country definition: CTRY_HUNGARY
 *            - country string: HU
 *            - country ID: 348
 *        - country definition: CTRY_ICELAND
 *            - country string: IS
 *            - country ID: 352
 *        - country definition: CTRY_INDIA
 *            - country string: IN
 *            - country ID: 356
 *        - country definition: CTRY_INDIA2
 *            - country string: IN
 *            - country ID: 5006
 *        - country definition: CTRY_INDONESIA
 *            - country string: ID
 *            - country ID: 360
 *        - country definition: CTRY_IRAQ
 *            - country string: IQ
 *            - country ID: 368
 *        - country definition: CTRY_IRELAND
 *            - country string: IE
 *            - country ID: 372
 *        - country definition: CTRY_ISRAEL
 *            - country string: IL
 *            - country ID: 376
 *        - country definition: CTRY_ITALY
 *            - country string: IT
 *            - country ID: 380
 *        - country definition: CTRY_JAMAICA
 *            - country string: JM
 *            - country ID: 388
 *        - country definition: CTRY_JAPAN
 *            - country string: JP
 *            - country ID: 392
 *        - country definition: CTRY_JORDAN
 *            - country string: JO
 *            - country ID: 400
 *        - country definition: CTRY_KAZAKHSTAN
 *            - country string: KZ
 *            - country ID: 398
 *        - country definition: CTRY_KENYA
 *            - country string: KE
 *            - country ID: 404
 *        - country definition: CTRY_KOREA_ROC
 *            - country string: KR
 *            - country ID: 410
 *        - country definition: CTRY_KUWAIT
 *            - country string: KW
 *            - country ID: 414
 *        - country definition: CTRY_LATVIA
 *            - country string: LV
 *            - country ID: 428
 *        - country definition: CTRY_LEBANON
 *            - country string: LB
 *            - country ID: 422
 *        - country definition: CTRY_LIBYA
 *            - country string: LY
 *            - country ID: 434
 *        - country definition: CTRY_LIECHTENSTEIN
 *            - country string: LI
 *            - country ID: 438
 *        - country definition: CTRY_LITHUANIA
 *            - country string: LT
 *            - country ID: 440
 *        - country definition: CTRY_LUXEMBOURG
 *            - country string: LU
 *            - country ID: 442
 *        - country definition: CTRY_MACAU
 *            - country string: MO
 *            - country ID: 446
 *        - country definition: CTRY_MACEDONIA
 *            - country string: MK
 *            - country ID: 807
 *        - country definition: CTRY_MALAWI
 *            - country string: MW
 *            - country ID: 454
 *        - country definition: CTRY_MALAYSIA
 *            - country string: MY
 *            - country ID: 458
 *        - country definition: CTRY_MALDIVES
 *            - country string: MV
 *            - country ID: 462
 *        - country definition: CTRY_MALTA
 *            - country string: MT
 *            - country ID: 470
 *        - country definition: CTRY_MARSHALL_ISLANDS
 *            - country string: MH
 *            - country ID: 584
 *        - country definition: CTRY_MARTINIQUE
 *            - country string: MQ
 *            - country ID: 474
 *        - country definition: CTRY_MAURITANIA
 *            - country string: MR
 *            - country ID: 478
 *        - country definition: CTRY_MAURITIUS
 *            - country string: MU
 *            - country ID: 480
 *        - country definition: CTRY_MAYOTTE
 *            - country string: YT
 *            - country ID: 175
 *        - country definition: CTRY_MEXICO
 *            - country string: MX
 *            - country ID: 484
 *        - country definition: CTRY_MICRONESIA
 *            - country string: FM
 *            - country ID: 583
 *        - country definition: CTRY_MONACO
 *            - country string: MC
 *            - country ID: 492
 *        - country definition: CTRY_MOROCCO
 *            - country string: MA
 *            - country ID: 504
 *        - country definition: CTRY_MYANMAR
 *            - country string: MM
 *            - country ID: 104
 *        - country definition: CTRY_NETHERLANDS
 *            - country string: NL
 *            - country ID: 528
 *        - country definition: CTRY_NEW_ZEALAND
 *            - country string: NZ
 *            - country ID: 554
 *        - country definition: CTRY_NICARAGUA
 *            - country string: NI
 *            - country ID: 558
 *        - country definition: CTRY_NORWAY
 *            - country string: NO
 *            - country ID: 578
 *        - country definition: CTRY_OMAN
 *            - country string: OM
 *            - country ID: 512
 *        - country definition: CTRY_PAKISTAN
 *            - country string: PK
 *            - country ID: 586
 *        - country definition: CTRY_PALAU
 *            - country string: PW
 *            - country ID: 585
 *        - country definition: CTRY_PANAMA
 *            - country string: PA
 *            - country ID: 591
 *        - country definition: CTRY_PARAGUAY
 *            - country string: PY
 *            - country ID: 600
 *        - country definition: CTRY_PERU
 *            - country string: PE
 *            - country ID: 604
 *        - country definition: CTRY_PHILIPPINES
 *            - country string: PH
 *            - country ID: 608
 *        - country definition: CTRY_POLAND
 *            - country string: PL
 *            - country ID: 616
 *        - country definition: CTRY_PORTUGAL
 *            - country string: PT
 *            - country ID: 620
 *        - country definition: CTRY_PUERTO_RICO
 *            - country string: PR
 *            - country ID: 630
 *        - country definition: CTRY_QATAR
 *            - country string: QA
 *            - country ID: 634
 *        - country definition: CTRY_REUNION
 *            - country string: RE
 *            - country ID: 638
 *        - country definition: CTRY_ROMANIA
 *            - country string: RO
 *            - country ID: 642
 *        - country definition: CTRY_RUSSIA
 *            - country string: RU
 *            - country ID: 643
 *        - country definition: CTRY_SAMOA
 *            - country string: WS
 *            - country ID: 882
 *        - country definition: CTRY_SAUDI_ARABIA
 *            - country string: SA
 *            - country ID: 682
 *        - country definition: CTRY_SENEGAL
 *            - country string: SN
 *            - country ID: 686
 *        - country definition: CTRY_SERBIA
 *            - country string: RS
 *            - country ID: 688
 *        - country definition: CTRY_MONTENEGRO
 *            - country string: ME
 *            - country ID: 499
 *        - country definition: CTRY_SINGAPORE
 *            - country string: SG
 *            - country ID: 702
 *        - country definition: CTRY_SLOVAKIA
 *            - country string: SK
 *            - country ID: 703
 *        - country definition: CTRY_SLOVENIA
 *            - country string: SI
 *            - country ID: 705
 *        - country definition: CTRY_SOUTH_AFRICA
 *            - country string: ZA
 *            - country ID: 710
 *        - country definition: CTRY_SPAIN
 *            - country string: ES
 *            - country ID: 724
 *        - country definition: CTRY_SRI_LANKA
 *            - country string: LK
 *            - country ID: 144
 *        - country definition: CTRY_SURINAME
 *            - country string: SR
 *            - country ID: 740
 *        - country definition: CTRY_SWEDEN
 *            - country string: SE
 *            - country ID: 752
 *        - country definition: CTRY_SWITZERLAND
 *            - country string: CH
 *            - country ID: 756
 *        - country definition: CTRY_TAIWAN
 *            - country string: TW
 *            - country ID: 158
 *        - country definition: CTRY_TANZANIA
 *            - country string: TZ
 *            - country ID: 834
 *        - country definition: CTRY_THAILAND
 *            - country string: TH
 *            - country ID: 764
 *        - country definition: CTRY_TOGO
 *            - country string: TG
 *            - country ID: 768
 *        - country definition: CTRY_TRINIDAD_Y_TOBAGO
 *            - country string: TT
 *            - country ID: 780
 *        - country definition: CTRY_TUNISIA
 *            - country string: TN
 *            - country ID: 788
 *        - country definition: CTRY_TURKEY
 *            - country string: TR
 *            - country ID: 792
 *        - country definition: CTRY_UAE
 *            - country string: AE
 *            - country ID: 784
 *        - country definition: CTRY_UKRAINE
 *            - country string: UA
 *            - country ID: 804
 *        - country definition: CTRY_UNITED_KINGDOM
 *            - country string: GB
 *            - country ID: 826
 *        - country definition: CTRY_UNITED_STATES
 *            - country string: US
 *            - country ID: 840
 *        - country definition: CTRY_URUGUAY
 *            - country string: UY
 *            - country ID: 858
 *        - country definition: CTRY_UZBEKISTAN
 *            - country string: UZ
 *            - country ID: 860
 *        - country definition: CTRY_VANUATU
 *            - country string: VU
 *            - country ID: 548
 *        - country definition: CTRY_VENEZUELA
 *            - country string: VE
 *            - country ID: 862
 *        - country definition: CTRY_VIET_NAM
 *            - country string: VN
 *            - country ID: 704
 *        - country definition: CTRY_YEMEN
 *            - country string: YE
 *            - country ID: 887
 *        - country definition: CTRY_ZIMBABWE
 *            - country string: ZW
 *            - country ID: 716
 */
enum CountryCode {
    CTRY_AFGHANISTAN          = 4,       /* Afghanistan */
    CTRY_ALBANIA              = 8,       /* Albania */
    CTRY_ALGERIA              = 12,      /* Algeria */
    CTRY_AMERICAN_SAMOA       = 16,      /* American Samoa */
    CTRY_ANGUILLA             = 660,     /* Anguilla */
    CTRY_ARGENTINA            = 32,      /* Argentina */
    CTRY_ARMENIA              = 51,      /* Armenia */
    CTRY_AUSTRALIA            = 36,      /* Australia */
    CTRY_AUSTRIA              = 40,      /* Austria */
    CTRY_AZERBAIJAN           = 31,      /* Azerbaijan */
    CTRY_BAHAMAS              = 44,      /* Bahamas */
    CTRY_BAHRAIN              = 48,      /* Bahrain */
    CTRY_BANGLADESH           = 50,      /* Bangladesh */
    CTRY_BARBADOS             = 52,      /* Barbados */
    CTRY_BELARUS              = 112,     /* Belarus */
    CTRY_BELGIUM              = 56,      /* Belgium */
    CTRY_BELIZE               = 84,      /* Belize */
    CTRY_BERMUDA              = 60,      /* Berumuda */
    CTRY_BHUTAN               = 64,      /* Bhutan */
    CTRY_BOLIVIA              = 68,      /* Bolivia */
    CTRY_BOSNIA_HERZ          = 70,      /* Bosnia and Herzegowina */
    CTRY_BRAZIL               = 76,      /* Brazil */
    CTRY_BRUNEI_DARUSSALAM    = 96,      /* Brunei Darussalam */
    CTRY_BULGARIA             = 100,     /* Bulgaria */
    CTRY_BURKINA_FASO         = 854,     /* Burkina-Faso */
    CTRY_CAMBODIA             = 116,     /* Cambodia */
    CTRY_CANADA               = 124,     /* Canada */
    CTRY_CAYMAN_ISLANDS       = 136,     /* Cayman Islands */
    CTRY_CENTRAL_AFRICA_REPUBLIC = 140,  /* Central Africa Republic */
    CTRY_CHAD                 = 148,     /* Chad */
    CTRY_CHILE                = 152,     /* Chile */
    CTRY_CHINA                = 156,     /* People's Republic of China */
    CTRY_CHRISTMAS_ISLAND     = 162,     /* Christmas Island */
    CTRY_COLOMBIA             = 170,     /* Colombia */
    CTRY_COSTA_RICA           = 188,     /* Costa Rica */
    CTRY_CROATIA              = 191,     /* Croatia */
    CTRY_CYPRUS               = 196,
    CTRY_CZECH                = 203,     /* Czech Republic */
    CTRY_DENMARK              = 208,     /* Denmark */
    CTRY_DOMINICA             = 212,     /* Dominica */
    CTRY_DOMINICAN_REPUBLIC   = 214,     /* Dominican Republic */
    CTRY_ECUADOR              = 218,     /* Ecuador */
    CTRY_EGYPT                = 818,     /* Egypt */
    CTRY_EL_SALVADOR          = 222,     /* El Salvador */
    CTRY_ETHIOPIA             = 231,     /* Ethiopia */
    CTRY_ESTONIA              = 233,     /* Estonia */
    CTRY_FAEROE_ISLANDS       = 234,     /* Faeroe Islands */
    CTRY_FINLAND              = 246,     /* Finland */
    CTRY_FRANCE               = 250,     /* France */
    CTRY_FRENCH_GUIANA        = 254,     /* French Guiana */
    CTRY_FRENCH_POLYNESIA     = 258,     /* French Polynesia */
    CTRY_GEORGIA              = 268,     /* Georgia */
    CTRY_GERMANY              = 276,     /* Germany */
    CTRY_GHANA                = 288,     /* Ghana */
    CTRY_GIBRALTAR            = 292,     /* Gibraltar */
    CTRY_GREECE               = 300,     /* Greece */
    CTRY_GREENLAND            = 304,     /* Greenland */
    CTRY_GRENADA              = 308,     /* Grenada */
    CTRY_GUADELOUPE           = 312,     /* Guadeloupe */
    CTRY_GUAM                 = 316,     /* Guam */
    CTRY_GUATEMALA            = 320,     /* Guatemala */
    CTRY_GUYANA               = 328,     /* Guyana */
    CTRY_HAITI                = 332,     /* Haiti */
    CTRY_HONDURAS             = 340,     /* Honduras */
    CTRY_HONG_KONG            = 344,     /* Hong Kong S.A.R., P.R.C. */
    CTRY_HUNGARY              = 348,     /* Hungary */
    CTRY_ICELAND              = 352,     /* Iceland */
    CTRY_INDIA                = 356,     /* India */
    CTRY_INDONESIA            = 360,     /* Indonesia */
    CTRY_IRAQ                 = 368,     /* Iraq */
    CTRY_IRELAND              = 372,     /* Ireland */
    CTRY_ISRAEL               = 376,     /* Israel */
    CTRY_ITALY                = 380,     /* Italy */
    CTRY_COTE_D_IVOIRE        = 384,     /* Cote D'voire */
    CTRY_JAMAICA              = 388,     /* Jamaica */
    CTRY_JAPAN                = 392,     /* Japan (JP) */
    CTRY_JORDAN               = 400,     /* Jordan */
    CTRY_KAZAKHSTAN           = 398,     /* Kazakhstan */
    CTRY_KENYA                = 404,     /* Kenya */
    CTRY_KOREA_ROC            = 410,     /* South Korea */
    CTRY_KUWAIT               = 414,     /* Kuwait */
    CTRY_LATVIA               = 428,     /* Latvia */
    CTRY_LEBANON              = 422,     /* Lebanon */
    CTRY_LESOTHO              = 426,     /* Lesotho */
    CTRY_LIBYA                = 434,     /* Libya */
    CTRY_LIECHTENSTEIN        = 438,     /* Liechtenstein */
    CTRY_LITHUANIA            = 440,     /* Lithuania */
    CTRY_LUXEMBOURG           = 442,     /* Luxembourg */
    CTRY_MACAU                = 446,     /* Macau SAR */
    CTRY_MACEDONIA            = 807,     /* the Former Yugoslav Republic of Macedonia */
    CTRY_MALAWI               = 454,     /* Malawi */
    CTRY_MALAYSIA             = 458,     /* Malaysia */
    CTRY_MALDIVES             = 462,     /* Maldives */
    CTRY_MALTA                = 470,     /* Malta */
    CTRY_MARSHALL_ISLANDS     = 584,     /* Marshall Islands */
    CTRY_MARTINIQUE           = 474,     /* Martinique */
    CTRY_MAURITANIA           = 478,     /* Mauritania */
    CTRY_MAURITIUS            = 480,     /* Mauritius */
    CTRY_MAYOTTE              = 175,     /* Mayotte */
    CTRY_MEXICO               = 484,     /* Mexico */
    CTRY_MICRONESIA           = 583,     /* Federated States of Micronesia */
    CTRY_MOLDOVA              = 498,     /* Republic of Moldova */
    CTRY_MONACO               = 492,     /* Principality of Monaco */
    CTRY_MONGOLIA             = 496,     /* Mongolia */
    CTRY_MOROCCO              = 504,     /* Morocco */
    CTRY_MYANMAR              = 104,     /* Myanmar */
    CTRY_NAMIBIA              = 516,     /* Namibia */
    CTRY_NEPAL                = 524,     /* Nepal */
    CTRY_NETHERLANDS          = 528,     /* Netherlands */
    CTRY_NETHERLANDS_ANTILLES = 530,     /* Netherlands-Antilles */
    CTRY_ARUBA                = 533,     /* Aruba */
    CTRY_NEW_ZEALAND          = 554,     /* New Zealand */
    CTRY_NICARAGUA            = 558,     /* Nicaragua */
    CTRY_NIGERIA              = 566,     /* Nigeria */
    CTRY_NORWAY               = 578,     /* Norway */
    CTRY_NORTHERN_MARIANA_ISLANDS = 580, /* Northern Mariana Islands */
    CTRY_OMAN                 = 512,     /* Oman */
    CTRY_PAKISTAN             = 586,     /* Islamic Republic of Pakistan */
    CTRY_PALAU                = 585,     /* Palau */
    CTRY_PANAMA               = 591,     /* Panama */
    CTRY_PAPUA_NEW_GUINEA     = 598,     /* Papua New Guinea */
    CTRY_PARAGUAY             = 600,     /* Paraguay */
    CTRY_PERU                 = 604,     /* Peru */
    CTRY_PHILIPPINES          = 608,     /* Republic of the Philippines */
    CTRY_POLAND               = 616,     /* Poland */
    CTRY_PORTUGAL             = 620,     /* Portugal */
    CTRY_PUERTO_RICO          = 630,     /* Puerto Rico */
    CTRY_QATAR                = 634,     /* Qatar */
    CTRY_REUNION              = 638,     /* Reunion */
    CTRY_ROMANIA              = 642,     /* Romania */
    CTRY_RUSSIA               = 643,     /* Russia */
    CTRY_RWANDA               = 646,     /* Rwanda */
    CTRY_ST_BARTHELEMY        = 652,     /* Saint Barthelemy */
    CTRY_ST_KITTS_NEVIS       = 659,     /* Saint Kitts and Nevis */
    CTRY_ST_LUCIA             = 662,     /* Saint Lucia */
    CTRY_ST_MARTIN            = 663,     /* Saint Martin */
    CTRY_ST_PIERRE_MIQUELON   = 666,     /* Saint Pierre and Miquelon */
    CTRY_ST_VINCENT_GRENADIENS= 670,     /* Saint Vincent and the Grenadiens */
    CTRY_SAMOA                = 882,     /* Samoa */
    CTRY_SAUDI_ARABIA         = 682,     /* Saudi Arabia */
    CTRY_SENEGAL              = 686,     /* Senegal */
    CTRY_SERBIA               = 688,     /* Republic of Serbia */
    CTRY_MONTENEGRO           = 499,     /* Montenegro */
    CTRY_SINGAPORE            = 702,     /* Singapore */
    CTRY_SLOVAKIA             = 703,     /* Slovak Republic */
    CTRY_SLOVENIA             = 705,     /* Slovenia */
    CTRY_SOUTH_AFRICA         = 710,     /* South Africa */
    CTRY_SPAIN                = 724,     /* Spain */
    CTRY_SRI_LANKA            = 144,     /* Sri Lanka */
    CTRY_SURINAME             = 740,     /* Suriname */
    CTRY_SWEDEN               = 752,     /* Sweden */
    CTRY_SWITZERLAND          = 756,     /* Switzerland */
    CTRY_TAIWAN               = 158,     /* Taiwan */
    CTRY_TANZANIA             = 834,     /* Tanzania */
    CTRY_THAILAND             = 764,     /* Thailand */
    CTRY_TOGO                 = 768,     /* Togo */
    CTRY_TRINIDAD_Y_TOBAGO    = 780,     /* Trinidad y Tobago */
    CTRY_TUNISIA              = 788,     /* Tunisia */
    CTRY_TURKEY               = 792,     /* Turkey */
    CTRY_UAE                  = 784,     /* U.A.E. */
    CTRY_TURKS_CAICOS         = 796,     /* Turks and Caicos */
    CTRY_UGANDA               = 800,     /* Uganda */
    CTRY_UKRAINE              = 804,     /* Ukraine */
    CTRY_UNITED_KINGDOM       = 826,     /* United Kingdom */
    CTRY_UNITED_STATES        = 840,     /* United States */
    CTRY_URUGUAY              = 858,     /* Uruguay */
    CTRY_UZBEKISTAN           = 860,     /* Uzbekistan */
    CTRY_VANUATU              = 548,     /* Vanuatu */
    CTRY_VENEZUELA            = 862,     /* Venezuela */
    CTRY_VIET_NAM             = 704,     /* Viet Nam */
    CTRY_VIRGIN_ISLANDS       = 850,     /* Virgin Islands(U.S.) */
    CTRY_WALLIS_FUTUNA        = 876,     /* Wallis and Futuna */
    CTRY_YEMEN                = 887,     /* Yemen */
    CTRY_ZIMBABWE             = 716,     /* Zimbabwe */

    /*
    ** "Special" codes for multiply defined countries, with the exception
    ** of Japan and US.
    */

    CTRY_BELGIUM2             = 5002,    /* Belgium/Cisco implementation */
    CTRY_INDIA2               = 5006     /* India with 5.9GHz support */
};


#if AH_NEED_PRIV_REGDMN
#include "ol_regdomain_priv.h"
#else
#include "ol_regdomain_common.h"
#endif

/* New function added for regdmn devices */
struct ol_regdmn {
    ol_scn_t scn_handle; /* handle to device */
    osdev_t  osdev;      /* handle to use OS-independent services */
    REGDMN_REG_DOMAIN      ol_regdmn_current_rd;       /* Current regulatory domain */
    REGDMN_REG_DOMAIN      ol_regdmn_current_rd_ext;    /* Regulatory domain Extension reg from EEPROM*/
    REGDMN_CTRY_CODE       ol_regdmn_countryCode;     /* current country code */
    REGDMN_REG_DOMAIN      ol_regdmn_currentRDInUse;  /* Current 11d domain in used */
    REGDMN_REG_DOMAIN      ol_regdmn_currentRD5G;     /* Current 5G regulatory domain */
    REGDMN_REG_DOMAIN      ol_regdmn_currentRD2G;     /* Current 2G regulatory domain */
    char                ol_regdmn_iso[4];          /* current country iso + NULL */
    HAL_DFS_DOMAIN      ol_regdmn_dfsDomain;       /* current dfs domain */
    HAL_VENDORS         ol_regdmn_vendor;
    int                 ol_regdmn_xchanmode;
    A_UINT32            ol_regdmn_ctl_2G;
    A_UINT32            ol_regdmn_ctl_5G;
    A_UINT32            ol_regdmn_ch144;
    A_UINT32            ol_regdmn_ch144_eppovrd;     /* Enable/disable (1/0) EPPROM for Channel 144*/
    char                ol_regdmn_default_iso[4];    /* default 11d country if no country can be found from beacons or other ways. */
};

typedef struct ol_regdmn *ol_regdmn_t;

int ol_regdmn_attach(ol_scn_t scn_handle);
void ol_regdmn_detach(struct ol_regdmn* ol_regdmn_handle);
bool ol_regdmn_set_regdomain(struct ol_regdmn* ol_regdmn_handle, REGDMN_REG_DOMAIN regdomain);
bool ol_regdmn_get_regdomain(struct ol_regdmn* ol_regdmn_handle, REGDMN_REG_DOMAIN *regdomain);
bool ol_regdmn_set_regdomain_ext(struct ol_regdmn* ol_regdmn_handle, REGDMN_REG_DOMAIN regdomain);
int ol_regdmn_getchannels(struct ol_regdmn *ol_regdmn_handle, u_int cc, bool outDoor, bool xchanMode,  IEEE80211_REG_PARAMETERS *reg_parm);
void ol_regdmn_start(struct ol_regdmn *ol_regdmn_handle, IEEE80211_REG_PARAMETERS *reg_parm );
void ol_80211_channel_setup(struct ieee80211com *ic, enum ieee80211_clist_cmd cmd,
              struct ieee80211_ath_channel *chans, int nchan, const u_int8_t *regclassids, u_int nregclass, int countrycode);
bool ol_regdmn_set_ch144(struct ol_regdmn *ol_regdmn_handle, u_int ch144);
bool ol_regdmn_get_ch144(struct ol_regdmn *ol_regdmn_handle, u_int *ch144);
bool ol_regdmn_set_ch144_eppovrd(struct ol_regdmn *ol_regdmn_handle, u_int ch144);

bool __ahdecl
ol_regdmn_init_channels(struct ol_regdmn *ol_regdmn_handle,
              struct ieee80211_ath_channel *chans, u_int maxchans, u_int *nchans,
              u_int8_t *regclassids, u_int maxregids, u_int *nregids,
              REGDMN_CTRY_CODE cc, u_int32_t modeSelect,
              bool enableOutdoor, bool enableExtendedChannels,
              bool block_dfs_enable);

/*
 * Return bit mask of wireless modes supported by the hardware.
 */
u_int ol_regdmn_getWirelessModes(struct ol_regdmn* ol_regdmn_handle);

/*
 * Find the HAL country code from its ISO name.
 */
extern REGDMN_CTRY_CODE __ahdecl ol_regdmn_findCountryCode(u_int8_t *countryString);
extern u_int8_t  __ahdecl ol_regdmn_findCTLByCountryCode(REGDMN_CTRY_CODE countrycode, bool is2G);
extern REGDMN_CTRY_CODE __ahdecl ol_regdmn_findCountryCodeForCurrentRD(u_int8_t *countryString, REGDMN_REG_DOMAIN curRd);

/*
 * Find the HAL country code from its domain enum.
 */
extern REGDMN_CTRY_CODE __ahdecl ol_regdmn_findCountryCodeByRegDomain(REGDMN_REG_DOMAIN regdmn);

/*
 * Return the current regulatory domain information
 */
void __ahdecl ol_regdmn_getCurrentCountry(struct ol_regdmn* ol_regdmn_handle, IEEE80211_COUNTRY_ENTRY* ctry);

bool __ahdecl ol_regdmn_ispublicsafetysku(struct ol_regdmn* ol_regdmn_handle);


/**
* @brief        Checks if 160 MHz flag is set in wireless_modes
*
* @param hal_cap:   pointer to HAL_REG_CAPABILITIES
*
* @return       true if 160 MHz flag is set, false otherwise.
*/
bool ol_regdmn_get_160mhz_support(TARGET_HAL_REG_CAPABILITIES *hal_cap);

/**
* @brief        Checks if 80+80 MHz flag is set in wireless_modes
*
* @param hal_cap:   pointer to HAL_REG_CAPABILITIES
*
* @return       true if 80+80 MHz flag is set, false otherwise.
*/

bool ol_regdmn_get_80p80mhz_support(TARGET_HAL_REG_CAPABILITIES *hal_cap);
