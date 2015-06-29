#ifndef Settings_H_
#define Settings_H_

#include <vector>
using namespace std;

class Demand{
 public:
  char type;
  double lower_bound;
  double upper_bound;
  Demand();
  Demand(char p_type, double p_lower_bound, double p_upper_bound);
  double draw_random();
};

class BH{
 public:
  double b;
  double h;
  BH();
  BH(double p_b, double p_h);
};

class Settings{
 public:
  int reps;
  vector<Demand> demands;
  vector<double> epsilon;
  vector<double> delta;
  vector<BH> bh;
  Settings();
  Settings(char* path);
};

#endif // Settings_H_
