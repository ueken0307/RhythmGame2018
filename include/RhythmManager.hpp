#ifndef RHYTHM_MANAGER_HPP
#define RHYTHM_MANAGER_HPP

#include "TimeManager.hpp"

#include<vector>


class BpmData {
public:
  BpmData(int,double,int);
  int count;
  double bpm;
  int beat;
};

class RhythmManager {
public:
  RhythmManager() {};
  RhythmManager(std::vector<BpmData>&,double);
  void start();
  void update();
  int getBmsCount() const { return bmsCount; };
  double getSecond() const { return timer.nowSecond(offset); }
  double BtoS(int) const;

private:
  std::vector<BpmData> bpmDatas;
  std::vector<double> changeTimes;
  TimeManager timer;
  double offset;
  int bmsCount;
};

#endif // RHYTHM_MANAGER_HPP
