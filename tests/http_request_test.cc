#include "gtest/gtest.h"
#include "http/http_header.h"
#include "http/http_request.h"
#include "tkc/utils.h"

#include <string>
using std::string;

static ret_t http_request_on_event_dummy(void* ctx, http_request_t* req, http_response_t* resp) {
  return RET_OK;
}

TEST(HttpRequest, get) {
  const char* url = "http://www.zlg.com";

  http_request_t* request = http_request_create_get(url, http_request_on_event_dummy, NULL);

  ASSERT_STREQ(HTTP_METHOD_GET, request->method);
  ASSERT_STREQ(url, request->url);
  ASSERT_EQ(request->header == NULL, TRUE);
  ASSERT_EQ(request->on_event == http_request_on_event_dummy, TRUE);
  ASSERT_EQ(request->on_event_ctx == NULL, TRUE);
  ASSERT_EQ(request->body == NULL, TRUE);
  ASSERT_EQ(request->body_size, 0);

  http_request_destroy(request);
}

TEST(HttpRequest, delete) {
  const char* url = "http://www.zlg.com/unused.html";

  http_request_t* request = http_request_create_delete(url, http_request_on_event_dummy, NULL);

  ASSERT_STREQ(HTTP_METHOD_DELETE, request->method);
  ASSERT_STREQ(url, request->url);
  ASSERT_EQ(request->header == NULL, TRUE);
  ASSERT_EQ(request->on_event == http_request_on_event_dummy, TRUE);
  ASSERT_EQ(request->on_event_ctx == NULL, TRUE);
  ASSERT_EQ(request->body == NULL, TRUE);
  ASSERT_EQ(request->body_size, 0);

  http_request_destroy(request);
}

TEST(HttpRequest, put) {
  const char* url = "http://www.zlg.com/put.html";
  const char* data = "hello awtk";
  const char* type = "text/plain";
  uint32_t data_size = strlen(data) + 1;
  http_request_t* request =
      http_request_create_put(url, http_request_on_event_dummy, NULL, type, data, data_size);

  ASSERT_STREQ(HTTP_METHOD_PUT, request->method);
  ASSERT_STREQ(url, request->url);
  ASSERT_STREQ(http_header_find(request->header, "Content-Type"), type);
  ASSERT_EQ(request->on_event == http_request_on_event_dummy, TRUE);
  ASSERT_EQ(request->on_event_ctx == NULL, TRUE);
  ASSERT_STREQ((char*)(request->body), data);
  ASSERT_EQ(request->body_size, data_size);

  http_request_destroy(request);
}

TEST(HttpRequest, post) {
  const char* url = "http://www.zlg.com/post.html";
  const char* data = "hello awtk";
  const char* type = "text/plain";
  uint32_t data_size = strlen(data) + 1;
  http_request_t* request =
      http_request_create_post(url, http_request_on_event_dummy, NULL, type, data, data_size);

  ASSERT_STREQ(HTTP_METHOD_POST, request->method);
  ASSERT_STREQ(url, request->url);
  ASSERT_STREQ(http_header_find(request->header, "Content-Type"), type);
  ASSERT_EQ(request->on_event == http_request_on_event_dummy, TRUE);
  ASSERT_EQ(request->on_event_ctx == NULL, TRUE);
  ASSERT_STREQ((char*)(request->body), data);
  ASSERT_EQ(request->body_size, data_size);

  http_request_destroy(request);
}
