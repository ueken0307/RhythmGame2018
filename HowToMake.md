# 譜面の作成方法

## 0.ファイルのダウンロード
### 実行ファイル
[ここ](https://github.com/ueken0307/RhythmGame2018/releases)からダウンロードできる  
ダウンロードしたファイルを解凍したら、  
- EditInfo.exe
- Editor.exe
- SimplePlayer.exe
- Musicsフォルダ

が出てくる。作成する譜面はMusicsフォルダ以下に配置する。  

![editor_download](https://user-images.githubusercontent.com/16350703/47419718-bde9c000-d7b7-11e8-81ca-4452c4727b42.png)

### 階層図

```
EditInfo.exe
Editor.exe
SimplePlayer.exe
Musics  ---------------- 　曲名　 ---------- info.json
                |                        |- easy.json
                |                        |- normal.json
                |                        |- hard.json
                |                        |- something.mp3
                |
                |------- 　曲名　 ---------- info.json
                                         |- easy.json
                                         |- normal.json
                                         |- hard.json 
                                         |- aiueo.mp3
```

## 1.EditInfo.exeを起動してinfo.jsonを作成する
EditInfo.exeを起動して、フォルダ名（日本語ダメ）を入力して新規作成を押して、フォルダとinfo.jsonを作成する。

## 2.音楽ファイルの配置
作成されたフォルダに、音楽ファイルを配置する。  
EditInfo.exeを起動して、フォルダ名を入力して読み込み、`音楽ファイル名`のところに配置した音楽ファイルの名前を拡張子付きで入力する。

## 3.Editor.exeを起動して最初のBPMを入力する
Editor.exeを起動したら、Bを押し、一番最初の拍の部分をクリックして、BPMを追加する。  
とりあえず保存をクリックし、フォルダ名とファイル名を入力して保存を押す。その後、保存されていることを確認する。  
Editor.exeの操作方法は以下の画像の通り。  

![editor_howto](https://user-images.githubusercontent.com/16350703/47419167-8a5a6600-d7b6-11e8-90b7-5ccac59db882.png)

## 4.SimplePlayer.exeを起動して再生する
SimplePlayer.exeを起動して、`フォルダ名`と`ファイル名`と`スタート小節`を入力して決定を押す。(ファイル名には.jsonは含めなくてok)  
このとき、決定を押しても画面が変わらない場合は、`フォルダ名`が`ファイル名`が間違っている、EditInfo.exeで指定した`音楽ファイル名`が間違っているなどが考えられる。  
再生画面に遷移したら、**スペースキーで再生開始** し、 **エンターキーを押すとファイル指定画面に戻る** 。

## 5.曲と譜面のズレの調整    
曲と譜面がズレている場合はEditInfo.exeを起動して`オフセット`の値をいじって調整する。 

## 6.編集と再生を繰り返して譜面を作っていく
Editor.exeで譜面を編集して保存して、SimplePlayer.exeで再生して確認。を繰り返していくので、両方起動した状態でok。

## 注意
こまめに保存しよう。
