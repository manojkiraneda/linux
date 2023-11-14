/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2021 Aspeed Technology Inc.
 */
#ifndef _ASPEED_ESPI_IOC_H
#define _ASPEED_ESPI_IOC_H

#include <linux/ioctl.h>
#include <linux/types.h>

/*
 * eSPI cycle type encoding
 *
 * Section 5.1 Cycle Types and Packet Format,
 * Intel eSPI Interface Base Specification, Rev 1.0, Jan. 2016.
 */
#define ESPI_FLASH_READ			0x00
#define ESPI_FLASH_WRITE		0x01
#define ESPI_FLASH_ERASE		0x02
#define ESPI_FLASH_SUC_CMPLT		0x06
#define ESPI_FLASH_SUC_CMPLT_D_MIDDLE	0x09
#define ESPI_FLASH_SUC_CMPLT_D_FIRST	0x0b
#define ESPI_FLASH_SUC_CMPLT_D_LAST	0x0d
#define ESPI_FLASH_SUC_CMPLT_D_ONLY	0x0f
#define ESPI_FLASH_UNSUC_CMPLT		0x0c
#define ESPI_FLASH_UNSUC_CMPLT_ONLY	0x0e

/*
 * eSPI packet format structure
 *
 * Section 5.1 Cycle Types and Packet Format,
 * Intel eSPI Interface Base Specification, Rev 1.0, Jan. 2016.
 */
struct espi_comm_hdr {
	uint8_t cyc;
	uint8_t len_h : 4;
	uint8_t tag : 4;
	uint8_t len_l;
};

struct espi_flash_rwe {
	uint8_t cyc;
	uint8_t len_h : 4;
	uint8_t tag : 4;
	uint8_t len_l;
	uint32_t addr_be;
	uint8_t data[];
} __packed;

struct espi_flash_cmplt {
	uint8_t cyc;
	uint8_t len_h : 4;
	uint8_t tag : 4;
	uint8_t len_l;
	uint8_t data[];
} __packed;

struct aspeed_espi_ioc {
	uint32_t pkt_len;
	uint8_t *pkt;
};

/*
 * we choose the longest header and the max payload size
 * based on the Intel specification to define the maximum
 * eSPI packet length
 */
#define ESPI_PLD_LEN_MIN	(1UL << 6)
#define ESPI_PLD_LEN_MAX	(1UL << 12)
#define ESPI_PKT_LEN_MAX	(sizeof(struct espi_perif_msg) + ESPI_PLD_LEN_MAX)

#define __ASPEED_ESPI_IOCTL_MAGIC	0xb8

/*
 * The IOCTL-based interface works in the eSPI packet in/out paradigm.
 *
 * For the eSPI packet format, refer to
 *   Section 5.1 Cycle Types and Packet Format,
 *   Intel eSPI Interface Base Specification, Rev 1.0, Jan. 2016.
 *
 * For the example user apps using these IOCTL, refer to
 *   https://github.com/AspeedTech-BMC/aspeed_app/tree/master/espi_test
 */

/*
 * Flash Channel (CH3)
 *  - ASPEED_ESPI_FLASH_GET_RX
 *      Receive an eSPI flash packet
 *  - ASPEED_ESPI_FLASH_PUT_TX
 *      Transmit an eSPI flash packet
 */
#define ASPEED_ESPI_FLASH_GET_RX	_IOR(__ASPEED_ESPI_IOCTL_MAGIC, \
					     0x30, struct aspeed_espi_ioc)
#define ASPEED_ESPI_FLASH_PUT_TX	_IOW(__ASPEED_ESPI_IOCTL_MAGIC, \
					     0x31, struct aspeed_espi_ioc)

#endif
