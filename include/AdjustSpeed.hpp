#ifndef ADJUST_SPEED_HPP

#include <Siv3D.hpp>
#include <HamFramework.hpp>

#include"ShareData.hpp"

class AdjustSpeed :public SceneManager<String, ShareData>::Scene {
public:
  void init() override;
  void update() override;
  void draw() const override;
  void drawFadeIn(double t) const override;
  void drawFadeOut(double t) const override;

private:

};

#endif // !ADJUST_HPP
