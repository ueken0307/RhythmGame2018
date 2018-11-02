#ifndef ADJUST_SPEED_HPP

#include <Siv3D.hpp>
#include <HamFramework.hpp>

#include"ShareData.hpp"
#include"RhythmManager.hpp"

class AdjustSpeed :public SceneManager<String, ShareData>::Scene {
public:
  void init() override;
  void update() override;
  void draw() const override;
  void drawFadeIn(double t) const override;
  void drawFadeOut(double t) const override;

private:
  RhythmManager rhythmManager;
  int judgeLineY;
  int noteHeight;

  Font f30;
};

#endif // !ADJUST_HPP
