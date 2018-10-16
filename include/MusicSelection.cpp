#include "MusicSelection.hpp"

#include <Windows.h>

void MusicSelection::init() {
  //---------get musics
  WIN32_FIND_DATA findFileData;
  HANDLE hFind;
  TCHAR *target = TEXT("Musics\\*");

  printf("\n-----LoadedInfos------\n");
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

        printf("MusicFIleName:%ls\n", infos[infos.size() - 1].getMusicFileName().c_str());
        printf("Title:%ls\n", infos[infos.size() - 1].getTitle().c_str());
        printf("Artist:%ls\n", infos[infos.size() - 1].getArtist().c_str());
        printf("Bpm:%ls\n", infos[infos.size() - 1].getBpm().c_str());
        printf("Offset:%lf\n", infos[infos.size() - 1].getOffset());
        printf("PlayLevels:");
        for (const auto & i : infos[infos.size() - 1].getPlayLevels()) {
          printf("%d,", i);
        }
        printf("\n----------------------\n\n");

      }
    } while (FindNextFile(hFind,&findFileData));
    FindClose(hFind);

    selectMusic = 0;
    selectLevel = 0;
    if (static_cast<int>(infos.size())) {
      printf("select:%ls\n", infos[selectMusic].getTitle().c_str());
      printf("select:%d(Lv:%d)\n", selectLevel, infos[selectMusic].getPlayLevels()[selectLevel]);
    }
    else {
      printf("Not found\n");
    }
  }
  //----------

}

void MusicSelection::update() {
  if (infos.size()) {
    if (Input::KeyEnter.clicked && infos[selectMusic].getPlayLevels()[selectLevel] != 0) {
      printf("Next scene is Game\n");
      m_data->folderName = infos[selectMusic].getFolderName();
      m_data->musicFileName = infos[selectMusic].getMusicFileName();
      m_data->level = selectLevel;
      m_data->offset = infos[selectMusic].getOffset();
      changeScene(L"Game");
    }

    if (Input::KeyUp.clicked) {
      selectMusic = (selectMusic + static_cast<int>(infos.size()) - 1) % static_cast<int>(infos.size());
      printf("select:%ls\n", infos[selectMusic].getTitle().c_str());
    }

    if (Input::KeyDown.clicked) {
      selectMusic = (selectMusic + 1) % static_cast<int>(infos.size());
      printf("select:%ls\n", infos[selectMusic].getTitle().c_str());
    }

    if (Input::KeyRight.clicked && selectLevel < (static_cast<int>(infos[selectMusic].getPlayLevels().size()) - 1)) {
      selectLevel++;
      printf("select:%d(Lv:%d)\n", selectLevel, infos[selectMusic].getPlayLevels()[selectLevel]);
    }

    if (Input::KeyLeft.clicked && 0 < selectLevel) {
      selectLevel--;
      printf("select:%d(Lv:%d)\n", selectLevel, infos[selectMusic].getPlayLevels()[selectLevel]);
    }
  }

}

void MusicSelection::draw() const {

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