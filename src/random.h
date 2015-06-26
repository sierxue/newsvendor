// random.h

#ifndef RANDOM_H_
#define RANDOM_H_
#include <math.h>
#include "RngStream.h"
//http://math.la.asu.edu/~eubank/webpage/rngStreamPaper.pdf

double const PI=4*atan(1.0);

class Rand{
private:
	RngStream rand_stream;
	double reserved_normal;
	bool reserve_used;
	double std_normal();
public:
	double uniform(double min, double max);
	int geometric(double success);
	double truncated_exponential(double lambda);
	double normal(double mean, double std);
	Rand();
};

#endif  // RANDOM_H_
