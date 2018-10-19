#include<Siv3D.hpp>
#include<Windows.h>

#include"RhythmManager.hpp"
#include"NoteData.hpp"

void update();
void play();
void draw();
void drawEdit(int sX,int sY);
void drawPlay(int sX, int sY);

class EditNoteData {
public:
  EditNoteData() {};
  EditNoteData(int x, int y, int length) { this->x = x; this->y = y; this->length = length; };
  int x, y;
  int length;
};

class EditBpmData {
public:
  EditBpmData() {};
  EditBpmData(int y, double bpm, int beat) { this->y = y; this->bpm = bpm; this->beat = beat; };
  int y;
  double bpm;
  int beat;
};

class Measure {
public:
  std::vector<EditNoteData> notes;
  std::vector<EditBpmData> bpms;
};

std::vector<Measure> measures;

std::vector<std::vector<Rect>> clickRect;
std::vector<std::vector<Rect>> drawRect;
std::vector<std::vector<bool>> noteIsClicked;

int buttonWidth = 40;
int pedalWidth = 2 * buttonWidth;
int editNoteHeight = 20;
int editWidth = 2 * pedalWidth + 4 * buttonWidth;
int editHeight = 640;

int editStartX = 10;
int editStartY = 10;


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

  for (int i = 0; i < 32; ++i) {
    std::vector<Rect> tmpC;
    std::vector<Rect> tmpD;
    std::vector<bool>tmpFlag;

    int cY = editStartY + editNoteHeight * i ;
    int dY = editStartY + editNoteHeight * i + editNoteHeight / 2;

    for (int j = 0; j < 6; ++j){
      tmpFlag.push_back(false);
      if (j == 0) {
        tmpC.push_back(Rect(editStartX,cY, pedalWidth, editNoteHeight));
        tmpD.push_back(Rect(editStartX, dY, pedalWidth, editNoteHeight / 2));
      } else if (j == 5) {
        tmpC.push_back(Rect(editStartX + pedalWidth + 4 * buttonWidth, cY, pedalWidth, editNoteHeight));
        tmpD.push_back(Rect(editStartX + pedalWidth + 4 * buttonWidth, dY, pedalWidth, editNoteHeight / 2));
      } else {
        tmpC.push_back(Rect(editStartX + pedalWidth + (j - 1) * buttonWidth, cY, buttonWidth, editNoteHeight));
        tmpD.push_back(Rect(editStartX + pedalWidth + (j - 1) * buttonWidth, dY, buttonWidth, editNoteHeight / 2));
      }
    }

    clickRect.push_back(tmpC);
    drawRect.push_back(tmpD);
    noteIsClicked.push_back(tmpFlag);
  }
  
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
  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 6; ++j) {
      if (clickRect[i][j].leftClicked) {
        if (noteIsClicked[i][j]) {
          noteIsClicked[i][j] = false;
        } else {
          noteIsClicked[i][j] = true;
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
  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 6; ++j) {
      if (noteIsClicked[i][j]) {
        drawRect[i][j].draw(Color(0, 255, 0));
      }
    }
  }

  //topLine
  Line(sX, sY,sX + editWidth,sY).draw();
  //bottomLine
  Line(sX, sY + editHeight, sX + editWidth, sY + editHeight).draw();
  //leftLine
  Line(sX, sY, sX, sY + editHeight).draw();
  //rightLine
  Line(sX + editWidth, sY, sX + editWidth, sY + editHeight).draw();

  for (int i = 0; i < 5; ++i) {
    Line(sX + pedalWidth + i * buttonWidth, sY, sX + pedalWidth + i * buttonWidth, sY + editHeight).draw();
  }

  for (int i = 1; i < 32; ++i) {
    Line(sX, sY + i*editNoteHeight, sX + editWidth, sY + i*editNoteHeight).draw();
  }

}

void drawPlay(int sX, int sY) {

}