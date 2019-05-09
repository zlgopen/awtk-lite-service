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
 * @method http_request
 *
 * 发起一个HTTP请求。
 * 
 * @param {http_request_t*} request HTTP请求对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t http_request(http_request_t* request);

/**
 * @method http_init
 *
 * 全局初始化。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t http_init(void);

/**
 * @method http_deinit
 *
 * 全局~初始化。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t http_deinit(void);

END_C_DECLS

#endif /*TK_HTTP_H*/

