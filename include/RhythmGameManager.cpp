#include"RhythmGameManager.hpp"

NoteData::NoteData(int count,int lane,int length) {
  this->count = count;
  this->lane = lane;
  this->length = length;
}

RhythmGameManager::RhythmGameManager(std::vector<NoteData> notes, std::vector<BpmData> bpms, double offset):rhythmManager(bpms,offset) {
  for (auto i : notes) {
    this->notes.push_back(i);
  }
}

void RhythmGameManager::start(bool isAuto = false) {
  this->isAuto = isAuto;
  rhythmManager.start();
}

void RhythmGameManager::update() {
  rhythmManager.update();
}