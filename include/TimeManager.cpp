#include "TimeManager.hpp"

std::chrono::high_resolution_clock::time_point getTime() {
  return std::chrono::high_resolution_clock::now();
}

int diff(std::chrono::high_resolution_clock::time_point start,
  std::chrono::high_resolution_clock::time_point end) {
  std::chrono::microseconds diff = 
    std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  return static_cast<int>(diff.count());
}

TimeManager::TimeManager() {
  startTime = getTime();
  nowTime = getTime();
}

void TimeManager::start() {
  startTime = getTime();
  nowTime = getTime();
}

void TimeManager::update() {
  nowTime = getTime();
}

double TimeManager::nowSeconds() {
  return diff(startTime, nowTime)*0.000001;
}

int TimeManager::nowMicroSeconds() {
  return diff(startTime, nowTime);
}
