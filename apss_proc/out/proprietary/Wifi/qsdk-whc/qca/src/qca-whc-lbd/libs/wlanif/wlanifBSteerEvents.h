// vim: set et sw=4 sts=4 cindent:
/*
 * @File: wlanifBSteerEvents.h
 *
 * @Abstract: Load balancing daemon band steering events interface
 *
 * @Notes: This header should not be included directly by other components
 *         within the load balancing daemon. It should be considered
 *         private to the wlanif module.
 *
 * Copyright (c) 2014, 2016-2017 Qualcomm Technologies, Inc.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * 2014, 2016 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 */

#ifndef wlanifBSteerEvents__h
#define wlanifBSteerEvents__h

#include "lbd_types.h"  // for LBD_STATUS
#include "wlanifBSteerControl.h"

//sonlib cmn already includes this
#if SONLIB_SUPPORT_ENABLED
#include <sys/types.h>  // for fixed-width integer types
#include <ieee80211_band_steering_api.h>
#endif

#if defined(__cplusplus)
extern "C" {
#endif

// Out of package forward decls.
struct dbgModule;

/* package API */

/**
 * @brief Initialize the band steering event interface but do not trigger
 *        the starting of the events.
 *
 * Triggering the events is done using wlanifBSteerEventsEnable().
 *
 * @param [in] dbgModule  the handle to use for logging
 *
 * @return a handle to the state for this instance, or NULL if it could
 *         not be created
 */
wlanifBSteerEventsHandle_t wlanifBSteerEventsCreate(struct dbgModule *dbgModule,
                                                    wlanifBSteerControlHandle_t controlHandle);

/**
 * @brief Turn on the event generation from the band steering module.
 *
 * This should be called after all interested entities have registered for the
 * events so that they do not miss any of them.
 *
 * @param [in] handle  the handle returned from wlanifBSteerEventsCreate() to
 *                     use to enable the events.
 * @param [in] sysindex of vap.

 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS wlanifBSteerEventsEnable(wlanifBSteerEventsHandle_t handle,
                                    u_int32_t sysIndex);
/**
 * @brief Destroy the band steering event interface.
 *
 * When this completes, no further events will be generated.
 *
 * @param [in] handle  the handle returned from wlanifBSteerEventsCreate() to
 *                     destroy
 *
 * @return LBD_OK if it was successfully destroyed; otherwise LBD_NOK
 */
LBD_STATUS wlanifBSteerEventsDestroy(wlanifBSteerEventsHandle_t handle);

/**
 * @brief Register a callback function to observe STA stats
 *
 * @param [in] handle  the handle returned from wlanifBSteerEventsCreate()
 * @param [in] callback  the function to invoke for new STA
 *                       stats
 * @param [in] cookie  the parameter to pass to the callback function
 *
 * @return LBD_OK if the observer was successfully registered; otherwise
 *         LBD_NOK (due to no free slots)
 */
LBD_STATUS wlanifBSteerEventsRegisterSTAStatsObserver(
    wlanifBSteerEventsHandle_t handle, wlanif_staStatsObserverCB callback,
    void *cookie);

/**
 * @brief Unregister a callback function so that it no longer will receive
 *        STA stats notification.
 *
 * The parameters provided must match those given in the original
 * wlanif_registerSTAStatsObserver() call.
 *
 * @param [in] handle  the handle returned from
 *                     wlanifBSteerEventsCreate()
 * @param [in] callback  the function that was previously registered
 * @param [in] cookie  the parameter that was provided when the function was
 *                     registered
 *
 * @return LBD_OK if the observer was successfully unregistered; otherwise
 *         LBD_NOK
 */
LBD_STATUS wlanifBSteerEventsUnregisterSTAStatsObserver(
    wlanifBSteerEventsHandle_t handle, wlanif_staStatsObserverCB callback,
    void *cookie);

/**
 * @brief Inform wlanif that a STA associated or disassociated on a remote
 *        node.
 *
 * @param [in] handle  the handle returned from wlanifBSteerEventsCreate()
 * @param [in] staAddr  the MAC address of the STA whose association changed
 * @param [in] bssid  the BSS on which the STA associated/disassociated
 * @param [in] isAssoc  whether the STA is associated now or disassociated
 *                      on this BSS
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS wlanifBSteerEventsHandleRemoteAssocUpdate(
        wlanifBSteerEventsHandle_t handle, const struct ether_addr *staAddr,
        const struct ether_addr *bssid, LBD_BOOL isAssoc);

#if defined(__cplusplus)
}
#endif

#endif  // wlanifBSteerEvents__h
