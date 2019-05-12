/**
 * File:   request_queue.h
 * Author: AWTK Develop Team
 * Brief:  service request queue
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

#ifndef TK_REQUEST_QUEUE_H
#define TK_REQUEST_QUEUE_H

#include "tkc/mutex.h"

BEGIN_C_DECLS

typedef ret_t (*request_queue_on_request_t)(void* ctx, uint32_t cmd, uint32_t payload_size,
                                            const void* payload);

/**
 * @class request_queue_t
 *
 * 请求队列。
 *
 */
typedef struct _request_queue_t {
  /*private*/
  uint8_t* buff;
  uint32_t size;
  uint32_t r;
  uint32_t w;
  bool_t full;

  tk_mutex_t* mutex;
  uint8_t* payload;
  uint32_t max_payload_size;
  void* on_request_ctx;
  request_queue_on_request_t on_request;
} request_queue_t;

/**
 * @method request_queue_create
 * 创建request queue对象。
 *
 * @param {uint32_t} size Buffer的大小。
 * @param {uint32_t} max_payload_size payload最大长度。
 * @param {request_queue_on_request_t} on_request 请求处理函数。
 * @param {void*} on_request_ctx 请求处理函数的上下文。
 *
 * @return {request_queue_t*} 返回request_queue对象。
 */
request_queue_t* request_queue_create(uint32_t size, uint32_t max_payload_size,
                                      request_queue_on_request_t on_request, void* on_request_ctx);

/**
 * @method request_queue_send
 * 发送一个请求。
 *
 * @param {request_queue_t*} q request_queue对象。
 * @param {uint32_t} cmd 命令ID。
 * @param {uint32_t} payload_size 数据长度。
 * @param {const void*} payload 数据(可以为NULL)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t request_queue_send(request_queue_t* q, uint32_t cmd, uint32_t payload_size,
                         const void* payload);

/**
 * @method request_queue_process
 * 处理队列中的请求(由服务线程调用)。
 *
 * @param {request_queue_t*} q request_queue对象。
 * @param {uint32_t} max_requests 最大处理请求的个数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t request_queue_process(request_queue_t* q, uint32_t max_requests);

/**
 * @method request_queue_destroy
 * 销毁队列。
 *
 * @param {request_queue_t*} q request_queue对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t request_queue_destroy(request_queue_t* q);

/*public for test*/
uint32_t request_queue_get_available_data(request_queue_t* q);
uint32_t request_queue_get_available_space(request_queue_t* q);
ret_t request_queue_read_data(request_queue_t* q, uint8_t* payload, uint32_t size);
ret_t request_queue_write_data(request_queue_t* q, const uint8_t* payload, uint32_t size);

END_C_DECLS

#endif /*TK_REQUEST_QUEUE_H*/
