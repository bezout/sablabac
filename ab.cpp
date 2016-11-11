#include <iostream>
#include <typeinfo>
#include <tuple>


template<typename ...> struct List {};

struct True{};
struct False{};
template<bool> struct Inf : True {};
template<> struct Inf<false> : False {};

auto if_then_else(True, auto a, auto) { return a; }
auto if_then_else(False , auto, auto b) { return b; }

template<int I> struct Int{};
static const Int<1> _1;
template<int I, int J> Int<I+J> operator+(Int<I>, Int<J>){return {};}

template<typename N> auto iterate(N,N, auto l, auto f) { return f(l); }
auto iterate(auto I, auto N, auto l, auto f) { return iterate(I+_1,N,f(l),f); }
template<int N> auto repeat(auto l, auto f) { return iterate(Int<0>{},Int<N>{},l,f); }

template<typename A, typename ... R> List<R...,A> bubble_sort_impl(List<A>, List<R...>) { return {}; }

template<typename A, typename B, typename ... C, typename ... R> auto bubble_sort_impl(List<A,B,C...>, List<R...>)
{
  return if_then_else(
    Inf<(sizeof(A)<=sizeof(B))>{},
    bubble_sort_impl(List<B,C...>{}, List<R...,A>{}),
    bubble_sort_impl(List<A,C...>{}, List<R...,B>{})
  );
}

template<typename ... T> auto sort(List<T...> l)
{
  auto bubble_sort = [](auto l) { return bubble_sort_impl(l,List<>{}); };
  return repeat<sizeof...(T)>(l,bubble_sort);
}

template<typename ... T> std::tuple<T...> as_tuple(List<T...>) { return {}; }

template<typename ... T> auto sort(std::tuple<T...>)
{
  return as_tuple(sort(List<T...>{}));
}


struct TripleDouble{ double a,b,c;};

void disp(int) { std::cout << "int";}
void disp(double) { std::cout << "double";}
void disp(long double) { std::cout << "long double";}
void disp(char) { std::cout << "char";}
void disp(float) { std::cout << "float";}
void disp(short int) { std::cout << "short int";}
void disp(TripleDouble) { std::cout << "double*3";}
template<typename T> void disp(T) {}//std::cout << typeid(T).name();}
void disp(List<>){}

template<template <typename ...> typename List, typename A, typename ... B> void disp(List<A, B...>)
{
  std::cout << " "; disp(A{}); std::cout << "(" << sizeof(A) << "),";
  disp(List<B...>{});
}

int main()
{
  using L1 = List<long double,double,int,TripleDouble,short int,char,float>;
  auto l = L1{};
  std::cout << " From       : ";disp(l);std::cout << std::endl;  
  auto sorted = sort(l);
  std::cout << " To         : ";  disp(sorted);;std::cout << std::endl;  
  
  std::tuple<long double,double,int,TripleDouble,short int,char,float> tuple;
  std::cout << " From tuple : ";disp(tuple);std::cout << std::endl;
  std::cout << " To tuple   : ";disp(sort(tuple));std::cout << std::endl;
}

