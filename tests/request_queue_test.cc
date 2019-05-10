#include "gtest/gtest.h"
#include "lite_service/request_queue.h"

#include <string>
using std::string;

static ret_t request_queue_on_request_log(void* ctx, uint32_t cmd, uint32_t payload_size,
                                          const void* payload) {
  char buff[512];
  string& str = *(string*)ctx;
  if (payload_size > 0) {
    snprintf(buff, sizeof(buff) - 1, "%d %d %s", cmd, payload_size, (const char*)payload);
  } else {
    snprintf(buff, sizeof(buff) - 1, "%d %d ", cmd, payload_size);
  }

  str += buff;

  return RET_OK;
}

TEST(RequestQueue, basic) {
  string log;
  request_queue_t* q = request_queue_create(256, 16, request_queue_on_request_log, &log);
  ASSERT_EQ(q->r, 0);
  ASSERT_EQ(q->w, 0);
  ASSERT_EQ(q->size, 256);
  ASSERT_EQ(q->full, FALSE);
  ASSERT_EQ(q->max_payload_size, 16);

  ASSERT_EQ(request_queue_get_available_data(q), 0);
  ASSERT_EQ(request_queue_get_available_space(q), q->size);

  request_queue_destroy(q);
}

TEST(RequestQueue, send_fail) {
  string log;
  const char* str = "1234567890qwertyuiopasdfghjkllllcxsdrgsrtss";
  request_queue_t* q = request_queue_create(256, 16, request_queue_on_request_log, &log);

  ASSERT_NE(request_queue_send(q, 1, 17, (const uint8_t*)str), RET_OK);
  ASSERT_NE(request_queue_send(q, 1, 100, (const uint8_t*)str), RET_OK);

  request_queue_destroy(q);
}

TEST(RequestQueue, read_write) {
  string log;
  uint32_t i = 0;
  uint8_t c = i;
  uint32_t size = 256;
  request_queue_t* q = request_queue_create(size, 16, request_queue_on_request_log, &log);

  for (i = 0; i < size; i++) {
    c = i;
    ASSERT_EQ(request_queue_write_data(q, &c, 1), RET_OK);
    ASSERT_EQ(request_queue_get_available_data(q), i + 1);
    ASSERT_EQ(request_queue_get_available_space(q), q->size - i - 1);
  }

  ASSERT_NE(request_queue_write_data(q, &c, 1), RET_OK);

  for (i = 0; i < size; i++) {
    ASSERT_EQ(request_queue_read_data(q, &c, 1), RET_OK);
    ASSERT_EQ(c, (uint8_t)i);
    ASSERT_EQ(request_queue_get_available_space(q), i + 1);
    ASSERT_EQ(request_queue_get_available_data(q), q->size - i - 1);
  }

  ASSERT_NE(request_queue_read_data(q, &c, 1), RET_OK);

  for (i = 0; i < size; i++) {
    ASSERT_EQ(request_queue_write_data(q, &c, 1), RET_OK);
    ASSERT_EQ(request_queue_get_available_data(q), 1);
    ASSERT_EQ(request_queue_read_data(q, &c, 1), RET_OK);
    ASSERT_EQ(request_queue_get_available_space(q), q->size);
  }

  for (i = 0; i < size; i++) {
    ASSERT_EQ(request_queue_write_data(q, &c, 1), RET_OK);
  }
  ASSERT_EQ(request_queue_read_data(q, &c, 1), RET_OK);
  ASSERT_EQ(request_queue_read_data(q, &c, 1), RET_OK);
  ASSERT_EQ(request_queue_get_available_space(q), 2);
  ASSERT_EQ(request_queue_write_data(q, &c, 1), RET_OK);
  ASSERT_EQ(request_queue_get_available_space(q), 1);
  ASSERT_EQ(request_queue_write_data(q, &c, 1), RET_OK);
  ASSERT_EQ(request_queue_get_available_space(q), 0);
  ASSERT_NE(request_queue_write_data(q, &c, 1), RET_OK);

  request_queue_destroy(q);
}

TEST(RequestQueue, send_process) {
  string log;
  uint32_t size = 256;
  const char* str = "abcd";
  request_queue_t* q = request_queue_create(size, 16, request_queue_on_request_log, &log);
  ASSERT_EQ(request_queue_send(q, 1, 0, NULL), RET_OK);
  ASSERT_EQ(request_queue_get_available_data(q), 8);

  ASSERT_EQ(request_queue_process(q, 1), RET_OK);
  ASSERT_STREQ(log.c_str(), "1 0 ");

  ASSERT_EQ(request_queue_process(q, 1), RET_OK);
  ASSERT_STREQ(log.c_str(), "1 0 ");

  log = "";
  ASSERT_EQ(request_queue_send(q, 1234, strlen(str) + 1, str), RET_OK);
  ASSERT_EQ(request_queue_process(q, 1), RET_OK);
  ASSERT_STREQ(log.c_str(), "1234 5 abcd");

  log = "";
  ASSERT_EQ(request_queue_send(q, 1234, strlen(str) + 1, str), RET_OK);
  ASSERT_EQ(request_queue_send(q, 1234, strlen(str) + 1, str), RET_OK);
  ASSERT_EQ(request_queue_send(q, 1234, strlen(str) + 1, str), RET_OK);
  ASSERT_EQ(request_queue_send(q, 1234, strlen(str) + 1, str), RET_OK);

  log = "";
  ASSERT_EQ(request_queue_process(q, 1), RET_OK);
  ASSERT_STREQ(log.c_str(), "1234 5 abcd");

  log = "";
  ASSERT_EQ(request_queue_process(q, 3), RET_OK);
  ASSERT_STREQ(log.c_str(), "1234 5 abcd1234 5 abcd1234 5 abcd");

  request_queue_destroy(q);
}
