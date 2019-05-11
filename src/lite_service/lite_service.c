/**
 * File:   lite_service.c
 * Author: AWTK Develop Team
 * Brief:  light service interface
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
#include "tkc/platform.h"
#include "lite_service/lite_service.h"

lite_service_t* lite_service_create(const lite_service_vtable_t* vt, void* init_data) {
  lite_service_t* service = NULL;
  return_value_if_fail(vt != NULL && (vt->run != NULL || vt->on_request != NULL) &&
                           vt->size >= sizeof(lite_service_t),
                       NULL);

  service = (lite_service_t*)TKMEM_ALLOC(vt->size);
  return_value_if_fail(service != NULL, NULL);

  memset(service, 0x00, vt->size);

  service->vt = vt;
  service->thread = NULL;
  service->init_data = init_data;
  if (vt->queue_size > 128 && vt->on_request != NULL) {
    request_queue_on_request_t on_request = (request_queue_on_request_t)(vt->on_request);
    service->queue =
        request_queue_create(vt->queue_size, vt->max_payload_size, on_request, service);
  }

  return service;
}

static ret_t lite_service_run_default(lite_service_t* service) {
  return_value_if_fail(service != NULL && service->vt != NULL && service->vt->on_request != NULL,
                       RET_BAD_PARAMS);

  while (request_queue_process(service->queue, 10) == RET_OK) {
    sleep_ms(100);
  }

  return RET_OK;
}

ret_t lite_service_run(lite_service_t* service) {
  return_value_if_fail(service != NULL && service->vt != NULL, RET_BAD_PARAMS);

  if (service->vt->run != NULL) {
    return service->vt->run(service);
  } else {
    return lite_service_run_default(service);
  }
}

uint32_t lite_service_set_on_event(lite_service_t* service, event_func_t on_event, void* ctx) {
  return_value_if_fail(service != NULL && service->vt != NULL && on_event != NULL, TK_INVALID_ID);

  service->on_event = on_event;
  service->on_event_ctx = ctx;

  return RET_OK;
}

typedef struct _idle_data_t {
  event_func_t on_event;
  void* on_event_ctx;
  char event[4];
} idle_data_t;

static ret_t lite_service_dispatch_in_idle(const idle_info_t* info) {
  idle_data_t* data = (idle_data_t*)(info->ctx);
  event_t* e = (event_t*)(data->event);

  data->on_event(data->on_event_ctx, e);

  TKMEM_FREE(data);

  return RET_REMOVE;
}

ret_t lite_service_dispatch(lite_service_t* service, event_t* e, uint32_t size) {
  idle_data_t* data = NULL;
  return_value_if_fail(service != NULL && service->vt != NULL && e != NULL, RET_BAD_PARAMS);

  if (service->on_event != NULL) {
    data = (idle_data_t*)TKMEM_ALLOC(sizeof(idle_data_t) + size);
    return_value_if_fail(data != NULL, RET_OOM);

    memcpy(data->event, e, size);
    data->on_event = service->on_event;
    data->on_event_ctx = service->on_event_ctx;

    if (service->idle_queue) {
      service->idle_queue(lite_service_dispatch_in_idle, data);
    } else {
      idle_queue(lite_service_dispatch_in_idle, data);
    }
  }

  return RET_OK;
}

ret_t lite_service_request(lite_service_t* service, uint32_t cmd, uint32_t data_size,
                           const void* data) {
  return_value_if_fail(service != NULL && service->vt != NULL && service->queue != NULL,
                       RET_BAD_PARAMS);

  return request_queue_send(service->queue, cmd, data_size, data);
}

ret_t lite_service_set_idle_queue(lite_service_t* service, lite_service_idle_queue_t idle_queue) {
  return_value_if_fail(service != NULL, RET_BAD_PARAMS);

  service->idle_queue = idle_queue;

  return RET_OK;
}

ret_t lite_service_destroy(lite_service_t* service) {
  return_value_if_fail(service != NULL && service->vt != NULL, RET_BAD_PARAMS);

  if (service->vt->on_destroy != NULL) {
    service->vt->on_destroy(service);
  }

  if (service->thread != NULL) {
    tk_thread_destroy(service->thread);
  }

  if (service->queue != NULL) {
    request_queue_destroy(service->queue);
  }
  memset(service, 0x00, service->vt->size);

  TKMEM_FREE(service);

  return RET_OK;
}
