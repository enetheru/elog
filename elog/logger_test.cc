#include <sstream>
#include <string>
#include <gtest/gtest.h>
#include "logger.h"

namespace LOG {

namespace {

class PseudoOutputStream {
 public:
  PseudoOutputStream& operator<<(const char* str) {
    buffer_ += str;
    return *this;
  }

  const std::string& GetBuffer() const {
    return buffer_;
  }

 private:
  std::string buffer_;
};

}  // anonymous namespace

TEST(LoggerTest, LogLevelNames) {
  EXPECT_STREQ(log_level_names[INFO], "INFO");
  EXPECT_STREQ(log_level_names[WARN], "WARN");
  EXPECT_STREQ(log_level_names[ERROR], "ERROR");
  EXPECT_STREQ(log_level_names[FATAL], "FATAL");
}

TEST(LoggerTest, OutputLogLevelName) {
  PseudoOutputStream stream;
  OutputLogLevelName(INFO, stream);
  EXPECT_EQ("[INFO] ", stream.GetBuffer());
}

}  // namespace LOG
