#include "Game.hpp"

struct  JudgeEffect :IEffect {
  double time;
  String str;
  Font f20;
  Color color;

  JudgeEffect(String str,Color color) :f20(20), time(0.2),str(str),color(color) {}


  bool update(double t)override {
    f20(str).drawCenter({ Window::BaseWidth() / 2, Window::BaseHeight()/2 - t * 200 },color);

    return t < time;
  }
};

void Game::init() {
  for (int i = 20; i > 0; --i) {
    speedSec.push_back(0.3*i);
    //printf("%lf\n", speedSec[speedSec.size() - 1]);
  }
  //test
  speed = 18;

  beforeSec = (m_data->startMeasure == 0) ? 1.0 : 0.0;
  offsetSec = m_data->offset;

  std::vector<BpmData> bpms;

  printf("selected -> %ls\n",m_data->folderName.c_str());
  printf("selected -> %ls\n", m_data->fileName.c_str());

  JSONReader reader(L"Musics/" + m_data->folderName + L"/" + m_data->fileName + L".json");

  //printf("---------bpm---------\n");
  for (const auto &i : reader[L"bpms"].getArray()) {
    bpms.push_back(BpmData(i[L"time"].get<int32>(), i[L"bpm"].get<double>(), i[L"beat"].get<int32>()));
    //printf("time:%8d  bpm:%lf\n", i[L"time"].get<int32>(), i[L"bpm"].get<double>());
  }

  rhythmManager = RhythmManager(bpms, -offsetSec -beforeSec,m_data->startMeasure);

  //printf("---------note--------\n");
  for (const auto &i : reader[L"notes"].getArray()) {
    notes.push_back(NoteData(i[L"time"].get<int32>(),rhythmManager.BtoS(i[L"time"].get<int32>()), i[L"lane"].get<int32>(), i[L"length"].get<int32>()));
    //printf("time:%8d  lane:%3d  length:%8d second:%lf\n", i[L"time"].get<int32>(), i[L"lane"].get<int32>(), i[L"length"].get<int32>(), rhythmManager.BtoS(i[L"time"].get<int32>()));
  }

  startSec = rhythmManager.getStartSecond();

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

  judgeDurations.push_back((1.0 / 60.0) * 3.0);
  judgeStrs.push_back(L"Perfect");
  judgeDurations.push_back((1.0 / 60.0) * 5.0);
  judgeStrs.push_back(L"Good");

  music = Sound(L"Musics/" + m_data->folderName + L"/" + m_data->musicFileName);
  music.setLoop(false);
  music.setVolume(0.5);
  music.setPosSec(startSec);

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

  if (!startFlag && (Input::KeySpace.clicked || Input::KeyS.clicked || Input::KeyD.clicked || Input::KeyK.clicked || Input::KeyL.clicked)) {
    startFlag = true;
    rhythmManager.start();
  }

  if (startFlag) {
    if (rhythmManager.getSecond() + offsetSec - startSec > 0.0) {
      music.play();
    }

    rhythmManager.update();

    judge();
    //printf("second:%lf\n", rhythmManager.getSecond());
    //printf("Bcount:%8d\n", rhythmManager.getBmsCount());
  }

  //自動再生のとき
  if (m_data->autoFlag) {
    for (auto &i : notes) {
      if (!i.isEndEffect && i.second <= rhythmManager.getSecond()) {
        i.isEndEffect = true;
        tapSound.stop();
        tapSound.play();
      }
    }
  }
  
  
}

void Game::judge() {

  std::array<bool, 6> clickeds = { Input::KeyA.clicked, Input::KeyS.clicked ,Input::KeyD.clicked ,Input::KeyK.clicked ,Input::KeyL.clicked , Input::KeySemicolon.clicked };
  std::array<bool, 6> presseds = { Input::KeyA.pressed, Input::KeyS.pressed ,Input::KeyD.pressed ,Input::KeyK.pressed ,Input::KeyL.pressed , Input::KeySemicolon.pressed };

  
  //始点の処理
  for (int i = 0; i < clickeds.size(); ++i) {
    if (clickeds[i]) {
      for (int j = 0; j < notes.size(); ++j) {

        if (!notes[j].isEndEffect && !notes[j].isLong && notes[j].lane == i) {
          int result = checkJudge(notes[j]);

          if (result != -1) {
            printf("%s %lf\n", judgeStrs[result].narrow().c_str(), notes[j].second - rhythmManager.getSecond());
            //長押しノーツなら、isLongFlagをオンにし、通常ノーツではisEndEffectをオンにする
            if (notes[j].length != 0) {
              notes[j].isLong = true;
            }
            else {
              notes[j].isEndEffect = true;
            }
            tapSound.stop();
            tapSound.play();

            effect.add<JudgeEffect>(judgeStrs[result] +  L"(" + ToString(notes[j].second - rhythmManager.getSecond()) + L")",(result == 0)? Color(255,0,0):Color(0,255,0));
          }
          
          break;
        }
      }
    }
  }

  //長押しノーツ処理
  for (int i = 0; i < notes.size(); ++i) {
    //判定が終わって無くて長押し状態のノーツ
    if (!notes[i].isEndEffect && !notes[i].isEndLong && notes[i].isLong) {
      if (presseds[notes[i].lane]) {
        if (rhythmManager.BtoS(notes[i].count + notes[i].length) - rhythmManager.getSecond() < judgeDurations[judgeDurations.size() - 1]) {
          notes[i].isEndLong = true;
          printf("%s (Hold)\n", judgeStrs[0].narrow().c_str());
          effect.add<JudgeEffect>(judgeStrs[0] + L"(Hold)",Color(255, 0, 0));
          tapSound.stop();
          tapSound.play();
        }
      }
      else {
        notes[i].isEndEffect = true;
        printf("Miss\n");
        effect.add<JudgeEffect>(L"Miss", Color(0, 0, 255));
      }


    }

  }
  

  //通り過ぎて一番ゆるい判定範囲超えてるノーツをミスにする
  for (auto &i:notes) {
    if (!i.isEndEffect && !i.isLong && i.second - rhythmManager.getSecond() < - judgeDurations[judgeDurations.size() - 1]) {
      printf("Miss\n");
      i.isEndEffect = true;

      effect.add<JudgeEffect>(L"Miss", Color(0, 0, 255));
    }
  }
}

int Game::checkJudge(NoteData &note){
  double duration = fabs(rhythmManager.getSecond() - note.second);

  for (int i = 0; i < judgeDurations.size(); ++i) {
    if (duration <= judgeDurations[i]) {
      return i;
    }
  }

  return -1;
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
  
  effect.update();
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

    //判定の終わってるノーツは表示しない
    if (i.isEndEffect) {
      continue;
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