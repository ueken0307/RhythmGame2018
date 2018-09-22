#include "RhythmManager.hpp"


BpmData::BpmData(int count, double bpm) {
  this->count = count;
  this->bpm = bpm;
}

RhythmManager::RhythmManager(std::vector<BpmData> &bpmDatas,double offset){
  for (auto& bpmData : bpmDatas) {
    this->bpmDatas.push_back(BpmData(bpmData.count, bpmData.bpm));
    this->changeTimes.push_back((60.0 / bpmData.bpm)*(bpmData.count / 2400.0));
  }

  this->offset = offset;
  this->bmsCount = 0;
}

void RhythmManager::start() {
  bmsCount = 0;
}

void RhythmManager::update() {
  //calc bmscount
}
