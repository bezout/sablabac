#pragma once

#include "exceptions.hpp"
#include <string>
#include <typeinfo>

namespace lma
{
  struct clement_type{};
  void clement(clement_type){}

  template<class T, class Enable = void> struct Name
  {
    static std::string name() { return typeid(T).name();}
  };

  template<class T> std::string name()
  {
    return Name<T>::name();
  }
   
  template<> struct Name<float> { static std::string name() { return "float";} };
  template<> struct Name<double> { static std::string name() { return "double";} };
   
  struct Adl{};

  template<class> struct Type {};
}
