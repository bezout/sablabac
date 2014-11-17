#include <iostream>
#include <string>

template<class ... T> struct Container
{
  void disp() const { std::cout << std::endl;}
};

template<class A, class ... T> struct Container<A,T...> : Container<T...>
{
  A a;
  Container(A a_, T ... t):Container<T...>(t...),a(a_) {}
  void disp() const
  {
    std::cout << a << " ";
    Container<T...>::disp();
  }
};


template<class ... T> Container<T ... > f(T ... t)
{
  return Container<T ... >(t ...);
}

int main()
{
  
  auto container = f(1,1.2f,std::string("un virgule trois"),1.4);
  
  container.disp();
  
}
