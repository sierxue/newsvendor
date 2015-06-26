#include "instance.h"


Instance::Instance(char demand_type,
		   double demand_param1,
		   double demand_param2,
		   double epsilon,
		   double delta,
		   double b,
		   double h) {
  m_demand_type = demand_type;
  m_demand_param_1 = demand_param1;
  m_demand_param_2 = demand_param2;
  m_epsilon = epsilon;
  m_delta = delta;
  m_b = b;
  m_h = h;
}

void Instance::evaluate() {
  m_y_star = 1;
  m_y_hat = 2;
  m_c_star = 3;
  m_c_hat = 4;
}
