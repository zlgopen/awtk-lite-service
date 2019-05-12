## http\_response\_t
### 概述

 HTTP response

 在request->on_event回调函数中，可以直接访问本类的成员。



----------------------------------
### 函数
<p id="http_response_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#http_response_t_http_response_add_header">http\_response\_add\_header</a> | 增加一个header。 |
| <a href="#http_response_t_http_response_append_body_data">http\_response\_append\_body\_data</a> | 追加下载的数据。 |
| <a href="#http_response_t_http_response_create">http\_response\_create</a> | 创建一个response对象。 |
| <a href="#http_response_t_http_response_destroy">http\_response\_destroy</a> | 销毁response对象。 |
| <a href="#http_response_t_http_response_find_header">http\_response\_find\_header</a> | 查找指定的key，如果找到返回它的value。 |
| <a href="#http_response_t_http_response_lock">http\_response\_lock</a> | 锁定response对象。 |
| <a href="#http_response_t_http_response_parse_line">http\_response\_parse\_line</a> | 解析一行数据。 |
| <a href="#http_response_t_http_response_set_done">http\_response\_set\_done</a> | 设置完成标志。 |
| <a href="#http_response_t_http_response_set_fail">http\_response\_set\_fail</a> | 设置失败标志。 |
| <a href="#http_response_t_http_response_set_status_code">http\_response\_set\_status\_code</a> | 设置status。 |
| <a href="#http_response_t_http_response_set_uploaded_size">http\_response\_set\_uploaded\_size</a> | 更新已经上传数据的大小。 |
| <a href="#http_response_t_http_response_unlock">http\_response\_unlock</a> | 解锁response对象。 |
### 属性
<p id="http_response_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#http_response_t_body">body</a> | void* | 下载的数据。 |
| <a href="#http_response_t_body_size">body\_size</a> | uint32\_t | 下载的数据的长度(已经下载)。 |
| <a href="#http_response_t_done;">done;</a> | bool\_t | 整个请求完成。 |
| <a href="#http_response_t_fail;">fail;</a> | bool\_t | 请求失败。 |
| <a href="#http_response_t_header">header</a> | http\_header\_t* | header信息。 |
| <a href="#http_response_t_status_code">status\_code</a> | uint32\_t | HTTP response 的status code。 |
| <a href="#http_response_t_uploaded_size">uploaded\_size</a> | uint32\_t | PUT/POST请求，已经上传的数据的长度。 |
#### http\_response\_add\_header 函数
-----------------------

* 函数功能：

> <p id="http_response_t_http_response_add_header"> 增加一个header。





* 函数原型：

```
ret_t http_response_add_header (http_response_t* response, const char* key, const char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| response | http\_response\_t* | http response对象。 |
| key | const char* | header的键名。 |
| value | const char* | header的键值。 |
#### http\_response\_append\_body\_data 函数
-----------------------

* 函数功能：

> <p id="http_response_t_http_response_append_body_data"> 追加下载的数据。





* 函数原型：

```
ret_t http_response_append_body_data (http_response_t* response, const void* body, uint32_t data_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| response | http\_response\_t* | http response对象。 |
| body | const void* | 数据。 |
| data\_size | uint32\_t | 数据的长度。 |
#### http\_response\_create 函数
-----------------------

* 函数功能：

> <p id="http_response_t_http_response_create"> 创建一个response对象。




* 函数原型：

```
http_response_t* http_response_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | http\_response\_t* | 返回response对象。 |
#### http\_response\_destroy 函数
-----------------------

* 函数功能：

> <p id="http_response_t_http_response_destroy"> 销毁response对象。





* 函数原型：

```
ret_t http_response_destroy (http_response_t* response);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| response | http\_response\_t* | http response对象。 |
#### http\_response\_find\_header 函数
-----------------------

* 函数功能：

> <p id="http_response_t_http_response_find_header"> 查找指定的key，如果找到返回它的value。





* 函数原型：

```
const char* http_response_find_header (http_response_t* response, const char* key);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 如果找到，返回指定key的value，否则返回NULL。 |
| response | http\_response\_t* | response对象。 |
| key | const char* | header的key。 |
#### http\_response\_lock 函数
-----------------------

* 函数功能：

> <p id="http_response_t_http_response_lock"> 锁定response对象。

 在request的on_event回调函数访问response的属性无需锁定。





* 函数原型：

```
ret_t http_response_lock (http_response_t* response);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| response | http\_response\_t* | http response对象。 |
#### http\_response\_parse\_line 函数
-----------------------

* 函数功能：

> <p id="http_response_t_http_response_parse_line"> 解析一行数据。





* 函数原型：

```
ret_t http_response_parse_line (http_response_t* response, const char* buffer);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| response | http\_response\_t* | http response对象。 |
| buffer | const char* | 一行数据。 |
#### http\_response\_set\_done 函数
-----------------------

* 函数功能：

> <p id="http_response_t_http_response_set_done"> 设置完成标志。





* 函数原型：

```
ret_t http_response_set_done (http_response_t* response, bool_t done);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| response | http\_response\_t* | http response对象。 |
| done | bool\_t | 是否完成。 |
#### http\_response\_set\_fail 函数
-----------------------

* 函数功能：

> <p id="http_response_t_http_response_set_fail"> 设置失败标志。





* 函数原型：

```
ret_t http_response_set_fail (http_response_t* response, bool_t fail);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| response | http\_response\_t* | http response对象。 |
| fail | bool\_t | 是否失败。 |
#### http\_response\_set\_status\_code 函数
-----------------------

* 函数功能：

> <p id="http_response_t_http_response_set_status_code"> 设置status。





* 函数原型：

```
ret_t http_response_set_status_code (http_response_t* response, uint32_t code);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| response | http\_response\_t* | http response对象。 |
| code | uint32\_t | 响应码。 |
#### http\_response\_set\_uploaded\_size 函数
-----------------------

* 函数功能：

> <p id="http_response_t_http_response_set_uploaded_size"> 更新已经上传数据的大小。





* 函数原型：

```
ret_t http_response_set_uploaded_size (http_response_t* response, uint32_t uploaded_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| response | http\_response\_t* | http response对象。 |
| uploaded\_size | uint32\_t | 已经上传数据的大小。 |
#### http\_response\_unlock 函数
-----------------------

* 函数功能：

> <p id="http_response_t_http_response_unlock"> 解锁response对象。

 在request的on_event回调函数访问response的属性无需锁定。





* 函数原型：

```
ret_t http_response_unlock (http_response_t* response);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| response | http\_response\_t* | http response对象。 |
#### body 属性
-----------------------
> <p id="http_response_t_body"> 下载的数据。



* 类型：void*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### body\_size 属性
-----------------------
> <p id="http_response_t_body_size"> 下载的数据的长度(已经下载)。



* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### done; 属性
-----------------------
> <p id="http_response_t_done;"> 整个请求完成。



* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### fail; 属性
-----------------------
> <p id="http_response_t_fail;"> 请求失败。



* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### header 属性
-----------------------
> <p id="http_response_t_header"> header信息。



* 类型：http\_header\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### status\_code 属性
-----------------------
> <p id="http_response_t_status_code"> HTTP response 的status code。



* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### uploaded\_size 属性
-----------------------
> <p id="http_response_t_uploaded_size"> PUT/POST请求，已经上传的数据的长度。



* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
