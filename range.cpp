#include <iostream>
#include <vector>
#include <algorithm>

struct RangeIndex
{
  int begin_index;
  int end_index;
  bool empty() const { return begin_index == end_index; }
  size_t size() const { return end_index-begin_index; }
  
  struct RangeIndexIterator
  {
    int index;
    bool operator!=(const RangeIndexIterator& rg_it) const { return rg_it.index != index; }
    void operator++() { index++; }
    auto& operator*() { return index; }
  };
    
  RangeIndexIterator begin() { return RangeIndexIterator{begin_index}; }
  RangeIndexIterator end() { return RangeIndexIterator{end_index}; }
};


template<typename T> struct Range
{
  T& container;
  RangeIndex indexes;

  auto begin() { return std::next(container.begin(),indexes.begin_index); }
  auto end() { return std::next(container.begin(),indexes.end_index); }
  
  bool empty() const { return indexes.empty(); }
  size_t size() const { return indexes.size(); }
};



template<typename T>
Range<T> range(T& container, int begin, int end)
{
  return Range<T>{container,begin,end};
}

template<typename T>
Range<T&&> range(T&& container, int begin, int end)
{
  return Range<T&&>{container,begin,end};
}

template<typename T>
Range<T> range(T& container)
{
  return Range<T>{container,0,(int)container.size()};
}


template<typename T> std::ostream& operator<<(std::ostream& o, Range<T> rg)
{
  if (rg.empty()) return o << "{}";
  o << " [" << rg.indexes.begin_index << "," << rg.indexes.end_index << "]:{";
  for(auto& x : rg)
    o << x << ",";
  o << "\b}";
  return o;
}

template<typename T> void reverse(Range<T> rg)
{
  std::reverse(rg.begin(),rg.end());
}

template<typename T> void sort(Range<T> rg)
{
  std::sort(rg.begin(),rg.end());
}

int nb_copy = 0;

struct MyVector
{
  
  std::vector<int> v;
  MyVector()
  {
    nb_copy++;
  }

  MyVector(const MyVector& mv)
  {
    v = mv.v;
    nb_copy++;
  }
  MyVector& operator=(const MyVector& mv)
  {
    v = mv.v;
    return *this;
  }
  ~MyVector()
  {
    std::cout << " total copy = " << nb_copy << std::endl;
  }
  auto begin() { return v.begin(); }
  auto end() { return v.end(); }
  size_t size() const { return v.size(); }  
};


int main()
{
  MyVector v1;
  v1.v =   {0,1,2,3,4,5,6,7,8,9};
  
  auto r1 = range(v1,2,7);
  
  std::cout << range(v1) << std::endl;    
  std::cout << r1 << std::endl;


  reverse(r1);
  std::cout << " Reverse :\n" << r1 << std::endl;
  std::cout << range(v1) << std::endl;
  
  sort(r1);
  std::cout << " Sorted :\n" << r1 << std::endl;    

  std::cout << range(v1) << std::endl;
  for(auto& x : r1)
    x += 10;  
  std::cout << " +1:\n" << r1 << std::endl;    
    std::cout << range(v1) << std::endl;
  std::cout << " range(range ...):\n";  
  std::cout << range(range(r1,0,5),1,4) << std::endl;    
  
  std::cout << " rrrr indexes: ";
  for(auto ind : range(range(r1,0,5),1,4).indexes)
  {
    std::cout << ind << ",";
  }
  std::cout << std::endl;
}


