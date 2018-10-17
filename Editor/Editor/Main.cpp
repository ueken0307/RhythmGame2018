#include<Siv3D.hpp>
#include <Windows.h>

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
  Graphics::SetBackground(Color(40));
  Graphics::SetVSyncEnabled(false);
  //----------
  
  while (System::Update()){
    
  }

#ifdef _DEBUG
  fclose(out);
  fclose(in);
  FreeConsole();
#endif // _DEBUG

}
