#pragma once

#include "traits.hpp"
#include "time.hpp"
#include <boost/format.hpp>

namespace lma
{
  struct DefaultVerbose
  {
    template<class ... S>
    void at_begin_bundle_adjustment(const S& ...) const {}
    
    template<class ... S>
    void at_begin_bundle_adjustment_iteration(const S&...) const {}
    
    template<class ... S>
    void at_end_bundle_adjustment_iteration(const S&...) const {}
    
    template<class ... S>
    void at_end_bundle_adjustment(const S&...) const {}
  };
}

namespace lma {

template<class> struct LevMar;

namespace internal {

template<class Policy>
static void print_stats(const LevMar<Policy> &a, const boost::format &format)
{
  std::cerr
  << boost::format(format) % "Pre-Process" % a.preprocess
  << boost::format(format) % "Residual evaluations" % a.residual_evaluations
  << boost::format(format) % "Jacobian evaluations" % a.jacobian_evaluations
  << boost::format(format) % "Solver normal eq." % a.norm_eq_
  ;
}

}

struct Verbose: DefaultVerbose
{
  template<class Solver, class Algorithm>
  void at_begin_bundle_adjustment(Solver& s, const Algorithm& lm) const
  {
    clock_total.tic();
    clock_iteration.tic();

    std::cerr
    << std::endl
    << "Bundle adjustment using:"
    << "\e[33m"
    << std::endl
    << name<Solver>() << " : "
    << name<Algorithm>()
    << "\e[36m"
    << std::endl;

    //boost::fusion::for_each(s.bundle.opt_container.map(), print_var());
    //boost::fusion::for_each(s.bundle.fonction_container.map(), print_observation());

    std::cerr
    << "\e[m"
    << std::endl
    << boost::format("%3s  %7s  %10s  %11s  %10s  %11s  %8s  %8s")
      % "#"
      % "D"
      % "Cost"
      % "dCost"
      % "RMS"
      % "dRMS"
      % "IT"
      % "TT"
    << std::endl
    << std::string(82, '-')
    << std::endl;

    print_iteration(s,lm,"");
  }

  template<class Solver, class Algorithm>
  void at_begin_bundle_adjustment_iteration(const Solver&, const Algorithm&) const
  {
    clock_iteration.tic();
  }

  template<class Solver, class Algorithm, class NormalEquation>
  void at_end_bundle_adjustment_iteration(const Solver& s, const Algorithm& lm, const NormalEquation& ) const
  {
    this->print_iteration(s,lm,lm.is_better() ? "\e[32m" : "\e[31m");
  }

  template<class Solver, class Algo>
  void at_end_bundle_adjustment(const Solver& s, const Algo& algo) const
  {
    static const boost::format format("\e[36m%-21s: %g\e[m\n");

    std::cerr
    << std::endl
    << boost::format(format) % "Initial" % s.initial_cost
    << boost::format(format) % "Final" % s.final_cost
    << boost::format(format) % "Change" % (s.initial_cost - s.final_cost)
    << std::endl
    ;

//    internal::print_stats(algo, format);

    std::cerr
    << boost::format(format) % "Total time" % clock_total.toc()
    ;
  }

  double total_time() const { return clock_total.toc(); }
  
private:

  mutable Tic clock_total, clock_iteration;
/*
  struct print_var { template<template<class, class> class Pair, class Key, class Value> void operator()(const Pair<Key, Value> &o) const {
    std::cerr << boost::format("%s (%d)\n") % ttt::name<Key>() %  o.second.size();
  }};

  struct print_observation { template<template<class, class> class Pair, class Key, class Value> void operator()(const Pair<Key, Value> &o) const {
    std::cerr << boost::format("%s (\e[1m%d\e[21m)\n") % ttt::name<Key>() % o.second.size();
  }};
*/
  template<class Solver, class LM>
  void print_iteration(const Solver& s, const LM& lm, const char *color) const
  {
    std::cerr
    << boost::format("%s%3d  %7.1g  %10.5g  %+11.5g  %10.5g  %+11.5g  %8.3g  %8.3g\e[m")
      % color
      % lm.nb_iteration
      % lm.lambda
      % lm.cost2()
      % (lm.cost2() - lm.cost1())
      % lm.rms2()
      % (lm.rms2() - lm.rms1())
      % clock_iteration.toc()
      % clock_total.toc()
    << std::endl;
  }
};

struct VerboseNormalEquation : Verbose
{
  template<class Solver, class Algorithm, class NormalEquation>
  void at_end_bundle_adjustment_iteration(const Solver& s, const Algorithm& lm, const NormalEquation& ne) const
  {
    Verbose::at_end_bundle_adjustment_iteration(s,lm,ne);
    std::cout << " Jacobian \n" << ne.jacobian << std::endl;
    std::cout << " Hessian \n" << ne.hessian << std::endl;
    std::cout << " errors \n" << ne.residuals << std::endl;
    std::cout << " JTE \n" << ne.jte << std::endl;
    std::cout << " Delta \n" << ne.delta << std::endl;
  }
};

}
