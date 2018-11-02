#include"AdjustSpeed.hpp"


void AdjustSpeed::init() {
  std::vector<BpmData> bpms;
  bpms.push_back(BpmData(0, 120.0, 4));
  if (m_data->speed < 0 || m_data->speedSec.size() <= m_data->speed) m_data->speed = 10;
  rhythmManager = RhythmManager(bpms, -m_data->speedSec[m_data->speed]);

  noteHeight = 10;
  judgeLineY = 1000;
  f30 = Font(30);

  rhythmManager.start();
}

void AdjustSpeed::update() {
  if (Input::KeyEnter.clicked) {
    changeScene(L"Game");
  }

  if (Input::KeyL.clicked || Input::KeyLeft.clicked) {
    if (m_data->speed < m_data->speedSec.size() - 1) m_data->speed++;
    std::vector<BpmData> bpms;
    bpms.push_back(BpmData(0, 120.0, 4));
    rhythmManager = RhythmManager(bpms, -m_data->speedSec[m_data->speed]);
    rhythmManager.start();

  }

  if (Input::KeyK.clicked || Input::KeyRight.clicked) {
    if (0 < m_data->speed) m_data->speed--;
    std::vector<BpmData> bpms;
    bpms.push_back(BpmData(0, 120.0, 4));
    rhythmManager = RhythmManager(bpms, -m_data->speedSec[m_data->speed]);
    rhythmManager.start();

  }

  rhythmManager.update();
}

void AdjustSpeed::draw() const{
  for (int i = 0; i < 5; ++i) {
    int y = static_cast<int>(judgeLineY - ((rhythmManager.BtoS(9600/4 * i) - rhythmManager.getSecond()) / m_data->speedSec[m_data->speed])*judgeLineY);
    Rect(Window::BaseWidth() / 2 - 30, y - noteHeight / 2, 60, noteHeight).draw();
  }

  f30(L"スピード:" + ToString(m_data->speed + 1)).draw(Window::BaseWidth() / 2 - 300, Window::BaseHeight() / 2);

  Line(Window::BaseWidth() / 2 - 30, 0, Window::BaseWidth() / 2 - 30, Window::BaseHeight()).draw();
  Line(Window::BaseWidth() / 2 + 30, 0, Window::BaseWidth() / 2 + 30, Window::BaseHeight()).draw();
  Line(Window::BaseWidth() / 2 - 30, 1000, Window::BaseWidth() / 2 + 30, 1000).draw();

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