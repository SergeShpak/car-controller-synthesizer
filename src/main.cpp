#include <iostream>
#include <list>
#include <ibex.h>

#include <ode_generator.hpp>
#include <paver.hpp>
#include <utils.hpp>

bool acceptingFunction(ibex::simulation*, ODEGenerator*, ibex::IntervalVector);

int main(int argc, char **argv){
  IntervalsWriter *writer = new IntervalsWriter("./output/accepted",
                                                "./output/rejected");
  std::cout << "Output files have been created" << std::endl;
  double consigne = 10.0;
  double frottement_terre = 50.0;
  double frottement_air = 0.4;
  ibex::Interval masse(950, 1150);
  ODEGeneratorParameters params = 
    { consigne, {0, 0}, frottement_terre, frottement_air, masse };
  ODEGenerator *ode_generator = new ODEGenerator(params);
  PaverParameters paver_params = { 1 };
  Paver *paver = new Paver(ode_generator, paver_params, writer);
  ibex::Interval K_p(500, 1000);
  ibex::Interval K_i(30, 60);
  ibex::IntervalVector K(2);
  K[0] = K_p;
  K[1] = K_i;
  std::stack<ibex::IntervalVector> acceptedPavings = 
                            paver->ComputePaving(K, acceptingFunction);
  std::cout << "Pavements computed" << std::endl;
  while(!acceptedPavings.empty()) {
    std::cout << acceptedPavings.top() << std::endl;
    acceptedPavings.pop();
  }
  return 0;
}


bool acceptingFunction(ibex::simulation *simu, ODEGenerator *generator,
                      ibex::IntervalVector params) {
  ibex::IntervalVector accepted_box = generator->getAcceptableBox();
  bool finished_well = simu->finished_in(accepted_box); 
  ibex::Function func = generator->getControlledODE(params);
  ibex::Interval v_deriv_evalualtion = (func[0]).eval(simu->get_last());
  std::cout << "y'(10) = " << v_deriv_evalualtion << std::endl;
  bool is_stabilised 
          = v_deriv_evalualtion.is_subset(generator->getStabilityConstraint());
  bool is_acceptable = true;
  if (!finished_well) {
    std::cout << "Simulation finished outside the acceptable box" << std::endl;
    is_acceptable = false;
  }
  ibex::IntervalVector overshoot_box = generator->getOvershootBox();
  bool overshooted = simu->go_out(overshoot_box);
  if (overshooted) {
    std::cout << "Simulation has overshooted" << std::endl;
    is_acceptable = false;
  } 
  if (!is_stabilised) {
    std::cout << "System has not stabilised" << std::endl;
    is_acceptable = false;
  }
  return is_acceptable;
}
