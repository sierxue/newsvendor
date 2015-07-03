#ifndef Settings_H_
#define Settings_H_

#include <vector>
using namespace std;

class Demand{
 public:
  char type;
  double lower_bound;
  double upper_bound;
  double lambda;
  Demand();
  Demand(char p_type, double p_param2, double p_param1);
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
  vector<double> N_frac;
  Settings();
  Settings(char* path);
};

#endif // Settings_H_
