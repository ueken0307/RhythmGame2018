#include "MusicSelection.hpp"

void MusicSelection::init() {

}

void MusicSelection::update() {
  if (Input::KeyRight.clicked) {
    printf("Next scene is Game");
    changeScene(L"Game");
  }
}

void MusicSelection::draw() const {

}

void MusicSelection::drawFadeIn(double t) const {
  draw();
  Transformer2D tr(Graphics2D::GetTransform().inverse());
  Window::ClientRect().draw(ColorF(0.0).setAlpha(1.0 - t));
}

void MusicSelection::drawFadeOut(double t) const {
  draw();
  Transformer2D tr(Graphics2D::GetTransform().inverse());
  Window::ClientRect().draw(ColorF(0.0).setAlpha(t));
}