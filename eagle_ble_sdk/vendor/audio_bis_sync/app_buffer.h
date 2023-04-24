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

#include "tl_common.h"


#if (__PROJECT_B91_AUDIO_BIS_SYNC__)



/******************************** BIS SYNC RX ISO PDU buffer FIFO allocation, Begin **********************************/
/**
 * @brief	bisMaxRxPDU is the maximum number of data octets that can be carried in a BIS Data PDU
 * refer to BLE SPEC "2.6 ISOCHRONOUS PHYSICAL CHANNEL PDU"
 * usage limitation:
 * 1. should be in range of 0 ~ 251
 */
#define		BIS_SYNC_MAX_RX_PDU			44 //LC3 10ms-> 40B, mic 4B plus, 44B enough

/**
 * @brief	BIS SYNC RX buffer size & number
 *  		BIS SYNC RX buffer is shared by all bis_sync to hold LinkLayer RF TX BIS data.
 * usage limitation for MAX_RX_ISO_FIFO_SIZE:
 * 1. should be greater than or equal to XXX, TODO
 * 2. should be be an integer multiple of 4 (4 Byte align)
 * 3. user can use formula:  size = BIS_PDU_ALLIGN4_RXBUFF(bisMaxRxPDU)
 * usage limitation for ISO_RX_FIFO_NUM:
 * 1. must be: 2^n, (power of 2)
 * 2. at least 4; recommended value: 8, 16
 */
#define		MAX_RX_ISO_FIFO_SIZE		BIS_PDU_ALLIGN4_RXBUFF(BIS_SYNC_MAX_RX_PDU)
#define		ISO_RX_FIFO_NUM				(32)


extern	u8	app_bisSyncRxfifo[];
/******************************* BIS SYNC RX ISO PDU buffer FIFO allocation, End *************************************/




/*************************** BIS SYNC RX ISOAL SDU buffer FIFO allocation, Begin *************************************/
#define		MAX_RX_SDU_LEN				IAL_SDU_ALLIGN4_BUFF(48) //LC3 10ms-> 40B, iso hdr 8B plus, 48B enough

#define		IAL_SDU_RX_FIFO_NUM			(32)

extern	u8	bltSduRxFifo[];
/***************************** BIS SYNC RX ISOAL SDU buffer FIFO allocation, End  ************************************/




/********************************** BIS BYNC Global Data Definition, Begin *****************************************/
#define		APP_BIG_BCST_NUMBER			0	// Number of Supported BIG_BCST number (must <= LL_BIG_BCST_NUM_MAX)
#define		APP_BIG_SYNC_NUMBER			1	// Number of Supported BIG_SYNC number (must <= LL_BIG_SYNC_NUM_MAX)
#define		APP_BIS_NUM_IN_PER_BIG_BCST	2	// Number of Supported BISes in Per BIG_BCST (must <= LL_BIS_IN_PER_BIG_BCST_NUM_MAX)
#define		APP_BIS_NUM_IN_PER_BIG_SYNC	2	// Number of Supported BISes in Per BIG_SYNC (must <= LL_BIS_IN_PER_BIG_SYNC_NUM_MAX)

// The total number of BIS supported by BIG_BCST
#define		APP_BIS_NUM_IN_ALL_BIG_BCST	(APP_BIG_BCST_NUMBER * APP_BIS_NUM_IN_PER_BIG_BCST)
// The total number of BIS supported by BIG_SYNC
#define		APP_BIS_NUM_IN_ALL_BIG_SYNC	(APP_BIG_SYNC_NUMBER * APP_BIS_NUM_IN_PER_BIG_SYNC)
// The total number of BIS supported by BIG (BCST and SYNC)
#define		APP_TOTAL_BIS_NUMBER		(APP_BIS_NUM_IN_ALL_BIG_BCST + APP_BIS_NUM_IN_ALL_BIG_SYNC)

extern	u8	app_bigSyncParam[];
extern	u8  app_bisToatlParam[];
/********************************** BIS SYNC Global Data Definition, end  *****************************************/



#endif /* APP_BUFFER_H_ */

#endif
