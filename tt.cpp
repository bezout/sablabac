#include <boost/mpl/if.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/remove.hpp>
#include <boost/mpl/deref.hpp>

struct A{};
struct B{};
struct C{};

using namespace boost;

typedef mpl::vector< mpl::vector<mpl::int_<0>,mpl::int_<1>,mpl::int_<2>>, A,B,C> V;
//typedef mpl::vector<A,B,C> V;


  template<class L> struct FindOrder
  {
    typedef typename
    mpl::find_if<
                  L,
                  mpl::is_sequence<mpl::_1>
                >::type iterator;
    
    template<class List, class Order> struct Result { typedef List list; typedef Order order; };
    
    typedef typename
    mpl::if_<
              boost::is_same<
                              iterator,
                              mpl::end<L>
                            >,
              Result<L,mpl::vector<>>,
              Result<typename mpl::remove<L,typename mpl::deref<iterator>::type>::type, typename mpl::deref<iterator>::type>
            >::type type;
  };



void f(char){}
int main()
{
  
  f(FindOrder<V>::type::list());
  
  //f(order);
}
