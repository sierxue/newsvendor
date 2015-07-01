#include <iostream>
#include "random.h"
#include "instance.h"
#include <cmath> 
#include <fstream>
#include <numeric>
using namespace std;

XY::XY() {
  x = 0;
  y = 0;
}

XY::XY(double p_x, double p_y) {
  x = p_x;
  y = p_y;
}

Instance::Instance() {
  reps = 0;
  demand = Demand();
  bh = BH();
  N_frac = 1;
}

Instance::Instance(double p_reps, Demand p_demand, BH p_bh, double p_N_frac) {
  reps = p_reps;
  demand = p_demand;
  bh = p_bh;
  N_frac = p_N_frac;
}

void Instance::evaluate() {
  Rand rnd;
  vector<double> samples;
  vector<double> SAA_eps;
  vector<double> SAA_eps_smoothed;
  vector<XY> CDF;

  double upper = demand.upper_bound;
  double lower = demand.lower_bound;
  
  // Calculate optimal inventory and cost
  y_star = demand.lower_bound + (upper - lower)*(bh.b/(bh.b+bh.h));
  c_star = cost_uniform(y_star, lower, upper, 1/(upper-lower), bh);

  // Calculate sample average approximation N
  N = 1000*N_frac;
  
  // Main loop
  for(int i = 0; i < reps; i++) {
    samples = vector<double>(); // Clears vector
    for(int j = 0; j < N; j++) {
      samples.push_back(rnd.uniform(lower, upper));
    }
    sort(samples.begin(), samples.end());

    // Calculate SAA
    y_hat = samples[ceil((N*bh.b)/(bh.b + bh.h))]; // Find the b/(b+h)th quantile
    c_hat = cost_uniform(y_hat, lower, upper, 1/(upper-lower), bh);
    SAA_eps.push_back((c_hat/c_star)-1);

    // Calculate smoothed
    CDF = CDF_from_samples(samples);
    y_hat_smoothed = interp_eval(CDF, bh.b/(bh.b + bh.h));
    c_hat_smoothed = cost_smoothed(y_hat_smoothed, CDF, bh);
    SAA_eps_smoothed.push_back((c_hat_smoothed/c_star)-1);
    
    if(c_hat < c_star) 
      cout << "ERROR: Simulated cost is better than optimal." << endl;
    //if(c_hat_smoothed < c_star) 
      //      cout << "ERROR: Smoothed cost is better than optimal." << endl;
  }

  // Calculate statistics
  SAA_eps_avg = average(SAA_eps);
  SAA_eps_conf = conf(SAA_eps, reps);
  SAA_eps_avg_smoothed = average(SAA_eps_smoothed);
  SAA_eps_conf_smoothed = conf(SAA_eps_smoothed, reps);

}

void Instance::print_output(char* path) {
  ofstream file;
  file.open(path,ios::app);
  file << reps << ",";
  file << demand.type << ",";
  file << demand.lower_bound << ",";
  file << demand.upper_bound << ",";
  file << bh.b << ",";
  file << bh.h << ",";
  file << N_frac << ",";
  file << N << ",";
  file << y_star << ",";
  file << c_star << ",";
  file << SAA_eps_avg << ",";
  file << SAA_eps_conf << ",";
  file << SAA_eps_avg_smoothed << ",";
  file << SAA_eps_conf_smoothed << "\n";
  file.close();
}

void print_output_header(char* path) {
  ofstream file;
  file.open(path,ios::out);
  file << "reps,demand_type,lower_bound,upper_bound,b,h,N_frac,N,y_star,c_star,SAA_eps_avg,SAA_eps_conf,SAA_eps_avg_smoothed,SAA_eps_conf_smoothed\n";
}

double cost_uniform(double y, double lower, double upper, double height, BH bh) {
  return height*((pow(y,2)*(bh.h+bh.b))/2.0 - y*(bh.h*lower + bh.b*upper) + (bh.h*pow(lower,2) + bh.b*pow(upper,2))/2.0);
}

double cost_smoothed(double y, vector<XY> CDF, BH bh) {
  double total_cost = 0;
  double rect_cost = 0;
  double height = 0; // height of PDF = slope of PDF

  // Integrals evaluated for each type of rectangle.
  for(int i = 0; i < CDF.size() - 1; i++) {
    height = (CDF[i+1].y - CDF[i].y)/(CDF[i+1].x - CDF[i].x); 
    if(CDF[i].x < y && y < CDF[i+1].x) { // y is in the rectangle
      rect_cost = cost_uniform(y, CDF[i].x, CDF[i+1].x, height, bh);
    }
    else if(y < CDF[i].x) { // y is entirely below the rectangle
      rect_cost = height*bh.h*((pow(CDF[i+1].x,2)-pow(CDF[i].x,2))/2.0 + y*(CDF[i].x - CDF[i+1].x));
    }
    else if(CDF[i+1].x < y) { // y is entirely above the rectangle
      rect_cost = height*bh.h*((pow(CDF[i].x,2)-pow(CDF[i+1].x,2))/2.0 + y*(CDF[i+1].x - CDF[i].x));
    }
    else if(CDF[i+1].x == y) { // y is on the upper boundary of the rectangle
      rect_cost = height*bh.h*(pow(y,2)/2.0 - CDF[i].x*y + pow(CDF[i].x,2)/2.0);
    }
    else if(CDF[i].x == y) { // y is on the lower boundary of the rectangle
      rect_cost = height*bh.b*(pow(y,2)/2.0 - CDF[i+1].x*y + pow(CDF[i+1].x,2)/2.0);
    }
    else {
      cout << "SHOULDN'T REACH HERE: CDF[i].x =" << CDF[i].x << ",CDF[i+1] = "<< CDF[i+1].x << ", y = " << y << endl;
    }

    total_cost += rect_cost;

  }
  return total_cost;
}

double interp_eval(vector<XY> func, double input) {
  // Takes a set of (x,y) coordinates and evaluates the linearly interpolated
  // function. Assumes function is one-to-one and increasing (like a CDF). 
  // Find index where input and function meet.
  vector<double> x;
  vector<double> y;
  
  for(int i = 0; i < func.size(); i++) {
    x.push_back(func[i].x);
    y.push_back(func[i].y);
  }

  double lower, upper, base, extra;
  std::vector<double>::iterator it;
  it = lower_bound(y.begin(), y.end(), input);
  if (it == y.begin()) {
    upper = it-y.begin(); // no smaller value  than val in vector
    return x[upper];
  }
  else if (it == y.end()) {
    lower = (it-1)-y.begin(); // no bigger value than val in vector
    return x[lower];
  }
  else {
    lower = (it-1)-y.begin();
    upper = it-y.begin();
    base = x[lower];
    extra = (input - y[lower])*(x[upper]-x[lower])/(y[upper]-y[lower]);
    return base + extra;
  }
}
  
vector<XY> CDF_from_samples(vector<double> samples) {
  // Sort samples 
  if(!is_sorted(samples.begin(), samples.end()))
    sort(samples.begin(), samples.end());

  // Generate CDF
  vector<XY> CDF;
  double x;
  double y;

  for(int i = 0; i < samples.size(); i++) {
    x = samples[i];
    y = i/double(samples.size()-1);
    CDF.push_back(XY(x,y));
  }  
  return CDF;
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
