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
 * DOC: qdf_vfs
 * QCA driver framework (QDF) virtual filesystem management APIs
 */

#if !defined(__QDF_VFS_H)
#define __QDF_VFS_H

/* Include Files */
#include <qdf_types.h>

struct qdf_vfs_attr;
struct qdf_vf_bin_attr;
struct qdf_dev_obj;

/**
 * qdf_vfs_set_file_attributes() - set file attributes
 * @devobj: Device object
 * @attr: File attribute
 *
 * This function will set the attributes of a file
 *
 * Return: QDF_STATUS_SUCCESS on success
 */
QDF_STATUS
qdf_vfs_set_file_attributes(struct kobject *devobj,
			    struct attribute_group *attr);

/**
 * qdf_vfs_clear_file_attributes() - clear file attributes
 * @devobj: Device object
 * @attr: File attribute
 *
 * This function will clear the attributes of a file
 *
 * Return: QDF_STATUS_SUCCESS on success
 */
QDF_STATUS
qdf_vfs_clear_file_attributes(struct kobject *devobj,
			      struct attribute_group *attr);

/**
 * qdf_vfs_create_binfile() - create binfile
 * @devobj: Device object
 * @attr: File attribute
 *
 * This function will create a binary file
 *
 * Return: QDF_STATUS_SUCCESS on success
 */
QDF_STATUS
qdf_create_dev_file(struct kobject *devobj,
		       struct bin_attribute *attr);

/**
 * qdf_vfs_delete_binfile() - delete binfile
 * @devobj: Device object
 * @attr: File attribute
 *
 * This function will delete a binary file
 *
 * Return: QDF_STATUS_SUCCESS on success
 */
QDF_STATUS
qdf_vfs_delete_binfile(struct kobject *devobj,
		       struct bin_attribute *attr);
#endif
