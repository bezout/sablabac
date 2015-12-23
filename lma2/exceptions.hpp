#pragma once

#include <stdexcept>

namespace lma
{
  struct NAN_ERROR : std::runtime_error
  {
    NAN_ERROR(const std::string str) : std::runtime_error( str) {}
  };

  struct INF_ERROR : std::runtime_error
  {
    INF_ERROR(const std::string str) : std::runtime_error( str) {}
  };

  struct ZeroOrInfiniteError : std::runtime_error
  {
    ZeroOrInfiniteError(const std::string str) : std::runtime_error( str) {}
  };
}

