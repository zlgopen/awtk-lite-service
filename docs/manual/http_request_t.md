## http\_request\_t
### 概述

 HTTP request



----------------------------------
### 函数
<p id="http_request_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#http_request_t_http_request_abort">http\_request\_abort</a> | 设置abort标志，通知服务取消本次请求。 |
| <a href="#http_request_t_http_request_add_header">http\_request\_add\_header</a> | 增加一个header。 |
| <a href="#http_request_t_http_request_create_delete">http\_request\_create\_delete</a> | 创建一个DELETE请求。 |
| <a href="#http_request_t_http_request_create_get">http\_request\_create\_get</a> | 创建一个GET请求。 |
| <a href="#http_request_t_http_request_create_post">http\_request\_create\_post</a> | 创建一个POST请求。 |
| <a href="#http_request_t_http_request_create_put">http\_request\_create\_put</a> | 创建一个PUT请求。 |
| <a href="#http_request_t_http_request_destroy">http\_request\_destroy</a> | 销毁request对象。 |
### 属性
<p id="http_request_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#http_request_t_abort">abort</a> | bool\_t | 取消本次请求。 |
| <a href="#http_request_t_body">body</a> | void* | PUT/POST请求上传的数据。 |
| <a href="#http_request_t_body_size">body\_size</a> | uint32\_t | PUT/POST请求上传的数据的长度。 |
| <a href="#http_request_t_header">header</a> | http\_header\_t* | 额外的header信息。 |
| <a href="#http_request_t_method">method</a> | char* | method。 |
| <a href="#http_request_t_response">response</a> | http\_response\_t* | 请求的结果。 |
| <a href="#http_request_t_url">url</a> | char* | url。 |
#### http\_request\_abort 函数
-----------------------

* 函数功能：

> <p id="http_request_t_http_request_abort"> 设置abort标志，通知服务取消本次请求。





* 函数原型：

```
ret_t http_request_abort (http_request_t* request);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| request | http\_request\_t* | http request对象。 |
#### http\_request\_add\_header 函数
-----------------------

* 函数功能：

> <p id="http_request_t_http_request_add_header"> 增加一个header。





* 函数原型：

```
ret_t http_request_add_header (http_request_t* request, const char* key, const char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| request | http\_request\_t* | http request对象。 |
| key | const char* | header的键名。 |
| value | const char* | header的键值。 |
#### http\_request\_create\_delete 函数
-----------------------

* 函数功能：

> <p id="http_request_t_http_request_create_delete"> 创建一个DELETE请求。

 回调函数在GUI线程中执行，在回调函数中可以直接操作GUI的控件。





* 函数原型：

```
http_request_t* http_request_create_delete (const char* url, http_request_on_event_t on_event, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | http\_request\_t* | 返回request对象。 |
| url | const char* | url。 |
| on\_event | http\_request\_on\_event\_t | 事件回调函数。 |
| ctx | void* | 事件回调函数的上下文。 |
#### http\_request\_create\_get 函数
-----------------------

* 函数功能：

> <p id="http_request_t_http_request_create_get"> 创建一个GET请求。

 回调函数在GUI线程中执行，在回调函数中可以直接操作GUI的控件。





* 函数原型：

```
http_request_t* http_request_create_get (const char* url, http_request_on_event_t on_event, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | http\_request\_t* | 返回request对象。 |
| url | const char* | url。 |
| on\_event | http\_request\_on\_event\_t | 事件回调函数。 |
| ctx | void* | 事件回调函数的上下文。 |
#### http\_request\_create\_post 函数
-----------------------

* 函数功能：

> <p id="http_request_t_http_request_create_post"> 创建一个POST请求。

 回调函数在GUI线程中执行，在回调函数中可以直接操作GUI的控件。





* 函数原型：

```
http_request_t* http_request_create_post (const char* url, http_request_on_event_t on_event, void* ctx, const char* content_type, const void* data, uint32_t data_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | http\_request\_t* | 返回request对象。 |
| url | const char* | url。 |
| on\_event | http\_request\_on\_event\_t | 事件回调函数。 |
| ctx | void* | 事件回调函数的上下文。 |
| content\_type | const char* | 上传内容的类型。 |
| data | const void* | 要上传的数据。 |
| data\_size | uint32\_t | 要上传的数据的长度。 |
#### http\_request\_create\_put 函数
-----------------------

* 函数功能：

> <p id="http_request_t_http_request_create_put"> 创建一个PUT请求。

 回调函数在GUI线程中执行，在回调函数中可以直接操作GUI的控件。





* 函数原型：

```
http_request_t* http_request_create_put (const char* url, http_request_on_event_t on_event, void* ctx, const char* content_type, const void* data, uint32_t data_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | http\_request\_t* | 返回request对象。 |
| url | const char* | url。 |
| on\_event | http\_request\_on\_event\_t | 事件回调函数。 |
| ctx | void* | 事件回调函数的上下文。 |
| content\_type | const char* | 上传内容的类型。 |
| data | const void* | 要上传的数据。 |
| data\_size | uint32\_t | 要上传的数据的长度。 |
#### http\_request\_destroy 函数
-----------------------

* 函数功能：

> <p id="http_request_t_http_request_destroy"> 销毁request对象。





* 函数原型：

```
ret_t http_request_destroy (http_request_t* request);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| request | http\_request\_t* | http request对象。 |
#### abort 属性
-----------------------
> <p id="http_request_t_abort"> 取消本次请求。



* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### body 属性
-----------------------
> <p id="http_request_t_body"> PUT/POST请求上传的数据。



* 类型：void*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### body\_size 属性
-----------------------
> <p id="http_request_t_body_size"> PUT/POST请求上传的数据的长度。



* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### header 属性
-----------------------
> <p id="http_request_t_header"> 额外的header信息。



* 类型：http\_header\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### method 属性
-----------------------
> <p id="http_request_t_method"> method。



* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### response 属性
-----------------------
> <p id="http_request_t_response"> 请求的结果。



* 类型：http\_response\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### url 属性
-----------------------
> <p id="http_request_t_url"> url。



* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
