#include "gtest/gtest.h"
#include "lite_service/service_thread.h"
#include "lite_service/lite_service.h"

#include <string>
using std::string;

static string s_log;
static ret_t lite_service_on_request_log(lite_service_t* service, uint32_t cmd,
                                         uint32_t payload_size, const void* payload) {
  char buff[512];
  string& str = s_log;
  if (payload != NULL) {
    snprintf(buff, sizeof(buff) - 1, "%d %d %s", cmd, payload_size, (const char*)payload);
  } else {
    snprintf(buff, sizeof(buff) - 1, "%d %d ", cmd, payload_size);
  }

  str += buff;

  if (cmd == 0) {
    return RET_DONE;
  }

  return RET_OK;
}

TEST(LiteService, basic) {
  lite_service_vtable_t vt1;
  memset(&vt1, 0x00, sizeof(vt1));
  vt1.size = sizeof(lite_service_t);
  vt1.queue_size = 256;
  vt1.max_payload_size = 16;
  vt1.on_request = lite_service_on_request_log;

  lite_service_t* s = lite_service_create(&vt1, NULL);

  s_log = "";
  ASSERT_EQ(lite_service_request(s, 1234, 4, "abc"), RET_OK);
  ASSERT_EQ(lite_service_request(s, 1235, 4, "bcd"), RET_OK);
  ASSERT_EQ(lite_service_request(s, 0, 4, "bcd"), RET_OK);
  ASSERT_EQ(lite_service_run(s), RET_OK);
  ASSERT_STREQ(s_log.c_str(), "1234 4 abc1235 4 bcd0 4 bcd");

  lite_service_destroy(s);
}

TEST(LiteService, thread) {
  lite_service_vtable_t vt1;
  memset(&vt1, 0x00, sizeof(vt1));
  vt1.size = sizeof(lite_service_t);
  vt1.queue_size = 256;
  vt1.max_payload_size = 16;
  vt1.on_request = lite_service_on_request_log;

  tk_thread_t* t = service_thread_start(&vt1, NULL, NULL, NULL);

  s_log = "";
  ASSERT_EQ(service_thread_request(t, 1234, 4, "abc"), RET_OK);
  ASSERT_EQ(service_thread_request(t, 1235, 4, "bcd"), RET_OK);
  ASSERT_EQ(service_thread_request(t, 0, 4, "bcd"), RET_OK);

  tk_thread_join(t);

  ASSERT_STREQ(s_log.c_str(), "1234 4 abc1235 4 bcd0 4 bcd");
}
