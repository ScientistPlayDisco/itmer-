// Authors: Handling
// Created by 60263 on 2020/10/24.
// This is 

#include "TimerManager.h"
#include <cstdint>
#include <sys/timerfd.h>
#include <poll.h>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <algorithm>
#include <iostream>
namespace wd {
TimerManager::TimerManager() : fd_(createTimerfd()),
                               isStarted_(false),
                               currentIndex_(0) {
  timerWheelList_.reserve(kSize);
  for (int kI = 0; kI < kSize; ++kI) {
    //timerWheelList_.emplace_back();
    timerWheelList_.push_back(list<TimerTask *>());
  }

}
//TimerManager::TimerManager() : fd_(createTimerfd()), isStarted_(false) {}
//int TimerManager::createTimerfd() {
//  int fd = ::timerfd_create(CLOCK_REALTIME, 0);
//  if (fd < 0)
//    perror("timerfd_create");
//  return fd;
//}
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

void TimerManager::stop() {
  setTimerfd(0, 0);
  if (isStarted_)
    isStarted_ = false;
}

void TimerManager::addTask(TimerTask *timerTask) {
  if (timerTask) {
    //根据seconds确定在哪一个时间槽slot
    int idx = (timerTask->getTimeInterval() + currentIndex_) % kSize;
    //放入时间槽
    timerWheelList_[idx].push_back(timerTask);
    //确定圈数
    int count = timerTask->getTimeInterval() / kSize;
    timerTask->setCount(count);
  }
}
void TimerManager::removeTask(TimerTask *timerTask) {
  if (timerTask) {
    for (auto &slot:timerWheelList_) {
      for (auto ptask: slot) {
        if (ptask == timerTask) {
          slot.remove(ptask);
          break;
        }
      }
    }
  }
}

void TimerManager::handleRead() {
  uint64_t howmany = 0;
  int ret = ::read(fd_, &howmany, sizeof(howmany));
  if (ret != sizeof(howmany)) {
    perror("read");
  }
}

//处理任务
void TimerManager::handleTask() {
  //当前环形索引++，
  ++currentIndex_;
  currentIndex_ %= kSize;

  list<TimerTask *> _cacheList;
  //取出当前应当处理的时间槽
  auto &slot = timerWheelList_[currentIndex_];

  for (auto iter = slot.begin(); iter != slot.end();) {
    //如果圈数已经小于等于零即表示该任务需要执行
    if ((*iter)->getCount() <= 0) {
      //取出
      auto tmp = *iter;
      _cacheList.push_back(tmp);
      std::cout<<222<<std::endl;
      iter = slot.erase(iter);
      //重新add
      addTask(tmp);
    } else {
      //
      (*iter)->decreaseCount();
      ++iter;
    }
  }
  //执行缓存队列中的任务
  for (auto ptask:_cacheList) {
    ptask->process();
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
int TimerManager::createTimerfd() {
  int fd = ::timerfd_create(CLOCK_REALTIME, 0);
  if (fd < 0) {
    perror("timerfd_create");
  }
  return fd;
}
}//end of namespace wdke