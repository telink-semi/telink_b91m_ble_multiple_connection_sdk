/********************************************************************************************************
 * @file	app_audio.c
 *
 * @brief	This is the source file for BLE SDK
 *
 * @author	BLE GROUP
 * @date	2021.01
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
#include "app_audio.h"
#include "app_config.h"
#include "app_lc3.h"
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "algorithm/lc3/inc/lc3_types.h"
#include "algorithm/lc3/inc/lc3_enc.h"
#include "algorithm/lc3/inc/lc3_dec.h"



#if (__PROJECT_B91_AUDIO_BIS_BCST__)
#if (LE_AUDIO_DEMO_ENABLE)


u8 gAppLc3CurMode = 0;
LC3_ENC_STRU gAppLc3EncCtrl;


int app_lc3_init(u8 interval)
{
	LC3ENC_Error ret = 0;

	if(interval == APP_LC3_INTERVAL_10MS){
		ret = lc3enc_init(&gAppLc3EncCtrl, 16000, 1, 32000, 0);
	}else if(interval == APP_LC3_INTERVAL_7_5MS){
		ret = lc3enc_init(&gAppLc3EncCtrl, 16000, 1, 32000, 1);
	}else{
		return -1;
	}
	if(ret != LC3ENC_OK){}

	gAppLc3CurMode = interval;
	
	return 0;
}


int app_lc3_encode(u8 *pRawData, u16 dataLen, u8 *pDstBuff)
{
	LC3ENC_Error ret;

	if(gAppLc3CurMode == 0){
		if(dataLen != 320) return -1;
	}else if(gAppLc3CurMode == 0){
		if(dataLen != 240) return -1;
	}

	ret = lc3enc_encode_frame(&gAppLc3EncCtrl, (s16*)pRawData, pDstBuff);
	if(ret != LC3ENC_OK) return-1;

	return 0;
}



#endif //#if (LE_AUDIO_DEMO_ENABLE)
#endif //#if (__PROJECT_B91_AUDIO_BIS_BCST__)

