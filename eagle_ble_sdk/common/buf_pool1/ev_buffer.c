/********************************************************************************************************
 * @file	ev_buffer.c
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
#include "ev_buffer.h"
#include "user_config.h"
#include "mempool.h"
#include "common/utility.h"
#include "common/assert.h"
#include <string.h>
#include "drivers/B91/ext_driver/ext_misc.h"

#ifdef WIN32
#include <malloc.h>
#endif


#if (1)

#define DEFAULT_BUFFER_GROUP_NUM                 3


/**************************** Private Variable Definitions *******************/

typedef struct {
	mem_pool_t *qHead;
	u16 size;
	u8 availBufNum;
	u8 reserved;
} ev_buf_groups_t;


typedef struct bufm_vars {
    ev_buf_groups_t bufGroups[DEFAULT_BUFFER_GROUP_NUM];
} ev_buf_vars_t;


ev_buf_vars_t ev_buf_vs;
ev_buf_vars_t *ev_buf_v = &ev_buf_vs;


MEMPOOL_DECLARE(size_0_pool, size_0_mem, BUFFER_GROUP_0, BUFFER_NUM_IN_GROUP0);
MEMPOOL_DECLARE(size_1_pool, size_1_mem, BUFFER_GROUP_1, BUFFER_NUM_IN_GROUP1);
MEMPOOL_DECLARE(size_2_pool, size_2_mem, BUFFER_GROUP_2, BUFFER_NUM_IN_GROUP2);

/*********************************************************************
 * @fn      ev_buf_isExisted
 *
 * @brief   Return whether the buffer is in the available buffer 
 *
 * @param   index
 * @param   block
 *
 * @return  TRUE or FALSE
 */
u8 ev_buf_isExisted(u8 index, mem_block_t *block)
{
    mem_pool_t *pool = (mem_pool_t *)ev_buf_v->bufGroups[index].qHead;
    mem_block_t *curBlock = pool->free_list;

    while(curBlock){
        if(block == curBlock){
            return TRUE;
        } 
        curBlock = curBlock->next_block;
    }

    return FALSE;
}

u8 *ev_buf_retriveMempoolHeader(u8 *pd)
{
    return pd - (OFFSETOF(ev_bufItem_t, data) - OFFSETOF(mem_block_t, data));
}


/*********************************************************************
 * @fn      ev_buf_reset
 *
 * @brief   Reset the EV Buffer module
 *
 * @param   None
 *
 * @return  None
 */
void ev_buf_reset(void)
{
    u16 size[DEFAULT_BUFFER_GROUP_NUM] = {BUFFER_GROUP_0, BUFFER_GROUP_1, BUFFER_GROUP_2};
    mem_pool_t *memPool[DEFAULT_BUFFER_GROUP_NUM] = {&size_0_pool, &size_1_pool, &size_2_pool};
    u8 *mem[DEFAULT_BUFFER_GROUP_NUM] = {size_0_mem, size_1_mem, size_2_mem};
    u8 buffCnt[DEFAULT_BUFFER_GROUP_NUM] = {BUFFER_NUM_IN_GROUP0, BUFFER_NUM_IN_GROUP1, BUFFER_NUM_IN_GROUP2};

    memset((u8 *)ev_buf_v, 0, sizeof(ev_buf_vars_t));

    /* reinitialize available buffer */
    for(u8 i = 0; i < DEFAULT_BUFFER_GROUP_NUM; i++){
        ev_buf_v->bufGroups[i].availBufNum = buffCnt[i];
        ev_buf_v->bufGroups[i].qHead = mempool_init(memPool[i], mem[i], size[i], buffCnt[i]);
        ev_buf_v->bufGroups[i].size = size[i];
    }  
}

/*********************************************************************
 * @fn      ev_buf_init
 *
 * @brief   Initialize the EV Buffer module
 *
 * @param   None
 *
 * @return  None
 */
void ev_buf_init(void)
{
    ev_buf_reset();
}

/*********************************************************************
 * @fn      ev_buf_allocate
 *
 * @brief   Allocate an available buffer according to the requested size
 *          The allocated buffer will have only three kind of size, defined 
 *          in @ref EV_BUFFER_CONSTANT 
 *
 * @param   size - requested size
 *
 * @return  Pointer to an allocated buffer.
 *          NULL means the there is no available buffer.
 */

#if EV_BUFFER_DEBUG
u8 *my_ev_buf_allocate(u16 size, u16 line)
#else
u8 *ev_buf_allocate(u16 size)
#endif
{
    if((size == 0) || (size > MAX_BUFFER_SIZE)){
        /* the size parameter is wrong */
        return NULL;
    }
    u32 r = irq_disable();
    u8 index = U8_MAX;

    /* find related the buffer blocks */
    for(u8 i = 0; i < DEFAULT_BUFFER_GROUP_NUM; i++){
        if((size <= ev_buf_v->bufGroups[i].size - OFFSETOF(ev_bufItem_t, data)) && ev_buf_v->bufGroups[i].availBufNum){
            index = i;
            break;
        }
    }
    if((index == U8_MAX ) || (!ev_buf_v->bufGroups[index].availBufNum)){
        /* no available buffer */
    	irq_restore(r);
        return NULL;
    }
    u8 *temp = (u8 *)mempool_alloc(ev_buf_v->bufGroups[index].qHead);
    if(!temp){
    	irq_restore(r);
    	return NULL;
    }
    ev_buf_v->bufGroups[index].availBufNum--;

    ev_bufItem_t *pNewBuf = (ev_bufItem_t *)(temp - 4);
    pNewBuf->groupIndex = index;
#if EV_BUFFER_DEBUG
    pNewBuf->line = line;
    pNewBuf->flag = 0xfe;
#endif
    irq_restore(r);
    return pNewBuf->data;
}


/*********************************************************************
 * @fn      ev_buf_free
 *
 * @brief   Free the specified buffer 
 *
 * @param   pBuf - the pointer to the specified buffer to free.
 *
 * @return  status
 */
#if EV_BUFFER_DEBUG
volatile u32 T_DBG_evFreeBuf = 0;
volatile u16 T_DBG_evFreeBufLine = 0;
buf_sts_t my_ev_buf_free(u8 *pBuf, u16 line)
#else
buf_sts_t ev_buf_free(u8 *pBuf)
#endif
{
    u32 r = irq_disable();

    if(!is_ev_buf(pBuf)){
#if EV_BUFFER_DEBUG
    	T_DBG_evFreeBuf = (u32)pBuf;
    	T_DBG_evFreeBufLine = line;
#endif

    	//TODO: Throw exceptions to the application layer
    }

    ev_bufItem_t *pDelBuf = ev_buf_getHead(pBuf);

    /* check whether the buffer is duplicated release */
    if(ev_buf_isExisted(pDelBuf->groupIndex, (mem_block_t *)pDelBuf)){

#if EV_BUFFER_DEBUG
    	T_DBG_evFreeBuf = (u32)pBuf;
    	T_DBG_evFreeBufLine = line;
#endif

    	//TODO: Throw exceptions to the application layer

        irq_restore(r);
        return BUFFER_DUPLICATE_FREE;
    }

    mempool_free(ev_buf_v->bufGroups[pDelBuf->groupIndex].qHead, ev_buf_retriveMempoolHeader(pBuf));
    ev_buf_v->bufGroups[pDelBuf->groupIndex].availBufNum++;    

#if EV_BUFFER_DEBUG
    pDelBuf->line = line;
    pDelBuf->flag = 0xff;
#endif

    irq_restore(r);
    return BUFFER_SUCC;
}



/*********************************************************************
 * @fn      ev_buf_getHead
 *
 * @brief   Get the header pointer of a buffer item 
 *
 * @param   pd - the pointer of a data, which is previously allocated
 *
 * @return  Pointer of bufferItem
 */
ev_bufItem_t *ev_buf_getHead(u8 *pd)
{
    return (ev_bufItem_t *)(pd - OFFSETOF(ev_bufItem_t, data));
}

/*********************************************************************
 * @fn      ev_buf_getTail
 *
 * @brief   Get the pointer from a EV BUFFER tail.
 *
 * @param   pd - the pointer of a data, which is previously allocated
 * @param   offsetToTail - The offset to Tail
 *
 * @return  Pointer of the specified memory
 */
u8 *ev_buf_getTail(u8 *pd, int offsetToTail)
{
	u32 index;
	u16 size[DEFAULT_BUFFER_GROUP_NUM] = {BUFFER_GROUP_0, BUFFER_GROUP_1, BUFFER_GROUP_2};

	memcpy((u8*)&index, pd - 4, 4);
	assert((index < 3) && (index >= 0));
	return (u8*)(pd - 8 + size[index] - offsetToTail);
}


u8 is_ev_buf(void *arg){
	 if( ((u32)arg >= (u32)(size_0_mem) && (u32)arg <= ((u32)(size_0_mem) + sizeof(size_0_mem))) ||
		  ((u32)arg >= (u32)(size_1_mem) && (u32)arg <= ((u32)(size_1_mem) + sizeof(size_1_mem))) ||
		  ((u32)arg >= (u32)(size_2_mem) && (u32)arg <= ((u32)(size_2_mem) + sizeof(size_2_mem))) ){
		 return 1;
	 }
	 return 0;
}

u16 ev_buf_getFreeMaxSize(void)
{
	u16 size = 0;

	for(u8 i = 0; i < DEFAULT_BUFFER_GROUP_NUM; i++){
		if(ev_buf_v->bufGroups[i].availBufNum){
			if((ev_buf_v->bufGroups[i].size - OFFSETOF(ev_bufItem_t, data)) > size){
				size = ev_buf_v->bufGroups[i].size - OFFSETOF(ev_bufItem_t, data);
			}
		}
	}

	return size;
}

#endif  /* MODULE_BUFM_ENABLE */






