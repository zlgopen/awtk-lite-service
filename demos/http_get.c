/**
 * File:   button.c
 * Author: AWTK Develop Team
 * Brief:  button demo
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
 * 2018-08-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "http/http.h"

typedef struct _app_info_t {
  widget_t* ok;
  widget_t* abort;
  widget_t* status;
  http_request_t* request;
} app_info_t;

static app_info_t s_app;

static ret_t on_http_event(void* ctx, http_request_t* req, http_response_t* resp) {
  char buff[128];
  tk_snprintf(buff, sizeof(buff), 
      "status=%d downloaded=%u", resp->status_code, resp->body_size);
  widget_set_text_utf8(s_app.status, buff);
  widget_invalidate_force(s_app.status, NULL);

  if (resp->done || resp->fail) {
    log_debug("%s\n", resp->body);

    widget_set_enable(s_app.ok, TRUE);
    widget_set_enable(s_app.abort, FALSE);

    http_request_destroy(s_app.request);
    s_app.request = NULL;
  }

  return RET_OK;
}

static ret_t on_ok_click(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(e->target);
  const char* url = "https://github.com/zlgopen/awtk";
  s_app.request = http_request_create_get(url, on_http_event, widget);

  http_request(s_app.request);

  widget_set_enable(s_app.ok, FALSE);
  widget_set_enable(s_app.abort, TRUE);

  return RET_OK;
}

static ret_t on_abort_click(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(e->target);

  http_request_abort(s_app.request);
  widget_set_enable(widget, FALSE);

  return RET_OK;
}

void application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* ok = button_create(win, 0, 0, 0, 0);
  widget_t* abort = button_create(win, 0, 0, 0, 0);
  widget_t* status = label_create(win, 0, 0, 0, 0);

  http_init();

  widget_set_text(ok, L"GET");
  widget_set_self_layout_params(ok, "center", "middle", "50%", "30");
  widget_on(ok, EVT_CLICK, on_ok_click, ok);

  widget_set_text(abort, L"Abort");
  widget_set_self_layout_params(abort, "center", "m:60", "50%", "30");
  widget_on(abort, EVT_CLICK, on_abort_click, abort);
  
  widget_set_self_layout_params(status, "center", "m:-60", "100%", "30");

  s_app.ok = ok;
  s_app.abort = abort;
  s_app.status = status;
  s_app.request = NULL;
  widget_set_enable(s_app.abort, FALSE);

  widget_layout(win);
}

#include "demo_main.c"
