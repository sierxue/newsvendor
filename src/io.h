#ifndef IO_H_
#define IO_H_

#include "instance.h"
#include <vector>
using namespace std;

vector<double> read_settings(char* path);
vector<double> read_demand(char* path);
vector<double> read_epsilon(char* path);
vector<double> read_delta(char* path);
vector<double> read_bh(char* path);
void write_output_header(char* path);
void write_output_entry(char* path, Instance data);

#endif // IO_H_
