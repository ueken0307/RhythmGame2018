#ifndef SHARE_DATA_HPP
#define SHARE_DATA_HPP

class ShareData {

public:
  ShareData() {
    judgeStrs.push_back(L"Perfect");
    judgeStrs.push_back(L"Good");
    judgeStrs.push_back(L"Miss");

    judgeDurations.push_back((1.0 / 60.0) * 3.0);
    judgeDurations.push_back((1.0 / 60.0) * 5.0);

    judgePoints.push_back(1.0);
    judgePoints.push_back(0.5);
    judgePoints.push_back(0);

    judgeCounts.resize(3);

    for (int i = 20; i > 0; --i) {
      speedSec.push_back(i*0.1);
    }

  };

  String title;
  String artist;
  String folderName;
  String fileName;
  String musicFileName;

  double offset;
  double judgeOffset;
  int startMeasure;
  bool autoFlag;

  std::vector<double> speedSec;
  int speed;

  int noteNum;

  int score;
  std::vector<String> judgeStrs;
  std::vector<double> judgeDurations;
  std::vector<double> judgePoints;
  std::vector<int> judgeCounts;

  String nextScene;
};


#endif //SHARE_DATA_HPP