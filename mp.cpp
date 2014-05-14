#include <iostream>
#include <vector>
#include <iterator>

namespace xpr
{
  struct A
  {
    int value;
    A(int val):value(val){std::cout << " Default constructor " << value << std::endl;}
    
    int eval() const { return value; }
    template<class Expr>
    A(Expr expr):value(expr.eval()){}
  };

  template<class X, class Y> struct Add
  {
    const X& x;
    const Y& y;
    Add(const X& x_, const Y& y_):x(x_),y(y_){}
    auto eval() const -> decltype (x.eval() + y.eval()) { return x.eval() + y.eval(); }
  };

  template<class X, class Y> Add<X,Y> operator+(const X& a, const Y& b)
  {
    return Add<X,Y>(a,b);
  }

  int test_expr()
  {
    A b(1),c(10),d(100),e(1000);
    std::cout << "\n Evalutation " << std::endl;
    A a = b + c + d + e;
    std::cout << " result : " << (b + c + d + e).eval() << std::endl;
    std::cout << " result : " << a.value << std::endl;
  }
}


namespace ttt
{
  struct False { enum {value = false}; };
  struct True { enum {value = true}; };

  template<class A, class B> struct Equal : False {};
  template<class A> struct Equal<A,A> : True {};

  struct Null {};

  template<class T, class Q> struct List
  {
    typedef T Head;
    typedef Q Queue;
  };

  template<class A = Null, class B = Null, class C = Null, class D = Null, class E = Null, class F = Null> struct MakeTL : List<A,List<B,List<C,List<D,List<E,List<F,Null>>>>>> {};
  
  template<class TL> struct Size
  {
    enum { value = Size< typename TL::Queue >::value + 1 };
  };

  template<> struct Size<Null>
  {
    enum { value = 0 };
  };

  template<class TL, int I> struct At : At< typename TL::Queue , I-1 > {};

  template<class TL> struct At<TL,0>
  {
    typedef typename TL::Head type;
  };

  template<class TL, class T> struct Find
  {
    enum { value = Equal< typename TL::Head, T >::value || Find< typename TL::Queue, T >::value };
  };

  template<class T> struct Find<Null,T> : False {};

  template<class A, class B> struct Pair{};

  template<class TL> struct Tuple
  {
    typedef typename TL::Head Type;
    typedef Tuple<typename TL::Queue> Tuple_;
    Type obj;
    Tuple_ tuple;
  };

  template<> struct Tuple<Null> {};
  
  template<class TL, class T>
  T& at(Tuple<TL>& tuple, Pair<T,T>) { return tuple.obj; }

  template<class TL, class Type, class T>
  T& at(Tuple<TL>& tuple, Pair<Type,T>) { return at<T>(tuple.tuple); }
    
  template<class T, class TL> T& at(Tuple<TL>& tuple)
  {
    static_assert((Find<TL,T>::value),"Type doesn't exist");
    return at(tuple, Pair<typename Tuple<TL>::Type,T>());
  }
  
  void name(int) { std::cout << " int" << std::endl;}
  void name(char) { std::cout << " char" << std::endl;}
  void name(double) { std::cout << " double" << std::endl;}
  
  int test_tl()
  {
    std::cout << Equal<int,double>::value << std::endl;
    std::cout << Equal<double,double>::value << std::endl;
    
    typedef MakeTL<int,char,double> TL;
    
    std::cout << " Nb type : " << Size<TL>::value << std::endl;
    name(At<TL,0>::type());
    name(At<TL,1>::type());
    name(At<TL,2>::type());
    std::cout << " find int " << Find<TL,int>::value << std::endl;
    std::cout << " find float " << Find<TL,float>::value << std::endl;
    
    Tuple<TL> tuple;
  //   tuple.get<float>();
    at<int>(tuple) = 2;
    at<double>(tuple) = 0.123456;
    std::cout << " int " << at<int>(tuple) << std::endl;
    std::cout << " double " << at<double>(tuple) << std::endl;
  }
}


namespace vect
{
  typedef std::vector<int> Vector;

  template<class A, class B> struct Add
  {
    const A& a;
    const B& b;
    Add(const A& a_, const B& b_):a(a_),b(b_){}
    size_t size() const { return a.size(); }
  };

  int eval(const Vector& a, size_t i)
  {
    return a[i];
  }

  template<class A, class B> int eval(const Add<A,B>& expr, size_t i)
  {
    return eval(expr.a,i) + eval(expr.b,i);
  }

  template<class A, class B> Add<A,B> operator+(const A& a, const B& b)
  {
    return Add<A,B>(a,b);
  }

  template<class Expr>
  void assign(Vector& a, const Expr& expr)
  {
    a.resize(expr.size());
    for(size_t i = 0 ; i < expr.size() ; ++i)
      a[i] = eval(expr,i);
  }

  std::ostream& operator<<(std::ostream& o, const Vector& a)
  {
    std::copy(a.begin(),a.end(),std::ostream_iterator<Vector::value_type>(o," "));
    return o;
  }

  void test_vect()
  {
    size_t n = 10;
    Vector a(n,0),b(n,1),c(n,10),d(n,100),e(n,1000);
    assign(a,b + c + d + e);
    std::cout << a << std::endl;
  }
}

int main()
{
  xpr::test_expr();
  ttt::test_tl();
  vect::test_vect();
}
