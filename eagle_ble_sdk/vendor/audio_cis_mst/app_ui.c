/********************************************************************************************************
 * @file	app_ui.c
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
#include "app.h"
#if !(LE_AUDIO_DEMO_ENABLE)
#include "default_att.h"
#else
#include "app_att.h"
#endif
#include "app_ui.h"
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "application/keyboard/keyboard.h"
#include "application/usbstd/usbkeycode.h"



int	master_pairing_enable = 0;
int master_unpair_enable = 0;

int master_disconnect_connhandle;   //mark the master connection which is in un_pair disconnection flow




int master_auto_connect = 0;
int user_manual_pairing;



#if (UI_KEYBOARD_ENABLE)

_attribute_ble_data_retention_	int 	key_not_released;
_attribute_ble_data_retention_	int 	key_type;

#define CONSUMER_KEY   	   			1
#define KEYBOARD_KEY   	   			2
#define PAIR_UNPAIR_KEY   	   		3

_attribute_ble_data_retention_  int		startCISTest = 0;

/**
 * @brief   Check changed key value.
 * @param   none.
 * @return  none.
 */
void key_change_proc(void)
{

	u8 key0 = kb_event.keycode[0];
	u8 key1 = kb_event.keycode[1];
//	u8 key_buf[8] = {0,0,0,0,0,0,0,0};

	key_not_released = 1;
	if (kb_event.cnt == 2)   //two key press
	{
		if( (key0 == CR_VOL_UP && key1 == CR_VOL_DN) || (key0 == CR_VOL_DN && key1 == CR_VOL_UP)){
			if(!startCISTest)
			{
				startCISTest = 3;
				my_dump_str_data(APP_DUMP_EN, "SW2 & SW3", &startCISTest, 1);
			}
		}
		else if( (key0 == BTN_PAIR && key1 == CR_VOL_DN) || (key0 == CR_VOL_DN && key1 == BTN_PAIR)){

			if(!startCISTest){
				startCISTest = 4;
				my_dump_str_data(APP_DUMP_EN, "SW2 & SW4", &startCISTest, 1);
			}
		}
	}
	else if(kb_event.cnt == 1)
	{
		if(key0 >= CR_VOL_UP )  //volume up/down
		{
			key_type = CONSUMER_KEY;
			#if !(LE_AUDIO_DEMO_ENABLE)
			u16 consumer_key;
			#endif
			if(key0 == CR_VOL_UP){  	//volume up
				#if !(LE_AUDIO_DEMO_ENABLE)
				consumer_key = MKEY_VOL_UP;
				#endif

				if(!startCISTest){
					startCISTest = 2;
					my_dump_str_data(APP_DUMP_EN, "SW3", &startCISTest, 1);
				}
			}
			else if(key0 == CR_VOL_DN){ //volume down
				#if !(LE_AUDIO_DEMO_ENABLE)
				consumer_key = MKEY_VOL_DN;
				#endif

				if(!startCISTest){
					startCISTest = 1;
					my_dump_str_data(APP_DUMP_EN, "SW2", &startCISTest, 1);
				}
			}


			/*Here is just Telink Demonstration effect. Cause the demo board has limited key to use, when Vol+/Vol- key pressed, we
			send media key "Vol+" or "Vol-" to master for all slave in connection.
			For users, you should known that this is not a good method, you should manage your device and GATT data transfer
			according to  conn_dev_list[]
			 * */
			#if !(LE_AUDIO_DEMO_ENABLE)
			for(int i=MASTER_MAX_NUM; i < (MASTER_MAX_NUM + SLAVE_MAX_NUM); i++){ //slave index is from "MASTER_MAX_NUM" to "MASTER_MAX_NUM + SLAVE_MAX_NUM - 1"
				if(conn_dev_list[i].conn_state){
					blc_gatt_pushHandleValueNotify (conn_dev_list[i].conn_handle, HID_CONSUME_REPORT_INPUT_DP_H, (u8 *)&consumer_key, 2);
				}
			}
			#endif
		}
		else{
			key_type = PAIR_UNPAIR_KEY;

			if(key0 == BTN_PAIR)   //Manual pair triggered by Key Press
			{
				master_pairing_enable = 1;
				my_dump_str_data(APP_DUMP_EN, "UI PAIR begin", 0, 0);
			}
			else if(key0 == BTN_UNPAIR) //Manual un_pair triggered by Key Press
			{
				/*Here is just Telink Demonstration effect. Cause the demo board has limited key to use, only one "un_pair" key is
				 available. When "un_pair" key pressed, we will choose and un_pair one device in connection state */
				if(conn_master_num){ //at least 1 master connection exist

					if(!master_disconnect_connhandle){  //if one master un_pair disconnection flow not finish, here new un_pair not accepted

						/* choose one master connection to disconnect */
						for(int i=0; i < MASTER_MAX_NUM; i++){ //slave index is from 0 to "MASTER_MAX_NUM - 1"
							if(conn_dev_list[i].conn_state){
								master_unpair_enable = conn_dev_list[i].conn_handle;  //mark connHandle on master_unpair_enable
								my_dump_str_data(APP_DUMP_EN, "UI UNPAIR", &master_unpair_enable, 2);
								break;
							}
						}
					}
				}
			}

		}

	}
	else   //kb_event.cnt == 0,  key release
	{
		key_not_released = 0;
		if(key_type == CONSUMER_KEY)
		{
			#if !(LE_AUDIO_DEMO_ENABLE)
			u16 consumer_key = 0;
			//Here is just Telink Demonstration effect. for all slave in connection, send release for previous "Vol+" or "Vol-" to master
			for(int i=MASTER_MAX_NUM; i < (MASTER_MAX_NUM + SLAVE_MAX_NUM); i++){ //slave index is from "MASTER_MAX_NUM" to "MASTER_MAX_NUM + SLAVE_MAX_NUM - 1"
				if(conn_dev_list[i].conn_state){
					blc_gatt_pushHandleValueNotify ( conn_dev_list[i].conn_handle, HID_CONSUME_REPORT_INPUT_DP_H, (u8 *)&consumer_key, 2);
				}
			}
			#endif
		}
		else if(key_type == KEYBOARD_KEY)
		{

		}
		else if(key_type == PAIR_UNPAIR_KEY){
			if(master_pairing_enable){
				master_pairing_enable = 0;
				my_dump_str_data(APP_DUMP_EN, "UI PAIR end", 0, 0);
			}

			if(master_unpair_enable){
				master_unpair_enable = 0;
			}
		}
	}


}



_attribute_ble_data_retention_		static u32 keyScanTick = 0;

/**
 * @brief      keyboard task handler
 * @param[in]  e    - event type
 * @param[in]  p    - Pointer point to event parameter.
 * @param[in]  n    - the length of event parameter.
 * @return     none.
 */
void proc_keyboard (u8 e, u8 *p, int n)
{
	if(clock_time_exceed(keyScanTick, 10 * 1000)){  //keyScan interval: 10mS
		keyScanTick = clock_time();
	}
	else{
		return;
	}

	kb_event.keycode[0] = 0;
	int det_key = kb_scan_key (0, 1);

	if (det_key){
		key_change_proc();
	}
}




#endif   //end of UI_KEYBOARD_ENABLE




/**
 * @brief   BLE Unpair handle for master
 * @param   none.
 * @return  none.
 */
void proc_master_role_unpair(void)
{
#if (!BLE_MASTER_SMP_ENABLE)
    if(blm_manPair.manual_pair && clock_time_exceed(blm_manPair.pair_tick, 2000000)){  //@@
    	blm_manPair.manual_pair = 0;
    }
#endif


	//terminate and un_pair process, Telink demonstration effect: triggered by "un_pair" key press
	if(master_unpair_enable){

		dev_char_info_t* dev_char_info = dev_char_info_search_by_connhandle(master_unpair_enable); //connHandle has marked on on master_unpair_enable

		if( dev_char_info ){ //un_pair device in still in connection state

			if(blc_ll_disconnect(master_unpair_enable, HCI_ERR_REMOTE_USER_TERM_CONN) == BLE_SUCCESS){

				master_disconnect_connhandle = master_unpair_enable; //mark conn_handle

				master_unpair_enable = 0;  //every "un_pair" key can only triggers one connection disconnect


				#if (BLE_MASTER_SIMPLE_SDP_ENABLE)
					// delete ATT handle storage on flash
					dev_char_info_delete_peer_att_handle_by_peer_mac(dev_char_info->peer_adrType, dev_char_info->peer_addr);
				#endif


				// delete this device information(mac_address and distributed keys...) on FLash
				#if (BLE_MASTER_SMP_ENABLE)
					blc_smp_deleteBondingSlaveInfo_by_PeerMacAddress(dev_char_info->peer_adrType, dev_char_info->peer_addr);
				#else
					user_tbl_slave_mac_delete_by_adr(dev_char_info->peer_adrType, dev_char_info->peer_addr);
				#endif

			}

		}
		else{ //un_pair device can not find in device list, it's not connected now

			master_unpair_enable = 0;  //every "un_pair" key can only triggers one connection disconnect
		}

	}
}













