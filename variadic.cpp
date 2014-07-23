#include <iostream>

template<class ... T> struct Tuple {};

template<class _Elt, class ... Rest> struct Tuple<_Elt, Rest ...> : Tuple<Rest ...>
{
  typedef _Elt Elt;
  Elt elt;
  
  typedef Tuple<Rest ...> Tail;
  Tail& tail() { return static_cast<Tail&>(*this); }
};

template<size_t I> struct Get
{
  template<class ... T> 
  static auto& get(Tuple<T...>& tuple)// -> decltype(Get<I-1>::get(tuple.tail()))
  {
    return Get<I-1>::get(tuple.tail());
  }
};

template<> struct Get<0>
{
  template<class ... T> 
  static auto& get(Tuple<T...>& tuple)// -> decltype(tuple.elt)
  {
    return tuple.elt;
  }
};

template<size_t I, class ... T> auto get(Tuple<T...>& tuple)
{
  return Get<I>::get(tuple);
}

template<size_t I, class Tuple> struct TypeAccessor 
: TypeAccessor<I-1,typename Tuple::Tail>
{
};

template<class Elt, class ... Rest> struct TypeAccessor<0,Tuple<Elt,Rest ...>>
{
  typedef Elt Element;
  typedef Tuple<Elt,Rest...> Tail;
};

template<size_t I, class ... Types> auto& get2(Tuple<Types...>& tuple)
{
  return static_cast<typename TypeAccessor<I,Tuple<Types...>>::Tail&>(tuple).elt;
}

int main()
{
  typedef Tuple<int,double> Tuple1;
  
  Tuple<int,double> t;
  std::cout << (t.elt = 1.1) << std::endl;
  std::cout << (t.tail().elt = 1.1) << std::endl;
  std::cout << " =========== " << std::endl;
  std::cout << get<0>(t) << std::endl;
  std::cout << get<1>(t) << std::endl;
  
  std::cout << " =========== " << std::endl;
  std::cout << TypeAccessor<0,Tuple1>::Element(1.1) << std::endl;
  std::cout << TypeAccessor<1,Tuple1>::Element(1.1) << std::endl;
  std::cout << " =========== " << std::endl;
  std::cout << (get2<0>(t)=2.2) << std::endl;
  std::cout << (get2<1>(t)=2.2) << std::endl;
}
