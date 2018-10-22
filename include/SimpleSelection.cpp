#include"SimpleSelection.hpp"

void SimpleSelection::init(){
  gui = GUI(GUIStyle::Default);
  gui.setTitle(L"読み込み");
  gui.add(GUIText::Create(L"フォルダ名"));
  gui.addln(L"folderName", GUITextField::Create(20));
  gui.add(GUIText::Create(L"ファイル名"));
  gui.addln(L"fileName", GUITextField::Create(20));
  gui.add(L"ok", GUIButton::Create(L"決定"));
  gui.setCenter(Point(Window::Width() / 2, Window::Height() / 2));
  gui.show(true);

}

void SimpleSelection::update(){
  if (gui.button(L"ok").pushed) {
    if (JSONReader(L"Musics/" + gui.textField(L"folderName").text + L"/info.json").isOpened() &&
      JSONReader(L"Musics/" + gui.textField(L"folderName").text + L"/" + gui.textField(L"fileName").text + L".json")) {
      String folderName = gui.textField(L"folderName").text;
      String fileName = gui.textField(L"fileName").text;

      JSONReader reader(L"Musics/" + folderName + L"/info.json");

      m_data->folderName = folderName;
      m_data->fileName = fileName;
      m_data->offset = reader[L"offset"].get<double>();
      m_data->musicFileName = reader[L"musicFileName"].get<String>();

      changeScene(L"Game");
    }
  }
}

void SimpleSelection::draw() const{

}

void SimpleSelection::drawFadeIn(double t) const {
  draw();
  Transformer2D tr(Graphics2D::GetTransform().inverse());
  Window::ClientRect().draw(ColorF(0.0).setAlpha(1.0 - t));
}

void SimpleSelection::drawFadeOut(double t) const {
  draw();
  Transformer2D tr(Graphics2D::GetTransform().inverse());
  Window::ClientRect().draw(ColorF(0.0).setAlpha(t));
}