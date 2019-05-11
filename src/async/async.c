/**
 * File:   async.c
 * Author: AWTK Develop Team
 * Brief:  async call
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
 * 2019-05-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/event.h"

#include "async/async.h"
#include "lite_service/service_thread.h"

typedef struct _async_request_t {
  void* ctx;
  async_exec_t exec;
  async_on_result_t on_result;

  ret_t result;
} async_request_t;

typedef enum _async_call_event_t { EVT_ASYNC_CALL_RESULT = 512 } async_call_event_t;

static ret_t async_call_service_run(lite_service_t* service) {
  event_t e = event_init(EVT_ASYNC_CALL_RESULT, service);
  async_request_t* request = (async_request_t*)(service->init_data);

  request->result = request->exec(request->ctx);

  return lite_service_dispatch(service, &e, sizeof(e));
}

static const lite_service_vtable_t s_async_call_service_vt = {
    .size = sizeof(lite_service_t),
    .type = "async",
    .desc = "async call",
    .run = async_call_service_run
};

static ret_t async_call_service_on_event(void* ctx, event_t* e) {
  ret_t ret = RET_OK;
  async_request_t* request = (async_request_t*)ctx;

  if (request->on_result != NULL) {
    ret = request->on_result(request->ctx, request->result);
  }

  TKMEM_FREE(request);

  return RET_OK;
}

static async_request_t* async_request_create(async_exec_t exec, async_on_result_t on_result,
                                             void* ctx) {
  async_request_t* request = NULL;
  return_value_if_fail(exec != NULL, NULL);

  request = TKMEM_ZALLOC(async_request_t);
  return_value_if_fail(request != NULL, NULL);

  request->ctx = ctx;
  request->exec = exec;
  request->on_result = on_result;

  return request;
}

ret_t async_call(async_exec_t exec, async_on_result_t on_result, void* ctx) {
  tk_thread_t* thread = NULL;
  async_request_t* request = async_request_create(exec, on_result, ctx);
  return_value_if_fail(request != NULL, RET_BAD_PARAMS);

  thread = service_thread_start(&s_async_call_service_vt, request, 
      async_call_service_on_event, request);

  return thread != NULL ? RET_OK : RET_FAIL;
}
