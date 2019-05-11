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

  ENSURE(wbuffer_init_extendable(&(response->wbuff)) != NULL);
  response->mutex = tk_mutex_create();
  ENSURE(response->mutex != NULL);

  return response;
}

ret_t http_response_set_status_code(http_response_t* response, uint32_t code) {
  return_value_if_fail(response != NULL, RET_BAD_PARAMS);

  return_value_if_fail(http_response_lock(response) == RET_OK, RET_FAIL);
  response->status_code = code;

  http_response_unlock(response);

  return RET_OK;
}

ret_t http_response_add_header(http_response_t* response, const char* key, const char* value) {
  return_value_if_fail(response != NULL && key != NULL && value != NULL, RET_BAD_PARAMS);

  return_value_if_fail(http_response_lock(response) == RET_OK, RET_FAIL);
  response->header = http_header_prepend(response->header, key, value);
  http_response_unlock(response);

  return RET_OK;
}

const char* http_response_find(http_response_t* response, const char* key) {
  return_value_if_fail(response != NULL && key != NULL, NULL);

  return http_header_find(response->header, key);
}

ret_t http_response_append_body_data(http_response_t* response, const void* data,
                                     uint32_t data_size) {
  ret_t ret = RET_OK;
  return_value_if_fail(response != NULL && data != NULL && data_size > 0, RET_BAD_PARAMS);

  return_value_if_fail(http_response_lock(response) == RET_OK, RET_FAIL);
  ret = wbuffer_write_binary(&(response->wbuff), data, data_size);
  response->body = response->wbuff.data;
  response->body_size = response->wbuff.cursor;
  *((char*)(response->body) + response->body_size) = '\0';
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

  wbuffer_deinit(&(response->wbuff));
  tk_mutex_destroy(response->mutex);
  http_header_destroy(response->header);

  memset(response, 0x00, sizeof(http_response_t));
  TKMEM_FREE(response);

  return RET_OK;
}

ret_t http_response_parse_line(http_response_t* response, const char* buffer) {
  return_value_if_fail(response != NULL && buffer != NULL, RET_BAD_PARAMS);

  char* key = NULL;
  char* value = NULL;
  tk_strncpy(response->line_buffer, buffer, sizeof(response->line_buffer) - 1);

  key = response->line_buffer;
  value = strchr(key, ':');

  if (strncmp(key, "HTTP/", 5) == 0) {
    char* p = strchr(key, ' ');
    if (p != NULL) {
      while (*p && *p == ' ') p++;
      http_response_set_status_code(response, tk_atoi(p));
    }
  } else if (value != NULL) {
    char* end = NULL;
    *value++ = '\0';
    while (*value && *value == ' ') value++;

    end = strchr(value, '\r');
    if (end == NULL) {
      end = strchr(value, '\n');
    }
    if (end != NULL) {
      *end = '\0';
    }

    response->header = http_header_prepend(response->header, key, value);
  }

  return RET_OK;
}
