#include <iostream>
#include <eigen3/Eigen/Dense>

using namespace std;

typedef Eigen::Matrix<double,6,6> M66;
typedef Eigen::Matrix<double,6,1> M61;
	
template<class M> struct Container1d
{
	std::vector<M,Eigen::aligned_allocator<M> > v;
	Container1d(size_t n) : v(n,M::Zero()){}
	M& operator()(int i) { return v.at(i); }
	const M& operator()(int i) const { return v.at(i); }
	int size() const { return v.size() ; }
	int disp() 
	{
		return v.size();
	}
};

template<class M> struct Container2d
{
	std::vector<std::vector<M,Eigen::aligned_allocator<M> > > v;
	Container2d(size_t n, size_t m) 
		: v(n,std::vector<M,Eigen::aligned_allocator<M> >(m,M::Zero())){}
	//~ M& operator(int i) { return v.at(i); }
	
	int size(int i) const { return v.at(i).size() ; }
	const M& operator()(int i, int j) const { return v.at(i).at(j); }
};

template<class A, class B, class C>
void prod(A& a, const B& b, const C& c)
{
for(int i = 0 ; i < a.size() ; ++i)
	for(int j = 0 ; j < b.size(i) ; ++j)
	{
		a(i) += b(i,j) * c(j);
	}
}

size_t n = 10000;
size_t m = 2000;
	
void f2()
{
	std::vector<M66,Eigen::aligned_allocator<M66> > a(n*m,M66::Zero());
	std::vector<M61,Eigen::aligned_allocator<M61> > b(m,M61::Zero());
	std::vector<M61,Eigen::aligned_allocator<M61> > c(m,M61::Zero());
	
	for(size_t i = 0 ; i < a.size() ; ++i)
		c.at(i%1000) += a.at(i) * b.at(i%1000);
		
	std::cout << c.size() << std::endl;
}

void f1()
{
	size_t s1 = n;
	size_t s2 = m;
	Container2d<M66> container1(s1,s2);
	Container1d<M61> container2(s2);
	Container1d<M61> container3(s1);
	
	prod(container3,container1,container2);
	container3.disp();
}

int main()
{
	//~ f1();
	
	f2();
}
