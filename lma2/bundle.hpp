#pragma once

#include "container.hpp"
#include <map>
#include <boost/function.hpp>

namespace lma
{
  
  template<class Functor, class Parameters, int NbInstanceOfFunctor_, int NbInstanceOfParameters_> struct Data
  {
    static constexpr int NbInstanceOfFunctor = NbInstanceOfFunctor_;
    static constexpr int NbInstanceOfParameters = NbInstanceOfParameters_;

    Container<std::pair<Functor,Parameters*>,NbInstanceOfFunctor> functors;
    
    
    Container<Parameters*,NbInstanceOfParameters> parameters;
    Container<Parameters,NbInstanceOfParameters> save_parameters;
    
    std::set<Parameters*> parameters_set;

    void add(const Functor& f, Parameters* p)
    {
      functors.push_back(std::make_pair(f,p));
      
      
      //TODO ADD F & P
      if (find(parameters_set.begin(),parameters_set.end(),p)==parameters_set.end())
      {
        parameters_set.insert(p);
        parameters.push_back(p);
      }
    }
    
    void update()
    {
      parameters_set.clear();
      //TODO MAJ GRAPHE DE DONNEES
    }
    
    void update_parameters(const auto& deltas)
    {
      for(size_t i = 0 ; i < parameters.size() ; ++i)
        apply_increment(*parameters[i],deltas[i].data());
    }
    
    void save()
    {
      save_parameters.resize(parameters.size());
      for(size_t i = 0 ; i < parameters.size() ; ++ i)
        save_parameters[i] = *parameters[i];
    }

    void restore()
    {
      for(size_t i = 0 ; i < parameters.size() ; ++ i)
        *parameters[i] = save_parameters[i];
    }
    
    void call_analytical(auto& jacobians) const
    {
      jacobians.resize(functors.size());
      for(size_t i = 0 ; i < functors.size() ; ++i)
        functors[i].first.analytical(*functors[i].second,jacobians[i]);
    }
    
    template<class CastResidual>
    int compute_error(auto& errors, const Type<CastResidual>&) const
    {
      int success = 0;
      errors.resize(functors.size());
      for(size_t i = 0 ; i < functors.size() ; ++i)
        success += functors[i].first(*functors[i].second,CastResidual(errors[i]).data());
      return success;
    }
  };
}
