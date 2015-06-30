// random.c
//
// Michael Lawrence, mklawren@cs.ubc.ca
// 25/10/2010
//
// Implementation of functions in random.h

#include "random.h"
using namespace std;

double Rand::uniform(double min, double max){
	return min+(max-min)*(rand_stream.RandU01());
}

int Rand::geometric(double success){
	return (int) floor(log(rand_stream.RandU01())/log(1-success));
}

double Rand::truncated_exponential(double lambda){
	double max=-log(0.05)/lambda;
	return min(-log(1-rand_stream.RandU01())/lambda,max);
}

double Rand::normal(double mean, double std){
	double Z;
	if (! reserve_used) {
		Z=reserved_normal;
		reserve_used=true;
	}
	else {
	double U=rand_stream.RandU01();
	double V=rand_stream.RandU01();
	Z=sqrt(-2*log(U))*cos(2*PI*V);
	reserved_normal=sqrt(-2*log(V))*cos(2*PI*U);
	reserve_used=false;
	}
	return Z*std+mean;
}

Rand::Rand(){
	reserve_used=true;
}
