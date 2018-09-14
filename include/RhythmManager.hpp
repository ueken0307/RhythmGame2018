#ifndef RHYTHM_MANAGER_HPP
#define RHYTHM_MANAGER_HPP

class RhythmManager {
public:
  RhythmManager();
  void start();
  void update();
  void setOffset(int);
  int getBmsCount() const { return bmsCount; };

private:
  int offset;
  int bmsCount;
};

#endif // RHYTHM_MANAGER_HPP
