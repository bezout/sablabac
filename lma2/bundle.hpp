#pragma once

#include "container.hpp"
#include <map>

namespace lma
{
  
  template<class Functor, class Parameters, int NbInstanceOfFunctor_, int NbInstanceOfParameters_> struct Data
  {
    static constexpr int NbInstanceOfFunctor = NbInstanceOfFunctor_;
    static constexpr int NbInstanceOfParameters = NbInstanceOfParameters_;

    Container<Functor,NbInstanceOfFunctor> functors;
    
    Container<Parameters*,NbInstanceOfParameters> parameters;
    Container<Parameters,NbInstanceOfParameters> save_parameters;
    
    std::map<int,Parameters*> functor_map;
    std::set<Parameters*> parameters_set;
    
    void add(const Functor& f, Parameters* p)
    {
      functors.push_back(f);
      
      //TODO ADD F & P
      if (find(parameters_set.begin(),parameters_set.end(),p)==parameters_set.end())
      {
        parameters_set.insert(p);
        parameters.push_back(p); // <-------
      }
    }
    
    void update()
    {
      //TODO MAJ GRAPHE DE DONNEES
    }
    
    void update_parameters(const auto& deltas)
    {
      loop(deltas, parameters,[](auto& delta, auto& p){ update_parameters(*p,delta.data()); });
    }
    
    void save()
    {
      loop(parameters,save_parameters,[](auto& p, auto& sp) { sp = *p; } );
    }

    void restore()
    {
      loop(save_parameters, parameters,[](auto& sp, auto& p) { *p = sp; } );
    }
    
    void call_analytical(auto& jacobians) const
    {
      loop(functors,parameters,jacobians,[&](auto& f, auto& p, auto& j) { f.analytical(*p,j); });
    }
    
    template<class CastResidual>
    int compute_error(auto& errors, const Type<CastResidual>&) const
    {
      int success = 0;
      loop(functors,parameters,errors,[&](auto& f, auto& p, auto& e) {success += f(*p,CastResidual(e).data());});
      return success;
    }
  };
}
