#include "MusicInfo.hpp"

MusicInfo::MusicInfo(String name) {
  JSONReader reader(L"Musics/"+ name + L"/info.json");

  title = reader[L"title"].get<String>();
  artist = reader[L"artist"].get<String>();
  bpm = reader[L"bpm"].get<String>();
  offset = reader[L"offset"].get<int32>();

  for (const auto & i : reader[L"playLevel"].getArray()) {
    playLevel.push_back(i.get<int32>());
  }

}