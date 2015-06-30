#ifndef Instance_H_
#define Instance_H_
#include "settings.h"

void print_output_header(char* path);
double cost(double y, Demand d, BH bh);

class Instance{
 public:
  int reps;
  Demand demand;
  BH bh;
  double N_frac;
  double N;
  double y_hat;
  double y_star;
  double c_hat;
  double c_star;
  double SAA_eps_avg;
  double SAA_eps_std;
  Instance();
  Instance(double p_reps, Demand p_demand, BH p_bh, double p_N_frac);
  
  void evaluate();
  void print_output(char* path);
};

#endif // Instance_H_
