#include <iostream>
#include "random.h"
#include "instance.h"
#include <cmath> 
#include <fstream>
#include <numeric>
using namespace std;

Instance::Instance() {
  reps = 0;
  demand = Demand();
  epsilon = 0;
  delta = 0;
  bh = BH();
}

Instance::Instance(double p_reps, Demand p_demand, double p_epsilon, double p_delta, BH p_bh) {
  reps = p_reps;
  demand = p_demand;
  epsilon = p_epsilon;
  delta = p_delta;
  bh = p_bh;
}

void Instance::evaluate() {
  Rand rnd;
  vector<double> samples;
  vector<double> SAA_eps;
  double wins;
  

  // Calculate optimal inventory and cost
  y_star = demand.lower_bound + (demand.upper_bound - demand.lower_bound)*(bh.b/(bh.b+bh.h));
  c_star = cost(y_star, demand, bh);

  // Calculate sample average approximation
  N = ceil((9.0/(2.0*pow(epsilon,2)))*pow(min(bh.b,bh.h)/(bh.b+bh.h),-2)*log(2.0/delta));

  // Main loop
  for(int i = 0; i < reps; i++) {
    samples = vector<double>(); // Clears memory
    for(int j = 0; j < N; j++) {
      samples.push_back(rnd.uniform(demand.lower_bound, demand.upper_bound));
    }
    sort(samples.begin(), samples.end());
    y_hat = samples[ceil((bh.b*N)/(bh.b + bh.h))]; // Find the b/(b+h)th quantile
    c_hat = cost(y_hat, demand, bh);
    SAA_eps.push_back((c_hat/c_star)-1);
    
    if(c_hat < c_star) 
      cout << "ERROR: Simulated cost is better than optimal.";
    
    if((1+epsilon)*c_star > c_hat) 
      wins++;

  }

  // Calculate statistics
  SAA_within_eps = wins/double(reps);
  SAA_eps_avg = accumulate(SAA_eps.begin(), SAA_eps.end(), 0.0)/double(SAA_eps.size());

  double SAA_eps_var = 0;
  for(int i = 0; i < SAA_eps.size(); i++)
    SAA_eps_var += pow(SAA_eps[i] - SAA_eps_avg, 2);

  SAA_eps_std = sqrt(SAA_eps_var/double(SAA_eps.size()));
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
  file << N << ",";
  file << y_star << ",";
  file << c_star << ",";
  file << SAA_within_eps << ",";
  file << SAA_eps_avg << ",";
  file << SAA_eps_std << "\n";
  file.close();
}

void print_output_header(char* path) {
  ofstream file;
  file.open(path,ios::out);
  file << "reps,demand_type,lower_bound,upper_bound,epsilon,delta,b,h,N,y_star,c_star,SAA_within_eps,SAA_eps_avg,SAA_eps_std\n";
}
  
double cost(double y, Demand d, BH bh) {
  return (pow(y,2)*(bh.h+bh.b))/2.0 - y*(bh.h*d.lower_bound + bh.b*d.upper_bound) + (bh.h*pow(d.lower_bound,2) + bh.b*pow(d.upper_bound,2))/2.0;
}
