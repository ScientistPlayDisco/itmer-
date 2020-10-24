// Authors: Handling
// Created by 60263 on 2020/10/24.
// This is 

#ifndef ITIMER__TIMERMANAGER_H_
#define ITIMER__TIMERMANAGER_H_
#include "TimerTask.h"


namespace wd {
class TimerManager {
 public:
  explicit TimerManager(TimerTask* timer_task);
  void start();
  void stop();
 private:
  int createTimerfd();
  void setTimerfd(int initialTime, int periodicTime);
  void handleRead();
 private:
  int fd_;
  bool isStarted_;
  TimerTask* task_;
};
}

#endif //ITIMER__TIMERMANAGER_H_

