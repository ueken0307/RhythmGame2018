#include<Siv3D.hpp>
#include<Windows.h>

#include"RhythmManager.hpp"
#include"NoteData.hpp"

void update();
void play();
void draw();
void drawEdit(int sX,int sY);
void drawPlay(int sX, int sY);

void generateIsClicked();
void nextMeasure();
void prevMeasure();
void jumpMeasure(int measure);

class EditNoteData {
public:
  EditNoteData() {};
  EditNoteData(int split, int y, int x, int length) { this->split = split; this->y = y; this->x = x; this->length = length; };
  int split;
  int x, y;
  int length;
};

class EditBpmData {
public:
  EditBpmData() {};
  EditBpmData(int split,int y, double bpm, int beat) { this->split = split; this->y = y; this->bpm = bpm; this->beat = beat; };
  int split;
  int y;
  double bpm;
  int beat;
};

class Measure {
public:
  std::vector<EditNoteData> notes;
  std::vector<EditBpmData> bpms;
};

class EditRects {
public:
  std::vector<std::vector<Rect>> click;
  std::vector<std::vector<Rect>> draw;
  std::vector<std::vector<bool>> isClicked;
};

std::vector<Measure> measures;
int currentMeasure = 0;

EditRects edit32;
EditRects edit24;

std::vector<Rect> splitButtons;
std::vector<Texture> splitPics;
std::vector<Rect> moveButtons;
std::vector<Texture> movePics;

int buttonWidth = 40;
int pedalWidth = 2 * buttonWidth;

int editStartX = 10;
int editStartY = 10;
int editWidth = 2 * pedalWidth + 4 * buttonWidth;
int editHeight = 96*6;
int editNoteHeight32 = editHeight / 32;
int editNoteHeight24 = editHeight / 24;

int split = 8;
int splitList[] = {4,8,16,32,3,6,12,24};
int splitStartX = editStartX + editWidth + 10;
int splitStartY = editStartY;

int moveStartX = (editWidth / 2) + editStartX - (40 / 2) * 3 - 10;
int moveStartY = editStartY + editHeight + 10;

Font f20;

GUI jumpGUI;

void Main(){

#ifdef _DEBUG
  //デバッグ時のみコンソール出力を有効にする
  AllocConsole();
  FILE* out = 0;
  freopen_s(&out, "CON", "w", stdout);
  FILE* in = 0;
  freopen_s(&in, "CON", "r", stdin);
#endif // _DEBUG

  //----------
  Window::SetTitle(L"高専祭2018 譜面作成");
  Window::Resize(1200, 700);
  Graphics::SetBackground(Color(40));
  Graphics::SetVSyncEnabled(false);
  //----------

  for (int i = 32-1; i >= 0; --i) {
    std::vector<Rect> tmpC;
    std::vector<Rect> tmpD;
    std::vector<bool>tmpFlag;

    int cY = editStartY + editNoteHeight32 * i ;
    int dY = editStartY + editNoteHeight32 * i + editNoteHeight32 / 2;

    for (int j = 0; j < 6; ++j){
      tmpFlag.push_back(false);
      if (j == 0) {
        tmpC.push_back(Rect(editStartX,cY, pedalWidth, editNoteHeight32));
        tmpD.push_back(Rect(editStartX, dY, pedalWidth, editNoteHeight32 / 2));
      } else if (j == 5) {
        tmpC.push_back(Rect(editStartX + pedalWidth + 4 * buttonWidth, cY, pedalWidth, editNoteHeight32));
        tmpD.push_back(Rect(editStartX + pedalWidth + 4 * buttonWidth, dY, pedalWidth, editNoteHeight32 / 2));
      } else {
        tmpC.push_back(Rect(editStartX + pedalWidth + (j - 1) * buttonWidth, cY, buttonWidth, editNoteHeight32));
        tmpD.push_back(Rect(editStartX + pedalWidth + (j - 1) * buttonWidth, dY, buttonWidth, editNoteHeight32 / 2));
      }
    }

    edit32.click.push_back(tmpC);
    edit32.draw.push_back(tmpD);
    edit32.isClicked.push_back(tmpFlag);
  }

  for (int i = 24-1; i >= 0; --i) {
    std::vector<Rect> tmpC;
    std::vector<Rect> tmpD;
    std::vector<bool>tmpFlag;

    int cY = editStartY + editNoteHeight24 * i;
    int dY = editStartY + editNoteHeight24 * i + editNoteHeight24 / 2;

    for (int j = 0; j < 6; ++j) {
      tmpFlag.push_back(false);
      if (j == 0) {
        tmpC.push_back(Rect(editStartX, cY, pedalWidth, editNoteHeight24));
        tmpD.push_back(Rect(editStartX, dY, pedalWidth, editNoteHeight24 / 2));
      }
      else if (j == 5) {
        tmpC.push_back(Rect(editStartX + pedalWidth + 4 * buttonWidth, cY, pedalWidth, editNoteHeight24));
        tmpD.push_back(Rect(editStartX + pedalWidth + 4 * buttonWidth, dY, pedalWidth, editNoteHeight24 / 2));
      }
      else {
        tmpC.push_back(Rect(editStartX + pedalWidth + (j - 1) * buttonWidth, cY, buttonWidth, editNoteHeight24));
        tmpD.push_back(Rect(editStartX + pedalWidth + (j - 1) * buttonWidth, dY, buttonWidth, editNoteHeight24 / 2));
      }
    }

    edit24.click.push_back(tmpC);
    edit24.draw.push_back(tmpD);
    edit24.isClicked.push_back(tmpFlag);
  }

  for (int i = 0; i < 8; ++i) {
    splitButtons.push_back(Rect(splitStartX, splitStartY + i * 50, 40));
    splitPics.push_back(Texture(L"/11" + ToString(i)));
  }

  for (int i = 0; i < 3; ++i) {
    moveButtons.push_back(Rect(moveStartX + i * 50, moveStartY, 40));
    movePics.push_back(Texture(L"/13" + ToString(i)));
  }
  
  //first measure
  measures.resize(1);

  f20 = Font(20);

  jumpGUI = GUI(GUIStyle::Default);
  jumpGUI.setTitle(L"移動先の小節");
  jumpGUI.addln(L"targetMeasure", GUITextField::Create(4));
  jumpGUI.add(L"ok", GUIButton::Create(L"移動"));
  jumpGUI.add(L"cancel", GUIButton::Create(L"キャンセル"));
  jumpGUI.setCenter(Point(Window::Width()/2,Window::Height()/2));
  jumpGUI.show(false);

  while (System::Update()){
    update();
    draw();
  }

#ifdef _DEBUG
  fclose(out);
  fclose(in);
  FreeConsole();
#endif // _DEBUG

}

void update() {
  if (Input::KeyRight.clicked || moveButtons[1].leftClicked) {
    nextMeasure();
  }
  if (Input::KeyLeft.clicked  || moveButtons[0].leftClicked) {
    prevMeasure();
  }
  if (moveButtons[2].leftClicked) {
    jumpGUI.textField(L"targetMeasure").setText(ToString(currentMeasure + 1));
    jumpGUI.show(true);
  }

  if (jumpGUI.button(L"ok").pressed) {
    int num = Parse<int>(jumpGUI.textField(L"targetMeasure").text);
    if (num > 0) {
      jumpMeasure(num -1);
    }
    jumpGUI.show(false);
  }

  if (jumpGUI.button(L"cancel").pressed) {
    jumpGUI.show(false);
  }

  for (int i = 0; i < 8; ++i) {
    if (splitButtons[i].leftClicked) {
      split = splitList[i];
    }
      
  }

  //ノーツ追加・削除処理
  if (split % 3 == 0) {
    for (int i = 0; i < 24; ++i) {
      for (int j = 0; j < 6; ++j) {
        if (edit24.click[i][j].leftClicked) {
          int index = (i / (24 / split))*(24 / split);
          auto & cM = measures[currentMeasure];

          if (edit24.isClicked[index][j]) {
            //対応するノーツを削除
            for (int k = 0; k < cM.notes.size(); ++k) {
              auto &cN = cM.notes[k];
              if (cN.x == j && index == ((24 / cN.split)*cN.y)) {
                cM.notes.erase(cM.notes.begin() + k);
                break;
              }
            }
          }
          else {
            //ノーツ追加
            cM.notes.push_back(EditNoteData(split, i / (24 / split), j, 0));
          }
          //isClickedを反転
          edit24.isClicked[index][j] = !edit24.isClicked[index][j];
        }
      }
    }
  }
  else {
    for (int i = 0; i < 32; ++i) {
      for (int j = 0; j < 6; ++j) {
        if (edit32.click[i][j].leftClicked) {
          int index = (i / (32 / split))*(32 / split);
          auto & cM = measures[currentMeasure];

          if (edit32.isClicked[index][j]) {
            //対応するノーツを削除
            for (int k = 0; k < cM.notes.size(); ++k) {
              auto &cN = cM.notes[k];
              if (cN.x == j && index == ((32 / cN.split)*cN.y)) {
                cM.notes.erase(cM.notes.begin() + k);
                break;
              }
            }
          }
          else {
            //ノーツ追加
            cM.notes.push_back(EditNoteData(split, i / (32 / split), j, 0));
          }
          //isClickedを反転
          edit32.isClicked[index][j] = !edit32.isClicked[index][j];
        }
      }
    }
  }

}

void play() {

}

void draw() {
  drawEdit(editStartX,editStartY);
  drawPlay(0,0);
}

void drawEdit(int sX, int sY) {
  for (int i = 0; i < 8; ++i) {
    if (splitList[i] == split) {
      splitButtons[i].draw(Color(150));
    }
    splitButtons[i](splitPics[i]).draw();
  }

  for (int i = 0; i < 3; ++i) {
    moveButtons[i](movePics[i]).draw();
  }

  f20(currentMeasure + 1).draw(moveStartX - 80, moveStartY);

  for (int i = 0; i < 24; ++i) {
    for (int j = 0; j < 6; ++j) {
      if (edit24.isClicked[i][j]) {
        edit24.draw[i][j].draw(ColorF(0, 255, 0,0.5));
      }
    }
  }
  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 6; ++j) {
      if (edit32.isClicked[i][j]) {
        edit32.draw[i][j].draw(ColorF(255, 0, 0,0.5));
      }
    }
  }
  

  //topLine
  //Line(sX, sY,sX + editWidth,sY).draw();
  //bottomLine
  Line(sX, sY + editHeight, sX + editWidth, sY + editHeight).draw();
  //leftLine
  Line(sX, sY, sX, sY + editHeight).draw();
  //rightLine
  Line(sX + editWidth, sY, sX + editWidth, sY + editHeight).draw();

  for (int i = 0; i < 5; ++i) {
    Line(sX + pedalWidth + i * buttonWidth, sY, sX + pedalWidth + i * buttonWidth, sY + editHeight).draw();
  }

  if (split % 3 == 0) {
    for (int i = 0; i < 24; i += (24 / split)) {
      Line(sX, sY + i*editNoteHeight24, sX + editWidth, sY + i*editNoteHeight24).draw();
    }
  } else {
    for (int i = 0; i < 32; i += (32 / split)) {
      Line(sX, sY + i*editNoteHeight32, sX + editWidth, sY + i*editNoteHeight32).draw();
    }
  }
}

void drawPlay(int sX, int sY) {

}

void generateIsClicked() {
  //リセット処理
  for (auto &i : edit24.isClicked) {
    for (auto &j : i) {
      j = false;
    }
  }

  for (auto &i : edit32.isClicked) {
    for (auto &j : i) {
      j = false;
    }
  }

  //notesから反映
  for (const auto &i : measures[currentMeasure].notes) {
    if (i.split % 3 == 0) {
      edit24.isClicked[i.y * (24/i.split)][i.x] = true;
    } else {
      edit32.isClicked[i.y * (32 / i.split)][i.x] = true;
    }
  }
}

void nextMeasure() {
  currentMeasure++;
  if (currentMeasure >= measures.size()) {
    //次の小節が存在しないとき
    measures.resize(currentMeasure + 1);
  }
  //notesからisClickedに反映
  generateIsClicked();
}

void prevMeasure() {
  if (0 <= currentMeasure-1) {
    currentMeasure--;
    //notesからisClickedに反映
    generateIsClicked();
  }
}

void jumpMeasure(int measure) {
  currentMeasure = measure;
  if (currentMeasure >= measures.size()) {
    //ジャンプ先の小節が存在しないとき
    measures.resize(currentMeasure + 1);
  }
  //notesからisClickedに反映
  generateIsClicked();
}