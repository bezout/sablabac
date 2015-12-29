#include <iostream> 

template<int I> struct Int { enum {value=I}; int dyn;};

//static constexpr Int<5> dimension() { return {}; }
static constexpr Int<-1> dimension() { return Int<-1>{5}; }


template<class> struct Container;

template<> struct Container<Int<-1>>
{
  Container(){std::cout << " dynamic " << std::endl;}
};

template<int I> struct Container<Int<I>>
{
  Container(){std::cout << " static : " << I << std::endl;}
};

int main()
{
  Container<decltype(dimension())> c;
  return 0;
}
