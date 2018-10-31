#ifndef SHARE_DATA_HPP
#define SHARE_DATA_HPP

class ShareData {

public:
  String folderName;
  String fileName;
  String musicFileName;
  double offset;
  int startMeasure;
  double judgeOffset;
  int speed;

  bool autoFlag;

  String nextScene;
};

#endif //SHARE_DATA_HPP