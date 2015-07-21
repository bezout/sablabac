#include <vector>
#include <iostream>

typedef std::vector<int> NUplet;
typedef std::vector<NUplet> Lists;

/*
void n_for(int i, const Lists& lists, Lists& result, NUplet& n_uplet)
{
  if (i!=lists.size())
  {
    for(auto & x : lists[i])
    {
      n_uplet[i] = x;
      n_for(i+1,lists,result,n_uplet);
    }
  }
  else
    result.push_back(n_uplet);
}

Lists combinaison(const Lists& lists)
{
  Lists result;
  NUplet n_uplet(lists.size(),0);
  n_for(0,lists,result,n_uplet);
  return result;
}
*/


template<class Lists>
class NFor
{
  public:
    NFor(const Lists& lists_):lists(lists_),n_uplet(lists.size(),0){iterate(0);}
    const Lists& solution() { return result; }

  private:
    void iterate(int i)
    {
      if (i!=lists.size())
      {
        for(auto & x : lists[i])
        {
          n_uplet[i] = x;
          iterate(i+1);
        }
      }
      else
        result.push_back(n_uplet);
    }
    const Lists& lists;
    Lists result;
    typename Lists::value_type n_uplet;
};

Lists combinaison(const Lists& lists)
{
  return NFor<Lists>(lists).solution();
}

int main()
{
  Lists w {{1,2},{3,4,5},{6,7,8,9}};
  Lists result = combinaison(w);

  std::cout << std::endl << std::endl;
  for(auto& x : result)
  {
    for(auto& y : x)
      std::cout << y << ",";
    std::cout << std::endl;
  }
  std::cout << std::endl;

}


