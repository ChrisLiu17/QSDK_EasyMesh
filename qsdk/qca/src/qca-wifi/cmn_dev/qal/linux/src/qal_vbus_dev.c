/*
 * Copyright (c) 2019 The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * DOC: qal_vbus_dev
 * This file provides OS dependent virtual bus device related APIs
 */

#include "qdf_util.h"
#include "qal_vbus_dev.h"
#include "qdf_module.h"
#include <linux/of_gpio.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
#include <linux/reset.h>
#endif

QDF_STATUS
qal_vbus_get_iorsc(int devnum, uint32_t flag, char *devname)
{
	int ret;

	if (!devname)
		return QDF_STATUS_E_INVAL;

	ret = gpio_request_one(devnum, flag, devname);

	return qdf_status_from_os_return(ret);
}

qdf_export_symbol(qal_vbus_get_iorsc);

QDF_STATUS
qal_vbus_release_iorsc(int devnum)
{
	if (devnum <= 0)
		return QDF_STATUS_E_INVAL;

	gpio_free(devnum);

	return QDF_STATUS_SUCCESS;
}

qdf_export_symbol(qal_vbus_release_iorsc);

QDF_STATUS
qal_vbus_get_resource(struct qdf_pfm_hndl *pfhndl,
		      struct qdf_vbus_resource **rsc,
		      uint32_t restype, uint32_t residx)
{
	struct resource *rsrc;

	if (!pfhndl)
		return QDF_STATUS_E_INVAL;

	rsrc = platform_get_resource((struct platform_device *)pfhndl,
				     restype, residx);
	if (!rsrc)
		return QDF_STATUS_E_FAILURE;

	*rsc = (struct qdf_vbus_resource *)rsrc;

	return QDF_STATUS_SUCCESS;
}

qdf_export_symbol(qal_vbus_get_resource);

QDF_STATUS
qal_vbus_get_irq(struct qdf_pfm_hndl *pfhndl, const char *str, int *irq)
{
	int inum;

	if (!pfhndl || !str)
		return QDF_STATUS_E_INVAL;

	inum = platform_get_irq_byname((struct platform_device *)pfhndl, str);

	if (inum < 0)
		return QDF_STATUS_E_FAULT;

	*irq = inum;

	return QDF_STATUS_SUCCESS;
}

qdf_export_symbol(qal_vbus_get_irq);

QDF_STATUS
qal_vbus_register_driver(struct qdf_pfm_drv *pfdev)
{
	int ret;

	if (!pfdev)
		return QDF_STATUS_E_INVAL;

	ret = platform_driver_register((struct platform_driver *)pfdev);

	return qdf_status_from_os_return(ret);
}

qdf_export_symbol(qal_vbus_register_driver);

QDF_STATUS
qal_vbus_deregister_driver(struct qdf_pfm_drv *pfdev)
{
	if (!pfdev)
		return QDF_STATUS_E_INVAL;

	platform_driver_unregister((struct platform_driver *)pfdev);

	return QDF_STATUS_SUCCESS;
}

qdf_export_symbol(qal_vbus_deregister_driver);
