## service\_thread\_t
### 概述
 服务线程。




----------------------------------
### 函数
<p id="service_thread_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#service_thread_t_service_thread_request">service\_thread\_request</a> | 请求服务。 |
| <a href="#service_thread_t_service_thread_start">service\_thread\_start</a> | 启动服务线程。 |
#### service\_thread\_request 函数
-----------------------

* 函数功能：

> <p id="service_thread_t_service_thread_request"> 请求服务。





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

> <p id="service_thread_t_service_thread_start"> 启动服务线程。





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
