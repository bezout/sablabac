#include "lma2/predef.hpp"

// PROBLEM DEFINITION

using namespace lma;

struct Parameters{ double x,y;};

std::ostream& operator<<(std::ostream& o, const Parameters& p) { return o << "Parameters(" << p.x << "," << p.y << ")"; }

template<class Float> void resize(Eigen::Matrix<Float,1,2>&, size_t) {}

template<class Float> void resize(Eigen::Matrix<Float,1,Eigen::Dynamic>& mat, size_t n) { mat.resize(n); }

struct Reprojection
{
  bool operator()(const Parameters& p, double& r) const
  {
    auto& x = p.x;
    auto& y = p.y;
    r = 0.5*x*x*x + 10.0*y*y;
    return true;
  }

  void analytical(const Parameters& p, auto& mat) const
  {
    resize(mat,2);
    auto& x = p.x;
    auto& y = p.y;
    mat[0] = 0.5*3.0*p.x*p.x;
    mat[1] = 10.0*2.0*p.y;
  }
};

struct Rosenbrock
{
  bool operator()(const Parameters& p, double& r) const
  {
    auto& x = p.x;
    auto& y = p.y;
    r = (1.0 - x) * (1.0 - x) + 100.0 * (y - x * x) * (y - x * x);
    return true;
  }

  void analytical(const Parameters& p, auto& mat) const
  {
    resize(mat,2);
    auto& x = p.x;
    auto& y = p.y;
    mat[0] = -2.0 * (1.0 - x) - 200.0 * (y - x * x) * 2.0 * x;
    mat[1] = 200.0 * (y - x * x);
  }
};

constexpr size_t size(Type<Parameters>) { return 2; }
constexpr size_t size(Type<double>) { return 1; }

void update_parameters(Parameters& parameters, auto delta[size(Type<Parameters>{})])
{
  //static_assert( size(Type<Parameters>{}) == 2 , "size(Type<Parameters>{})==2");
  parameters.x += delta[0];
  parameters.y += delta[1];
}

namespace lma
{
  template<> struct Name<Reprojection> { static std::string name() { return "Reprojection"; } };
  template<> struct Name<Parameters> { static std::string name() { return "Parameters"; } };
}

#include "lma2/solver.hpp"



int main()
{
  Parameters parameters {0.9,0.9};
  Solver<Rosenbrock> solver;
  solver.add(Rosenbrock{},&parameters);
  
  solver.solve(LM<double>{100,1},Verbose{});//.solve(LM<double>{15,1},Verbose{});
  /*
  parameters = {-1,-1};
  solver.solve(LM<double>{},Verbose{});
  */
}

// float,double
// dynamic,static
// update_policy ?


// reset && g++-5 simple_lma.cpp -std=c++1y -I/home/datta/develop/root/include/eigen3 -isystem/home/datta/develop/root/include/eigen3 && ./a.out
