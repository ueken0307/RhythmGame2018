#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include <Windows.h>

#include "ShareData.hpp"
#include "MusicSelection.hpp"
#include "Game.hpp"
#include "Result.hpp"


void Main() {

#ifdef _DEBUG
  //デバッグ時のみコンソール出力を有効にする
  AllocConsole();
  FILE* out = 0;
  freopen_s(&out, "CON", "w", stdout);
  FILE* in = 0;
  freopen_s(&in, "CON", "r", stdin);
#endif // _DEBUG

  //----------
  Window::SetTitle(L"高専祭2018");
  ScalableWindow::Setup(1920, 1080);
  Window::Resize(672, 378);
  Graphics::SetBackground(Color(40));
  Graphics::SetVSyncEnabled(false);
  //----------
  SceneManager<String, ShareData> scene;
  scene.add<MusicSelection>(L"MusicSelection");
  scene.add<Game>(L"Game");
  scene.add<Result>(L"Result");
  //----------  

  while (s3d::System::Update()) {
    {
      //可変サイズのもの
      const auto transformer = ScalableWindow::CreateTransformer();
      scene.updateAndDraw();
    }
    ScalableWindow::DrawBlackBars();
  }


#ifdef _DEBUG
  fclose(out);
  fclose(in);
  FreeConsole();
#endif // _DEBUG

}
