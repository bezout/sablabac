#include "lma2/predef.hpp"

// PROBLEM DEFINITION

using namespace lma;

struct Parameters{ double x,y;};

std::ostream& operator<<(std::ostream& o, const Parameters& p) { return o << "Parameters(" << p.x << "," << p.y << ")"; }

template<class Float> void resize(Eigen::Matrix<Float,1,2>&, size_t) {}
template<class Float> void resize(Eigen::Matrix<Float,1,Eigen::Dynamic>& mat, size_t n) { mat.resize(n); }

struct Rosenbrock
{
  double sigma;
  bool operator()(const Parameters& p, double& r) const
  {
    auto& x = p.x;
    auto& y = p.y;
    r = (1.0 - x) * (1.0 - x) + 100.0 * (y - x * x) * (y - x * x);
    r *= sigma;
    return true;
  }

  void analytical(const Parameters& p, auto mat) const
  {
    //resize(mat,2);
    auto& x = p.x;
    auto& y = p.y;
    mat(0,0) = -2.0 * (1.0 - x) - 200.0 * (y - x * x) * 2.0 * x;
    mat(0,1) = 200.0 * (y - x * x);
    mat *= sigma;
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



void test_static(auto lm, auto verbose)
{
  Parameters parameters {0.9,0.9};
  Solver<Rosenbrock>::SetNbInstanceOfFunctors<3>::type::SetNbInstanceOfParameters<1>::type()
    .add(Rosenbrock{1},&parameters)
    .add(Rosenbrock{2},&parameters)
    .add(Rosenbrock{3},&parameters)
    .solve(lm,verbose);
}

void test_dynamic(auto lm, auto verbose)
{
  Parameters parameters {0.9,0.9};
  Solver<Rosenbrock>()
    .add(Rosenbrock{1},&parameters)
    .add(Rosenbrock{2},&parameters)
    .add(Rosenbrock{3},&parameters)
    .solve(lm,verbose);
}

int main()
{
  int nb_iteration = 2;
  //auto verbose = VerboseNormalEquation{};
  auto verbose = Verbose{};
  test_static(LMN<float>{nb_iteration,1.f},verbose);
  test_static(LMN<double>{nb_iteration,1.},verbose);
  test_dynamic(LMN<double>{nb_iteration,1.},verbose);
  
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
 *   TOUT FAIRE EN DENSE
 *   avec n/m statique ou dynamique
 *   1 : n instances d'1 type d'erreur pour 1 instance d'1 type de paramètre
 *   2 : n instances d'1 type d'erreur pour m instances d'1 type de paramètre
 *   3 : n[...] instances de N types d'erreur pour m instances d'1 type de paramètre
 *   4 : n[...] instances de N types d'erreur pour m[...] instances de M types de paramètre
 * 
 *   PUIS EN EPARSE
 * 
 * 
 * update_policy ?
 */

// reset && time g++-5 simple_lma.cpp -std=c++1y -I/home/datta/develop/root/include/eigen3 -isystem/home/datta/develop/root/include/eigen3 -lboost_system -lboost_chrono && ./a.out
