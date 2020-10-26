#include "TimerManager.h"
#include "Thread.h"
#include <unistd.h>
#include <iostream>
#include <memory>
using namespace std;

using namespace wd;

class MyTimerTask
    : public TimerTask {
 public:
  MyTimerTask(int seconds)
      : TimerTask(seconds) {}

  void process() {
    ::srand(::time(nullptr));
    int number = ::rand() % 100;
    cout << " >> number : " << number << endl;
  }
};

class PrintString
    : public TimerTask {
 public:
  PrintString(int seconds)
      : TimerTask(seconds) {}

  void process() {
    cout << "hello,world" << endl;
  }
};

void test0() {
  unique_ptr<TimerTask> timerTask1(new MyTimerTask(3));
  unique_ptr<TimerTask> timerTask2(new PrintString(5));
  TimerManager timer;
  timer.addTask(timerTask1.get());
  unique_ptr<Thread> thread(new Thread(
      std::bind(&TimerManager::start, &timer)));
  thread->start();
  //sleep(10);
  timer.addTask(timerTask2.get());
  cout<<111<<endl;
  sleep(20);
  timer.removeTask(timerTask2.get());

  sleep(10);
  timer.stop();
}

int main() {
  test0();
  return 0;
}
