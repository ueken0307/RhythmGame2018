#ifndef RHYTHM_GAME_MANAGER_HPP
#define RHYTHM_GAME_MANAGER_HPP

#include "RhythmManager.hpp"

class Note {
public:
  Note(int, int, int);
  int count;
  int lane;
  int length;
};

class RhythmGameManager {
public:


private:
  RhythmManager rhythmManager;
  std::vector<Note> notes;


};

#endif // !RHYTHM_GAME_MANAGER_HPP