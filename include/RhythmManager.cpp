#include "RhythmManager.hpp"


BpmData::BpmData(int count, double bpm) {
  this->count = count;
  this->bpm = bpm;
}

RhythmManager::RhythmManager(std::vector<BpmData> &bpmDatas,double offset = 0.0){

  //BPM•Ï‰»•b‚ÌŽæ“¾
  this->bpmDatas.push_back(BpmData(bpmDatas[0].count, bpmDatas[0].bpm));
  this->changeTimes.push_back(0.0);

  for (int i = 1; i < bpmDatas.size(); ++i) {
    this->bpmDatas.push_back(BpmData(bpmDatas[i].count, bpmDatas[i].bpm));
    this->changeTimes.push_back((60.0 / bpmDatas[i-1].bpm)*((bpmDatas[i].count - bpmDatas[i-1].count) / 2400.0) + changeTimes[i-1]);

    printf("%lf\n",changeTimes[i]);
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
