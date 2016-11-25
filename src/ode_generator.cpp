#include <ode_generator.hpp>
#include <ibex.h>

ODEGenerator::ODEGenerator(struct ODEGeneratorParameters params) : etat(2) {
  consigne = params.consigne;
  for(int i = 0; i < 2; i++) {
    etat[i] = params.valeur_etat_init[i];
  }
  frottement_air = params.frottement_air;
  frottement_terre = params.frottement_terre;
  masse = params.masse;
}

ibex::Function ODEGenerator::getControlledODE(ibex::IntervalVector K) {
  ibex::Interval K_p = K[0];
  ibex::Interval K_i = K[1];
  ibex::Variable v(2);
  ibex::Function v_dot(v,
      ibex::Return((K_p * (consigne - v[0]) + K_i * v[1] -
                  v[0] * (frottement_terre + frottement_air * v[0])) / masse,
                  consigne - v[0]));
  return v_dot;
}

ibex::ivp_ode ODEGenerator::generateIVP(ibex::IntervalVector K) {
  ibex::Function controlledODE = getControlledODE(K);
  ibex::ivp_ode vdb(controlledODE, 0.0, etat);
  return vdb; 
}

ibex::IntervalVector ODEGenerator::getAcceptableBox() {
  ibex::IntervalVector box(2);
  box[0] = ibex::Interval(consigne * 0.98, consigne * 1.02);
  box[1] = ibex::Interval::ALL_REALS;
  return box;
}

ibex::IntervalVector ODEGenerator::getOvershootBox() {
  ibex::IntervalVector box(2);
  box[0] = ibex::Interval(0, consigne * 1.1);
  box[1] - ibex::Interval::ALL_REALS;
  return box;
}

ibex::Interval ODEGenerator::getStabilityConstraint() {
  ibex::Interval stability_constraint(-0.35, 0.35);
  return stability_constraint;
}
