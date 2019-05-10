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

static ret_t on_http_event(void* ctx, http_request_t* req, http_response_t* resp) {
  widget_t* widget = WIDGET(ctx);
  if (resp->done || resp->fail) {
    widget_set_enable(widget, TRUE);
  }

  log_debug("downloaded=%u\n", resp->downloaded_size);

  return RET_OK;
}

static ret_t on_click(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(e->target);
  const char* url = "https://github.com/zlgopen/awtk";
  http_request_t* request = http_request_create_get(url, on_http_event, widget);

  http_request(request);
  widget_set_enable(widget, FALSE);

  return RET_OK;
}

void application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* ok = button_create(win, 0, 0, 0, 0);

  http_init();
  widget_set_text(ok, L"GET");
  widget_set_self_layout_params(ok, "center", "middle", "50%", "30");
  widget_on(ok, EVT_CLICK, on_click, NULL);

  widget_layout(win);
}

#include "demo_main.c"
