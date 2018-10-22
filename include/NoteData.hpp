#ifndef NOTE_DATA_HPP
#define NOTE_DATA_HPP

class NoteData {
public:
  NoteData(int count, double second, int lane, int length, bool isEndEffect = false);
  int count;
  double second;
  int lane;
  int length;
  bool isEndEffect;
};


#endif // !NOTE_DATA_HPP