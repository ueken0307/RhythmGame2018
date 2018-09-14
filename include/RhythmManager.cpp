#include "RhythmManager.hpp"

RhythmManager::RhythmManager(std::vector<BpmData> &bpmDatas,double offset){
  for (auto& bpmData : bpmDatas) {
    this->bpmDatas.push_back(BpmData(bpmData.count, bpmData.bpm));
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
