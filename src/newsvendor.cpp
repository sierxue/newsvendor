#include <iostream>
#include "settings.h"
#include "instance.h"
using namespace std;

int main (int argc, char* argv[]) {

  Settings settings = Settings(argv[1]);

  int count = 1;
  int max = settings.demands.size()*
    settings.bh.size()*
    settings.N_frac.size();
  
  print_output_header(argv[2]);
  for(int i1 = 0; i1 < settings.demands.size(); i1++) {
    for(int i2 = 0; i2 < settings.bh.size(); i2++) {
      for(int i3 = 0; i3 < settings.N_frac.size(); i3++) {
	cout << "Evaluating " << count << " of " << max << endl;
	count++;
	Instance instance = Instance(settings.reps,
				     settings.demands[i1],
				     settings.bh[i2],
				     settings.N_frac[i3]);
	instance.evaluate();
	instance.print_output(argv[2]);
      }
    }
  }
  
  return 0;
}
