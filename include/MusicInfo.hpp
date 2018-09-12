#ifndef MUSIC_INFO_HPP
#define MUSIC_INFO_HPP

#include<Siv3D.hpp>

class MusicInfo {
public:
  MusicInfo(String);

  String getTitle() const { return title; };
  String getArtist() const { return artist; };
  String getBpm() const { return bpm; };
  int getOffset() const { return offset; };
  std::vector<int> getPlayLevel() const { return playLevel; };
  

private:
  String title;
  String artist;
  String bpm;
  std::vector<int> playLevel; 
  int offset;

};

#endif // !MUSIC_INFO_HPP
