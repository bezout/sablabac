#pragma once

#include "predef.hpp"

namespace lma
{
  template<class From, class To> struct CastResidual
  {
    From& original;
    To temporary;
    To& data() { return temporary; }
    ~CastResidual() { original = From(temporary) ; }
    CastResidual(From& original_):original(original_) {}
  };

  template<class From> struct CastResidual<From,From>
  {
    From& original;
    From& data() { return original; }
    CastResidual(From& original_):original(original_) {}
  };


  template<class Float, class InfoFunctor>
  struct NormalEquation
  {
    using Parameters      = typename InfoFunctor::Parameters;
    using FunctorResidual = typename InfoFunctor::Residual;
    
    static constexpr size_t NbParameters = size(Type<Parameters>{});
    static constexpr size_t NbError = size(Type<FunctorResidual>{});
    
    using Hessian          = Matrix<Float,NbParameters,NbParameters>;
    using Jacobian         = Matrix<Float,NbError,NbParameters>;
    using Delta            = Matrix<Float,NbParameters,1>;
    
    using EquationResidual = Matrix<Float,NbError,1>;
    using CResidual = CastResidual<EquationResidual,FunctorResidual>;
    
    Hessian h;
    Jacobian j;
    Delta delta;
    Delta jte;
    EquationResidual e;
    
    static void disp()
    {
      std::cout << " Hessian  : " << name<Hessian>() << std::endl;
      std::cout << " Jacobian : " << name<Jacobian>() << std::endl;
      std::cout << " Delta    : " << name<Delta>() << std::endl;
      std::cout << " Residual : " << name<EquationResidual>() << std::endl;
    }

    std::pair<Float,int> compute_error(const auto& bundle)
    {
      bool success = bundle.f(*bundle.parameters,CResidual(e).data());
      return {squared_norm(e)/2.0,success ? 1 : 0};
    }

    Delta& solve(const auto& lm, const auto& bundle)
    {
      compute_jacobian(bundle);
      return solve_delta(lm);
    }
    
    Float compute_scale(Float lambda) const
    {
      Float scale = 0.;
      for (size_t j=0; j < rows(delta); j++)
        scale += delta(j) * (lambda * delta(j) + jte(j));
      return scale;
    }
    
    private:
      void compute_jacobian(const auto& bundle)
      {
        bundle.call_analytical(j);
      }
          
      Hessian& damping(Hessian& h, const Float& lambda)
      {
        for(int i = 0 ; i < cols(h) ; ++i)
          h(i,i) += lambda;
        return h;
      }
    
      Delta& solve_delta(const auto& lm)
      {
        return delta = llt( damping(h = j.transpose()*j, lm.lambda), jte = -j.transpose()*e, NbParameters);
        //return delta = ( damping(h = j.transpose()*j, lm.lambda) ).llt().solve(jte = -j.transpose()*e);
      }
  };


  template<class Parameters, class F> struct Bundle0
  {
    Parameters* parameters = nullptr;
    F f;
    Parameters save_parameters;
    
    void save() { save_parameters = *parameters; }
    void restore() { *parameters = save_parameters; }

    void call_analytical(auto& j) const
    {
      f.analytical(*parameters,j);
    }
  };

  template<class Float> struct LM
  {
    size_t max_iteration = 10;
    Float lambda = 0.001;
    Float eps = 0.99999;
    
    std::pair<Float,int> error1,error2;
    
    Float cost1() const { return error1.first; }
    Float cost2() const { return error2.first; }
    
    Float rms1() const { return std::sqrt(cost1() / Float(error1.second)); }
    Float rms2() const { return std::sqrt(cost2() / Float(error2.second)); }
    
    size_t nb_iteration = 0;

    
    bool stop() const { return (nb_iteration == max_iteration) || (is_better() && (rms2() > eps * rms1())); }
    bool is_better() const { return cost2() < cost1(); }

    void update(std::pair<Float,int> current_error, const auto&)
    {
      if (is_better())// was better
        error1 = error2;

      error2 = current_error;

      lambda = is_better() ? std::max(lambda/10.0,1e-10) : std::min(lambda*10.0,1e30);
      nb_iteration ++;
    }
  };
  
  template<class Float> struct LMN
  {
    size_t max_iteration = 10;
    Float lambda = 0.001;
    Float eps = 0.99999;
    
    double _goodStepLowerScale = 1./3.; ///< lower bound for lambda decrease if a good LM step
    double _goodStepUpperScale = 2./3.; ///< upper bound for lambda decrease if a good LM step
    double v = 2.;
    
    std::pair<Float,int> error1,error2;
    
    Float cost1() const { return error1.first; }
    Float cost2() const { return error2.first; }
    
    Float rms1() const { return std::sqrt(cost1() / Float(error1.second)); }
    Float rms2() const { return std::sqrt(cost2() / Float(error2.second)); }
    
    size_t nb_iteration = 0;

    
    bool stop() const { return (nb_iteration == max_iteration) || (is_better() && (rms2() > eps * rms1())); }
    bool is_better() const { return cost2() < cost1(); }

    void update(std::pair<Float,int> current_error, const auto& normal_equation)
    {
      if (is_better())// was better
        error1 = error2;

      error2 = current_error;
      
      double rho = 2.0*(cost1()-cost2()) / (normal_equation.compute_scale(lambda) + 1e-3);
      //std::cout << " dcost : " << cost1()-cost2() << " ; rho = " << rho << " ; scale = " << normal_equation.compute_scale(lambda) << std::endl;

      if (is_better())
      {
        double alpha = 1. - lma::pow(2.*rho-1.,3);
        double scaleFactor = (std::max)(_goodStepLowerScale, (std::min)(alpha, _goodStepUpperScale));
        lambda *= scaleFactor;
        v = 2;
      }
      else
      {
        lambda*=v;
        v *= 2;
      }
      nb_iteration ++;
    }
  };
  

  template<class F> struct Solver
  {
    double initial_cost, final_cost;
    using InfoFunctor = AnalyseFunctor<F>;
    using Parameters  = typename InfoFunctor::Parameters;
    using Residual    = typename InfoFunctor::Residual;
    using Bundle      = Bundle0<Parameters,F>;
    
    Bundle bundle;
    
    Solver& add(const F& f_, Parameters* parameters_)
    {
      bundle = {parameters_, f_};
      return *this;
    }
    
    template<template<class Policy> class Policy, class Float, class Verbose=DefaultVerbose>
    Solver& solve(Policy<Float> lm, Verbose verbose = Verbose{})
    {
      using NormalEq = NormalEquation<Float,InfoFunctor>;
      NormalEq normal_equation;
      lm.error1 = lm.error2 = normal_equation.compute_error(bundle);
      initial_cost = lm.cost1();
      
      verbose.at_begin_bundle_adjustment(*this,lm);
      
      do
      {
        verbose.at_begin_bundle_adjustment_iteration(*this,lm);

        bundle.save();

        auto& delta = normal_equation.solve(lm,bundle);

        update_parameters(*bundle.parameters,delta.data());
        
        lm.update( normal_equation.compute_error(bundle), normal_equation);
        
        if (!lm.is_better())
          bundle.restore();
        
        verbose.at_end_bundle_adjustment_iteration(*this,lm);
        
      } while (!lm.stop());
      
      final_cost = std::min(lm.cost1(),lm.cost2());
      verbose.at_end_bundle_adjustment(*this,lm);
      
      return *this;
    }
  };

}
