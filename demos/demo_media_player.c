/**
 * File:   demo_media_player.c
 * Author: AWTK Develop Team
 * Brief:  media_player demo
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
#include "media_player/media_player_ffmpeg.h"

static ret_t on_load_click(void* ctx, event_t* e) {
  media_player_t* player = (media_player_t*)ctx;

  media_player_load(player, "./test.mp4");

  return RET_OK;
}

static ret_t on_forward_click(void* ctx, event_t* e) {
  media_player_t* player = (media_player_t*)ctx;

  return RET_OK;
}

static ret_t on_start_click(void* ctx, event_t* e) {
  media_player_t* player = (media_player_t*)ctx;

  media_player_start(player);

  return RET_OK;
}

static ret_t on_stop_click(void* ctx, event_t* e) {
  media_player_t* player = (media_player_t*)ctx;

  media_player_stop(player);

  return RET_OK;
}

static ret_t on_pause_click(void* ctx, event_t* e) {
  media_player_t* player = (media_player_t*)ctx;

  media_player_pause(player);

  return RET_OK;
}

static ret_t on_quit_click(void* ctx, event_t* e) {
  tk_quit();
  return RET_OK;
}

static ret_t mutable_image_prepare_image(void* ctx, bitmap_t* image) {
  media_player_t* player = (media_player_t*)ctx;

  return media_player_get_video_frame(player, image);
}

void application_init() {
  media_player_t* player = media_player_ffmpeg_create();

  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* mutable = mutable_image_create(win, 0, 0, win->w, win->h);
  widget_t* load = button_create(win, 0, 0, 0, 0);
  widget_t* start = button_create(win, 0, 0, 0, 0);
  widget_t* forward = button_create(win, 0, 0, 0, 0);
  widget_t* pause = button_create(win, 0, 0, 0, 0);
  widget_t* stop = button_create(win, 0, 0, 0, 0);
  widget_t* quit = button_create(win, 0, 0, 0, 0);
  widget_t* status = label_create(win, 0, 0, 0, 0);

  mutable_image_set_prepare_image(mutable, mutable_image_prepare_image, player);
  widget_set_self_layout_params(status, "center", "m:-120", "100%", "30");

  widget_set_text(load, L"load");
  widget_set_self_layout_params(load, "center", "middle:-80", "50%", "30");
  widget_on(load, EVT_CLICK, on_load_click, player);

  widget_set_text(start, L"start");
  widget_set_self_layout_params(start, "center", "middle:-40", "50%", "30");
  widget_on(start, EVT_CLICK, on_start_click, player);

  widget_set_text(forward, L"forward");
  widget_set_self_layout_params(forward, "center", "middle", "50%", "30");
  widget_on(forward, EVT_CLICK, on_forward_click, player);

  widget_set_text(pause, L"pause");
  widget_set_self_layout_params(pause, "center", "middle:40", "50%", "30");
  widget_on(pause, EVT_CLICK, on_pause_click, player);

  widget_set_text(stop, L"stop");
  widget_set_self_layout_params(stop, "center", "m:80", "50%", "30");
  widget_on(stop, EVT_CLICK, on_stop_click, player);

  widget_set_text(quit, L"Quit");
  widget_set_self_layout_params(quit, "center", "m:120", "50%", "30");
  widget_on(quit, EVT_CLICK, on_quit_click, quit);

  widget_layout(win);
}

void application_deinit() {
}

#include "demo_main.c"
