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

  double offsetSec;
  double beforeSec;
  Sound music,tapSound;
};

#endif // GAME_HPP
