#pragma once

#include "predef.hpp"

#include "ct-analysis.hpp"
#include "block.hpp"
#include "verbose.hpp"
#include "tools.hpp"
#include "normal_equations.hpp"
#include "bundle.hpp"
#include "lm_policies.hpp"

namespace lma
{

  template<class Functor, int NbInstanceOfFunctor=-1, int NbInstanceOfParameters=-1> struct Solver
  {
    template<int I> struct SetNbInstanceOfFunctors
    {
      using type = Solver<Functor,I,NbInstanceOfParameters>;
    };
    
    template<int I> struct SetNbInstanceOfParameters
    {
      using type = Solver<Functor,NbInstanceOfFunctor,I>;
    };
    
    
    double initial_cost, final_cost;
    using InfoFunctor = AnalyseFunctor<Functor>;
    using Parameters  = typename InfoFunctor::Parameters;
    using Residual    = typename InfoFunctor::Residual;
    using Bundle      = Data<Functor,Parameters,NbInstanceOfFunctor,NbInstanceOfParameters>;
    
    Bundle bundle;
    
    Solver& add(const Functor& f_, Parameters* parameters_)
    {
      bundle.add(f_,parameters_);
      return *this;
    }
    
    template<template<class Policy> class Policy, class Float, class Verbose=DefaultVerbose>
    Solver& solve(Policy<Float> lm, Verbose verbose = Verbose{})
    {
      using NormalEq = NormalEquation<Float,InfoFunctor,NbInstanceOfFunctor,NbInstanceOfParameters>;
      NormalEq normal_equation;
      
      bundle.update();
      normal_equation.resize(bundle);
      
      lm.error1 = lm.error2 = normal_equation.compute_error(bundle);
      initial_cost = lm.cost1();
      
      verbose.at_begin_bundle_adjustment(*this,lm);
      
      do
      {
        verbose.at_begin_bundle_adjustment_iteration(*this,lm);

        bundle.save();

        auto& deltas = normal_equation.solve(lm,bundle);

        bundle.update_parameters(deltas);
        
        lm.update( normal_equation.compute_error(bundle), normal_equation);
        
        if (!lm.is_better())
          bundle.restore();
        
        verbose.at_end_bundle_adjustment_iteration(*this,lm,normal_equation);
        
      } while (!lm.stop());
      
      final_cost = std::min(lm.cost1(),lm.cost2());
      verbose.at_end_bundle_adjustment(*this,lm);
      
      return *this;
    }
  };
  
template<class F, int I, int J> struct Name<Solver<F,I,J>>
{
  static std::string name(){ return std::string("Solver<") + lma::name<F>() + ",#F(" + std::to_string(I) + "),#P(" + std::to_string(J) + ")>"; }  
};


}
