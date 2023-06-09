/********************************************************************************************************
 * @file	app_config.h
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
#pragma once

#include "../feature_config.h"

#if (FEATURE_TEST_MODE == TEST_COC)




#define MASTER_MAX_NUM								2
#define SLAVE_MAX_NUM								1


///////////////////////// Feature Configuration////////////////////////////////////////////////
#define	FLASH_SIZE_CONFIG		   					FLASH_SIZE_1M  //very important, user need confirm !!!
#define BLE_SLAVE_SMP_ENABLE						1   //1 for smp,  0 no security
#define BLE_MASTER_SMP_ENABLE						1   //1 for smp,  0 no security
#define BLE_MASTER_SIMPLE_SDP_ENABLE				0   //simple service discovery for BLE master

///////////////////////// UI Configuration ////////////////////////////////////////////////////
#define UI_LED_ENABLE								1
#define	UI_KEYBOARD_ENABLE							1

///////////////////////// DEBUG  Configuration ////////////////////////////////////////////////
#define DEBUG_GPIO_ENABLE							0
#define UART_PRINT_DEBUG_ENABLE                     0  //printf
#define APP_DUMP_EN									0
#define MASTER_CONNECT_SLAVE_MAC_FILTER_EN			0

#define APP_DEFAULT_HID_BATTERY_OTA_ATTRIBUTE_TABLE			1

#define L2CAP_CREDIT_BASED_FLOW_CONTROL_MODE_EN     1 



#define LL_FEATURE_SUPPORT_CONNECTED_ISOCHRONOUS_STREAM_MASTER      0
#define LL_FEATURE_SUPPORT_CONNECTED_ISOCHRONOUS_STREAM_SLAVE		0
#define LL_FEATURE_SUPPORT_ISOCHRONOUS_BROADCASTER					0
#define LL_FEATURE_SUPPORT_SYNCHRONIZED_RECEIVER					0
#define LL_FEATURE_SUPPORT_ISOCHRONOUS_CHANNELS						0


#if (UI_KEYBOARD_ENABLE)   // if test pure power, kyeScan GPIO setting all disabled
		//---------------  KeyMatrix PC2/PC0/PC3/PC1 -----------------------------
		#define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
		#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K

		#define	KB_LINE_HIGH_VALID				0   //drive pin output 0 when keyscan, scanpin read 0 is valid

		#define BTN_PAIR						0x01
		#define BTN_UNPAIR						0x02

		#define	CR_VOL_UP						0xf0  ////
		#define	CR_VOL_DN						0xf1


		/**
		 *  @brief  Normal keyboard map
		 */
		#define	KB_MAP_NORMAL					{	{CR_VOL_DN,		BTN_PAIR},	 \
													{CR_VOL_UP,		BTN_UNPAIR}, }



		//////////////////// KEY CONFIG (EVK board) ///////////////////////////
		#define	KB_DRIVE_PINS 					{GPIO_PC2, GPIO_PC0}
		#define	KB_SCAN_PINS					{GPIO_PC3, GPIO_PC1}

		//drive pin as gpio
		#define	PC2_FUNC						AS_GPIO
		#define	PC0_FUNC						AS_GPIO

		//drive pin need 100K pulldown
		#define	PULL_WAKEUP_SRC_PC2				MATRIX_ROW_PULL
		#define	PULL_WAKEUP_SRC_PC0				MATRIX_ROW_PULL

		//drive pin open input to read gpio wakeup level
		#define PC2_INPUT_ENABLE				1
		#define PC0_INPUT_ENABLE				1

		//scan pin as gpio
		#define	PC3_FUNC						AS_GPIO
		#define	PC1_FUNC						AS_GPIO

		//scan  pin need 10K pullup
		#define	PULL_WAKEUP_SRC_PC3				MATRIX_COL_PULL
		#define	PULL_WAKEUP_SRC_PC1				MATRIX_COL_PULL

		//scan pin open input to read gpio level
		#define PC3_INPUT_ENABLE				1
		#define PC1_INPUT_ENABLE				1
#endif




#if UI_LED_ENABLE
	/**
	 *  @brief  Definition gpio for led
	 */
	#define GPIO_LED_BLUE        				GPIO_PB4
	#define GPIO_LED_GREEN      				GPIO_PB5
	#define GPIO_LED_WHITE       				GPIO_PB6
	#define GPIO_LED_RED         				GPIO_PB7

	#define LED_ON_LEVAL 						1 		//gpio output high voltage to turn on led

	#define PB4_FUNC							AS_GPIO
	#define PB5_FUNC							AS_GPIO
	#define PB6_FUNC							AS_GPIO
	#define PB7_FUNC							AS_GPIO

	#define	PB4_OUTPUT_ENABLE					1
	#define	PB5_OUTPUT_ENABLE					1
	#define PB6_OUTPUT_ENABLE					1
	#define	PB7_OUTPUT_ENABLE					1
#endif




/////////////////////////////////////// PRINT DEBUG INFO ///////////////////////////////////////
#if (UART_PRINT_DEBUG_ENABLE)
	#define PRINT_BAUD_RATE             		115200
	#define DEBUG_INFO_TX_PIN           		GPIO_PA0
	#define PULL_WAKEUP_SRC_PA0         		PM_PIN_PULLUP_10K
	#define PA0_OUTPUT_ENABLE         			1
	#define PA0_DATA_OUT                     	1 //must
#endif





#if(DEBUG_GPIO_ENABLE)
	#define GPIO_CHN0							GPIO_PE1
	#define GPIO_CHN1							GPIO_PE2
	#define GPIO_CHN2							GPIO_PA0
    #define GPIO_CHN3							GPIO_PA4
	#define GPIO_CHN4							GPIO_PA3
	#define GPIO_CHN5							GPIO_PB0
	#define GPIO_CHN6							GPIO_PB2
	#define GPIO_CHN7							GPIO_PE0

	#define GPIO_CHN8							GPIO_PA2
	#define GPIO_CHN9							GPIO_PA1
	#define GPIO_CHN10							GPIO_PB1
	#define GPIO_CHN11							GPIO_PB3
	#define GPIO_CHN12							GPIO_PC7
	#define GPIO_CHN13							GPIO_PC6
	#define GPIO_CHN14							GPIO_PC5
	#define GPIO_CHN15							GPIO_PC4


	#define PE1_OUTPUT_ENABLE					1
	#define PE2_OUTPUT_ENABLE					1
	#define PA0_OUTPUT_ENABLE					1
	#define PA4_OUTPUT_ENABLE					1
	#define PA3_OUTPUT_ENABLE					1
	#define PB0_OUTPUT_ENABLE					1
	#define PB2_OUTPUT_ENABLE					1
	#define PE0_OUTPUT_ENABLE					1

	#define PA2_OUTPUT_ENABLE					1
	#define PA1_OUTPUT_ENABLE					1
	#define PB1_OUTPUT_ENABLE					1
	#define PB3_OUTPUT_ENABLE					1
	#define PC7_OUTPUT_ENABLE					1
	#define PC6_OUTPUT_ENABLE					1
	#define PC5_OUTPUT_ENABLE					1
	#define PC4_OUTPUT_ENABLE					1
#endif  //end of DEBUG_GPIO_ENABLE




#include "../../common/default_config.h"

#endif

