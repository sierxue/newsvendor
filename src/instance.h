#ifndef INSTANCE_H_
#define INSTANCE_H_

class Instance{
 public:
  char m_demand_type;
  double m_demand_param_1;
  double m_demand_param_2;
  double m_epsilon;
  double m_delta;
  double m_b;
  double m_h;
  double m_y_star;
  double m_y_hat;
  double m_c_hat;
  double m_c_star;
  bool m_within_eps;
  
  Instance(char demand_type,
	   double demand_param_1,
	   double demand_param_2,
	   double epsilon,
	   double delta,
	   double b,
	   double h);

  void evaluate();
};

#endif // INSTANCE_H_
