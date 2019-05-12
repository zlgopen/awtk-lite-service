## request\_queue\_t
### 概述

 请求队列。



----------------------------------
### 函数
<p id="request_queue_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#request_queue_t_request_queue_create">request\_queue\_create</a> | 创建request queue对象。 |
| <a href="#request_queue_t_request_queue_destroy">request\_queue\_destroy</a> | 销毁队列。 |
| <a href="#request_queue_t_request_queue_process">request\_queue\_process</a> | 处理队列中的请求(由服务线程调用)。 |
| <a href="#request_queue_t_request_queue_send">request\_queue\_send</a> | 发送一个请求。 |
#### request\_queue\_create 函数
-----------------------

* 函数功能：

> <p id="request_queue_t_request_queue_create"> 创建request queue对象。





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

> <p id="request_queue_t_request_queue_destroy"> 销毁队列。





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

> <p id="request_queue_t_request_queue_process"> 处理队列中的请求(由服务线程调用)。





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

> <p id="request_queue_t_request_queue_send"> 发送一个请求。





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
