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
 * 2019-05-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "http/http_header.h"
#include "http/http_request.h"
#include "http/http_response.h"

static http_request_t* http_request_create(const char* url, const char* method,
                                           http_request_on_event_t on_event, void* ctx,
                                           const char* content_type, const void* data,
                                           uint32_t data_size) {
  uint32_t url_size = strlen(url) + 1;
  uint32_t size = sizeof(http_request_t) + url_size + data_size;
  http_request_t* request = (http_request_t*)TKMEM_ALLOC(size);

  return_value_if_fail(request != NULL, NULL);

  memset(request, 0x00, sizeof(http_request_t));

  request->method = method;
  request->url = (char*)request + sizeof(http_request_t);

  request->on_event_ctx = ctx;
  request->on_event = on_event;
  memcpy(request->url, url, url_size);

  if (data != NULL) {
    request->body_size = data_size;
    request->body = request->url + url_size;
    memcpy(request->body, data, data_size);
  }

  if (content_type != NULL) {
    request->header = http_header_create("Content-Type", content_type);
  }

  request->response = http_response_create();

  return request;
}

http_request_t* http_request_create_get(const char* url, http_request_on_event_t on_event,
                                        void* ctx) {
  return_value_if_fail(url != NULL && on_event != NULL, NULL);

  return http_request_create(url, HTTP_METHOD_GET, on_event, ctx, NULL, NULL, 0);
}

http_request_t* http_request_create_delete(const char* url, http_request_on_event_t on_event,
                                           void* ctx) {
  return_value_if_fail(url != NULL && on_event != NULL, NULL);

  return http_request_create(url, HTTP_METHOD_DELETE, on_event, ctx, NULL, NULL, 0);
}

http_request_t* http_request_create_put(const char* url, http_request_on_event_t on_event,
                                        void* ctx, const char* content_type, const void* data,
                                        uint32_t data_size) {
  return_value_if_fail(url != NULL && on_event != NULL && content_type != NULL && data != NULL,
                       NULL);

  return http_request_create(url, HTTP_METHOD_PUT, on_event, ctx, content_type, data, data_size);
}

http_request_t* http_request_create_post(const char* url, http_request_on_event_t on_event,
                                         void* ctx, const char* content_type, const void* data,
                                         uint32_t data_size) {
  return_value_if_fail(url != NULL && on_event != NULL && content_type != NULL && data != NULL,
                       NULL);

  return http_request_create(url, HTTP_METHOD_POST, on_event, ctx, content_type, data, data_size);
}

ret_t http_request_add_header(http_request_t* request, const char* key, const char* value) {
  return_value_if_fail(request != NULL && key != NULL && value != NULL, RET_BAD_PARAMS);

  request->header = http_header_prepend(request->header, key, value);

  return RET_OK;
}

ret_t http_request_abort(http_request_t* request) {
  return_value_if_fail(request != NULL && request->url != NULL, RET_BAD_PARAMS);

  request->abort = TRUE;

  return RET_OK;
}

ret_t http_request_destroy(http_request_t* request) {
  return_value_if_fail(request != NULL && request->url != NULL, RET_BAD_PARAMS);

  http_header_destroy(request->header);
  http_response_destroy(request->response);
  memset(request, 0x00, sizeof(http_request_t));

  TKMEM_FREE(request);

  return RET_OK;
}
