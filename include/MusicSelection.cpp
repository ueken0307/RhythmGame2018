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
          reader[L"bpm"].get<String>(), reader[L"offset"].get<double>(),tmpLevels));

      }
    } while (FindNextFile(hFind,&findFileData));
    FindClose(hFind);

    selectMusic = 0;
    selectLevel = 0;
  }
  //----------

  f30 = Font(30);
}

void MusicSelection::update() {
  if (infos.size()) {
    if (Input::KeyEnter.clicked && infos[selectMusic].getPlayLevels()[selectLevel] != 0) {
      String levelStr[] = { L"easy",L"normal",L"hard" };

      m_data->title = infos[selectMusic].getTitle();
      m_data->artist = infos[selectMusic].getArtist();
      m_data->folderName = infos[selectMusic].getFolderName();
      m_data->fileName = levelStr[selectLevel];
      m_data->musicFileName = infos[selectMusic].getMusicFileName();
      m_data->offset = infos[selectMusic].getOffset();
      m_data->startMeasure = 0;
      m_data->autoFlag = false;
      m_data->judgeOffset = 0.00;
      m_data->nextScene = L"Result";
      m_data->speed = 14;
      changeScene(L"Game");
    }

    if (Input::KeyUp.clicked || Input::KeyS.clicked) {
      selectMusic = (selectMusic + static_cast<int>(infos.size()) - 1) % static_cast<int>(infos.size());
    }

    if (Input::KeyDown.clicked || Input::KeyD.clicked) {
      selectMusic = (selectMusic + 1) % static_cast<int>(infos.size());
    }

    if ((Input::KeyRight.clicked || Input::KeyL.clicked) && selectLevel < (static_cast<int>(infos[selectMusic].getPlayLevels().size()) - 1)) {
      selectLevel++;
    }

    if ((Input::KeyLeft.clicked || Input::KeyK.clicked) && 0 < selectLevel) {
      selectLevel--;
    }
  }

}

void MusicSelection::draw() const {
  String levelStr[] = { L"easy",L"normal",L"hard" };
  f30(L"フォルダ名 :" + infos[selectMusic].getFolderName()).draw();
  f30(L"タイトル :" + infos[selectMusic].getTitle()).draw({ 0,40 });
  f30(L"難易度" + levelStr[selectLevel] + L"(" + ToString(infos[selectMusic].getPlayLevels()[selectLevel]) + L")").draw({0,80});

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