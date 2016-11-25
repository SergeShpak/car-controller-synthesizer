#include <ibex.h>
#include <stack>
#include <utility>

#include <ode_generator.hpp>
#include <utils.hpp>

struct PaverParameters {
  double min_width;
};

class Paver {
private:
  double min_width;
  int current_dimension = 0;
  IntervalsWriter *writer;
  Paver();
  ODEGenerator *generator;
  std::pair<ibex::IntervalVector, ibex::IntervalVector> getNewPavements(
      ibex::IntervalVector pavement);
public:
  Paver(ODEGenerator *generator, PaverParameters, IntervalsWriter*);
  std::stack<ibex::IntervalVector> ComputePaving(ibex::IntervalVector, 
                  bool (*acceptingFunction)(ibex::simulation*, ODEGenerator*,
                    ibex::IntervalVector));
};
