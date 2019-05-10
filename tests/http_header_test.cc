#include "gtest/gtest.h"
#include "http/http_header.h"
#include "tkc/utils.h"

#include <string>
using std::string;

TEST(HttpHeader, basic) {
  const char* key = "Content-Type";
  const char* value = "text/plain";

  http_header_t* header = http_header_create(key, value);

  ASSERT_STREQ(key, header->key);
  ASSERT_STREQ(value, header->value);

  http_header_destroy(header);
}

TEST(HttpHeader, list) {
  char key[32];
  char value[32];
  uint32_t i = 0;
  uint32_t n = 100;
  http_header_t* header = NULL;

  for (i = 0; i < n; i++) {
    tk_snprintf(key, sizeof(key), "k%d", i);
    tk_snprintf(value, sizeof(value), "v%d", i);

    header = http_header_prepend(header, key, value);
    ASSERT_STREQ(key, header->key);
    ASSERT_STREQ(value, header->value);
  }

  for (i = 0; i < n; i++) {
    tk_snprintf(key, sizeof(key), "k%d", i);
    tk_snprintf(value, sizeof(value), "v%d", i);

    ASSERT_STREQ(value, http_header_find(header, key));
  }

  http_header_destroy(header);
}
