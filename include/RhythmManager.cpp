#include "RhythmManager.hpp"


BpmData::BpmData(int count, double bpm,int beat) {
  this->count = count;
  this->bpm = bpm;
  this->beat = beat;
}

RhythmManager::RhythmManager(std::vector<BpmData> &bpmDatas,double offset = 0.0){

  //BPM•Ï‰»•b‚ÌŽæ“¾
  this->bpmDatas.push_back(BpmData(bpmDatas[0].count, bpmDatas[0].bpm, bpmDatas[0].beat));
  this->changeTimes.push_back(0.0);

  for (int i = 1; i < bpmDatas.size(); ++i) {
    this->bpmDatas.push_back(BpmData(bpmDatas[i].count, bpmDatas[i].bpm, bpmDatas[i].beat));
    this->changeTimes.push_back((60.0 / bpmDatas[i-1].bpm)*((bpmDatas[i].count - bpmDatas[i-1].count) / (9600.0/bpmDatas[i-1].beat)) + changeTimes[i-1]);
  }

  this->offset = offset;
  this->bmsCount = 0;
}

double RhythmManager::BtoS(int count) const {
  int tmp;
  for (int i = 0; i < bpmDatas.size(); ++i) {
    if (bpmDatas[i].count <= count) {
      tmp = i;
    } else {
      break;
    }
  }
  return changeTimes[tmp] + ((count - bpmDatas[tmp].count)/(9600.0/bpmDatas[tmp].beat)) * (60.0/bpmDatas[tmp].bpm);
}

void RhythmManager::start() {
  bmsCount = 0;
  timer.start();
}

void RhythmManager::update() {
  timer.update();

  for (int i = 0; i < changeTimes.size(); ++i) {
    if (changeTimes[i] <= timer.nowSecond(offset) ) {
      bmsCount = bpmDatas[i].count + ((timer.nowSecond(offset) - changeTimes[i]) * (bpmDatas[i].bpm / 60.0) * (9600.0/bpmDatas[i].beat));
    }
  }
}
