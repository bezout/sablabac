#include <iostream>

namespace adl
{
  struct adl_{};
  
  template<class T> void f(T t)
  {
    g(t,adl_());
  }

  void g(int, adl_)
  {
    std::cout << "int" << std::endl;
  }

  void g(double, adl_)
  {
    std::cout << "double" << std::endl;
  }
}

int main()
{
  adl::f(1);
  adl::f(1.0);
}
