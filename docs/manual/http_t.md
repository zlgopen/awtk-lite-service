## http\_t
### 概述
 HTTP请求。




----------------------------------
### 函数
<p id="http_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#http_t_http_deinit">http\_deinit</a> | 全局~初始化，在应用程序结束时调用。 |
| <a href="#http_t_http_init">http\_init</a> | 全局初始化，在应用程序初始化时调用。 |
| <a href="#http_t_http_request">http\_request</a> | 启动一个lite service线程处理HTTP请求，在处理过程会把事件通过request->on_event通知调用者。 |
#### http\_deinit 函数
-----------------------

* 函数功能：

> <p id="http_t_http_deinit"> 全局~初始化，在应用程序结束时调用。





* 函数原型：

```
ret_t http_deinit ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### http\_init 函数
-----------------------

* 函数功能：

> <p id="http_t_http_init"> 全局初始化，在应用程序初始化时调用。





* 函数原型：

```
ret_t http_init ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### http\_request 函数
-----------------------

* 函数功能：

> <p id="http_t_http_request"> 启动一个lite service线程处理HTTP请求，在处理过程会把事件通过request->on_event通知调用者。

注意：

> * request->on_event在UI线程中执行，所以在该函数中可以操作GUI的控件。
> * request只能在request->response的fail标志或done标志为TRUE后才能销毁。
> * 可以通过http_request_abort取消请求，取消请求异步执行，可能会有一定延迟。
> * 本函数适合一般的REST API，不适合传输大文件，也不适合同时发送大量请求。






* 函数原型：

```
ret_t http_request (http_request_t* request);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| request | http\_request\_t* | HTTP请求对象。 |
