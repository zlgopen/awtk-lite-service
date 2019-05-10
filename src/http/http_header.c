/**
 * File:   http_header.c
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
 * 2019-05-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "http/http_header.h"

http_header_t* http_header_create(const char* key, const char* value) {
  uint32_t size = 0;
  uint32_t key_size = 0;
  uint32_t value_size = 0;
  http_header_t* header = NULL;

  return_value_if_fail(key != NULL && value != NULL, NULL);

  key_size = strlen(key) + 1;
  value_size = strlen(value) + 1;
  size = sizeof(http_header_t) + key_size + value_size;

  header = (http_header_t*)TKMEM_ALLOC(size);
  return_value_if_fail(header != NULL, NULL);

  header->next = NULL;
  header->key = ((char*)header) + sizeof(http_header_t);
  header->value = header->key + key_size;

  memcpy(header->key, key, key_size);
  memcpy(header->value, value, value_size);

  return header;
}

http_header_t* http_header_prepend(http_header_t* header, const char* key, const char* value) {
  http_header_t* prev = http_header_create(key, value);
  return_value_if_fail(prev != NULL, header);

  prev->next = header;

  return prev;
}

const char* http_header_find(http_header_t* header, const char* key) {
  http_header_t* iter = header;
  return_value_if_fail(key != NULL, NULL);

  while (iter != NULL) {
    if (tk_str_eq(iter->key, key)) {
      return iter->value;
    }

    iter = iter->next;
  }

  return NULL;
}

ret_t http_header_destroy(http_header_t* header) {
  http_header_t* iter = header;

  while (iter != NULL) {
    http_header_t* next = iter->next;
    TKMEM_FREE(next);
    iter = next;
  }

  return RET_OK;
}
