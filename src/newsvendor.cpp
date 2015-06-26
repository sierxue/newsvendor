#include <iostream>
#include "random.h"
using namespace std;

int main (int argc, char* argv[]) {

  char* settings_file = argv[1];
  char* demand_file = argv[2];
  char* epsilon_file = argv[3];
  char* delta_file = argv[4];
  char* bh_file = argv[5];
  char* output_file = argv[6];
  
  

  
  Rand rnd;
  double x = rnd.uniform(0,1);
  double y = rnd.uniform(0,1);
  cout << x << y << endl;
  cout << "argc = " << argc << endl;
  for(int i = 0; i < argc; i++)
    cout << "arv[" << i << "] = " << argv[i] << endl;
  cout << "Hello, world, bitches!\n";
  return 0;
}
