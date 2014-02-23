#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>

template<class E1, class E2> struct Operator
{
  E1 left;
  E2 right;

  Operator(E1 e1_, E2 e2_):left(e1_),right(e2_){}
  
  };

template<class E1, class E2> struct Add : Operator<E1,E2>
{
  typedef Operator<E1,E2> parent;
  Add(E1 e1_, E2 e2_):parent(e1_,e2_){}
};

template<class A, class B> auto eval(Add<A,B> op)
  -> decltype (eval(op.left)+eval(op.right))
   { return eval(op.left)+eval(op.right); }
   
template<class E1, class E2> struct Minus : Operator<E1,E2>
{
  typedef Operator<E1,E2> parent;
  Minus(E1 e1_, E2 e2_):parent(e1_,e2_){}
};

template<class A, class B> auto eval(Minus<A,B> op)
  -> decltype (eval(op.left)-eval(op.right))
   { return eval(op.left)-eval(op.right); }

template<class E1, class E2> struct Mul : Operator<E1,E2>
{
  typedef Operator<E1,E2> parent;
  Mul(E1 e1_, E2 e2_):parent(e1_,e2_){}
};

template<class A, class B> auto eval(Mul<A,B> op)
  -> decltype (eval(op.left)*eval(op.right))
   { return eval(op.left)*eval(op.right); }
   

template<class T> struct Box
{
  T value;
  
  Box(T value_ = T()) : value(value_) {}

  template<class Expr> Box(Expr expr) : value(eval(expr))
  {
  }
};

template<class T> T eval(Box<T> box) { return box.value; }

template<class T> struct Name { static std::string name(){ return "";} };
template<class T> static std::string show() { return Name<T>::name(); }
template<class T> struct Name<T&> { static std::string name(){ return show<T>() + "&";} };

template<> struct Name<double> { static std::string name(){ return "double";} };

template<class A, class B> struct Name<Add<A,B>> { 
  static std::string name() { return std::string() + "Add<" + show<A>() + "," + show<B>() + ">"; } };

template<class A, class B> struct Name<Minus<A,B>> { 
  static std::string name() { return std::string() + "Minus<" + show<A>() + "," + show<B>() + ">"; } };
  
template<class A, class B> struct Name<Mul<A,B>> { 
  static std::string name() { return std::string() + "Mul<" + show<A>() + "," + show<B>() + ">"; } };

template<class T> struct Name<Box<T>> { static std::string name(){ return std::string() + "Box<" + show<T>() + ">";} };

template<class E1, class E2> Add<E1,E2> operator+(E1 e1, E2 e2)
{
  return Add<E1,E2>(e1,e2);
}

template<class E1, class E2> Minus<E1,E2> operator-(E1 e1, E2 e2)
{
  return Minus<E1,E2>(e1,e2);
}

template<class E1, class E2> Mul<E1,E2> operator*(E1 e1, E2 e2)
{
  return Mul<E1,E2>(e1,e2);
}

template<class T> std::ostream& operator<<(std::ostream& o, const Box<T>& mb)
{
  //std::copy(mb.data.begin(),mb.data.end(),std::ostream_iterator<double>(o," "));
  return o << mb.value;
}

template<int I, int J> struct Matrix {};

template<int I, int J> struct Name<Matrix<I,J>> { static std::string name(){ return std::string() + "Matrix<" + ">";} };

int main()
{
  Box<double> a(1.0),b(2.0),c(3.0),d(3.0),e(2.0);
  
  auto expr = a + b + c * (d - e);
  std::cout << " name : " << show<decltype(expr)>() << std::endl;
  std::cout << " value : " << eval(Box<double>(expr)) << std::endl;

  Matrix<2,3> m1;
  Box<Matrix<2,3>> box1(m1);
  
  
  //auto expr2 = box1 + box1;

  //std::cout << " expr2 : " << show<decltype(expr2)>() << std::endl;
  
  //Box<Matrix<2,3>> result(expr2);
  //std::cout << " value2 : " << eval(expr2) << std::endl;
}
