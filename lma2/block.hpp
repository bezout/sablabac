#pragma once

#include "traits.hpp"
#include <Eigen/Core>


namespace lma
{
  template<class Float, int I, int J> struct CreateBlock
  {
    using Block = Eigen::Matrix<Float,I,J>;
  };
  
  template<class Float> struct CreateBlock<Float,1,1>
  {
    using Block = Float;
  };
  
  template<class Float, int J> struct CreateBlock<Float,0,J>
  {
    using Block = Eigen::Matrix<Float,Eigen::Dynamic,J>;
  };
  
  template<class Float, int I> struct CreateBlock<Float,I,0>
  {
    using Block = Eigen::Matrix<Float,I,Eigen::Dynamic>;
  };

  template<class Float> struct CreateBlock<Float,0,0>
  {
    using Block = Eigen::Matrix<Float,Eigen::Dynamic,Eigen::Dynamic>;
  };  
  
  template<class Float, int I, int J> 
  using Block = typename CreateBlock<Float,I,J>::Block;


  float squared_norm(const float& value /* enable if is floating point*/) { return value * value ; }

  double squared_norm(const double& value) { return value * value ; }

  template<class Float, int I, int J>
  Float squared_norm(const Eigen::Matrix<Float,I,J>& mat) { return mat.squaredNorm() ; }

  template<class Float, int I, int J>
  constexpr int cols(const Eigen::Matrix<Float,I,J>& mat)
  {
    return J;
  }
  
  template<class Float, int I, int J>
  constexpr int rows(const Eigen::Matrix<Float,I,J>& mat)
  {
    return I;
  }
  
  template<class Float, int I>
  int cols(const Eigen::Matrix<Float,I,Eigen::Dynamic>& mat)
  {
    return mat.cols();
  }
  
  template<class Float, int J>
  int rows(const Eigen::Matrix<Float,Eigen::Dynamic,J>& mat)
  {
    return mat.rows();
  }
  
  template<class Float, int I, int J>
  void disp_size(const Eigen::Matrix<Float,I,J>& mat, std::string name)
  {
    std::cout << " Size of " << name << " : " << rows(mat) << ", " << cols(mat) << std::endl;
  }
  
  void disp_size(const float&, std::string name)
  {
    std::cout << " Size of " << name << " : 1 " << std::endl;
  }
  
  void disp_size(const double&, std::string name)
  {
    std::cout << " Size of " << name << " : 1 " << std::endl;
  }
  
  template<class Float, int I, int J>
  struct Name<Eigen::Matrix<Float,I,J>>
  {
    static std::string name()
    {
      return std::string("Eigen::Matrix<") + lma::name<Float>() + "," + (I==-1?"Dynamic":std::to_string(I)) + "," + (J==-1?"Dynamic":std::to_string(J)) + ">";
    }
  };

}


namespace lma
{
  constexpr auto pow(const auto& value, int i)
  {
    return (i==0) ? value : value * pow(value,i-1);
  }
}
