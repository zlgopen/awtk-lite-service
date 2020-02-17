/**
 * File:   demo_http.c
 * Author: AWTK Develop Team
 * Brief:  http demo
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-05-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "http/http.h"

typedef struct _app_info_t {
  widget_t* get;
  widget_t* put;
  widget_t* del;
  widget_t* post;
  widget_t* abort;
  widget_t* status;
  http_request_t* request;
} app_info_t;

static app_info_t s_app;

static ret_t buttons_enable(bool_t start) {
  widget_set_enable(s_app.get, !start);
  widget_set_enable(s_app.put, !start);
  widget_set_enable(s_app.del, !start);
  widget_set_enable(s_app.post, !start);
  widget_set_enable(s_app.abort, start);

  return RET_OK;
}

typedef struct _update_info_t {
  uint32_t status_code;
  uint32_t body_size;
  bool_t done;
} update_info_t;

static update_info_t* update_info_create(uint32_t status_code, uint32_t body_size, bool_t done) {
  update_info_t* info = TKMEM_ZALLOC(update_info_t);
  return_value_if_fail(info != NULL, NULL);
  info->status_code = status_code;
  info->body_size = body_size;
  info->done = done;

  return info;
}

static ret_t update_info_destroy(update_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(info);

  return RET_OK;
}

static ret_t update_ui_in_idle(const idle_info_t* info) {
  char buff[128];
  update_info_t* update_info = (update_info_t*)(info->ctx);
  tk_snprintf(buff, sizeof(buff), "status=%d downloaded=%u", update_info->status_code,
              update_info->body_size);
  widget_set_text_utf8(s_app.status, buff);
  widget_invalidate_force(s_app.status, NULL);

  log_debug("%s\n", buff);
  if (update_info->done) {
    buttons_enable(FALSE);
    http_request_destroy(s_app.request);
    s_app.request = NULL;
  }
  update_info_destroy(update_info);

  return RET_OK;
}

static ret_t on_http_event(void* ctx, http_request_t* req) {
  http_response_t* resp = req->response;
  bool_t done = resp->done || resp->fail;
  update_info_t* info = update_info_create(resp->status_code, resp->body_size, done);

  idle_queue(update_ui_in_idle, info);
  if (done) {
    // log_debug("%s\n", resp->body);
  }

  return RET_OK;
}

static ret_t on_get_click(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(e->target);
  const char* url = "http://127.0.0.1:8000/get";
  s_app.request = http_request_create_get(url, on_http_event, widget);

  http_request(s_app.request);

  buttons_enable(TRUE);

  return RET_OK;
}

static ret_t on_put_click(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(e->target);
  const char* url = "http://127.0.0.1:8000/put";
  const char* data = "this is put request";

  s_app.request =
      http_request_create_put(url, on_http_event, widget, "text/plain", data, strlen(data));

  http_request(s_app.request);

  buttons_enable(TRUE);

  return RET_OK;
}

static ret_t on_del_click(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(e->target);
  const char* url = "http://127.0.0.1:8000/delete";

  s_app.request = http_request_create_delete(url, on_http_event, widget);

  http_request(s_app.request);

  buttons_enable(TRUE);

  return RET_OK;
}

static ret_t on_post_click(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(e->target);
  const char* url = "http://127.0.0.1:8000/post";
  const char* data = "this is post request";

  s_app.request =
      http_request_create_post(url, on_http_event, widget, "text/plain", data, strlen(data));

  http_request(s_app.request);

  buttons_enable(TRUE);

  return RET_OK;
}

static ret_t on_abort_click(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(e->target);

  http_request_abort(s_app.request);
  widget_set_enable(widget, FALSE);

  return RET_OK;
}

static ret_t on_quit_click(void* ctx, event_t* e) {
  tk_quit();
  return RET_OK;
}

void application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* get = button_create(win, 0, 0, 0, 0);
  widget_t* put = button_create(win, 0, 0, 0, 0);
  widget_t* del = button_create(win, 0, 0, 0, 0);
  widget_t* post = button_create(win, 0, 0, 0, 0);
  widget_t* abort = button_create(win, 0, 0, 0, 0);
  widget_t* quit = button_create(win, 0, 0, 0, 0);
  widget_t* status = label_create(win, 0, 0, 0, 0);

  http_init();

  widget_set_self_layout_params(status, "center", "m:-120", "100%", "30");

  widget_set_text(get, L"get");
  widget_set_self_layout_params(get, "center", "middle:-80", "50%", "30");
  widget_on(get, EVT_CLICK, on_get_click, get);

  widget_set_text(put, L"put");
  widget_set_self_layout_params(put, "center", "middle:-40", "50%", "30");
  widget_on(put, EVT_CLICK, on_put_click, put);

  widget_set_text(del, L"del");
  widget_set_self_layout_params(del, "center", "middle", "50%", "30");
  widget_on(del, EVT_CLICK, on_del_click, del);

  widget_set_text(post, L"post");
  widget_set_self_layout_params(post, "center", "middle:40", "50%", "30");
  widget_on(post, EVT_CLICK, on_post_click, post);

  widget_set_text(abort, L"Abort");
  widget_set_self_layout_params(abort, "center", "m:80", "50%", "30");
  widget_on(abort, EVT_CLICK, on_abort_click, abort);

  widget_set_text(quit, L"Quit");
  widget_set_self_layout_params(quit, "center", "m:120", "50%", "30");
  widget_on(quit, EVT_CLICK, on_quit_click, quit);

  s_app.get = get;
  s_app.put = put;
  s_app.del = del;
  s_app.post = post;
  s_app.abort = abort;
  s_app.status = status;
  s_app.request = NULL;
  widget_set_enable(s_app.abort, FALSE);

  widget_layout(win);
}

void application_deinit() {
  http_deinit();
}

#include "demo_main.c"
