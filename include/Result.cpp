#include "Result.hpp"

void Result::init() {
  f30 = Font(20);
}

void Result::update() {
  if (Input::KeyEnter.clicked) {
    changeScene(L"MusicSelection");
  }
}

void Result::draw() const {
  f30(ToString(m_data->score)).draw(0, 0);
  for (int i = 0; i < m_data->judgeCounts.size(); ++i) {
    f30(m_data->judgeStrs[i] + L":" + ToString(m_data->judgeCounts[i])).draw(0, 40 * (i + 1));
  }

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