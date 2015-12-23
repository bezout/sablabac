#pragma once


namespace lma
{

  template<class Float> struct LM
  {
    int max_iteration = 10;
    Float lambda = Float(0.001);
    Float eps = Float(0.99999);
    
    std::pair<Float,int> error1,error2;
    
    Float cost1() const { return error1.first; }
    Float cost2() const { return error2.first; }
    
    Float rms1() const { return std::sqrt(cost1() / Float(error1.second)); }
    Float rms2() const { return std::sqrt(cost2() / Float(error2.second)); }
    
    size_t nb_iteration = 0;
    
    bool stop() const { return (nb_iteration == max_iteration) || (is_better() && (rms2() > eps * rms1())); }
    bool is_better() const { return cost2() < cost1(); }

    void update(std::pair<Float,int> current_error, const auto& eq)
    {
      if (is_better())// was better
        error1 = error2;

      error2 = current_error;

      lambda = is_better() ? std::max(lambda/Float(10.0),Float(1e-10)) : std::min(lambda*Float(10.0),Float(1e30));
      nb_iteration ++;
    }
  };
  
  template<class Float> struct LMN : LM<Float> // variante de Nielsen
  {
    LMN(auto ... inits):LM<Float>{inits...} {}
    using LM<Float>::is_better;
    using LM<Float>::error1;
    using LM<Float>::error2;
    using LM<Float>::cost1;
    using LM<Float>::cost2;
    using LM<Float>::lambda;
    using LM<Float>::nb_iteration;

    Float _goodStepLowerScale = Float(1./3.); ///< lower bound for lambda decrease if a good LM step
    Float _goodStepUpperScale = Float(2./3.); ///< upper bound for lambda decrease if a good LM step
    Float v = Float(2.);

    void update(std::pair<Float,int> current_error, const auto& normal_equation)
    {
      if (is_better())// was better
        error1 = error2;

      error2 = current_error;

      if (is_better())
      {
        Float rho = Float(2.0)*(cost1()-cost2()) / (normal_equation.compute_scale(lambda) + Float(1e-3));
        lambda *= (std::max)(_goodStepLowerScale, (std::min)(Float(1) - lma::pow(Float(2.)*rho-Float(1.),3), _goodStepUpperScale));
        v = Float(2);
      }
      else
      {
        lambda*=v;
        v *= Float(2);
      }
      nb_iteration ++;
    }
  };


  template<class Float> struct Name<LM<Float>> { static std::string name() { return std::string("LM<") + lma::name<Float>() + ">"; } };
  template<class Float> struct Name<LMN<Float>> { static std::string name() { return std::string("LM-Nielsen<") + lma::name<Float>() + ">"; } };
    
}
