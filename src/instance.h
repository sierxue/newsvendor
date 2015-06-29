#ifndef Instance_H_
#define Instance_H_
#include "settings.h"

void print_output_header(char* path);
double cost(double y, Demand d, BH bh);

class Instance{
 public:
  int reps;
  Demand demand;
  double epsilon;
  double delta;
  BH bh;
  double N;
  double y_hat;
  double y_star;
  double c_hat;
  double c_star;
  double SAA_within_eps;
  double SAA_eps_avg;
  double SAA_eps_std;
  Instance();
  Instance(double p_reps, Demand p_demand, double p_epsilon, double p_delta, BH p_bh);

  void evaluate();
  void print_output(char* path);
};

#endif // Instance_H_
