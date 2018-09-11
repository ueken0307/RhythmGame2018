#ifndef MUSIC_SELECTION_HPP
#define MUSIC_SELECTION_HPP

#include <Siv3D.hpp>
#include <HamFramework.hpp>

#include "ShareData.hpp"

class MusicSelection :public SceneManager<String, ShareData>::Scene {
public:
  void init() override;
  void update() override;
  void draw() const override;
  void drawFadeIn(double) const override;
  void drawFadeOut(double) const override;

private:

};

#endif // MUSIC_SELECTION_HPP
