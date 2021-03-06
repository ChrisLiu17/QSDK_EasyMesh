/*
 * Copyright (c) 2017-2018 The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 *
 */
#ifndef __NSS_CRYPTO_CTRL_H
#define __NSS_CRYPTO_CTRL_H

#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>

#define NSS_CRYPTO_CMN_MSG_LEN \
	(sizeof(struct nss_crypto_cmn_msg) - sizeof(struct nss_cmn_msg))

struct nss_crypto_engine;
struct nss_crypto_ctx;

typedef int (*nss_crypto_ctx_fill_method_t)(struct nss_crypto_ctx *ctx, struct nss_crypto_session_data *data, struct nss_crypto_cmn_ctx *msg, void __iomem *vaddr);

/*
 * nss_crypto_algo_info
 *	holds crypto algorithm specific information
 */
struct nss_crypto_algo_info {
	const char *name;			/* name of the algorithm DTSI specific */
	uint16_t cipher_key_len;		/* cipher key length */
	uint16_t auth_key_len;			/* authentication key length */
	uint16_t cipher_blk_len;		/* cipher block len */
	uint16_t iv_len;			/* iv len */
	uint16_t digest_sz;			/* digest size */
	uint16_t auth_blk_len;			/* authentication block size */
	uint32_t *auth_fips_iv;			/* authentication fips iv */
	bool cipher_valid;			/* algorithm supports cipher mode */
	bool auth_valid;			/* algorithm supports auth mode */
};

/*
 * nss_crypto_ctx
 *	context class
 */
struct nss_crypto_ctx {
	uint32_t nss_ifnum;			/* Engine interface number */
	struct kref ref;			/* session_reference */
	struct nss_crypto_node *node;		/* crypto node associated with the context */
	struct nss_crypto_algo_info *info;	/* algorithm specific info */
	void *hw_info;				/* HW specific info */

	struct dentry *dentry;			/* debugfs entry for context stats */
	struct nss_crypto_cmn_stats stats;	/* Context stats */

	struct nss_crypto_cmn_msg nim;		/* crypto context message */
	struct delayed_work free_work;		/* crypto context free worker */
	unsigned long free_timeout;		/* crypto context free timeout ticks */

	uint16_t idx;				/* session index */
	enum nss_crypto_cmn_algo algo;		/* session algorithm */
};

/*
 * nss_crypto_ctrl_stats
 *	Host maintained control stats
 */
struct nss_crypto_ctrl_stats {
	atomic_t alloc;			/**< Sessions allocated */
	atomic_t alloc_fail_node;	/**< Session fill failed */
	atomic_t alloc_fail_nomem;	/**< No memory */
	atomic_t alloc_fail_nospace;	/**< Bitmap full */
	atomic_t alloc_fail_noalgo;	/**< Invalid algorithm */
	atomic_t alloc_fail_noresp;	/**< Alloc tx msg failure */
	atomic_t free;			/**< Sessions freed */
	atomic_t free_fail_msg;		/**< Tx msg failure */
	atomic_t free_fail_inuse;	/**< Session in use */
	atomic_t free_fail_queue;	/**< queue congested */
	atomic_t free_fail_retry;	/**< Session free retry failure */
};

/*
 * nss_crypto_ctrl
 *	crypto driver control
 */
struct nss_crypto_ctrl {
	struct list_head node_head;		/* list of nodes */
	struct list_head user_head;		/* list of users */

	uint32_t max_ctx_size;			/* crypto context size */
	uint32_t max_contexts;			/* max supported context(s) */
	struct nss_crypto_ctx *ctx_tbl;		/* index table for each session */

	struct mutex mutex;			/* mutex */
	struct delayed_work probe_work;		/* crypto probe worker */

	struct completion complete;		/* Wait queue for processing pending items */
	struct nss_ctx_instance *nss_msg_hdl;	/* NSS driver message handle */
	struct dentry *dentry;			/* debugfs driver stats entry */

	struct dentry *ctrl_dentry;			/* debugfs driver stats entry */
	struct nss_crypto_ctrl_stats cstats; /* control stats */

	bool active;				/* crypto is active */
};

/*
 * nss_crypto_node
 *	crypto node class
 *
 * Note: this represents a particular node for crypto
 * operation. So, a node can contain multiple engines
 * inside it for distributing the crypto tasks
 */
struct nss_crypto_node {
	struct list_head list;			/* next node */
	struct list_head eng_head;		/* list of engines */

	struct nss_ctx_instance *nss_data_hdl;	/* nss driver data handle */

	nss_crypto_ctx_fill_method_t fill_ctx;	/* context alloc */

	uint32_t nss_ifnum;			/* NSS FW crypto Interface*/
	uint32_t dma_mask;			/* max dma rings supported */
	uint32_t dma_qdepth;			/* DMA queue length */

	struct dentry *dentry;			/* entry for node stats */
	struct net_device *ndev;		/* netdevice associated with node */
	struct device *dev;			/* platform device associated with node */
	struct nss_crypto_cmn_stats stats;	/* Node stats */

	bool tx_enabled;			/* transform is enabled from host */
	bool algo[NSS_CRYPTO_CMN_ALGO_MAX];	/* Algorithm support table */
};

/*
 * nss_crypto_engine
 *	engine class
 *
 * Note: this represents '1' instance of the actual HW.
 * It can be chained with multiple such instances thus
 * representing a crypto processing node
 */
struct nss_crypto_engine {
	struct list_head list;			/* next engine */
	struct nss_crypto_node *node;		/* pointer to node */
	struct device *dev;			/* engine device */
	uint32_t id;				/* engine id */

	uint32_t dma_mask;			/* no. of DMA rings available */

	phys_addr_t crypto_paddr;		/* starting physical address of Crypto regs */
	void __iomem *crypto_vaddr;		/* starting virtual address of Crypto regs */
	struct dentry *dentry;                  /* entry for engine stats */
	struct nss_crypto_debugfs_entry *debugfs_entries;	/* debugfs entries */
	size_t debugfs_num_entries;		/* debugfs entries count */
};

/*
 * extern API(s)
 */
extern struct nss_crypto_ctrl g_control;

extern struct nss_crypto_node *nss_crypto_node_alloc(struct platform_device *pdev, uint32_t nss_ifnum, const char *name);
extern void nss_crypto_node_free(struct nss_crypto_node *node);
extern int nss_crypto_node_init(struct nss_crypto_node *node, struct device_node *np);

extern struct nss_crypto_engine *nss_crypto_engine_alloc(struct platform_device *pdev);
extern void nss_crypto_engine_free(struct nss_crypto_engine *eng);
extern int nss_crypto_engine_init(struct nss_crypto_node *node, struct nss_crypto_engine *eng);

extern void nss_crypto_ctx_free(struct kref *ref);
extern void nss_crypto_user_free(struct kref *ref);

extern void nss_crypto_process_event(void *app_data, struct nss_crypto_cmn_msg *msg);
extern void nss_crypto_transform_done(struct net_device *net, struct sk_buff *skb, struct napi_struct *napi);
#endif /* __NSS_CRYPTO_CTRL_H*/
