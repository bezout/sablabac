#pragma once

#include <array>
#include <vector>

namespace lma
{
  template<class T> using AlignedVector = std::vector<T,Eigen::aligned_allocator<T>>;
  
  template<class F, int Dimension>
  struct Container
  {
    std::array<F,Dimension> data;
    int last = 0;
    void push_back(const F& f) { assert(last < Dimension); data[last] = f; ++last; }
    constexpr size_t size() const { return Dimension; }
    inline void resize(size_t) {}
  };
  
  template<class F>
  struct Container<F,-1>
  {
    AlignedVector<F> data;
    void push_back(const F& f) { data.push_back(f); }
    size_t size() const { return data.size(); }
    void resize(size_t s) { data.resize(s); }
  };
  
  template<class F, int Dimension> auto begin(const Container<F,Dimension>& container) { return container.data.begin(); }
  template<class F, int Dimension> auto end(const Container<F,Dimension>& container) { return container.data.end(); }
  
  template<class F, int Dimension> auto begin(Container<F,Dimension>& container) { return container.data.begin(); }
  template<class F, int Dimension> auto end(Container<F,Dimension>& container) { return container.data.end(); }


  //const &, &, f
  template<class X, class Y, int D>
  void loop(const Container<X,D>& x, Container<Y,D>& y, auto f)
  {
    y.resize(x.size());
    for(size_t i = 0 ; i < x.size() ; ++i)
      f(x.data[i],y.data[i]);
  }
  
  //const &, const &, f
  template<class X, class Y, int D>
  void loop(const Container<X,D>& x, const Container<Y,D>& y, auto f)
  {
    for(size_t i = 0 ; i < x.size() ; ++i)
      f(x.data[i],y.data[i]);
  }
  
  //const &, const &, &, f
  template<class X, class Y, class Z, int D>
  void loop(const Container<X,D>& x, const Container<Y,D>& y, Container<Z,D>& z, auto f)
  {
    z.resize(x.size());
    for(size_t i = 0 ; i < x.size() ; ++i)
      f(x.data[i],y.data[i],z.data[i]);
  }
  
}
