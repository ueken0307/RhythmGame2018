#include "Game.hpp"

void Game::init() {
  String levelStr[] = { L"easy",L"normal",L"hard" };

  std::vector<BpmData> bpms;

  printf("selected -> %ls\n",m_data->folderName.c_str());
  printf("selected -> %d\n", m_data->level);

  JSONReader reader(L"Musics/" + m_data->folderName + L"/" + levelStr[m_data->level] + L".json");

  printf("---------bpm---------\n");
  for (const auto &i : reader[L"bpms"].getArray()) {
    bpms.push_back(BpmData(i[L"time"].get<int32>(), i[L"bpm"].get<double>(), i[L"beat"].get<int32>()));
    printf("time:%8d  bpm:%lf\n", i[L"time"].get<int32>(), i[L"bpm"].get<double>());
  }

  printf("---------note--------\n");
  for (const auto &i : reader[L"notes"].getArray()) {
    notes.push_back(NoteData(i[L"time"].get<int32>(), i[L"lane"].get<int32>(), i[L"length"].get<int32>()));
    printf("time:%8d  lane:%3d  length:%8d\n", i[L"time"].get<int32>(), i[L"lane"].get<int32>(), i[L"length"].get<int32>());
  }

  startFlag = false;
  rhythmManager = RhythmManager(bpms,1.0);
  printf("second:%lf\n", rhythmManager.getSecond());
  printf("Bcount:%8d\n", rhythmManager.getBmsCount());
}

void Game::update() {
  if (Input::KeyEnter.clicked) {
    printf("Next scene is Result\n");
    changeScene(L"Result");
  }

  if (!startFlag && Input::KeySpace.clicked) {
    rhythmManager.start();
    startFlag = true;
  }

  if (startFlag) {
    rhythmManager.update();
    printf("second:%lf\n", rhythmManager.getSecond());
    printf("Bcount:%8d\n", rhythmManager.getBmsCount());
  }
  
}

void Game::draw() const {

}

void Game::drawFadeIn(double t) const {
  draw();
  Transformer2D tr(Graphics2D::GetTransform().inverse());
  Window::ClientRect().draw(ColorF(0.0).setAlpha(1.0 - t));
}

void Game::drawFadeOut(double t) const {
  draw();
  Transformer2D tr(Graphics2D::GetTransform().inverse());
  Window::ClientRect().draw(ColorF(0.0).setAlpha(t));
}