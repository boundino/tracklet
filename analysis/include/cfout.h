#ifndef _XJJC_CFOUT_H_
#define _XJJC_CFOUT_H_

/* cfout.h */

#include <iostream>
#include <fstream>

namespace xjjc
{
  class cfout
  {
  public:
    cfout(std::ostream& out1, std::ostream& out2) : out1_(out1), out2_(out2) {}
    ~cfout(){};
    std::ostream& out1_;
    std::ostream& out2_;
  };
}

template <typename T> xjjc::cfout& operator<<(xjjc::cfout& cf, const T& t)
{
  cf.out1_<<t;
  cf.out2_<<t;
  return cf;
}

xjjc::cfout& operator<<(xjjc::cfout& cf, std::ostream&(*f)(std::ostream&))
{
  cf.out1_<<f;
  cf.out2_<<f;
  return cf;
}

#endif
