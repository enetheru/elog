#ifndef ELOG_GENERAL_LOG_H_
#define ELOG_GENERAL_LOG_H_

#include <sstream>
#include <string>
#include "logger.h"
#include "logger_factory.h"

namespace LOG {

template <LogLevel LEVEL>
class GeneralLog {
 public:
  GeneralLog(const char* source_file_name,
             int line_number,
             Logger* logger = NULL)
      : logger_(logger ? *logger : GetLogger()),
        source_file_name_(source_file_name),
        line_number_(line_number) {
  }

  template <typename T>
  GeneralLog& operator<<(const T& t) {
    string_builder_ << t;
    return *this;
  }

  GeneralLog& GetReference() {
    return *this;
  }

  void PushMessage() const {
    const std::string& message = string_builder_.str();
    logger_.PushMessage(LEVEL, source_file_name_, line_number_, message);
  }

 private:
  Logger& logger_;
  std::ostringstream string_builder_;
  const char* source_file_name_;
  int line_number_;
};

template <>
void GeneralLog<FATAL>::PushMessage() const {
  const std::string& message = string_builder_.str();
  logger_.PushFatalMessageAndThrow(source_file_name_, line_number_, message);
}

template <>
void GeneralLog<CHECK>::PushMessage() const {
  const std::string& message = string_builder_.str();
  logger_.PushCheckMessageAndThrow(source_file_name_, line_number_, message);
}

}  // namespace LOG

#endif  // ELOG_GENERAL_LOG_H_
