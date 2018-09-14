#ifndef RHYTHM_MANAGER_HPP
#define RHYTHM_MANAGER_HPP

#include "TimeManager.hpp"

#include<vector>


class BpmData {
public:
  BpmData(int,double);
  int count;
  double bpm;
};

BpmData::BpmData(int count,double bpm) {
  this->count = count;
  this->bpm = bpm;
}

class RhythmManager {
public:
  RhythmManager(std::vector<BpmData>&,double);
  void start();
  void update();
  int getBmsCount() const { return bmsCount; };

private:
  std::vector<BpmData> bpmDatas;
  TimeManager timer;
  double offset;
  int bmsCount;
};

#endif // RHYTHM_MANAGER_HPP
