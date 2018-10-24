# 譜面の作成方法

## ファイルの説明
### 階層図

```
Editor.exe
SimplePlayer.exe
Musics  ---------------- 　曲名　 ---------- info.json
                |                        |- easy.json
                |                        |- normal.json
                |                        |- hard.json
                |
                |------- 　曲名　 ---------- info.json
                                         |- easy.json
                                         |- normal.json
                                         |- hard.json 
```

### 実行ファイル
[ここ](https://github.com/ueken0307/RhythmGame2018/releases)からダウンロードできる

![editor_download](https://user-images.githubusercontent.com/16350703/47419718-bde9c000-d7b7-11e8-81ca-4452c4727b42.png)

#### Editor.exe
譜面のエディタ。読み込みと保存ができる。

#### SimplePlayer.exe
作成した譜面を再生して確認するプログラム。

### Musicsフォルダ
このフォルダ以下に`曲名`のフォルダを作成して譜面を置いていく。  
※フォルダ名は日本語は使えない。

#### info.json
曲のファイルを保存するファイル。  

```:info.json
{
  "title" : "タイトル",
  "artist" : "作曲者",
  "bpm" : "200",
  "playLevel":[1,5,9],
  "offset":1.2,
  "musicFileName":"ファイル名.mp3",
  "jacketFileName":"ファイル名.png"
}

```

- title  
曲のタイトル。**とりあえず書かなくてもいいよ。**
- artist  
作曲者。**とりあえず書かなくてもいいよ。**
- bpm  
表記上のbpmなので`"20~300"`とかしてもok。**とりあえず書かなくてもいいよ。**
- playLevel  
レベル書くところ。**とりあえず書かなくてもいいよ。**
- offset  
曲と譜面のタイミング調整の値。
単位は秒。  
曲のファイルの先頭に含まれている空白の分の値だから、負の値には絶対にならない。
- musicFileName  
音楽ファイル名。
拡張子はmp3。 
- jacketFileName  
ジャケットファイル名。
拡張子はpng。**とりあえず書かなくてもいいよ。**

#### easy.json , normal.json , hard.json
難易度ごとの譜面ファイル。

## 譜面の作成手順

### 1.作りたい曲のフォルダを作成する。
Musicsフォルダ内に作る。  
日本語は使えないので`コンギョ` を作るときはMusicsフォルダ内に`kongyo`フォルダを作る。  

```
Editor.exe
SimplePlayer.exe
Musics  ---------------- template --------- info.json
                |
                |------- kongyo
```

### 2.tmplateフォルダからinfo.jsonをコピー
Musicsフォルダ内のtmplateフォルダからinfo.jsonを作ったフォルダにコピーする。

### 3.音楽ファイル名の指定
作ったフォルダに音楽ファイルを置く。  
info.json内の"musicFileName"の値を置いたファイルの名前に変える。（サクラエディタとかメモ帳で開いて）  

```
Editor.exe
SimplePlayer.exe
Musics  ---------------- template --------- info.json
                |
                |-------  kongyo  --------- info.json
                                         |- コンギョ.mp3
```

```info.json
{
  "title" : "タイトル",
  "artist" : "作曲者",
  "bpm" : "200",
  "playLevel":[1,5,9],
  "offset":1.2,
  "musicFileName":"コンギョ.mp3", ←ここ
  "jacketFileName":"ファイル名.png"
}
```

### 4.Editor.exeで譜面を作成する。
Editor.exeを起動したらとりあえずBをクリックして、小節の一番最初(下)をクリックして最初のBPMを設定する。  
`保`をクリックしてとりあえず保存する。  
コンギョのときは、フォルダ名は`kongyo`。
ファイル名は何でもok。ファイル名には`.json`は含めないでok。
    
譜面作成ソフトの操作方法は画像を参照  

![editor_howto](https://user-images.githubusercontent.com/16350703/47419167-8a5a6600-d7b6-11e8-90b7-5ccac59db882.png)

### 5.再生して確認
SimplePlayer.exeを起動して、フォルダ名とファイル名を指定して`決定`をクリックしたら再生画面に移る。  
  
再生画面で、スペースキーを押したら再生が始まる。  
再生画面で、エンターキーを押したら元のファイル指定の画面に戻る。
    
曲と譜面がズレている場合はinfo.jsonの`offset`の値をいじって調整する。 
  
Editor.exeで譜面を編集して保存して、SimplePlayer.exeで再生して確認。を繰り返していくので、両方起動した状態でok。
