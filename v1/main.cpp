#include "TimerManager.h"

#include <iostream>
#include <memory>
using namespace std;

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
void test0() {
  unique_ptr<TimerTask> timerTask(new MyTimerTask(3));
  wd::TimerManager timer(timerTask.get());
  timer.start();
  timer.stop();
}

int main() {
  test0();
  return 0;
}
