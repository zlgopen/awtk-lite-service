/**
 * File:   http_response.h
 * Author: AWTK Develop Team
 * Brief:  http response
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
 * 2019-05-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_HTTP_RESPONSE_H
#define TK_HTTP_RESPONSE_H

#include "tkc/mutex.h"
#include "tkc/buffer.h"
#include "http/http_common.h"

BEGIN_C_DECLS

/**
 * @class http_response_t
 *
 * HTTP response
 *
 * 在request->on_event回调函数中，可以直接访问本类的成员。
 *
 */
struct _http_response_t {
  /**
   * @property {uint32_t} status_code
   * @annotation ["readable"]
   * HTTP response 的status code。
   */
  uint32_t status_code;

  /**
   * @property {http_header_t*} header
   * @annotation ["readable"]
   * header信息。
   */
  http_header_t* header;

  /**
   * @property {void*} body
   * @annotation ["readable"]
   * 下载的数据。
   */
  void* body;

  /**
   * @property {uint32_t} body_size
   * @annotation ["readable"]
   * 下载的数据的长度(已经下载)。
   */
  uint32_t body_size;

  /**
   * @property {uint32_t} uploaded_size
   * @annotation ["readable"]
   * PUT/POST请求，已经上传的数据的长度。
   */
  uint32_t uploaded_size;

  /**
   * @property {bool_t} done;
   * @annotation ["readable"]
   * 整个请求完成。
   */
  bool_t done;

  /**
   * @property {bool_t} fail;
   * @annotation ["readable"]
   * 请求失败。
   */
  bool_t fail;

  /*private*/
  wbuffer_t wbuff;
  tk_mutex_t* mutex;
  char line_buffer[2048];
};

/**
 * @method http_response_create
 * 创建一个response对象。
 *
 * @return {http_response_t*} 返回response对象。
 */
http_response_t* http_response_create(void);

/**
 * @method http_response_set_status_code
 * 设置status。
 *
 * @param {http_response_t*} response http response对象。
 * @param {uint32_t} code 响应码。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t http_response_set_status_code(http_response_t* response, uint32_t code);

/**
 * @method http_response_add_header
 * 增加一个header。
 *
 * @param {http_response_t*} response http response对象。
 * @param {const char*} key header的键名。
 * @param {const char*} value header的键值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t http_response_add_header(http_response_t* response, const char* key, const char* value);

/**
 * @method http_response_find_header
 * 查找指定的key，如果找到返回它的value。
 *
 * @param {http_response_t*} response response对象。
 * @param {const char*} key header的key。
 *
 * @return {const char*} 如果找到，返回指定key的value，否则返回NULL。
 */
const char* http_response_find(http_response_t* response, const char* key);

/**
 * @method http_response_append_body_data
 * 追加下载的数据。
 *
 * @param {http_response_t*} response http response对象。
 * @param {const void*} body 数据。
 * @param {uint32_t} data_size 数据的长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t http_response_append_body_data(http_response_t* response, const void* data,
                                     uint32_t data_size);

/**
 * @method http_response_set_done
 * 设置完成标志。
 *
 * @param {http_response_t*} response http response对象。
 * @param {bool_t} done 是否完成。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t http_response_set_done(http_response_t* response, bool_t done);

/**
 * @method http_response_set_fail
 * 设置失败标志。
 *
 * @param {http_response_t*} response http response对象。
 * @param {bool_t} fail 是否失败。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t http_response_set_fail(http_response_t* response, bool_t fail);

/**
 * @method http_response_set_uploaded_size
 * 更新已经上传数据的大小。
 *
 * @param {http_response_t*} response http response对象。
 * @param {uint32_t} uploaded_size 已经上传数据的大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t http_response_set_uploaded_size(http_response_t* response, uint32_t uploaded_size);

/**
 * @method http_response_parse_line
 * 解析一行数据。
 *
 * @param {http_response_t*} response http response对象。
 * @param {const char*} buffer 一行数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t http_response_parse_line(http_response_t* response, const char* buffer);

/**
 * @method http_response_lock
 * 锁定response对象。
 *
 * 在request的on_event回调函数访问response的属性无需锁定。
 *
 * @param {http_response_t*} response http response对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t http_response_lock(http_response_t* response);

/**
 * @method http_response_unlock
 * 解锁response对象。
 *
 * 在request的on_event回调函数访问response的属性无需锁定。
 *
 * @param {http_response_t*} response http response对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t http_response_unlock(http_response_t* response);

/**
 * @method http_response_destroy
 * 销毁response对象。
 *
 * @param {http_response_t*} response http response对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t http_response_destroy(http_response_t* response);

END_C_DECLS

#endif /*TK_HTTP_RESPONSE_H*/
