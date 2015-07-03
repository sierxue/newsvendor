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
  if(demand.type == 'u')
    y_star = lower + (upper - lower)*quantile;
  else if(demand.type == 'e')
    y_star = -log(1-quantile)/demand.lambda;
  else
    y_star = 0;
  c_star = cost(y_star, demand, bh);

  // Calculate sample average approximation N
  N = 1000*N_frac;
  
  // Main loop
  for(int i = 0; i < reps; i++) {
    d_samples = vector<double>(); // Clears vector
    for(int j = 0; j < N; j++) {
      if(demand.type == 'u')
	d_samples.push_back(lower + samples[i][j]*(upper-lower));
      else if(demand.type == 'e')
	d_samples.push_back(-(log(1-samples[i][j]))/demand.lambda);
      else
	cout << "Demand type not found." << endl;
    }
    sort(d_samples.begin(), d_samples.end());

    // Calculate SAA
    y_hat = d_samples[ceil((d_samples.size()*quantile)-1)]; // Find the b/(b+h)th quantile
    c_hat = cost(y_hat, demand, bh);
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
  if(demand.type == 'u') {
    file << demand.lower_bound << ",";
    file << demand.upper_bound << ",";
  }
  else if(demand.type == 'e') {
    file << demand.lambda << ",";
    file << "x" << ",";
  }
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
  file.close();
}

void print_output_header(char* path) {
  ofstream file;
  file.open(path,ios::out);
  file << "reps,demand_type,param1,param2,epsilon,delta,b,h,N_frac,N,y_star,c_star,SAA_eps_avg,SAA_eps_conf\n";
}

double cost(double y, Demand demand, BH bh) {
  double b = bh.b;
  double h = bh.h;

  if(demand.type == 'u') { 
    double upper = demand.upper_bound;
    double lower = demand.lower_bound;
    double height = 1/(upper - lower);
    return height*((pow(y,2)*(h+b))/2.0 - y*(h*lower + b*upper) + (h*pow(lower,2) + b*pow(upper,2))/2.0);
  }
  else if(demand.type == 'e') {
    double lambda = demand.lambda;
    cout << "LAMB: " << lambda << endl;
    cout << "y:::: " << y << endl;
    cout << "1st:: " << (1/lambda)*exp(-lambda*y)*(b+h) << endl;
    cout << "2nd:: " << h/lambda << endl;
    return (1/lambda)*exp(-lambda*y)*(b+h) - (h/lambda) + h*y;
  }
  else
    return 0;
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
