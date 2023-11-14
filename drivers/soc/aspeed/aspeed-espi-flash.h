/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2023 IBM Corporation
 */
#ifndef _ASPEED_ESPI_FLASH_H_
#define _ASPEED_ESPI_FLASH_H_

#include <linux/mtd/mtd.h>
#include <linux/dma-mapping.h>

/**
 * enum aspeed_espi_flash_safs_mode
 * @SAFS_MODE_MIX: Mix Mode, where read commands are processed by HW, but write
 *                 commands are processed by SW
 * @SAFS_MODE_SW: Software Mode, where read/write/erase commands are processed
 *                by software.
 * @SAFS_MODE_HW: Hardware Mode, where read/write/erase commands are processed
 *                by hardware.
 * @SAFS_MODE_NO_SUPPORT: SAFS is not supported
 *
 * This enumeration captures various SAFS modes supported.
 */
enum aspeed_espi_flash_safs_mode {
	SAFS_MODE_MIX,
	SAFS_MODE_SW,
	SAFS_MODE_HW,
	SAFS_MODE_NO_SUPPORT,
};

/**
 * struct aspeed_espi_flash_dma - structure to capture the dma transactions
 * @tc_virt: Description of member1.
 * @tx_aadr: Description of member2.
 *           One can provide multiple line descriptions
 *           for members.
 *
 * Description of the structure.
 */
struct aspeed_espi_flash_dma {
	void *tx_virt;
	dma_addr_t tx_addr;
	void *rx_virt;
	dma_addr_t rx_addr;
};

/**
 * struct aspeed_espi_flash_dma - structure to capture the dma transactions
 * @tc_virt: Description of member1.
 * @tx_aadr: Description of member2.
 *           One can provide multiple line descriptions
 *           for members.
 *
 * Description of the structure.
 */
struct aspeed_espi_flash {

	unsigned short		page_offset;	/* offset in flash address */
	unsigned int		page_size;	/* of bytes per page */

	struct mutex			lock;
	struct aspeed_espi_flash_dma	dma;

	struct mtd_info			mtd;
	struct aspeed_espi_ctrl		*ctrl;
	uint8_t				erase_mask;

	uint32_t			tx_sts;
	uint32_t			rx_sts;

	wait_queue_head_t		wq;

	spinlock_t			spinlock;
};

void aspeed_espi_flash_event(uint32_t sts, struct aspeed_espi_flash *espi_flash);
void aspeed_espi_flash_enable(struct aspeed_espi_flash *espi_flash);
void *aspeed_espi_flash_alloc(struct device *dev, struct aspeed_espi_ctrl *espi_ctrl);
void aspeed_espi_flash_free(struct device *dev, struct aspeed_espi_flash *espi_flash);

#endif
