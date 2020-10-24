// Authors: Handling
// Created by 60263 on 2020/10/24.
// This is 

#include "TimerManager.h"

#include <stdint.h>
#include <sys/timerfd.h>
#include <poll.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
namespace wd {
TimerManager::TimerManager(TimerTask *timer_task) : fd_(createTimerfd()),
                                                    isStarted_(false),
                                                    task_(timer_task) {

}
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
  setTimerfd(1, task_->getTimeInterval());
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
      if (task_)
        task_->process();
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

}//end of namespace wd