#include "RhythmManager.hpp"

RhythmManager::RhythmManager():offset(0),bmsCount(0) {
  
}

void RhythmManager::start() {
  offset = 0;
  bmsCount = 0;
}

void RhythmManager::update() {
  //calc bmscount
}

void RhythmManager::setOffset(int offset) {
  this->offset = offset;
}
