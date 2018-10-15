#ifndef GAME_HPP
#define GAME_HPP

#include <Siv3D.hpp>
#include <HamFramework.hpp>

#include "ShareData.hpp"
#include "RhythmManager.hpp"
#include "RhythmGameManager.hpp"

class Game :public SceneManager<String, ShareData>::Scene {
public:
  void init() override;
  void update() override;
  void draw() const override;
  void drawFadeIn(double) const override;
  void drawFadeOut(double) const override;

private:
  RhythmManager rhythmManager;
  std::vector<NoteData> notes;
  bool startFlag;
};

#endif // GAME_HPP
