#include <tuple>
#include <iostream>

auto f()
{
  return std::make_tuple(1.0,2,'c');
}

int main()
{
  double a;
  int b;
  char c;
  std::tie(a,b,c) = f();
  std::cout << a << " ; " << b << " ; " << c << std::endl;
  return 0;
}
