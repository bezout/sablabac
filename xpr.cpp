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
  vect::test_vect();
}
