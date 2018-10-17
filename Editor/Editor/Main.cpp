#include<Siv3D.hpp>
#include<Windows.h>

#include"RhythmManager.hpp"
#include"NoteData.hpp"

void play();
void draw();
void drawEdit(int sX,int sY);
void drawPlay(int sX, int sY);


int buttonLWidth = 40;
int pedalWidth = 2 * buttonLWidth;
int height32 = 20;
int width = 2 * pedalWidth + 4 * buttonLWidth;
int height = 640;


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
  
  while (System::Update()){
    draw();
  }

#ifdef _DEBUG
  fclose(out);
  fclose(in);
  FreeConsole();
#endif // _DEBUG

}

void play() {

}

void draw() {
  drawEdit(10,10);
  drawPlay(0,0);
}

void drawEdit(int sX, int sY) {

  //topLine
  Line(sX, sY,sX + width,sY).draw();
  //bottomLine
  Line(sX, sY + height, sX + width, sY + height).draw();
  //leftLine
  Line(sX, sY, sX, sY + height).draw();
  //rightLine
  Line(sX + width, sY, sX + width, sY + height).draw();

  for (int i = 0; i < 5; ++i) {
    Line(sX + pedalWidth + i * buttonLWidth, sY, sX + pedalWidth + i * buttonLWidth, sY + height).draw();
  }

  for (int i = 1; i < 32; ++i) {
    Line(sX, sY + i*height32, sX + width, sY + i*height32).draw();
  }

}

void drawPlay(int sX, int sY) {

}