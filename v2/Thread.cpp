// Authors: Handling
// Created by 60263 on 2020/10/16.
// This is 

#include "Thread.h"
namespace wd {
namespace current_thread {
__thread const char *name;
}

using ThreadCallback = Thread::ThreadCallback;
struct ThreadData {
  std::string _name;
  ThreadCallback _cb;

  ThreadData(const std::string &name, ThreadCallback &cb)
      : _name(name),
        _cb(cb) {}

  void runInThread() {
    //在回调函数之前进行threadname的赋值
    //获取线程名
    current_thread::name = (_name == std::string()) ? "wd_thread" : _name.c_str();

    if (_cb)
      _cb();
  }

  //回调函数之后
};

Thread::Thread(ThreadCallback &&cb, const std::string &name) : _pthid(0),
                                                               _isRunning(false),
                                                               _cb(std::move(cb)),
                                                               _name(name) {}
Thread::~Thread() {
  if (_isRunning) {
    pthread_detach(_pthid);
  }
}

void Thread::start() {
  ThreadData *pdata = new ThreadData(_name, _cb);
  if (pthread_create(&_pthid, nullptr, threadFunc, pdata)) {
    perror("pthread_create");
    return;
  }
  _isRunning = true;
}

void Thread::join() {
  if (_isRunning) {
    pthread_join(_pthid, nullptr);
    //不设置会执行pthread_detach
    _isRunning = false;
  }
}

void *Thread::threadFunc(void *arg) {
  ThreadData *p = static_cast<ThreadData *>(arg);
  if (p) {

    p->runInThread();
  }
  delete p;
  return nullptr;
}
}