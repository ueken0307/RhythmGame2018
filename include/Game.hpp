#ifndef GAME_HPP
#define GAME_HPP

#include <Siv3D.hpp>
#include <HamFramework.hpp>

#include "ShareData.hpp"
#include "RhythmManager.hpp"
#include "NoteData.hpp"

class Game :public SceneManager<String, ShareData>::Scene {
public:
  void init() override;
  void update() override;
  void draw() const override;
  void drawFadeIn(double t) const override;
  void drawFadeOut(double t) const override;

  void judge();
  int checkJudge(NoteData &note);
  void drawNotes() const;
private:
  RhythmManager rhythmManager;
  std::vector<NoteData> notes;
  std::vector<double> speedSec;
  int speed;
  bool startFlag;
  int wWidth;
  int wHeight;
  int pedalLaneWidth;
  int buttonLaneWidth;
  int allLaneWidth;
  int sideWidth;
  int judgeLineY;
  int noteHeight;
  int pedalHoldWidth;
  int buttonHoldWidth;

  std::array<int, 6> laneStartXs;
  std::array<int, 6> laneWidths;
  std::array<int, 6> holdWidths;

  double offsetSec;
  double beforeSec;
  double startSec;
  Sound music,tapSound;

  std::vector<double> judgeDurations;
  std::vector<String> judgeStrs;

  Effect effect;
};

#endif // GAME_HPP
