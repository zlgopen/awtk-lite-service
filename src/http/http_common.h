/**
 * File:   http_common.h
 * Author: AWTK Develop Team
 * Brief:  http common
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

#ifndef TK_HTTP_COMMON_H
#define TK_HTTP_COMMON_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

struct _http_response_t;
typedef struct _http_response_t http_response_t;

struct _http_request_t;
typedef struct _http_request_t http_request_t;

struct _http_header_t;
typedef struct _http_header_t http_header_t;

/**
 * @enum http_method_t
 * @annotation ["string"]
 * @prefix HTTP_METHOD_
 * HTPP METHOD。
 */

/**
 * @const HTTP_METHOD_GET
 * GET请求。
 */
#define HTTP_METHOD_GET "GET"

/**
 * @const HTTP_METHOD_PUT
 * PUT请求。
 */
#define HTTP_METHOD_PUT "PUT"

/**
 * @const HTTP_METHOD_POST
 * POST请求。
 */
#define HTTP_METHOD_POST "POST"

/**
 * @const HTTP_METHOD_DELETE
 * DELETE请求。
 */
#define HTTP_METHOD_DELETE "DELETE"

typedef enum _http_event_t {
  HTTP_EVENT_START = 512,
  HTTP_EVENT_PROGRESS,
  HTTP_EVENT_ERROR,
  HTTP_EVENT_DONE
} http_event_t;

typedef enum _http_cmd_t {
  HTTP_CMD_STOP = 1,
} http_cmd_t;

END_C_DECLS

#endif /*TK_HTTP_COMMON_H*/
