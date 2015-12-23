#pragma once

#include "string.hpp"

#include <boost/thread/thread.hpp>

#if BOOST_VERSION >= 104700
  #include <boost/chrono.hpp>
#else
  #include <boost/date_time/gregorian/gregorian_types.hpp>
  #include <boost/date_time/posix_time/posix_time_types.hpp>
#endif

namespace lma
{
  inline double now(void)
  {
  #if BOOST_VERSION >= 104700
    return double(boost::chrono::duration_cast<boost::chrono::microseconds>(boost::chrono::steady_clock::now().time_since_epoch()).count()) * 1e-6;
  #else
    return double((boost::posix_time::microsec_clock::local_time() - boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1))).total_microseconds()) * 1e-6;
  #endif
  }

  struct Tic
  {
    double t;
    std::string name;
    Tic(std::string name_ =""):name(name_) { tic(); }

    void tic() { t = now(); }

    double toc() const { return now() - t; }

    std::ostream& disp(std::ostream& o = std::cout , const std::string& str="") const
    {
      double d = toc();
      o << color.white() << color.bold() << " [TIC] " << str << " " << name << " : \t\t" << d << " sec." << color.reset() << std::endl;
      return o;
    }
  };
}
