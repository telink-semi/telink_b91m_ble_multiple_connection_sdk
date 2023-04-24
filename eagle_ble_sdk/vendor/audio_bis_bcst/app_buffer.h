/********************************************************************************************************
 * @file	app_buffer.h
 *
 * @brief	This is the header file for BLE SDK
 *
 * @author	BLE GROUP
 * @date	2020.06
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#ifndef APP_BUFFER_H_
#define APP_BUFFER_H_

#include "app_config.h"
#include "tl_common.h"


#if (__PROJECT_B91_AUDIO_BIS_BCST__)

/**
 * @brief	connMaxRxOctets
 * refer to BLE SPEC "4.5.10 Data PDU length management" & "2.4.2.21 LL_LENGTH_REQ and LL_LENGTH_RSP"
 * usage limitation:
 * 1. should be in range of 27 ~ 251
 */
#define ACL_CONN_MAX_RX_OCTETS			27


/**
 * @brief	connMaxTxOctets
 * refer to BLE SPEC "4.5.10 Data PDU length management" & "2.4.2.21 LL_LENGTH_REQ and LL_LENGTH_RSP"
 *  in this SDK, we separate this value into 2 parts: slaveMaxTxOctets and masterMaxTxOctets,
 *  for purpose to save some SRAM costed by when slave and master use different connMaxTxOctets.
 * usage limitation:
 * 1. slaveMaxTxOctets and masterMaxTxOctets should be in range of 27 ~ 251
 */
#define ACL_MASTER_MAX_TX_OCTETS		27
#define ACL_SLAVE_MAX_TX_OCTETS			27




/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin ************************************************/
/**
 * @brief	ACL RX buffer size & number
 *  		ACL RX buffer is shared by all connections to hold LinkLayer RF RX data.
 * usage limitation for ACL_RX_FIFO_SIZE:
 * 1. should be greater than or equal to (connMaxRxOctets + 21)
 * 2. should be be an integer multiple of 16 (16 Byte align)
 * 3. user can use formula:  size = CAL_LL_ACL_RX_FIFO_SIZE(connMaxRxOctets)
 * usage limitation for ACL_RX_FIFO_NUM:
 * 1. must be: 2^n, (power of 2)
 * 2. at least 4; recommended value: 8, 16
 */
#define ACL_RX_FIFO_SIZE				48  // ACL_CONN_MAX_RX_OCTETS + 21, then 16 Byte align
#define ACL_RX_FIFO_NUM					8	// must be: 2^n


/**
 * @brief	ACL TX buffer size & number
 *  		ACL MASTER TX buffer is shared by all master connections to hold LinkLayer RF TX data.
*			ACL SLAVE  TX buffer is shared by all slave  connections to hold LinkLayer RF TX data.
 * usage limitation for ACL_MASTER_TX_FIFO_SIZE & ACL_SLAVE_TX_FIFO_SIZE:
 * 1. should be greater than or equal to (connMaxTxOctets + 10)
 * 2. should be be an integer multiple of 16 (16 Byte align)
 * 3. user can use formula:  size = CAL_LL_ACL_TX_FIFO_SIZE(connMaxTxOctets)
 * usage limitation for ACL_MASTER_TX_FIFO_NUM & ACL_SLAVE_TX_FIFO_NUM:
 * 1. must be: (2^n) + 1, (power of 2, then add 1)
 * 2. at least 9; recommended value: 9, 17, 33; other value not allowed.
 * usage limitation for size * (number - 1)
 * 1. (ACL_MASTER_TX_FIFO_SIZE * (ACL_MASTER_TX_FIFO_NUM - 1)) must be less than 4096 (4K)
 *    (ACL_SLAVE_TX_FIFO_SIZE  * (ACL_SLAVE_TX_FIFO_NUM - 1))  must be less than 4096 (4K)
 *    so when ACL TX FIFO size equal to or bigger than 256, ACL TX FIFO number can only be 9(can not use 17 or 33), cause 256*(17-1)=4096
 */
#define ACL_MASTER_TX_FIFO_SIZE			48	// ACL_MASTER_MAX_TX_OCTETS + 10, then 16 Byte align
#define ACL_MASTER_TX_FIFO_NUM			9	// must be: (2^n) + 1

#define ACL_SLAVE_TX_FIFO_SIZE			48  // ACL_MASTER_MAX_TX_OCTETS + 10, then 16 Byte align
#define ACL_SLAVE_TX_FIFO_NUM			9   // must be: (2^n) + 1




extern	u8	app_acl_rxfifo[];
extern	u8	app_acl_mstTxfifo[];
extern	u8	app_acl_slvTxfifo[];
/******************** ACL connection LinkLayer TX & RX data FIFO allocation, End ***************************************************/



/***************** ACL connection L2CAP layer MTU TX & RX data FIFO allocation, Begin ********************************/

/*Note:
 * if support LE Secure Connections, L2CAP buffer must >= 72.([64+6]+3)/4*4), 4B align.
 * MTU Buff size = Extra_Len(6)+ ATT_MTU_MAX
 *  1. should be greater than or equal to (ATT_MTU + 6)
 *  2. should be be an integer multiple of 4 (4 Byte align)
 */
#define ATT_MTU_MASTER_RX_MAX_SIZE  23
#define	MTU_M_BUFF_SIZE_MAX			CAL_MTU_BUFF_SIZE(ATT_MTU_MASTER_RX_MAX_SIZE)

#define ATT_MTU_SLAVE_RX_MAX_SIZE   23
#define	MTU_S_BUFF_SIZE_MAX			CAL_MTU_BUFF_SIZE(ATT_MTU_SLAVE_RX_MAX_SIZE)


extern	u8 mtu_m_rx_fifo[];

extern	u8 mtu_s_rx_fifo[];
extern	u8 mtu_s_tx_fifo[];
/***************** ACL connection L2CAP layer MTU TX & RX data FIFO allocation, End **********************************/




/******************************** EXT ADV && PERD ADV buffer FIFO allocation, Begin **********************************/
/** Number of Supported Advertising Sets, no exceed "ADV_SETS_NUMBER_MAX" */
#define	APP_ADV_SETS_NUMBER					2
/** Maximum Advertising Data Length,   (if legacy ADV, max length 31 bytes is enough) */
#define APP_MAX_LENGTH_ADV_DATA				1024
/** Maximum Scan Response Data Length, (if legacy ADV, max length 31 bytes is enough) */
#define APP_MAX_LENGTH_SCAN_RESPONSE_DATA	1024
/** Number of Supported Periodic Advertising Sets, no exceed "PERIODIC_ADV_NUMBER_MAX" */
#define	APP_PER_ADV_SETS_NUMBER				1
/** Maximum Periodic Advertising Data Length */
#define APP_MAX_LENGTH_PER_ADV_DATA			1024

extern	u8 app_advSet_buffer[];
extern	u8 app_advData_buffer[];
extern	u8 app_scanRspData_buffer[];

extern	u8 testAdvData[];
extern  u8 testScanRspData[];

extern	u8 app_perdAdvSet_buffer[];
extern	u8 app_perdAdvData_buffer[];
/********************************  EXT ADV && PERD ADV buffer FIFO allocation, End **********************************/




/******************************** BIS BCST TX ISO PDU buffer FIFO allocation, Begin **********************************/
/**
 * @brief	bisMaxTxPDU is the maximum number of data octets that can be carried in a BIS Data PDU
 * refer to BLE SPEC "2.6 ISOCHRONOUS PHYSICAL CHANNEL PDU"
 * usage limitation:
 * 1. should be in range of 0 ~ 251
 */
#define		BIS_BCST_MAX_TX_PDU			44 //LC3 10ms-> 40B, mic 4B plus, 44B enough

/**
 * @brief	BIS BCST TX buffer size & number
 *  		BIS BCST TX buffer is shared by all bis_bcst to hold LinkLayer RF TX BIS data.
 * usage limitation for MAX_TX_ISO_FIFO_SIZE:
 * 1. should be greater than or equal to XXX, TODO
 * 2. should be be an integer multiple of 4 (4 Byte align)
 * 3. user can use formula:  size = BIS_PDU_ALLIGN4_RXBUFF(bisMaxTxPDU)
 * usage limitation for ISO_TX_FIFO_NUM:
 * 1. must be: 2^n, (power of 2)
 * 2. at least 4; recommended value: 8, 16
 */
#define		MAX_TX_ISO_FIFO_SIZE		BIS_PDU_ALLIGN4_TXBUFF(BIS_BCST_MAX_TX_PDU)
#define		ISO_TX_FIFO_NUM				(32)


extern	u8	app_bisBcstTxfifo[];
/****************************** BIS BCST TX ISO PDU buffer FIFO allocation, End **************************************/




/*************************** BIS BCST TX ISOAL SDU buffer FIFO allocation, Begin *************************************/
#define		MAX_TX_SDU_LEN				IAL_SDU_ALLIGN4_BUFF(48) //LC3 10ms-> 40B, iso hdr 8B plus, 48B enough

#define		IAL_SDU_TX_FIFO_NUM			(32)

extern	u8	bltSduTxFifo[];
/***************************** BIS BCST TX ISOAL SDU buffer FIFO allocation, End  ************************************/




/********************************** BIS BCST Global Data Definition, Begin *****************************************/
#define		APP_BIG_BCST_NUMBER			1	// Number of Supported BIG_BCST number (must <= LL_BIG_BCST_NUM_MAX)
#define		APP_BIG_SYNC_NUMBER			0	// Number of Supported BIG_SYNC number (must <= LL_BIG_SYNC_NUM_MAX)
#define		APP_BIS_NUM_IN_PER_BIG_BCST	2	// Number of Supported BISes in Per BIG_BCST (must <= LL_BIS_IN_PER_BIG_BCST_NUM_MAX)
#define		APP_BIS_NUM_IN_PER_BIG_SYNC	2	// Number of Supported BISes in Per BIG_SYNC (must <= LL_BIS_IN_PER_BIG_SYNC_NUM_MAX)

// The total number of BIS supported by BIG_BCST
#define		APP_BIS_NUM_IN_ALL_BIG_BCST	(APP_BIG_BCST_NUMBER * APP_BIS_NUM_IN_PER_BIG_BCST)
// The total number of BIS supported by BIG_SYNC
#define		APP_BIS_NUM_IN_ALL_BIG_SYNC	(APP_BIG_SYNC_NUMBER * APP_BIS_NUM_IN_PER_BIG_SYNC)
// The total number of BIS supported by BIG (BCST and SYNC)
#define		APP_TOTAL_BIS_NUMBER		(APP_BIS_NUM_IN_ALL_BIG_BCST + APP_BIS_NUM_IN_ALL_BIG_SYNC)

extern	u8	app_bigBcstParam[];
extern	u8  app_bisToatlParam[];
/********************************** BIS BCST Global Data Definition, end  *****************************************/



#endif /* APP_BUFFER_H_ */

#endif
