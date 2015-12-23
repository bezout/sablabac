#pragma once

namespace lma
{
  template<class P, int ddl> struct DDL
  {
    static void apply_increment(P& p, auto delta[ddl])
    {
    }
  };

  template<class P> struct DDL<P,-1>
  {
    int ddl = 0;
    
    static void apply_increment(P& p, auto* delta)
    {
    }
  };

}
