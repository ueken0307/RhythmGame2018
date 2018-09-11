#ifndef RESULT_HPP
#define RESULT_HPP

#include <Siv3D.hpp>
#include <HamFramework.hpp>

#include "ShareData.hpp"

class Result :public SceneManager<String, ShareData>::Scene {
public:
  void init() override;
  void update() override;
  void draw() const override;
  void drawFadeIn(double) const override;
  void drawFadeOut(double) const override;

private:

};

#endif // RESULT_HPP
