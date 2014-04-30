#include <iostream>

template<class T> struct S
{
  S(){std::cout << " S<T>()" << std::endl;}
};

template<class R, class A, class B> struct S<R(A,B)>
{
  S(){std::cout << " S<R(A,B)>()" << std::endl;}
};

int main()
{
    S<int>();
    S<int(int,double)>();
}

