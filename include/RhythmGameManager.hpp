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
  RhythmGameManager() {};
  RhythmGameManager(std::vector<NoteData>,std::vector<BpmData>,double);
  void start(bool);
  void update();
  int getBmsCount() { return rhythmManager.getBmsCount(); };
  double getSecond() { return rhythmManager.getSecond(); };

private:
  RhythmManager rhythmManager;
  std::vector<NoteData> notes;
  bool isAuto;


};

#endif // !RHYTHM_GAME_MANAGER_HPP