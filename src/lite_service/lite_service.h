/**
 * File:   lite_service.h
 * Author: AWTK Develop Team
 * Brief:  light service interface
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
 * 2019-05-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LITE_SERVICE_H
#define TK_LITE_SERVICE_H

#include "tkc/mutex.h"
#include "tkc/thread.h"
#include "tkc/emitter.h"
#include "base/idle.h"
#include "lite_service/request_queue.h"

BEGIN_C_DECLS

struct _lite_service_t;
typedef struct _lite_service_t lite_service_t;

struct _lite_service_vtable_t;
typedef struct _lite_service_vtable_t lite_service_vtable_t;

typedef ret_t (*lite_service_idle_queue_t)(idle_func_t on_idle, void* ctx);

typedef ret_t (*lite_service_run_t)(lite_service_t* service);
typedef ret_t (*lite_service_on_destroy_t)(lite_service_t* service);
typedef ret_t (*lite_service_on_request_t)(lite_service_t* service, uint32_t cmd,
                                           uint32_t data_size, const void* data);

struct _lite_service_vtable_t {
  uint32_t size;
  const char* type;
  const char* desc;
  uint32_t queue_size;
  uint32_t max_payload_size;

  lite_service_run_t run;
  lite_service_on_request_t on_request;
  lite_service_on_destroy_t on_destroy;
};

/**
 * @class lite_service_t
 *
 * 服务基类。
 *
 */
struct _lite_service_t {
  request_queue_t* queue;
  const lite_service_vtable_t* vt;

  void* init_data;
  event_func_t on_event;
  void* on_event_ctx;

  tk_thread_t* thread;

  /*for test injection*/
  lite_service_idle_queue_t idle_queue;
};

/**
 * @method lite_service_create
 * 创建lite service对象。
 *
 * @param {lite_service_vtable_t*} vt lite_service虚表。
 * @param {void*} init_data 初始化数据。
 *
 * @return {lite_service_t*} 返回lite_service对象。
 */
lite_service_t* lite_service_create(const lite_service_vtable_t* vt, void* init_data);

/**
 * @method lite_service_run
 * 运行服务(由服务线程调用)。
 *
 * @param {lite_service_t*} service lite_service对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lite_service_run(lite_service_t* service);

/**
 * @method lite_service_set_on_event
 * 注册服务的事件。事件处理函数自动放到GUI线程执行。
 *
 * @param {lite_service_t*} service lite_service对象。
 * @param {event_func_t} on_event 事件处理函数。
 * @param {void*} ctx 事件处理函数的上下文。
 *
 * @return {uint32_t} 返回ID用于注销。
 */
uint32_t lite_service_set_on_event(lite_service_t* service, event_func_t on_event, void* ctx);

/**
 * @method lite_service_request
 * 请求服务。
 *
 * @param {lite_service_t*} service lite_service对象。
 * @param {uint32_t} cmd 命令ID。
 * @param {uint32_t} data_size 数据长度。
 * @param {const void*} data 数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lite_service_request(lite_service_t* service, uint32_t cmd, uint32_t data_size,
                           const void* data);

/**
 * @method lite_service_dispatch
 * 分发事件(放在UI的idle中执行)。
 *
 * @param {lite_service_t*} service lite_service对象。
 * @param {event_t*} event 事件。
 * @param {uint32_t} event_size 事件的大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lite_service_dispatch(lite_service_t* service, event_t* event, uint32_t event_size);

/**
 * @method lite_service_destroy
 * 销毁服务。
 *
 * @param {lite_service_t*} service lite_service对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t lite_service_destroy(lite_service_t* service);

/*for tester injection*/
ret_t lite_service_set_idle_queue(lite_service_t* service, lite_service_idle_queue_t idle_queue);

END_C_DECLS

#endif /*TK_LITE_SERVICE_H*/
