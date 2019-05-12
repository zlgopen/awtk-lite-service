# awtk-lite-service

为[AWTK](https://github.com/zlgopen/awtk)开发的轻量级服务框架，以及常见服务的实现。

## 一、概述

在开发[AWTK](https://github.com/zlgopen/awtk)应用程序时，有时需要调用耗时很长的函数，如果在GUI线程直接调用，就会阻塞GUI线程，让界面无法刷新。这时我们需要创建一个线程，把这个函数放在线程中执行，让同步调用变成异步调用，等它执行完成再通知GUI线程更新界面。

多线程编程是件麻烦的事情，很容易出现错误，而且出现问题之后很难查找原因。lite-service的目的就是为了简化这个开发过程，它提供了一个轻量级的服务框架，可以方便的实现基于线程的服务，而调用者不需了解线程相关的东西，就可以把同步调用转换成异步调用。

除了服务框架外，我们也将提供一些基础的服务，如：异步化、HTTP网络请求、媒体播放器和其它一些常见服务。

> 请注意: 这里假设调用者是GUI线程，执行结果和事件会发给GUI线程，事件处理函数是在GUI线程执行的，可直接操作GUI控件。

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

## 二、服务框架

### 1. 服务的类型。

常见的服务有以下几种情况：

* 在后台线程执行一个函数，执行完成后，将结果返回给调用者。比如打开WIFI，打开完后通知调用者，调用者更新界面。

* 在后台线程执行一个函数，在执行的过程中，有进度信息通知调用者，有时调用者也取消执行。如HTTP请求就是这样，下载的进度信息需要通知调用者，让用户知道当前进度，用户也可以随时取消下载。

* 在后台线程执行一个函数，在执行的过程中，服务还可以接收调用者的请求。如媒体播放器，在播放的过程中可以接受暂停、前进和后退等操作。

服务可以完成就退出，也可以常驻内存，随时响应请求。

### 2. 调用者请求服务

* 通过初始化参数告诉服务要做的事情。

* 通过共享状态告诉服务要做的事情。

* 通过请求队列告诉服务要做的事情。

### 3. 服务通知调用者

服务通过idle\_queue函数，把事件/结果发送到GUI线程，在idle函数中，执行调用者提供的回调函数。所以在回调函数中可以直接操作GUI控件。

### 4. 基础组件

根据以上这些情况，lite service提供了一些基础组件，以简化开发过程。

* **请求队列。**调用者通过它发送请求，服务通过它分发请求。

* **服务基类。**提供了服务需要的基础功能，并定义了具体服务需要实现的接口。

* **服务线程。**服务线程的管理，目前没使用线程池，以后实际需要进行完善。


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

```
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

