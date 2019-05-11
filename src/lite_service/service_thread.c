/**
 * File:   service_thread.c
 * Author: AWTK Develop Team
 * Brief:  service thread
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
 * 2019-05-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "lite_service/service_thread.h"

static void* service_entry(void* args) {
  lite_service_t* service = (lite_service_t*)args;
  return_value_if_fail(service != NULL, NULL);

  log_debug("service_entry entry\n");
  lite_service_run(service);
  lite_service_destroy(service);
  log_debug("service_entry done\n");

  return NULL;
}

tk_thread_t* service_thread_start(const lite_service_vtable_t* vt, void* init_data,
                                  event_func_t on_event, void* on_event_ctx) {
  tk_thread_t* thread = NULL;
  lite_service_t* service = lite_service_create(vt, init_data);

  return_value_if_fail(service != NULL, NULL);

  lite_service_set_on_event(service, on_event, on_event_ctx);
  thread = tk_thread_create(service_entry, service);
  return_value_if_fail(thread != NULL, NULL);
  ENSURE(tk_thread_start(thread) == RET_OK);

  service->thread = thread;

  return thread;
}

ret_t service_thread_request(tk_thread_t* thread, uint32_t cmd, uint32_t data_size,
                             const void* data) {
  lite_service_t* service = tk_thread_get_args(thread);
  return_value_if_fail(service != NULL, RET_BAD_PARAMS);

  return lite_service_request(service, cmd, data_size, data);
}

ret_t service_thread_set_idle_queue(tk_thread_t* thread, lite_service_idle_queue_t idle_queue) {
  lite_service_t* service = tk_thread_get_args(thread);
  return_value_if_fail(service != NULL, RET_BAD_PARAMS);

  return lite_service_set_idle_queue(service, idle_queue);
}
