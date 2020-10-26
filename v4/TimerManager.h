// Authors: Handling
// Created by 60263 on 2020/10/24.
// This is 

#ifndef ITIMER__TIMERMANAGER_H_
#define ITIMER__TIMERMANAGER_H_
#include "TimerTask.h"
#include "MutexLock.h"
#include <list>
#include <vector>
using std::vector;
using std::list;

namespace wd {
class TimerManager {
 public:
  //TimerManager();
  TimerManager();
  void start();
  void stop();

  void addTask(TimerTask * timerTask);
  void removeTask(TimerTask * timerTask);
 private:
  int createTimerfd();
  void setTimerfd(int initialTime, int periodicTime);
  void handleRead();
  void handleTask();
 private:
  int fd_;
  bool isStarted_;

  int currentIndex_;
  vector<list <TimerTask *>> timerWheelList_;

  static const int kSize = 10; //slot 数量
};
}

#endif //ITIMER__TIMERMANAGER_H_

