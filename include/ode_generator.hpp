#include <list>
#include <ibex.h>

#ifndef ODE_GENERATOR_HPP
#define ODE_GENERATOR_HPP

struct ODEGeneratorParameters {
  double consigne;
  double valeur_etat_init[2];
  double frottement_terre;
  double frottement_air;
  ibex::Interval masse;
};

class ODEGenerator {
private:
  double consigne;
  double frottement_terre;
  double frottement_air;
  ibex::IntervalVector etat;
  ibex::Interval masse;
  ODEGenerator();
public:
  ODEGenerator(struct ODEGeneratorParameters params);
  ibex::Function getControlledODE(ibex::IntervalVector K);
  ibex::ivp_ode generateIVP(ibex::IntervalVector K);
  ibex::IntervalVector getAcceptableBox();
  ibex::IntervalVector getOvershootBox();
  ibex::Interval getStabilityConstraint();
};

#endif
