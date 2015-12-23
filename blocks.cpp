#include <vector>
#include <array>
#include <tuple>
#include <iostream>



template<int I> struct Int {};

template<> struct Int<-1> { int value; };
using IntD = Int<-1>;

IntD operator+(IntD i, int inc) { return IntD{i.value+inc}; }
bool operator!=(IntD i, IntD j) { return i.value!=j.value; }

template<int I, int J>
Int<I+J> operator+(Int<I>, Int<J>) { return {}; }

template<class T> const T& at(const std::vector<T>& v, IntD i) { return v.at(i.value); }

template<class T, size_t S, int I> const T& at(const std::array<T,S>& a, Int<I>) { return std::get<I>(a); }
template<class ... T, int I> const typename std::tuple_element<I,std::tuple<T...>>::type& at(const std::tuple<T...>& t, Int<I>) { return std::get<I>(t); }


void loop(IntD i, IntD n, auto& container, auto f)
{
  f(at(container,i));
  if (i + 1 !=n)
    loop(i+1,n,container,f);
}


template<int I, int F>
void loop(Int<I> i, Int<F> n, auto& container, auto f)
{
  f(at(container,i));
  loop( i + Int<1>{} ,n,container,f);
}

template<int F> void loop(Int<F> i, Int<F> n, auto& container, auto f) {}



int main()
{

  std::vector<int>  v = {1,2,3,4};
  std::array<int,4> a = {1,2,3,4};
  std::tuple<int,int,int,int> t = std::make_tuple(1,2,3,4);
  
  auto f = [](auto x){ std::cout << x << ", ";};
  
  std::cout << " vector : ";
  loop(Int<-1>{0},Int<-1>{4},v,f);
  std::cout << std::endl;
  
  std::cout << " array  : ";
  loop(Int<0>{},Int<4>{},a,f);
  std::cout << std::endl;

  std::cout << " tuple  : ";
  loop(Int<0>{},Int<4>{},t,f);
  std::cout << std::endl;

}

