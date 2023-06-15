#ifndef __INCLUDE_TOOL_H_
#define __INCLUDE_TOOL_H_

#include "xjjcuti.h"

std::string tcomb(std::string comb) {
  std::string r(comb);
  r.insert(1, " #otimes ");
  r = Form("%sPIX %s", (atoi(comb.c_str())>50?"F":"B"), r.c_str());
  return r;
}

std::string tcent(std::string tag) {
  
  if(!xjjc::str_contains(tag, ".s.")) return "";
  tag = xjjc::str_erasestar(tag, "*.s.");
  auto v = xjjc::str_divide(tag, ".");
  if(v.size() != 2) return "";
  float cmax = 5*(20-atof(v[0].c_str())),
    cmin = 5*(20-atof(v[1].c_str()));
  return Form("Cent. %.0f-%.0f%s", cmin, cmax, "%");
}


#endif
