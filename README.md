# awtk-lite-service

为[AWTK](https://github.com/zlgopen/awtk)开发的常见服务。

## 一、概述

在开发[AWTK](https://github.com/zlgopen/awtk)应用程序时，有时需要调用耗时很长的函数，如果在GUI线程直接调用，就会阻塞GUI线程，让界面无法刷新。这时我们需要创建一个线程，把这个函数放在线程中执行，让同步调用变成异步调用，等它执行完成再通知GUI线程更新界面。


这里在tkc的action thread基础上，提供一些基础的服务，如：异步化、HTTP网络请求、媒体播放器和其它一些常见服务。


### 1.编译：

本项目依赖[AWTK](https://github.com/zlgopen/awtk)，请将[AWTK](https://github.com/zlgopen/awtk)取到同级目录，先编译[AWTK](https://github.com/zlgopen/awtk)，然后再编译本项目：

```
scons
```

### 2.运行Demo：

* 异步请求的demo

```
./bin/demo_async
```
* HTTP请求的demo

```
./bin/demo_http
```
> 测试HTTP之前，需要用nodejs启动本地HTTP服务：
>
> ```
> node server/index.js
> ```

## 二、框架

> 新版本用tkc中的action thread代替了旧版本的lite service。

## 三、async服务

async服务把一个同步函数转成一个异步函数。使用方法如下：

下面这个函数耗时3秒，在GUI线程执行就会让界面无法刷新，所以需要异步化处理。

```
static ret_t do_sth_take_time(void* ctx) {
  sleep_ms(3000);

  return RET_OK;
}
```

为了让它异步执行，我们定义一个接受结果的函数(可选，如不要执行结果，使用NULL即可)：

```
static ret_t on_do_sth_take_time_done(void* ctx, ret_t result) {

  return RET_OK;
}
```

然后调用async_call转换为异步调用：

```
async_call(do_sth_take_time, on_do_sth_take_time_done, widget);
```

> 具体用法请参考：demos/demo_async.c 

## 四、HTTP服务

目前实现了GET/POST/DELETE/PUT四种方法，可以满足常见的REST API调用。但不适合大文件传输和同时大量并发请求。

**接受事件的回调函数的原型：**

```
typedef ret_t (*http_request_on_event_t)(void* ctx, http_request_t* request, http_response_t* resp);
```

**发起请求的函数：**

```
ret_t http_request(http_request_t* request);
```

如：

```c
  request = http_request_create_get(url, on_http_event, widget);
  http_request(request);

```

以下几点值得注意：

* resp->done 为TRUE表示请求成功完成。
* resp->fail 为TRUE表示请求失败。
* resp->done 或者 resp->fail 为TRUE表示请求完成了，此时才可以释放request对象。
* resp->body 是返回的内容。
* resp->body_size 是返回的内容的长度。
* resp->status_code 是HTTP响应码。
* resp->header 是响应头。
* 设置request->abort标志来取消请求，取消请求是异步。

> 具体用法请参考：demos/demo_http.c 

## 五、媒体播放

基于ffmpeg实现媒体播放功能(TODO)

## 六、API文档

[API文档](docs/manual)

