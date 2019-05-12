/**
 * File:   http.h
 * Author: AWTK Develop Team
 * Brief:  http
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

#ifndef TK_HTTP_H
#define TK_HTTP_H

#include "http/http_request.h"
#include "http/http_response.h"

BEGIN_C_DECLS

/**
 * @class http_t
 * HTTP请求。
 *
 * @annotation ["fake"]
 *
 */

/**
 * @method http_request
 * 启动一个lite service线程处理HTTP请求，在处理过程会把事件通过request->on_event通知调用者。
 *
 *注意：
 *
 *> * request->on_event在UI线程中执行，所以在该函数中可以操作GUI的控件。
 *> * request只能在request->response的fail标志或done标志为TRUE后才能销毁。
 *> * 可以通过http_request_abort取消请求，取消请求异步执行，可能会有一定延迟。
 *> * 本函数适合一般的REST API，不适合传输大文件，也不适合同时发送大量请求。
 *
 * @param {http_request_t*} request HTTP请求对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t http_request(http_request_t* request);

/**
 * @method http_init
 * 全局初始化，在应用程序初始化时调用。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t http_init(void);

/**
 * @method http_deinit
 * 全局~初始化，在应用程序结束时调用。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t http_deinit(void);

END_C_DECLS

#endif /*TK_HTTP_H*/
