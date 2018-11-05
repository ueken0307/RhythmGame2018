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

  setting = GUI(GUIStyle::Default);
  setting.setTitle(L"設定");
  setting.add(GUIText::Create(L"オフセット値"));
  setting.addln(L"offset",GUITextField::Create(8));
  setting.textField(L"offset").setText(ToString(m_data->judgeOffset));
  setting.add(L"full", GUIButton::Create(L"フルスクリーンにする"));
  setting.addln(L"window", GUIButton::Create(L"ウィンドウにする"));
  setting.add(L"ok", GUIButton::Create(L"ok"));
  setting.show(false);


}

void MusicSelection::update() {
  if (Input::KeyE.clicked) {
    setting.show(true);
    setting.setCenter(Point(Window::Width() / 2, Window::Height() / 2));
  }

  if (setting.button(L"ok").pushed) {
    setting.show(false);
  }

  if (Window::GetState().fullscreen) {
    setting.button(L"full").enabled = false;
    setting.button(L"window").enabled = true;
  }
  else {
    setting.button(L"full").enabled = true;
    setting.button(L"window").enabled = false;
  }

  if (setting.button(L"full").pushed) {
    Window::SetFullscreen(true, Graphics::GetFullScreenSize().back());
    setting.setCenter(Point(Window::Width() / 2, Window::Height() / 2));
  }

  if (setting.button(L"window").pushed) {
    Window::SetFullscreen(false, Size(672, 378));
    setting.setCenter(Point(Window::Width() / 2, Window::Height() / 2));
  }

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
      m_data->judgeOffset = Parse<double>(setting.textField(L"offset").text);
      m_data->nextScene = L"Result";
      m_data->jacket = infos[selectMusic].getTexture();
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
      s3d::RoundRect(x, y, 800, 100,20).draw(Color(170));
      f30(infos[(selectMusic + infos.size() + i - 5) % infos.size()].getTitle()).drawCenter({ x + 400,y + 50 },Color(255));

    }
    else {
      int x = 250 - i * 25 - scrollX.easeOut();
      int y = -210 + i * 140 + scrollY.easeOut();
      s3d::RoundRect(x, y, 800, 100,20).draw(Color(170));
      f30(infos[(selectMusic + infos.size() + i - 5) % infos.size()].getTitle()).drawCenter({ x + 400,y + 50 }, Color(255));
    }
  }

  int sX = 950;
  int sY = 1080/2-25;
  Triangle({sX,sY + 25}, {sX+50,sY}, {sX+50,sY+50}).draw(Color(255, 0, 0));

  //ジャケット
  Rect(1250, 50, 500)(infos[selectMusic].getTexture()).draw();
  //タイトル
  f30(infos[selectMusic].getTitle()).drawCenter(1500, 600);
  //アーティスト
  f30(infos[selectMusic].getArtist()).drawCenter(1500, 670);
  //bpm
  f30(infos[selectMusic].getBpm()).drawCenter(1500, 740);

  std::vector<Color> levelColors = {Color(0,200,0),Color(200,100,0), Color(200,0,0)};
  for (int i = 0; i < 3; ++i) {
    if(i == selectLevel)s3d::Circle(1200 + i * 250, 900, 120).draw(Color(100));
    s3d::Circle(1200 + i * 250, 900, 100).draw((infos[selectMusic].getPlayLevels()[i] == 0)? Color(150) : levelColors[i]);

    f30(m_data->levelStrs[i]).drawCenter(1200 + i * 250, 900, Color(255));
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