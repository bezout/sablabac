#include <iostream>

template<class T, size_t N>
constexpr size_t truc(T(&)[N])
{
    return N;
}


int main()
{
int a[5];
std::cout << " size : " << truc(a) << std::endl;;
}

