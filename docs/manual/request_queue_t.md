## request\_queue\_t
### 概述

 请求队列。



----------------------------------
### 函数
<p id="request_queue_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#request_queue_t_request_queue_create">request\_queue\_create</a> |  |
| <a href="#request_queue_t_request_queue_destroy">request\_queue\_destroy</a> |  |
| <a href="#request_queue_t_request_queue_process">request\_queue\_process</a> |  |
| <a href="#request_queue_t_request_queue_send">request\_queue\_send</a> |  |
| <a href="#request_queue_t_service_thread_request">service\_thread\_request</a> |  |
| <a href="#request_queue_t_service_thread_start">service\_thread\_start</a> |  |
#### request\_queue\_create 函数
-----------------------

* 函数功能：

> <p id="request_queue_t_request_queue_create">
 创建request queue对象。





* 函数原型：

```
request_queue_t* request_queue_create (uint32_t size, uint32_t max_payload_size, request_queue_on_request_t on_request, void* on_request_ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | request\_queue\_t* | 返回request\_queue对象。 |
| size | uint32\_t | Buffer的大小。 |
| max\_payload\_size | uint32\_t | payload最大长度。 |
| on\_request | request\_queue\_on\_request\_t | 请求处理函数。 |
| on\_request\_ctx | void* | 请求处理函数的上下文。 |
#### request\_queue\_destroy 函数
-----------------------

* 函数功能：

> <p id="request_queue_t_request_queue_destroy">
 销毁队列。





* 函数原型：

```
ret_t request_queue_destroy (request_queue_t* q);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| q | request\_queue\_t* | request\_queue对象。 |
#### request\_queue\_process 函数
-----------------------

* 函数功能：

> <p id="request_queue_t_request_queue_process">
 处理队列中的请求(由服务线程调用)。





* 函数原型：

```
ret_t request_queue_process (request_queue_t* q, uint32_t max_requests);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| q | request\_queue\_t* | request\_queue对象。 |
| max\_requests | uint32\_t | 最大处理请求的个数。 |
#### request\_queue\_send 函数
-----------------------

* 函数功能：

> <p id="request_queue_t_request_queue_send">
 发送一个请求。





* 函数原型：

```
ret_t request_queue_send (request_queue_t* q, uint32_t cmd, uint32_t payload_size, const void* payload);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| q | request\_queue\_t* | request\_queue对象。 |
| cmd | uint32\_t | 命令ID。 |
| payload\_size | uint32\_t | 数据长度。 |
| payload | const void* | 数据(可以为NULL)。 |
#### service\_thread\_request 函数
-----------------------

* 函数功能：

> <p id="request_queue_t_service_thread_request">
 请求服务。





* 函数原型：

```
ret_t service_thread_request (tk_thread_t* service, uint32_t cmd, uint32_t data_size, const void* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| service | tk\_thread\_t* | service thread对象。 |
| cmd | uint32\_t | 命令ID。 |
| data\_size | uint32\_t | 数据长度。 |
| data | const void* | 数据。 |
#### service\_thread\_start 函数
-----------------------

* 函数功能：

> <p id="request_queue_t_service_thread_start">
 启动服务线程。





* 函数原型：

```
ret_t service_thread_start (const lite_service_vtable_t* vt, void* init_data, event_func_t on_event, void* on_event_ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| vt | const lite\_service\_vtable\_t* | lite\_service vtable对象。 |
| init\_data | void* | 初始化数据。 |
| on\_event | event\_func\_t | 事件处理函数。 |
| on\_event\_ctx | void* | 事件处理函数的上下文。 |
