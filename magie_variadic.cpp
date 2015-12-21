#include <iostream>
#include <tuple>

#include <initializer_list>
#include <functional>
#include <utility>
#include <sstream>

template<class T> std::string msg(T t)
{
  std::stringstream str;
  str << " " << typeid(t).name() << "=" << t;
  return str.str();
}

template<class Tuple, std::size_t... I>
void print_impl(std::ostream& o, const Tuple & t, std::index_sequence<I...>)
{
  using swallow = int[];
  (void)swallow{0, (void(o << (I == 0? "" : ", ") << msg(std::get<I>(t))), 0)...};
  std::cout << std::endl;
}

template<class ... T>
void print(std::ostream& o, const std::tuple<T...>& tuple)
{
  print_impl(o,tuple,std::index_sequence_for<T...>{});
}

int main()
{
  typedef std::tuple<bool,char,unsigned char,int,unsigned int,float,double> Tuple;

  Tuple tuple = std::make_tuple(false,'A','B',-1,2,1.2f,1.3);

  print(std::cout,tuple);
}
