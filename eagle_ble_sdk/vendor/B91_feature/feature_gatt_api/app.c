/********************************************************************************************************
 * @file	app.c
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
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "app_config.h"
#include "app.h"
#include "app_buffer.h"
#include "../default_att.h"
#include "app_ui.h"


#if (FEATURE_TEST_MODE == TEST_GATT_API)


int	master_smp_pending = 0; 		// SMP: security & encryption;




const u8	tbl_advData[] = {
	 5,  DT_COMPLETE_LOCAL_NAME, 				'g', 'a', 't', 't',
	 2,	 DT_FLAGS, 								0x05, 					// BLE limited discoverable mode and BR/EDR not supported
	 3,  DT_APPEARANCE, 						0x80, 0x01, 			// 384, Generic Remote Control, Generic category
	 5,  DT_INCOMPLT_LIST_16BIT_SERVICE_UUID,	0x12, 0x18, 0x0F, 0x18,	// incomplete list of service class UUIDs (0x1812, 0x180F)
};

const u8	tbl_scanRsp [] = {
	 5,  DT_COMPLETE_LOCAL_NAME, 				'g', 'a', 't', 't',
};



u16 startHandle   = 1;
u16 gapServerUUID = 0x1800;
u16 serverDeclarationUUID         = 0x2800;
static const u16 my_characterUUID = GATT_UUID_CHARACTER;

u8 otaUUID[] = WRAPPING_BRACES(TELINK_SPP_DATA_OTA);
static const u8 my_PnPtrs [] = {0x02, 0x8a, 0x24, 0x66, 0x82, 0x01, 0x00};




#define TEST_FIND_INFO_REQ            1
#define TEST_FIND_BY_TYPE_VALUE_REQ   2
#define TEST_READ_BY_TYPE_REQ         3
#define TEST_READ_REQ                 4
#define TEST_READ_BLOB_REQ            5
#define TEST_READ_BY_GROUP_TYPE_REQ   6

#define TEST_API  TEST_READ_BY_GROUP_TYPE_REQ


/** --------------------NOTE---------------------------------------------------
 * the feature test need hardware and firmware follow:
 * Hardware:
 *         1. EVK:    as 4 Master 3 slave device
 *         2. dongle: as slave device
 * Firmware:
 *         1. you need compile this project to gain bin file, then burn the bin file to EVK
 *         2. you need compile "ble sample project" to gain bin file, then burn the bin file to dongle.
 */
void feature_gatt_api_test_mainloop(void)
{
	switch(TEST_API){

	case TEST_FIND_INFO_REQ:
	{
		for(int i=0; i<conn_master_num; i++){
			if(conn_dev_list[i].conn_handle){
				blc_gatt_pushFindInformationRequest(conn_dev_list[i].conn_handle, 0x0001, 0xFFFF);
			}
		}
		break;
	}
	case TEST_FIND_BY_TYPE_VALUE_REQ:
	{
		for(int i=0; i<conn_master_num; i++){
			if(conn_dev_list[i].conn_handle){
				#if(0)
					blc_gatt_pushFindByTypeValueRequest(conn_dev_list[i].conn_handle, 0x0001, 0xFFFF, GATT_UUID_PRIMARY_SERVICE, (u8*)&gapServerUUID, sizeof(gapServerUUID));
				#elif(0)
					blc_gatt_pushFindByTypeValueRequest(conn_dev_list[i].conn_handle, 0x0001, 0xFFFF, GATT_UUID_CHARACTER, (u8*)&my_PnCharVal, sizeof(my_PnCharVal));
				#elif(1)
					blc_gatt_pushFindByTypeValueRequest(conn_dev_list[i].conn_handle, 0x0001, 0xFFFF, CHARACTERISTIC_UUID_PNP_ID, (u8*)&my_PnPtrs, sizeof(my_PnPtrs));
				#endif
			}
		}
		break;
	}
	case TEST_READ_BY_TYPE_REQ:
	{
		for(int i=0; i<conn_master_num; i++){
			if(conn_dev_list[i].conn_handle){
				blc_gatt_pushReadByTypeRequest(conn_dev_list[i].conn_handle, 0x0001, 0xFFFF, (u8*)&otaUUID, sizeof(otaUUID));
			}
		}
		break;
	}
	case TEST_READ_REQ:
	{
		for(int i=0; i<conn_master_num; i++){
			if(conn_dev_list[i].conn_handle){
				blc_gatt_pushReadRequest(conn_dev_list[i].conn_handle, 3);//device name
			}
		}
		break;
	}
	case TEST_READ_BLOB_REQ:
	{
		for(int i=0; i<conn_master_num; i++){
			if(conn_dev_list[i].conn_handle){
				blc_gatt_pushReadBlobRequest(conn_dev_list[i].conn_handle, 3, 1);//device name
			}
		}
		break;
	}
	case TEST_READ_BY_GROUP_TYPE_REQ:
	{
		for(int i=0; i<conn_master_num; i++){
			if(conn_dev_list[i].conn_handle){

				#if (0)
				blc_gatt_pushReadByGroupTypeRequest(conn_dev_list[i].conn_handle, 0x0001, 0xFFFF, \
													(u8*)&serverDeclarationUUID, sizeof(serverDeclarationUUID));
				#elif(1)
				blc_gatt_pushReadByGroupTypeRequest(conn_dev_list[i].conn_handle, 0x0001, 0xFFFF, \
													(u8*)&my_characterUUID, sizeof(my_characterUUID));
				#endif
			}
		}
		break;
	}

	default:
		break;
	}
}

/**
 * @brief      BLE Adv report event handler
 * @param[in]  p         Pointer point to event parameter buffer.
 * @return
 */

int app_le_adv_report_event_handle(u8 *p)
{
	event_adv_report_t *pa = (event_adv_report_t *)p;
	s8 rssi = pa->data[pa->len];


	/*********************** Master Create connection demo: Key press or ADV pair packet triggers pair  ********************/
	if(master_smp_pending){ 	 //if previous connection SMP & SDP not finish, can not create a new connection
		return 1;
	}
	if (master_disconnect_connhandle){ //one master connection is in un_pair disconnection flow, do not create a new one
		return 1;
	}

	int master_auto_connect = 0;
	int user_manual_pairing = 0;

	//manual pairing methods 1: key press triggers
	user_manual_pairing = master_pairing_enable && (rssi > -66);  //button trigger pairing(RSSI threshold, short distance)

	#if (BLE_MASTER_SMP_ENABLE)
		master_auto_connect = blc_smp_searchBondingSlaveDevice_by_PeerMacAddress(pa->adr_type, pa->mac);
	#else
		//search in slave mac_address table to find whether this device is an old device which has already paired with master
		master_auto_connect = user_tbl_slave_mac_search(pa->adr_type, pa->mac);
	#endif


	if(master_auto_connect || user_manual_pairing){

		/* send create connection command to Controller, trigger it switch to initiating state. After this command, Controller
		 * will scan all the ADV packets it received but not report to host, to find the specified device(mac_adr_type & mac_adr),
		 * then send a "CONN_REQ" packet, enter to connection state and send a connection complete event
		 * (HCI_SUB_EVT_LE_CONNECTION_COMPLETE) to Host*/
		u8 status = blc_ll_createConnection( SCAN_INTERVAL_100MS, SCAN_WINDOW_100MS, INITIATE_FP_ADV_SPECIFY,  \
								 pa->adr_type, pa->mac, OWN_ADDRESS_PUBLIC, \
								 CONN_INTERVAL_31P25MS, CONN_INTERVAL_31P25MS, 0, CONN_TIMEOUT_4S, \
								 0, 0xFFFF);


		if(status == BLE_SUCCESS){ //create connection success
			#if (!BLE_MASTER_SMP_ENABLE)
			    // for Telink referenced pair&bonding,
				if(user_manual_pairing && !master_auto_connect){  //manual pair but not auto connect
					blm_manPair.manual_pair = 1;
					blm_manPair.mac_type = pa->adr_type;
					memcpy(blm_manPair.mac, pa->mac, 6);
					blm_manPair.pair_tick = clock_time();
				}
			#endif
		}
	}
	/*********************** Master Create connection demo code end  *******************************************************/


	return 0;
}


/**
 * @brief      BLE Connection complete event handler
 * @param[in]  p         Pointer point to event parameter buffer.
 * @return
 */
int app_le_connection_complete_event_handle(u8 *p)
{
	hci_le_connectionCompleteEvt_t *pConnEvt = (hci_le_connectionCompleteEvt_t *)p;

	if(pConnEvt->status == BLE_SUCCESS){

		dev_char_info_insert_by_conn_event(pConnEvt);

		if( pConnEvt->role == LL_ROLE_MASTER ) // master role, process SMP and SDP if necessary
		{
			#if (BLE_MASTER_SMP_ENABLE)
				master_smp_pending = pConnEvt->connHandle; // this connection need SMP
			#else
				//manual pairing, device match, add this device to slave mac table
				if(blm_manPair.manual_pair && blm_manPair.mac_type == pConnEvt->peerAddrType && !memcmp(blm_manPair.mac, pConnEvt->peerAddr, 6)){
					blm_manPair.manual_pair = 0;

					user_tbl_slave_mac_add(pConnEvt->peerAddrType, pConnEvt->peerAddr);
				}
			#endif
		}
	}

	return 0;
}



/**
 * @brief      BLE Disconnection event handler
 * @param[in]  p         Pointer point to event parameter buffer.
 * @return
 */
int 	app_disconnect_event_handle(u8 *p)
{
	event_disconnection_t	*pCon = (event_disconnection_t *)p;

	//terminate reason
	if(pCon->reason == HCI_ERR_CONN_TIMEOUT){  	//connection timeout

	}
	else if(pCon->reason == HCI_ERR_REMOTE_USER_TERM_CONN){  	//peer device send terminate command on link layer

	}
	//master host disconnect( blm_ll_disconnect(current_connHandle, HCI_ERR_REMOTE_USER_TERM_CONN) )
	else if(pCon->reason == HCI_ERR_CONN_TERM_BY_LOCAL_HOST){

	}
	else{

	}



	//if previous connection SMP & SDP not finished, clear flag
#if (BLE_MASTER_SMP_ENABLE)
	if(master_smp_pending == pCon->connHandle){
		master_smp_pending = 0;
	}
#endif

	if(master_disconnect_connhandle == pCon->connHandle){  //un_pair disconnection flow finish, clear flag
		master_disconnect_connhandle = 0;
	}

	dev_char_info_delete_by_connhandle(pCon->connHandle);




	return 0;
}



//////////////////////////////////////////////////////////
// event call back
//////////////////////////////////////////////////////////
/**
 * @brief      BLE controller event handler call-back.
 * @param[in]  h       event type
 * @param[in]  p       Pointer point to event parameter buffer.
 * @param[in]  n       the length of event parameter.
 * @return
 */
int app_controller_event_callback (u32 h, u8 *p, int n)
{
	if (h &HCI_FLAG_EVENT_BT_STD)		//Controller HCI event
	{
		u8 evtCode = h & 0xff;

		//------------ disconnect -------------------------------------
		if(evtCode == HCI_EVT_DISCONNECTION_COMPLETE)  //connection terminate
		{
			app_disconnect_event_handle(p);
		}
		else if(evtCode == HCI_EVT_LE_META)  //LE Event
		{
			u8 subEvt_code = p[0];

			//------hci le event: le connection complete event---------------------------------
			if (subEvt_code == HCI_SUB_EVT_LE_CONNECTION_COMPLETE)	// connection complete
			{
				app_le_connection_complete_event_handle(p);
			}
			//--------hci le event: le adv report event ----------------------------------------
			else if (subEvt_code == HCI_SUB_EVT_LE_ADVERTISING_REPORT)	// ADV packet
			{
				//after controller is set to scan state, it will report all the adv packet it received by this event

				app_le_adv_report_event_handle(p);
			}
			//------hci le event: le connection update complete event-------------------------------
			else if (subEvt_code == HCI_SUB_EVT_LE_CONNECTION_UPDATE_COMPLETE)	// connection update
			{

			}
		}
	}


	return 0;

}




/**
 * @brief      BLE host event handler call-back.
 * @param[in]  h       event type
 * @param[in]  para    Pointer point to event parameter buffer.
 * @param[in]  n       the length of event parameter.
 * @return
 */
int app_host_event_callback (u32 h, u8 *para, int n)
{
	u8 event = h & 0xFF;

	switch(event)
	{
		case GAP_EVT_SMP_PAIRING_BEGIN:
		{
		}
		break;

		case GAP_EVT_SMP_PAIRING_SUCCESS:
		{
		}
		break;

		case GAP_EVT_SMP_PAIRING_FAIL:
		{
		#if (BLE_MASTER_SMP_ENABLE)
			gap_smp_pairingFailEvt_t *p = (gap_smp_pairingFailEvt_t *)para;

			if( dev_char_get_conn_role_by_connhandle(p->connHandle) == LL_ROLE_MASTER){  //master connection
				if(master_smp_pending == p->connHandle){
					master_smp_pending = 0;
				}
			}
		#endif
		}
		break;

		case GAP_EVT_SMP_CONN_ENCRYPTION_DONE:
		{

		}
		break;

		case GAP_EVT_SMP_SECURITY_PROCESS_DONE:
		{
		#if (BLE_MASTER_SMP_ENABLE)
			gap_smp_connEncDoneEvt_t* p = (gap_smp_connEncDoneEvt_t*)para;

			if( dev_char_get_conn_role_by_connhandle(p->connHandle) == LL_ROLE_MASTER){  //master connection

				if(master_smp_pending == p->connHandle){
					master_smp_pending = 0;
				}

			}
		#endif
		}
		break;
		case GAP_EVT_ATT_EXCHANGE_MTU:
		{

		}
		break;

		case GAP_EVT_GATT_HANDLE_VLAUE_CONFIRM:
		{

		}
		break;

		default:
		break;
	}

	return 0;
}





u8 A_attRspPkt[200];

#define			HID_HANDLE_CONSUME_REPORT			25
#define			HID_HANDLE_KEYBOARD_REPORT			29
#define			AUDIO_HANDLE_MIC					52
#define			OTA_HANDLE_DATA						48

/**
 * @brief      BLE GATT data handler call-back.
 * @param[in]  connHandle     connection handle.
 * @param[in]  pkt             Pointer point to data packet buffer.
 * @return
 */
int app_gatt_data_handler (u16 connHandle, u8 *pkt)
{

	if( dev_char_get_conn_role_by_connhandle(connHandle) == LL_ROLE_MASTER)   //GATT data for Master
	{
		rf_packet_att_t *pAtt = (rf_packet_att_t*)pkt;
		switch(pAtt->opcode)
		{
		case ATT_OP_FIND_INFO_RSP:
			memcpy(A_attRspPkt, (u8*)&pAtt->opcode, min(sizeof(A_attRspPkt), pAtt->l2capLen));
			break;
		case ATT_OP_FIND_BY_TYPE_VALUE_RSP:
			memcpy(A_attRspPkt, (u8*)&pAtt->opcode, min(sizeof(A_attRspPkt), pAtt->l2capLen));
			break;
		case ATT_OP_READ_RSP:
			memcpy(A_attRspPkt, (u8*)&pAtt->opcode, min(sizeof(A_attRspPkt), pAtt->l2capLen));
			break;
		case ATT_OP_READ_BLOB_RSP:
			memcpy(A_attRspPkt, (u8*)&pAtt->opcode, min(sizeof(A_attRspPkt), pAtt->l2capLen));
			break;
		case ATT_OP_READ_BY_TYPE_RSP:
			memcpy(A_attRspPkt, (u8*)&pAtt->opcode, min(sizeof(A_attRspPkt), pAtt->l2capLen));
			break;
		case ATT_OP_READ_BY_GROUP_TYPE_RSP:
			memcpy(A_attRspPkt, (u8*)&pAtt->opcode, min(sizeof(A_attRspPkt), pAtt->l2capLen));
			break;
		default:
			break;
		}
	}
	else{//GATT data for Slave
	}


	return 0;
}


















///////////////////////////////////////////

/**
 * @brief		user initialization when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
_attribute_no_inline_ void user_init_normal(void)
{
	/* random number generator must be initiated here( in the beginning of user_init_nromal).
	 * When deepSleep retention wakeUp, no need initialize again */
	random_generator_init();

//////////////////////////// BLE stack Initialization  Begin //////////////////////////////////

	u8  mac_public[6];
	u8  mac_random_static[6];
	blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);


	//////////// LinkLayer Initialization  Begin /////////////////////////
	blc_ll_initBasicMCU();

	blc_ll_initStandby_module(mac_public);						   //mandatory

    blc_ll_initLegacyAdvertising_module(); 	//adv module: 		 mandatory for BLE slave,

    blc_ll_initLegacyScanning_module(); 	//scan module: 		 mandatory for BLE master

	blc_ll_initInitiating_module();			//initiate module: 	 mandatory for BLE master

	blc_ll_initAclConnection_module();
	blc_ll_initAclMasterRole_module();
	blc_ll_initAclSlaveRole_module();



	blc_ll_setMaxConnectionNumber( MASTER_MAX_NUM, SLAVE_MAX_NUM);

	blc_ll_setAclConnMaxOctetsNumber(ACL_CONN_MAX_RX_OCTETS, ACL_MASTER_MAX_TX_OCTETS, ACL_SLAVE_MAX_TX_OCTETS);

	/* all ACL connection share same RX FIFO */
	blc_ll_initAclConnRxFifo(app_acl_rxfifo, ACL_RX_FIFO_SIZE, ACL_RX_FIFO_NUM);
	/* ACL Master TX FIFO */
	blc_ll_initAclConnMasterTxFifo(app_acl_mstTxfifo, ACL_MASTER_TX_FIFO_SIZE, ACL_MASTER_TX_FIFO_NUM, MASTER_MAX_NUM);
	/* ACL Slave TX FIFO */
	blc_ll_initAclConnSlaveTxFifo(app_acl_slvTxfifo, ACL_SLAVE_TX_FIFO_SIZE, ACL_SLAVE_TX_FIFO_NUM, SLAVE_MAX_NUM);


	blc_ll_setAclMasterConnectionInterval(CONN_INTERVAL_31P25MS);

	rf_set_power_level_index (RF_POWER_P3dBm);


	//////////// LinkLayer Initialization  End /////////////////////////



	//////////// HCI Initialization  Begin /////////////////////////
	blc_hci_registerControllerDataHandler (blc_l2cap_pktHandler);

	blc_hci_registerControllerEventHandler(app_controller_event_callback); //controller hci event to host all processed in this func

	//bluetooth event
	blc_hci_setEventMask_cmd (HCI_EVT_MASK_DISCONNECTION_COMPLETE);

	//bluetooth low energy(LE) event
	blc_hci_le_setEventMask_cmd(		HCI_LE_EVT_MASK_CONNECTION_COMPLETE  \
									|	HCI_LE_EVT_MASK_ADVERTISING_REPORT \
									|   HCI_LE_EVT_MASK_CONNECTION_UPDATE_COMPLETE);


	u8 check_status = blc_controller_check_appBufferInitialization();
	if(check_status != BLE_SUCCESS){
		/* here user should set some log to know which application buffer incorrect*/
		write_log32(check_status);
		while(1);
	}
	//////////// HCI Initialization  End /////////////////////////


	//////////// Host Initialization  Begin /////////////////////////
	/* Host Initialization */
	/* GAP initialization must be done before any other host feature initialization !!! */
	blc_gap_init();

	/* L2CAP buffer Initialization */
	blc_l2cap_initAclConnMasterMtuBuffer(mtu_m_rx_fifo, MTU_M_BUFF_SIZE_MAX, 			0,					 0);
	blc_l2cap_initAclConnSlaveMtuBuffer(mtu_s_rx_fifo, MTU_S_BUFF_SIZE_MAX, mtu_s_tx_fifo, MTU_S_BUFF_SIZE_MAX);

	/* GATT Initialization */
	my_gatt_init();

	blc_gatt_register_data_handler(app_gatt_data_handler);

	/* SMP Initialization */
	#if (BLE_SLAVE_SMP_ENABLE || BLE_MASTER_SMP_ENABLE)
		blc_smp_configPairingSecurityInfoStorageAddressAndSize(FLASH_ADR_SMP_PAIRING, FLASH_SMP_PAIRING_MAX_SIZE);
	#endif

	#if (BLE_SLAVE_SMP_ENABLE)  //Slave SMP Enable
		blc_smp_setSecurityLevel_slave(Unauthenticated_Pairing_with_Encryption);  //LE_Security_Mode_1_Level_2
	#else
		blc_smp_setSecurityLevel_slave(No_Security);
	#endif

	#if (BLE_MASTER_SMP_ENABLE) //Master SMP Enable
		blc_smp_setSecurityLevel_master(Unauthenticated_Pairing_with_Encryption);  //LE_Security_Mode_1_Level_2
	#else
		blc_smp_setSecurityLevel_master(No_Security);
		user_master_host_pairing_management_init(); 		//Telink referenced pairing&bonding without standard pairing in BLE Spec
	#endif

	blc_smp_smpParamInit();


	//host(GAP/SMP/GATT/ATT) event process: register host event callback and set event mask
	blc_gap_registerHostEventHandler( app_host_event_callback );
	blc_gap_setEventMask( GAP_EVT_MASK_SMP_PAIRING_BEGIN 			|  \
						  GAP_EVT_MASK_SMP_PAIRING_SUCCESS   		|  \
						  GAP_EVT_MASK_SMP_PAIRING_FAIL				|  \
						  GAP_EVT_MASK_SMP_SECURITY_PROCESS_DONE);
	//////////// Host Initialization  End /////////////////////////

//////////////////////////// BLE stack Initialization  End //////////////////////////////////




//////////////////////////// User Configuration for BLE application ////////////////////////////
	blc_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );
	blc_ll_setScanRspData( (u8 *)tbl_scanRsp, sizeof(tbl_scanRsp));
	blc_ll_setAdvParam(ADV_INTERVAL_30MS, ADV_INTERVAL_30MS, ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, 0, NULL, BLT_ENABLE_ADV_ALL, ADV_FP_NONE);
	blc_ll_setAdvEnable(BLC_ADV_ENABLE);  //ADV enable

	blc_ll_setScanParameter(SCAN_TYPE_PASSIVE, SCAN_INTERVAL_100MS, SCAN_WINDOW_100MS, OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_ANY);
	blc_ll_setScanEnable (BLC_SCAN_ENABLE, DUP_FILTER_DISABLE);

////////////////////////////////////////////////////////////////////////////////////////////////
}




/**
 * @brief		user initialization when MCU wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
void user_init_deepRetn(void)
{

}


/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////

/**
 * @brief     BLE main loop
 * @param[in]  none.
 * @return     none.
 */
int main_idle_loop (void)
{

	////////////////////////////////////// BLE entry /////////////////////////////////
	blc_sdk_main_loop();


	////////////////////////////////////// UI entry /////////////////////////////////
	#if (UI_KEYBOARD_ENABLE)
		proc_keyboard (0,0, 0);
	#endif

	static u32 gatt_api_testStarttick;
	if(clock_time_exceed(gatt_api_testStarttick, 2*1000*1000)){

		gatt_api_testStarttick = clock_time();

		feature_gatt_api_test_mainloop();

		gpio_toggle(GPIO_LED_RED);
	}


	proc_master_role_unpair();


	return 0; //must return 0 due to SDP flow
}



_attribute_no_inline_ void main_loop (void)
{
	main_idle_loop ();
}

#endif



