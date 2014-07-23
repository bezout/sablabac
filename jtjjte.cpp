#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Eigenvalues>
#include <iostream>

using namespace Eigen;

  typedef Matrix<double,10,3> J;
  typedef Matrix<double,3,3> H;
  typedef Matrix<double,10,1> E;
  typedef Matrix<double,3,1> R;
  
  void solve1(J j, const E& e, double me)
  {
    R residu = j.transpose() * e * me;
    
    j *= std::sqrt(me);
    H h = j.transpose() * j;
    
    
    R result = h.fullPivHouseholderQr().solve(residu);
    std::cout << " resultat :"  << result.transpose() << std::endl;
  }
  
  void solve2(const J& j, const E& e, double me)
  {
    H h = (H::Identity()*me) * j.transpose() * j;
    R residu = j.transpose() * e * me;
    
    R result = h.fullPivHouseholderQr().solve(residu);
    std::cout << " resultat :"  << result.transpose() << std::endl;
  }
  
int main()
{
  J j = J::Random();
  E e = E::Random();
  double me = 10.0;
  solve1(j,e,me);
  solve2(j,e,me);
}
