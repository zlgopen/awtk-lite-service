/**
 * File:   request_queue.c
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

#include "tkc/mem.h"
#include "tkc/mutex.h"
#include "lite_service/request_queue.h"

request_queue_t* request_queue_create(uint32_t size, uint32_t max_payload_size,
                                      request_queue_on_request_t on_request, void* on_request_ctx) {
  request_queue_t* q = NULL;
  return_value_if_fail(size >= 128 && max_payload_size >= 8 && on_request != NULL, NULL);

  q = TKMEM_ZALLOC(request_queue_t);
  goto_error_if_fail(q != NULL);

  q->buff = TKMEM_ALLOC(size);
  goto_error_if_fail(q->buff != NULL);

  q->mutex = tk_mutex_create();
  goto_error_if_fail(q->mutex != NULL);

  q->payload = TKMEM_ALLOC(max_payload_size);
  goto_error_if_fail(q->payload != NULL);

  q->size = size;
  q->full = FALSE;
  q->on_request = on_request;
  q->on_request_ctx = on_request_ctx;
  q->max_payload_size = max_payload_size;

  return q;

error:
  if (q != NULL) {
    if (q->buff != NULL) {
      TKMEM_FREE(q->buff);
    }

    if (q->mutex != NULL) {
      tk_mutex_destroy(q->mutex);
    }

    if (q->payload != NULL) {
      TKMEM_FREE(q->payload);
    }
  }

  return NULL;
}

uint32_t request_queue_get_available_space(request_queue_t* q) {
  uint32_t available = 0;
  return_value_if_fail(q != NULL && q->buff != NULL, 0);

  if (q->full) {
    available = 0;
  } else if (q->w < q->r) {
    available = q->r - q->w;
  } else {
    available = q->r + (q->size - q->w);
  }

  return available;
}

uint32_t request_queue_get_available_data(request_queue_t* q) {
  uint32_t available = 0;
  return_value_if_fail(q != NULL && q->buff != NULL, 0);

  if (q->full) {
    available = q->size;
  } else if (q->r <= q->w) {
    available = q->w - q->r;
  } else {
    available = q->w + (q->size - q->r);
  }

  return available;
}

ret_t request_queue_write_data(request_queue_t* q, const uint8_t* payload, uint32_t size) {
  uint32_t i = 0;
  return_value_if_fail(q != NULL && size <= request_queue_get_available_space(q), RET_BAD_PARAMS);

  for (i = 0; i < size; i++) {
    if (q->w == q->size) {
      q->w = 0;
    }
    q->buff[q->w++] = payload[i];
  }

  if (q->w == q->size) {
    q->w = 0;
  }

  if (q->r == q->w) {
    q->full = TRUE;
  }

  return RET_OK;
}

ret_t request_queue_read_data(request_queue_t* q, uint8_t* payload, uint32_t size) {
  uint32_t i = 0;
  return_value_if_fail(q != NULL && size <= request_queue_get_available_data(q), RET_BAD_PARAMS);

  for (i = 0; i < size; i++) {
    if (q->r == q->size) {
      q->r = 0;
    }
    payload[i] = q->buff[q->r++];
  }

  q->full = FALSE;
  if (q->r == q->size) {
    q->r = 0;
  }

  return RET_OK;
}

ret_t request_queue_send(request_queue_t* q, uint32_t cmd, uint32_t payload_size,
                         const void* payload) {
  uint32_t size = sizeof(cmd) + sizeof(payload_size) + payload_size;
  return_value_if_fail(q != NULL && request_queue_get_available_space(q) >= size, RET_BAD_PARAMS);
  return_value_if_fail(payload_size <= q->max_payload_size, RET_BAD_PARAMS);

  return_value_if_fail(tk_mutex_lock(q->mutex) == RET_OK, RET_FAIL);

  ENSURE(request_queue_write_data(q, (const uint8_t*)(&cmd), sizeof(cmd)) == RET_OK);
  ENSURE(request_queue_write_data(q, (const uint8_t*)(&payload_size), sizeof(payload_size)) ==
         RET_OK);
  if (payload_size > 0 && payload != NULL) {
    ENSURE(request_queue_write_data(q, payload, payload_size) == RET_OK);
  }

  ENSURE(tk_mutex_unlock(q->mutex) == RET_OK);

  return RET_OK;
}

ret_t request_queue_process(request_queue_t* q, uint32_t max_requests) {
  uint32_t i = 0;
  uint32_t cmd = 0;
  ret_t ret = RET_OK;
  uint32_t payload_size = 0;
  return_value_if_fail(q != NULL && q->buff != NULL && q->on_request != NULL, RET_BAD_PARAMS);

  return_value_if_fail(tk_mutex_lock(q->mutex) == RET_OK, RET_FAIL);

  for (i = 0; i < max_requests; i++) {
    if (request_queue_get_available_data(q) >= 8) {
      memset(q->payload, 0x00, q->max_payload_size);
      ENSURE(request_queue_read_data(q, (uint8_t*)(&cmd), sizeof(cmd)) == RET_OK);
      ENSURE(request_queue_read_data(q, (uint8_t*)(&payload_size), sizeof(payload_size)) == RET_OK);
      ENSURE(request_queue_read_data(q, q->payload, payload_size) == RET_OK);

      ret = q->on_request(q->on_request_ctx, cmd, payload_size, q->payload);
      if (ret != RET_OK) {
        break;
      }
    } else {
      break;
    }
  }

  ENSURE(tk_mutex_unlock(q->mutex) == RET_OK);

  return ret;
}

ret_t request_queue_destroy(request_queue_t* q) {
  return_value_if_fail(q != NULL && q->buff != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(q->buff);
  TKMEM_FREE(q->payload);
  tk_mutex_destroy(q->mutex);

  memset(q, 0x00, sizeof(request_queue_t));

  TKMEM_FREE(q);

  return RET_OK;
}
