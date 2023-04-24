/********************************************************************************************************
 * @file	app_att.h
 *
 * @brief	This is the header file for BLE SDK
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
#ifndef BLM_ATT_H_
#define BLM_ATT_H_

#include "tl_common.h"

#if (LE_AUDIO_DEMO_ENABLE)

#include "stack/ble/profile/audio/audio_cap.h"

#define APP_GMCS_CCID    0x01
#define APP_GTBS_CCID    0x11


// Context Type
#define AUDIO_CONTEXT_TYPE_UNSPECIFIED         BIT(0) // Unspecified. Matches any audio content.
#define AUDIO_CONTEXT_TYPE_CONVERSATIONAL      BIT(1) // Conversation between humans as, for example, in telephony or video calls.
#define AUDIO_CONTEXT_TYPE_MEDIA               BIT(2) // Media as, for example, in music, public radio, podcast or video soundtrack.
#define AUDIO_CONTEXT_TYPE_INSTRUCTIONAL       BIT(3) // Instructional audio as, for example, in navigation, traffic announcements or user guidance
#define AUDIO_CONTEXT_TYPE_ATTENTION_SEEKING   BIT(4) // Attention seeking audio as, for example, in beeps signalling arrival of a message or keyboard clicks. 
#define AUDIO_CONTEXT_TYPE_IMMEDIATE_ALERT     BIT(5) // Immediate alerts as, for example, in a low battery alarm, timer expiry or alarm clock.
#define AUDIO_CONTEXT_TYPE_MAN_MACHINE         BIT(6) // Man machine communication as, for example, with voice recognition or virtual assistant.
#define AUDIO_CONTEXT_TYPE_EMERGENCY_ALERT     BIT(7) // Emergency alerts as, for example, with fire alarms or other urgent alerts.
#define AUDIO_CONTEXT_TYPE_RINGTONE            BIT(8) // Ringtone as in a call alert.
#define AUDIO_CONTEXT_TYPE_TV                  BIT(9) // Audio associated with a television program and/or with metadata conforming to the Bluetooth Broadcast TV profile.


#define MCP_CTRL_OPCODE_PLAY               0x01
#define MCP_CTRL_OPCODE_PAUSE              0x02
#define MCP_CTRL_OPCODE_FAST_REWIND        0x03
#define MCP_CTRL_OPCODE_FAST_FORWARD       0x04
#define MCP_CTRL_OPCODE_STOP               0x05
#define MCP_CTRL_OPCODE_MOVE_RELATIVE      0x10
#define MCP_CTRL_OPCODE_PREVIOUS_SEGMENT   0x20
#define MCP_CTRL_OPCODE_NEXT_SEGMENT       0x21
#define MCP_CTRL_OPCODE_FIRST_SEGMENT      0x22
#define MCP_CTRL_OPCODE_LAST_SEGMENT       0x23
#define MCP_CTRL_OPCODE_GOTO_SEGMENT       0x24
#define MCP_CTRL_OPCODE_PREVIOUS_TRACK     0x30
#define MCP_CTRL_OPCODE_NEXT_TRACK         0x31
#define MCP_CTRL_OPCODE_FIRST_TRACK        0x32
#define MCP_CTRL_OPCODE_LAST_TRACK         0x33
#define MCP_CTRL_OPCODE_GOTO_TRACK         0x34
#define MCP_CTRL_OPCODE_PREVIOUS_GROUP     0x40
#define MCP_CTRL_OPCODE_NEXT_GROUP         0x41
#define MCP_CTRL_OPCODE_FIRST_GROUP        0x42
#define MCP_CTRL_OPCODE_LAST_GROUP         0x43
#define MCP_CTRL_OPCODE_GOTO_GROUP         0x44


#define MCP_CTRL_SUPPORTED_OPCODE_PLAY               0x00000001
#define MCP_CTRL_SUPPORTED_OPCODE_PAUSE              0x00000002
#define MCP_CTRL_SUPPORTED_OPCODE_FAST_REWIND        0x00000004
#define MCP_CTRL_SUPPORTED_OPCODE_FAST_FORWARD       0x00000008
#define MCP_CTRL_SUPPORTED_OPCODE_STOP               0x00000010
#define MCP_CTRL_SUPPORTED_OPCODE_MOVE_RELATIVE      0x00000020
#define MCP_CTRL_SUPPORTED_OPCODE_PREVIOUS_SEGMENT   0x00000040
#define MCP_CTRL_SUPPORTED_OPCODE_NEXT_SEGMENT       0x00000080
#define MCP_CTRL_SUPPORTED_OPCODE_FIRST_SEGMENT      0x00000100
#define MCP_CTRL_SUPPORTED_OPCODE_LAST_SEGMENT       0x00000200
#define MCP_CTRL_SUPPORTED_OPCODE_GOTO_SEGMENT       0x00000400
#define MCP_CTRL_SUPPORTED_OPCODE_PREVIOUS_TRACK     0x00000800
#define MCP_CTRL_SUPPORTED_OPCODE_NEXT_TRACK         0x00001000
#define MCP_CTRL_SUPPORTED_OPCODE_FIRST_TRACK        0x00002000
#define MCP_CTRL_SUPPORTED_OPCODE_LAST_TRACK         0x00004000
#define MCP_CTRL_SUPPORTED_OPCODE_GOTO_TRACK         0x00008000
#define MCP_CTRL_SUPPORTED_OPCODE_PRIVOUS_GROUP      0x00010000
#define MCP_CTRL_SUPPORTED_OPCODE_NEXT_GROUP         0x00020000
#define MCP_CTRL_SUPPORTED_OPCODE_FIRST_FROUP        0x00040000
#define MCP_CTRL_SUPPORTED_OPCODE_LAST_GROUP         0x00080000
#define MCP_CTRL_SUPPORTED_OPCODE_GOTO_GROUP         0x00100000

#define MCP_CTRL_SUPPORTED_OPCODE_DEFAULT     (\
	MCP_CTRL_SUPPORTED_OPCODE_PLAY |\
	MCP_CTRL_SUPPORTED_OPCODE_PAUSE |\
	MCP_CTRL_SUPPORTED_OPCODE_FAST_REWIND |\
	MCP_CTRL_SUPPORTED_OPCODE_FAST_FORWARD |\
	MCP_CTRL_SUPPORTED_OPCODE_STOP |\
	MCP_CTRL_SUPPORTED_OPCODE_MOVE_RELATIVE |\
	MCP_CTRL_SUPPORTED_OPCODE_PREVIOUS_SEGMENT |\
	MCP_CTRL_SUPPORTED_OPCODE_NEXT_SEGMENT |\
	MCP_CTRL_SUPPORTED_OPCODE_FIRST_SEGMENT |\
	MCP_CTRL_SUPPORTED_OPCODE_LAST_SEGMENT |\
	MCP_CTRL_SUPPORTED_OPCODE_GOTO_SEGMENT)


#define MCP_MEDIA_STATE_INACTIVE           0x00
#define MCP_MEDIA_STATE_PLAYING            0x01
#define MCP_MEDIA_STATE_PAUSED             0x02
#define MCP_MEDIA_STATE_SEEKING            0x03


///////////////////////////////////// ATT  HANDLER define ///////////////////////////////////////
typedef enum
{
	APP_ATT_H_START = 0,


	//// RAAS 1:13////
	/**********************************************************************************************/
	RAAS_PS_H,
	RAAS_SELECTABLE_ARES_CD_H, //M Mandatory:Read,Notify; Optional:
	RAAS_SELECTABLE_ARES_DP_H,
	RAAS_SELECTABLE_ARES_CCC_H,
	RAAS_CONFIGURED_ARES_CD_H, //M Mandatory:Read,Notify; Optional:
	RAAS_CONFIGURED_ARES_DP_H,
	RAAS_CONFIGURED_ARES_CCC_H,
	RAAS_CONFIGURED_CONTENT_CD_H, //M Mandatory:Read,Notify; Optional:
	RAAS_CONFIGURED_CONTENT_DP_H,
	RAAS_CONFIGURED_CONTENT_CCC_H,
	RAAS_MODIFY_ARES_CD_H, //M Mandatory:Write,Notify; Optional:
	RAAS_MODIFY_ARES_DP_H,
	RAAS_MODIFY_ARES_CCC_H,
	
	//// GMCS 14:27////
	/**********************************************************************************************/
	GMCS_PS_H, // 14
	GMCS_PLAYER_NAME_CD_H,  //M Mandatory:Read; Optional:Notify
	GMCS_PLAYER_NAME_DP_H,
	GMCS_PLAYER_NAME_CCC_H,
	GMCS_TRACK_CHANGED_CD_H, //M Mandatory:Notify; Optional:
	GMCS_TRACK_CHANGED_DP_H,
	GMCS_TRACK_CHANGED_CCC_H,
	GMCS_TRACK_TITLE_CD_H, //M Mandatory:Read; Optional:Notify
	GMCS_TRACK_TITLE_DP_H,
	GMCS_TRACK_TITLE_CCC_H,
	GMCS_TRACK_DURATION_CD_H, //M Mandatory:Read; Optional:Notify
	GMCS_TRACK_DURATION_DP_H,
	GMCS_TRACK_DURATION_CCC_H,
	GMCS_TRACK_POSITION_CD_H, //M Mandatory:Read,Write,Write Without Response; Optional:Notify
	GMCS_TRACK_POSITION_DP_H,
	GMCS_TRACK_POSITION_CCC_H,
	GMCS_MEDIA_STATE_CD_H, //M Mandatory:Read,Notify; Optional:
	GMCS_MEDIA_STATE_DP_H,
	GMCS_MEDIA_STATE_CCC_H,
	GMCS_CONTROL_POINT_CD_H, //O Mandatory:Read,Write,Write Without Response,Notify; Optional:
	GMCS_CONTROL_POINT_DP_H,
	GMCS_CONTROL_POINT_CCC_H,
	GMCS_SUPPORTED_OPCODE_CD_H, //O Mandatory:Read,Notify; Optional:
	GMCS_SUPPORTED_OPCODE_DP_H,
	GMCS_SUPPORTED_OPCODE_CCC_H,
	GMCS_CCID_CD_H, //O Mandatory:Read; Optional:
	GMCS_CCID_DP_H,
	
	
	//// GTBS 28:60////
	/**********************************************************************************************/
	GTBS_PS_H, // 28
	GTBS_PROVIDER_NAME_CD_H,       //M Mandatory:Read, Notify;   Optional:
	GTBS_PROVIDER_NAME_DP_H,
	GTBS_PROVIDER_NAME_CCC_H,
	GTBS_CALLER_IDENTIFIER_CD_H,   //M Mandatory:Read;   Optional:
	GTBS_CALLER_IDENTIFIER_DP_H,
	GTBS_TECHNOLOGY_CD_H,          //M Mandatory:Read, Notify;   Optional:
	GTBS_TECHNOLOGY_DP_H,
	GTBS_TECHNOLOGY_CCC_H,
	GTBS_URI_LIST_CD_H,            //M Mandatory:Read;   Optional:Notify
	GTBS_URI_LIST_DH_H,
	GTBS_CURRENT_CALL_LIST_CD_H,   //M Mandatory:Read, Notify;   Optional:
	GTBS_CURRENT_CALL_LIST_DP_H,
	GTBS_CURRENT_CALL_LIST_CCC_H,
	GTBS_CCID_CD_H,                //M Mandatory:Read;   Optional:
	GTBS_CCID_DP_H,
	GTBS_STATUS_FLAGS_CD_H,        //M Mandatory:Read, Notify;   Optional:
	GTBS_STATUS_FLAGS_DP_H,
	GTBS_STATUS_FLAGS_CCC_H,
	GTBS_CALL_STATE_CD_H,          //M Mandatory:Read, Notify;   Optional:
	GTBS_CALL_STATE_DP_H,
	GTBS_CALL_STATE_CCC_H,
	GTBS_CALL_CONTROL_POINT_CD_H,  //M Mandatory:Write, Write Without Response, Notify;   Optional:
	GTBS_CALL_CONTROL_POINT_DP_H,
	GTBS_CALL_CONTROL_POINT_CCC_H,
	GTBS_CCP_OPTIONAL_OPCODES_CD_H, //M Mandatory:Read;   Optional:
	GTBS_CCP_OPTIONAL_OPCODES_DP_H,
	GTBS_TERMINATIONO_REASON_CD_H,  //M Mandatory:Notify;   Optional:
	GTBS_TERMINATIONO_REASON_DP_H,
	GTBS_TERMINATIONO_REASON_CCC_H,
	GTBS_INCOMING_CALL_CD_H,        //M Mandatory:Read, Notify;   Optional:
	GTBS_INCOMING_CALL_DP_H,
	GTBS_INCOMING_CALL_CCC_H,
	
	APP_ATT_END_H,
}APP_ATT_HANDLE;


void my_gatt_init(void);
void app_audio_gatt_init(void);



//#define GTBS_UCI_
//skype	Skype	Microsoft	
//lync	Lync	Microsoft	
//ftime	Facetime	Apple	
//jabr	Jabber	Cisco	
//bbv	Blackberry Voice	Blackberry	
//bbm	Blackberry Messenger	Blackberry	
//eyebm	Eyebeam	Counterpath	
//wbrtc	Any WebRTC enabled browser	N/A	Firefox and Chrome Browsers
//qq	QQ	Tencent Technology	
//spika	Spika	Studio Djetelina d.o.o?	
//gtalk	Google Talk	Google	
//fbch	Facebook Chat	Facebook	
//tgrm	Telegram	Telegram Messenger LLP	
//lne	Line	Line Corporation	http://line.me/en/
//wtsap	WhatsApp	Facebook	
//vbr	Viber	Rakuten	
//mme	MessageMe	Yahoo	
//kkt	KakaoTalk	Daum Communications	
//wcht	WeChat	Tencent	
//tgo	Tango	TangoMe Inc.	
//kik	KIK	KIK Interactive	
//nbuz	Nimbuzz	Nimbuzz BV / Nimbuzz Internet India Pvt. Ltd)	
//hgus	Hangouts	Google	
//con	ChatOn	Samsung	
//mngr	Messanger	Facebook	
//chsec	ChatSecure	
//icht	iChat	Apple	
//rnds	Rounds	Rounds Entertainment Ltd.	
//imo	imo.im	PageBites, Inc.
//qik	Skye Qik	Microsoft	
//lbn	Libon	Orange Vall¨¦e	
//vonm	Vonage Mobile	Vonage	
//ov	ooVo	ooVoo LLC
//un000 to un999	Unknown	Unknown

#endif
#endif /* BLM_ATT_H_ */
