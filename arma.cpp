
#include <blaze/Blaze.h>


#include <nt2/table.hpp>
#include <nt2/include/functions/size.hpp>
#include <nt2/include/functions/transpose.hpp>
#include <nt2/linalg/mtimes.hpp>


#include <stdlib.h>
#include <ctime>
#include <sys/time.h>

#include <eigen3/Eigen/Core>
#include <armadillo>
#include <vector>

#include <TooN/TooN.h>
static const size_t K = 9;
static const size_t L = 3;
static const size_t M = 1;

template<int K, int L> struct Arma
{
  typedef arma::mat::fixed<K,L> type;
};

template<int K> struct Arma<K,1>
{
  typedef arma::vec::fixed<K> type;
};

template<int K, int L> struct TOON
{
  typedef TooN::Matrix<K,L> type;
};

template<int K> struct TOON<K,1>
{
  typedef TooN::Vector<K> type;
};

template<int K, int L> struct BLAZE
{
  typedef blaze::StaticMatrix<double,K,L> type;
};

template<int K> struct BLAZE<K,1>
{
  typedef blaze::StaticVector<double,K> type;
};

typedef Arma<K,L>::type Arma93;
typedef Arma<K,M>::type Arma91;
typedef Arma<L,M>::type Arma33;

typedef Eigen::Matrix<double,K,L> Eigen93;
typedef Eigen::Matrix<double,K,M> Eigen91;
typedef Eigen::Matrix<double,L,M> Eigen33;

typedef TOON<K,L>::type TooN93;
typedef TOON<K,M>::type TooN91;
typedef TOON<L,M>::type TooN33;

typedef BLAZE<K,L>::type Blaze93;
typedef BLAZE<K,M>::type Blaze91;
typedef BLAZE<L,M>::type Blaze33;

typedef nt2::table<double, nt2::settings( nt2::of_size_<9,3>, nt2::C_index_) > NT293;
typedef nt2::table<double, nt2::settings( nt2::of_size_<9,1>, nt2::C_index_) > NT291;
typedef nt2::table<double, nt2::settings( nt2::of_size_<3,1>, nt2::C_index_) > NT233;
//typedef nt2::table<double,9,3> Nt293;

static inline double _now() {
    size_t hi, lo;
    __asm __volatile ("rdtsc" : "=a" (lo), "=d" (hi));
    return double((long long)hi << 32 | lo);
}

static inline double  now() {
    struct timeval tp;
    gettimeofday(&tp,NULL);
    return double(tp.tv_sec) + double(tp.tv_usec)*1e-6;
}

int main()
{
  size_t n = 1000000;
  double t1,t2,t3,t4,t5;
  
  {
    std::vector<Arma93> v39(n);
    std::vector<Arma91> r91(n);
    std::vector<Arma33> v33(n);
      
    t1 = now();
    for(size_t i = 0 ; i < n ; ++i)
      r91[i] = v39[i] * v33[i];
    t1 = now() - t1;

    std::cout << r91.back() << std::endl;
  }
  
  {
    std::vector<Eigen93,Eigen::aligned_allocator<Eigen93>> v39(n);
    std::vector<Eigen91,Eigen::aligned_allocator<Eigen91>> r91(n);
    std::vector<Eigen33,Eigen::aligned_allocator<Eigen33>> v33(n);
    
    
    t2 = now();
    for(size_t i = 0 ; i < n ; ++i)
      r91[i] = v39[i] * v33[i];
    t2 = now() - t2;
    
    std::cout << r91.back() << std::endl;
  }
  
  {
    std::vector<TooN93> v39(n);
    std::vector<TooN91> r91(n);
    std::vector<TooN33> v33(n);
    
    
    t3 = now();
    for(size_t i = 0 ; i < n ; ++i)
      r91[i] = v39[i] * v33[i];
    t3 = now() - t3;
    
    std::cout << r91.back() << std::endl;
  }


  {
    std::vector<Blaze93> v39(n);
    std::vector<Blaze91> r91(n);
    std::vector<Blaze33> v33(n);
      
    t4 = now();
    for(size_t i = 0 ; i < n ; ++i)
      r91[i] = v39[i] * v33[i];
    t4 = now() - t4;

    std::cout << r91.back() << std::endl;
  }
  
  {
    std::vector<NT293> v39(n);
    std::vector<NT291> r91(n);
    std::vector<NT233> v33(n);
      
    t5 = now();
    for(size_t i = 0 ; i < n ; ++i)
      r91[i] = nt2::multiplies(v39[i],v33[i]);
    t5 = now() - t5;

    std::cout << r91.back() << std::endl;
  }
  
  std::cout << " Arma  : "  << t1 << std::endl;
  std::cout << " Eigen : "  << t2 << std::endl;
  std::cout << " TooN  : "  << t3 << std::endl;
  std::cout << " Blaze : "  << t4 << std::endl;
  std::cout << " NT2   : "  << t5 << std::endl;
}
