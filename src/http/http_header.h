/**
 * File:   http_header.h
 * Author: AWTK Develop Team
 * Brief:  http header
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

#ifndef TK_HTTP_HEADER_H
#define TK_HTTP_HEADER_H

#include "http/http_common.h"

BEGIN_C_DECLS

/**
 * @class http_header_t
 *
 * HTTP request/response中header的key/value对象。
 *
 */
struct _http_header_t {
  /**
   * @property {char*} key
   * @annotation ["readable"]
   * 键名。
   */
  char* key;

  /**
   * @property {char*} value
   * @annotation ["readable"]
   * 键值。
   */
  char* value;

  /**
   * @property {http_header_t*} next
   * @annotation ["readable"]
   * 下一个节点。
   */
  http_header_t* next;
};

/**
 * @method http_header_create
 * 创建一个key/value对象。
 *
 * @param {const char*} key 键名。
 * @param {const char*} value 键值。
 *
 * @return {http_header_t} 返回header对象。
 */
http_header_t* http_header_create(const char* key, const char* value);

/**
 * @method http_header_prepend
 * 创建一个key/value对象，并把next指向header。
 *
 * @param {const char*} key 键名。
 * @param {const char*} value 键值。
 *
 * @return {http_header_t} 返回header对象。
 */
http_header_t* http_header_prepend(http_header_t* header, const char* key, const char* value);

/**
 * @method http_header_find
 * 查找指定的key，如果找到返回它的value。
 *
 * @param {http_header_t*} header header对象。
 * @param {const char*} key header的key。
 *
 * @return {const char*} 如果找到，返回指定key的value，否则返回NULL。
 */
const char* http_header_find(http_header_t* header, const char* key);

/**
 * @method http_header_destroy
 * 销毁header(包括后继节点)。
 *
 * @param {http_header_t*} header header对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t http_header_destroy(http_header_t* header);

END_C_DECLS

#endif /*TK_HTTP_HEADER_H*/
