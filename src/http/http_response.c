/**
 * File:   http_response.c
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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "http/http_header.h"
#include "http/http_response.h"

http_response_t* http_response_create(void) {
  http_response_t* response = TKMEM_ZALLOC(http_response_t);
  return_value_if_fail(response != NULL, NULL);

  response->mutex = tk_mutex_create();
  ENSURE(response->mutex != NULL);

  return response;
}

ret_t http_response_set_status(http_response_t* response, uint32_t code, const char* text) {
  return_value_if_fail(response != NULL && text != NULL && response->status_text == NULL, RET_BAD_PARAMS);

  return_value_if_fail(http_response_lock(response) == RET_OK, RET_FAIL);
  response->status_code = code;
  response->status_text = tk_strdup(text);
  http_response_unlock(response);

  return RET_OK;
}

ret_t http_response_add_header(http_response_t* response, const char* key, const char* value) {
  return_value_if_fail(response != NULL && key != NULL  && value != NULL, RET_BAD_PARAMS);

  return_value_if_fail(http_response_lock(response) == RET_OK, RET_FAIL);
  response->header = http_header_prepend(response->header, key, value);
  http_response_unlock(response);

  return RET_OK;
}

const char* http_response_find(http_response_t* response, const char* key) {
  return_value_if_fail(response != NULL && key != NULL, NULL);

  return http_header_find(response->header, key);
}

ret_t http_response_set_body(http_response_t* response, void* body, uint32_t body_size) {
  ret_t ret = RET_OK;
  return_value_if_fail(response != NULL && body != NULL && body_size > 0, RET_BAD_PARAMS);
  return_value_if_fail(http_response_lock(response) == RET_OK, RET_FAIL);

  response->body = TKMEM_ALLOC(body_size + 1);
  if(response->body != NULL) {
    response->body_size = body_size;
    memcpy(response->body, body, body_size);
    ((char*)(response->body))[body_size] = '\0';
  } else {
    ret = RET_OOM;
  }

  http_response_unlock(response);

  return ret;
}

ret_t http_response_set_done(http_response_t* response, bool_t done) {
  return_value_if_fail(response != NULL, RET_BAD_PARAMS);

  return_value_if_fail(http_response_lock(response) == RET_OK, RET_FAIL);
  response->done = done;
  http_response_unlock(response);

  return RET_OK;
}


ret_t http_response_set_fail(http_response_t* response, bool_t fail) {
  return_value_if_fail(response != NULL, RET_BAD_PARAMS);

  return_value_if_fail(http_response_lock(response) == RET_OK, RET_FAIL);
  response->fail = fail;
  http_response_unlock(response);

  return RET_OK;
}

ret_t http_response_set_uploaded_size(http_response_t* response, uint32_t uploaded_size) {
  return_value_if_fail(response != NULL, RET_BAD_PARAMS);

  return_value_if_fail(http_response_lock(response) == RET_OK, RET_FAIL);
  response->uploaded_size = uploaded_size;
  http_response_unlock(response);

  return RET_OK;
}

ret_t http_response_set_downloaded_size(http_response_t* response, uint32_t downloaded_size) {
  return_value_if_fail(response != NULL, RET_BAD_PARAMS);

  return_value_if_fail(http_response_lock(response) == RET_OK, RET_FAIL);
  response->downloaded_size = downloaded_size;
  http_response_unlock(response);

  return RET_OK;
}

ret_t http_response_lock(http_response_t* response) {
  return_value_if_fail(response != NULL, RET_BAD_PARAMS);

  return tk_mutex_lock(response->mutex);
}

ret_t http_response_unlock(http_response_t* response) {
  return_value_if_fail(response != NULL, RET_BAD_PARAMS);

  return tk_mutex_unlock(response->mutex);
}

ret_t http_response_destroy(http_response_t* response) {
  return_value_if_fail(response != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(response->body);
  tk_mutex_destroy(response->mutex);
  TKMEM_FREE(response->status_text);
  http_header_destroy(response->header);
  memset(response, 0x00, sizeof(http_response_t));

  TKMEM_FREE(response);

  return RET_OK;
}

