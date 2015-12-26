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
    
    //TODO  HESSIAN DENSE ?
    HessianBlock h;
    
    Matrix<JacobianBlock,NbInstanceOfFunctor> j;
    Container<ResidualBlock,NbInstanceOfFunctor> e;

    //TODO  VECTOR DENSE ?    
    Container<DeltaBlock,NbInstanceOfParameters> delta;
    Container<DeltaBlock,NbInstanceOfParameters> jte;

    static void disp()
    {/*
      std::cout << " Hessian  : " << name<Hessian>() << std::endl;
      std::cout << " Jacobian : " << name<Jacobian>() << std::endl;
      std::cout << " Delta    : " << name<Delta>() << std::endl;
      std::cout << " Residual : " << name<EquationResidual>() << std::endl;*/
    }

    std::pair<Float,int> compute_error(const auto& bundle)
    {
      int success = bundle.compute_error(e, Type<FinalResidual>{});
      Float total_error = 0;
      for(auto& x : e)
        total_error += squared_norm(x);
      return {total_error/2.0,success};
    }

    Container<DeltaBlock,NbInstanceOfParameters>& solve(const auto& lm, const auto& bundle)
    {
      compute_jacobian(bundle);
      return solve_delta(lm);
    }
    
    Float compute_scale(Float lambda) const
    {
      Float scale = 0.;
      loop(delta,jte,[&](auto& d, auto& jte_)
      {
        for (size_t j=0; j < rows(d); j++)
          scale += d(j) * (lambda * d(j) + jte_(j));// SIMPLIFIABLE AVEC VECTEUR DENSE ?
      });
      return scale;
    }
    
    private:

      void compute_jacobian(const auto& bundle)
      {
        bundle.call_analytical(j);
      }

      HessianBlock& damping(HessianBlock& h, const Float& lambda)
      {
        for(int i = 0 ; i < cols(h) ; ++i)
          h(i,i) += lambda;
        return h;
      }
    
      Container<DeltaBlock,NbInstanceOfParameters>& solve_delta(const auto& lm)
      {
        // MULTI F / P
        jte.resize(e.size());
        delta.resize(e.size());
        assert( begin(j)     != end(j) );
        assert( begin(e)     != end(e) );
        assert( begin(jte)   != end(jte) );
        assert( begin(delta) != end(delta) );
        
        auto& x      = *begin(j);
        auto& err    = *begin(e);
        auto& jte_   = *begin(jte);
        auto& delta_ = *begin(delta);
        
        damping(h = x.transpose() * x,lm.lambda);
        jte_ = -x.transpose()*err;
        delta_ = llt(h,jte_, NbParameters);
        return delta;
        //return delta = llt( damping(h = x.transpose()*x, lm.lambda), jte = -x.transpose()*err, NbParameters);
        //return delta = ( damping(h = j.transpose()*j, lm.lambda) ).llt().solve(jte = -j.transpose()*e);
      }
  };
  
}

