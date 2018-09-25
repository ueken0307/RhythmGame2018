#include "Result.hpp"

void Result::init() {

}

void Result::update() {
  if (Input::KeyRight.clicked) {
    printf("Next scene is MusicSelection\n");
    changeScene(L"MusicSelection");
  }
}

void Result::draw() const {

}

void Result::drawFadeIn(double t) const {
  draw();
  Transformer2D tr(Graphics2D::GetTransform().inverse());
  Window::ClientRect().draw(ColorF(0.0).setAlpha(1.0 - t));
}

void Result::drawFadeOut(double t) const {
  draw();
  Transformer2D tr(Graphics2D::GetTransform().inverse());
  Window::ClientRect().draw(ColorF(0.0).setAlpha(t));
}