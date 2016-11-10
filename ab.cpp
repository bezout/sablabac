#include <iostream>
#include <typeinfo>

template<typename ...> struct List {};


void disp(List<>) {}
template<typename A, typename ... B> void disp(List<A, B...>)
{
  std::cout << " sizeof(" << typeid(A).name() << ")=" << sizeof(A) << std::endl;
  disp(List<B...>{});
}

struct True{};
struct False{};

auto if_then_else(True, auto a, auto b) { return a; }
auto if_then_else(False , auto a, auto b) { return b; }



template<bool b> struct Inf : True {};
template<> struct Inf<false> : False {};

template<typename A, typename ... R> auto reverse_impl(List<A>, List<R...>)
{
  return List<A,R...>{};
}

template<typename A, typename B, typename ... C, typename ... R> auto reverse_impl(List<A,B,C...>, List<R...>)
{
  return if_then_else(
      Inf<(sizeof(A)>sizeof(B))>{},
      reverse_impl(List<B,C...>{}, List<A,R...>{}),
      reverse_impl(List<A,C...>{}, List<B,R...>{})
    );
}

template<typename ... T> auto reverse(List<T...> L, auto R)
{
  return reverse_impl(reverse_impl(reverse_impl(reverse_impl(reverse_impl(L,R),R),R),R),R);
}

int main()
{
  using L1 = List<long double,double,int,short int,char>;
  std::cout << " L1 : " << std::endl;
  disp(L1{});
  
  std::cout << " L2 : " << std::endl;
  disp(reverse(L1{},List<>{}));
}

