#include <iostream>
#include "settings.h"
#include "instance.h"
using namespace std;

int main (int argc, char* argv[]) {

  Settings settings = Settings(argv[1]);

  int count = 1;
  int max = settings.demands.size()*
    settings.epsilon.size()*
    settings.delta.size()*
    settings.bh.size()*
    settings.N_frac.size();
  
  print_output_header(argv[2]);
  for(int i1 = 0; i1 < settings.demands.size(); i1++) {
    for(int i2 = 0; i2 < settings.epsilon.size(); i2++) {
      for(int i3 = 0; i3 < settings.delta.size(); i3++) {
	for(int i4 = 0; i4 < settings.bh.size(); i4++) {
	  for(int i5 = 0; i5 < settings.N_frac.size(); i5++) {
	    cout << "Evaluating " << count << " of " << max << endl;
	    count++;
	    Instance instance = Instance(settings.reps,
					 settings.demands[i1],
					 settings.epsilon[i2],
					 settings.delta[i3],
					 settings.bh[i4],
					 settings.N_frac[i5]);
	    instance.evaluate();
	    instance.print_output(argv[2]);
	  }
	}
      }
    }
  }
  return 0;
}
