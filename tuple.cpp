#include <tuple>
#include <iostream>

template<int I> struct Int{};

template<int F, class ... T> std::ostream& disp(std::tuple<T...> tuple, std::ostream& o, Int<F>,Int<F>)
{
 return o << "#";
}

template<int I, int F, class ... T> std::ostream& disp(std::tuple<T...> tuple, std::ostream& o, Int<I>, Int<F>)
{
  return disp(tuple,(o << std::get<I>(tuple) << " "),Int<I+1>(),Int<F>());
}

template<class ... T> std::ostream& operator<<(std::ostream& o, const std::tuple<T...>& tuple)
{
  typedef Int< std::tuple_size<std::tuple<T...>>::value > F;
  return disp(tuple,o,Int<0>(),F());
}

int main()
{
  std::cout << std::make_tuple(0,1.1,'a',"tuple",true) << std::endl;
}
