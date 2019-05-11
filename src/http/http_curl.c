/**
 * File:   http_curl.c
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
#include "tkc/buffer.h"

#include <curl/curl.h>
#include "http/http_header.h"
#include "http/http_response.h"
#include "http/http_request.h"
#include "lite_service/lite_service.h"
#include "lite_service/service_thread.h"

typedef struct _http_agent_curl_t {
  lite_service_t lite_service;

  CURL* curl;
  http_request_t* request;
  http_response_t* response;
  uint32_t up_content_length;
  uint32_t down_content_length;

  char header_buffer[2048];
  struct curl_slist* request_header;
} http_agent_curl_t;

static ret_t http_agent_set_header(http_agent_curl_t* http, http_header_t* header) {
  char str[512];
  http_header_t* iter = header;
  struct curl_slist* request_header = NULL;

  if (header == NULL) {
    return RET_OK;
  }

  while (iter != NULL) {
    tk_snprintf(str, sizeof(str) - 1, "%s: %s", iter->key, iter->value);
    request_header = curl_slist_append(request_header, str);

    iter = iter->next;
  }

  http->request_header = request_header;
  curl_easy_setopt(http->curl, CURLOPT_HTTPHEADER, request_header);

  return RET_OK;
}

static ret_t http_agent_dispatch(lite_service_t* service, http_event_t type) {
  event_t e = event_init(type, service);

  return lite_service_dispatch(service, &e, sizeof(e));
}

static int http_on_curl_progress(void* ctx, curl_off_t dltotal, curl_off_t dlnow,
                                 curl_off_t ultotal, curl_off_t ulnow) {
  long http_code = 0;
  http_agent_curl_t* http = (http_agent_curl_t*)ctx;

  if (!(http->response->status_code)) {
    curl_easy_getinfo(http->curl, CURLINFO_RESPONSE_CODE, &http_code);
    http_response_set_status_code(http->response, http_code);
  }

  return http->request->abort;
}

#if LIBCURL_VERSION_NUM < 0x072000
/* for libcurl older than 7.32.0 (CURLOPT_PROGRESSFUNCTION) */
static int http_on_curl_progress_older(void* p, double dltotal, double dlnow, double ultotal,
                                       double ulnow) {
  return http_on_curl_progress(p, (curl_off_t)dltotal, (curl_off_t)dlnow, (curl_off_t)ultotal,
                               (curl_off_t)ulnow);
}
#endif

size_t http_on_curl_body_data(char* buffer, size_t size, size_t nmemb, void* ctx) {
  double cl = 0;
  size_t total_size = size * nmemb;
  lite_service_t* service = (lite_service_t*)ctx;
  http_agent_curl_t* http = (http_agent_curl_t*)ctx;

  if (http->down_content_length <= 0) {
    if (curl_easy_getinfo(http->curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &cl)) {
      http->down_content_length = cl;
    }
  }

  http_response_append_body_data(http->response, buffer, total_size);
  http_agent_dispatch(service, HTTP_EVENT_PROGRESS);

  return total_size;
}

static size_t http_on_curl_header_data(char* buffer, size_t size, size_t nitems, void* ctx) {
  http_agent_curl_t* http = (http_agent_curl_t*)ctx;

  http_response_parse_line(http->response, buffer);

  return size * nitems;
}

static CURL* curl_create_with_request(lite_service_t* service, http_request_t* request) {
  CURL* curl = curl_easy_init();
  http_agent_curl_t* http = (http_agent_curl_t*)service;

  return_value_if_fail(curl != NULL, NULL);

  http->curl = curl;
  http->up_content_length = 0;
  http->down_content_length = 0;

  curl_easy_setopt(curl, CURLOPT_URL, request->url);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 5);

  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, http_on_curl_header_data);
  curl_easy_setopt(curl, CURLOPT_HEADERDATA, service);

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_on_curl_body_data);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, service);

#if LIBCURL_VERSION_NUM >= 0x072000
  curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, http_on_curl_progress);
  curl_easy_setopt(curl, CURLOPT_XFERINFODATA, service);
#else
  curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, http_on_curl_progress_older);
  curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, service);
#endif
  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
  curl_easy_setopt(curl, CURLOPT_VERBOSE, TRUE);
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request->method);

  if (request->header != NULL) {
    http_agent_set_header(http, request->header);
  }

  if (request->body != NULL && request->body_size > 0) {
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request->body);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, request->body_size);
  }

  return curl;
}

static ret_t http_agent_run(lite_service_t* service) {
  CURLcode res = 0;
  CURL* curl = NULL;
  http_response_t* response = NULL;
  http_agent_curl_t* http = (http_agent_curl_t*)service;
  http_request_t* request = (http_request_t*)(service->init_data);
  return_value_if_fail(request != NULL, RET_BAD_PARAMS);

  response = request->response;
  http->request = request;
  http->response = response;
  curl = curl_create_with_request(service, request);
  if (curl) {
    http_agent_dispatch(service, HTTP_EVENT_START);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
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
  http_agent_curl_t* http = (http_agent_curl_t*)service;
  if (http->request_header != NULL) {
    curl_slist_free_all(http->request_header);
  }

  return RET_OK;
}

static ret_t http_agent_on_request(lite_service_t* service, uint32_t cmd, uint32_t data_size,
                                   const void* data) {
  return RET_OK;
}

static const lite_service_vtable_t s_http_agent_vt = {.size = sizeof(http_agent_curl_t),
                                                      .type = "http",
                                                      .desc = "http agent",
                                                      .queue_size = 5120,
                                                      .max_payload_size = 512,
                                                      .run = http_agent_run,
                                                      .on_request = http_agent_on_request,
                                                      .on_destroy = http_agent_on_destroy};

static ret_t http_agent_on_event(void* ctx, event_t* e) {
  ret_t ret = RET_OK;
  http_request_t* request = (http_request_t*)ctx;

  if (request->on_event != NULL) {
    http_response_lock(request->response);
    ret = request->on_event(request->on_event_ctx, request, request->response);
    http_response_unlock(request->response);
  }

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
