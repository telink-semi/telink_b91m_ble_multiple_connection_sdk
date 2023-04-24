/********************************************************************************************************
 * @file	main.c
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
#include "app.h"

#if(FREERTOS_ENABLE)
#include <FreeRTOS.h>
#include <task.h>
#endif

#if (FREERTOS_ENABLE)
static void led_task(void *pvParameters){
	gpio_function_en(GPIO_PB4);			// PD3 EAGLE WEARABLE BOARD; PB4 EAGLE EVK.
	reg_gpio_pb_oen &= ~ GPIO_PB4;
	while(1){
		reg_gpio_pb_out |= GPIO_PB4;
		vTaskDelay(1000);
		//printf("%s\r\n", __FUNCTION__);
		reg_gpio_pb_out &= ~GPIO_PB4;
		vTaskDelay(1000);
	}
}

static void led_task1(void *pvParameters){
	gpio_function_en(GPIO_PB5);			// PD4 EAGLE WEARABLE BOARD; PB5 EAGLE EVK.
	reg_gpio_pb_oen &= ~ GPIO_PB5;
	while(1){
		reg_gpio_pb_out |= GPIO_PB5;
		vTaskDelay(500);
		//printf("%s\r\n", __FUNCTION__);
		reg_gpio_pb_out &= ~GPIO_PB5;
		vTaskDelay(500);
	}
}

static TaskHandle_t hProtoTask;
static void proto_task( void *pvParameters ){
	while(1){
		main_loop();
	}
}
#endif

/**
 * @brief		BLE RF interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void rf_irq_handler(void)
{
//	DBG_CHN14_HIGH;

	blc_sdk_irq_handler ();

//	DBG_CHN14_LOW;
}

/**
 * @brief		System timer interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void stimer_irq_handler(void)
{
	DBG_CHN15_HIGH;

	blc_sdk_irq_handler ();

	DBG_CHN15_LOW;
}

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ int main(void)
{
	#if (BLE_APP_PM_ENABLE)
		blc_pm_select_internal_32k_crystal();
	#endif

	sys_init(DCDC_1P4_DCDC_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);

	/* detect if MCU is wake_up from deep retention mode */
	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp

	CCLK_32M_HCLK_32M_PCLK_16M;

	rf_drv_ble_init();

	gpio_init(!deepRetWakeUp);

	/* load customized freq_offset cap value. */
	blc_app_loadCustomizedParameters();  

	if( deepRetWakeUp ){ //MCU wake_up from deepSleep retention mode
		user_init_deepRetn ();
	}
	else{ //MCU power_on or wake_up from deepSleep mode
		user_init_normal();
	}


	irq_enable();

#if (FREERTOS_ENABLE)			//   (0 && FREERTOS_ENABLE)
	extern void vPortRestoreTask();
	// 线程间的通讯队列会丢失(除非强制队列为空的时候才可以retention ?)。那么其他系统组件呢? sem, notification ?
	// malloc的数据会完全丢失
	// 任务初始化，会再次初始化造成状态不一致
	// 全局变量虽然可以特意用retention，但是因为程序重新跑了，变量状态不一定正确。
	// 以上这些都需要客户自己编写的任务，格外的小心而且要非常专业。挂一漏万都会造成奔溃
	if(0){	//  Tasks do not support deep retention, due to RAM limitation
		vPortRestoreTask();
	}else{
//		extern void bt_ll_proto_task_sleep(unsigned int wakeup_tick);
//		bt_ll_register_sleep_func(bt_ll_proto_task_sleep);
		xTaskCreate( led_task, "tLed", configMINIMAL_STACK_SIZE, (void*)0, (tskIDLE_PRIORITY+1), 0 );
		xTaskCreate( led_task1,"tLed1",configMINIMAL_STACK_SIZE, (void*)0, (tskIDLE_PRIORITY+1), 0 );
		xTaskCreate( proto_task, "tProto", configMINIMAL_STACK_SIZE*12, (void*)0, (tskIDLE_PRIORITY+1), &hProtoTask );
		vTaskStartScheduler();
	}
#else

	while(1)
	{
		main_loop ();
	}
#endif
	return 0;
}
