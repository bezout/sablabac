#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/find.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/transform.hpp>
#include <vector>

int main()
{
    typedef boost::mpl::vector<int,double,char,float,unsigned char,size_t> list;
    typedef boost::mpl::transform<list,std::vector<boost::mpl::_1>>::type list2;
    typedef boost::fusion::result_of::as_vector<list2>::type Vector;
    
    Vector vector;
    boost::fusion::at_c<0>(vector).size();
    boost::fusion::deref(boost::fusion::find<std::vector<int>>(vector)).empty();
}
