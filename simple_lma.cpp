#include <Eigen/Core>
#include <Eigen/Cholesky>
#include <iostream>

struct clement_type{}; void clement(clement_type){}

struct Adl{};

template<class> struct Type{};

template<class> struct AnalyseExtractFromType;
template<class Return, class F, class P, class R> struct AnalyseExtractFromType< Return (F::*) (const P&, R&) const >
{
  typedef P Parameters;
  typedef R Residual;
};

template<class F> struct AnalyseFunctor : AnalyseExtractFromType<decltype(&F::operator())> {};

template<class Float, size_t I, size_t J> struct CreateMatrix
{
  using Mat = Eigen::Matrix<Float,I,J>;
};

template<class Float, size_t I, size_t J> using Matrix = typename CreateMatrix<Float,I,J>::Mat;

template<class Float, size_t NbParameters, size_t NbError>
struct NormalEquation
{
  using Hessian = Matrix<Float,NbParameters,NbParameters>;
  using Jacobian = Matrix<Float,NbError,NbParameters>;
  using Residual = Matrix<Float,NbError,1>;
  using Delta = Matrix<Float,NbParameters,1>;
  
  Hessian h;
  Jacobian j;
  Residual e;
  Delta delta;
  
  void compute_jacobian(const auto& bundle)
  {
    bundle.call_analytical(j);
  }

  std::pair<Float,bool> compute_error(const auto& bundle)
  {
    bool success = bundle.f(*bundle.parameters,e[0]);
    return {e.squaredNorm(),success};
  }
  
  Delta& solve()
  {
    return delta = (j.transpose() * j + Hessian::Identity()*0.001).llt().solve(-j.transpose()*e);
  }
};

template<class Parameters, class F> struct Bundle0
{
  Parameters* parameters = nullptr;
  F f;
  
  void call_analytical(auto& j) const
  {
    f.analytical(*parameters,j);
  }
};

template<class F> struct Solver
{
  using InfoFunctor = AnalyseFunctor<F>;
  using Parameters  = typename InfoFunctor::Parameters;
  using Residual    = typename InfoFunctor::Residual;
  using Bundle      = Bundle0<Parameters,F>;
  
  Bundle bundle;
  
  Solver& add(const F& f_, Parameters* parameters_)
  {
    bundle = {parameters_, f_};
    return *this;
  }
  
  template<class Float>
  Solver& solve(Type<Float>)
  {
    NormalEquation<Float,size(Type<Parameters>{}),size(Type<Residual>{})> normal_equation;
    std::pair<Float,bool> pair = normal_equation.compute_error(bundle);
    std::cout << " erreur initial : " << pair.first << std::endl;
    
    do
    {
      normal_equation.compute_jacobian(bundle);

      auto& delta = normal_equation.solve();
      update_parameters(*bundle.parameters,delta.data());
      
      pair = normal_equation.compute_error(bundle);
      std::cout << " erreur courante : " << pair.first << std::endl;
    } while(!stop());
    
    return *this;
  }
  
  bool stop() { return true; }  
};


struct Parameters{ double x,y;};

struct Reprojection
{
  bool operator()(const Parameters& p, double& r) const
  {
    r = p.x * p.x + 2.0 * p.y;
    return true;
  }

  void analytical(const Parameters& p, auto& mat) const
  {
    mat[0] = 2.0*p.x;
    mat[1] = 2.0;
  }
};

constexpr size_t size(Type<Parameters>) { return 2; }
constexpr size_t size(Type<double>) { return 1; }


void update_parameters(Parameters& parameters, const double delta[2])
{
  parameters.x += delta[0];
  parameters.y += delta[1];
}

int main()
{
  Parameters parameters {1,1};
  Solver<Reprojection>{}.add(Reprojection{},&parameters).solve(Type<double>{});
}
