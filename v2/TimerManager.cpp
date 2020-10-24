// Authors: Handling
// Created by 60263 on 2020/10/24.
// This is 

#include "TimerManager.h"
#include "Thread.h"
#include <cstdint>
#include <sys/timerfd.h>
#include <poll.h>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <algorithm>
namespace wd {
TimerManager::TimerManager(TimerTask *timer_task) : fd_(createTimerfd()),
                                                    isStarted_(false),
                                                    task_(timer_task) {

}
TimerManager::TimerManager() :fd_(createTimerfd()),isStarted_(false){}
int TimerManager::createTimerfd() {
  int fd = ::timerfd_create(CLOCK_REALTIME, 0);
  if (fd < 0)
    perror("timerfd_create");
  return fd;
}
void TimerManager::start() {
  struct pollfd pfd;
  pfd.fd = fd_;
  pfd.events = POLLIN;
  //setTimerfd(1, task_->getTimeInterval());
  setTimerfd(1, 1);
  isStarted_ = true;
  while (isStarted_) {
    int nready = ::poll(&pfd, 1, 5000);
    if (nready == -1 && errno == EINTR)
      continue;
    else if (nready == -1) {
      perror("poll");
      return;
    } else if (nready == 0) {
      printf("poll timeout!\n");
    } else {
      handleRead();
      handleTask();
    }
  }
}

void TimerManager::setTimerfd(int initialTime, int periodicTime) {
  struct itimerspec value;
  value.it_value.tv_sec = initialTime;
  value.it_value.tv_nsec = 0;

  value.it_interval.tv_sec = periodicTime;
  value.it_interval.tv_nsec = 0;

  int ret = timerfd_settime(fd_, 0, &value, nullptr);
  if (ret < 0) {
    perror("timerfd_settime");
  }
}

void TimerManager::stop() {
  setTimerfd(0, 0);
  if (isStarted_)
    isStarted_ = false;
}

void TimerManager::handleRead() {
  uint64_t howmany = 0;
  int ret = ::read(fd_, &howmany, sizeof(howmany));
  if (ret != sizeof(howmany)) {
    perror("read");
  }
}
void TimerManager::addTask(TimerTask *timerTask) {
  if (timerTask)
    _taskList.push_back(timerTask);
}
void TimerManager::removeTask(TimerTask *timerTask) {
  if (timerTask) {
    auto iter = std::find(_taskList.begin(), _taskList.end(), timerTask);
    if (iter != _taskList.end()) {
      _taskList.erase(iter);
    }
  }
}
void TimerManager::handleTask() {
  for (TimerTask *ptask : _taskList) {
    ptask->decreaseCount();
    if (ptask->getCount() <= 0) {
      ptask->process();
      ptask->initCount();
    }
  }

  //...删除任务

  //...添加新的任务
}

}//end of namespace wdke