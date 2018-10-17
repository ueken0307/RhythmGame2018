#include"NoteData.hpp"

NoteData::NoteData(int count, double second, int lane, int length, bool isEndEffect) {
  this->count = count;
  this->second = second;
  this->lane = lane;
  this->length = length;
  this->isEndEffect = isEndEffect;
}