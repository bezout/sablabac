#include <iostream>


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


namespace tl
{
  struct Null {};

  struct False { enum {value = false}; };
  struct True { enum {value = true}; };

  template<class A, class B> struct Equal : False {};
  template<class A> struct Equal<A,A> : True {};


  template<class T, class Q> struct List
  {
    typedef T Head;
    typedef Q Queue;
  };

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

  void name(int) { std::cout << " int" << std::endl;}
  void name(char) { std::cout << " char" << std::endl;}
  void name(double) { std::cout << " double" << std::endl;}

  template<class A, class B> struct Pair{};

  template<class TL> struct Tuple
  {
    typedef typename TL::Head Type;
    typedef Tuple<typename TL::Queue> Tuple_;
    Type obj;
    Tuple_ tuple;
  };

  template<class TL, class T>
  T& at(Tuple<TL>& tuple, Pair<T,T>) { return tuple.obj; }

  template<class TL, class Type, class T>
  T& at(Tuple<TL>& tuple, Pair<Type,T>) { return at<T>(tuple.tuple); }
    
  template<class T, class TL> T& at(Tuple<TL>& tuple)
  {
    static_assert((Find<TL,T>::value),"Type doesn't exist");
    return at(tuple, Pair<typename Tuple<TL>::Type,T>());
  }

  template<> struct Tuple<Null>{};


  int test_tl()
  {
    std::cout << Equal<int,double>::value << std::endl;
    std::cout << Equal<double,double>::value << std::endl;
    
    typedef List<int,List<char,List<double,Null>>> TL;
    
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

int main()
{
  xpr::test_expr();
  tl::test_tl();
}
