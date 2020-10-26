// Authors: Handling
// Created by 60263 on 2020/10/16.
// This is 

#ifndef DAY24__THREAD_H_
#define DAY24__THREAD_H_
#include "Noncopyable.h"
#include <pthread.h>
#include <functional>

namespace wd {
namespace current_thread {
extern __thread const char *name;
}

class Thread : public Noncopyable {
 public:
  using ThreadCallback = std::function<void()>;

  explicit Thread(ThreadCallback &&cb, const std::string &name = std::string());
  ~Thread();

  void start();
  void join();

  pthread_t getThreadId() const { return _pthid; }

  std::string getThreadName() const { return _name; }

 private:
  static void *threadFunc(void *arg);
  pthread_t _pthid;
  bool _isRunning;
  ThreadCallback _cb;
  std::string _name;
};
}
#endif //DAY24__THREAD_H_
