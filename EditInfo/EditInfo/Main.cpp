#include <Siv3D.hpp>

int outputInfo(String folderName,String title,String artist,String bpm,String musicFileName,String jacketFileName, double offset,std::array<int,3> playLevels) {
  TextWriter writer(L"Musics/" + folderName + L"/info.json");

  if (writer.isOpened()) {
    writer.writeln(L"{");
    writer.writeln(L"  \"title\" : \"" + title + L"\",");
    writer.writeln(L"  \"artist\" : \"" + artist + L"\",");
    writer.writeln(L"  \"bpm\" : \"" + bpm + L"\",");
    writer.writeln(L"  \"musicFileName\" : \"" + musicFileName + L"\",");
    writer.writeln(L"  \"jacketFileName\" : \"" + jacketFileName + L"\",");
    writer.writeln(L"  \"offset\" : " + ToString(offset) + L",");
    writer.writeln(L"  \"playLevel\" : [" + ToString(playLevels[0]) +L"," + ToString(playLevels[1]) + L"," + ToString(playLevels[2]) + L"]");
    writer.writeln(L"}");
    
    writer.close();
    return 0;
  }

  return -1;
}

void Main(){
  Window::Resize(750, 400);

  String folderName;
  double offset = 0.0;
  std::array<int, 3> playLevels = { 0 };

  GUI openGUI, alertGUI, editGUI;

  openGUI = GUI(GUIStyle::Default);
  openGUI.setTitle(L"開くor新規作成？");
  openGUI.add(GUIText::Create(L"フォルダ名"));
  openGUI.addln(L"folderName",GUITextField::Create(20));
  openGUI.add(L"open", GUIButton::Create(L"開く"));
  openGUI.add(L"new", GUIButton::Create(L"新規作成"));
  openGUI.setCenter(Point(Window::Width() / 2, Window::Height() / 2));

  alertGUI = GUI(GUIStyle::Default);
  alertGUI.setTitle(L"警告");
  alertGUI.add(GUIText::Create(L"ファイルが開けないまたは作成出来ません"));
  alertGUI.addln(L"ok", GUIButton::Create(L"OK"));
  alertGUI.setCenter(Point(Window::Width() / 2, Window::Height() / 2));
  alertGUI.show(false);

  bool editFlag = false;
  editGUI = GUI(GUIStyle::Default);
  editGUI.setTitle(L"情報");

  editGUI.add(GUIText::Create(L"タイトル"));
  editGUI.addln(L"title",GUITextField::Create(30));
  editGUI.add(GUIText::Create(L"アーティスト"));
  editGUI.addln(L"artist", GUITextField::Create(30));
  editGUI.add(GUIText::Create(L"BPM(表示用)"));
  editGUI.addln(L"bpm", GUITextField::Create(20));
  editGUI.addln(GUIText::Create(L"音楽ファイル名(拡張子も含めて)"));
  editGUI.addln(L"musicFileName", GUITextField::Create(30));
  editGUI.addln(GUIText::Create(L"ジャケット画像ファイル名(拡張子も含めて)"));
  editGUI.addln(L"jacketFileName", GUITextField::Create(30));
  editGUI.add(GUIText::Create(L"オフセット[秒](必ず正の値)"));
  editGUI.addln(L"offset", GUITextField::Create(12));
  editGUI.add(GUIText::Create(L"レベル"));
  editGUI.add(L"pL0", GUITextField::Create(2));
  editGUI.add(L"pL1", GUITextField::Create(2));
  editGUI.addln(L"pL2", GUITextField::Create(2));

  editGUI.add(L"save", GUIButton::Create(L"保存"));

  editGUI.setCenter(Point(Window::Width() / 2, Window::Height() / 2));
  editGUI.show(false);


  while (System::Update()){
    if (alertGUI.button(L"ok").pushed) {
      alertGUI.show(false);
    }

    if (editFlag) {
      if (editGUI.button(L"save").pushed) {
        std::array<int, 3> tmpLevels = { Parse<int>(editGUI.textField(L"pL0").text),Parse<int>(editGUI.textField(L"pL1").text) ,Parse<int>(editGUI.textField(L"pL2").text) };
        outputInfo(folderName, editGUI.textField(L"title").text, editGUI.textField(L"artist").text, editGUI.textField(L"bpm").text,
          editGUI.textField(L"musicFileName").text, editGUI.textField(L"jacketFileName").text, Parse<double>(editGUI.textField(L"offset").text), tmpLevels);
      }


    } else {
      openGUI.button(L"open").enabled = (openGUI.textField(L"folderName").text.length > 0);
      openGUI.button(L"new").enabled = (openGUI.textField(L"folderName").text.length > 0);
     

      if (openGUI.button(L"open").pushed) {
        JSONReader reader(L"Musics/" + openGUI.textField(L"folderName").text + L"/info.json");

        if (reader.isOpened()) {
          folderName = openGUI.textField(L"folderName").text;

          editGUI.textField(L"title").setText(reader[L"title"].get<String>());
          editGUI.textField(L"artist").setText(reader[L"artist"].get<String>());
          editGUI.textField(L"bpm").setText(reader[L"bpm"].get<String>());
          editGUI.textField(L"musicFileName").setText(reader[L"musicFileName"].get<String>());
          editGUI.textField(L"jacketFileName").setText(reader[L"jacketFileName"].get<String>());
          editGUI.textField(L"offset").setText(ToString(reader[L"offset"].get<double>()));
          std::vector<int> tmpLevels;
          for (const auto & i : reader[L"playLevel"].getArray()) {
            tmpLevels.push_back(i.get<int32>());
          }
          editGUI.textField(L"pL0").setText(ToString(tmpLevels[0]));
          editGUI.textField(L"pL1").setText(ToString(tmpLevels[1]));
          editGUI.textField(L"pL2").setText(ToString(tmpLevels[2]));

          openGUI.show(false);
          editFlag = true;
          editGUI.show(true);
        }
        else {
          alertGUI.show(true);
        }
      }

      if (openGUI.button(L"new").pushed) {
        std::array<int, 3> tmp = { 0,0,0 };
        if (outputInfo(openGUI.textField(L"folderName").text, L"", L"", L"", L"", L"", 0.0, tmp) != -1) {
          folderName =  openGUI.textField(L"folderName").text;

          openGUI.show(false);
          editFlag = true;
          editGUI.show(true);
        } else {
          alertGUI.show(true);
        }
      }
    }
  }
}
