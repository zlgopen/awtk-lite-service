/**
 * File:   http_request.h
 * Author: AWTK Develop Team
 * Brief:  http request
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
 * 2019-05-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_HTTP_REQUEST_H
#define TK_HTTP_REQUEST_H

#include "http/http_common.h"

BEGIN_C_DECLS

typedef ret_t (*http_request_on_event_t)(void* ctx, http_request_t* request, http_response_t* resp);

/**
 * @class http_request_t
 *
 * HTTP request
 *
 */
struct _http_request_t {
  /**
   * @property {char*} url
   * @annotation ["readable"]
   * url。
   */
  char* url;

  /**
   * @property {char*} method
   * @annotation ["readable"]
   * method。
   */
  const char* method;

  /**
   * @property {http_header_t*} header
   * @annotation ["readable"]
   * 额外的header信息。
   */
  http_header_t* header;

  /**
   * @property {void*} body
   * @annotation ["readable"]
   * PUT/POST请求上传的数据。
   */
  void* body;

  /**
   * @property {uint32_t} body_size
   * @annotation ["readable"]
   * PUT/POST请求上传的数据的长度。
   */
  uint32_t body_size;

  /**
   * @property {bool_t} abort
   * @annotation ["readable"]
   * 取消本次请求。
   */
  bool_t abort;

  /**
   * @property {http_response_t*} response
   * @annotation ["readable"]
   * 请求的结果。
   */
  http_response_t* response;

  /*private*/
  void* on_event_ctx;
  http_request_on_event_t on_event;
};

/**
 * @method http_request_create_get
 * 创建一个GET请求。
 *
 * 回调函数在GUI线程中执行，在回调函数中可以直接操作GUI的控件。
 *
 * @param {const char*} url url。
 * @param {http_request_on_event_t} on_event 事件回调函数。
 * @param {void*} ctx 事件回调函数的上下文。
 *
 * @return {http_request_t*} 返回request对象。
 */
http_request_t* http_request_create_get(const char* url, http_request_on_event_t on_event,
                                        void* ctx);

/**
 * @method http_request_create_delete
 * 创建一个DELETE请求。
 *
 * 回调函数在GUI线程中执行，在回调函数中可以直接操作GUI的控件。
 *
 * @param {const char*} url url。
 * @param {http_request_on_event_t} on_event 事件回调函数。
 * @param {void*} ctx 事件回调函数的上下文。
 *
 * @return {http_request_t*} 返回request对象。
 */
http_request_t* http_request_create_delete(const char* url, http_request_on_event_t on_event,
                                           void* ctx);

/**
 * @method http_request_create_put
 * 创建一个PUT请求。
 *
 * 回调函数在GUI线程中执行，在回调函数中可以直接操作GUI的控件。
 *
 * @param {const char*} url url。
 * @param {http_request_on_event_t} on_event 事件回调函数。
 * @param {void*} ctx 事件回调函数的上下文。
 * @param {const char*} content_type 上传内容的类型。
 * @param {const void*} data 要上传的数据。
 * @param {uint32_t} data_size 要上传的数据的长度。
 *
 * @return {http_request_t*} 返回request对象。
 */
http_request_t* http_request_create_put(const char* url, http_request_on_event_t on_event,
                                        void* ctx, const char* content_type, const void* data,
                                        uint32_t data_size);

/**
 * @method http_request_create_post
 * 创建一个POST请求。
 *
 * 回调函数在GUI线程中执行，在回调函数中可以直接操作GUI的控件。
 *
 * @param {const char*} url url。
 * @param {http_request_on_event_t} on_event 事件回调函数。
 * @param {void*} ctx 事件回调函数的上下文。
 * @param {const char*} content_type 上传内容的类型。
 * @param {const void*} data 要上传的数据。
 * @param {uint32_t} data_size 要上传的数据的长度。
 *
 * @return {http_request_t*} 返回request对象。
 */
http_request_t* http_request_create_post(const char* url, http_request_on_event_t on_event,
                                         void* ctx, const char* content_type, const void* data,
                                         uint32_t data_size);

/**
 * @method http_request_add_header
 * 增加一个header。
 *
 * @param {http_request_t*} request http request对象。
 * @param {const char*} key header的键名。
 * @param {const char*} value header的键值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t http_request_add_header(http_request_t* request, const char* key, const char* value);

/**
 * @method http_request_abort
 * 设置abort标志，通知服务取消本次请求。
 *
 * @param {http_request_t*} request http request对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t http_request_abort(http_request_t* request);

/**
 * @method http_request_destroy
 * 销毁request对象。
 *
 * @param {http_request_t*} request http request对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t http_request_destroy(http_request_t* request);

END_C_DECLS

#endif /*TK_HTTP_REQUEST_H*/
