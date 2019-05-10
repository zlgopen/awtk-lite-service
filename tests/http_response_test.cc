#include "gtest/gtest.h"
#include "http/http_header.h"
#include "http/http_response.h"
#include "tkc/utils.h"

#include <string>
using std::string;

TEST(HttpResponse, basic) {
  http_response_t* response = http_response_create();

  ASSERT_EQ(response->fail, FALSE);
  ASSERT_EQ(http_response_set_fail(response, TRUE), RET_OK);
  ASSERT_EQ(response->fail, TRUE);

  ASSERT_EQ(response->done, FALSE);
  ASSERT_EQ(http_response_set_done(response, TRUE), RET_OK);
  ASSERT_EQ(response->done, TRUE);

  ASSERT_EQ(response->downloaded_size, 0);
  ASSERT_EQ(http_response_set_downloaded_size(response, 100), RET_OK);
  ASSERT_EQ(response->downloaded_size, 100);

  ASSERT_EQ(response->uploaded_size, 0);
  ASSERT_EQ(http_response_set_uploaded_size(response, 128), RET_OK);
  ASSERT_EQ(response->uploaded_size, 128);

  ASSERT_EQ(response->status_code, 0);
  ASSERT_EQ(response->status_text == NULL, TRUE);
  ASSERT_EQ(http_response_set_status(response, 200, "OK"), RET_OK);
  ASSERT_EQ(response->status_code, 200);
  ASSERT_STREQ(response->status_text, "OK");

  ASSERT_EQ(response->body_size, 0);
  ASSERT_EQ(response->body == NULL, TRUE);
  ASSERT_EQ(http_response_set_body(response, "OK", 2), RET_OK);
  ASSERT_EQ(response->body_size, 2);
  ASSERT_STREQ((char*)(response->body), "OK");

  http_response_destroy(response);
}
