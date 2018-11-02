#include "Result.hpp"

void Result::init() {
  f30 = Font(20);
  f60 = Font(60,L"Agency FB");
  f120 = Font(120, L"Comic Sans MS");

  for (int i = 0; i < 7; ++i) {
    judgePics.push_back(Texture(L"/21" + ToString(i)));
  }
}

void Result::update() {
  if (Input::KeyEnter.clicked) {
    changeScene(L"MusicSelection");
  }
}

void Result::draw() const {
  Polygon({ { 1400,0 },{1920,0},{1920,1080},{1920 / 2,1080} }).draw(Color(128));
  Line({ 1400,0 }, { 1920 / 2,1080 }).draw(5.0,Color(83, 255, 0));

  f120(ToString(m_data->score)).draw(1000 - f120(ToString(m_data->score)).region(0, 0).w, 200,Color(0, 217, 255));
  for (int i = 0; i < m_data->judgeCounts.size(); ++i) {
    f60(m_data->judgeStrs[i] + L":" + ToString(m_data->judgeCounts[i])).draw(100, 400 + f60(m_data->judgeStrs[i]).region(0,0).h * (i + 1), Color(0, 217, 255));
  }

  if (m_data->score == 1000000) {
    Rect(1400, 300, 400, 600)(judgePics[0]).draw();
  }
  else if (m_data->score >= 950000) {
    Rect(1400, 300, 400, 600)(judgePics[1]).draw();
  }
  else if (m_data->score >= 900000) {
    Rect(1400, 300, 400, 600)(judgePics[2]).draw();
  }
  else if (m_data->score >= 850000) {
    Rect(1400, 300, 400, 600)(judgePics[3]).draw();
  }
  else if (m_data->score >= 800000) {
    Rect(1400, 300, 400, 600)(judgePics[4]).draw();
  }
  else if (m_data->score >= 700000) {
    Rect(1400, 300, 400, 600)(judgePics[5]).draw();
  }
  else {
    Rect(1400, 300, 400, 600)(judgePics[6]).draw();
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