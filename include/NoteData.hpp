#ifndef NOTE_DATA_HPP
#define NOTE_DATA_HPP

class NoteData {
public:
  NoteData(int count, double second, int lane, int length, bool isEndEffect = false,bool isLongFlag = false);
  int count;
  double second;
  int lane;
  int length;
  bool isEndEffect;
  bool isLongFlag;
};


#endif // !NOTE_DATA_HPP
