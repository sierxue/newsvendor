#include <iostream>
#include "random.h"
#include "instance.h"
#include <cmath> 
#include <fstream>
#include <numeric>
using namespace std;

Instance::Instance() {
  reps = 1;
  demand = Demand();
  epsilon = 0.05;
  delta = 0.05;
  bh = BH();
  N_frac = 1;
}

Instance::Instance(double p_reps, Demand p_demand, double p_epsilon, double p_delta, BH p_bh, double p_N_frac) {
  reps = p_reps;
  demand = p_demand;
  epsilon = p_epsilon;
  delta = p_delta;
  bh = p_bh;
  N_frac = p_N_frac;
}

void Instance::evaluate(vector<vector<double> > samples) {

  vector<double> d_samples;
  vector<double> SAA_eps;

  double upper = demand.upper_bound;
  double lower = demand.lower_bound;
  
  // Calculate optimal inventory and cost
  double quantile = bh.b/(bh.b + bh.h);
  y_star = lower + (upper - lower)*quantile;
  c_star = cost_uniform(y_star, lower, upper, 1/(upper-lower), bh);

  // Calculate sample average approximation N
  N = 1000*N_frac;
  
  // Main loop
  for(int i = 0; i < reps; i++) {
    d_samples = vector<double>(); // Clears vector
    for(int j = 0; j < N; j++) {
      d_samples.push_back(lower + samples[i][j]*(upper-lower));
    }
    sort(d_samples.begin(), d_samples.end());

    // Calculate SAA
    y_hat = d_samples[ceil((d_samples.size()*quantile)-1)]; // Find the b/(b+h)th quantile
    c_hat = cost_uniform(y_hat, lower, upper, 1/(upper-lower), bh);
    SAA_eps.push_back((c_hat/c_star)-1);

    if(c_hat < c_star) 
      cout << "ERROR: Simulated cost is better than optimal." << endl;
  }

  // Calculate statistics
  SAA_eps_avg = average(SAA_eps);
  SAA_eps_conf = conf(SAA_eps, reps);
}

void Instance::print_output(char* path) {
  ofstream file;
  file.open(path,ios::app);
  file << reps << ",";
  file << demand.type << ",";
  file << demand.lower_bound << ",";
  file << demand.upper_bound << ",";
  file << epsilon << ",";
  file << delta << ",";
  file << bh.b << ",";
  file << bh.h << ",";
  file << N_frac << ",";
  file << N << ",";
  file << y_star << ",";
  file << c_star << ",";
  file << SAA_eps_avg << ",";
  file << SAA_eps_conf << "\n";
  //file << SAA_eps_avg_interp << ",";
  //file << SAA_eps_conf_interp << "\n";
  file.close();
}

void print_output_header(char* path) {
  ofstream file;
  file.open(path,ios::out);
  file << "reps,demand_type,lower_bound,upper_bound,epsilon,delta,b,h,N_frac,N,y_star,c_star,SAA_eps_avg,SAA_eps_conf\n";
}

double cost_uniform(double y, double lower, double upper, double height, BH bh) {
  return height*((pow(y,2)*(bh.h+bh.b))/2.0 - y*(bh.h*lower + bh.b*upper) + (bh.h*pow(lower,2) + bh.b*pow(upper,2))/2.0);
}

double interp_eval(vector<double> samples, double fractile) {
  if(!is_sorted(samples.begin(), samples.end()))
    sort(samples.begin(), samples.end());

  if(fractile < 1/double(samples.size()))
    return samples[0];
  else if(fractile > 1-(1/double(samples.size())))
    return samples[samples.size()-1];
  else {
    double true_ind = fractile*(samples.size())-1;
    int lower_ind = floor(true_ind);
    int upper_ind = ceil(true_ind);
    // cout << true_ind << ":" << lower_ind << ":" << upper_ind << endl;
    if(lower_ind == upper_ind)
      return samples[lower_ind];
    else
      return samples[lower_ind] + (true_ind-lower_ind)*(samples[upper_ind]-samples[lower_ind])/(upper_ind-lower_ind);
  }
}
  
double average(vector<double> data) {
  return accumulate(data.begin(), data.end(), 0.0)/double(data.size());
}

double conf(vector<double> data, int N) {
  double var = 0;
  for(int i = 0; i < data.size(); i++)
    var += pow(data[i] - average(data), 2);
  
  double std = sqrt(var/double(data.size()));
  return 1.96*std/sqrt(N); // 95% confidence interval
}
