#ifndef RHYTHM_GAME_MANAGER_HPP
#define RHYTHM_GAME_MANAGER_HPP

#include "RhythmManager.hpp"

class NoteData {
public:
  NoteData(int, int, int);
  int count;
  int lane;
  int length;
};

class RhythmGameManager {
public:


private:
  RhythmManager rhythmManager;
  std::vector<NoteData> notes;


};

#endif // !RHYTHM_GAME_MANAGER_HPP