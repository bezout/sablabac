#pragma once

#include "traits.hpp"
#include <Eigen/Core>
//#include <Eigen/Cholesky>

namespace lma
{
  template<class Float, size_t I, size_t J> struct CreateMatrix
  {
    using Mat = Eigen::Matrix<Float,I,J>;
  };
  
  template<class Float> struct CreateMatrix<Float,1,1>
  {
    using Mat = Float;
  };
  
  template<class Float, size_t J> struct CreateMatrix<Float,0,J>
  {
    using Mat = Eigen::Matrix<Float,Eigen::Dynamic,J>;
  };
  
  template<class Float, size_t I> struct CreateMatrix<Float,I,0>
  {
    using Mat = Eigen::Matrix<Float,I,Eigen::Dynamic>;
  };

  template<class Float> struct CreateMatrix<Float,0,0>
  {
    using Mat = Eigen::Matrix<Float,Eigen::Dynamic,Eigen::Dynamic>;
  };  
  
  template<class Float, size_t I, size_t J> 
  using Matrix = typename CreateMatrix<Float,I,J>::Mat;


  float squared_norm(const float& value /* enable if is floating point*/) { return value * value ; }

  double squared_norm(const double& value) { return value * value ; }


  template<class Float, int I, int J>
  constexpr size_t cols(const Eigen::Matrix<Float,I,J>& mat)
  {
    return J;
  }
  
  template<class Float, int I, int J>
  constexpr size_t rows(const Eigen::Matrix<Float,I,J>& mat)
  {
    return I;
  }
  
  template<class Float, int I>
  size_t cols(const Eigen::Matrix<Float,I,Eigen::Dynamic>& mat)
  {
    return mat.cols();
  }
  
  template<class Float, int J>
  size_t rows(const Eigen::Matrix<Float,Eigen::Dynamic,J>& mat)
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
  
  
  
  template<class Matrix, class Vector>
  constexpr Vector llt(Matrix u, Vector x, int size)
  {
    for(int i = 0 ; i < size ; ++i)
    {
      for(int k = 0 ; k < i ; ++k)
        u(i,i) -= u(k,i) * u(k,i);
      
      assert(u(i,i)>0);
      if (u(i,i)<=0) throw ZeroOrInfiniteError("LLT");
      u(i,i) = std::sqrt(u(i,i));
      
      for(int j = i + 1; j < size ; ++j)
      {
        for(int k = 0 ; k < i ; ++k)
          u(i,j) -= u(k,i) * u(k,j);
        u(i,j) /= u(i,i);
      }
    }

    for(int j = 0 ; j < size; ++j)
    {
      for(int i = 0 ; i < j ; ++i)
        x[j] -= u(i,j) * x[i];
      x[j] /= u(j,j);
    }
    
    for(int j = size - 1 ; j >=0  ; --j)
    {
      for(int i = j+1 ; i < size ; ++i)
        x[j] -= u(j,i) * x[i];
      x[j] /= u(j,j);
    }
    return x;
  }
  
  
}


namespace lma
{
  template<class T>
  constexpr T pow(const T& value, size_t I)
  {
    if (I==1) return value;
    if (I==2) return value * value;
    if (I==3) return value * value * value;
    return 0;
  }
}
