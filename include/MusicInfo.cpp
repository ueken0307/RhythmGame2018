#include "MusicInfo.hpp"

MusicInfo::MusicInfo(String title,String artist,String bpm,double offset,std::vector<int> playLevels) {
  this->title = title;
  this->artist = artist;
  this->bpm = bpm;
  this->offset = offset;

  for (const auto & i : playLevels) {
    this->playLevels.push_back(i);
  }

}