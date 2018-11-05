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
  void drawFadeIn(double t) const override;
  void drawFadeOut(double t) const override;

private:
  Font f30,f60,f120;
  std::vector<Texture> judgePics;
};

#endif // RESULT_HPP
