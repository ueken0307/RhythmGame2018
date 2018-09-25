#ifndef MUSIC_SELECTION_HPP
#define MUSIC_SELECTION_HPP

#include <Siv3D.hpp>
#include <HamFramework.hpp>

#include "ShareData.hpp"
#include "MusicInfo.hpp"

class MusicSelection :public SceneManager<String, ShareData>::Scene {
public:
  void init() override;
  void update() override;
  void draw() const override;
  void drawFadeIn(double) const override;
  void drawFadeOut(double) const override;

private:
  std::vector<MusicInfo> infos;
  int nowSelect;

};

#endif // MUSIC_SELECTION_HPP
