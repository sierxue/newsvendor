#include "settings.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

Demand::Demand() {
  type = 'u';
  lower_bound = 0;
  upper_bound = 1;
}

Demand::Demand(char p_type, double p_lower_bound, double p_upper_bound) {
  type = p_type;
  lower_bound = p_lower_bound;
  upper_bound = p_upper_bound;
}

BH::BH() {
  b = 0;
  h = 0;
}

BH::BH(double p_b, double p_h) {
  b = p_b;
  h = p_h;
}

Settings::Settings() {
  reps = 0;
  demands.push_back(Demand());
  epsilon.push_back(0);
  delta.push_back(0);
  bh.push_back(BH());
  N_frac.push_back(1);
} 

Settings::Settings(char* path) {
  char param;
  FILE* file;
  char line_buffer[1024];

  file = fopen(path,"r");
  if (file == NULL) {
    perror ("Error opening file");
  }
  else {
    int number_read = 0;
    while (fgets(line_buffer, 1024, file)) {
      if (strncmp("reps", line_buffer, 4) == 0) {
	sscanf(line_buffer, "%s %d", &param, &reps);
      }
      else if (strncmp("N_frac", line_buffer, 5) == 0) {
        double tmp_N_frac;
	sscanf(line_buffer, "%s %lf", &param, &tmp_N_frac);
	N_frac.push_back(tmp_N_frac);
      }
      else if (strncmp("demand", line_buffer, 5) == 0) {
        char tmp_type;
	double tmp_param1;
	double tmp_param2;
        sscanf(line_buffer, "%s %c %lf %lf", &param, &tmp_type, &tmp_param1, &tmp_param2);
	Demand demand = Demand(tmp_type, tmp_param1, tmp_param2);
	demands.push_back(demand);
      }
      else if (strncmp("epsilon", line_buffer, 7) == 0) {
        double tmp_epsilon;
	sscanf(line_buffer, "%s %lf", &param, &tmp_epsilon);
	epsilon.push_back(tmp_epsilon);
      }
      else if (strncmp("delta", line_buffer, 5) == 0) {
	double tmp_delta;
	sscanf(line_buffer, "%s %lf", &param, &tmp_delta);
	delta.push_back(tmp_delta);
      }
      else if (strncmp("bh", line_buffer, 1) == 0) {
	double tmp_b;
	double tmp_h;
	sscanf(line_buffer, "%s %lf %lf", &param, &tmp_b, &tmp_h);
	BH bh1 = BH(tmp_b, tmp_h);
	bh.push_back(bh1);
      }
      else{
	cout << "No match for entry:" << line_buffer << endl;
      }
    }
  }
  fclose(file);
}
