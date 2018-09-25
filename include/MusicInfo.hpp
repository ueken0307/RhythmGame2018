#ifndef MUSIC_INFO_HPP
#define MUSIC_INFO_HPP

#include<Siv3D.hpp>

class MusicInfo {
public:
  MusicInfo(String, String, String, double, std::vector<int>);

  String getTitle() const { return title; };
  String getArtist() const { return artist; };
  String getBpm() const { return bpm; };
  double getOffset() const { return offset; };
  std::vector<int> getPlayLevels() const { return playLevels; };
  

private:
  String title;
  String artist;
  String bpm;
  std::vector<int> playLevels; 
  double offset;

};

#endif // !MUSIC_INFO_HPP
