/********************************************************************************************************
 * @file	app_buffer.c
 *
 * @brief	This is the source file for BLE SDK
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
#include "app_buffer.h"
#include "app_config.h"
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"



#if (__PROJECT_B91_AUDIO_BIS_SYNC__)



/************************* BIS SYNC RX ISO PDU buffer FIFO allocation, Begin ******************************/
/**
 * @brief	BIS SYNC RX buffer. size & number defined in app_buffer.h
 */
_attribute_ble_data_retention_	u8	app_bisSyncRxfifo[MAX_RX_ISO_FIFO_SIZE * ISO_RX_FIFO_NUM * APP_BIS_NUM_IN_ALL_BIG_SYNC] = {0};

/************************* BIS SYNC RX ISO PDU buffer FIFO allocation, End ********************************/



/************************ BIS SYNC RX ISOAL SDU buffer FIFO allocation, Begin *****************************/
_attribute_ble_data_retention_	u8	bltSduRxFifo[MAX_RX_SDU_LEN * IAL_SDU_RX_FIFO_NUM * APP_BIS_NUM_IN_ALL_BIG_SYNC];
/************************ BIS SYNC RX ISOAL SDU buffer FIFO allocation, End  ******************************/




/********************************** BIS BSYNC Global Data Definition, Begin ***************************************/
_attribute_ble_data_retention_	u8  app_bigSyncParam[BIG_SYNC_PARAM_LENGTH * APP_BIG_SYNC_NUMBER];
_attribute_ble_data_retention_	u8  app_bisToatlParam[BIS_PARAM_LENGTH * APP_TOTAL_BIS_NUMBER];
/********************************** BIS BSYNC Global Data Definition, End  ****************************************/



#endif






