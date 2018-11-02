#include "MusicSelection.hpp"

#include <Windows.h>

void MusicSelection::init() {
  //---------get musics
  WIN32_FIND_DATA findFileData;
  HANDLE hFind;
  TCHAR *target = TEXT("Musics\\*");

  //-----LoadedInfos------
  hFind = FindFirstFile(target, &findFileData);
  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      //ignore parent folder & current folder
      if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0) {
        printf("FolderName:%ls\n", findFileData.cFileName);

        String name = findFileData.cFileName;
        JSONReader reader(L"Musics/" + name + L"/info.json");

        std::vector<int> tmpLevels;
        for (const auto & i : reader[L"playLevel"].getArray()) {
          tmpLevels.push_back(i.get<int32>());
        }
        infos.push_back(MusicInfo(name, reader[L"musicFileName"].get<String>(),reader[L"title"].get<String>(), reader[L"artist"].get<String>(),
          reader[L"bpm"].get<String>(), reader[L"offset"].get<double>(),tmpLevels, Texture(L"Musics/" + name + L"/" + reader[L"jacketFileName"].get<String>())));

      }
    } while (FindNextFile(hFind,&findFileData));
    FindClose(hFind);
  }
  //----------

  selectMusic = 0;
  selectLevel = 0;

  //前回の選曲を受け継ぐ
  if (m_data->folderName.length != 0) {
    for (int i = 0; i < infos.size();++i) {
      if (m_data->folderName == infos[i].getFolderName()) {
        selectMusic = i;
        selectLevel = m_data->levelNum;
      }
    }
  }

  f30 = Font(30);

  scrollY = EasingController<int>(0, 140, Easing::Quad, 200);
  scrollX = EasingController<int>(0, 25, Easing::Quad, 200);
  isUp = false;
}

void MusicSelection::update() {
  if (infos.size()) {
    if (Input::KeyEnter.clicked && infos[selectMusic].getPlayLevels()[selectLevel] != 0) {

      m_data->title = infos[selectMusic].getTitle();
      m_data->artist = infos[selectMusic].getArtist();
      m_data->folderName = infos[selectMusic].getFolderName();
      m_data->fileName = m_data->levelStrs[selectLevel];
      m_data->levelNum = selectLevel;
      m_data->musicFileName = infos[selectMusic].getMusicFileName();
      m_data->offset = infos[selectMusic].getOffset();
      m_data->startMeasure = 0;
      m_data->autoFlag = false;
      m_data->judgeOffset = 0.00;
      m_data->nextScene = L"Result";
      m_data->speed = 14;
      changeScene(L"AdjustSpeed");
    }

    if ((Input::KeyRight.clicked || Input::KeyL.clicked) && selectLevel < (static_cast<int>(infos[selectMusic].getPlayLevels().size()) - 1)) {
      selectLevel++;
    }

    if ((Input::KeyLeft.clicked || Input::KeyK.clicked) && 0 < selectLevel) {
      selectLevel--;
    }


    if (scrollY.isEnd()) {
      scrollY.reset();

      if (isUp) {
        selectMusic = (selectMusic + 1) % static_cast<int>(infos.size());
      }
      else {
        selectMusic = (selectMusic + static_cast<int>(infos.size()) - 1) % static_cast<int>(infos.size());
      }
    }

    if (scrollX.isEnd()) {
      scrollX.reset();
    }

    if (!scrollY.isActive()) {
      if (Input::KeyUp.pressed || Input::KeyS.pressed) {
        //selectMusic = (selectMusic + static_cast<int>(infos.size()) - 1) % static_cast<int>(infos.size());
        isUp = false;
        scrollX.start();
        scrollY.start();
      }

      if (Input::KeyDown.pressed || Input::KeyD.pressed) {
        //selectMusic = (selectMusic + 1) % static_cast<int>(infos.size());
        isUp = true;
        scrollX.start();
        scrollY.start();
      }
    }
  }

}

void MusicSelection::draw() const {

  for (int i = 0; i < 11; ++i) {
    if (isUp) {
      int x = 250 - i * 25 + scrollX.easeOut();
      int y = -210 + i * 140 + -scrollY.easeOut();
      Rect(x, y, 800, 100).draw();
      f30(L"タイトル :" + infos[(selectMusic + infos.size() + i - 5) % infos.size()].getTitle()).draw({ x,y },Color(0,0,0));
      f30(L"フォルダ名 :" + infos[(selectMusic + infos.size() + i - 5) % infos.size()].getFolderName()).draw({ x,y + 40 }, Color(0, 0, 0));

    }
    else {
      int x = 250 - i * 25 - scrollX.easeOut();
      int y = -210 + i * 140 + scrollY.easeOut();
      Rect(x, y, 800, 100).draw();
      f30(L"タイトル :" + infos[(selectMusic + infos.size() + i - 5) % infos.size()].getTitle()).draw({ x,y }, Color(0, 0, 0));
      f30(L"フォルダ名 :" + infos[(selectMusic + infos.size() + i - 5) % infos.size()].getFolderName()).draw({ x,y + 40 }, Color(0, 0, 0));
    }
  }

  int sX = 950;
  int sY = 1080/2-25;
  Triangle({sX,sY + 25}, {sX+50,sY}, {sX+50,sY+50}).draw(Color(255, 0, 0));

  //ジャケット
  Rect(1250, 50, 500)(infos[selectMusic].getTexture()).draw();
  //タイトル
  f30(infos[selectMusic].getTitle()).draw(1100, 600);
  //アーティスト
  f30(infos[selectMusic].getArtist()).draw(1100, 640);
  //bpm
  f30(infos[selectMusic].getBpm()).draw(1100, 680);

  for (int i = 0; i < 3; ++i) {
    Rect(1000 + i * 300, 850, 150).draw((selectLevel == i)? Color(255,0,0) : Color(255));
    f30(ToString(infos[selectMusic].getPlayLevels()[i])).draw({ 1000 + i * 300, 850 }, Color(0));
  }

}

void MusicSelection::drawFadeIn(double t) const {
  draw();
  Transformer2D tr(Graphics2D::GetTransform().inverse());
  Window::ClientRect().draw(ColorF(0.0).setAlpha(1.0 - t));
}

void MusicSelection::drawFadeOut(double t) const {
  draw();
  Transformer2D tr(Graphics2D::GetTransform().inverse());
  Window::ClientRect().draw(ColorF(0.0).setAlpha(t));
}