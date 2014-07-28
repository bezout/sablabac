#include <iostream>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/bind.hpp>

void f(int a) {std::cout << " f " << a << std::endl;}

void f(int a, int b) {std::cout << " f " << a << " " << b << std::endl;}

void f(int a, int b, int c) {std::cout << " f " << a << " " << b << " " << c << std::endl;}

template<class V> V add_residual(const V& vector)
{
  return vector;
}

struct F
{
  void operator()(int a) const {f(a);}
  void operator()(int a, int b) const {f(a,b);}
  void operator()(int a, int b, int c) const {f(a,b,c);}
};

template<class F, class ...T> auto wrap(const F& f, T... args)
{
  boost::bind(&F::operator(),f,args...)();
  //boost::bind(&F::operator(),f,1);
  return 0;
}

int main()
{
  wrap(F(),1);
  wrap(F(),1,2);
  wrap(F(),1,2,3);
  
}
