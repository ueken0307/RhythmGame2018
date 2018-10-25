#include "Game.hpp"

void Game::init() {
  for (int i = 20; i > 0; --i) {
    speedSec.push_back(0.3*i);
    printf("%lf\n", speedSec[speedSec.size() - 1]);
  }
  //test
  speed = 16;

  beforeSec = 1.0;
  offsetSec = m_data->offset;

  std::vector<BpmData> bpms;

  printf("selected -> %ls\n",m_data->folderName.c_str());
  printf("selected -> %ls\n", m_data->fileName.c_str());

  JSONReader reader(L"Musics/" + m_data->folderName + L"/" + m_data->fileName + L".json");

  printf("---------bpm---------\n");
  for (const auto &i : reader[L"bpms"].getArray()) {
    bpms.push_back(BpmData(i[L"time"].get<int32>(), i[L"bpm"].get<double>(), i[L"beat"].get<int32>()));
    printf("time:%8d  bpm:%lf\n", i[L"time"].get<int32>(), i[L"bpm"].get<double>());
  }

  rhythmManager = RhythmManager(bpms, m_data->offset + beforeSec);

  printf("---------note--------\n");
  for (const auto &i : reader[L"notes"].getArray()) {
    notes.push_back(NoteData(i[L"time"].get<int32>(),rhythmManager.BtoS(i[L"time"].get<int32>()), i[L"lane"].get<int32>(), i[L"length"].get<int32>()));
    printf("time:%8d  lane:%3d  length:%8d second:%lf\n", i[L"time"].get<int32>(), i[L"lane"].get<int32>(), i[L"length"].get<int32>(), rhythmManager.BtoS(i[L"time"].get<int32>()));
  }

  startSec = rhythmManager.BtoS(9600 * m_data->startMeasure);

  startFlag = false;

  wWidth = Window::BaseWidth();
  wHeight = Window::BaseHeight();
  pedalLaneWidth = 200;
  buttonLaneWidth = 100;
  allLaneWidth = pedalLaneWidth * 2 + buttonLaneWidth * 4;
  sideWidth = (wWidth - allLaneWidth) / 2;
  judgeLineY = 1000;
  noteHeight = 10;
  pedalHoldWidth = 100;
  buttonHoldWidth = 50;

  music = Sound(L"Musics/" + m_data->folderName + L"/" + m_data->musicFileName);
  music.setLoop(false);
  music.setVolume(0.5);

  tapSound = Sound(L"/200");
  tapSound.setLoop(false);
  tapSound.setVolume(1.0);

  printf("second:%lf\n", rhythmManager.getSecond());
  printf("Bcount:%8d\n", rhythmManager.getBmsCount());
}

void Game::update() {
  if (Input::KeyEnter.clicked) {
    String tmp = L"Next scene is" + m_data->nextScene + L"\n";
    printf(tmp.narrow().c_str());
    changeScene(m_data->nextScene);
  }

  if (!startFlag && Input::KeySpace.clicked) {
    startFlag = true;
    rhythmManager.start();
  }

  if (startFlag) {
    if (rhythmManager.getSecond() + offsetSec > 0.0) {
      music.play();
    }

    rhythmManager.update();
    //printf("second:%lf\n", rhythmManager.getSecond());
    //printf("Bcount:%8d\n", rhythmManager.getBmsCount());
  }

  for (auto &i : notes) {
    if (!i.isEndEffect && i.second <= rhythmManager.getSecond()) {
      i.isEndEffect = true;
      tapSound.stop();
      tapSound.play();
    }
  }
  
}

void Game::draw() const {

  drawNotes();

  //first
  Line(sideWidth, 0, sideWidth, wHeight).draw();
  //final
  Line(wWidth - sideWidth, 0, wWidth - sideWidth, wHeight).draw();
  //second～
  for (int i = 0; i < 5; ++i) {
    Line(sideWidth + pedalLaneWidth + i * buttonLaneWidth, 0, sideWidth + pedalLaneWidth + i * buttonLaneWidth, wHeight).draw();
  }
  
  //judgeLine
  Line(sideWidth, judgeLineY, wWidth - sideWidth, judgeLineY).draw();
  
  
}

void Game::drawNotes() const{

  ColorF normal(255,255,255), holdStart(0, 100, 200), holdEnd(0, 100, 200), hold(0, 160, 180);

  for (const auto &i : notes) {
    //ノーツのy座標
    int y = static_cast<int>(judgeLineY - ((i.second - rhythmManager.getSecond())/speedSec[speed])*judgeLineY);

    //まだ表示エリアに入っていないノーツは表示しない
    if (y < -noteHeight) {
      break;
    }

    //ノーツ描画
    if (i.length == 0) {
      //普通のノーツ
      if (i.lane == 0) {
        //左ペダル
        Rect(sideWidth, y - noteHeight / 2, pedalLaneWidth, noteHeight).draw(normal);
      } else if (i.lane == 5) {
        //右ペダル
        Rect(wWidth - sideWidth - pedalLaneWidth, y - noteHeight / 2, pedalLaneWidth, noteHeight).draw(normal);
      } else {
        //4鍵
        Rect(sideWidth + pedalLaneWidth + (i.lane - 1) * buttonLaneWidth, y - noteHeight / 2, buttonLaneWidth, noteHeight).draw(normal);
      }

    } else {
      //長押し終点ノーツのy座標
      int endY = static_cast<int>(judgeLineY - ((rhythmManager.BtoS(i.count + i.length) - rhythmManager.getSecond()) / speedSec[speed])*judgeLineY);

      if (i.lane == 0) { //左ペダル

        //長押し部分
        Rect(sideWidth + (pedalLaneWidth - pedalHoldWidth)/2, endY + noteHeight / 2, pedalHoldWidth, y - (endY + noteHeight)).draw(hold);
        //始点
        Rect(sideWidth, y - noteHeight / 2, pedalLaneWidth, noteHeight).draw(holdStart);
        //終点
        Rect(sideWidth, endY - noteHeight / 2, pedalLaneWidth, noteHeight).draw(holdEnd);

      } else if (i.lane == 5) {//右ペダル
        int startX = wWidth - sideWidth - pedalLaneWidth;

        //長押し部分
        Rect(startX + (pedalLaneWidth - pedalHoldWidth) / 2, endY + noteHeight / 2, pedalHoldWidth, y - (endY + noteHeight)).draw(hold);
        //始点
        Rect(startX, y - noteHeight / 2, pedalLaneWidth, noteHeight).draw(holdStart);
        //終点
        Rect(startX, endY - noteHeight / 2, pedalLaneWidth, noteHeight).draw(holdEnd);

      } else {//4鍵
        int startX = sideWidth + pedalLaneWidth + (i.lane - 1) * buttonLaneWidth;

        //長押し部分
        Rect(startX + (buttonLaneWidth - buttonHoldWidth)/2, endY + noteHeight / 2, buttonHoldWidth, y - (endY + noteHeight)).draw(hold);
        //始点
        Rect(startX, y - noteHeight / 2, buttonLaneWidth, noteHeight).draw(holdStart);
        //終点
        Rect(startX, endY - noteHeight / 2, buttonLaneWidth, noteHeight).draw(holdEnd);
      }
    }
  }
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