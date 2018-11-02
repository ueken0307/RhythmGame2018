#include"AdjustSpeed.hpp"


void AdjustSpeed::init() {

}

void AdjustSpeed::update() {
  if (Input::KeyEnter.clicked) {
    changeScene(L"Game");
  }

}

void AdjustSpeed::draw() const{

}

void AdjustSpeed::drawFadeIn(double t) const {
  draw();
  Transformer2D tr(Graphics2D::GetTransform().inverse());
  Window::ClientRect().draw(ColorF(0.0).setAlpha(1.0 - t));
}

void AdjustSpeed::drawFadeOut(double t) const {
  draw();
  Transformer2D tr(Graphics2D::GetTransform().inverse());
  Window::ClientRect().draw(ColorF(0.0).setAlpha(t));
}