#ifndef TIME_MANAGER_HPP
#define TIME_MANAGER_HPP

#include <chrono>

class TimeManager {
public:
  TimeManager();
  void start();
  void update();
  double nowSecond(double) const;
  int nowMicroSecond(int) const;
  
private:
  std::chrono::high_resolution_clock::time_point startTime;
  std::chrono::high_resolution_clock::time_point nowTime;
};

#endif // TIME_MANAGER_HPP
