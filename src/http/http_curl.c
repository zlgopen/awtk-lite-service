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
#include "tkc/action_thread_pool.h"

typedef struct _http_agent_curl_t {
  CURL* curl;
  http_request_t* request;
  http_response_t* response;
  uint32_t up_content_length;
  uint32_t down_content_length;

  char header_buffer[2048];
  struct curl_slist* request_header;
} http_agent_curl_t;

static http_agent_curl_t* http_agent_curl_from_action(qaction_t* action) {
  http_agent_curl_t* http = (http_agent_curl_t*)(action->args);

  return http;
}

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

static ret_t http_agent_dispatch(qaction_t* action, event_t* e) {
  return qaction_notify(action, e);
}

static int http_on_curl_progress(void* ctx, curl_off_t dltotal, curl_off_t dlnow,
                                 curl_off_t ultotal, curl_off_t ulnow) {
  long http_code = 0;
  qaction_t* action = (qaction_t*)ctx;
  http_agent_curl_t* http = http_agent_curl_from_action(action);

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
  int percent = 0;
  progress_event_t progress;
  size_t total_size = size * nmemb;
  qaction_t* action = (qaction_t*)ctx;
  http_agent_curl_t* http = http_agent_curl_from_action(action);

  if (http->down_content_length <= 0) {
    const char* value = http_response_find(http->response, "Content-Length");

    if(value != NULL) {
      http->down_content_length = tk_atoi(value);
    }
  }

  http_response_append_body_data(http->response, buffer, total_size);
 
  if(http->down_content_length > 0) {
    percent = ((100 * http->response->body_size)/http->down_content_length);
  } else {
    percent = 50;
  }

  http_agent_dispatch(action, progress_event_init(&progress, percent));

  return total_size;
}

static size_t http_on_curl_header_data(char* buffer, size_t size, size_t nitems, void* ctx) {
  qaction_t* action = (qaction_t*)ctx;
  http_agent_curl_t* http = http_agent_curl_from_action(action);

  http_response_parse_line(http->response, buffer);

  return size * nitems;
}

static CURL* curl_create_with_request(qaction_t* action, http_request_t* request) {
  CURL* curl = curl_easy_init();
  http_agent_curl_t* http = http_agent_curl_from_action(action);

  return_value_if_fail(curl != NULL, NULL);

  http->curl = curl;
  http->up_content_length = 0;
  http->down_content_length = 0;

  curl_easy_setopt(curl, CURLOPT_URL, request->url);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 5);

  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, http_on_curl_header_data);
  curl_easy_setopt(curl, CURLOPT_HEADERDATA, action);

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_on_curl_body_data);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, action);

#if LIBCURL_VERSION_NUM >= 0x072000
  curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, http_on_curl_progress);
  curl_easy_setopt(curl, CURLOPT_XFERINFODATA, action);
#else
  curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, http_on_curl_progress_older);
  curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, action);
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

static ret_t http_agent_exec(qaction_t* action) {
  CURLcode res = 0;
  CURL* curl = NULL;
  http_response_t* response = NULL;
  http_agent_curl_t* http = http_agent_curl_from_action(action);
  http_request_t* request = http->request;
  return_value_if_fail(request != NULL, RET_BAD_PARAMS);

  response = request->response;
  http->request = request;
  http->response = response;
  curl = curl_create_with_request(action, request);
  if (curl) {
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      http_response_set_fail(request->response, TRUE);
    } else {
      http_response_set_done(request->response, TRUE);
    }

    curl_easy_cleanup(curl);
  } else {
    http_response_set_fail(request->response, TRUE);
  }

  return RET_OK;
}

static ret_t http_agent_on_event(qaction_t* action, event_t* e) {
  ret_t ret = RET_OK;
  http_agent_curl_t* http = http_agent_curl_from_action(action);
  http_request_t* request = http->request;

  if (request->on_event != NULL) {
    http_response_lock(request->response);
    ret = request->on_event(request->on_event_ctx, request);
    http_response_unlock(request->response);
  }

  if(e->type == EVT_DONE) {
    if (http->request_header != NULL) {
      curl_slist_free_all(http->request_header);
    }

    qaction_destroy(action);
  }

  return RET_OK;
}

static action_thread_pool_t* s_http_thread_pool = NULL;

ret_t http_init(void) {
  curl_global_init(CURL_GLOBAL_ALL);
  return_value_if_fail(s_http_thread_pool == NULL, RET_BAD_PARAMS);
  s_http_thread_pool = action_thread_pool_create(5, 1);
  return_value_if_fail(s_http_thread_pool != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

ret_t http_deinit(void) {
  curl_global_cleanup();
  return_value_if_fail(s_http_thread_pool != NULL, RET_BAD_PARAMS);

  action_thread_pool_destroy(s_http_thread_pool);
  s_http_thread_pool = NULL;

  return RET_OK;
}

ret_t http_request(http_request_t* request) {
  qaction_t* a = NULL;
  http_agent_curl_t* http = NULL;
  return_value_if_fail(request != NULL, RET_BAD_PARAMS);

  a = qaction_create(http_agent_exec, NULL, sizeof(http_agent_curl_t));
  
  http = http_agent_curl_from_action(a);
  http->request = request;
  ENSURE(qaction_set_on_event(a, http_agent_on_event) == RET_OK);

  return action_thread_pool_exec(s_http_thread_pool, a);
}

