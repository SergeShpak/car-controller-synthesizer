#include <ibex.h>
#include <stack>
#include <utility>
#include <exception>

#include <paver.hpp>

Paver::Paver(ODEGenerator *generator, PaverParameters params, 
            IntervalsWriter *writer) {
  this->generator = generator;
  this->min_width = params.min_width;
  this->writer = writer;
}

std::stack<ibex::IntervalVector> Paver::ComputePaving( 
                                                ibex::IntervalVector pavement,
                bool (*acceptingFunction)(ibex::simulation*, ODEGenerator*, 
                                          ibex::IntervalVector params)) {
  std::stack<ibex::IntervalVector> stock;
  std::stack<ibex::IntervalVector> accepted;
  std::stack<ibex::IntervalVector> rejected;
  stock.push(pavement);
  while (!stock.empty()) {
    ibex::IntervalVector currentPavement = stock.top();
    stock.pop();
    ibex::ivp_ode ivp = 
            generator->generateIVP(currentPavement);  
    ibex::simulation *simu = new ibex::simulation(&ivp, 10.0, ibex::RK4, 1e-7);
    simu->run_simulation();
    
    std::cout << "Current pavement: " << currentPavement << std::endl;
    bool isAccepted = acceptingFunction(simu, this->generator, currentPavement);
    bool isRejected = false;
    delete simu;
    if (isAccepted) {
      std::cout << "Current pavement is accepted" << std::endl;
      writer->WriteAccepted(currentPavement);
      accepted.push(currentPavement);
    }
    if (!isAccepted && currentPavement[0].diam() < min_width 
        && currentPavement[1].diam() < min_width) {
      rejected.push(currentPavement);
      writer->WriteRejected(currentPavement);
      std::cout << "Current pavement is rejected" << std::endl;
      isRejected = true;
    }
    if (!isAccepted && !isRejected) {
      std::pair<ibex::IntervalVector, ibex::IntervalVector> new_pavements = 
        getNewPavements(currentPavement);
      stock.push(new_pavements.first);
      stock.push(new_pavements.second); 
      std::cout << "Current pavement is bisected" << std::endl;
      std::cout << "New boxes: " << new_pavements.first << " " 
                << new_pavements.second << std::endl;
    }
    std::cout << "There are currently " << stock.size() 
              << " boxes in the stock" << std::endl;
    std::cout << "Accepted: " << accepted.size() 
              << "\tRejected: " << rejected.size() << std::endl;
    std::cout << std::endl;
  }
  return accepted;
}

std::pair<ibex::IntervalVector, ibex::IntervalVector> Paver::getNewPavements(
    ibex::IntervalVector pavement) {
  if (pavement[0].diam() < min_width && pavement[1].diam() < min_width) {
    throw new std::runtime_error(
        "Cannot generate new pavement: diameter of the box is too small");
  }
  if (pavement[0].diam() < min_width) {
    current_dimension = 1;
  } 
  if (pavement[1].diam() < min_width) {
    current_dimension = 0;
  }
  if (pavement[0].diam() >= min_width && pavement[1].diam() >= min_width) { 
    current_dimension = current_dimension == 0 ? 1 : 0;
  }
  std::pair<ibex::IntervalVector, ibex::IntervalVector> bisection = 
                                      pavement.bisect(current_dimension, 0.5);
  return bisection;
}
