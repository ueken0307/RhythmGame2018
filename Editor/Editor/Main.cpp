﻿#include<Siv3D.hpp>
#include<Windows.h>

#include"RhythmManager.hpp"
#include"NoteData.hpp"

void update();
void draw();
void drawEdit(int sX,int sY);

void import(String folderName,String fileName);
void saveFile(String folderName,String fimeName);

void genelateData(std::vector<BpmData> &bpms, std::vector<NoteData> &notes);

void generateIsClicked();
void nextMeasure();
void prevMeasure();
void jumpMeasure(int measure);

class EditNoteData {
public:
  EditNoteData() {};
  EditNoteData(int split, int y, int x, int longSplit = 0, int length = 0) {
    this->split = split; this->y = y; this->x = x; this->longSplit = longSplit; this->length = length; };
  int split;
  int x, y;
  int longSplit;
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
  std::vector<bool> isBpmClicked;
};

std::vector<Measure> measures;
int currentMeasure = 0;

EditRects edit32;
EditRects edit24;

std::vector<Rect> splitButtons;
std::vector<Texture> splitPics;
std::vector<Rect> moveButtons;
std::vector<Texture> movePics;
std::vector<Rect> noteTypeButtons;
std::vector<Texture> noteTypePics;
Rect importButton,saveButton;
Texture importPic,savePic;

int buttonWidth = 40;
int pedalWidth = 2 * buttonWidth;

int editStartX = 10;
int editStartY = 10;
int editWidth = 2 * pedalWidth + 4 * buttonWidth;
int editHeight = 96*6;
int editNoteHeight32 = editHeight / 32;
int editNoteHeight24 = editHeight / 24;

int laneWidths[] = { pedalWidth,buttonWidth,buttonWidth,buttonWidth,buttonWidth,pedalWidth };
int laneStartXs[] = { editStartX,
editStartX + pedalWidth,
editStartX + pedalWidth + buttonWidth * 1 ,
editStartX + pedalWidth + buttonWidth * 2 ,
editStartX + pedalWidth + buttonWidth * 3 ,
editStartX + pedalWidth + buttonWidth * 4 };

int split = 8;
int splitList[] = {4,8,16,32,3,6,12,24};
int splitStartX = editStartX + editWidth + 120;
int splitStartY = editStartY;

int moveStartX = (editWidth / 2) + editStartX - (40 / 2) * 3 - 10;
int moveStartY = editStartY + editHeight + 10;

int noteType = 0;
int noteTypeStartX = splitStartX;
int noteTypeStartY = splitStartY + 50 * 8 + 30;

bool jumpFlag = false;

bool longFlag = false;
int longX = 0, longY = 0, longSplit = 0;

bool bpmFlag = false;
int bpmY = 0;

bool importFlag = false;
bool saveFlag = false;

Font f10,f20;

GUI jumpGUI,longGUI,bpmGUI,importGUI,saveGUI;

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
  Window::Resize(600, 650);
  Graphics::SetBackground(Color(40));
  System::SetExitEvent(WindowEvent::CloseButton);
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
    edit32.isBpmClicked.push_back(false);
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
    edit24.isBpmClicked.push_back(false);
  }

  for (int i = 0; i < 8; ++i) {
    splitButtons.push_back(Rect(splitStartX, splitStartY + i * 50, 40));
    splitPics.push_back(Texture(L"/11" + ToString(i)));
  }

  for (int i = 0; i < 3; ++i) {
    moveButtons.push_back(Rect(moveStartX + i * 50, moveStartY, 40));
    movePics.push_back(Texture(L"/13" + ToString(i)));
  }

  for (int i = 0; i < 3; ++i) {
    noteTypeButtons.push_back(Rect(noteTypeStartX, noteTypeStartY + i * 50, 40));
    noteTypePics.push_back(Texture(L"/12" + ToString(i)));
  }
  
  importButton = Rect(splitStartX + 100, splitStartY, 40);
  importPic = Texture(L"/160");

  saveButton = Rect(splitStartX + 100,splitStartY + 50,40);
  savePic = Texture(L"/161");

  //first measure
  measures.resize(1);

  f20 = Font(20);
  f10 = Font(10);

  jumpGUI = GUI(GUIStyle::Default);
  jumpGUI.setTitle(L"移動先の小節");
  jumpGUI.addln(L"targetMeasure", GUITextField::Create(4));
  jumpGUI.add(L"ok", GUIButton::Create(L"移動"));
  jumpGUI.add(L"cancel", GUIButton::Create(L"キャンセル"));
  jumpGUI.setCenter(Point(Window::Width()/2,Window::Height()/2));
  jumpGUI.show(false);
  
  longGUI = GUI(GUIStyle::Default);
  longGUI.setTitle(L"長押しの長さ");
  longGUI.add(L"split", GUITextField::Create(3));
  longGUI.add(GUIText::Create(L"分で"));
  longGUI.add(L"length", GUITextField::Create(4));
  longGUI.addln(GUIText::Create(L"拍"));
  longGUI.add(L"ok", GUIButton::Create(L"決定"));
  longGUI.add(L"cancel", GUIButton::Create(L"キャンセル"));
  longGUI.setCenter(Point(Window::Width() / 2, Window::Height() / 2));
  longGUI.show(false);

  bpmGUI = GUI(GUIStyle::Default);
  bpmGUI.setTitle(L"BPM変化");
  bpmGUI.add(GUIText::Create(L"BPM"));
  bpmGUI.addln(L"bpm", GUITextField::Create(8));
  bpmGUI.add(GUIText::Create(L"拍"));
  bpmGUI.addln(L"beat", GUITextField::Create(2));
  bpmGUI.add(L"ok", GUIButton::Create(L"決定"));
  bpmGUI.add(L"cancel", GUIButton::Create(L"キャンセル"));
  bpmGUI.setCenter(Point(Window::Width() / 2, Window::Height() / 2));
  bpmGUI.show(false);

  importGUI = GUI(GUIStyle::Default);
  importGUI.setTitle(L"読み込み");
  importGUI.add(GUIText::Create(L"フォルダ名"));
  importGUI.addln(L"folderName", GUITextField::Create(20));
  importGUI.add(GUIText::Create(L"ファイル名"));
  importGUI.addln(L"fileName", GUITextField::Create(20));
  importGUI.add(L"ok", GUIButton::Create(L"決定"));
  importGUI.add(L"cancel", GUIButton::Create(L"キャンセル"));
  importGUI.setCenter(Point(Window::Width() / 2, Window::Height() / 2));
  importGUI.show(false);

  saveGUI = GUI(GUIStyle::Default);
  saveGUI.setTitle(L"保存");
  saveGUI.add(GUIText::Create(L"フォルダ名"));
  saveGUI.addln(L"folderName", GUITextField::Create(20));
  saveGUI.add(GUIText::Create(L"ファイル名"));
  saveGUI.addln(L"fileName", GUITextField::Create(20));
  saveGUI.add(L"ok", GUIButton::Create(L"決定"));
  saveGUI.add(L"cancel", GUIButton::Create(L"キャンセル"));
  saveGUI.setCenter(Point(Window::Width() / 2, Window::Height() / 2));
  saveGUI.show(false);

  while (System::Update()){
    if (jumpFlag) {
      if (jumpGUI.button(L"ok").pushed) {
        int num = Parse<int>(jumpGUI.textField(L"targetMeasure").text);
        if (num > 0) {
          jumpMeasure(num - 1);
        }
        jumpGUI.show(false);
        jumpFlag = false;
      }

      if (jumpGUI.button(L"cancel").pushed) {
        jumpGUI.show(false);
        jumpFlag = false;
      }
    }
    else if (longFlag) {
      //ロングノーツ入力時
      if (longGUI.button(L"ok").pushed) {
        int sp = Parse<int>(longGUI.textField(L"split").text);
        int len = Parse<int>(longGUI.textField(L"length").text);
        if (sp > 0 && len > 0) {
          measures[currentMeasure].notes.push_back(EditNoteData(longSplit,longY, longX,sp,len));
        } else {
          if (split % 3 == 0) {
            //isClickedを反転
            edit24.isClicked[longY * (24 / split)][longX] = !edit24.isClicked[longY * (24 / split)][longX];
          }
          else {
            //isClickedを反転
            edit32.isClicked[longY * (32 / split)][longX] = !edit32.isClicked[longY * (32 / split)][longX];
          }
        }
        longFlag = false;
        longGUI.show(false);
      }

      if (longGUI.button(L"cancel").pushed) {
        if (split % 3 == 0) {
          //isClickedを反転
          edit24.isClicked[longY * (24 / split)][longX] = !edit24.isClicked[longY * (24 / split)][longX];
        }
        else {
          //isClickedを反転
          edit32.isClicked[longY * (32 / split)][longX] = !edit32.isClicked[longY * (32 / split)][longX];
        }
        longFlag = false;
        longGUI.show(false);
      }
    }
    else if (bpmFlag) {
      //bpm変化入力時
      if (bpmGUI.button(L"ok").pushed) {
        double bpm = Parse<double>(bpmGUI.textField(L"bpm").text);
        int beat = Parse<int>(bpmGUI.textField(L"beat").text);
        if (bpm > 0 && beat > 0) {
          measures[currentMeasure].bpms.push_back(EditBpmData(split,bpmY,bpm,beat));
        }
        else {
          if (split % 3 == 0) {
            //isBpmClickedを反転
            edit24.isBpmClicked[bpmY * (24 / split)] = !edit24.isBpmClicked[bpmY * (24 / split)];
          }
          else {
            //isBpmClickedを反転
            edit32.isBpmClicked[bpmY * (32 / split)] = !edit32.isBpmClicked[bpmY * (32 / split)];
          }
        }
        bpmFlag = false;
        bpmGUI.show(false);
      }

      if (bpmGUI.button(L"cancel").pushed) {
        if (split % 3 == 0) {
          //isBpmClickedを反転
          edit24.isBpmClicked[bpmY * (24 / split)] = !edit24.isBpmClicked[bpmY * (24 / split)];
        }
        else {
          //isBpmClickedを反転
          edit32.isBpmClicked[bpmY * (32 / split)] = !edit32.isBpmClicked[bpmY * (32 / split)];
        }
        bpmFlag = false;
        bpmGUI.show(false);
      }

    }
    else if(importFlag) {
      if (importGUI.button(L"ok").pushed) {
        String folderName = importGUI.textField(L"folderName").text;
        String fileName = importGUI.textField(L"fileName").text;
        import(folderName, fileName);

        importFlag = false;
        importGUI.show(false);
      }

      if (importGUI.button(L"cancel").pushed) {
        importFlag = false;
        importGUI.show(false);
      }
    }
    else if (saveFlag) {
      if (saveGUI.button(L"ok").pushed) {
        String folderName = saveGUI.textField(L"folderName").text;
        String fileName = saveGUI.textField(L"fileName").text;
        saveFile(folderName, fileName);

        saveFlag = false;
        saveGUI.show(false);
      }

      if (saveGUI.button(L"cancel").pushed) {
        saveFlag = false;
        saveGUI.show(false);
      }
    }
    else {
      update();
    }
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
    jumpFlag = true;
  }

  for (int i = 0; i < 8; ++i) {
    if (splitButtons[i].leftClicked) {
      split = splitList[i];
    }
      
  }

  for (int i = 0; i < 3;++i) {
    if (noteTypeButtons[i].leftClicked) {
      noteType = i;
    }
  }

  if (importButton.leftClicked) {
    importGUI.show(true);
    importFlag = true;
  }

  if (saveButton.leftClicked) {
    saveGUI.show(true);
    saveFlag = true;
  }

  //ノーツ追加・削除処理
  if (noteType!=2) {
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
              if (noteType == 0) {
                cM.notes.push_back(EditNoteData(split, i / (24 / split), j));
              }
              else if (noteType == 1) {
                longSplit = split;
                longY = i / (24 / split);
                longX = j;
                longFlag = true;
                longGUI.show(true);
              }

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
              if (noteType == 0) {
                cM.notes.push_back(EditNoteData(split, i / (32 / split), j));
              }
              else if (noteType == 1) {
                longSplit = split;
                longY = i / (32 / split);
                longX = j;
                longFlag = true;
                longGUI.show(true);
              }

            }
            //isClickedを反転
            edit32.isClicked[index][j] = !edit32.isClicked[index][j];
          }
        }
      }
    }
  }
  
  //bpm変化追加・削除処理
  if (noteType == 2) {
    if (split % 3 == 0) {
      for (int i = 0; i < 24; ++i) {
        for (int j = 0; j < 6; ++j) {
          if (edit24.click[i][j].leftClicked) {
            int index = (i / (24 / split))*(24 / split);
            auto & cM = measures[currentMeasure];

            if (edit24.isBpmClicked[index]) {
              //対応するbpm変化を削除
              for (int k = 0; k < cM.bpms.size(); ++k) {
                auto &cB = cM.bpms[k];
                if (index == ((24 / cB.split)*cB.y)) {
                  cM.bpms.erase(cM.bpms.begin() + k);
                  break;
                }
              }
            }
            else {
              bpmY = i / (24 / split);
              bpmFlag = true;
              bpmGUI.show(true);
            }
            //isClickedを反転
            edit24.isBpmClicked[index] = !edit24.isBpmClicked[index];
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

            if (edit32.isBpmClicked[index]) {
              //対応するbpm変化を削除
              for (int k = 0; k < cM.bpms.size(); ++k) {
                auto &cB = cM.bpms[k];
                if (index == ((32 / cB.split)*cB.y)) {
                  cM.bpms.erase(cM.bpms.begin() + k);
                  break;
                }
              }
            }
            else {
              bpmY = i / (32 / split);
              bpmFlag = true;
              bpmGUI.show(true);
            }
            //isClickedを反転
            edit32.isBpmClicked[index] = !edit32.isBpmClicked[index];
          }
        }
      }
    }
  }
}

void draw() {
  drawEdit(editStartX,editStartY);
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

  for (int i = 0; i < 3; ++i) {
    if (i == noteType) {
      noteTypeButtons[i].draw(Color(150));
    }
    noteTypeButtons[i](noteTypePics[i]).draw();
  }

  importButton(importPic).draw();
  saveButton(savePic).draw();

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
  
  //長押しノーツ
  for (int i = 0; i <= currentMeasure; ++i) {
    for (int j = 0; j < measures[i].notes.size(); ++j) {
      auto &cN = measures[i].notes[j];
      if (cN.length && cN.longSplit) {
        int startBmsCount = i * 9600 + (9600.0 / cN.split)*cN.y;
        int longBmsCount = (9600 / cN.longSplit)*cN.length;
        //現在以前の長押しノーツの表示
        if (startBmsCount + longBmsCount > currentMeasure * 9600) {
          if (startBmsCount + longBmsCount - currentMeasure * 9600 <= 9600) {
            //現在の小節で終端があるノーツ

            int startY = editStartY + (editHeight - ((startBmsCount + longBmsCount - currentMeasure * 9600) / 9600.0) * 576);
            if (startBmsCount < currentMeasure * 9600) {
              //始点が現在の小節より前なら
              Rect(laneStartXs[cN.x], startY, laneWidths[cN.x], ((startBmsCount + longBmsCount - currentMeasure * 9600) / 9600.0) * 576).draw(ColorF(0, 0, 255, 0.5));
            }
            else {
              //始点が現在の小節なら
              Rect(laneStartXs[cN.x], startY, laneWidths[cN.x], (longBmsCount / 9600.0) * 576).draw(ColorF(0, 0, 255, 0.5));
            }


          }
          else {
            //現在の小節で終わらないノーツ
            if (startBmsCount < currentMeasure * 9600) {
              //始点が現在の小節より前なら
              Rect(laneStartXs[cN.x], editStartY, laneWidths[cN.x], editHeight).draw(ColorF(0, 0, 255, 0.5));
            }
            else {
              //始点が現在の小節なら
              Rect(laneStartXs[cN.x], editStartY, laneWidths[cN.x], editHeight - ((startBmsCount - currentMeasure * 9600) / 9600.0)*editHeight).draw(ColorF(0, 0, 255, 0.5));
            }
          }
        }
      }
    }
  }

  //bpm
  for (auto &i : measures[currentMeasure].bpms) {
    int bpmDrawOffsetX = 20;
    int bpmDrawOffsetY = -15;
    if (i.split % 3 == 0) {
      Circle(editStartX + editWidth, editStartY + editNoteHeight24 * (24 - (24/i.split) * i.y),5).draw(ColorF(255,255,0));
      f10(ToString(i.bpm) + L"(" + ToString(i.beat) + L")").draw(
        Point(editStartX + editWidth + bpmDrawOffsetX, editStartY + editNoteHeight24 * (24 - (24 / i.split) * i.y ) + bpmDrawOffsetY), ColorF(255, 255, 0));
    } else {
      Circle(editStartX + editWidth, editStartY + editNoteHeight32 * (32 - (32 / i.split) * i.y), 5).draw(ColorF(255, 255, 0));
      f10(ToString(i.bpm) + L"(" + ToString(i.beat) + L")").draw(
        Point(editStartX + editWidth + bpmDrawOffsetX, editStartY + editNoteHeight32 * (32 - (32 / i.split) * i.y ) + bpmDrawOffsetY), ColorF(255, 255, 0));
    }
  }

  //vertical line
  if (split % 3 == 0) {
    for (int i = 0; i < 24; i += (24 / split)) {
      Color color = i % (24 / 3) == 0 ? Color(255, 0, 0) : Color(255);
      Line(sX, sY + i*editNoteHeight24, sX + editWidth, sY + i*editNoteHeight24).draw(color);
    }
  } else {
    for (int i = 0; i < 32; i += (32 / split)) {
      Color color = i % (32 / 4) == 0 ? Color(255, 0, 0) : Color(255);
      Line(sX, sY + i*editNoteHeight32, sX + editWidth, sY + i*editNoteHeight32).draw(color);
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

  for (auto &i : edit24.isBpmClicked) {
    i = false;
  }

  for (auto &i : edit32.isBpmClicked) {
    i = false;
  }

  //notesから反映
  for (const auto &i : measures[currentMeasure].notes) {
    if (i.split % 3 == 0) {
      edit24.isClicked[i.y * (24/i.split)][i.x] = true;
    } else {
      edit32.isClicked[i.y * (32 / i.split)][i.x] = true;
    }
  }

  //bpmsから反映
  for (const auto &i : measures[currentMeasure].bpms) {
    if (i.split % 3 == 0) {
      edit24.isBpmClicked[i.y * (24 / i.split)] = true;
    }
    else {
      edit32.isBpmClicked[i.y * (32 / i.split)] = true;
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

void import(String folderName,String fileName) {
  currentMeasure = 0;

  JSONReader reader(L"Musics/" + folderName + L"/" + fileName + L".json");
  if (reader.isOpened()) {
    measures.clear();

    printf("---------bpm---------\n");
    for (const auto &i : reader[L"bpms"].getArray()) {
      int time = i[L"time"].get<int32>();

      if (measures.size() < (time / 9600) + 1) {
        measures.resize((time / 9600) + 1);
      }

      if ((time % 9600) % (9600 / 24) == 0 && (time % 9600) % (9600 / 32) != 0) {
        measures[time  / 9600].bpms.push_back(
          EditBpmData(24, (time % 9600) / (9600 / 24), i[L"bpm"].get<double>(), i[L"beat"].get<int32>()));
        printf("24,%d,%lf,%d\n", (time % 9600) / (9600 / 24), i[L"bpm"].get<double>(), i[L"beat"].get<int32>());
      } else {
        measures[time / 9600].bpms.push_back(
          EditBpmData(32, (time % 9600) / (9600 / 32), i[L"bpm"].get<double>(), i[L"beat"].get<int32>()));
        printf("32,%d,%lf,%d\n", (time % 9600) / (9600 / 32), i[L"bpm"].get<double>(), i[L"beat"].get<int32>());
      } 
    }
    printf("---------note--------\n");
    for (const auto &i : reader[L"notes"].getArray()) {
      int time = i[L"time"].get<int32>();
      int length = i[L"length"].get<int32>();
      int lane = i[L"lane"].get<int32>();

      if (measures.size() < (time / 9600) + 1) {
        measures.resize((time / 9600) + 1);
      }

      if ((time % 9600) % (9600 / 24) == 0 && (time % 9600) % (9600 / 32) != 0) {
        if (length % (9600 / 24) == 0 && length % (9600 / 32) != 0) {
          measures[time / 9600].notes.push_back(
            EditNoteData(24, (time % 9600) / (9600 / 24), lane, 24, length / (9600 / 24)));
          printf("24,%d,(%d),%d,%d\n", (time % 9600) / (9600 / 24), lane, 24, length / (9600 / 24));
        } else {
          measures[time / 9600].notes.push_back(
            EditNoteData(24, (time % 9600) / (9600 / 24), lane, 32, length / (9600 / 32)));
          printf("24,%d,(%d),%d,%d\n", (time % 9600) / (9600 / 24), lane, 32, length / (9600 / 32));
        }        
      }
      else {
        if (length % (9600 / 24) == 0 && length % (9600 / 32) != 0) {
          measures[time / 9600].notes.push_back(
            EditNoteData(32, (time % 9600) / (9600 / 32), lane, 24, length / (9600 / 24)));
          printf("32,%d,(%d),%d,%d\n", (time % 9600) / (9600 / 32), lane, 24, length / (9600 / 24));
        }
        else {
          measures[time / 9600].notes.push_back(
            EditNoteData(32, (time % 9600) / (9600 / 32), lane, 32, length / (9600 / 32)));
          printf("32,%d,(%d),%d,%d\n", (time % 9600) / (9600 / 32), lane, 32, length / (9600 / 32));
        }
      }
    }
    generateIsClicked();
  }
}

void saveFile(String folderName, String fileName) {
  FILE *fp;
  String path = L"Musics/" + folderName + L"/" + fileName + L".json";
  fopen_s(&fp,path.narrow().c_str(),"w");

  if (fp != NULL) {
    auto indent = [=](int n) {for (int i = 0; i < n; ++i) { fprintf(fp, "  "); } };
    std::vector<BpmData> bpms;
    std::vector<NoteData> notes;
    genelateData(bpms, notes);

    fprintf_s(fp, "{\n");
    indent(1);
    fprintf_s(fp, "\"bpms\":[\n");
    for (int i = 0; i < bpms.size();++i){
      indent(2);
      fprintf_s(fp, "{\n");
      indent(3);
      fprintf_s(fp, "\"time\":%d,\n", bpms[i].count);
      indent(3);
      fprintf_s(fp, "\"bpm\":%lf,\n", bpms[i].bpm);
      indent(3);
      fprintf_s(fp, "\"beat\":%d\n", bpms[i].beat);
      indent(2);
      if (i != bpms.size() -1) {
        fprintf_s(fp, "},\n");
      } else {
        fprintf_s(fp, "}\n");
      }
    }
    indent(1);
    fprintf_s(fp, "],\n\n");

    indent(1);
    fprintf_s(fp, "\"notes\":[\n");
    for (int i = 0; i <notes.size(); ++i) {
      indent(2);
      fprintf_s(fp, "{\n");
      indent(3);
      fprintf_s(fp, "\"time\":%d,\n", notes[i].count);
      indent(3);
      fprintf_s(fp, "\"lane\":%d,\n", notes[i].lane);
      indent(3);
      fprintf_s(fp, "\"length\":%d\n", notes[i].length);
      indent(2);
      if (i != notes.size() - 1) {
        fprintf_s(fp, "},\n");
      }
      else {
        fprintf_s(fp, "}\n");
      }
    }
    indent(1);
    fprintf_s(fp, "]\n");
    fprintf_s(fp, "}");
    fclose(fp);
  }
  else {
    printf("Can not open\n");
  }
}

void genelateData(std::vector<BpmData> &bpms,std::vector<NoteData> &notes) {
  for (int i = 0; i < measures.size(); ++i) {
    for (const auto &j : measures[i].bpms) {
      int count = i * 9600 + (9600 / j.split) * j.y;
      bpms.push_back(BpmData(count,j.bpm,j.beat));
    }
  }
  if (bpms.size()) {
    RhythmManager rm(bpms, 0.0);

    for (int i = 0; i < measures.size(); ++i) {
      for (const auto &j : measures[i].notes) {
        int count = i * 9600 + (9600 / j.split) * j.y;
        int length = 0;
        if (j.longSplit != 0 && j.length != 0) {
          length = (9600 / j.longSplit) * j.length;
        }
        notes.push_back(NoteData(count, rm.BtoS(count), j.x, length));
      }
    }

    std::sort(bpms.begin(), bpms.end(), [](const BpmData &left, const BpmData &right) {return left.count < right.count; });
    std::sort(notes.begin(), notes.end(), [](const NoteData &left, const NoteData &right) {return left.count < right.count; });
  }
}