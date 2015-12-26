#include "lma2/predef.hpp"

// PROBLEM DEFINITION

using namespace lma;

struct Parameters{ double x,y;};

std::ostream& operator<<(std::ostream& o, const Parameters& p) { return o << "Parameters(" << p.x << "," << p.y << ")"; }

template<class Float> void resize(Eigen::Matrix<Float,1,2>&, size_t) {}
template<class Float> void resize(Eigen::Matrix<Float,1,Eigen::Dynamic>& mat, size_t n) { mat.resize(n); }

struct Rosenbrock
{
  bool operator()(const Parameters& p, double& r) const
  {
    auto& x = p.x;
    auto& y = p.y;
    r = (1.0 - x) * (1.0 - x) + 100.0 * (y - x * x) * (y - x * x);
    return true;
  }

  void analytical(const Parameters& p, auto mat) const
  {
    //resize(mat,2);
    auto& x = p.x;
    auto& y = p.y;
    mat(0,0) = -2.0 * (1.0 - x) - 200.0 * (y - x * x) * 2.0 * x;
    mat(0,1) = 200.0 * (y - x * x);
  }
};

//constexpr int size(const auto&) { return -1 ;}
constexpr int size(Type<Parameters>) { return 2; }
constexpr int size(Type<double>) { return 1; }

void apply_increment(Parameters& parameters, const auto* delta)
{
  //static_assert( size(Type<Parameters>{}) == 2 , "size(Type<Parameters>{})==2");
  parameters.x += delta[0];
  parameters.y += delta[1];
}

namespace lma
{
  template<> struct Name<Parameters> { static std::string name() { return "Parameters"; } };
  template<> struct Name<Rosenbrock> { static std::string name() { return "Rosenbrock"; } };
  
  /*
  template<> struct DDL<Parameters,2>
  {
    void update_parameters(Parameters& parameters, const auto* delta)
    {
      parameters.x += delta[0];
      parameters.y += delta[1];
    }
  };*/
}

#include "lma2/solver.hpp"



void test_static(auto lm)
{
  Parameters parameters {0.9,0.9};
  Solver<Rosenbrock>::SetNbInstanceOfFunctors<2>::type::SetNbInstanceOfParameters<1>::type()
    .add(Rosenbrock{},&parameters)
    .add(Rosenbrock{},&parameters)
    .solve(lm,Verbose{});
}

void test_dynamic(auto lm)
{
  Parameters parameters {0.9,0.9};
  Solver<Rosenbrock>()
    .add(Rosenbrock{},&parameters)
    .add(Rosenbrock{},&parameters)
    .solve(lm,Verbose{});
}

int main()
{
  int nb_iteration = 15;
  
  test_static(LMN<double>{nb_iteration,1.});
  test_dynamic(LMN<double>{nb_iteration,1.});
  
/*
  test_static(LM<double>{nb_iteration,1.});
  test_dynamic(LM<double>{nb_iteration,1.});
  test_static(LM<float>{nb_iteration,1.f});
  test_dynamic(LM<float>{nb_iteration,1.f});
  
  test_static(LMN<double>{nb_iteration,1.});
  test_dynamic(LMN<double>{nb_iteration,1.});
  test_static(LMN<float>{nb_iteration,1.f});
  test_dynamic(LMN<float>{nb_iteration,1.f});
*/

}

// float,double
// dynamic,static
// LM,LMN


/*
 *  PLAN :
 *   avec n/m statique ou dynamique
 *   1 : n instances d'1 type d'erreur pour 1 instance d'1 type de paramètre
 *   2 : n instances d'1 type d'erreur pour m instances d'1 type de paramètre
 *   3 : n[...] instances de N types d'erreur pour m instances d'1 type de paramètre
 *   4 : n[...] instances de N types d'erreur pour m[...] instances de M types de paramètre
 * 
 * update_policy ?
 */

// reset && time g++-5 simple_lma.cpp -std=c++1y -I/home/datta/develop/root/include/eigen3 -isystem/home/datta/develop/root/include/eigen3 -lboost_system -lboost_chrono && ./a.out
