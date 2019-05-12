/**
 * File:   service_thread.h
 * Author: AWTK Develop Team
 * Brief:  service thread
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ===============================================================
 * 2019-05-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SERVICE_THREAD_H
#define TK_SERVICE_THREAD_H

#include "tkc/thread.h"
#include "lite_service/lite_service.h"

BEGIN_C_DECLS

/**
 * @class service_thread_t
 * 服务线程。
 *
 * @annotation ["fake"]
 *
 */

/**
 * @method service_thread_start
 * 启动服务线程。
 *
 * @param {const lite_service_vtable_t*} vt lite_service vtable对象。
 * @param {void*} init_data 初始化数据。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} on_event_ctx 事件处理函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
tk_thread_t* service_thread_start(const lite_service_vtable_t* vt, void* init_data,
                                  event_func_t on_event, void* on_event_ctx);

/**
 * @method service_thread_request
 * 请求服务。
 *
 * @param {tk_thread_t*} service service thread对象。
 * @param {uint32_t} cmd 命令ID。
 * @param {uint32_t} data_size 数据长度。
 * @param {const void*} data 数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t service_thread_request(tk_thread_t* service, uint32_t cmd, uint32_t data_size,
                             const void* data);

/*for tester injection*/
ret_t service_thread_set_idle_queue(tk_thread_t* thread, lite_service_idle_queue_t idle_queue);

END_C_DECLS

#endif /*TK_SERVICE_THREAD_H*/
