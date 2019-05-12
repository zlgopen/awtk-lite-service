## http\_header\_t
### 概述

 HTTP request/response中header的key/value对象。



----------------------------------
### 函数
<p id="http_header_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#http_header_t_http_header_create">http\_header\_create</a> | 创建一个key/value对象。 |
| <a href="#http_header_t_http_header_destroy">http\_header\_destroy</a> | 销毁header(包括后继节点)。 |
| <a href="#http_header_t_http_header_find">http\_header\_find</a> | 查找指定的key，如果找到返回它的value。 |
| <a href="#http_header_t_http_header_prepend">http\_header\_prepend</a> | 创建一个key/value对象，并把next指向header。 |
### 属性
<p id="http_header_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#http_header_t_key">key</a> | char* | 键名。 |
| <a href="#http_header_t_next">next</a> | http\_header\_t* | 下一个节点。 |
| <a href="#http_header_t_value">value</a> | char* | 键值。 |
#### http\_header\_create 函数
-----------------------

* 函数功能：

> <p id="http_header_t_http_header_create"> 创建一个key/value对象。





* 函数原型：

```
http_header_t http_header_create (const char* key, const char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | http\_header\_t | 返回header对象。 |
| key | const char* | 键名。 |
| value | const char* | 键值。 |
#### http\_header\_destroy 函数
-----------------------

* 函数功能：

> <p id="http_header_t_http_header_destroy"> 销毁header(包括后继节点)。





* 函数原型：

```
ret_t http_header_destroy (http_header_t* header);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| header | http\_header\_t* | header对象。 |
#### http\_header\_find 函数
-----------------------

* 函数功能：

> <p id="http_header_t_http_header_find"> 查找指定的key，如果找到返回它的value。





* 函数原型：

```
const char* http_header_find (http_header_t* header, const char* key);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 如果找到，返回指定key的value，否则返回NULL。 |
| header | http\_header\_t* | header对象。 |
| key | const char* | header的key。 |
#### http\_header\_prepend 函数
-----------------------

* 函数功能：

> <p id="http_header_t_http_header_prepend"> 创建一个key/value对象，并把next指向header。





* 函数原型：

```
http_header_t http_header_prepend (const char* key, const char* value);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | http\_header\_t | 返回header对象。 |
| key | const char* | 键名。 |
| value | const char* | 键值。 |
#### key 属性
-----------------------
> <p id="http_header_t_key"> 键名。



* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### next 属性
-----------------------
> <p id="http_header_t_next"> 下一个节点。



* 类型：http\_header\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### value 属性
-----------------------
> <p id="http_header_t_value"> 键值。



* 类型：char*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
