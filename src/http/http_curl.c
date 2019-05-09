/**
 * File:   http_curl.h
 * Author: AWTK Develop Team
 * Brief:  http implemented with libcurl.
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
#include "tkc/event.h"
#include <curl/curl.h>
#include "http/http_response.h"
#include "http/http_request.h"
#include "lite_service/service_thread.h"

static ret_t http_agent_dispatch(lite_service_t* service, http_event_t type) {
  event_t e = event_init(type, service);

  return lite_service_dispatch(service, &e, sizeof(e));
}

static ret_t http_agent_run(lite_service_t* service) {
  CURLcode res = 0;
  CURL *curl = NULL;
  http_request_t* request = (http_request_t*)(service->init_data);
  return_value_if_fail(request != NULL, RET_BAD_PARAMS);

  curl = curl_easy_init();
  if(curl) {
    http_agent_dispatch(service, HTTP_EVENT_START);
    curl_easy_setopt(curl, CURLOPT_URL, request->url);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
      http_agent_dispatch(service, HTTP_EVENT_ERROR);
      http_response_set_fail(request->response, TRUE);
    } else {
      http_agent_dispatch(service, HTTP_EVENT_DONE);
      http_response_set_done(request->response, TRUE);
    }

    curl_easy_cleanup(curl);
  } else {
    http_agent_dispatch(service, HTTP_EVENT_ERROR);
  }

  return RET_OK;
}

static ret_t http_agent_on_destroy(lite_service_t* service) {
  return RET_OK;
}

static ret_t http_agent_on_request(lite_service_t* service, uint32_t cmd, uint32_t data_size, const void* data) {

  return RET_OK;
}

static const lite_service_vtable_t s_http_agent_vt = {
  .size = sizeof(lite_service_t),
  .type = "http",
  .desc = "http agent",
  .queue_size = 5120,
  .max_payload_size = 512,
  .run = http_agent_run,
  .on_request = http_agent_on_request,
  .on_destroy = http_agent_on_destroy
};

static ret_t http_agent_on_event(void* ctx, event_t* e) {
  http_request_t* request = (http_request_t*)ctx;
  lite_service_t* service = (lite_service_t*)(e->target);

  log_debug("on_event: %d\n", e->type);

  http_response_lock(request->response);
  request->on_event(request->on_event_ctx, request, request->response);
  http_response_unlock(request->response);

  return RET_OK;
}

ret_t http_request(http_request_t* request) {
  tk_thread_t* thread = NULL;
  return_value_if_fail(request != NULL, RET_BAD_PARAMS);

  thread = service_thread_start(&s_http_agent_vt, request, http_agent_on_event, request);

  return thread != NULL ? RET_OK : RET_FAIL;
}


ret_t http_init(void) {
  curl_global_init(CURL_GLOBAL_ALL);

  return RET_OK;
}

ret_t http_deinit(void) {
  curl_global_cleanup();
  
  return RET_OK;
}
