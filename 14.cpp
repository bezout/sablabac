#include <iostream>
#include <string>

auto f = [](auto f1) { return f1; };

int main()
{
std::cout << f(f(f(f(f("pouet")+f(0))))) << std::endl;
}
