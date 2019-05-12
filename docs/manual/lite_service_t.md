## lite\_service\_t
### 概述

 服务基类。



----------------------------------
### 函数
<p id="lite_service_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#lite_service_t_lite_service_create">lite\_service\_create</a> | 创建lite service对象。 |
| <a href="#lite_service_t_lite_service_destroy">lite\_service\_destroy</a> | 销毁服务。 |
| <a href="#lite_service_t_lite_service_dispatch">lite\_service\_dispatch</a> | 分发事件(放在UI的idle中执行)。 |
| <a href="#lite_service_t_lite_service_request">lite\_service\_request</a> | 请求服务。 |
| <a href="#lite_service_t_lite_service_run">lite\_service\_run</a> | 运行服务(由服务线程调用)。 |
| <a href="#lite_service_t_lite_service_set_on_event">lite\_service\_set\_on\_event</a> | 注册服务的事件。事件处理函数自动放到GUI线程执行。 |
#### lite\_service\_create 函数
-----------------------

* 函数功能：

> <p id="lite_service_t_lite_service_create"> 创建lite service对象。





* 函数原型：

```
lite_service_t* lite_service_create (lite_service_vtable_t* vt, void* init_data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | lite\_service\_t* | 返回lite\_service对象。 |
| vt | lite\_service\_vtable\_t* | lite\_service虚表。 |
| init\_data | void* | 初始化数据。 |
#### lite\_service\_destroy 函数
-----------------------

* 函数功能：

> <p id="lite_service_t_lite_service_destroy"> 销毁服务。





* 函数原型：

```
ret_t lite_service_destroy (lite_service_t* service);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| service | lite\_service\_t* | lite\_service对象。 |
#### lite\_service\_dispatch 函数
-----------------------

* 函数功能：

> <p id="lite_service_t_lite_service_dispatch"> 分发事件(放在UI的idle中执行)。





* 函数原型：

```
ret_t lite_service_dispatch (lite_service_t* service, event_t* event, uint32_t event_size);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| service | lite\_service\_t* | lite\_service对象。 |
| event | event\_t* | 事件。 |
| event\_size | uint32\_t | 事件的大小。 |
#### lite\_service\_request 函数
-----------------------

* 函数功能：

> <p id="lite_service_t_lite_service_request"> 请求服务。





* 函数原型：

```
ret_t lite_service_request (lite_service_t* service, uint32_t cmd, uint32_t data_size, const void* data);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| service | lite\_service\_t* | lite\_service对象。 |
| cmd | uint32\_t | 命令ID。 |
| data\_size | uint32\_t | 数据长度。 |
| data | const void* | 数据。 |
#### lite\_service\_run 函数
-----------------------

* 函数功能：

> <p id="lite_service_t_lite_service_run"> 运行服务(由服务线程调用)。





* 函数原型：

```
ret_t lite_service_run (lite_service_t* service);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| service | lite\_service\_t* | lite\_service对象。 |
#### lite\_service\_set\_on\_event 函数
-----------------------

* 函数功能：

> <p id="lite_service_t_lite_service_set_on_event"> 注册服务的事件。事件处理函数自动放到GUI线程执行。





* 函数原型：

```
uint32_t lite_service_set_on_event (lite_service_t* service, event_func_t on_event, void* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回ID用于注销。 |
| service | lite\_service\_t* | lite\_service对象。 |
| on\_event | event\_func\_t | 事件处理函数。 |
| ctx | void* | 事件处理函数的上下文。 |
