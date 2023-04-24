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
#include "app_usb.h"
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"


#if (__PROJECT_B91_AUDIO_BIS_SYNC__)
#if (APP_AUDIO_USB_ENABLE)



extern void usb_handle_irq(void);


#define AUDIO_USB_BUFFER_LENGTH      APP_AUDIO_FRAME_BYTES//(AUDIO_ADPCM_BUFFER_SIZE<<2)

#define AUDIO_SAMPLE_RATE            MIC_SAMPLE_RATE
#define	AUDIO_USB_UNIT_LENGTH        ((AUDIO_SAMPLE_RATE/1000) << 1)
#define	AUDIO_USB_UNIT_COUNT         (AUDIO_USB_BUFFER_LENGTH/AUDIO_USB_UNIT_LENGTH)

u8 sAppAudioDataSerial;
u8 *sAppAudioDataPtr = 0;
u8 gAudioUsbBuffer0[AUDIO_USB_BUFFER_LENGTH];
u8 gAudioUsbBuffer1[AUDIO_USB_BUFFER_LENGTH];
volatile u8 gAudioUsbCount = 0;
u8 gAudioUsbRptr = 0;
u8 gAudioUsbWptr = 0;


volatile u32 AAAA_usb_test001 = 0;
volatile u32 AAAA_usb_test002 = 0;
volatile u32 AAAA_usb_test003 = 0;

void app_usb_init(void)
{
	//set USB ID
	REG_ADDR8(0x1401f4) = 0x65;
	REG_ADDR16(0x1401fe) = 0x08d0;
	REG_ADDR8(0x1401f4) = 0x00;
	
	//////////////// config USB ISO IN/OUT interrupt /////////////////
	reg_usb_ep_irq_mask = BIT(7);			//audio in interrupt enable
	plic_interrupt_enable(IRQ11_USB_ENDPOINT);
	plic_set_priority(IRQ11_USB_ENDPOINT, 1);
	reg_usb_ep6_buf_addr = 0x80;
	reg_usb_ep7_buf_addr = 0x60;
	reg_usb_ep_max_size = (256 >> 3);
	
	app_usb_enable(true);
	
	gAudioUsbCount = 0;
	gAudioUsbRptr = 0;
	gAudioUsbRptr = 0;
}

void app_usb_enable(bool enable)
{
	if(enable) usb_dp_pullup_en(1);
	else usb_dp_pullup_en(0);
}

void app_usb_handler(void)
{
	usb_handle_irq();
}


bool app_usb_fill_buffer(u8 *pData)
{
	if(gAudioUsbCount == 2) return false;
	
	if(gAudioUsbWptr == 0){
		memcpy(gAudioUsbBuffer0, pData, AUDIO_USB_BUFFER_LENGTH);
		gAudioUsbWptr = 1;
	}else{
		memcpy(gAudioUsbBuffer1, pData, AUDIO_USB_BUFFER_LENGTH);
		gAudioUsbWptr = 0;
	}
	
	gAudioUsbCount ++;
	
	return true;
}
u8 *app_usb_get_buffer(void)
{
	if(gAudioUsbCount == 0) return NULL;

	if(gAudioUsbRptr == 0){
		gAudioUsbRptr = 1;
		return gAudioUsbBuffer0;
	}else{
		gAudioUsbRptr = 0;
		return gAudioUsbBuffer1;
	}

}

_attribute_ram_code_ void app_usb_send_handler(void)
{
	reg_usb_ep7_ptr = 0;
	
	if(sAppAudioDataSerial == 0){
		sAppAudioDataPtr = app_usb_get_buffer();
	}
	if(sAppAudioDataPtr == 0){
#if 0
		u8 i;
		for(i=0; i<AUDIO_USB_UNIT_LENGTH; i++){
			reg_usb_ep7_dat = 0x00;
		}
#endif
		return;
	}
	#if 1
	{
		u8 i;
		u8 *pData;
		u8 unitCount;

			
		unitCount = AUDIO_USB_UNIT_COUNT;
		pData = sAppAudioDataPtr+(sAppAudioDataSerial*AUDIO_USB_UNIT_LENGTH);
		for(i=0; i<AUDIO_USB_UNIT_LENGTH; i++){
			reg_usb_ep7_dat = pData[i];
		}
		sAppAudioDataSerial ++;
		if(sAppAudioDataSerial == unitCount){
			sAppAudioDataSerial = 0;
			gAudioUsbCount --;
		}
	}
	#endif
}


_attribute_ram_code_ void  app_usb_irq_handler(void)
{
	if(reg_usb_irq & BIT(7)){
		reg_usb_irq = BIT(7);
		app_usb_send_handler();
		reg_usb_ep7_ctrl = BIT(0);
	}
}


#endif
#endif //#if (__PROJECT_B91_AUDIO_BIS_SYNC__)
