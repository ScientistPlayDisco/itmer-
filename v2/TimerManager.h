// Authors: Handling
// Created by 60263 on 2020/10/24.
// This is 

#ifndef ITIMER__TIMERMANAGER_H_
#define ITIMER__TIMERMANAGER_H_
#include <list>
#include "TimerTask.h"


namespace wd {
class TimerManager {
 public:
  TimerManager();
  explicit TimerManager(TimerTask* timer_task);
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
  TimerTask* task_;

  std::list<TimerTask *> _taskList;

  //MutexLock _mutex;
  std::list<TimerTask *> _pendingTaskList;

  std::list<TimerTask *> _pendingRemoveList;
};
}

#endif //ITIMER__TIMERMANAGER_H_

