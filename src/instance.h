#ifndef Instance_H_
#define Instance_H_
#include "settings.h"

class XY{
 public:
  double x;
  double y;
  XY();
  XY(double x, double y);
};

class Instance{
 public:
  int reps;
  Demand demand;
  BH bh;
  double N_frac;
  double N;
  double y_hat;
  double y_hat_smoothed;
  double y_star;
  double c_hat;
  double c_hat_smoothed;
  double c_star;
  double SAA_eps_avg;
  double SAA_eps_conf;
  double SAA_eps_avg_smoothed;
  double SAA_eps_conf_smoothed;
  Instance();
  Instance(double p_reps, Demand p_demand, BH p_bh, double p_N_frac);
  
  void evaluate();
  void print_output(char* path);
};

void print_output_header(char* path);
double cost_uniform(double y, double lower, double upper, double height, BH bh);
double cost_smoothed(double y, vector<XY> CDF, BH bh);
vector<XY> CDF_from_samples(vector<double> samples);
double interp_eval(vector<XY>, double input);
double average(vector<double> data);
double conf(vector<double> data, int N);

#endif // Instance_H_
