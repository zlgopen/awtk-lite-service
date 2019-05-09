#include "gtest/gtest.h"
#include "http/http.h"
#include "base/idle.h"
#include "tkc/platform.h"
#include <string>
using std::string;

static ret_t http_request_on_event_dummy(void* ctx, http_request_t* req, http_response_t* resp) {
  if(resp->done) {
    *(bool_t*)ctx = TRUE;
  }

  return RET_OK;
}

TEST(HTTP, basic) {
  bool_t done = FALSE;
  const char* url = "http://www.zlg.com";
  http_request_t* request = http_request_create_get(url, http_request_on_event_dummy, &done);

  http_request(request);

  while(!done) {
    idle_dispatch();
    sleep_ms(100);
  }
}

