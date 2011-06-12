#ifndef ELOG_THREAD_WIN32_H_
#define ELOG_THREAD_WIN32_H_

#include <functional>
#include <process.h>
#include <windows.h>
#include "util.h"

namespace LOG {

class ScopedThreadHandle : NonCopyable {
 public:
  explicit ScopedThreadHandle(HANDLE handle = NULL)
      : handle_(handle) {
  }

  ~ScopedThreadHandle() {
    Reset();
  }

  HANDLE Get() const {
    return handle_;
  }

  void Reset(HANDLE handle = NULL) {
    if (handle_) {
      CloseHandle(handle_);
    }
    handle_ = handle;
  }

 private:
  HANDLE handle_;
};

class Thread : NonCopyable {
 public:
  Thread() {
  }

  template <typename ThreadBody>
  explicit Thread(ThreadBody thread_body)
      : thread_body_(thread_body) {
  }

  ~Thread() {
  }

  template <typename ThreadBody>
  void set_thread_body(ThreadBody thread_body) {
    thread_body_ = thread_body;
  }

  void Run() {
    HANDLE handle = reinterpret_cast<HANDLE>(
            _beginthreadex(NULL, 0, StaticThreadBody, NULL, 0, NULL));
    thread_handle_.Reset(handle);
  }

  void Detach() {
    // do nothing
  }

  void Join() {
    HANDLE handle = thread_handle_.Get();
    if (handle) {
      WaitForSingleObject(handle, INFINITE);
    }
  }

 private:
  static int WINAPI StaticThreadBody(LPVOID self_ptr) {
    Thread& self = *reinterpret_cast<Thread*>(self_ptr);
    self.thread_body_();
    return NULL;
  }

  ScopedThreadHandle thread_handle_;
  std::tr1::function<void ()> thread_body_;
};

}  // namespace LOG

#endif  // ELOG_THREAD_WIN32_H_