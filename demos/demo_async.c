/**
 * File:   demo_async.c
 * Author: AWTK Develop Team
 * Brief:  async demo
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
#include "async/async.h"

static ret_t do_sth_take_time(void* ctx) {
  log_debug("do_sth_take_time begin\n");
  sleep_ms(3000);
  log_debug("do_sth_take_time done\n");

  return RET_OK;
}

static ret_t on_do_sth_take_time_done(void* ctx, ret_t result) {
  widget_set_enable(WIDGET(ctx), TRUE);
  log_debug("on_do_sth_take_time_done: result = %d\n", result);

  return RET_OK;
}

static ret_t on_exec_click(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(e->target);

  async_call(do_sth_take_time, on_do_sth_take_time_done, widget);

  widget_set_enable(widget, FALSE);

  return RET_OK;
}

void application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* exec = button_create(win, 0, 0, 0, 0);
  widget_t* status = label_create(win, 0, 0, 0, 0);

  widget_set_text(status, L"exe will take 3000 ms");
  widget_set_self_layout_params(status, "center", "m:-60", "100%", "30");

  widget_set_text(exec, L"exec");
  widget_set_self_layout_params(exec, "center", "middle", "50%", "30");
  widget_on(exec, EVT_CLICK, on_exec_click, exec);

  widget_layout(win);
}

#include "demo_main.c"
