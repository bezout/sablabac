#pragma once

#include <array>
#include <vector>
#include "block.hpp"
#include "container.hpp"
//#include <Eigen/Cholesky>


namespace lma
{
  
  template<class F, int Dimension>
  struct Matrix
  {
    std::array<F,Dimension> data;
    int last = 0;
    void push_back(const F& f) { assert(last < Dimension); data[last] = f; ++last; }
    constexpr size_t size() const { return Dimension; }
    inline void resize(size_t) {}
    const F& operator[](size_t i) const { assert( i < size() ); return data[i]; }
    F& operator[](size_t i) { assert( i < size() ); return data[i]; }
  };
  
  template<class F>
  struct Matrix<F,-1>
  {
    AlignedVector<F> data;
    void push_back(const F& f) { data.push_back(f); }
    size_t size() const { return data.size(); }
    void resize(size_t s) { data.resize(s); }
    const F& operator[](size_t i) const { assert( i < size() ); return data[i]; }
    F& operator[](size_t i) { assert( i < size() ); return data[i]; }
  };
  
  template<class F, int Dimension> auto begin(const Matrix<F,Dimension>& container) { return container.data.begin(); }
  template<class F, int Dimension> auto end(const Matrix<F,Dimension>& container) { return container.data.end(); }
  
  template<class F, int Dimension> auto begin(Matrix<F,Dimension>& container) { return container.data.begin(); }
  template<class F, int Dimension> auto end(Matrix<F,Dimension>& container) { return container.data.end(); }
  
  
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
