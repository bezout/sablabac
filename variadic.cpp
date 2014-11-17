#include <sstream>
#include <iostream>

std::string disp();

template<class T, class ... List>
std::string disp(T t, List ... list)
{
  std::stringstream str;
  str << t;
  return str.str() + std::string(" ") + disp(list...);
}

std::string disp()
{
  return std::string();
}

template<class ... List> struct Tuple {};

template<class T, class ... List> struct Tuple<T,List...> : Tuple<List...>
{
  T objet;
  Tuple(T t, List ... list): Tuple<List...>(list...),objet(t) {}
  
  void display() const
  {
    std::cout << objet << " ";
    Tuple<List...>::display();
  }
};

template<> struct Tuple<>
{
  Tuple(){}
  
  void display() const
  {
    std::cout << std::endl;
  }
};

template<class ... List>
Tuple<List...> make_tuple(List ... list)
{
  return Tuple<List...>(list...);
}

int main()
{
  std::cout << disp() << std::endl;
  std::cout << disp(1,2.0) << std::endl;
  std::cout << disp(1,2.0,'a') << std::endl;
  std::cout << disp(1,2.0,'+',"Laurent") << std::endl;
  
  make_tuple(1,2.0,'+',"Laurent").display();
}
