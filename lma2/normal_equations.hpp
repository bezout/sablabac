#pragma once

#include "container.hpp"
#include "matrix.hpp"

namespace lma
{
  
  template<class Float, class InfoFunctor, int NbInstanceOfFunctor_, int NbInstanceOfParameters_>
  struct NormalEquation
  {

    using Parameters      = typename InfoFunctor::Parameters;
    using FunctorResidual = typename InfoFunctor::Residual;
   
    static constexpr int NbInstanceOfFunctor = NbInstanceOfFunctor_;
    static constexpr int NbInstanceOfParameters = NbInstanceOfParameters_;

    // ddl pour un bloc de parametre
    static constexpr size_t NbParameters = size(Type<Parameters>{});
    
    // dimension d'un bloc d'erreur
    static constexpr size_t NbError = size(Type<FunctorResidual>{});
    
    using HessianBlock          = Block<Float,NbParameters,NbParameters>;
    using JacobianBlock         = Block<Float,NbError,NbParameters>;
    using DeltaBlock            = Block<Float,NbParameters,1>;
    
    using ResidualBlock = Block<Float,NbError,1>;
    using FinalResidual = CastResidual<ResidualBlock,FunctorResidual>;
    
    using Hessian = Matrix<HessianBlock,NbInstanceOfParameters>;
    using Jacobian = Matrix<JacobianBlock,NbInstanceOfFunctor>;
    
    using Delta = Vector<DeltaBlock,NbInstanceOfParameters>;
    using JTE = Vector<DeltaBlock,NbInstanceOfParameters>;
    
    using Residuals = Vector<ResidualBlock,NbInstanceOfFunctor>;
    
    
    Jacobian jacobian;
    Residuals residuals;
    Hessian hessian;
    Delta delta;
    JTE jte;

    static void disp()
    {/*
      std::cout << " Hessian  : " << name<Hessian>() << std::endl;
      std::cout << " Jacobian : " << name<Jacobian>() << std::endl;
      std::cout << " Delta    : " << name<Delta>() << std::endl;
      std::cout << " Residual : " << name<EquationResidual>() << std::endl;*/
    }

    void resize(const auto& bundle)
    {
      jacobian.resize(bundle.total_errors(),bundle.total_parameters());
      hessian.resize(bundle.total_parameters(),bundle.total_parameters());
      delta.resize(bundle.total_parameters());
      jte.resize(bundle.total_parameters());
      residuals.resize(bundle.total_errors());
    }

    std::pair<Float,int> compute_error(const auto& bundle)
    {
      int success = bundle.compute_error(residuals, Type<FinalResidual>{});
      Float total_error = 0;
      total_error += squared_norm(residuals);
      return {total_error/2.0,success};
    }

    Delta& solve(const auto& lm, const auto& bundle)
    {
      compute_jacobian(bundle);
      return solve_delta(lm);
    }
    
    Float compute_scale(Float lambda) const
    {
      return delta.dot(lambda * delta + jte);
    }
    
    private:

      void compute_jacobian(const auto& bundle)
      {
        bundle.call_analytical(jacobian);
      }

      Delta& solve_delta(const auto& lm)
      {
        delta = llt(damping(hessian = jacobian.transpose() * jacobian, lm.lambda), jte = -jacobian.transpose() * residuals, hessian.cols());
        return delta;
      }
  };
  
}

