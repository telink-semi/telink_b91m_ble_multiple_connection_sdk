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
#include "app_config.h"
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#if (__PROJECT_B91_AUDIO_BIS_BCST__)
#if (LE_AUDIO_DEMO_ENABLE)
#include "app_buffer.h"
#include "app_audio.h"
#include "app_lc3.h"


#define APP_AUDIO_INPUT_DMIC      0
#define APP_AUDIO_INPUT_LINEIN    1
#define APP_AUDIO_INPUT_SOURCE    APP_AUDIO_INPUT_DMIC


u8 gAppAudioOutSDU[APP_AUDIO_COMP_BUFF_LEN+40] = {
		0,0,0,0,
		0,0,
		0,0,

		/* HCI ISO data header (4B) */
		0x20, 0x40,  //handle
		U16_LO(APP_AUDIO_COMP_BUFF_LEN+8), U16_HI(APP_AUDIO_COMP_BUFF_LEN+8),

		/*** ISO SDU Load ***/

		/* ISO Data header(8B) */
		0,0,0,0,	//time-stamp (TS_flag == 1)
		1,0,        //packet SN
		U16_LO(APP_AUDIO_COMP_BUFF_LEN), U16_HI(APP_AUDIO_COMP_BUFF_LEN),  //sdu_len

		/* ISO Data */
		1,2,3,4,5,6,7,8,9,10,
};


#define APP_AUDIO_MIC_BUFFER_SIZE    (APP_AUDIO_FRAME_BYTES << 1)

u8	 gAppAudioBuffer[APP_AUDIO_MIC_BUFFER_SIZE];
bool gAppAudioEnable = false;
bool gAppAudioIsSend = false;

extern u8  app_bisBcstNum;
extern u16 app_bisBcstHandle[APP_BIS_NUM_IN_PER_BIG_BCST];

int app_audio_init(void)
{
	#if (APP_AUDIO_BIS_INTERVEL == 10)
	app_lc3_init(APP_LC3_INTERVAL_10MS);
	#else
	app_lc3_init(APP_LC3_INTERVAL_7_5MS);
	#endif

//	gAppAudioEnable = false;
	gAppAudioIsSend = true;

	app_audio_enable(true);
	#if (APP_AUDIO_DMIC_ENABLE)
		#if (APP_AUDIO_INPUT_SOURCE == APP_AUDIO_INPUT_DMIC)
		audio_set_dmic_pin(DMIC_GROUPD_D4_DAT_D5_D6_CLK);// DMIC_GROUPB_B2_DAT_B3_B4_CLK
		audio_init(DMIC_IN_TO_BUF_TO_LINE_OUT, AUDIO_16K, MONO_BIT_16);
		#elif (APP_AUDIO_INPUT_SOURCE == APP_AUDIO_INPUT_LINEIN)
		audio_init(LINE_IN_TO_BUF_TO_LINE_OUT, AUDIO_16K, MONO_BIT_16);
		#endif
	audio_rx_dma_chain_init(DMA2, (u16*)gAppAudioBuffer, APP_AUDIO_MIC_BUFFER_SIZE);
//	audio_tx_dma_chain_init(DMA3, (u16*)gAppAudioBuffer, APP_AUDIO_MIC_BUFFER_SIZE);
	#endif
	
	return 0;
}
void app_audio_deinit(void)
{
	
}

int app_audio_enable(bool enable)
{
	if(enable){
		
		gAppAudioEnable = true;
	}else{
		gAppAudioEnable = false;
		audio_clk_en(0,0);
		audio_rx_dma_dis();
		audio_codec_adc_power_down();
	}
	return 0;
}


void app_audio_handler(void)
{
	app_audio_send_handler();
}

void app_audio_send_handler(void)
{
	s16 *ps;
	static u16 sRawDataRptr = 0;
	u16 micWptr = (audio_get_rx_dma_wptr(DMA2) - (u32)gAppAudioBuffer) >> 1;
	u16 l = (micWptr >= sRawDataRptr) ? (micWptr-sRawDataRptr) : 0xffff;
	
	if(l < APP_AUDIO_FRAME_SAMPLE) return;

	if(!gAppAudioEnable || !gAppAudioIsSend) return;

	ps = ((s16*)gAppAudioBuffer) + sRawDataRptr;
	
	//app_usb_fill_buffer((u8*)ps);
//	app_lc3_encode((u8*)ps, APP_AUDIO_FRAME_BYTES, gAppAudioOutSDU+20);

	u8 coded_raw[40];
	app_lc3_encode((u8*)ps, APP_AUDIO_FRAME_BYTES, coded_raw);
	
	//my_dump_str_data(APP_DUMP_EN,"lc3 encode", &ret, 1);
	DBG_CHN8_TOGGLE;

	//send BIS PDU dft: unframed PDU, framed PDU TODO: latter
	for(int i = 0; i < app_bisBcstNum; i++){


		blc_bis_sendData(app_bisBcstHandle[i], coded_raw, APP_AUDIO_COMP_BUFF_LEN);
	
	}

	sRawDataRptr = sRawDataRptr ? 0 : APP_AUDIO_FRAME_SAMPLE;
}


#endif //LE_AUDIO_DEMO_ENABLE
#endif //#if (__PROJECT_B91_AUDIO_BIS_BCST__)
