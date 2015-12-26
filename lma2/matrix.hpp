#pragma once

#include "block.hpp"
//#include "container.hpp"
//#include <Eigen/Cholesky>
//#include <Eigen/Dense>


namespace lma
{
  
  template<class Block, int Dimension> struct CreateMatrix
  {
    using Matrix = Eigen::MatrixXd;
  };
  
  template<class Block, int Dimension>
  using Matrix = typename CreateMatrix<Block,Dimension>::Matrix;
  
  template<class Block, int Dimension> struct CreateVector
  {
    using Vector = Eigen::VectorXd;
  };
  
  template<class Block, int Dimension>
  using Vector = typename CreateVector<Block,Dimension>::Vector;
  
  template<class Mat, class Float>
  Mat& damping(Mat& mat, const Float& lambda)
  {
    for(int i = 0 ; i < mat.cols() ; ++i)
      mat(i,i) += lambda;
    return mat;
  }
 
  
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
