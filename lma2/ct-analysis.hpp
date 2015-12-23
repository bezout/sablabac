#pragma once

namespace lma
{
  template<class> struct AnalyseExtractFromType;
  
  template<class Return, class F, class P, class R> struct AnalyseExtractFromType< Return (F::*) (const P&, R&) const >
  {
    typedef P Parameters;
    typedef R Residual;
  };

  template<class F> struct AnalyseFunctor : AnalyseExtractFromType<decltype(&F::operator())> {};
}
