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
  beforeSec = (m_data->startMeasure == 0) ? 1.0 : 0.0;


  printf("selected -> %ls\n",m_data->folderName.c_str());
  printf("selected -> %ls\n", m_data->fileName.c_str());

  JSONReader reader(L"Musics/" + m_data->folderName + L"/" + m_data->fileName + L".json");

  //---------bpm---------
  std::vector<BpmData> bpms;
  for (const auto &i : reader[L"bpms"].getArray()) {
    bpms.push_back(BpmData(i[L"time"].get<int32>(), i[L"bpm"].get<double>(), i[L"beat"].get<int32>()));
  }

  rhythmManager = RhythmManager(bpms, -m_data->offset -beforeSec,m_data->startMeasure);

  //---------note--------
  m_data->noteNum = 0;
  for (const auto &i : reader[L"notes"].getArray()) {
    notes.push_back(NoteData(i[L"time"].get<int32>(),rhythmManager.BtoS(i[L"time"].get<int32>()), i[L"lane"].get<int32>(), i[L"length"].get<int32>()));
    m_data->noteNum++;
    if(i[L"length"].get<int32>()!=0) m_data->noteNum++;
  }

  //判定ごとのカウントを初期化
  for (auto &i : m_data->judgeCounts) {
    i = 0;
  }

  m_data->score = 0;

  startSec = rhythmManager.getStartSecond();

  //flags
  startFlag = false;

  //x,y,width,height
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

  laneStartXs = {sideWidth , sideWidth + pedalLaneWidth , sideWidth + pedalLaneWidth + buttonLaneWidth,
    sideWidth + pedalLaneWidth + buttonLaneWidth * 2, sideWidth + pedalLaneWidth + buttonLaneWidth * 3,
    sideWidth + pedalLaneWidth + buttonLaneWidth * 4};

  laneWidths = { pedalLaneWidth,buttonLaneWidth ,buttonLaneWidth ,buttonLaneWidth ,buttonLaneWidth ,pedalLaneWidth };

  holdWidths = { pedalHoldWidth,buttonHoldWidth ,buttonHoldWidth ,buttonHoldWidth ,buttonHoldWidth ,pedalHoldWidth };

  //sounds
  music = Sound(L"Musics/" + m_data->folderName + L"/" + m_data->musicFileName);
  music.setLoop(false);
  music.setVolume(0.5);
  music.setPosSec(startSec);

  tapSound = Sound(L"/200");
  tapSound.setLoop(false);
  tapSound.setVolume(1.0);
}

void Game::update() {
  if (Input::KeyEnter.clicked) {
    changeScene(m_data->nextScene);
  }

  if (!startFlag && (Input::KeySpace.clicked || Input::KeyS.clicked || Input::KeyD.clicked || Input::KeyK.clicked || Input::KeyL.clicked)) {
    startFlag = true;
    rhythmManager.start();
  }

  if (startFlag) {
    if (rhythmManager.getSecond() + m_data->offset - startSec > 0.0) {
      music.play();
    }

    rhythmManager.update();

    judge();

    double tmpScore = 0.0;
    double oneNotePoint = 1000000.0 / m_data->noteNum;
    for (int i = 0; i < m_data->judgePoints.size(); ++i) {
      tmpScore += m_data->judgePoints[i] * oneNotePoint * m_data->judgeCounts[i];
    }
    m_data->score = round(tmpScore);
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
            //長押しノーツなら、isLongFlagをオンにし、通常ノーツではisEndEffectをオンにする
            if (notes[j].length != 0) {
              notes[j].isLong = true;
            }
            else {
              notes[j].isEndEffect = true;
            }
            tapSound.stop();
            tapSound.play();

            m_data->judgeCounts[result]++;
            effect.add<JudgeEffect>(m_data->judgeStrs[result] +  L"(" + ToString(notes[j].second - rhythmManager.getSecond()) + L")",(result == 0)? Color(255,0,0):Color(0,255,0));
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
        if (rhythmManager.BtoS(notes[i].count + notes[i].length) - (rhythmManager.getSecond() + m_data->judgeOffset) < m_data->judgeDurations[m_data->judgeDurations.size() - 1]) {
          notes[i].isEndLong = true;
          m_data->judgeCounts[0]++;
          effect.add<JudgeEffect>(m_data->judgeStrs[0] + L"(Hold)",Color(255, 0, 0));
        }
      }
      else {
        //途中で離したとき
        notes[i].isEndEffect = true;
        m_data->judgeCounts.back()++;
        effect.add<JudgeEffect>(m_data->judgeStrs.back(), Color(0, 0, 255));
      }
    }
  }
  

  //通り過ぎて一番ゆるい判定範囲超えてるノーツをミスにする
  for (auto &i:notes) {
    if (!i.isEndEffect && !i.isLong && i.second - (rhythmManager.getSecond() + m_data->judgeOffset) < - m_data->judgeDurations.back()) {
      i.isEndEffect = true;
      m_data->judgeCounts.back()++;
      effect.add<JudgeEffect>(m_data->judgeStrs.back(), Color(0, 0, 255));

      if (i.length != 0) {
        //始点ミス判定
        m_data->judgeCounts.back()++;
      }

    }
  }
}

int Game::checkJudge(NoteData &note){
  double duration = fabs(rhythmManager.getSecond() + m_data->judgeOffset - note.second);

  for (int i = 0; i < m_data->judgeDurations.size(); ++i) {
    if (duration <= m_data->judgeDurations[i]) {
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
  //second〜
  for (int i = 0; i < 5; ++i) {
    Line(sideWidth + pedalLaneWidth + i * buttonLaneWidth, 0, sideWidth + pedalLaneWidth + i * buttonLaneWidth, wHeight).draw();
  }
  
  //judgeLine
  Line(sideWidth, judgeLineY, wWidth - sideWidth, judgeLineY).draw();
  
  effect.update();
}

void Game::drawNotes() const{

  Color normal(255,255,255), holdStart(0, 100, 200), holdEnd(0, 100, 200), hold(0, 100, 120),holdActive(0,160,180);

  for (const auto &i : notes) {
    //ノーツのy座標
    int y = static_cast<int>(judgeLineY - ((i.second - rhythmManager.getSecond())/ m_data->speedSec[m_data->speed])*judgeLineY);

    //まだ表示エリアに入っていないノーツは表示しない
    if (y < -noteHeight) {
      break;
    }

    //判定の終わってるノーツは表示しない
    if (i.length ==0 && i.isEndEffect) {
      continue;
    }

    //ノーツ描画
    if (i.length == 0) {
      Rect(laneStartXs[i.lane], y - noteHeight / 2, laneWidths[i.lane], noteHeight).draw(normal);
      
    } else {
      //長押し終点ノーツのy座標
      int endY = static_cast<int>(judgeLineY - ((rhythmManager.BtoS(i.count + i.length) - rhythmManager.getSecond()) / m_data->speedSec[m_data->speed])*judgeLineY);
      
      if (i.isEndLong && endY > judgeLineY) {
        continue;
      }

      if (endY < wHeight) {

        if (i.isLong && !i.isEndEffect) {
          //押している状態のノーツ

          //長押し部分
          Rect(laneStartXs[i.lane] + (laneWidths[i.lane] - holdWidths[i.lane]) / 2, endY + noteHeight / 2, holdWidths[i.lane], judgeLineY - (endY + noteHeight)).draw(holdActive);
          //始点
          Rect(laneStartXs[i.lane], judgeLineY - noteHeight / 2, laneWidths[i.lane], noteHeight).draw(holdStart);
          //終点
          Rect(laneStartXs[i.lane], endY - noteHeight / 2, laneWidths[i.lane], noteHeight).draw(holdEnd);
        }
        else {
          //長押し部分
          Rect(laneStartXs[i.lane] + (laneWidths[i.lane] - holdWidths[i.lane]) / 2, endY + noteHeight / 2, holdWidths[i.lane], y - (endY + noteHeight)).draw(hold);
          //始点
          Rect(laneStartXs[i.lane], y - noteHeight / 2, laneWidths[i.lane], noteHeight).draw(holdStart);
          //終点
          Rect(laneStartXs[i.lane], endY - noteHeight / 2, laneWidths[i.lane], noteHeight).draw(holdEnd);
        }
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