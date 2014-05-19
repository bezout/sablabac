#include <iostream>
#include <vector>

namespace ttt
{
  struct Clement{}; void clement(Clement){}
  template<char> struct Type {};
  
  struct False { enum {value = false}; };
  struct True { enum {value = true}; };

  template<class A, class B> struct Equal : False {};
  template<class A> struct Equal<A,A> : True {};

  struct Null {};

  template<class A = Null, class B = Null, class C = Null> struct Tag {};
  
  template<class T, class Q> struct List
  {
    typedef T Head;
    typedef Q Queue;
  };

  template<class A = Null, class B = Null, class C = Null, class D = Null, class E = Null, class F = Null, class G = Null> 
  struct MakeTL : List<A,List<B,List<C,List<D,List<E,List<F,List<G,Null>>>>>>> {};
  
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

  template<class TL> struct Tuple
  {
    typedef typename TL::Head Type;
    typedef Tuple<typename TL::Queue> Tuple_;
    Type obj;
    Tuple_ tuple;
  };

  template<> struct Tuple<Null> {};
  template<> struct Tuple<List<Null,Null>> {};
  template<> struct Tuple<List<Null,List<Null,Null>>> {};
  template<> struct Tuple<List<Null,List<Null,List<Null,Null>>>> {};
  template<> struct Tuple<List<Null,List<Null,List<Null,List<Null,Null>>>>> {};
  
  template<class TL, class T> T& at(Tuple<TL>& tuple, Tag<T,T>) { return tuple.obj; }

  template<class TL, class Type, class T> T& at(Tuple<TL>& tuple, Tag<Type,T>) { return at<T>(tuple.tuple); }
    
  template<class T, class TL> T& at(Tuple<TL>& tuple)
  {
    static_assert((Find<TL,T>::value),"Type doesn't exist");
    return at(tuple, Tag<typename Tuple<TL>::Type,T>());
  }
  
  template<size_t I, class TL> typename At<TL,I>::type& at(Tuple<TL>& tuple)
  {
    return at(tuple,Tag<typename Tuple<TL>::Type,typename At<TL,I>::type>());
  }
  
  struct _1 {};
  struct _2 {};
  
  template<class T> struct Name { static std::string name() { return "***"; } };
  template<class T> std::string name() { return Name<T>::name();}

  template<class F, class T> struct Apply {};
  
  template<template<class> class F, class _1, class T> struct Apply<F<_1>,T>
  {
    typedef typename F<T>::type type;
  };
  
  template<template<class, class> class F, class _1, class _2, class T> struct Apply<F<_1,_2>,T>
  {
    typedef typename F<T,_2>::type type;
  };

  template<class TL, class Op> struct Transform
  {
    typedef List< typename Apply<Op,typename TL::Head>::type, typename Transform< typename TL::Queue, Op>::type > type;
  };
  
  template<class T, class Op> struct Transform<List<T,Null>,Op> { typedef List<typename Apply<Op,T>::type,Null> type; };
  template<class T, class Op> struct Transform<List<T,List<Null,Null>>,Op> { typedef List<typename Apply<Op,T>::type,Null> type; };
  template<class T, class Op> struct Transform<List<T,List<Null,List<Null,Null>>>,Op> { typedef List<typename Apply<Op,T>::type,Null> type; };
  template<class T, class Op> struct Transform<List<T,List<Null,List<Null,List<Null,Null>>>>,Op> { typedef List<typename Apply<Op,T>::type,Null> type; };
  template<class T, class Op> struct Transform<List<T,List<Null,List<Null,List<Null,List<Null,Null>>>>>,Op> { typedef List<typename Apply<Op,T>::type,Null> type; };
  template<class T, class Op> struct Transform<List<T,List<Null,List<Null,List<Null,List<Null,List<Null,Null>>>>>>,Op> { typedef List<typename Apply<Op,T>::type,Null> type; };
  
    
  template<> struct Name<void> { static std::string name() { return "void"; } };
  template<> struct Name<int> { static std::string name() { return "int"; } };
  template<> struct Name<char> { static std::string name() { return "char"; } };
  template<> struct Name<double> { static std::string name() { return "double"; } };
  template<> struct Name<_1> { static std::string name() { return "_1"; } };
  template<char C> struct Name<Type<C>> { static std::string name() { return std::string() + "Type<" + C + ">"; } };
  template<class H, class Q> struct Name<List<H,Q>> { static std::string name() { return ttt::name<H>() + "," + ttt::name<Q>(); } };
  template<class H> struct Name<List<H,Null>> { static std::string name() { return ttt::name<H>() + "Null"; } };
  template<class Q> struct Name<List<Null,Q>> { static std::string name() { return std::string() + "Null" + ttt::name<Q>(); } };
  template<> struct Name<List<Null,Null>> { static std::string name() { return "<Null,Null>"; } };
  

  template<class ... T> struct Name<MakeTL<T ...>> { static std::string name() { 
    return std::string() 
    + "MakeTL<" 
    + ttt::name<typename MakeTL<T ...>::Head>() 
    + ","
    + ttt::name<typename MakeTL<T ...>::Queue>()
    + ">"; } };
  
  template<class F, class T> struct Name<Apply<F,T>> { static std::string name() { return "Apply<" + ttt::name<F>() + "," + ttt::name<T>() + ">"; } };
  
  template<class T> struct Name<std::vector<T>> { static std::string name() { return "std::vector<" + ttt::name<T>() + ">"; } };
  
  template<class T> struct MakeVector { typedef std::vector<T,std::allocator<T>> type; };
  
  
  
  template<class Type, class TL, class T = Type> void push_back(Tuple<TL>& tuple, const T& obj)
  {
    at<Type>(tuple).push_back(obj);
  }
  
  template<class TL, class T> size_t size(Tuple<TL>& tuple, Tag<T>)
  {
    return at_c(tuple,Tag<T>()).size();
  }

  int test_tl()
  {
    std::cout << Equal<int,double>::value << std::endl;
    std::cout << Equal<double,double>::value << std::endl;
    
    typedef MakeTL<Type<'A'>,Type<'B'>,Type<'C'>,int,char,double> TL;
    typedef Transform<TL,MakeVector<_1>>::type TL2;
    std::cout << " TL  >> " << name<TL>() << std::endl;
    std::cout << " TL2 >> " << name<TL2>() << std::endl;
    
    std::cout << "***-- " << name<At<TL2,1>::type>() << std::endl;
    
    std::cout << " apply : " << name<Apply<MakeVector<_1>,int>>() << std::endl;
    std::cout << " result : " << name<Apply<MakeVector<_1>,int>::type>() << std::endl;

    std::cout << " Nb type : " << Size<TL>::value << std::endl;
    std::cout << name<At<TL,0>::type>() << std::endl;
    std::cout << name<At<TL,1>::type>() << std::endl;
    std::cout << name<At<TL,2>::type>() << std::endl;
    std::cout << " find int " << Find<TL,int>::value << std::endl;
    std::cout << " find float " << Find<TL,float>::value << std::endl;
    
    Tuple<TL> tuple;
    
  //   tuple.get<float>();
    at<int>(tuple) = 2;
    at<double>(tuple) = 0.123456;
    Tuple<TL2> tuple2;
    at<0>(tuple2).size();
    push_back<MakeVector<Type<'A'>>::type>(tuple2,Type<'A'>());
    //std::cout << " Size :" << size(tuple2,Tag<Type<'A'>>()) << std::endl;
    //std::cout << " int " << at<int>(tuple) << std::endl;
    //std::cout << " double " << at<double>(tuple) << std::endl;
  }
}

int main()
{
  ttt::test_tl();
}
